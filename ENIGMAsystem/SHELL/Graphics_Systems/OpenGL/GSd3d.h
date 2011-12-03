/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

extern bool d3dMode;

void d3d_start();
void d3d_end();
void d3d_set_perspective(int enable);
void d3d_set_hidden(int enable);
void d3d_set_lighting(int enable);
void d3d_set_culling(int enable);
void d3d_set_fog(int enable, int color, int start, int end);
void d3d_set_shading(bool smooth);

void d3d_primitive_begin(int kind);
void d3d_vertex(double x, double y, double z);
void d3d_vertex_color(double x, double y, double z, int color, double alpha);
void d3d_primitive_begin_texture(int kind, int texId);
void d3d_vertex_texture(double x, double y, double z, double tx, double ty);
void d3d_vertex_texture_color(double x, double y, double z, double tx, double ty, int color, double alpha);
void d3d_primitive_end();
void d3d_set_projection(double xfrom,double yfrom,double zfrom,double xto,double yto,double zto,double xup,double yup,double zup);
void d3d_set_projection_ext(double xfrom,double yfrom,double zfrom,double xto,double yto,double zto,double xup,double yup,double zup,double angle,double aspect,double znear,double zfar);
void d3d_set_projection_ortho(int x, int y, int width, int height, int angle);

void d3d_draw_wall(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep);
void d3d_draw_floor(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep);
void d3d_draw_block(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep);
void d3d_draw_cylinder(double x1, double y1, double z1, double x2, double y2, double z2, int texid, int hrep, int vrep, bool closed, int steps);
void d3d_draw_cone(double x1, double y1, double z1, double x2, double y2, double z2, int texid, int hrep, int vrep, bool closed, int steps);
void d3d_draw_ellipsoid(double x1, double y1, double z1, double x2, double y2, double z2, int texid, int hrep, int vrep, int steps);

void d3d_transform_set_identity();
void d3d_transform_add_translation(double xt,double yt,double zt);
void d3d_transform_add_scaling(double xs,double ys,double zs);
void d3d_transform_add_rotation_x(double angle);
void d3d_transform_add_rotation_y(double angle);
void d3d_transform_add_rotation_z(double angle);
void d3d_transform_set_translation(double xt,double yt,double zt);
void d3d_transform_set_scaling(double xs,double ys,double zs);
void d3d_transform_set_rotation_x(double angle);
void d3d_transform_set_rotation_y(double angle);
void d3d_transform_set_rotation_z(double angle);
void d3d_transform_stack_push();
void d3d_transform_stack_pop();

#include <string>

unsigned int d3d_model_create();
void d3d_model_destroy(const unsigned int id);
void d3d_model_copy(const unsigned int id, const unsigned int source);
unsigned int d3d_model_duplicate(const unsigned int source);
bool d3d_model_exists(const unsigned int id);
void d3d_model_clear(const unsigned int id);
void d3d_model_save(const unsigned int id, std::string fname);
void d3d_model_load(const unsigned int id, std::string fname);
void d3d_model_draw(const unsigned int id, double x, double y, double z, int texId);
void d3d_model_primitive_begin(const unsigned int id, int kind);
void d3d_model_primitive_end(const unsigned int id);
void d3d_model_vertex(const unsigned int id, double x, double y, double z);
void d3d_model_vertex_color(const unsigned int id, double x, double y, double z, int col, double alpha);
void d3d_model_vertex_texture(const unsigned int id, double x, double y, double z, double tx, double ty);
void d3d_model_vertex_texture_color(const unsigned int id, double x, double y, double z, double tx, double ty, int col, double alpha);
void d3d_model_block(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep);
void d3d_model_cylinder(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed, int steps);
void d3d_model_cone(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed, int steps);
void d3d_model_ellipsoid(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, int steps);
void d3d_model_wall(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep);
void d3d_model_floor(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep);
