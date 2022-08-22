/** Copyright (C) 2011 Polygone
*** Copyright (C) 2011 Josh Ventura
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

#ifndef ENIGMA_MOVE_FUNCTIONS_H
#define ENIGMA_MOVE_FUNCTIONS_H

#include "scalar.h"

namespace enigma_user {

void motion_set(int dir, cs_scalar newspeed);
void motion_add(cs_scalar newdirection, cs_scalar newspeed);
void move_snap(const cs_scalar hsnap, const cs_scalar vsnap);
void move_wrap(const bool hor, const bool vert, const cs_scalar margin);
void move_towards_point(const cs_scalar point_x, const cs_scalar point_y, const cs_scalar newspeed);
bool place_snapped(int hsnap, int vsnap);
void move_random(const cs_scalar snapHor, const cs_scalar snapVer);

}  //namespace enigma_user

#endif  //ENIGMA_MOVE_FUNCTIONS_H
