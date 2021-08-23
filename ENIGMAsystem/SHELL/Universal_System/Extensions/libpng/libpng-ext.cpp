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
#include "Universal_System/image_formats_exts.h"
#include "Universal_System/nlpo2.h"
#include "Widget_Systems/widgets_mandatory.h"

#include "libpng-util.h"

using std::string;

namespace enigma {

std::vector<RawImage> image_load_png(const std::string& filename) {
  unsigned error;
  std::vector<RawImage> imgs(1);
  
  error = libpng_decode32_file(&imgs[0].pxdata, &imgs[0].w, &imgs[0].h, filename.c_str(), true);
  if (error) {
    DEBUG_MESSAGE("libpng-util error " + std::to_string(error), MESSAGE_TYPE::M_ERROR);
    return std::vector<RawImage>();
  }

  return imgs;
}

int image_save_png(const std::string& filename, const unsigned char* data, unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, bool flipped)
{
  // FIXME: the save functions should take const RawImages too instead of unsigned char*
  RawImage in(const_cast<unsigned char*>(data), fullwidth, fullheight);
  RawImage img = image_crop(in, width, height);
  
  // Don't delete data
  in.pxdata = nullptr;
  
  if (flipped) { 
    image_flip(img);
  }
  
  unsigned error = libpng_encode32_file(img.pxdata, width, height, filename.c_str(), false);
  
  
  if (error) return -1; else return 1;
}

void extension_libpng_init() {
  image_add_loader(".png", image_load_png);
  image_add_saver(".png", image_save_png);
}

} // namespace enigma
