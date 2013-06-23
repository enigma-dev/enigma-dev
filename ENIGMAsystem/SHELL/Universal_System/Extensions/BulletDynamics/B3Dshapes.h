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

#ifndef _B3DSHAPES__H
#define _B3DSHAPES__H

namespace enigma_user {

int b3d_shape_create_plane();
int b3d_shape_create_sphere(double radius);
int b3d_shape_create_plane_static(double nx, double ny, double nz, double constant);
int b3d_shape_create_sphere_static();
void b3d_shape_calculate_local_inertia(int id, double mass, double ix, double iy, double iz);
void b3d_shape_delete(int id);

}
#endif
