/** Copyright (C) 2018-2019 Robert B. Colton
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

#include <string>

#include "strings_util.h"

#include <png.h>

unsigned libpng_encode32_file(const unsigned char* image, const unsigned w, const unsigned h, const char* filename, bool bgra) {
  #ifdef _WIN32
  std::wstring wstr = widen(filename);
  FILE *fp; errno_t err = _wfopen_s(&fp, wstr.c_str(), L"wb");
  if (err || fp == nullptr) return -1;
  #else
  FILE *fp = fopen(filename, "wb");
  if (fp == nullptr) return -1;
  #endif

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) return -1;
  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_write_struct(&png, NULL);
    return -2;
  }

  png_init_io(png, fp);

  // Write header (8 bit colour depth)
  png_set_IHDR(png, info, w, h,
               8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
               
  if (bgra)
    png_set_bgr(png);

  png_write_info(png, info);

  for (unsigned i = 0; i < h; ++i) {
    png_write_row(png, (png_bytep)&image[sizeof(png_byte) * 4 * w * i]);
  }
  png_write_end(png, NULL);

  fclose(fp);
  png_free_data(png, info, PNG_FREE_ALL, -1);
  png_destroy_write_struct(&png, &info);

  return 0;
}

unsigned libpng_decode32_file(unsigned char** out, unsigned* w, unsigned* h, const char* filename, bool bgra) {
  (*w) = 0; (*h) = 0;
  #ifdef _WIN32
  std::wstring wstr = widen(filename);
  FILE *fp; errno_t err = _wfopen_s(&fp, wstr.c_str(), L"rb");
  if (err || fp == nullptr) return -1;
  #else
  FILE *fp = fopen(filename, "rb");
  if (fp == nullptr) return -1;
  #endif

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) return -2;
  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_read_struct(&png, NULL, NULL);
    return -3;
  }

  png_init_io(png, fp);
  png_read_info(png, info);

  png_byte color_type, bit_depth;
  (*w)       = png_get_image_width(png, info);
  (*h)       = png_get_image_height(png, info);
  color_type = png_get_color_type(png, info);
  bit_depth  = png_get_bit_depth(png, info);

  // Read any color_type into 8bit depth, RGBA format.
  // See http://www.libpng.org/pub/png/libpng-manual.txt

  if (bit_depth == 16)
    png_set_strip_16(png);

  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if (png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // These color_type don't have an alpha channel then fill it with 0xff.
  if (color_type == PNG_COLOR_TYPE_RGB ||
      color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  if (color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);
    
  if (bgra)
    png_set_bgr(png);

  png_read_update_info(png, info);

  png_bytep image;
  size_t pitch = sizeof(png_byte) * 4 * (*w); // number of bytes in a row
  image = new png_byte[pitch * (*h)];

  for (size_t y = 0; y < (*h); y++) {
    png_read_row(png, (png_bytep)&image[pitch * y], NULL);
  }

  png_destroy_read_struct(&png, &info, NULL);
  fclose(fp);

  (*out) = image;

  return 0;
}
