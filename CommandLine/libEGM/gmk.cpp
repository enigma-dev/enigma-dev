/** Copyright (C) 2018 Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "gmk.h"
#include "filesystem.h"
#include "action.h"

#include "libpng-util/libpng-util.h"

#include <fstream>
#include <utility>
#include <memory>
#include <future>
#include <functional>
#include <unordered_map>
#include <vector>
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

static const std::string gmk_data = "gmk_data";

namespace gmk {
ostream out(nullptr);
ostream err(nullptr);

static vector<std::string> tempFilesCreated;
static bool atexit_tempdata_cleanup_registered = false;
static void atexit_tempdata_cleanup() {
  for (const std::filesystem::path &tempFile : tempFilesCreated) {
    if (tempFile.parent_path().u8string().find(gmk_data) != std::string::npos)
      DeleteFolder(tempFile.parent_path());
    else if (std::filesystem::is_regular_file(tempFile)) DeleteFile(tempFile);
  }
}

void writeTempDataFile(const std::filesystem::path& write_fname, char *bytes, size_t length) {
  CreateDirectoryRecursive(write_fname);
  std::fstream fs(write_fname, std::fstream::out | std::fstream::binary);
  if (!fs.is_open()) return;
  fs.write(bytes, length);
  fs.close();
}

void writeTempDataFile(const std::filesystem::path& write_fname, std::unique_ptr<char[]> bytes, size_t length) {
  writeTempDataFile(write_fname, bytes.get(), length);
}

void writeTempBMPFile(const std::filesystem::path& write_fname, std::unique_ptr<char[]> bytes, size_t length, bool transparent=false) {
  static const unsigned MINHEADER = 54; //minimum BMP header size
  auto bmp = reinterpret_cast<const unsigned char*>(bytes.get()); // all of the following logic expects unsigned

  if (length < MINHEADER) {
    err << "Image from the GMK file had a length '" << length << "' smaller than the minimum header "
        << "size '" << MINHEADER << "' for the BMP format" << std::endl;
    return;
  }
  if (bmp[0] != 'B' || bmp[1] != 'M') {
    err << "Image from the GMK file did not have the correct BMP signature '"
        << bmp[0] << bmp[1] << "'" << std::endl;
    return;
  }
  unsigned pixeloffset = bmp[10] + 256 * bmp[11]; //where the pixel data starts
  //read width and height from BMP header
  size_t w = bmp[18] + bmp[19] * 256;
  size_t h = bmp[22] + bmp[23] * 256;
  //read number of channels from BMP header
  if (bmp[28] != 24 && bmp[28] != 32) {
    err << "Image from the GMK file was " << bmp[28] << " bit while this reader "
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
    err << "Image from the GMK file had a length '" << length << "' smaller than the estimated "
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

void writeTempBGRAFile(const std::filesystem::path& write_fname, std::unique_ptr<char[]> bytes, size_t width, size_t height) {
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

class Decoder {
  public:
  explicit Decoder(std::istream &in): in(in), decodeTable(nullptr) {
    if (atexit_tempdata_cleanup_registered) return;
    const int ret = std::atexit(atexit_tempdata_cleanup);
    if (ret != 0) {
      err << "Failed to register cleanup handler for process exit, temporary files will not be removed" << std::endl;
    } else {
      atexit_tempdata_cleanup_registered = true;
    }
  }

  template<class Function, class... Args>
  void threadTempFileWrite(Function&& f, const std::filesystem::path& write_fname, Args&&... args) {
    std::future<void> tempFileFuture = std::async(std::launch::async, f, std::move(args)...);
    tempFileFuturesCreated.emplace_back(std::make_pair(std::move(tempFileFuture), write_fname));
  }

  void processTempFileFutures() {
    for (auto &tempFilePair : tempFileFuturesCreated) {
      tempFilePair.second.append(tempFilePair.second.u8string());
      tempFilesCreated.push_back(tempFilePair.second.u8string());
    }
  }

  void beginInflate() {
    size_t limit = read4();
    zlibStart = in.tellg();
    zlibBuffer = decompress(limit, 0);
    zlibPos = 0;
  }

  void endInflate() {
    zlibBuffer.reset();
    zlibStart = 0;
    zlibPos = 0;
  }

  void skip(size_t count) {
    if (zlibBuffer)
      zlibPos += count;
    else
      in.seekg(count, std::ios_base::cur);
  }

  int read() {
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

  int read2() {
    int one = read(), two = read();
    return (one | (two << 8));
  }

  int read3() {
    int one = read(), two = read(), three = read();
    return (one | (two << 8) | (three << 16));
  }

  int read4() {
    int one = read(), two = read(), three = read(), four = read();
    return (one | (two << 8) | (three << 16) | (four << 24));
  }

  bool readBool() {
    return read4();
  }

  void read(char* bytes, size_t length, size_t off=0) {
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

  std::unique_ptr<char[]> read(size_t length, size_t off=0) {
    auto bytes = make_unique<char[]>(length);
    read(bytes.get(), length, off);
    return bytes;
  }

  std::string readStr() {
    size_t length = read4();
    std::unique_ptr<char[]> bytes = read(length);
    return std::string(bytes.get(), length);
  }

  double readD() {
    union { char bytes[8]; double dbl; } data;
    read(data.bytes, 8);
    return data.dbl;
  }

  std::unique_ptr<char[]> decompress(size_t &length, size_t off=0) {
    std::unique_ptr<char[]> src = read(length, off);

    z_stream zs = {}; // zero-initialize
    if (inflateInit(&zs) != Z_OK) {
      err << "Failed to initialize zlib inflate" << std::endl;
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
      err << "There was an error with code '" << ret << "' while using zlib to decompress data" << std::endl;
      return nullptr;
    }

    length = outstring.size();
    auto bytes = make_unique<char[]>(length);
    memcpy(bytes.get(), outstring.c_str(), length);

    return bytes;
  }

  void readData(const std::filesystem::path& write_fname, bool compressed=false) {
    size_t length = read4();
    std::unique_ptr<char[]> bytes = compressed ? decompress(length) : read(length);
    using WriteTempDataFileManaged = void(*)(const std::filesystem::path& write_fname, std::unique_ptr<char[]>, size_t);
    if (bytes) threadTempFileWrite((WriteTempDataFileManaged)writeTempDataFile, write_fname, write_fname, std::move(bytes), length);
  }

  void readCompressedData(const std::filesystem::path& write_fname) {
    readData(write_fname, true);
  }

  void readUncompressedData(const std::filesystem::path& write_fname) {
    readData(write_fname, false);
  }

  void readZlibImage(const std::filesystem::path& write_fname, bool transparent) {
    size_t length = read4();
    std::unique_ptr<char[]> bytes = decompress(length);
    if (bytes) threadTempFileWrite(writeTempBMPFile, write_fname, write_fname, std::move(bytes), length, transparent);
  }

  void readZlibImage(bool transparent=false) {
    readZlibImage(TempFileName(gmk_data), transparent);
  }

  void readBGRAImage(const std::filesystem::path& write_fname = TempFileName(gmk_data), size_t width=0, size_t height=0) {
    size_t length = read4();
    std::unique_ptr<char[]> bytes = read(length);
    if (bytes) threadTempFileWrite(writeTempBGRAFile, write_fname, write_fname, std::move(bytes), width, height);
  }

  void setSeed(int seed) {
    if (seed >= 0)
      decodeTable = makeDecodeTable(seed);
    else
      decodeTable = nullptr;
  }

  void postponeName(std::string* name, int id, TypeCase type) {
    postponeds[type][id].push_back(name);
  }

  void processPostoned(const std::string name, const int id, const TypeCase type) {
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

  private:
  std::istream &in;
  std::unique_ptr<char[]> zlibBuffer;
  int zlibPos, zlibStart;
  std::unique_ptr<int[]> decodeTable;
  std::unordered_map<TypeCase, std::unordered_map<int, std::vector<std::string*> > > postponeds;
  std::vector<std::pair<std::future<void>, std::filesystem::path> > tempFileFuturesCreated;

  std::unique_ptr<int[]> makeEncodeTable(int seed) {
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

  std::unique_ptr<int[]> makeDecodeTable(std::unique_ptr<int[]> encodeTable) {
    auto table = make_unique<int[]>(256);
    for (int i = 0; i < 256; i++) {
      table[encodeTable[i]] = i;
    }
    return table;
  }

  std::unique_ptr<int[]> makeDecodeTable(int seed) {
    return makeDecodeTable(makeEncodeTable(seed));
  }
};

void LoadSettingsIncludes(Decoder &dec) {
  int no = dec.read4();
  for (int i = 0; i < no; i++) {
    auto include = std::make_unique<Include>();
    dec.readStr(); // include filepath
  }
  dec.read4(); // INCLUDE_FOLDER 0=main 1=temp
  // OVERWRITE_EXISTING, REMOVE_AT_GAME_END
  dec.readBool(); dec.readBool();
}

int LoadSettings(Decoder &dec, Settings& set) {
  int ver = dec.read4();
  if (ver != 530 && ver != 542 && ver != 600 && ver != 702 && ver != 800 && ver != 810) {
    err << "Unsupported GMK Settings version: " << ver << std::endl;
    return 0;
  }

  General* gen = set.mutable_general();
  Graphics* gfx = set.mutable_graphics();
  Windowing *win = set.mutable_windowing();
  Info* inf = set.mutable_info();

  if (ver >= 800) dec.beginInflate();
  win->set_start_in_fullscreen(dec.readBool());
  if (ver >= 600) {
    gfx->set_interpolate_textures(dec.readBool());
  }
  win->set_show_border(!dec.readBool()); // inverted because negative in GM (e.g, "don't")
  gen->set_show_cursor(dec.readBool());
  gfx->set_view_scale(dec.read4());
  if (ver == 530) {
    dec.skip(8); // "fullscreen scale" & "only scale w/ hardware support"
  } else {
    win->set_is_sizeable(dec.readBool());
    win->set_stay_on_top(dec.readBool());
    gfx->set_color_outside_room_region(dec.read4());
  }
  dec.readBool(); // set_resolution

  if (ver == 530) {
    dec.skip(8); //Color Depth, Exclusive Graphics
    dec.read4(); // resolution
    dec.read4(); // int b
    //frequency = (b == 4) ? 0 : (b + 1);
    dec.skip(8); //vertical blank, caption in fullscreen
  } else {
    dec.read4(); // color depth
    dec.read4(); // resolution
    dec.read4(); // frequency
  }

  win->set_show_icons(!dec.readBool()); // inverted because negative in GM (e.g, "don't")
  if (ver > 530) gfx->set_use_synchronization(dec.readBool());
  if (ver >= 800) dec.readBool(); // DISABLE_SCREENSAVERS
  gfx->set_allow_fullscreen_change(dec.readBool());
  // LET_F1_SHOW_GAME_INFO, LET_ESC_END_GAME, LET_F5_SAVE_F6_LOAD
  dec.readBool(); dec.readBool(); dec.readBool();
  if (ver == 530) dec.skip(8); //unknown bytes, both 0
  if (ver > 600) {
    // LET_F9_SCREENSHOT, TREAT_CLOSE_AS_ESCAPE
    dec.readBool(); dec.readBool();
  }
  dec.read4(); // GAME_PRIORITY
  win->set_freeze_on_lose_focus(dec.readBool());
  int load_bar_mode = dec.read4(); // LOAD_BAR_MODE
  if (load_bar_mode == 2) { // 0=NONE 1=DEFAULT 2=CUSTOM
    if (ver < 800) {
      if (dec.read4() != -1) dec.readZlibImage(); // BACK_LOAD_BAR
      if (dec.read4() != -1) dec.readZlibImage(); // FRONT_LOAD_BAR
    } else { //ver >= 800
      if (dec.readBool()) dec.readZlibImage(); // BACK_LOAD_BAR
      if (dec.readBool()) dec.readZlibImage(); // FRONT_LOAD_BAR
    }
  }
  bool show_custom_load_image = dec.readBool(); // SHOW_CUSTOM_LOAD_IMAGE
  if (show_custom_load_image) {
    if (ver < 800) {
      if (dec.read4() != -1) dec.readZlibImage(); // LOADING_IMAGE
    } else if (dec.readBool()) {
      dec.readZlibImage(); // LOADING_IMAGE
    }
  }
  dec.readBool(); // IMAGE_PARTIALLY_TRANSPARENTY
  dec.read4(); // LOAD_IMAGE_ALPHA
  dec.readBool(); // SCALE_PROGRESS_BAR

  dec.skip(dec.read4()); // GAME_ICON

  // DISPLAY_ERRORS, WRITE_TO_LOG, ABORT_ON_ERROR
  dec.readBool(); dec.readBool(); dec.readBool();
  dec.read4(); // errors
  // TREAT_UNINIT_AS_0 = ((errors & 0x01) != 0)
  // ERROR_ON_ARGS = ((errors & 0x02) != 0)
  inf->set_author_name(dec.readStr());
  if (ver > 600)
    inf->set_version(dec.readStr());
  else
    inf->set_version(std::to_string(dec.read4()));
  inf->set_last_changed(dec.readD());
  inf->set_information(dec.readStr());
  if (ver < 800) {
    int no = dec.read4(); // number of constants
    for (int i = 0; i < no; i++) {
      dec.readStr(); // constant name
      dec.readStr(); // constant value
    }
  }
  if (ver > 600) {
    gen->set_version_major(dec.read4());
    gen->set_version_minor(dec.read4());
    gen->set_version_release(dec.read4());
    gen->set_version_build(dec.read4());

    gen->set_company(dec.readStr());
    gen->set_product(dec.readStr());
    gen->set_copyright(dec.readStr());
    gen->set_description(dec.readStr());

    if (ver >= 800) dec.skip(8); //last changed
  } else if (ver > 530) {
    LoadSettingsIncludes(dec);
  }
  dec.endInflate();

  return 1;
}

int LoadTriggers(Decoder &dec) {
  int ver = dec.read4();
  if (ver != 800) {
    err << "Unsupported GMK Triggers version: " << ver << std::endl;
    return 0;
  }

  int no = dec.read4();
  for (int i = 0; i < no; i++) {
    dec.beginInflate();
    if (!dec.readBool()) {
      dec.endInflate();
      continue;
    }
    ver = dec.read4();
    if (ver != 800) {
      err << "Unsupported GMK Trigger version: " << ver << std::endl;
      return 0;
    }
    dec.readStr(); // trigger name
    dec.readStr(); // trigger condition
    dec.read4(); // trigger check step
    dec.readStr(); // trigger constant
    dec.endInflate();
  }
  dec.skip(8); //last changed

  return 1;
}

int LoadConstants(Decoder &dec) {
  int ver = dec.read4();
  if (ver != 800) {
    err << "Unsupported GMK Constants version: " << ver << std::endl;
    return 0;
  }

  int no = dec.read4();
  for (int i = 0; i < no; i++) {
    dec.readStr(); // constant name
    dec.readStr(); // constant value
  }
  dec.skip(8); //last changed

  return 1;
}

std::unique_ptr<Sound> LoadSound(Decoder &dec, int ver, const std::string& name) {
  auto sound = std::make_unique<Sound>();

  int kind53 = -1;
  if (ver == 440)
    kind53 = dec.read4(); //kind (wav, mp3, etc)
  else
    sound->set_kind(static_cast<Sound::Kind>(dec.read4())); //normal, background, etc
  sound->set_file_extension(dec.readStr());
  
  const std::filesystem::path fName = TempFileName(gmk_data)/(name + sound->file_extension());

  if (ver == 440) {
    //-1 = no sound
    if (kind53 != -1) {
      dec.readCompressedData(fName);
      sound->set_data(fName.u8string());
    }
    dec.skip(8);
    sound->set_preload(!dec.readBool());
  } else {
    sound->set_file_name(dec.readStr());
    if (dec.readBool()) {
      if (ver == 600) {
        dec.readCompressedData(fName);
        sound->set_data(fName.u8string());
      } else {
        dec.readUncompressedData(fName);
        sound->set_data(fName.u8string());
      }
    }
    dec.read4(); // effects flags
    sound->set_volume(dec.readD());
    sound->set_pan(dec.readD());
    sound->set_preload(dec.readBool());
  }

  return sound;
}

std::unique_ptr<Sprite> LoadSprite(Decoder &dec, int ver, const std::string& name) {
  auto sprite = std::make_unique<Sprite>();

  int w = 0, h = 0;
  bool transparent = false;
  if (ver < 800) {
    w = dec.read4();
    h = dec.read4();
    sprite->set_bbox_left(dec.read4());
    sprite->set_bbox_right(dec.read4());
    sprite->set_bbox_bottom(dec.read4());
    sprite->set_bbox_top(dec.read4());
    transparent = dec.readBool();
    if (ver > 400) {
      sprite->set_smooth_edges(dec.readBool());
      sprite->set_preload(dec.readBool());
    }
    sprite->set_bbox_mode(static_cast<Sprite::BoundingBox>(dec.read4()));
    bool precise = dec.readBool();
    sprite->set_shape(precise ? Sprite::Shape::Sprite_Shape_PRECISE : Sprite::Shape::Sprite_Shape_RECTANGLE);
    if (ver == 400) {
      dec.skip(4); //use video memory
      sprite->set_preload(!dec.readBool());
    }
  } else {
    transparent = false;
  }
  sprite->set_origin_x(dec.read4());
  sprite->set_origin_y(dec.read4());

  int nosub = dec.read4();
  std::filesystem::path tempName = TempFileName(gmk_data);
  for (int i = 0; i < nosub; i++) {
    std::filesystem::path fName = tempName/(name + "_" + std::to_string(i) + ".png");
    if (ver >= 800) {
      int subver = dec.read4();
      if (subver != 800 && subver != 810) {
        err << "GMK Sprite with inner version '" << ver << "' has a subimage with id '" << i
            << "' that has an unsupported version '" << subver << "'" << std::endl;
        return nullptr;
      }
      w = dec.read4();
      h = dec.read4();
      if (w != 0 && h != 0) {
        dec.readBGRAImage(fName, w, h);
        sprite->add_subimages(fName.u8string());
      }
    } else {
      if (dec.read4() == -1) continue;
      dec.readZlibImage(fName, transparent);
      sprite->add_subimages(fName.u8string());
    }
  }
  sprite->set_width(w);
  sprite->set_height(h);

  if (ver >= 800) {
    sprite->set_shape(static_cast<Sprite::Shape>(dec.read4()));
    sprite->set_alpha_tolerance(dec.read4());
    sprite->set_separate_mask(dec.readBool());
    sprite->set_bbox_mode(static_cast<Sprite::BoundingBox>(dec.read4()));
    sprite->set_bbox_left(dec.read4());
    sprite->set_bbox_right(dec.read4());
    sprite->set_bbox_bottom(dec.read4());
    sprite->set_bbox_top(dec.read4());
  }

  return sprite;
}

std::unique_ptr<Background> LoadBackground(Decoder &dec, int ver, const std::string& name) {
  auto background = std::make_unique<Background>();

  int w = 0, h = 0;
  bool transparent = false;
  if (ver < 710) {
    w = dec.read4();
    h = dec.read4();
    transparent = dec.readBool();
    if (ver > 400) {
      background->set_smooth_edges(dec.readBool());
      background->set_preload(dec.readBool());
    } else {
      dec.skip(4); //use video memory
      background->set_preload(!dec.readBool());
    }
  }

  if (ver > 400) {
    background->set_use_as_tileset(dec.readBool());
    background->set_tile_width(dec.read4());
    background->set_tile_height(dec.read4());
    background->set_horizontal_offset(dec.read4());
    background->set_vertical_offset(dec.read4());
    background->set_horizontal_spacing(dec.read4());
    background->set_vertical_spacing(dec.read4());
  }

  const std::filesystem::path fName = TempFileName(gmk_data)/(name + ".png");
  
  if (ver < 710) {
    if (dec.readBool()) {
      if (dec.read4() != -1) {
        dec.readZlibImage(fName, transparent);
        background->set_image(fName.u8string());
      }
    }
  } else { // >= 710
    int dataver = dec.read4();
    if (dataver != 800) {
      err << "GMK Background with inner version '" << ver << "' has image data with "
          << "unsupported version '" << dataver << "'" << std::endl;
      return nullptr;
    }
    w = dec.read4();
    h = dec.read4();
    if (w != 0 && h != 0) {
      dec.readBGRAImage(fName, w, h);
      background->set_image(fName.u8string());
    }
  }

  background->set_width(w);
  background->set_height(h);

  return background;
}

std::unique_ptr<Path> LoadPath(Decoder &dec, int /*ver*/, const std::string& /*name*/) {
  auto path = std::make_unique<Path>();

  path->set_smooth(dec.readBool());
  path->set_closed(dec.readBool());
  path->set_precision(dec.read4());
  dec.postponeName(path->mutable_background_room_name(), dec.read4(), TypeCase::kRoom);
  path->set_hsnap(dec.read4());
  path->set_vsnap(dec.read4());
  int nopoints = dec.read4();
  for (int i = 0; i < nopoints; i++) {
    auto point = path->add_points();
    point->set_x(dec.readD());
    point->set_y(dec.readD());
    point->set_speed(dec.readD());
  }

  return path;
}

std::unique_ptr<Script> LoadScript(Decoder &dec, int /*ver*/, const std::string& /*name*/) {
  auto script = std::make_unique<Script>();

  script->set_code(dec.readStr());

  return script;
}

std::unique_ptr<Font> LoadFont(Decoder &dec, int /*ver*/, const std::string& /*name*/) {
  auto font = std::make_unique<Font>();

  font->set_font_name(dec.readStr());
  font->set_size(dec.read4());
  font->set_bold(dec.readBool());
  font->set_italic(dec.readBool());

  int rangemin = dec.read2();
  font->set_charset(dec.read());
  int aa = dec.read();
  //if (aa == 0 && gmk < 810) aa = 3;
  font->set_antialias(aa);

  auto char_range = font->add_ranges();
  char_range->set_min(rangemin);
  char_range->set_max(dec.read4());

  return font;
}

int LoadActions(Decoder &dec, std::string* code, std::string eventName) {
  int ver = dec.read4();
  if (ver != 400) {
    err << "Unsupported GMK actions version '" << ver <<
      "' for event '" << eventName << "'" << std::endl;
    return 0;
  }

  int noacts = dec.read4();
  std::vector<Action> actions;
  actions.reserve(noacts);
  for (int k = 0; k < noacts; k++) {
    Action action;
    dec.skip(4);
    action.set_libid(dec.read4());
    action.set_id(dec.read4());
    action.set_kind(static_cast<ActionKind>(dec.read4()));
    action.set_use_relative(dec.readBool());
    action.set_is_question(dec.readBool());
    action.set_use_apply_to(dec.readBool());
    action.set_exe_type(static_cast<ActionExecution>(dec.read4()));
    action.set_function_name(dec.readStr());
    action.set_code_string(dec.readStr());

    int numofargs = dec.read4(); // number of library action's arguments
    int numofargkinds = dec.read4(); // number of library action's argument kinds
    auto argkinds = make_unique<int[]>(numofargkinds);
    for (int x = 0; x < numofargkinds; x++)
      argkinds[x] = dec.read4(); // argument x's kind

    int applies_to = dec.read4();
    switch (applies_to) {
      case -1:
        action.set_who_name("self");
        break;
      case -2:
        action.set_who_name("other");
        break;
      default:
        action.set_who_name(std::to_string(applies_to));
    }
    action.set_relative(dec.readBool());

    int actualnoargs = dec.read4();
    for (int l = 0; l < actualnoargs; l++) {
      if (l >= numofargs) {
        dec.skip(dec.read4());
        continue;
      }
      auto argument = action.add_arguments();
      argument->set_kind(static_cast<ArgumentKind>(argkinds[l]));
      std::string strval = dec.readStr();

      using ArgumentMutator = std::function<std::string*(Argument*)>;
      using MutatorMap = std::unordered_map<ArgumentKind, ArgumentMutator>;

      static const MutatorMap mutatorMap({
        { ArgumentKind::ARG_SOUND,      &Argument::mutable_sound      },
        { ArgumentKind::ARG_BACKGROUND, &Argument::mutable_background },
        { ArgumentKind::ARG_SPRITE,     &Argument::mutable_sprite     },
        { ArgumentKind::ARG_SCRIPT,     &Argument::mutable_script     },
        { ArgumentKind::ARG_FONT,       &Argument::mutable_font       },
        { ArgumentKind::ARG_OBJECT,     &Argument::mutable_object     },
        { ArgumentKind::ARG_TIMELINE,   &Argument::mutable_timeline   },
        { ArgumentKind::ARG_ROOM,       &Argument::mutable_room       },
        { ArgumentKind::ARG_PATH,       &Argument::mutable_path       }
      });

      const auto &mutator = mutatorMap.find(argument->kind());
      if (mutator != mutatorMap.end()) {
        mutator->second(argument)->append(strval);
      } else {
        argument->set_string(strval);
      }
    }

    action.set_is_not(dec.readBool());
    actions.emplace_back(action);
  }

  code->append(Actions2Code(actions));

  return 1;
}

std::unique_ptr<Timeline> LoadTimeline(Decoder &dec, int /*ver*/, const std::string& /*name*/) {
  auto timeline = std::make_unique<Timeline>();

  int nomoms = dec.read4();
  for (int i = 0; i < nomoms; i++) {
    auto moment = timeline->add_moments();
    moment->set_step(dec.read4());
    if (!LoadActions(dec, moment->mutable_code(), "step_" + std::to_string(moment->step()))) return nullptr;
  }

  return timeline;
}

std::unique_ptr<Object> LoadObject(Decoder &dec, int /*ver*/, const std::string& /*name*/) {
  auto object = std::make_unique<Object>();

  dec.postponeName(object->mutable_sprite_name(), dec.read4(), TypeCase::kSprite);
  object->set_solid(dec.readBool());
  object->set_visible(dec.readBool());
  object->set_depth(dec.read4());
  object->set_persistent(dec.readBool());
  dec.postponeName(object->mutable_parent_name(), dec.read4(), TypeCase::kObject);
  dec.postponeName(object->mutable_mask_name(), dec.read4(), TypeCase::kSprite);

  int noEvents = dec.read4() + 1;
  for (int i = 0; i < noEvents; i++) {
    while (true) {
      int second = dec.read4();
      if (second == -1) break;

      auto *event = object->add_legacy_events();
      event->set_type(i);
      event->set_number(second);

      if (!LoadActions(dec, event->mutable_code(), event->name())) return nullptr;
    }
  }

  return object;
}

std::unique_ptr<Room> LoadRoom(Decoder &dec, int ver, const std::string& /*name*/) {
  auto room = std::make_unique<Room>();

  room->set_caption(dec.readStr());
  room->set_width(dec.read4());
  room->set_height(dec.read4());
  room->set_vsnap(dec.read4());
  room->set_hsnap(dec.read4());
  room->set_isometric(dec.readBool());
  room->set_speed(dec.read4());
  room->set_persistent(dec.readBool());
  room->set_color(dec.read4());
  //NOTE: GM8.1 is inconsistent packing a second boolean into this one.
  int clearBackgroundAndView = dec.read4();
  room->set_show_color((clearBackgroundAndView & 1) != 0);
  room->set_clear_view_background((clearBackgroundAndView & (1 << 1)) == 0);
  room->set_creation_code(dec.readStr());

  int nobackgrounds = dec.read4();
  for (int j = 0; j < nobackgrounds; j++) {
    auto background = room->add_backgrounds();
    background->set_visible(dec.readBool());
    background->set_foreground(dec.readBool());
    dec.postponeName(background->mutable_background_name(), dec.read4(), TypeCase::kBackground);
    background->set_x(dec.read4());
    background->set_y(dec.read4());
    background->set_htiled(dec.readBool());
    background->set_vtiled(dec.readBool());
    background->set_hspeed(dec.read4());
    background->set_vspeed(dec.read4());
    background->set_stretch(dec.readBool());
  }

  room->set_enable_views(dec.readBool());
  int noviews = dec.read4();
  for (int j = 0; j < noviews; j++) {
    auto view = room->add_views();
    view->set_visible(dec.readBool());
    view->set_xview(dec.read4());
    view->set_yview(dec.read4());
    view->set_wview(dec.read4());
    view->set_hview(dec.read4());
    view->set_xport(dec.read4());
    view->set_yport(dec.read4());
    if (ver > 520) {
      view->set_wport(dec.read4());
      view->set_hport(dec.read4());
    } else {
      //Older versions of GM assume port_size == view_size.
      view->set_wport(view->wview());
      view->set_hport(view->hview());
    }
    view->set_hborder(dec.read4());
    view->set_vborder(dec.read4());
    view->set_hspeed(dec.read4());
    view->set_vspeed(dec.read4());
    dec.postponeName(view->mutable_object_following(), dec.read4(), TypeCase::kObject);
  }

  int noinstances = dec.read4();
  for (int j = 0; j < noinstances; j++) {
    auto instance = room->add_instances();
    instance->set_x(dec.read4());
    instance->set_y(dec.read4());
    dec.postponeName(instance->mutable_object_type(), dec.read4(), TypeCase::kObject);
    instance->set_id(dec.read4());
    instance->set_creation_code(dec.readStr());
    instance->mutable_editor_settings()->set_locked(dec.readBool());
  }

  int notiles = dec.read4();
  for (int j = 0; j < notiles; j++) {
    auto tile = room->add_tiles();
    tile->set_x(dec.read4());
    tile->set_y(dec.read4());
    dec.postponeName(tile->mutable_background_name(), dec.read4(), TypeCase::kBackground);
    tile->set_xoffset(dec.read4());
    tile->set_yoffset(dec.read4());
    tile->set_width(dec.read4());
    tile->set_height(dec.read4());
    tile->set_depth(dec.read4());
    tile->set_id(dec.read4());
    tile->mutable_editor_settings()->set_locked(dec.readBool());
  }

  dec.readBool(); // REMEMBER_WINDOW_SIZE
  dec.read4(); // EDITOR_WIDTH
  dec.read4(); // EDITOR_HEIGHT
  dec.readBool(); // SHOW_GRID
  dec.readBool(); // SHOW_OBJECTS
  dec.readBool(); // SHOW_TILES
  dec.readBool(); // SHOW_BACKGROUNDS
  dec.readBool(); // SHOW_FOREGROUNDS
  dec.readBool(); // SHOW_VIEWS
  dec.readBool(); // DELETE_UNDERLYING_OBJECTS
  dec.readBool(); // DELETE_UNDERLYING_TILES
  if (ver == 520) dec.skip(6 * 4); //tile info
  // CURRENT_TAB, SCROLL_BAR_X, SCROLL_BAR_Y
  dec.read4(); dec.read4(); dec.read4();

  return room;
}

int LoadIncludes(Decoder &dec) {
  int ver = dec.read4();
  if (ver != 430 && ver != 600 && ver != 620 && ver != 800 && ver != 810) {
    err << "Unsupported GMK Includes version: " << ver << std::endl;
    return 0;
  }

  int no = dec.read4();
  for (int i = 0; i < no; i++) {
    if (ver >= 800) {
      dec.beginInflate();
      dec.skip(8); //last changed
    }
    ver = dec.read4();
    if (ver != 620 && ver != 800 && ver != 810) {
      err << "Unsupported GMK Include version: " << ver << std::endl;
      return 0;
    }

    auto include = std::make_unique<Include>();
    dec.readStr(); // filename
    dec.readStr(); // filepath
    dec.readBool(); // isOriginal
    include->set_size(dec.read4());
    include->set_store(dec.readBool());
    if (include->store()) { //store in editable?
      std::unique_ptr<char[]> data = dec.read(dec.read4()); // data
    }
    include->set_export_action(static_cast<Include::ExportAction>(dec.read4()));
    include->set_export_directory(dec.readStr());
    include->set_overwrite_existing(dec.readBool());
    include->set_free_after_export(dec.readBool());
    include->set_remove_at_game_end(dec.readBool());

    dec.endInflate();
  }

  return 1;
}

int LoadPackages(Decoder &dec) {
  int ver = dec.read4();
  if (ver != 700) {
    err << "Unsupported GMK Extension Packages version: " << ver << std::endl;
    return 0;
  }

  int no = dec.read4();
  for (int i = 0; i < no; i++) {
    dec.readStr(); // package name
  }

  return 1;
}

int LoadGameInformation(Decoder &dec) {
  int ver = dec.read4();
  if (ver != 430 && ver != 600 && ver != 620 && ver != 800 && ver != 810) {
    err << "Unsupported GMK Game Information version: " << ver << std::endl;
    return 0;
  }

  if (ver >= 800) dec.beginInflate();
  dec.read4(); // int bc, if non-zero then it's set
  // if (bc >= 0) // background color
  if (ver < 800)
    dec.readBool(); // embed game window
  else
    dec.readBool(); // show help in a separate window, inverted so negate it !
  if (ver > 430) {
    dec.readStr(); // form caption
    // LEFT, TOP, WIDTH, HEIGHT
    dec.read4(); dec.read4(); dec.read4(); dec.read4();
    // SHOW_BORDER, ALLOW_RESIZE, STAY_ON_TOP, PAUSE_GAME
    dec.readBool(); dec.readBool(); dec.readBool(); dec.readBool();
  }
  if (ver >= 800) dec.skip(8); //last changed
  dec.readStr(); // the rtf text
  dec.endInflate();

  return 1;
}

using FactoryFunction = std::function<std::unique_ptr<google::protobuf::Message>(Decoder&, int, const std::string&)>;

struct GroupFactory {
  TypeCase type;
  set<int> supportedGroupVersions;
  set<int> supportedVersions;
  FactoryFunction loadFunc;
};

int LoadGroup(Decoder &dec, TypeMap &typeMap, GroupFactory groupFactory) {
  TypeCase type = groupFactory.type;
  int groupVer = dec.read4();
  if (!groupFactory.supportedGroupVersions.count(groupVer)) {
    err << "GMK group '" << type << "' with version '" << groupVer << "' is unsupported" << std::endl;
    return 0;
  }

  int count = dec.read4();
  for (int i = 0; i < count; ++i) {
    if (groupVer >= 800) dec.beginInflate();
    if (!dec.readBool()) { // was deleted
      dec.endInflate();
      continue;
    }
    std::string name = dec.readStr();
    if (groupVer == 800) dec.skip(8); // last changed

    int ver = dec.read4();
    if (!groupFactory.supportedVersions.count(ver)) {
      err << "GMK resource of type '" << type << "' with name '" << name
          << "' has an unsupported version '" << ver << "'" << std::endl;
      return 0;
    }

    auto res = groupFactory.loadFunc(dec, ver, name);
    if (!res) {
      err << "There was a problem reading GMK resource of type '" << type << "' with name '" << name
          << "' and the project cannot be loaded" << std::endl;
      return 0;
    }

    const google::protobuf::Descriptor *desc = res->GetDescriptor();
    const google::protobuf::Reflection *refl = res->GetReflection();
    const google::protobuf::FieldDescriptor *field = desc->FindFieldByName("id");
    if (field) {
      refl->SetInt32(res.get(), field, i);
    }

    auto &resMap = typeMap[type];
    resMap[i] = std::move(res);
    dec.endInflate();
  }

  return 1;
}

void LoadTree(Decoder &dec, TypeMap &typeMap, TreeNode* root) {
  const int status = dec.read4();
  const int kind = dec.read4();
  const int id = dec.read4();
  const std::string name = dec.readStr();
  const int children = dec.read4();

  TreeNode *node = root->add_child();
  node->set_name(name);
  node->set_folder(status <= 2);
  if (node->folder()) {
    for (int i = 0; i < children; i++) {
      LoadTree(dec, typeMap, node);
    }
  } else {
    static const TypeCase RESOURCE_KIND[] = { TypeCase::TYPE_NOT_SET,TypeCase::kObject,TypeCase::kSprite,TypeCase::kSound,
      TypeCase::kRoom,TypeCase::TYPE_NOT_SET,TypeCase::kBackground,TypeCase::kScript,TypeCase::kPath,TypeCase::kFont,TypeCase::TYPE_NOT_SET,
      TypeCase::TYPE_NOT_SET,TypeCase::kTimeline,TypeCase::TYPE_NOT_SET,TypeCase::kShader };

    const TypeCase type = RESOURCE_KIND[kind];

    // Handle postponed id->name references
    dec.processPostoned(name, id, type);

    auto typeMapIt = typeMap.find(type);
    if (typeMapIt == typeMap.end()) {
      err << "No map of ids to protocol buffers for GMK kind '" << kind
          << "' so tree node with name '" << name << "' will not have "
          << "its protocol buffer set" << std::endl;
      return;
    }
    auto &resMap = typeMapIt->second;

    const google::protobuf::Descriptor *desc = node->GetDescriptor();
    const google::protobuf::Reflection *refl = node->GetReflection();
    const google::protobuf::FieldDescriptor *field = desc->FindFieldByNumber(type);
    if (!field) {
      err << "TreeNode protocol buffer does not have a field for GMK type '" << type << "'" << std::endl;
      return;
    }
    refl->SetAllocatedMessage(node, resMap[id].release(), field);
  }
}

std::unique_ptr<buffers::Project> LoadGMK(std::string fName, const EventData* event_data) {
  static const vector<GroupFactory> groupFactories({
    { TypeCase::kSound,      { 400, 800      }, { 440, 600, 800      }, LoadSound      },
    { TypeCase::kSprite,     { 400, 800, 810 }, { 400, 542, 800, 810 }, LoadSprite     },
    { TypeCase::kBackground, { 400, 800      }, { 400, 543, 710      }, LoadBackground },
    { TypeCase::kPath,       { 420, 800      }, { 530                }, LoadPath       },
    { TypeCase::kScript,     { 400, 800, 810 }, { 400, 800, 810      }, LoadScript     },
    { TypeCase::kFont,       { 440, 540, 800 }, { 540, 800           }, LoadFont       },
    { TypeCase::kTimeline,   { 500, 800      }, { 500                }, LoadTimeline   },
    { TypeCase::kObject,     { 400, 800      }, { 430                }, LoadObject     },
    { TypeCase::kRoom,       { 420, 800      }, { 520, 541           }, LoadRoom       }
  });
  TypeMap typeMap;
  std::ifstream in(fName, std::ios::binary);
  if (!in) {
    err << "Could not open GMK for reading: " << fName << std::endl;
    return nullptr;
  }
  Decoder dec(in);

  int identifier = dec.read4();
  if (identifier != 1234321) {
    err << "Invalid GMK identifier: " << identifier << std::endl;
    return nullptr;
  } else {
    out << "identifier: " << identifier << std::endl;
  }
  int ver = dec.read4();
  if (ver != 530 && ver != 600 && ver != 701 && ver != 800 && ver != 810) {
    err << "Unsupported GMK version: " << ver << std::endl;
    return nullptr;
  } else {
    out << "game version: " << ver << std::endl;
  }

  if (ver == 530) dec.skip(4);
  int game_id = 0;
  if (ver == 701) {
    int s1 = dec.read4();
    int s2 = dec.read4();
    dec.skip(s1 * 4);
    //since only the first byte of the game id isn't encrypted, we have to do some acrobatics here
    int seed = dec.read4();
    dec.skip(s2 * 4);
    int b1 = dec.read();
    dec.setSeed(seed);
    game_id = b1 | dec.read3() << 8;
  } else {
    game_id = dec.read4();
  }
  out << "game id: " << game_id << std::endl;
  dec.skip(16); //16 bytes GAME_GUID

  Settings settings;
  if (!LoadSettings(dec, settings)) return nullptr;

  if (ver >= 800) {
    if (!LoadTriggers(dec)) return nullptr;
    if (!LoadConstants(dec)) return nullptr;
  }

  for (auto factory : groupFactories) {
    if (!LoadGroup(dec, typeMap, factory)) return nullptr;
  }

  // last instance id, last tile id read after the final room is read
  dec.read4(); dec.read4();

  if (ver >= 700) {
    if (!LoadIncludes(dec)) return nullptr;
    if (!LoadPackages(dec)) return nullptr;
  }

  if (!LoadGameInformation(dec)) return nullptr;

  // Library Creation Code
  ver = dec.read4();
  if (ver != 500) {
    err << "Unsupported GMK Library Creation Code version: " << ver << std::endl;
    return 0;
  }
  int no = dec.read4();
  for (int i = 0; i < no; i++)
    dec.skip(dec.read4());

  // Room Execution Order
  ver = dec.read4();
  if (ver != 500 && ver != 540 && ver != 700) {
    err << "Unsupported GMK Room Execution Order version: " << ver << std::endl;
    return 0;
  }
  dec.skip(dec.read4() * 4);

  // Project Tree
  auto root = make_unique<TreeNode>();
  int rootnodes = (ver > 540) ? 12 : 11;
  while (rootnodes-- > 0) {
    LoadTree(dec, typeMap, root.get());
  }

  auto proj = std::make_unique<buffers::Project>();
  buffers::Game *game = proj->mutable_game();
  game->set_allocated_root(root.release());
  // ensure all temp data files are written and the paths are set in the protos
  dec.processTempFileFutures();
  
  LegacyEventsToEGM(proj.get(), event_data);

  return proj;
}

}  //namespace gmk
