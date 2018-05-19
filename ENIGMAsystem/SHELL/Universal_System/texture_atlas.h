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

#ifndef ENIGMA_TEXTUREATLAS_H
#define ENIGMA_TEXTUREATLAS_H

namespace enigma_user {
int texture_atlas_create(int w = -1, int h = -1);
//void texture_atlas_add_sprite(int tp, int spr);
void texture_atlas_add_sprite_position(int tp, int spr, int subimg, int x, int y, bool free_texture = true);
inline void texture_atlas_pack_begin(int tp);
inline void texture_atlas_pack_sprite(int tp, int spr);
inline void texture_atlas_pack_background(int tp, int bkg);
inline void texture_atlas_pack_font(int tp, int fnt);
inline bool texture_atlas_pack_end(int tp);
}  //namespace enigma_user

#endif
