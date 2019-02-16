/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2013 Ssss
*** Copyright (C) 2013-2014, 2019 Robert B. Colton
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

#include "libpng-ext.h"
#include "Universal_System/image_formats.h"
#include "Universal_System/nlpo2.h"

#include "libpng-util.h"

using std::string;

namespace enigma {

unsigned char* image_load_png(string filename, unsigned int* width, unsigned int* height, unsigned int* fullwidth, unsigned int* fullheight, bool flipped) {
  unsigned error;
  unsigned char* image = nullptr;
  unsigned pngwidth, pngheight;

  error = libpng_decode32_file(&image, &pngwidth, &pngheight, filename.c_str());
  if (error) {
    printf("libpng-util error %u\n", error);
    return NULL;
  }

  unsigned
    widfull = nlpo2dc(pngwidth) + 1,
    hgtfull = nlpo2dc(pngheight) + 1,
    ih,iw;
  const int bitmap_size = widfull*hgtfull*4;
  unsigned char* bitmap = new unsigned char[bitmap_size](); // Initialize to zero.

  for (ih = 0; ih < pngheight; ih++) {
    unsigned tmp = 0;
    if (!flipped) {
      tmp = ih*widfull*4;
    } else {
      tmp = (pngheight - 1 - ih)*widfull*4;
    }
    for (iw = 0; iw < pngwidth; iw++) {
      bitmap[tmp+0] = image[4*pngwidth*ih+iw*4+2];
      bitmap[tmp+1] = image[4*pngwidth*ih+iw*4+1];
      bitmap[tmp+2] = image[4*pngwidth*ih+iw*4+0];
      bitmap[tmp+3] = image[4*pngwidth*ih+iw*4+3];
      tmp+=4;
    }
  }

  delete[] image;
  *width  = pngwidth;
  *height = pngheight;
  *fullwidth  = widfull;
  *fullheight = hgtfull;
  return bitmap;
}

int image_save_png(string filename, const unsigned char* data, unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, bool flipped)
{
  //TODO: Use width/height instead of full size, lodepng didn't support this apparently
  //TODO: lodepng didn't let us specify if our image data is flipped
  //TODO: lodepng didn't support BGRA
  unsigned bytes = 4;

  unsigned char* bitmap = new unsigned char[width*height*bytes]();

  for (unsigned i = 0; i < height; i++) {
    unsigned tmp = i;
    unsigned bmp = i;
    if (!flipped) {
      tmp = height - 1 - tmp;
      bmp = height - 1 - bmp;
    }
    tmp *= bytes * fullwidth;
    bmp *= bytes * width;
    for (unsigned ii = 0; ii < width*bytes; ii += bytes) {
      bitmap[bmp + ii + 2] = data[tmp + ii + 0];
      bitmap[bmp + ii + 1] = data[tmp + ii + 1];
      bitmap[bmp + ii + 0] = data[tmp + ii + 2];
      bitmap[bmp + ii + 3] = data[tmp + ii + 3];
    }
  }

  unsigned error = libpng_encode32_file(bitmap, width, height, filename.c_str());

  delete[] bitmap;

  if (error) return -1; else return 1;
}

void extension_libpng_init() {
  image_add_loader(".png", image_load_png);
  image_add_saver(".png", image_save_png);
}

} // namespace enigma
