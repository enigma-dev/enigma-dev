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

#ifndef _B3D_WORLDS__H
#define _B3D_WORLDS__H

namespace enigma_user {

int b3d_world_create_softrigid();
int b3d_world_create_discrete();
int b3d_world_create_simple();
void b3d_world_delete(int id);
int b3d_world_get_type(int id);
void b3d_world_set_gravity(int id, double gx, double gy, double gz);
void b3d_world_add_bodyr(int id, int rigidbody);
void b3d_world_add_bodys(int id, int softbody);
void b3d_world_remove_bodyr(int id, int rigidbody);
void b3d_world_remove_bodys(int id, int softbody);
void b3d_world_update(int id, double timestep, double iterations); 

}
#endif
