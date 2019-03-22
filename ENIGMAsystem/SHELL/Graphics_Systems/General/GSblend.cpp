/** Copyright (C) 2008-2013 Josh Ventura
*** Copyright (C) 2013, 2019 Robert B. Colton
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

namespace enigma {

int currentblendmode[2] = {0,0};
int currentblendtype = 0;

} // namespace enigma

namespace enigma_user {

int draw_get_blend_mode() {
  return enigma::currentblendmode[0];
}

int draw_get_blend_mode_ext(bool src) {
  return enigma::currentblendmode[(src==true?0:1)];
}

int draw_get_blend_mode_type() {
  return enigma::currentblendtype;
}

} // namespace enigma_user
