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

#include "texture_atlas.h"
#include "Universal_System/spritestruct.h"
#include "Universal_System/nlpo2.h"
#include "Graphics_Systems/graphics_mandatory.h"

#ifdef DEBUG_MODE
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_sprite(spr,id) \
    if (id < -1 or size_t(id) > enigma::sprite_idmax or !enigma::spritestructarray[id]) { \
      show_error("Cannot access sprite with id " + toString(id), false); \
      return; \
    } enigma::sprite *const spr = enigma::spritestructarray[id];
#else
  #define get_sprite(spr,id) \
    enigma::sprite *const spr = enigma::spritestructarray[id];
#endif

namespace enigma {
  unordered_map<unsigned int, texture_atlas> texture_atlas_array;
  size_t texture_atlas_idmax = 0;
}

namespace enigma_user {
  int texture_atlas_create(int w, int h){
    size_t id = enigma::texture_atlas_idmax;

    bool found_empty = false;
    for (unsigned int i=0; i<enigma::texture_atlas_idmax; ++i){ //Find first empty slot
      if (enigma::texture_atlas_array.find(i) == enigma::texture_atlas_array.end()){
        id = i;
        found_empty = true;
        break;
      }
    }
    if (found_empty == false){ enigma::texture_atlas_idmax++; }

    unsigned int fullwidth = nlpo2dc(w)+1, fullheight = nlpo2dc(h)+1; //We only take power of two

    enigma::texture_atlas_array.emplace(id,enigma::texture_atlas());
    enigma::texture_atlas_array[id].id = id;
    enigma::texture_atlas_array[id].width = fullwidth;
    enigma::texture_atlas_array[id].height = fullheight;
    enigma::texture_atlas_array[id].texture = enigma::graphics_create_texture(fullwidth, fullheight, fullwidth, fullheight, nullptr, false);
    
    return id;
  }
  
  //Manually add sprite at position (NOT RECOMMENDED)
  void texture_atlas_add_sprite_position(int tp, int sprid, int subimg, int x, int y){
    get_sprite(spr, sprid);
    enigma::graphics_copy_texture(spr->texturearray[subimg], enigma::texture_atlas_array[tp].texture, x, y);
    enigma::graphics_delete_texture(spr->texturearray[subimg]);
    spr->texturearray[subimg] = enigma::texture_atlas_array[tp].texture;
    spr->texturexarray[subimg] = (double)x/(double)(enigma::texture_atlas_array[tp].width);
    spr->textureyarray[subimg] = (double)y/(double)(enigma::texture_atlas_array[tp].height);
    spr->texturewarray[subimg] = (double)spr->width/(double)(enigma::texture_atlas_array[tp].width);
    spr->textureharray[subimg] = (double)spr->height/(double)(enigma::texture_atlas_array[tp].height);
  }
}