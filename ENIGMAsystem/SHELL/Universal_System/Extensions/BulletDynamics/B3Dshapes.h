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

#ifndef _B3D_SHAPES__H
#define _B3D_SHAPES__H

namespace enigma_user {

enum {
  b3d_unknown,
  b3d_box,
  b3d_cylinder,
  b3d_capsule,
  b3d_cone,
  b3d_sphere,
  b3d_plane_static,
  b3d_compound,
  b3d_empty
};

int b3d_shape_create();
void b3d_shape_delete(int id);

// These functions create the shape and build it all in one call, only for basic shapes
int b3d_shape_create_box(double halfwidth, double halflength, double halfheight);
int b3d_shape_create_cylinder(double halfwidth, double halflength, double halfheight);
int b3d_shape_create_capsule(double radius, double height);
int b3d_shape_create_cone(double radius, double height);
int b3d_shape_create_sphere(double radius);
int b3d_shape_create_plane_static(double nx, double ny, double nz, double constant);
int b3d_shape_create_compound(bool aabbtree = true);
int b3d_shape_create_empty();

// These functions build the already created shape or else rebuild it into a new shape, if it was already built
void b3d_shape_box(int id, double halfwidth, double halflength, double halfheight);
void b3d_shape_cylinder(int id, double halfwidth, double halflength, double halfheight);
void b3d_shape_capsule(int id, double radius, double height);
void b3d_shape_cone(int id, double radius, double height);
void b3d_shape_sphere(int id, double radius);
void b3d_shape_plane_static(int id, double nx, double ny, double nz, double constant);
void b3d_shape_compound(int id, bool aabbtree = true);
void b3d_shape_empty(int id);

// These functions are special for compound shapes which contain multiple child shapes
// It is important to note, if you intend to keep the shape but not access its children later, you should clear them after 
// calling the shape build function
// If you modify any of the shapes children after it has already been built, you need to call its build function again
void b3d_shape_add_child(int id, int child, double lx, double ly, double lz, double rx, double ry, double rz);
void b3d_shape_add_child(int id, int child, double lx, double ly, double lz, double qx, double qy, double qz, double qw);
int b3d_shape_get_child(int id, int child);
int b3d_shape_get_child_count(int id);
double b3d_shape_get_child_lx(int id, int child);
double b3d_shape_get_child_ly(int id, int child);
double b3d_shape_get_child_lz(int id, int child);
double b3d_shape_get_child_rx(int id, int child);
double b3d_shape_get_child_ry(int id, int child);
double b3d_shape_get_child_rz(int id, int child);
double b3d_shape_get_child_qx(int id, int child);
double b3d_shape_get_child_qy(int id, int child);
double b3d_shape_get_child_qz(int id, int child);
double b3d_shape_get_child_qw(int id, int child);

// These functions are special for shapes which are comprised of vertices, edges, planes, etc.
// It is important to note, if you intend to keep the shape but not modify its vertices later, accessing their coordinates you
// can do, you should clear them after calling the shape build function
// If you modify any of the shapes vertices after it has already been built, you need to call its build function again
void b3d_shape_add_vert(int id, double x, double y, double z);
void b3d_shape_remove_vert(int id, int vert);
double b3d_shape_get_vert_x(int id, int vert);
double b3d_shape_get_vert_y(int id, int vert);
double b3d_shape_get_vert_z(int id, int vert);
double b3d_shape_get_vert_xs(int id, int vert);
double b3d_shape_get_vert_ys(int id, int vert);
double b3d_shape_get_vert_zs(int id, int vert);
void b3d_shape_clear_verts(int id, int start, int end);
void b3d_shape_clear_verts_all(int id);
int b3d_shape_get_vert_count(int id);
int b3d_shape_get_edge_count(int id);
int b3d_shape_get_plane_count(int id);

// These are general functions which apply to all shapes.
void b3d_shape_calculate_local_inertia(int id, double mass, double ix, double iy, double iz);
int b3d_shape_get_type(int id);
double b3d_shape_get_scale_x(int id);
double b3d_shape_get_scale_y(int id);
double b3d_shape_get_scale_z(int id);
double b3d_shape_get_margin(int id);
void b3d_shape_set_margin(int id, double margin);
void b3d_shape_set_scale(int id, double sx, double sy, double sz);

}
#endif
