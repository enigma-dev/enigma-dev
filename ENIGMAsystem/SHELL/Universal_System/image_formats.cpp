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
#include "strings_util.h"
#include "image_formats_exts.h"
#include "Universal_System/estring.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/nlpo2.h"

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

// FIXME: as of 05-09-2020 some filesystem stuff like extension() isn't implemented on android
// https://github.com/android/ndk/issues/609
#ifdef __ANDROID__
std::string_view std::__ndk1::__fs::filesystem::path::__extension() const {
  std::string filename = u8string();
  size_t fp = filename.find_last_of(".");
  if (fp == string::npos){
    return "";
  }
  return filename.substr(fp);
}

int std::__ndk1::__fs::filesystem::path::__compare(std::__ndk1::basic_string_view<char, std::__ndk1::char_traits<char> > other) const {
  if (u8string() == other) return 0; 
  if (u8string() > other) return 1; else return -1; 
}
#endif

namespace enigma
{

std::map<std::filesystem::path, ImageLoadFunction> image_load_handlers = {{".bmp", image_load_bmp}, {".gif", image_load_gif}};
std::map<std::filesystem::path, ImageSaveFunction> image_save_handlers = {{".bmp", image_save_bmp}};

Color image_get_pixel_color(unsigned char* pxdata, unsigned w, unsigned h, unsigned x, unsigned y) {
  Color c;
  size_t index = 4 * (y * w + x);
  c.b = pxdata[index];
  c.g = pxdata[index + 1];
  c.r = pxdata[index + 2];
  c.a = pxdata[index + 3];
  return c;
}

void image_swap_color(unsigned char* pxdata, unsigned w, unsigned h, Color oldColor, Color newColor) {  
  unsigned int ih, iw;
  for (ih = 0; ih < h; ih++) {
    int index = ih * w * 4;
    
    for (iw = 0; iw < w; iw++) {
      if (
           pxdata[index]     == oldColor.b
        && pxdata[index + 1] == oldColor.g
        && pxdata[index + 2] == oldColor.r
        && pxdata[index + 3] == oldColor.a
        ) {
          pxdata[index]     = newColor.b;
          pxdata[index + 1] = newColor.g;
          pxdata[index + 2] = newColor.r;
          pxdata[index + 3] = newColor.a;
      }

      index += 4;
    }
  }
}

std::vector<RawImage> image_split(unsigned char* pxdata, unsigned w, unsigned h, unsigned imgcount) {
  std::vector<RawImage> imgs(imgcount);
  unsigned splitWidth = w / imgcount;
  
  for (unsigned i = 0; i < imgcount; ++i) {
    
    imgs[i].pxdata = new unsigned char[splitWidth * h * 4]();
    imgs[i].w = splitWidth;
    imgs[i].h = h;
    
    unsigned ih,iw;
    unsigned xcelloffset = i * splitWidth * 4;
    
    for (ih = 0; ih < h; ih++) {
      unsigned tmp = ih * w * 4 + xcelloffset;
      unsigned tmpcell = ih * splitWidth * 4;
      
      for (iw = 0; iw < splitWidth; iw++) {
        imgs[i].pxdata[tmpcell]     = pxdata[tmp];
        imgs[i].pxdata[tmpcell + 1] = pxdata[tmp + 1];
        imgs[i].pxdata[tmpcell + 2] = pxdata[tmp + 2];
        imgs[i].pxdata[tmpcell + 3] = pxdata[tmp + 3];
        tmp += 4;
        tmpcell += 4;
      }
    }
  }
  
  return imgs;
}

RawImage image_pad(unsigned char* pxdata, unsigned origWidth, unsigned origHeight, unsigned newWidth, unsigned newHeight) {
  RawImage padded;
  padded.w = newWidth;
  padded.h = newHeight;
  
  padded.pxdata = new unsigned char[4 * newWidth * newHeight + 1];
  
  unsigned char* imgpxptr = padded.pxdata;
  unsigned rowindex, colindex;
  for (rowindex = 0; rowindex < origHeight; rowindex++) {
    for (colindex = 0; colindex < origWidth; colindex++) {
      *imgpxptr++ = *pxdata++;
      *imgpxptr++ = *pxdata++;
      *imgpxptr++ = *pxdata++;
      *imgpxptr++ = *pxdata++;
    }
    
    std::memset(imgpxptr, 0, (newWidth - colindex) << 2);
    imgpxptr += (newWidth - colindex) << 2;
  }
  
  std::memset(imgpxptr, 0, (newHeight - origHeight) * newWidth);
  
  return padded;
}

RawImage image_crop(const unsigned char* pxdata, unsigned origWidth, unsigned origHeight, unsigned newWidth, unsigned newHeight) {
  RawImage img;
  img.w = newWidth;
  img.h = newHeight;
  img.pxdata = new unsigned char[newWidth * newHeight * 4];
  
  unsigned stride = 4;
  for (unsigned i = 0; i < newHeight; i++) {
    unsigned tmp = i;
    unsigned bmp = i;
    tmp *= stride * origWidth;
    bmp *= stride * newWidth;
    for (unsigned ii = 0; ii < newWidth*stride; ii += stride) {
      img.pxdata[bmp + ii + 2] = pxdata[tmp + ii + 0];
      img.pxdata[bmp + ii + 1] = pxdata[tmp + ii + 1];
      img.pxdata[bmp + ii + 0] = pxdata[tmp + ii + 2];
      img.pxdata[bmp + ii + 3] = pxdata[tmp + ii + 3];
    }
  }
  
  return img;
}

unsigned long *bgra_to_argb(unsigned char *bgra_data, unsigned pngwidth, unsigned pngheight, bool prepend_size) {
  unsigned widfull = nlpo2dc(pngwidth) + 1, hgtfull = nlpo2dc(pngheight) + 1, ih, iw;
  const int bitmap_size = widfull * hgtfull * 4;
  unsigned char *bitmap = new unsigned char[bitmap_size]();

  unsigned i = 0;
  unsigned elem_numb = pngwidth * pngheight + ((prepend_size) ? 2 : 0);
  unsigned long *result = new unsigned long[elem_numb]();
  if (prepend_size) {
    result[i++] = pngwidth; result[i++] = pngheight; // this is required for xlib icon hint
  }

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

void image_add_loader(const std::filesystem::path& extension, ImageLoadFunction fnc) {
  image_load_handlers[extension] = fnc;
}

void image_add_saver(const std::filesystem::path& extension, ImageSaveFunction fnc) {
  image_save_handlers[extension] = fnc;
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

/// Generic all-purpose image loading call that will regexp the filename for the format and call the appropriate function.
std::vector<RawImage> image_load(const std::filesystem::path& filename) {
  /*std::filesystem::path extension = filename.extension();
  if (extension.empty()) {
    DEBUG_MESSAGE("No extension in image filename: " + filename.u8string() + ". Assumimg .bmp", MESSAGE_TYPE::M_WARNING);
    extension = ".bmp";
  }
  
  auto handler = image_load_handlers.find(tolower(extension.u8string()));
  if (handler != image_load_handlers.end()) {
    return (*handler).second(filename);
  } else {
    DEBUG_MESSAGE("Unsupported image format extension in image filename: " + filename.u8string() , MESSAGE_TYPE::M_ERROR);
    return std::vector<RawImage>();
  }*/
}

/// Generic all-purpose image saving call.
int image_save(const std::filesystem::path& filename, const unsigned char* data, unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, bool flipped) {
  /*std::filesystem::path extension = filename.extension();
  auto handler = image_save_handlers.find(tolower(extension.u8string()));
  if (extension.empty() || handler != image_save_handlers.end()) {
    return (*handler).second(filename, data, width, height, fullwidth, fullheight, flipped);
  } else {
    DEBUG_MESSAGE("Unsupported image format extension in image filename: " + filename.u8string() + " saving as BMP" , MESSAGE_TYPE::M_WARNING);
    return image_save_bmp(filename, data, width, height, fullwidth, fullheight, flipped);
  }*/
}

std::vector<RawImage> image_load_bmp(const std::filesystem::path& filename) {
  if (std::ifstream bmp{filename.u8string()}) {
    std::stringstream buffer;
    buffer << bmp.rdbuf();
    return image_decode_bmp(buffer.str());
  }
  return std::vector<RawImage>();
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


std::vector<RawImage> image_decode_bmp(const string& image_data) {
  std::vector<RawImage> imgs;
  
  // Check file size against bitmap header size
  if (image_data.length() < sizeof(BMPFileHeader)) {
    fprintf(stderr, "Junk bitmap of size %lu", image_data.size());
    return imgs;
  }

  const BMPFileHeader &bmp_file = *(BMPFileHeader*) image_data.data();

  // Verify magic number, check header offset sanity.
  if (bmp_file.magic_b != 'B' || bmp_file.magic_m != 'M' ||
      bmp_file.dataStart + sizeof(BMPInfoHeader) > image_data.length()) {
    fprintf(stderr, "Junk bitmap of size %lu", image_data.size());
    return imgs;
  }

  const BMPInfoHeader &bmp_info =
      *(BMPInfoHeader*) (image_data.data() + sizeof(BMPFileHeader));

  if(bmp_info.bitsPerPixel != 32 && bmp_info.bitsPerPixel != 24) {
    fprintf(stderr, "No support for %dbpp bitmaps\n", bmp_info.bitsPerPixel);
    return imgs;
  }
  const bool rgba = bmp_info.isRGBA();
  const bool argb = bmp_info.isARGB();
  if (bmp_info.bitsPerPixel == 32 && !rgba && !argb) {
    fprintf(stderr, "No support for mask format (%08X, %08X, %08X, %08X)\n",
            bmp_info.maskRed, bmp_info.maskGreen, bmp_info.maskBlue,
            bmp_info.maskAlpha);
    return imgs;
  }
  
  imgs.resize(1);
  imgs[0].w = bmp_info.width;
  imgs[0].h = bmp_info.height;
  
  int sz = imgs[0].w * imgs[0].h * 4;
  imgs[0].pxdata = new unsigned char[sz];
  std::memcpy(imgs[0].pxdata, image_data.data() + bmp_file.dataStart, sz);
  return imgs;
}

int image_save_bmp(const std::filesystem::path& filename, const unsigned char* data, unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, bool flipped) {
  unsigned sz = width * height;
  FILE *bmp = fopen(filename.u8string().c_str(), "wb");
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
