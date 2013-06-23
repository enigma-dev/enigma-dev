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

int b3d_shape_create();
void b3d_shape_delete(int id);

// These functions create the shape and build it all in one call, only for basic shapes
int b3d_shape_create_box(double halfwidth, double halflength, double halfheight);
int b3d_shape_create_cylinder(double halfwidth, double halflength, double halfheight);
int b3d_shape_create_capsule(double radius, double height);
int b3d_shape_create_cone(double radius, double height);
int b3d_shape_create_sphere(double radius);
int b3d_shape_create_plane_static(double nx, double ny, double nz, double constant);

// These functions build the already created shape or else rebuild it into a new shape, if it was already built
void b3d_shape_box(int id, double halfwidth, double halflength, double halfheight);
void b3d_shape_cylinder(int id, double halfwidth, double halflength, double halfheight);
void b3d_shape_capsule(int id, double radius, double height);
void b3d_shape_cone(int id, double radius, double height);
void b3d_shape_sphere(int id, double radius);
void b3d_shape_plane_static(int id, double nx, double ny, double nz, double constant);

void b3d_shape_calculate_local_inertia(int id, double mass, double ix, double iy, double iz);
double b3d_shape_get_scale_x(int id);
double b3d_shape_get_scale_y(int id);
double b3d_shape_get_scale_z(int id);
double b3d_shape_get_margin(int id);
void b3d_shape_set_margin(int id, double margin);
void b3d_shape_set_scale(int id, double sx, double sy, double sz);

}
#endif
