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

#ifndef ENIGMA_TEXTUREATLAS
#define ENIGMA_TEXTUREATLAS

#include <vector>
#include <unordered_map>
using std::unordered_map;
using std::vector;

namespace enigma
{
  struct texture_element
  {
    int id, type; //type 0 - sprite, 1 - background, 2 - font
    texture_element(int id, int type) : id(id), type(type) {};
  };

  struct texture_atlas
  {
    int width,height,texture;
    vector<texture_element> textures;
  };

  extern unordered_map<unsigned int, texture_atlas> texture_atlas_array;
  bool textures_pack(int ta, bool free_textures = true);
}

namespace enigma_user
{
  int texture_atlas_create(int w=-1, int h=-1);
  //void texture_atlas_add_sprite(int tp, int spr);
  void texture_atlas_add_sprite_position(int tp, int spr, int subimg, int x, int y, bool free_texture = true);

  inline void texture_atlas_pack_begin(int tp){ enigma::texture_atlas_array[tp].textures.clear(); }
  inline void texture_atlas_pack_sprite(int tp, int spr){ enigma::texture_atlas_array[tp].textures.emplace_back(spr,0); }
  inline void texture_atlas_pack_background(int tp, int bkg){ enigma::texture_atlas_array[tp].textures.emplace_back(bkg,1); }
  inline void texture_atlas_pack_font(int tp, int fnt){ enigma::texture_atlas_array[tp].textures.emplace_back(fnt,2); }
  inline bool texture_atlas_pack_end(int tp) { return enigma::textures_pack(tp); }
}

#endif
