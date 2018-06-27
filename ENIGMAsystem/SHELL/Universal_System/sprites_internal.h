/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2013 Robert B. Colton
*** Copyright (C) 2015 Harijs Grinbergs
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

//FIXME: this should be ifdef shellmain but enigmas in a sorry state
#ifdef JUST_DEFINE_IT_RUN
#error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

#ifndef ENIGMA_SPRITESTRUCT
#define ENIGMA_SPRITESTRUCT

#include "resource_vec.h"
#include "nlpo2.h"

#include <stdlib.h>
#include <string>
#include <vector>
#include <array>

namespace enigma {
struct bbox_rect_t {
  bbox_rect_t() : left(0), top(0), right(0), bottom(0) {}
  bbox_rect_t(int left, int top, int right, int bottom) : left(left), top(top), right(right), bottom(bottom) {}
  int left, top, right, bottom;
};

struct SubImage {
  SubImage() = default;
  SubImage(const SubImage &copy);
  unsigned textureID = -1;
  double x = 0, y = 0, w = 0, h = 0;
  std::vector<void*> collisionData;
};

struct Sprite {
  Sprite(bool precise = false, bool transparent = false, bool smooth = false, bool preload = false, int xoffset = 0,
         int yoffset = 0, bool mipmap = false)
      : subimages(0) {}

  size_t subcount();
  void freeTextures();
  int width = 0, height = 0, xoffset = 0, yoffset = 0;
  bool smooth = false, preload = false, mipmap = false;
  int bbox_mode = 0;
  std::array<bbox_rect_t, 2> bbox;
  std::vector<SubImage> subimages;
};

int sprite_add(unsigned char *pxdata, unsigned w, unsigned h, int imgnumb, bool precise, bool transparent, bool smooth, bool preload, int xoffset, int yoffset, bool mipmap);
int sprite_set(unsigned char *pxdata, unsigned w, unsigned h, int imgnumb, bool precise, bool transparent, bool smooth, bool preload, int xoffset, int yoffset, bool mipmap);
int sprite_add_subimage(unsigned char *pxdata, unsigned w, unsigned h, int id, int imgCount, bool precise, bool transparent, bool smooth, int xoffset, int yoffset, bool mipmap);
const bbox_rect_t &sprite_get_bbox(int sprid);
const bbox_rect_t &sprite_get_bbox_relative(int sprid);

extern ResourceVec<Sprite> sprites;

}  // namespace enigma

#endif // ENIGMA_SPRITESTRUCT

