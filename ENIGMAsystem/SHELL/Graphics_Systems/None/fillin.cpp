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

#include "Graphics_Systems/graphics_mandatory.h" // Room dimensions.
#include "Graphics_Systems/General/GSbackground.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GStiles.h"
#include "Graphics_Systems/General/GSvertex.h"
#include "Graphics_Systems/General/GSsurface.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GSscreen.h"
#include "Graphics_Systems/General/GSd3d.h"
#include "Graphics_Systems/General/GSblend.h"
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Collision_Systems/collision_types.h"
#include "Universal_System/shaderstruct.h"
#include "Universal_System/scalar.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h" // Room dimensions.
#include "Universal_System/depth_draw.h"
#include "Universal_System/background_internal.h"
#include "Universal_System/sprites_internal.h"
#include "Universal_System/image_formats.h"
#include "Universal_System/nlpo2.h"
#include "Universal_System/fileio.h"
#include "Universal_System/estring.h"
#include "Universal_System/instance_system.h"
#include "Universal_System/graphics_object.h"
#include "Universal_System/estring.h"
#include "Platforms/platforms_mandatory.h"

#include <vector>
#include <limits>
#include <map>
#include <list>
#include <stack>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <algorithm>

#include <stdlib.h>     /* malloc, free, rand */
#include <floatcomp.h>
#include <math.h>

using namespace std;

//Below are all functions concerned with drawing anything, or that are tied to
//the GPU (such as surfaces) and as such have no business in a headless mode
namespace enigma
{

	void graphicssystem_initialize(){}

	int graphics_create_texture(unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, void* pxdata, bool mipmap){return -1;}
	int graphics_duplicate_texture(int tex, bool mipmap){return -1;}
	void graphics_copy_texture(int source, int destination, int x, int y){}
	void graphics_copy_texture_part(int source, int destination, int xoff, int yoff, int w, int h, int x, int y){}
	void graphics_replace_texture_alpha_from_texture(int tex, int copy_tex){}
	void graphics_delete_texture(int texid){}
	unsigned char* graphics_get_texture_pixeldata(unsigned texture, unsigned* fullwidth, unsigned* fullheight){return NULL;}

	void graphics_state_flush() {}

	void graphics_delete_vertex_buffer_peer(int buffer) {}
	void graphics_delete_index_buffer_peer(int buffer) {}

	void scene_begin() {}
	void scene_end() {}
}

namespace enigma_user
{
	string draw_get_graphics_error(){return "Running in headless mode";}

	void vertex_argb(int buffer, unsigned argb) {}
	void vertex_color(int buffer, int color, double alpha) {}
	void vertex_submit_offset(int buffer, int primitive, unsigned offset, unsigned vertex_start, unsigned vertex_count) {}
	void index_submit_range(int buffer, int vertex, int primitive, unsigned start, unsigned count) {}

	void texture_set_priority(int texid, double prio){}
	bool texture_mipmapping_supported(){return false;}
	bool texture_anisotropy_supported(){return false;}
	float texture_anisotropy_maxlevel(){return 0;}

	bool surface_is_supported(){return false;}
	int surface_create(int width, int height, bool depthbuffer, bool, bool){return -1;}
	int surface_create_msaa(int width, int height, int samples){return -1;}
	void surface_set_target(int id){}
	void surface_reset_target(void){}
	int surface_get_target(){return -1;}
	void surface_free(int id){}
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

	int screen_save(string filename){return -1;}
	int screen_save_part(string filename,unsigned x,unsigned y,unsigned w,unsigned h){return -1;}
	void screen_set_viewport(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height){}

	void draw_clear_alpha(int col,float alpha){}
	void draw_clear(int col){}
	void d3d_clear_depth(double value){}
	void d3d_stencil_clear_value(int value) {}
	void d3d_stencil_clear() {}
	void d3d_set_software_vertex_processing(bool software){}

	extern int window_get_region_height_scaled();
	int background_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload){return -1;}
}
