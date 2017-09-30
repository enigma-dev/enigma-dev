/** Copyright (C) 2013 Robert B. Colton
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

#ifndef ENIGMA_B2D_JOINTS__H
#define ENIGMA_B2D_JOINTS__H

#include <Box2D/Box2D.h>

namespace enigma_user
{

int b2d_joint_create_distance(int world, int bodya, int bodyb,  bool collide_connected = true);
int b2d_joint_create_revolute(int world, int bodya, int bodyb, bool limit, double lower, double upper);
int b2d_joint_create_prismatic(int world);
int b2d_joint_create_pulley(int world);
int b2d_joint_create_gear(int world);
int b2d_joint_create_mouse(int world, int bodya, int bodyb, bool collide, double x, double y);
void b2d_joint_delete(int id);

void b2d_joint_set_target(int id, double x, double y);
void b2d_joint_enable_motor(int id);

}

#endif // ENIGMA_B2D_JOINTS__H

