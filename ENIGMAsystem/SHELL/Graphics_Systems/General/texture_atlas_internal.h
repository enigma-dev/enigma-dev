/** Copyright (C) 2015 Harijs Grinbergs
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

#ifdef INCLUDED_FROM_SHELLMAIN
#error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

#ifndef ENIGMA_TEXTUREATLAS_INTERNAL_H
#define ENIGMA_TEXTUREATLAS_INTERNAL_H

#include <unordered_map>
#include <vector>

namespace enigma {
struct texture_element {
  int id, type;  //type 0 - sprite, 1 - background, 2 - font
  texture_element(int id, int type) : id(id), type(type){};
};

struct texture_atlas {
  int width, height, texture;
  std::vector<texture_element> textures;
};

extern std::unordered_map<unsigned int, texture_atlas> texture_atlas_array;
bool textures_pack(int ta, bool free_textures = true);
}  //namespace enigma

#endif  //ENIGMA_TEXTUREATLAS_INTERNAL_H
