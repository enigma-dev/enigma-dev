/** Copyright (C) 2008 Josh Ventura
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

#ifndef ENIGMA_IMAGEFORMATS_H
#define ENIGMA_IMAGEFORMATS_H

#include <vector>
#include <filesystem>

/// NOTE: These image formats expect the data to be un-aligned and always reads and writes with BGRA full color

namespace enigma
{

struct RawImage {
  RawImage(unsigned char* pxdata = nullptr, unsigned w = 0, unsigned h = 0) :
    pxdata(pxdata), w(w), h(h) {}
  ~RawImage() { delete[] pxdata; }
  RawImage(const RawImage&) = delete;
  RawImage(RawImage &&other): pxdata(other.pxdata), w(other.w), h(other.h) {
    other.pxdata = nullptr;
  }
  void resize(unsigned w, unsigned h) {
    delete[] pxdata;
    pxdata = new unsigned char[w*h*4];
    this->w = w;
    this->h = h;
  }
  unsigned char* pxdata;
  unsigned w, h;
};

struct Color {
  uint8_t b, g, r, a;
  uint32_t asInt() { return (b << 24) | (g << 16) | (r << 8) | a;  }
};

/// Color formats
enum {
  color_fmt_rgba,
  color_fmt_rgb,
  color_fmt_bgra,
  color_fmt_bgr
};

Color image_get_pixel_color(const RawImage& in, unsigned x, unsigned y);
void image_swap_color(RawImage& in, Color oldColor, Color newColor);
void image_remove_color(RawImage& in, Color oldColor);
void image_remove_color(RawImage& in);
/// Note splits horizontally
std::vector<RawImage> image_split(const RawImage& in, unsigned imgcount);
RawImage image_pad(const RawImage& in, unsigned newWidth, unsigned newHeight);
RawImage image_crop(const RawImage& in, unsigned newWidth, unsigned newHeight);
unsigned long *bgra_to_argb(unsigned char *bgra_data, unsigned pngwidth, unsigned pngheight, bool prepend_size = false);
unsigned char* mono_to_rgba(unsigned char* pxdata, unsigned width, unsigned height);

/// Reverses the scan-lines from top to bottom or vice verse, this is not actually to be used, you should load and save the data correctly to avoid duplicating it
void image_flip(RawImage& in);

/// Generic all-purpose image loading call that will regexp the filename for the format and call the appropriate function.
std::vector<RawImage> image_load(const std::filesystem::path& filename);
/// Generic all-purpose image saving call that will regexp the filename for the format and call the appropriate function.
int image_save(const std::filesystem::path& filename, const unsigned char* data, unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, bool flipped);

std::vector<RawImage> image_load_bmp(const std::filesystem::path& filename);
std::vector<RawImage> image_decode_bmp(const std::string& image_data);
std::vector<RawImage> image_load_gif(const std::filesystem::path& filename);
int image_save_bmp(const std::filesystem::path&, const unsigned char* data, unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, bool flipped);

} //namespace enigma

#endif //ENIGMA_IMAGEFORMATS_H
