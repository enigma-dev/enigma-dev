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

#ifndef ENIGMA_SPRITES_H
#define ENIGMA_SPRITES_H

#include "Universal_System/scalar.h"
#include "Universal_System/var4.h"

#include <string>

namespace enigma_user {

int sprite_get_width(int sprid);
int sprite_get_height(int sprid);
gs_scalar sprite_get_texture_width_factor(int sprid, int subimg);
gs_scalar sprite_get_texture_height_factor(int sprid, int subimg);

int sprite_get_bbox_bottom(int sprid);
int sprite_get_bbox_left(int sprid);
int sprite_get_bbox_right(int sprid);
int sprite_get_bbox_top(int sprid);
int sprite_get_bbox_mode(int sprid);
int sprite_get_bbox_bottom_relative(int sprid);
int sprite_get_bbox_left_relative(int sprid);
int sprite_get_bbox_right_relative(int sprid);
int sprite_get_bbox_top_relative(int sprid);

int sprite_get_number(int sprid);
int sprite_get_texture(int sprid, int subimage);
int sprite_get_xoffset(int sprid);
int sprite_get_yoffset(int sprid);

int sprite_add(std::string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload,
               int x_offset, int y_offset, bool mipmap = false);  //GM8+ compatible
int sprite_add(std::string filename, int imgnumb, bool transparent, bool smooth, int x_offset, int y_offset,
               bool mipmap = false);  //GM7+ compatible
bool sprite_replace(int ind, std::string fname, int imgnumb, bool precise, bool transparent, bool smooth, bool preload,
                    int x_offset, int y_offset, bool free_texture = true, bool mipmap = false);  //GM8+ compatible
bool sprite_replace(int ind, std::string fname, int imgnumb, bool transparent, bool smooth, int x_offset, int y_offset,
                    bool free_texture = true, bool mipmap = false);  //GM7+ compatible
bool sprite_exists(int spr);
void sprite_save(int ind, unsigned subimg, std::string fname);
//void sprite_save_strip(int ind, std::string fname); //FIXME: We don't support this yet
void sprite_delete(int ind, bool free_texture = true);
int sprite_duplicate(int ind);
void sprite_assign(int ind, int copy_sprite, bool free_texture = true);
void sprite_merge(int ind, int copy_sprite);
void sprite_set_offset(int ind, int xoff, int yoff);
void sprite_set_alpha_from_sprite(int ind, int copy_sprite, bool free_texture = true);
void sprite_set_bbox_mode(int ind, int mode); //FIXME: This sets the mode but the mode is currently meaningless
void sprite_set_bbox(int ind, int left, int top, int right, int bottom);
//void sprite_set_precise(int ind, bool precise); //FIXME: We don't support this yet
void sprite_collision_mask(int ind, bool sepmasks, int mode, int left, int top, int right, int bottom, int kind,
                           unsigned char tolerance); //FIXME: This only updates the bbox currently
var sprite_get_uvs(int ind, int subimg);

// Used for testing
int sprite_create_color(unsigned w, unsigned h, int col); 
bool sprite_textures_equal(int id1, int subimg1, int id2, int subimg2);
uint32_t sprite_get_pixel(int id, int subimg, unsigned x, unsigned y);

// Prototype function for Polygons
int sprite_attach_polygon(int spr_id, int poly_id);

}  //namespace enigma_user

#endif  //ENIGMA_SPRITES_H
