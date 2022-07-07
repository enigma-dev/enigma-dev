#include "zlib_util.h"
#include "action.h"

#include "libpng-util/libpng-util.h"

#include <utility>
#include <memory>
#include <functional>
#include <set>

#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

#include <zlib.h>

using namespace buffers;
using namespace buffers::resources;
using namespace std;

using TypeCase = TreeNode::TypeCase;
using IdMap = unordered_map<int, std::unique_ptr<google::protobuf::Message> >;
using TypeMap = unordered_map<TypeCase, IdMap>;
using egm::errStream;

static const std::string gmk_data = "gmk_data";

static vector<std::string> tempFilesCreated;
static bool atexit_tempdata_cleanup_registered = false;
static void atexit_tempdata_cleanup() {
  for (const std::filesystem::path &tempFile : tempFilesCreated) {
    if (tempFile.parent_path().u8string().find(gmk_data) != std::string::npos)
      DeleteFolder(tempFile.parent_path());
    else if (std::filesystem::is_regular_file(tempFile)) DeleteFile(tempFile);
  }
}

static void writeTempDataFile(const std::filesystem::path& write_fname, char *bytes, size_t length) {
  CreateDirectoryRecursive(write_fname);
  std::fstream fs(write_fname, std::fstream::out | std::fstream::binary);
  if (!fs.is_open()) return;
  fs.write(bytes, length);
  fs.close();
}

static void writeTempDataFile(const std::filesystem::path& write_fname, std::unique_ptr<char[]> bytes, size_t length) {
  writeTempDataFile(write_fname, bytes.get(), length);
}

static void writeTempBMPFile(const std::filesystem::path& write_fname, std::unique_ptr<char[]> bytes, size_t length, bool transparent=false) {
  static const unsigned MINHEADER = 54; //minimum BMP header size
  auto bmp = reinterpret_cast<const unsigned char*>(bytes.get()); // all of the following logic expects unsigned

  if (length < MINHEADER) {
    errStream << "Image from the GMK file had a length '" << length << "' smaller than the minimum header "
        << "size '" << MINHEADER << "' for the BMP format" << std::endl;
    return;
  }
  if (bmp[0] != 'B' || bmp[1] != 'M') {
    errStream << "Image from the GMK file did not have the correct BMP signature '"
        << bmp[0] << bmp[1] << "'" << std::endl;
    return;
  }
  unsigned pixeloffset = bmp[10] + 256 * bmp[11]; //where the pixel data starts
  //read width and height from BMP header
  size_t w = bmp[18] + bmp[19] * 256;
  size_t h = bmp[22] + bmp[23] * 256;
  //read number of channels from BMP header
  if (bmp[28] != 24 && bmp[28] != 32) {
    errStream << "Image from the GMK file was " << bmp[28] << " bit while this reader "
        << "only supports 24 and 32 bit BMP images" << std::endl;
    return;
  }
  unsigned numChannels = bmp[28] / 8;

  //The amount of scanline bytes is width of image times channels, with extra bytes added if needed
  //to make it a multiple of 4 bytes.
  unsigned scanlineBytes = w * numChannels;
  if (scanlineBytes % 4 != 0) scanlineBytes = (scanlineBytes / 4) * 4 + 4;

  unsigned dataSize = scanlineBytes * h;
  if (length < dataSize + pixeloffset) {
    errStream << "Image from the GMK file had a length '" << length << "' smaller than the estimated "
        << "size '" << (dataSize + pixeloffset) << "' based on the BMP header dimensions" << std::endl;
    return;
  }

  std::vector<unsigned char> rgba(w * h * 4);

  // get the bottom left pixel for transparency
  // NOTE: bmp is obviously upside down, so it's just the first pixel
  unsigned char t_pixel_r = bmp[pixeloffset+0],
                t_pixel_g = bmp[pixeloffset+1],
                t_pixel_b = bmp[pixeloffset+2];

  /*
  There are 3 differences between BMP and the raw image buffer for libpng:
  -it's upside down
  -it's in BGR instead of RGB format (or BRGA instead of RGBA)
  -each scanline has padding bytes to make it a multiple of 4 if needed
  The 2D for loop below does all these 3 conversions at once.
  */
  for (unsigned y = 0; y < h; y++) {
    for (unsigned x = 0; x < w; x++) {
      //pixel start byte position in the BMP
      unsigned bmpos = pixeloffset + (h - y - 1) * scanlineBytes + numChannels * x;
      //pixel start byte position in the new raw image
      unsigned newpos = 4 * y * w + 4 * x;
      if (numChannels == 3) {
        rgba[newpos + 0] = bmp[bmpos + 2]; //R<-B
        rgba[newpos + 1] = bmp[bmpos + 1]; //G<-G
        rgba[newpos + 2] = bmp[bmpos + 0]; //B<-R
        rgba[newpos + 3] = 255;            //A<-A
      } else {
        rgba[newpos + 0] = bmp[bmpos + 3]; //R<-A
        rgba[newpos + 1] = bmp[bmpos + 2]; //G<-B
        rgba[newpos + 2] = bmp[bmpos + 1]; //B<-G
        rgba[newpos + 3] = bmp[bmpos + 0]; //A<-R
      }

      if (transparent &&
        bmp[bmpos + 0] == t_pixel_r &&
        bmp[bmpos + 1] == t_pixel_g &&
        bmp[bmpos + 2] == t_pixel_b) {
        rgba[newpos + 3] = 0; //A<-A
      }
    }
  }

  CreateDirectoryRecursive(write_fname);
  libpng_encode32_file(rgba.data(), w, h, write_fname.u8string().c_str());
}

static void writeTempBGRAFile(const std::filesystem::path& write_fname, std::unique_ptr<char[]> bytes, size_t width, size_t height) {
  auto bgra = reinterpret_cast<unsigned char*>(bytes.get()); // all of the following logic expects unsigned

  for (unsigned y = 0; y < height; y++) {
    for (unsigned x = 0; x < width; x++) {
      unsigned pos = width * 4 * y + 4 * x;
      unsigned char temp = bgra[pos + 2];
      bgra[pos + 2] = bgra[pos + 0];
      bgra[pos + 0] = temp;
    }
  }

  CreateDirectoryRecursive(write_fname);
  libpng_encode32_file(bgra, width, height, write_fname.u8string().c_str());
}

Decoder::Decoder(std::istream &in): in(in), decodeTable(nullptr) {
  if (atexit_tempdata_cleanup_registered) return;
  const int ret = std::atexit(atexit_tempdata_cleanup);
  if (ret != 0) {
    errStream << "Failed to register cleanup handler for process exit, temporary files will not be removed" << std::endl;
  } else {
    atexit_tempdata_cleanup_registered = true;
  }
}

template<class Function, class... Args>
void Decoder::threadTempFileWrite(Function&& f, const std::filesystem::path& write_fname, Args&&... args) {
  std::future<void> tempFileFuture = std::async(std::launch::async, f, std::move(args)...);
  tempFileFuturesCreated.emplace_back(std::make_pair(std::move(tempFileFuture), write_fname));
}

void Decoder::processTempFileFutures() {
  for (auto &tempFilePair : tempFileFuturesCreated) {
    tempFilePair.second.append(tempFilePair.second.u8string());
    tempFilesCreated.push_back(tempFilePair.second.u8string());
  }
}

void Decoder::beginInflate() {
  size_t limit = read4();
  zlibStart = in.tellg();
  zlibBuffer = decompress(limit, 0);
  zlibPos = 0;
}

void Decoder::endInflate() {
  zlibBuffer.reset();
  zlibStart = 0;
  zlibPos = 0;
}

void Decoder::skip(size_t count) {
  if (zlibBuffer)
    zlibPos += count;
  else
    in.seekg(count, std::ios_base::cur);
}

int Decoder::read() {
  int byte, pos = in.tellg();
  if (zlibBuffer) {
    pos = zlibStart + zlibPos;
    byte = static_cast<unsigned char>(zlibBuffer[zlibPos++]);
  } else {
    byte = in.get();
  }
  if (decodeTable) {
    return (decodeTable[byte] - pos) & 0xFF;
  }
  return byte;
}

// NOTE: for read[2-4] the order of initialization is
// guaranteed, but order of evaluation is not

int Decoder::read2() {
  int one = read(), two = read();
  return (one | (two << 8));
}

int Decoder::read3() {
  int one = read(), two = read(), three = read();
  return (one | (two << 8) | (three << 16));
}

int Decoder::read4() {
  int one = read(), two = read(), three = read(), four = read();
  return (one | (two << 8) | (three << 16) | (four << 24));
}

bool Decoder::readBool() {
  return read4();
}

void Decoder::read(char* bytes, size_t length, size_t off) {
  int pos = in.tellg();
  if (zlibBuffer) {
    pos = zlibStart + zlibPos;
    memcpy(bytes, zlibBuffer.get() + zlibPos, length);
    zlibPos += length;
  } else {
    in.read(bytes, length);
  }
  if (decodeTable) {
    for (size_t i = 0; i < length; i++) {
      int t = bytes[off + i] & 0xFF;
      int x = (decodeTable[t] - pos - i) & 0xFF;
      bytes[off + i] = (char) x;
    }
  }
}

std::unique_ptr<char[]> Decoder::read(size_t length, size_t off) {
  auto bytes = make_unique<char[]>(length);
  read(bytes.get(), length, off);
  return bytes;
}

std::string Decoder::readStr() {
  size_t length = read4();
  std::unique_ptr<char[]> bytes = read(length);
  return std::string(bytes.get(), length);
}

double Decoder::readD() {
  union { char bytes[8]; double dbl; } data;
  read(data.bytes, 8);
  return data.dbl;
}

std::unique_ptr<char[]> Decoder::decompress(size_t &length, size_t off) {
  std::unique_ptr<char[]> src = read(length, off);

  z_stream zs = {}; // zero-initialize
  if (inflateInit(&zs) != Z_OK) {
    errStream << "Failed to initialize zlib inflate" << std::endl;
    return nullptr;
  }

  zs.next_in = (Bytef*)src.get();
  zs.avail_in = (uInt)length;

  int ret;
  char outbuffer[32768];
  std::string outstring;

  // get the decompressed bytes blockwise using repeated calls to inflate
  do {
    zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
    zs.avail_out = sizeof(outbuffer);

    ret = inflate(&zs, 0);

    if (outstring.size() < zs.total_out) {
        outstring.append(outbuffer, zs.total_out - outstring.size());
    }
  } while (ret == Z_OK);

  inflateEnd(&zs);

  if (ret != Z_STREAM_END) { // an error occurred that was not EOF
    errStream << "There was an error with code '" << ret << "' while using zlib to decompress data" << std::endl;
    return nullptr;
  }

  length = outstring.size();
  auto bytes = make_unique<char[]>(length);
  memcpy(bytes.get(), outstring.c_str(), length);

  return bytes;
}

void Decoder::readData(const std::filesystem::path& write_fname, bool compressed) {
  size_t length = read4();
  std::unique_ptr<char[]> bytes = compressed ? decompress(length) : read(length);
  using WriteTempDataFileManaged = void(*)(const std::filesystem::path& write_fname, std::unique_ptr<char[]>, size_t);
  if (bytes) threadTempFileWrite((WriteTempDataFileManaged)writeTempDataFile, write_fname, write_fname, std::move(bytes), length);
}

void Decoder::readCompressedData(const std::filesystem::path& write_fname) {
  readData(write_fname, true);
}

void Decoder::readUncompressedData(const std::filesystem::path& write_fname) {
  readData(write_fname, false);
}

void Decoder::readZlibImage(const std::filesystem::path& write_fname, bool transparent) {
  size_t length = read4();
  std::unique_ptr<char[]> bytes = decompress(length);
  if (bytes) threadTempFileWrite(writeTempBMPFile, write_fname, write_fname, std::move(bytes), length, transparent);
}

void Decoder::readZlibImage(bool transparent) {
  readZlibImage(TempFileName(gmk_data), transparent);
}

void Decoder::readBGRAImage(const std::filesystem::path& write_fname, size_t width, size_t height) {
  size_t length = read4();
  std::unique_ptr<char[]> bytes = read(length);
  if (bytes) threadTempFileWrite(writeTempBGRAFile, write_fname, write_fname, std::move(bytes), width, height);
}

void Decoder::setSeed(int seed) {
  if (seed >= 0)
    decodeTable = makeDecodeTable(seed);
  else
    decodeTable = nullptr;
}

void Decoder::postponeName(std::string* name, int id, TypeCase type) {
  postponeds[type][id].push_back(name);
}

void Decoder::processPostoned(const std::string name, const int id, const TypeCase type) {
  auto idMapIt = postponeds.find(type);
  if (idMapIt == postponeds.end()) {
    return; // no postponeds for this type
  }
  auto &idMap = idMapIt->second;
  auto mutableNameIt = idMap.find(id);
  if (mutableNameIt == idMap.end()) {
    return; // no postponeds for this id
  }
  for (std::string *mutableName : mutableNameIt->second)
    mutableName->append(name.c_str(), name.size());
}

std::unique_ptr<int[]> Decoder::makeEncodeTable(int seed) {
  auto table = make_unique<int[]>(256);

  int a = 6 + (seed % 250);
  int b = seed / 250;
  for (int i = 0; i < 256; i++)
    table[i] = i;
  for (int i = 1; i < 10001; i++) {
    int j = 1 + ((i * a + b) % 254);
    int t = table[j];
    table[j] = table[j + 1];
    table[j + 1] = t;
  }
  return table;
}

std::unique_ptr<int[]> Decoder::makeDecodeTable(std::unique_ptr<int[]> encodeTable) {
  auto table = make_unique<int[]>(256);
  for (int i = 0; i < 256; i++) {
    table[encodeTable[i]] = i;
  }
  return table;
}

std::unique_ptr<int[]> Decoder::makeDecodeTable(int seed) {
  return makeDecodeTable(makeEncodeTable(seed));
}

