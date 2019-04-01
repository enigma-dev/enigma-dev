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

#include <algorithm>
#include <floatcomp.h>
#include <vector>
#include <limits>
#include <map>
#include <list>
#include <stack>
#include "Universal_System/estring.h"

using namespace std;

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

	void texture_set_priority(int texid, double prio);
	void texture_set_enabled(bool enable);
	void texture_set_blending(bool enable);
	bool texture_mipmapping_supported();
	bool texture_anisotropy_supported();
	float texture_anisotropy_maxlevel();

	bool surface_is_supported();
	int surface_create(int width, int height, bool depthbuffer);
	int surface_create_msaa(int width, int height, int samples);
	void surface_set_target(int id);
	void surface_reset_target(void);
	int surface_get_target();
	void surface_free(int id);
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

	int draw_getpixel(int x,int y);
	int draw_getpixel_ext(int x,int y);

	extern int window_get_region_height_scaled();

	int sprite_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload, int xorig, int yorig);
	int sprite_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig);
	void sprite_add_from_screen(int id, int x, int y, int w, int h, bool removeback, bool smooth);

	void shader_set(int id);
	void shader_reset();
	int shader_get_uniform(int program, string name);
	int shader_get_sampler_index(int program, string name);
	void shader_set_uniform_f(int location, float v0);
	void shader_set_uniform_f(int location, float v0, float v1);
	void shader_set_uniform_f(int location, float v0, float v1, float v2);
	void shader_set_uniform_f(int location, float v0, float v1, float v2, float v3);
	void shader_set_uniform_i(int location, int v0);
	void shader_set_uniform_i(int location, int v0, int v1);
	void shader_set_uniform_i(int location, int v0, int v1, int v2);
	void shader_set_uniform_i(int location, int v0, int v1, int v2, int v3);

	extern int window_get_width();
	extern int window_get_height();
	extern int window_get_region_width();
	extern int window_get_region_height();

	int screen_save(string filename);
	int screen_save_part(string filename,unsigned x,unsigned y,unsigned w,unsigned h);
	unsigned int display_get_gui_width();
	unsigned int display_get_gui_height();

	void draw_clear_alpha(int col,float alpha);
	void draw_clear(int col);

	extern int window_get_region_height_scaled();
	int background_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload);
}
#endif
