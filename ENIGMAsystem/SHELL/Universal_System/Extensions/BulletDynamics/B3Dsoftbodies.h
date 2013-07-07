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

#ifndef _B3D_SOFTBODIES__H
#define _B3D_SOFTBODIES__H

namespace enigma_user {

int b3d_bodys_create(int sid, double mass = 0, double ix = 0, double iy = 0, double iz = 0);
void b3d_bodys_delete(int id);
double b3d_bodys_get_x(int id);
double b3d_bodys_get_y(int id);
double b3d_bodys_get_z(int id);
double b3d_bodys_get_rot_x(int id);
double b3d_bodys_get_rot_y(int id);
double b3d_bodys_get_rot_z(int id);
double b3d_bodys_get_quat_x(int id);
double b3d_bodys_get_quat_y(int id);
double b3d_bodys_get_quat_z(int id);
double b3d_bodys_get_quat_w(int id);
void b3d_bodys_set_position(int id, double x, double y, double z);
void b3d_bodys_set_rotation(int id, double yaw, double pitch, double roll);
void b3d_bodys_set_quaternion(int id, double qx, double qy, double qz, double qw);
void b3d_bodys_set_massdata(int id, double mass, double ix, double iy, double iz);

}
#endif
