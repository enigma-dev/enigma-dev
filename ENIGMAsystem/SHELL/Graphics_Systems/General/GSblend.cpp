/** Copyright (C) 2008-2013 Josh Ventura
*** Copyright (C) 2013, 2018-2019 Robert Colton
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

#include "GSblend.h"
#include "GSstdraw.h"

namespace enigma {

int blendMode[2]={enigma_user::bm_src_alpha,enigma_user::bm_inv_src_alpha};

} // namespace enigma

namespace enigma_user {

void draw_set_blend_mode(int mode) {
  enigma::draw_set_state_dirty();
  const static int dest_modes[] = {bm_inv_src_alpha,bm_one,bm_inv_src_color,bm_inv_src_color};

  enigma::blendMode[0] = (mode == bm_subtract) ? bm_zero : bm_src_alpha;
  enigma::blendMode[1] = dest_modes[mode % 4];
}

void draw_set_blend_mode_ext(int src, int dest) {
  enigma::draw_set_state_dirty();
  enigma::blendMode[0] = src;
  enigma::blendMode[1] = dest;
}

int draw_get_blend_mode_src() { return enigma::blendMode[0]; }
int draw_get_blend_mode_dest() { return enigma::blendMode[1]; }

} // namespace enigma_user
