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

#include <png.h>
#include <stdlib.h> // for malloc/free

/* structure to store PNG image bytes */
struct PngEncodeState
{
  png_bytep buffer;
  size_t size;
  png_structp png;
  png_infop info;
};

void PngWriteCallback(png_structp png_ptr, png_bytep data, png_size_t length) {
  PngEncodeState* p = (PngEncodeState*)png_get_io_ptr(png_ptr);
  size_t nsize = p->size + length;

  /* allocate or grow buffer */
  if (p->buffer)
    p->buffer = (png_bytep)realloc(p->buffer, nsize);
  else
    p->buffer = (png_bytep)malloc(nsize);

  if (!p->buffer)
    png_error(png_ptr, "png write error: could not malloc or realloc output buffer");

  /* copy new bytes to end of buffer */
  memcpy(p->buffer + p->size, data, length);
  p->size += length;
}

PngEncodeState PngEncodeStart(size_t w, size_t h) {
  PngEncodeState enc;
  enc.buffer = NULL;
  enc.size = 0;

  enc.png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!enc.png) {
    printf("png write error: failed to create png write struct");
    return enc;
  }
  enc.info = png_create_info_struct(enc.png);
  if (!enc.info) {
    printf("png write error: failed to create png info struct");
    png_destroy_write_struct(&enc.png, NULL);
    return enc;
  }

  png_set_IHDR(enc.png, enc.info, w, h, 8,
               PNG_COLOR_TYPE_RGBA,
               PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);
  png_set_write_fn(enc.png, &enc, PngWriteCallback, NULL);
  png_write_info(enc.png, enc.info);

  return enc;
}

void PngEncodeEnd(PngEncodeState enc) {
  png_destroy_write_struct(&enc.png, &enc.info);
}

unsigned libpng_encode32(unsigned char** out, size_t* outsize, const unsigned char* image, unsigned w, unsigned h) {
  PngEncodeState enc = PngEncodeStart(w, h);
  for (unsigned i = 0; i < h; ++i) {
    png_write_row(enc.png, (png_bytep)&image[sizeof(png_byte) * 4 * w * i]);
  }
  PngEncodeEnd(enc);
  (*out) = (unsigned char*)enc.buffer;
  (*outsize) = enc.size;
  return 0;
}

unsigned libpng_decode32_file(unsigned char** out, unsigned* w, unsigned* h, const char* filename) {
  (*w) = 0; (*h) = 0;
  FILE *fp = fopen(filename, "rb");

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) return -1;
  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_write_struct(&png, NULL);
    return -2;
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

  png_read_update_info(png, info);

  png_bytep image;
  size_t pitch = sizeof(png_byte) * 4 * (*w); // number of bytes in a row
  image = (png_bytep)malloc(pitch * (*h));

  for (size_t y = 0; y < (*h); y++) {
    png_read_row(png, (png_bytep)&image[pitch * y], NULL);
  }

  png_destroy_read_struct(&png, &info, NULL);
  fclose(fp);

  (*out) = image;

  return 0;
}

std::string libpng_error_text(unsigned error) {
  return "";
}
