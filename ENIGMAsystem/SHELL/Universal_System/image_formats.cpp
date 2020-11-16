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
#include <iostream>

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

Color image_get_pixel_color(const RawImage& in, unsigned x, unsigned y) {
  Color c;
  size_t index = 4 * (y * in.w + x);
  c.b = in.pxdata[index];
  c.g = in.pxdata[index + 1];
  c.r = in.pxdata[index + 2];
  c.a = in.pxdata[index + 3];
  return c;
}

void image_swap_color(RawImage& in, Color oldColor, Color newColor) {  
  #ifdef DEBUG_MODE
  if (in.pxdata == nullptr) {
    in.pxdata = new unsigned char[in.w * in.h * 4];
    std::fill(in.pxdata, in.pxdata + (in.w * in.h * 4), 255);
    DEBUG_MESSAGE("Attempt to access a null pointer" , MESSAGE_TYPE::M_ERROR);
    return;
  }
  #endif
  
  unsigned int ih, iw;
  for (ih = 0; ih < in.h; ih++) {
    int index = ih * in.w * 4;
    
    for (iw = 0; iw < in.w; iw++) {
      if (
           in.pxdata[index]     == oldColor.b
        && in.pxdata[index + 1] == oldColor.g
        && in.pxdata[index + 2] == oldColor.r
        && in.pxdata[index + 3] == oldColor.a
        ) {
          in.pxdata[index]     = newColor.b;
          in.pxdata[index + 1] = newColor.g;
          in.pxdata[index + 2] = newColor.r;
          in.pxdata[index + 3] = newColor.a;
      }

      index += 4;
    }
  }
}

std::vector<RawImage> image_split(const RawImage& in, unsigned imgcount) {
  std::vector<RawImage> imgs(imgcount);
  unsigned splitWidth = in.w / imgcount;
  
  for (unsigned i = 0; i < imgcount; ++i) {
    
    imgs[i].pxdata = new unsigned char[splitWidth * in.h * 4]();
    imgs[i].w = splitWidth;
    imgs[i].h = in.h;
    
    unsigned ih,iw;
    unsigned xcelloffset = i * splitWidth * 4;
    
    for (ih = 0; ih < in.h; ih++) {
      unsigned tmp = ih * in.w * 4 + xcelloffset;
      unsigned tmpcell = ih * splitWidth * 4;
      
      for (iw = 0; iw < splitWidth; iw++) {
        imgs[i].pxdata[tmpcell]     = in.pxdata[tmp];
        imgs[i].pxdata[tmpcell + 1] = in.pxdata[tmp + 1];
        imgs[i].pxdata[tmpcell + 2] = in.pxdata[tmp + 2];
        imgs[i].pxdata[tmpcell + 3] = in.pxdata[tmp + 3];
        tmp += 4;
        tmpcell += 4;
      }
    }
  }
  
  return imgs;
}

RawImage image_pad(const RawImage& in, unsigned newWidth, unsigned newHeight) {
  RawImage padded;
  padded.w = newWidth;
  padded.h = newHeight;
  
  padded.pxdata = new unsigned char[4 * newWidth * newHeight + 1];
  
  unsigned char* imgpxptr = padded.pxdata;
  const unsigned char* inpxdata = in.pxdata;
  unsigned rowindex, colindex;
  for (rowindex = 0; rowindex < in.h; rowindex++) {
    for (colindex = 0; colindex < in.w; colindex++) {
      *imgpxptr++ = *inpxdata++;
      *imgpxptr++ = *inpxdata++;
      *imgpxptr++ = *inpxdata++;
      *imgpxptr++ = *inpxdata++;
    }
    
    std::memset(imgpxptr, 0, (newWidth - colindex) << 2);
    imgpxptr += (newWidth - colindex) << 2;
  }
  
  std::memset(imgpxptr, 0, (newHeight - in.h) * newWidth);
  
  return padded;
}

RawImage image_crop(const RawImage& in, unsigned newWidth, unsigned newHeight) {
  RawImage img;
  img.w = newWidth;
  img.h = newHeight;
  img.pxdata = new unsigned char[newWidth * newHeight * 4];
  
  unsigned stride = 4;
  for (unsigned i = 0; i < newHeight; i++) {
    unsigned tmp = i;
    unsigned bmp = i;
    tmp *= stride * in.w;
    bmp *= stride * newWidth;
    for (unsigned ii = 0; ii < newWidth*stride; ii += stride) {
      img.pxdata[bmp + ii + 2] = in.pxdata[tmp + ii + 0];
      img.pxdata[bmp + ii + 1] = in.pxdata[tmp + ii + 1];
      img.pxdata[bmp + ii + 0] = in.pxdata[tmp + ii + 2];
      img.pxdata[bmp + ii + 3] = in.pxdata[tmp + ii + 3];
    }
  }
  
  return img;
}

unsigned long *bgra_to_argb(unsigned char *bgra_data, unsigned pngwidth, unsigned pngheight, bool prepend_size) {
  unsigned widfull = nlpo2(pngwidth), hgtfull = nlpo2(pngheight), ih, iw;
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

unsigned char* mono_to_rgba(unsigned char* pxdata, unsigned width, unsigned height) {
  unsigned char* rgba = new unsigned char[width * height * 4];
  for (unsigned i = 0; i < width * height; ++i) {
    unsigned index_out = i * 4;
    rgba[index_out] = rgba[index_out + 1] = rgba[index_out + 2] = 255;
    rgba[index_out + 3] = pxdata[i];
  }
  return rgba;
}

void image_add_loader(const std::filesystem::path& extension, ImageLoadFunction fnc) {
  image_load_handlers[extension] = fnc;
}

void image_add_saver(const std::filesystem::path& extension, ImageSaveFunction fnc) {
  image_save_handlers[extension] = fnc;
}

void image_flip(RawImage& in) {
  //flipped upside down
  unsigned bytes = 4;
  unsigned sz = in.w * in.h * bytes;
  unsigned char* rgbdata = new unsigned char[sz];
  for (unsigned int i = 0; i < in.h; i++) { // Doesn't matter the order now
    memcpy(&rgbdata[i*in.w*bytes*sizeof(unsigned char)],           // address of destination
           &in.pxdata[(in.h-i-1)*in.w*bytes*sizeof(unsigned char)],   // address of source
           in.w*bytes*sizeof(unsigned char) );                     // number of bytes to copy
  }
  
  delete[] in.pxdata;
  in.pxdata = rgbdata;
}

/// Generic all-purpose image loading call that will regexp the filename for the format and call the appropriate function.
std::vector<RawImage> image_load(const std::filesystem::path& filename) {
  std::filesystem::path extension = filename.extension();
  if (extension.empty()) {
    DEBUG_MESSAGE("No extension in image filename: " + filename.u8string() + ". Assumimg .bmp", MESSAGE_TYPE::M_WARNING);
    extension = ".bmp";
  }
  
  auto handler = image_load_handlers.find(ToLower(extension.u8string()));
  if (handler != image_load_handlers.end()) {
    return (*handler).second(filename);
  } else {
    DEBUG_MESSAGE("Unsupported image format extension in image filename: " + filename.u8string() , MESSAGE_TYPE::M_ERROR);
    return std::vector<RawImage>();
  }
}

/// Generic all-purpose image saving call.
int image_save(const std::filesystem::path& filename, const unsigned char* data, unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, bool flipped) {
  std::filesystem::path extension = filename.extension();
  auto handler = image_save_handlers.find(ToLower(extension.u8string()));
  if (extension.empty() || handler != image_save_handlers.end()) {
    return (*handler).second(filename, data, width, height, fullwidth, fullheight, flipped);
  } else {
    DEBUG_MESSAGE("Unsupported image format extension in image filename: " + filename.u8string() + " saving as BMP" , MESSAGE_TYPE::M_WARNING);
    return image_save_bmp(filename, data, width, height, fullwidth, fullheight, flipped);
  }
}

std::vector<RawImage> image_load_bmp(const std::filesystem::path& filename) {
  if (std::ifstream bmp{filename.u8string(), ios::in | ios::binary}) {
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
  bool rgba = bmp_info.isRGBA();
  bool argb = bmp_info.isARGB();
  if (bmp_info.bitsPerPixel == 32 && !rgba && !argb) {
    fprintf(stderr, "No support for mask format (%08X, %08X, %08X, %08X)\n",
            bmp_info.maskRed, bmp_info.maskGreen, bmp_info.maskBlue,
            bmp_info.maskAlpha);
            
    argb = true;
    //return imgs;
  }
  
  unsigned char* bitmap;
  auto &img = imgs.emplace_back();
  img.w = bmp_info.width;
  img.h = bmp_info.height;
  img.pxdata = bitmap = new unsigned char[img.w * img.h * 4](); // Initialize to 0.

  // Calculate the number of nulls that follows each line.
  const int overlap = bmp_info.width * (bmp_info.bitsPerPixel / 8) % 4;
  const int pad = overlap ? 4 - overlap : 0;

  printf("Bitmap pad: %d\n", pad);
  const char *bmp_it = image_data.data() + bmp_file.dataStart;

  for (unsigned ih = 0; ih < bmp_info.height; ih++) {
    unsigned tmp = (bmp_info.height - 1 - ih) * bmp_info.width * 4;
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
      tmp = lastbyte - fullwidth - i;
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
