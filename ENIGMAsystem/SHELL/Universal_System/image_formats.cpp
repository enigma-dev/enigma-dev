/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2013 Ssss
*** Copyright (C) 2013-2014 Robert B. Colton
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

#include "image_formats.h"
#include "image_formats_exts.h"
#include "Universal_System/estring.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/nlpo2.h"

#include "gif_format.h"

#include <map>
#include <fstream>      // std::ofstream
#include <sstream>
#include <algorithm>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>

using namespace std;

#include "nlpo2.h"
inline unsigned int lgpp2(unsigned int x){//Trailing zero count. lg for perfect powers of two
  x =  (x & -x) - 1;
  x -= ((x >> 1) & 0x55555555);
  x =  ((x >> 2) & 0x33333333) + (x & 0x33333333);
  x =  ((x >> 4) + x) & 0x0f0f0f0f;
  x += x >> 8;
  return (x + (x >> 16)) & 63;
}

namespace enigma
{

std::map<std::string, ImageLoadFunction> image_load_handlers;
std::map<std::string, ImageSaveFunction> image_save_handlers;

unsigned long *bgra_to_argb(unsigned char *bgra_data, unsigned pngwidth, unsigned pngheight) {
  unsigned widfull = nlpo2dc(pngwidth) + 1, hgtfull = nlpo2dc(pngheight) + 1, ih,iw;
  const int bitmap_size = widfull * hgtfull * 4;
  unsigned char *bitmap = new unsigned char[bitmap_size]();

  unsigned i = 0;
  unsigned elem_numb = pngwidth * pngheight + 2;
  unsigned long *result = new unsigned long[elem_numb]();
  result[i++] = pngwidth; result[i++] = pngheight; // this is required for xlib icon hint

  for (ih = 0; ih < pngheight; ih++) {
    unsigned tmp = ih * widfull * 4;
    for (iw = 0; iw < pngwidth; iw++) {
      bitmap[tmp + 0] = bgra_data[4 * pngwidth * ih + iw * 4 + 0];
      bitmap[tmp + 1] = bgra_data[4 * pngwidth * ih + iw * 4 + 1];
      bitmap[tmp + 2] = bgra_data[4 * pngwidth * ih + iw * 4 + 2];
      bitmap[tmp + 3] = bgra_data[4 * pngwidth * ih + iw * 4 + 3];
      result[i++] = bitmap[tmp + 0] | (bitmap[tmp + 1] << 8) | (bitmap[tmp + 2] << 16) | (bitmap[tmp + 3] << 24);
      tmp += 4;
    }
  }

  delete[] bitmap;
  return result;
}

void image_add_loader(std::string format, ImageLoadFunction fnc) {
  image_load_handlers[format] = fnc;
}

void image_add_saver(std::string format, ImageSaveFunction fnc) {
  image_save_handlers[format] = fnc;
}

unsigned char* image_flip(const unsigned char* data, unsigned width, unsigned height, unsigned bytes) {
  //flipped upside down
  unsigned sz = width * height;
  unsigned char* rgbdata = new unsigned char[sz * bytes];
  for (unsigned int i = 0; i < height; i++) { // Doesn't matter the order now
    memcpy(&rgbdata[i*width*bytes*sizeof(unsigned char)],           // address of destination
           &data[(height-i-1)*width*bytes*sizeof(unsigned char)],   // address of source
           width*bytes*sizeof(unsigned char) );                     // number of bytes to copy
  }

  return rgbdata;
}

string image_get_format(string filename) {
  size_t fp = filename.find_last_of(".");
  if (fp == string::npos){
    return "";
  }
  string ext = filename.substr(fp);
  transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
  return ext;
}

/// Generic all-purpose image loading call.
unsigned char* image_load(string filename, string format, unsigned int* width, unsigned int* height, unsigned int* fullwidth, unsigned int* fullheight, int* imgnumb, bool flipped) {
  if (format.compare(".bmp") == 0) {
    return image_load_bmp(filename, width, height, fullwidth, fullheight, flipped);
  } else if (format.compare(".gif") == 0) {
    return image_load_gif(filename, width, height, fullwidth, fullheight, imgnumb, flipped);
  }
  auto handler = image_load_handlers.find(format);
  if (handler != image_load_handlers.end()) {
    return (*handler).second(filename, width, height, fullwidth, fullheight, flipped);
  }
  return image_load_bmp(filename, width, height, fullwidth, fullheight, flipped);
}


/// Generic all-purpose image loading call that will regexp the filename for the format and call the appropriate function.
unsigned char* image_load(string filename, unsigned int* width, unsigned int* height, unsigned int* fullwidth, unsigned int* fullheight, int* imgnumb, bool flipped) {
  string format = image_get_format(filename);
  if (format.empty()) {
    format = ".bmp";
  }
  return image_load(filename, format, width, height, fullwidth, fullheight, imgnumb, flipped);
}

/// Generic all-purpose image saving call.
int image_save(string filename, const unsigned char* data, string format, unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, bool flipped) {
  if (format.compare(".bmp") == 0) {
    return image_save_bmp(filename, data, width, height, fullwidth, fullheight, flipped);
  }
  auto handler = image_save_handlers.find(format);
  if (handler != image_save_handlers.end()) {
    return (*handler).second(filename, data, width, height, fullwidth, fullheight, flipped);
  }
  return image_save_bmp(filename, data, width, height, fullwidth, fullheight, flipped);
}

/// Generic all-purpose image saving call that will regexp the filename for the format and call the appropriate function.
int image_save(string filename, const unsigned char* data, unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, bool flipped) {
  string format = image_get_format(filename);
  if (format.empty()) {
    format = ".bmp";
  }
  return image_save(filename, data, format, width, height, fullwidth, fullheight, flipped);
}

unsigned char* image_load_bmp(
    string filename, unsigned int* width, unsigned int* height,
    unsigned int* fullwidth, unsigned int* fullheight, bool flipped) {
  if (std::ifstream bmp{filename}) {
    std::stringstream buffer;
    buffer << bmp.rdbuf();
    return image_decode_bmp(buffer.str(), width, height,
                            fullwidth, fullheight, flipped);
  }
  return nullptr;
}

namespace {

#pragma pack(push, whatever)
#pragma pack(2)
enum class BitmapCompression: uint32_t {
  RGB       = 0x00,
  RLE8      = 0x01,
  RLE4      = 0x02,
  BITFIELDS = 0x03,
  JPEG      = 0x04,
  PNG       = 0x05,
  CMYK      = 0x0B,
  CMYKRLE8  = 0x0C,
  CMYKRLE4  = 0x0D
};

enum class LogicalColorSpace: uint32_t {
  CALIBRATED_RGB = 0,   // Invokes the wrath of the calibration vectors below
  sRGB    = 0x73524742, // 'sRGB', big endian
  WINDOWS = 0x57696E20  // 'Win ', big endian (Windows default colorspace)
};

enum class BitmapIntent: uint32_t {
  ABS_COLORIMETRIC, // Maintains white point; match to nearest color in dest gamut.
  BUSINESS, // Maintains saturation; used when undithered colors are required.
  GRAPHICS, // Maintains colorimetric match; used for everything
  IMAGES // Maintains contrast; used for photographs and natural images
};

struct FP2d30 {
  int32_t fraction: 30;
  int32_t whole: 2;
  FP2d30(): fraction(0), whole(0) {}
};

struct FP2d30Vec {
  FP2d30 x, y, z;
};

struct BMPFileHeader {
  const int8_t magic_b = 'B';
  const int8_t magic_m = 'M';
  uint32_t size = 0;
  const int16_t reserved1 = 0;
  const int16_t reserved2 = 0;
  const uint32_t dataStart;
};

static_assert(sizeof(BMPFileHeader) == 14);

struct BMPInfoHeader {
  const uint32_t size = sizeof(*this);
  uint32_t width, height;
  const uint16_t numMipmaps = 1;  // Bitmap still only supports one...
  uint16_t bitsPerPixel = 32;
  BitmapCompression compression = BitmapCompression::BITFIELDS;
  uint32_t imageSize = 0;
  uint32_t hPixelsPerMeter = 26;
  uint32_t vPixelsPerMeter = 26;
  uint32_t colorUsed = 0;
  uint32_t colorImportant = 0;
  uint32_t maskRed   = 0x000000FF;
  uint32_t maskGreen = 0x0000FF00;
  uint32_t maskBlue  = 0x00FF0000;
  uint32_t maskAlpha = 0xFF000000;
  LogicalColorSpace colorSpace = LogicalColorSpace::sRGB;
  FP2d30Vec endpointRed = {};
  FP2d30Vec endpointGreen = {};
  FP2d30Vec endpointBlue = {};
  uint32_t gammaRed = 0;
  uint32_t gammaGreen = 0;
  uint32_t gammaBlue = 0;
  BitmapIntent intent = BitmapIntent::GRAPHICS;
  int32_t profileData = 0;
  int32_t profileSize = 0;
  const int32_t reserved = 0;

  bool isRGBA() const {
    return  maskRed == 0xFF000000 && maskGreen == 0x00FF0000 &&
           maskBlue == 0x0000FF00 && maskAlpha == 0x000000FF;
  }
  bool isARGB() const {
    return maskAlpha == 0xFF000000 &&  maskRed == 0x00FF0000 &&
           maskGreen == 0x0000FF00 && maskBlue == 0x000000FF;
  }
};

#pragma pack(pop)
}  // namespace


unsigned char* image_decode_bmp(
    const string &image_data, unsigned int* width, unsigned int* height,
    unsigned int* fullwidth, unsigned int* fullheight, bool flipped) {
  // Check file size against bitmap header size
  if (image_data.length() < sizeof(BMPFileHeader)) {
    fprintf(stderr, "Junk bitmap of size %lu", image_data.size());
    return nullptr;
  }

  const BMPFileHeader &bmp_file = *(BMPFileHeader*) image_data.data();

  // Verify magic number, check header offset sanity.
  if (bmp_file.magic_b != 'B' || bmp_file.magic_m != 'M' ||
      bmp_file.dataStart + sizeof(BMPInfoHeader) > image_data.length()) {
    fprintf(stderr, "Junk bitmap of size %lu", image_data.size());
    return nullptr;
  }

  const BMPInfoHeader &bmp_info =
      *(BMPInfoHeader*) (image_data.data() + sizeof(BMPFileHeader));

  if(bmp_info.bitsPerPixel != 32 && bmp_info.bitsPerPixel != 24) {
    fprintf(stderr, "No support for %dbpp bitmaps\n", bmp_info.bitsPerPixel);
    return nullptr;
  }
  const bool rgba = bmp_info.isRGBA();
  const bool argb = bmp_info.isARGB();
  if (bmp_info.bitsPerPixel == 32 && !rgba && !argb) {
    fprintf(stderr, "No support for mask format (%08X, %08X, %08X, %08X)\n",
            bmp_info.maskRed, bmp_info.maskGreen, bmp_info.maskBlue,
            bmp_info.maskAlpha);
    return nullptr;
  }

  const unsigned widfull = nlpo2dc(bmp_info.width) + 1;
  const unsigned hgtfull = nlpo2dc(bmp_info.height) + 1;
  const unsigned bitmap_size = widfull*hgtfull*4;

  unsigned char* bitmap = new unsigned char[bitmap_size](); // Initialize to 0.

  // Calculate the number of nulls that follows each line.
  const int overlap = bmp_info.width * (bmp_info.bitsPerPixel / 8) % 4;
  const int pad = overlap ? 4 - overlap : 0;

  printf("Bitmap pad: %d\n", pad);
  const char *bmp_it = image_data.data() + bmp_file.dataStart;

  for (unsigned ih = 0; ih < bmp_info.height; ih++) {
    unsigned tmp = 0;
    if (flipped) {
      tmp = ih * widfull * 4;
    } else {
      tmp = (bmp_info.height - 1 - ih) * widfull * 4;
    }
    for (unsigned iw = 0; iw < bmp_info.width; iw++) {
      if (bmp_info.bitsPerPixel == 24) {
        bitmap[tmp+0] = *bmp_it++;
        bitmap[tmp+1] = *bmp_it++;
        bitmap[tmp+2] = *bmp_it++;
        bitmap[tmp+3] = (char) 0xFF;
      }
      else if (bmp_info.bitsPerPixel == 32) {
        if (argb) {
          bitmap[tmp+0] = *bmp_it++;
          bitmap[tmp+1] = *bmp_it++;
          bitmap[tmp+2] = *bmp_it++;
          bitmap[tmp+3] = *bmp_it++;
        } else {
          bitmap[tmp+3] = *bmp_it++;
          bitmap[tmp+0] = *bmp_it++;
          bitmap[tmp+1] = *bmp_it++;
          bitmap[tmp+2] = *bmp_it++;
        }
      }
      tmp += 4;
    }
    bmp_it += pad;
  }
  *width  = bmp_info.width;
  *height = bmp_info.height;
  *fullwidth  = widfull;
  *fullheight = hgtfull;
  return bitmap;
}

unsigned char* image_load_gif(string filename, unsigned int* width, unsigned int* height, unsigned int* fullwidth, unsigned int* fullheight, int* imgnumb, bool flipped) {
  unsigned int error = 0;
  unsigned char* image = 0;

  error = load_gif_file(filename.c_str(), image, *width, *height, *fullwidth, *fullheight, *imgnumb);
  if (error) {
    DEBUG_MESSAGE(load_gif_error_text(error), MESSAGE_TYPE::M_ERROR);
    return NULL;
  }

  return image;
}

int image_save_bmp(string filename, const unsigned char* data, unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, bool flipped) {
  unsigned sz = width * height;
  FILE *bmp = fopen(filename.c_str(), "wb");
  if (!bmp) return -1;
  fwrite("BM", 2, 1, bmp);

  sz <<= 2;
  fwrite(&sz,4,1,bmp);
  fwrite("\0\0\0\0\x36\0\0\0\x28\0\0",12,1,bmp);
  fwrite(&width,4,1,bmp);
  fwrite(&height,4,1,bmp);
  //NOTE: x20 = 32bit full color, x18 = 24bit no alpha
  fwrite("\1\0\x20\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",28,1,bmp);

  unsigned bytes = 4;

  width *= bytes;
  fullwidth *= bytes;
  unsigned lastbyte = fullwidth * height;

  for (unsigned i = 0; i < lastbyte; i += fullwidth) {
    unsigned tmp = i;
    if (!flipped) {
      tmp = lastbyte - i;
    }
    for (unsigned ii = 0; ii < width; ii += bytes) {
      fwrite(&data[tmp + ii + 0],sizeof(char),1,bmp);
      fwrite(&data[tmp + ii + 1],sizeof(char),1,bmp);
      fwrite(&data[tmp + ii + 2],sizeof(char),1,bmp);
      fwrite(&data[tmp + ii + 3],sizeof(char),1,bmp);
    }
  }

  fclose(bmp);
  return 0;
}

}
