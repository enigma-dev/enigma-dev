/** Copyright (C) 2008-2013 Josh Ventura, Polygone
*** Copyright (C) 2013,2019 Robert B. Colton
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

#include "GSd3d.h"

namespace enigma {

bool d3dMode=false, d3dPerspective=false, d3dHidden=false, d3dZWriteEnable=true;
int d3dCulling = enigma_user::rs_none;

} // namespace enigma

namespace enigma_user {

void d3d_set_perspective(bool enable) {
  // in GM8.1 and GMS v1.4 this does not take effect
  // until the next frame in screen_redraw
  enigma::d3dPerspective = enable;
}

} // namespace enigma_user
