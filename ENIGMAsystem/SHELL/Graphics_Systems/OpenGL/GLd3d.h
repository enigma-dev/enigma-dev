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

extern bool d3dMode;
extern double projection_matrix[16];
extern double transformation_matrix[16];

void d3d_start();
void d3d_end();
bool d3d_get_mode();
void d3d_set_perspective(bool enable);
void d3d_set_hidden(bool enable);
void d3d_set_zwriteenable(bool enable);
void d3d_set_lighting(bool enable);

void d3d_set_texturing(bool enable);
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

void d3d_primitive_begin(int kind);
void d3d_primitive_begin_texture(int kind, int texId);
void d3d_primitive_end();
void d3d_vertex(double x, double y, double z);
void d3d_vertex_color(double x, double y, double z, int color, double alpha);
void d3d_vertex_texture(double x, double y, double z, double tx, double ty);
void d3d_vertex_texture_color(double x, double y, double z, double tx, double ty, int color, double alpha);
void d3d_vertex_normal(double x, double y, double z, double nx, double ny, double nz);
void d3d_vertex_normal_color(double x, double y, double z, double nx, double ny, double nz, int color, double alpha);
void d3d_vertex_normal_texture(double x, double y, double z, double nx, double ny, double nz, double tx, double ty);
void d3d_vertex_normal_texture_color(double x, double y, double z, double nx, double ny, double nz, double tx, double ty, int color, double alpha);
void d3d_set_projection(double xfrom,double yfrom,double zfrom,double xto,double yto,double zto,double xup,double yup,double zup);
void d3d_set_projection_ext(double xfrom,double yfrom,double zfrom,double xto,double yto,double zto,double xup,double yup,double zup,double angle,double aspect,double znear,double zfar);
void d3d_set_projection_ortho(double x, double y, double width, double height, double angle);
void d3d_set_projection_perspective(double x, double y, double width, double height, double angle);

void d3d_draw_wall(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep);
void d3d_draw_floor(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep);
void d3d_draw_block(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep, bool closed = true);
void d3d_draw_cylinder(double x1, double y1, double z1, double x2, double y2, double z2, int texid, int hrep, int vrep, bool closed, int steps);
void d3d_draw_cone(double x1, double y1, double z1, double x2, double y2, double z2, int texid, double hrep, double vrep, bool closed, int steps);
void d3d_draw_ellipsoid(double x1, double y1, double z1, double x2, double y2, double z2, int texid, double hrep, double vrep, int steps);
void d3d_draw_icosahedron();
void d3d_draw_torus(double x1, double y1, double z1, int texId, int hrep, int vrep, int csteps, int tsteps, double radius, double tradius, double TWOPI = 2*3.14);

void d3d_transform_set_identity();
void d3d_transform_add_translation(double xt,double yt,double zt);
void d3d_transform_add_scaling(double xs,double ys,double zs);
void d3d_transform_add_rotation_x(double angle);
void d3d_transform_add_rotation_y(double angle);
void d3d_transform_add_rotation_z(double angle);
void d3d_transform_add_rotation_axis(double x, double y, double z, double angle);
void d3d_transform_set_translation(double xt,double yt,double zt);
void d3d_transform_set_scaling(double xs,double ys,double zs);
void d3d_transform_set_rotation_x(double angle);
void d3d_transform_set_rotation_y(double angle);
void d3d_transform_set_rotation_z(double angle);
void d3d_transform_set_rotation_axis(double x, double y, double z, double angle);
bool d3d_transform_stack_push();
bool d3d_transform_stack_pop();
void d3d_transform_stack_clear();
bool d3d_transform_stack_empty();
bool d3d_transform_stack_top();
bool d3d_transform_stack_disgard();

bool d3d_light_define_direction(int id, double dx, double dy, double dz, int col);
bool d3d_light_define_point(int id, double x, double y, double z, double range, int col);
bool d3d_light_define_specularity(int id, int r, int g, int b, double a);
void d3d_light_specularity(int facemode, int r, int g, int b, double a);
void d3d_light_shininess(int facemode, int shine);
void d3d_light_define_ambient(int col);
bool d3d_light_enable(int id, bool enable);

namespace enigma {
    void d3d_light_update_positions();
}

#include <string>

unsigned int d3d_model_create();
void d3d_model_destroy(const unsigned int id);
void d3d_model_copy(const unsigned int id, const unsigned int source);
unsigned int d3d_model_duplicate(const unsigned int source);
bool d3d_model_exists(const unsigned int id);
void d3d_model_clear(const unsigned int id);
void d3d_model_save(const unsigned int id, std::string fname);
bool d3d_model_load(const unsigned int id, std::string fname);
void d3d_model_draw(const unsigned int id, double x, double y, double z, int texId);
void d3d_model_primitive_begin(const unsigned int id, int kind);
void d3d_model_primitive_end(const unsigned int id);
void d3d_model_vertex(const unsigned int id, double x, double y, double z);
void d3d_model_vertex_color(const unsigned int id, double x, double y, double z, int col, double alpha);
void d3d_model_vertex_texture(const unsigned int id, double x, double y, double z, double tx, double ty);
void d3d_model_vertex_texture_color(const unsigned int id, double x, double y, double z, double tx, double ty, int col, double alpha);
void d3d_model_vertex_normal(const unsigned int id, double x, double y, double z, double nx, double ny, double nz);
void d3d_model_vertex_normal_color(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, int col, double alpha);
void d3d_model_vertex_normal_texture(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, double tx, double ty);
void d3d_model_vertex_normal_texture_color(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, double tx, double ty, int col, double alpha);
void d3d_model_wall(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep);
void d3d_model_floor(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep);
void d3d_model_block(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed = true);
void d3d_model_cylinder(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed, int steps);
void d3d_model_cone(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed, int steps);
void d3d_model_ellipsoid(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, int steps);
void d3d_model_icosahedron(const unsigned int id);
void d3d_model_torus(const unsigned int id, double x1, double y1, double z1, int hrep, int vrep, int csteps, int tsteps, double radius, double tradius, double TWOPI = 2*3.14);
