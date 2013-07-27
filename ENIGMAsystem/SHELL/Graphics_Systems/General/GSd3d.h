/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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

#ifndef ENIGMA_GL3D3D_H
#define ENIGMA_GL3D3D_H

#include <string>

namespace enigma {
    void d3d_light_update_positions();
}

// ***** RENDER STATE MODE CONSTANTS *****
namespace enigma_user {
enum {
  rs_fill,
  rs_line,
  rs_point,
  rs_front,
  rs_back,
  rs_front_back,
  rs_clockwise,
  rs_counterclockwise,
  rs_nicest,
  rs_fastest,
  rs_dontcare,
  rs_exp,
  rs_exp2,
  rs_linear,
  rs_never, 	// Always False
  rs_less, 	// source Z < depth Z
  rs_equal,	// source Z = depth Z
  rs_lequal, 	// source Z <= depth Z
  rs_greater,	// source Z > depth Z
  rs_notequal, 	// source Z != depth Z
  rs_gequal, 	// source Z >= depth Z
  rs_always     // Always True
};
}

extern double projection_matrix[16];
extern double transformation_matrix[16];

namespace enigma_user
{

/** Starts 3D mode.
**/
void d3d_start();
/** Ends 3d mode.
**/
void d3d_end();
/** Gets whether 3D mode is enabled.
@return This function returns true if 3D mode is enabled, and false if it is not.
**/
bool d3d_get_mode();
/** Sets the use of a perspective projection.
@param enable whether or not to enable perspective
**/
void d3d_set_perspective(bool enable);
/** Sets whether hidden surface removal is enabled.
@param enable whether or not to enable hidden surface removal
**/
void d3d_set_hidden(bool enable);
/** Sets whether depth buffer writing is enabled.
@param enable whether or not to enable depth writing
**/
void d3d_set_zwriteenable(bool enable);
/** Sets whether lighting is enabled.
@param enable whether or not to enable lighting
**/
void d3d_set_lighting(bool enable);

void d3d_set_culling(bool enable);
void d3d_set_culling_mode(int mode);
void d3d_set_culling_orientation(int mode);
void d3d_set_render_mode(int face, int fill);
void d3d_set_line_width(float value);
void d3d_set_point_size(float value);
void d3d_depth_operator(int mode);
void d3d_depth_clear();
void d3d_depth_clear_value(float value);
void d3d_set_fog(bool enable, int color, double start, double end);
void d3d_set_fog_enabled(bool enable);
void d3d_set_fog_hint(int mode);
void d3d_set_fog_mode(int mode);
void d3d_set_fog_color(int color);
void d3d_set_fog_start(double start);
void d3d_set_fog_end(double end);
void d3d_set_fog_density(double density);
void d3d_set_depth(double dep);
void d3d_set_shading(bool smooth);

void d3d_set_projection(float xfrom, float yfrom, float zfrom,float xto, float yto, float zto,float xup, float yup, float zup);
void d3d_set_projection_ext(float xfrom, float yfrom, float zfrom,float xto, float yto, float zto,float xup, float yup, float zup,double angle,double aspect,double znear,double zfar);
void d3d_set_projection_ortho(float x, float y, float width, float height, double angle);
void d3d_set_projection_perspective(float x, float y, float width, float height, double angle);

void d3d_draw_wall(float x1, float y1, float z1, float x2, float y2, float z2, int texId, float hrep, float vrep);
void d3d_draw_floor(float x1, float y1, float z1, float x2, float y2, float z2, int texId, float hrep, float vrep);
void d3d_draw_block(float x1, float y1, float z1, float x2, float y2, float z2, int texId, float hrep, float vrep, bool closed = true);
void d3d_draw_cylinder(float x1, float y1, float z1, float x2, float y2, float z2, int texid, float hrep, float vrep, bool closed, int steps);
void d3d_draw_cone(float x1, float y1, float z1, float x2, float y2, float z2, int texid, float hrep, float vrep, bool closed, int steps);
void d3d_draw_ellipsoid(float x1, float y1, float z1, float x2, float y2, float z2, int texid, float hrep, float vrep, int steps);
void d3d_draw_icosahedron(int texid);
void d3d_draw_torus(float x1, float y1, float z1, int texId, float hrep, float vrep, int csteps, int tsteps, float radius, float tradius, float TWOPI = 2*3.14);

// ***** TRANSFORMATIONS BEGIN *****
void d3d_transform_set_identity();
void d3d_transform_add_translation(float xt,float yt,float zt);
void d3d_transform_add_scaling(float xs,float ys,float zs);
void d3d_transform_add_rotation_x(double angle);
void d3d_transform_add_rotation_y(double angle);
void d3d_transform_add_rotation_z(double angle);
void d3d_transform_add_rotation_axis(float x, float y, float z, double angle);
void d3d_transform_set_translation(float xt,float yt,float zt);
void d3d_transform_set_scaling(float xs,float ys,float zs);
void d3d_transform_set_rotation_x(double angle);
void d3d_transform_set_rotation_y(double angle);
void d3d_transform_set_rotation_z(double angle);
void d3d_transform_set_rotation_axis(float x, float y, float z, double angle);
bool d3d_transform_stack_push();
bool d3d_transform_stack_pop();
void d3d_transform_stack_clear();
bool d3d_transform_stack_empty();
bool d3d_transform_stack_top();
bool d3d_transform_stack_disgard();
// ***** TRANSFORMATIONS END *****

// ***** LIGHTS BEGIN *****
bool d3d_light_define_direction(int id, float dx, float dy, float dz, int col);
bool d3d_light_define_point(int id, float x, float y, float z, double range, int col);
bool d3d_light_define_specularity(int id, int r, int g, int b, double a);
void d3d_light_specularity(int facemode, int r, int g, int b, double a);
void d3d_light_shininess(int facemode, int shine);
void d3d_light_define_ambient(int col);
bool d3d_light_enable(int id, bool enable);
// ***** LIGHTS END *****

}

#endif // ENIGMA_GL3D3D_H
