/** Copyright (C) 2020 Robert B. Colton
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

#ifndef ENIGMA_GSGPU_H
#define ENIGMA_GSGPU_H

#include "Universal_System/var4.h"

namespace enigma_user {

// NOTE: Game Maker uses clockwise culling to define the front face,
// OpenGL's mode sets what defines the front face, Direct3D's mode sets what defines the back face
enum {
  cull_noculling = 0,
  cull_clockwise,
  cull_counterclockwise
};

enum {
  cmpfunc_never = 0,    // Always False            D3DCMP_NEVER            GL_NEVER
  cmpfunc_less,         // source Z < depth Z      D3DCMP_LESS             GL_LESS
  cmpfunc_equal,        // source Z = depth Z      D3DCMP_EQUAL            GL_EQUAL
  cmpfunc_lessequal,    // source Z <= depth Z     D3DCMP_LESSEQUAL        GL_LEQUAL
  cmpfunc_greater,      // source Z > depth Z      D3DCMP_GREATER          GL_GREATER
  cmpfunc_notequal,     // source Z != depth Z     D3DCMP_NOTEQUAL         GL_NOTEQUAL
  cmpfunc_greaterequal, // source Z >= depth Z     D3DCMP_GREATEREQUAL     GL_GEQUAL
  cmpfunc_always        // Always True             D3DCMP_ALWAYS           GL_ALWAYS
};

enum {
  tf_point = 0,
  tf_linear,
  tf_anisotropic
};

enum {
  mip_off = 0,
  mip_on,
  mip_markedonly
};

bool gpu_get_blendenable();
bool gpu_get_ztestenable();
int gpu_get_zfunc();
bool gpu_get_zwriteenable();
var gpu_get_fog();
int gpu_get_cullmode();
int gpu_get_blendmode();
var gpu_get_blendmode_ext();
var gpu_get_blendmode_ext_sepalpha();
int gpu_get_blendmode_src();
int gpu_get_blendmode_dest();
int gpu_get_blendmode_srcalpha();
int gpu_get_blendmode_destalpha();
var gpu_get_colorwriteenable();
bool gpu_get_alphatestenable();
int gpu_get_alphatestref();
bool gpu_get_texfilter();
bool gpu_get_texfilter_ext(int sampler_index);
bool gpu_get_texrepeat();
bool gpu_get_texrepeat_ext(int sampler_index);

int gpu_get_tex_mip_filter();
int gpu_get_tex_mip_filter_ext(int sampler_index);
int gpu_get_tex_mip_bias();
int gpu_get_tex_mip_bias_ext(int sampler_index);
int gpu_get_tex_min_mip();
int gpu_get_tex_min_mip_ext(int sampler_index);
int gpu_get_tex_max_mip();
int gpu_get_tex_max_mip_ext(int sampler_index);
int gpu_get_tex_max_aniso();
int gpu_get_tex_max_aniso_ext(int sampler_index);
int gpu_get_tex_mip_enable();
int gpu_get_tex_mip_enable_ext(int sampler_index);

void gpu_set_blendenable(bool enable);
void gpu_set_ztestenable(bool enable);
void gpu_set_zfunc(int cmp_func);
void gpu_set_zwriteenable(bool enable);
void gpu_set_fog(bool enable, int color, double start, double end);
void gpu_set_cullmode(int cullmode);
void gpu_set_blendmode(int mode);
void gpu_set_blendmode_ext(int src, int dest);
void gpu_set_blendmode_ext_sepalpha(int src, int dest, int alphasrc, int alphadest);
void gpu_set_colorwriteenable(bool red, bool green, bool blue, bool alpha);
void gpu_set_alphatestenable(bool enable);
void gpu_set_alphatestref(int val);
void gpu_set_texfilter(bool enable);
void gpu_set_texfilter_ext(int sampler_id, bool enable);
void gpu_set_texrepeat(bool enable);
void gpu_set_texrepeat_ext(int sampler_id, bool enable);

void gpu_set_tex_mip_filter(int filter);
void gpu_set_tex_mip_filter_ext(int sampler_index, int filter);
void gpu_set_tex_mip_bias(int bias);
void gpu_set_tex_mip_bias_ext(int sampler_index, int bias);
void gpu_set_tex_min_mip(int minmip);
void gpu_set_tex_min_mip_ext(int sampler_index, int minmip);
void gpu_set_tex_max_mip(int maxmip);
void gpu_set_tex_max_mip_ext(int sampler_index, int maxmip);
void gpu_set_tex_max_aniso(int maxaniso);
void gpu_set_tex_max_aniso_ext(int sampler_index, int maxaniso);
void gpu_set_tex_mip_enable(int setting);
void gpu_set_tex_mip_enable_ext(int sampler_index, int setting);

void gpu_push_state();
void gpu_pop_state();
int gpu_get_state();
void gpu_set_state(int ds_map);

} // namespace enigma_user

#endif // ENIGMA_GSGPU_H
