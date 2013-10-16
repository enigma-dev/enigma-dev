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

#ifndef ENIGMA_GS3D3D_H
#define ENIGMA_GS3D3D_H 

#include "Universal_System/scalar.h"
#include <string>

namespace enigma {
    void d3d_light_update_positions();
}

// ***** RENDER STATE CONSTANTS *****
namespace enigma_user {
enum {
  rs_front,
  rs_back,
  rs_front_back,
  
  rs_nicest,
  rs_fastest,
  rs_dontcare,
};

enum {
  rs_exp,    // D3DFOG_EXP
  rs_exp2,   // D3DFOG_EXP2
  rs_linear, // D3DFOG_LINEAR
};

enum {
  rs_never, 	// Always False            D3DCMP_NEVER            GL_NEVER
  rs_less, 	    // source Z < depth Z      D3DCMP_LESS             GL_LESS
  rs_equal,	    // source Z = depth Z      D3DCMP_EQUAL            GL_EQUAL
  rs_lequal, 	// source Z <= depth Z     D3DCMP_LESSEQUAL        GL_LEQUAL
  rs_greater,	// source Z > depth Z      D3DCMP_GREATER          GL_GREATER
  rs_notequal, 	// source Z != depth Z     D3DCMP_NOTEQUAL         GL_NOTEQUAL
  rs_gequal, 	// source Z >= depth Z     D3DCMP_GREATEREQUAL     GL_GEQUAL
  rs_always     // Always True             D3DCMP_ALWAYS           GL_ALWAYS
};

// NOTE: Game Maker uses clockwise culling to define the front face, 
// OpenGL's mode sets what defines the front face, Direct3D's mode sets what defines the back face
enum {
  rs_none, // No culling 
  rs_cw, // Clockwise culling
  rs_ccw // Counter-clockwise culling
};

enum {
  rs_point, // Render vertices as points
  rs_line,  // Render in wireframe mode
  rs_solid  // Normal render mode
};

}

extern double projection_matrix[16];
extern double transformation_matrix[16];

namespace enigma_user
{

void d3d_depth_clear();
void d3d_depth_clear_value(float value);
void d3d_start();
void d3d_end();
bool d3d_get_mode();
void d3d_set_perspective(bool enable);
void d3d_set_hidden(bool enable);
void d3d_set_zwriteenable(bool enable);
void d3d_set_lighting(bool enable);

void d3d_set_culling(int mode);
int d3d_get_culling();
void d3d_set_fill_mode(int fill);
void d3d_set_line_width(float value);
void d3d_set_point_size(float value);
void d3d_set_depth_operator(int mode);
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

void d3d_set_projection(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto,gs_scalar xup, gs_scalar yup, gs_scalar zup);
void d3d_set_projection_ext(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup, double angle, double aspect, double znear, double zfar);
void d3d_set_projection_ortho(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, double angle);
void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, double angle);

void d3d_draw_wall(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep);
void d3d_draw_floor(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep);
void d3d_draw_block(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed = true);
void d3d_draw_cylinder(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texid, gs_scalar hrep, gs_scalar vrep, bool closed, int steps);
void d3d_draw_cone(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texid, gs_scalar hrep, gs_scalar vrep, bool closed, int steps);
void d3d_draw_ellipsoid(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texid, gs_scalar hrep, gs_scalar vrep, int steps);
void d3d_draw_icosahedron(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, int steps);
void d3d_draw_torus(gs_scalar x1, gs_scalar y1, gs_scalar z1, int texId, gs_scalar hrep, gs_scalar vrep, int csteps, int tsteps, float radius, float tradius);

// ***** TRANSFORMATIONS BEGIN *****
void d3d_transform_set_identity();
void d3d_transform_add_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt);
void d3d_transform_add_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs);
void d3d_transform_add_rotation_x(double angle);
void d3d_transform_add_rotation_y(double angle);
void d3d_transform_add_rotation_z(double angle);
void d3d_transform_add_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, double angle);
void d3d_transform_set_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt);
void d3d_transform_set_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs);
void d3d_transform_set_rotation_x(double angle);
void d3d_transform_set_rotation_y(double angle);
void d3d_transform_set_rotation_z(double angle);
void d3d_transform_set_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, double angle);
bool d3d_transform_stack_push();
bool d3d_transform_stack_pop();
void d3d_transform_stack_clear();
bool d3d_transform_stack_empty();
bool d3d_transform_stack_top();
bool d3d_transform_stack_disgard();
// ***** TRANSFORMATIONS END *****

// ***** LIGHTS BEGIN *****
bool d3d_light_define_direction(int id, gs_scalar dx, gs_scalar dy, gs_scalar dz, int col);
bool d3d_light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col);
bool d3d_light_define_specularity(int id, int r, int g, int b, double a);
void d3d_light_specularity(int facemode, int r, int g, int b, double a);
void d3d_light_shininess(int facemode, int shine);
void d3d_light_define_ambient(int col);
bool d3d_light_enable(int id, bool enable);
// ***** LIGHTS END *****

}

#endif // ENIGMA_GL3D3D_H 
