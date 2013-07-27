/** Copyright (C) 2008-2013 polygone
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

#include "Universal_System/scalar.h"

namespace enigma_user
{

int tile_add(int background, int left, int top, int width, int height, int x, int y, int depth, int alpha = 1, int color = 0xFFFFFF);
bool tile_delete(int id);
bool tile_exists(int id);
double tile_get_alpha(int id);
int tile_get_background(int id);
int tile_get_blend(int id);
int tile_get_depth(int id);
int tile_get_height(int id);
int tile_get_left(int id);
int tile_get_top(int id);
double tile_get_visible(int id);
bool tile_get_width(int id);
int tile_get_x(int id);
int tile_get_xscale(int id);
int tile_get_y(int id);
int tile_get_yscale(int id);
bool tile_set_alpha(int id, double alpha);
bool tile_set_background(int id, int background);
bool tile_set_blend(int id, int color);
bool tile_set_position(int id, int x, int y);
bool tile_set_region(int id, int left, int top, int width, int height);
bool tile_set_scale(int id, int xscale, int yscale);
bool tile_set_visible(int id, bool visible);
bool tile_set_depth(int id, int depth);
bool tile_layer_delete(int layer_depth);
bool tile_layer_delete_at(int layer_depth, int x, int y);
bool tile_layer_depth(int layer_depth, int depth);
int tile_layer_find(int layer_depth, int x, int y);
bool tile_layer_hide(int layer_depth);
bool tile_layer_show(int layer_depth);
bool tile_layer_shift(int layer_depth, int x, int y);

}

