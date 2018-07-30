/** Copyright (C) 2017 Faissal I. Bensefia
*** Copyright (C) 2008-2013 Robert B. Colton, Adriano Tumminelli
*** Copyright (C) 2014 Seth N. Hetu
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
#ifndef GS_NONE_FILLIN
#define GS_NONE_FILLIN
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>     /* malloc, free, rand */
#include <unordered_map>
#include <math.h>

using namespace std;
#include <algorithm>
#include <floatcomp.h>
#include <vector>
#include <limits>
#include <map>
#include <list>
#include <stack>
#include "Universal_System/estring.h"

namespace enigma
{
	void graphicssystem_initialize();
	void load_tiles();
	void delete_tiles();
	void rebuild_tile_layer(int layer_depth);

	int graphics_create_texture(unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, void* pxdata, bool mipmap);
	int graphics_duplicate_texture(int tex, bool mipmap);
	void graphics_copy_texture(int source, int destination, int x, int y);
	void graphics_copy_texture_part(int source, int destination, int xoff, int yoff, int w, int h, int x, int y);
	void graphics_replace_texture_alpha_from_texture(int tex, int copy_tex);
	void graphics_delete_texture(int texid);
	unsigned char* graphics_get_texture_pixeldata(unsigned texture, unsigned* fullwidth, unsigned* fullheight);

	bool fill_complex_polygon(const std::list<PolyVertex>& vertices, int defaultColor, bool allowHoles);
}

namespace enigma_user
{
	string draw_get_graphics_error();

	unsigned vertex_create_buffer();
	unsigned vertex_create_buffer_ext(unsigned size);
	void vertex_delete_buffer(int buffer);
	void vertex_begin(int buffer, int format);
	void vertex_end(int buffer);
	void vertex_freeze(int buffer);
	void vertex_submit(int buffer, int primitive);
	void vertex_submit(int buffer, int primitive, int texture);
	void vertex_delete(int buffer);
	void vertex_index(int buffer, unsigned id);
	void vertex_position(int buffer, gs_scalar x, gs_scalar y);
	void vertex_position_3d(int buffer, gs_scalar x, gs_scalar y, gs_scalar z);
	void vertex_normal(int buffer, gs_scalar nx, gs_scalar ny, gs_scalar nz);
	void vertex_texcoord(int buffer, gs_scalar u, gs_scalar v);
	void vertex_argb(int buffer, double alpha, unsigned char r, unsigned char g, unsigned char b);
	void vertex_colour(int buffer, int color, double alpha);
	void vertex_float1(int buffer, float f1);
	void vertex_float2(int buffer, float f1, float f2);
	void vertex_float3(int buffer, float f1, float f2, float f3);
	void vertex_float4(int buffer, float f1, float f2, float f3, float f4);
	void vertex_ubyte4(int buffer, unsigned char u1, unsigned char u2, unsigned char u3, unsigned char u4);
	void vertex_format_begin();
	void vertex_format_add(int id, int type, int attribute);
	void vertex_format_add_colour();
	void vertex_format_add_position();
	void vertex_format_add_position_3d();
	void vertex_format_add_textcoord();
	void vertex_format_add_normal();
	void vertex_format_add_custom(int type, int usage);
	unsigned vertex_format_end();

	int texture_add(string filename, bool mipmap);
	void texture_save(int texid, string fname);
	void texture_delete(int texid);
	bool texture_exists(int texid);
	void texture_preload(int texid);
	void texture_set_priority(int texid, double prio);
	void texture_set_enabled(bool enable);
	void texture_set_blending(bool enable);
	gs_scalar texture_get_width(int texid);
	gs_scalar texture_get_height(int texid);
	gs_scalar texture_get_texel_width(int texid);
	gs_scalar texture_get_texel_height(int texid);
	void texture_set_stage(int stage, int texid);
	void texture_reset();
	void texture_set_interpolation_ext(int sampler, bool enable);
	void texture_set_repeat_ext(int sampler, bool repeat);
	void texture_set_wrap_ext(int sampler, bool wrapu, bool wrapv, bool wrapw);
	void texture_set_border_ext(int sampler, int r, int g, int b, double a);
	void texture_set_filter_ext(int sampler, int filter);
	void texture_set_lod_ext(int sampler, double minlod, double maxlod, int maxlevel);
	bool texture_mipmapping_supported();
	bool texture_anisotropy_supported();
	float texture_anisotropy_maxlevel();
	void texture_anisotropy_filter(int sampler, gs_scalar levels);

	bool surface_is_supported();
	int surface_create(int width, int height, bool depthbuffer);
	int surface_create_msaa(int width, int height, int samples);
	void surface_set_target(int id);
	void surface_reset_target(void);
	int surface_get_target();
	void surface_free(int id);
	bool surface_exists(int id);
	int surface_get_texture(int id);
	int surface_get_width(int id);
	int surface_get_height(int id);
	int surface_getpixel(int id, int x, int y);
	int surface_getpixel_ext(int id, int x, int y);
	int surface_getpixel_alpha(int id, int x, int y);

	int surface_save(int id, string filename);

	int surface_save_part(int id, string filename, unsigned x, unsigned y, unsigned w, unsigned h);
	int background_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, bool preload);
	int sprite_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, bool preload, int xorig, int yorig);
	int sprite_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig);
	void sprite_add_from_surface(int ind, int id, int x, int y, int w, int h, bool removeback, bool smooth);
	void surface_copy_part(int destination, gs_scalar x, gs_scalar y, int source, int xs, int ys, int ws, int hs);
	void surface_copy(int destination, gs_scalar x, gs_scalar y, int source);

	int draw_get_msaa_maxlevel();
	bool draw_get_msaa_supported();
	void draw_set_msaa_enabled(bool enable);
	void draw_enable_alphablend(bool enable);
	bool draw_get_alpha_test();
	unsigned draw_get_alpha_test_ref_value();
	void draw_set_alpha_test(bool enable);
	void draw_set_alpha_test_ref_value(unsigned val);
	void draw_set_line_pattern(int pattern, int scale);

	int draw_getpixel(int x,int y);
	int draw_getpixel_ext(int x,int y);

	extern int window_get_region_height_scaled();

	int sprite_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload, int xorig, int yorig);
	int sprite_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig);
	void sprite_add_from_screen(int id, int x, int y, int w, int h, bool removeback, bool smooth);

	extern int window_get_width();
	extern int window_get_height();
	extern int window_get_region_width();
	extern int window_get_region_height();

	void screen_redraw();
	void screen_init();
	int screen_save(string filename);
	int screen_save_part(string filename,unsigned x,unsigned y,unsigned w,unsigned h);
	void screen_set_viewport(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height);
	void display_set_gui_size(unsigned int width, unsigned int height);
	unsigned int display_get_gui_width();
	unsigned int display_get_gui_height();

	void draw_primitive_begin(int kind);
	void draw_primitive_begin_texture(int kind, int tex);
	void draw_primitive_end();
	void draw_vertex(gs_scalar x, gs_scalar y);
	void draw_vertex_color(gs_scalar x, gs_scalar y, int col, float alpha);
	void draw_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty);
	void draw_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty, int col, float alpha);



	void d3d_set_lighting(bool enable);

	void d3d_set_fog(bool enable, int color, double start, double end);
	void d3d_set_fog_enabled(bool enable);
	void d3d_set_fog_mode(int mode);
	void d3d_set_fog_hint(int mode);
	void d3d_set_fog_color(int color);
	void d3d_set_fog_start(double start);
	void d3d_set_fog_end(double end);
	void d3d_set_fog_density(double density);
	void d3d_primitive_begin(int kind);
	void d3d_primitive_begin_texture(int kind, int texId);
	void d3d_primitive_end();
	void d3d_vertex(gs_scalar x, gs_scalar y, gs_scalar z);
	void d3d_vertex_color(gs_scalar x, gs_scalar y, gs_scalar z, int color, double alpha);
	void d3d_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty);
	void d3d_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty, int color, double alpha);
	void d3d_vertex_normal(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz);
	void d3d_vertex_normal_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, int color, double alpha);
	void d3d_vertex_normal_texture(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty);
	void d3d_vertex_normal_texture_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty, int color, double alpha);
	void d3d_draw_block(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed);
	void d3d_draw_floor(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep);
	void d3d_draw_wall(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep);
	void d3d_draw_cylinder(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed, int steps);
	void d3d_draw_cone(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed, int steps);
	void d3d_draw_ellipsoid(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, int steps);
	void d3d_draw_icosahedron(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, int steps);
	void d3d_draw_torus(gs_scalar x1, gs_scalar y1, gs_scalar z1, int texId, gs_scalar hrep, gs_scalar vrep, int csteps, int tsteps, double radius, double tradius);


	void d3d_model_part_draw(int id, int vertex_start, int vertex_count);
	void d3d_model_part_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z, int vertex_start, int vertex_count);
	void d3d_model_part_draw(int id, int texId, int vertex_start, int vertex_count);
	void d3d_model_part_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z, int texId, int vertex_start, int vertex_count);
	void d3d_model_draw(int id);
	void d3d_model_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z);
	void d3d_model_draw(int id, int texId);
	void d3d_model_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z, int texId);

	void d3d_set_perspective(bool enable);
	void d3d_set_projection(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom, gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup);
	void d3d_set_projection_ext(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom, gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup, gs_scalar angle, gs_scalar aspect, gs_scalar znear, gs_scalar zfar);
	void d3d_set_projection_ortho(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle);
	void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle);

	void d3d_transform_force_update();



	void d3d_set_fill_mode(int fill);
	void d3d_set_line_width(float value);
	void d3d_set_point_size(float value);
	void d3d_set_depth_operator(int mode);
	void d3d_set_depth(double dep);
	void d3d_clear_depth();
	void d3d_set_shading(bool smooth);
	void d3d_set_clip_plane(bool enable);

	bool d3d_light_define_direction(int id, gs_scalar dx, gs_scalar dy, gs_scalar dz, int col);
	bool d3d_light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col);
	bool d3d_light_define_specularity(int id, int r, int g, int b, double a);
	void d3d_light_specularity(int facemode, int r, int g, int b, double a);
	bool d3d_light_set_ambient(int id, int r, int g, int b, double a);
	bool d3d_light_set_specularity(int id, int r, int g, int b, double a);
	void d3d_light_shininess(int facemode, int shine);
	void d3d_light_define_ambient(int col);
	bool d3d_light_enable(int id, bool enable);

	void d3d_stencil_start_mask();
	void d3d_stencil_continue_mask();
	void d3d_stencil_use_mask();
	void d3d_stencil_end_mask();

	void draw_clear_alpha(int col,float alpha);
	void draw_clear(int col);

	extern int window_get_region_height_scaled();
	int background_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload);
}
#endif
