/** Copyright (C) 2017 Faissal I. Bensefia
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

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>     /* malloc, free, rand */
#include <unordered_map>
#include <math.h>

using namespace std;
#include "Universal_System/shaderstruct.h"
#include "Universal_System/scalar.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h" // Room dimensions.
#include "Universal_System/depth_draw.h"
#include "Universal_System/background_internal.h"
#include "Universal_System/image_formats.h"
#include "Universal_System/sprites_internal.h"
#include "Universal_System/image_formats.h"
#include "Universal_System/nlpo2.h"
#include "Universal_System/fileio.h"
#include "Universal_System/estring.h"
#include "Collision_Systems/collision_types.h"
#include "Universal_System/instance_system.h"
#include "Universal_System/graphics_object.h"
#include <algorithm>
#include "Graphics_Systems/graphics_mandatory.h" // Room dimensions.
#include "Graphics_Systems/General/GSprimitives.h"
#include "../General/GSmath.h"
#include "../General/GSbackground.h"
#include "../General/GStextures.h"
#include "../General/GStiles.h"
#include "../General/GSvertex.h"
#include "../General/GStextures.h"
#include "../General/GSsurface.h"
#include "../General/GSstdraw.h"
#include "../General/GSsprite.h"
#include "../General/GStextures.h"
#include "../General/GLSurfaceStruct.h"
#include "../General/GStextures.h"
#include "../General/GSscreen.h"
#include "../General/GSd3d.h"
#include "../General/GSblend.h"
#include "../General/GSmatrix.h"
#include "../General/GScolors.h"
#include "Platforms/platforms_mandatory.h"
#include <floatcomp.h>
#include <vector>
#include <limits>
#include <map>
#include <list>
#include <stack>
#include "Universal_System/estring.h"

//Below are all functions concerned with drawing anything, or that are tied to
//the GPU (such as surfaces) and as such have no business in a headless mode
namespace enigma
{
	void d3d_light_update_positions(){}

	void graphicssystem_initialize(){}
	void load_tiles(){}
	void delete_tiles(){}
	void rebuild_tile_layer(int layer_depth){}

	int graphics_create_texture(unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, void* pxdata, bool mipmap){return -1;}
	int graphics_duplicate_texture(int tex, bool mipmap){return -1;}
	void graphics_copy_texture(int source, int destination, int x, int y){}
	void graphics_copy_texture_part(int source, int destination, int xoff, int yoff, int w, int h, int x, int y){}
	void graphics_replace_texture_alpha_from_texture(int tex, int copy_tex){}
	void graphics_delete_texture(int texid){}
	unsigned char* graphics_get_texture_pixeldata(unsigned texture, unsigned* fullwidth, unsigned* fullheight){return NULL;}

	bool fill_complex_polygon(const std::list<PolyVertex>& vertices, int defaultColor, bool allowHoles){return false;}
}

namespace enigma_user
{
	string draw_get_graphics_error(){return "Running in headless mode";}
	unsigned vertex_format_create(){return (unsigned) -1;}

	void vertex_format_destroy(int id){}
	bool vertex_format_exists(int id){return false;}
	unsigned vertex_create_buffer(){return (unsigned) -1;}
	unsigned vertex_create_buffer_ext(unsigned size){return (unsigned) -1;}
	void vertex_delete_buffer(int buffer){}
	void vertex_begin(int buffer, int format){}
	void vertex_end(int buffer){}
	void vertex_freeze(int buffer){}
	void vertex_submit(int buffer, int primitive){}
	void vertex_submit(int buffer, int primitive, int texture){}
	void vertex_delete(int buffer){}
	void vertex_index(int buffer, unsigned id){}
	void vertex_position(int buffer, gs_scalar x, gs_scalar y){}
	void vertex_position_3d(int buffer, gs_scalar x, gs_scalar y, gs_scalar z){}
	void vertex_normal(int buffer, gs_scalar nx, gs_scalar ny, gs_scalar nz){}
	void vertex_texcoord(int buffer, gs_scalar u, gs_scalar v){}
	void vertex_argb(int buffer, double alpha, unsigned char r, unsigned char g, unsigned char b){}
	void vertex_colour(int buffer, int color, double alpha){}
	void vertex_float1(int buffer, float f1){}
	void vertex_float2(int buffer, float f1, float f2){}
	void vertex_float3(int buffer, float f1, float f2, float f3){}
	void vertex_float4(int buffer, float f1, float f2, float f3, float f4){}
	void vertex_ubyte4(int buffer, unsigned char u1, unsigned char u2, unsigned char u3, unsigned char u4){}
	void vertex_format_begin(){}
	void vertex_format_add(int id, int type, int attribute){}
	void vertex_format_add_colour(){}
	void vertex_format_add_position(){}
	void vertex_format_add_position_3d(){}
	void vertex_format_add_textcoord(){}
	void vertex_format_add_normal(){}
	void vertex_format_add_custom(int type, int usage){}
	unsigned vertex_format_end(){return (unsigned) -1;}

	int texture_add(string filename, bool mipmap){return -1;}
	void texture_save(int texid, string fname){}
	void texture_delete(int texid){}
	bool texture_exists(int texid){return false;}
	void texture_preload(int texid){}
	void texture_set_priority(int texid, double prio){}
	void texture_set_enabled(bool enable){}
	void texture_set_blending(bool enable){}
	gs_scalar texture_get_width(int texid){return -1;}
	gs_scalar texture_get_height(int texid){return -1;}
	gs_scalar texture_get_texel_width(int texid){return -1;}
	gs_scalar texture_get_texel_height(int texid){return -1;}
	void texture_set_stage(int stage, int texid){}
	void texture_reset(){}
	void texture_set_interpolation_ext(int sampler, bool enable){}
	void texture_set_repeat_ext(int sampler, bool repeat){}
	void texture_set_wrap_ext(int sampler, bool wrapu, bool wrapv, bool wrapw){}
	void texture_set_border_ext(int sampler, int r, int g, int b, double a){}
	void texture_set_filter_ext(int sampler, int filter){}
	void texture_set_lod_ext(int sampler, double minlod, double maxlod, int maxlevel){}
	bool texture_mipmapping_supported(){return false;}
	bool texture_anisotropy_supported(){return false;}
	float texture_anisotropy_maxlevel(){return 0;}
	void texture_anisotropy_filter(int sampler, gs_scalar levels){}

	bool surface_is_supported(){return false;}
	int surface_create(int width, int height, bool depthbuffer){return -1;}
	int surface_create_msaa(int width, int height, int samples){return -1;}
	void surface_set_target(int id){}
	void surface_reset_target(void){}
	int surface_get_target(){return -1;}
	void surface_free(int id){}
	bool surface_exists(int id){return false;}
	int surface_get_texture(int id){return -1;}
	int surface_get_width(int id){return -1;}
	int surface_get_height(int id){return -1;}
	int surface_getpixel(int id, int x, int y){return -1;}
	int surface_getpixel_ext(int id, int x, int y){return -1;}
	int surface_getpixel_alpha(int id, int x, int y){return -1;}

	int surface_save(int id, string filename){return -1;}

	int surface_save_part(int id, string filename, unsigned x, unsigned y, unsigned w, unsigned h){return -1;}
	int background_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, bool preload){return -1;}
	int sprite_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, bool preload, int xorig, int yorig){return -1;}
	int sprite_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig){return -1;}
	void sprite_add_from_surface(int ind, int id, int x, int y, int w, int h, bool removeback, bool smooth){}
	void surface_copy_part(int destination, gs_scalar x, gs_scalar y, int source, int xs, int ys, int ws, int hs){}
	void surface_copy(int destination, gs_scalar x, gs_scalar y, int source){}

	int draw_get_msaa_maxlevel(){return 0;}
	bool draw_get_msaa_supported(){return false;}
	void draw_set_msaa_enabled(bool enable){}
	void draw_enable_alphablend(bool enable){}
	bool draw_get_alpha_test(){return false;}
	unsigned draw_get_alpha_test_ref_value(){return 0;}
	void draw_set_alpha_test(bool enable){}
	void draw_set_alpha_test_ref_value(unsigned val){}
	void draw_set_line_pattern(int pattern, int scale){}

	int draw_getpixel(int x,int y){return -1;}
	int draw_getpixel_ext(int x,int y){return -1;}

	extern int window_get_region_height_scaled();

	int sprite_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload, int xorig, int yorig){return -1;}
	int sprite_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig){return -1;}
	void sprite_add_from_screen(int id, int x, int y, int w, int h, bool removeback, bool smooth){}

	int glsl_shader_create(int type){return -1;}
	int glsl_shader_load(int id, string fname){return -1;}
	bool glsl_shader_compile(int id){return false;}
	bool glsl_shader_get_compiled(int id){return false;}
	string glsl_shader_get_infolog(int id){return "Running in headless mode";}
	void glsl_shader_free(int id){}
	int glsl_program_create(){return -1;}
	bool glsl_program_link(int id){return false;}
	bool glsl_program_validate(int id){return false;}
	void glsl_program_attach(int id, int sid){}
	void glsl_program_detach(int id, int sid){}
	void shader_set(int id){}
	void shader_reset(){}
	int shader_get_uniform(int program, string name){return -1;}
	int shader_get_sampler_index(int program, string name){return -1;}
	void shader_set_uniform_f(int location, float v0){}
	void shader_set_uniform_f(int location, float v0, float v1){}
	void shader_set_uniform_f(int location, float v0, float v1, float v2){}
	void shader_set_uniform_f(int location, float v0, float v1, float v2, float v3){}
	void shader_set_uniform_i(int location, int v0){}
	void shader_set_uniform_i(int location, int v0, int v1){}
	void shader_set_uniform_i(int location, int v0, int v1, int v2){}
	void shader_set_uniform_i(int location, int v0, int v1, int v2, int v3){}

	extern int window_get_width();
	extern int window_get_height();
	extern int window_get_region_width();
	extern int window_get_region_height();

	void screen_redraw(){}
	void screen_init(){}
	int screen_save(string filename){return -1;}
	int screen_save_part(string filename,unsigned x,unsigned y,unsigned w,unsigned h){return -1;}
	void screen_set_viewport(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height){}
	void display_set_gui_size(unsigned int width, unsigned int height){}
	unsigned int display_get_gui_width(){return 0;}
	unsigned int display_get_gui_height(){return 0;}

	void draw_primitive_begin(int kind){}
	void draw_primitive_begin_texture(int kind, int tex){}
	void draw_primitive_end(){}
	void draw_vertex(gs_scalar x, gs_scalar y){}
	void draw_vertex_color(gs_scalar x, gs_scalar y, int col, float alpha){}
	void draw_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty){}
	void draw_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty, int col, float alpha){}

	void d3d_primitive_begin(int kind){}
	void d3d_primitive_begin_texture(int kind, int texId){}
	void d3d_primitive_end(){}
	void d3d_vertex(gs_scalar x, gs_scalar y, gs_scalar z){}
	void d3d_vertex_color(gs_scalar x, gs_scalar y, gs_scalar z, int color, double alpha){}
	void d3d_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty){}
	void d3d_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty, int color, double alpha){}
	void d3d_vertex_normal(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz){}
	void d3d_vertex_normal_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, int color, double alpha){}
	void d3d_vertex_normal_texture(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty){}
	void d3d_vertex_normal_texture_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty, int color, double alpha){}
	void d3d_draw_block(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed){}
	void d3d_draw_floor(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep){}
	void d3d_draw_wall(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep){}
	void d3d_draw_cylinder(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed, int steps){}
	void d3d_draw_cone(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed, int steps){}
	void d3d_draw_ellipsoid(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, int steps){}
	void d3d_draw_icosahedron(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, int steps){}
	void d3d_draw_torus(gs_scalar x1, gs_scalar y1, gs_scalar z1, int texId, gs_scalar hrep, gs_scalar vrep, int csteps, int tsteps, double radius, double tradius){}


	void d3d_model_part_draw(int id, int vertex_start, int vertex_count){}
	void d3d_model_part_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z, int vertex_start, int vertex_count){}
	void d3d_model_part_draw(int id, int texId, int vertex_start, int vertex_count){}
	void d3d_model_part_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z, int texId, int vertex_start, int vertex_count){}
	void d3d_model_draw(int id){}
	void d3d_model_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z){}
	void d3d_model_draw(int id, int texId){}
	void d3d_model_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z, int texId){}

	void d3d_set_perspective(bool enable){}
	void d3d_set_projection(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom, gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup){}
	void d3d_set_projection_ext(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom, gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup, gs_scalar angle, gs_scalar aspect, gs_scalar znear, gs_scalar zfar){}
	void d3d_set_projection_ortho(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle){}
	void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle){}

	void d3d_transform_force_update(){}
	void d3d_set_lighting(bool enable){}

	void d3d_set_fog(bool enable, int color, double start, double end){}
	void d3d_set_fog_enabled(bool enable){}
	void d3d_set_fog_mode(int mode){}
	void d3d_set_fog_hint(int mode){}
	void d3d_set_fog_color(int color){}
	void d3d_set_fog_start(double start){}
	void d3d_set_fog_end(double end){}
	void d3d_set_fog_density(double density){}

	void d3d_set_fill_mode(int fill){}
	void d3d_set_line_width(float value){}
	void d3d_set_point_size(float value){}
	void d3d_set_depth_operator(int mode){}
	void d3d_set_depth(double dep){}
	void d3d_clear_depth(){}
	void d3d_set_shading(bool smooth){}
	void d3d_set_clip_plane(bool enable){}

	bool d3d_light_define_direction(int id, gs_scalar dx, gs_scalar dy, gs_scalar dz, int col){return false;}
	bool d3d_light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col){return false;}
	bool d3d_light_define_specularity(int id, int r, int g, int b, double a){return false;}
	void d3d_light_specularity(int facemode, int r, int g, int b, double a){}
	bool d3d_light_set_ambient(int id, int r, int g, int b, double a){return false;}
	bool d3d_light_set_specularity(int id, int r, int g, int b, double a){return false;}
	void d3d_light_shininess(int facemode, int shine){}
	void d3d_light_define_ambient(int col){}
	bool d3d_light_enable(int id, bool enable){return false;}

	void d3d_stencil_start_mask(){}
	void d3d_stencil_continue_mask(){}
	void d3d_stencil_use_mask(){}
	void d3d_stencil_end_mask(){}

	void draw_clear_alpha(int col,float alpha){}
	void draw_clear(int col){}

	extern int window_get_region_height_scaled();
	int background_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload){return -1;}
}
