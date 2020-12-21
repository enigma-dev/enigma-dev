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

#include "GSgpu.h"

// these includes are only for temporarily hoisting the setters on top of legacy ones
#include "GSblend.h"
#include "GSstdraw.h"
#include "GStextures.h"
#include "GSd3d.h"
#include "GScolors.h"
#include "Graphics_Systems/graphics_mandatory.h"

namespace enigma_user {

bool gpu_get_blendenable() {
  return false;
}

bool gpu_get_ztestenable() {
  return false;
}

int gpu_get_zfunc() {
  return 0;
}

bool gpu_get_zwriteenable() {
  return false;
}

var gpu_get_fog() {
  return var();
}

int gpu_get_cullmode() {
  return 0;
}

int gpu_get_blendmode() {
  return 0;
}

var gpu_get_blendmode_ext() {
  return var();
}

var gpu_get_blendmode_ext_sepalpha() {
  return var();
}

int gpu_get_blendmode_src() {
  return 0;
}

int gpu_get_blendmode_dest() {
  return 0;
}

int gpu_get_blendmode_srcalpha() {
  return 0;
}

int gpu_get_blendmode_destalpha() {
  return 0;
}

var gpu_get_colorwriteenable() {
  return var();
}

bool gpu_get_alphatestenable() {
  return false;
}

int gpu_get_alphatestref() {
  return 0;
}

bool gpu_get_texfilter() {
  return false;
}

bool gpu_get_texfilter_ext(int sampler_index) {
  return false;
}

bool gpu_get_texrepeat() {
  return false;
}

bool gpu_get_texrepeat_ext(int sampler_index) {
  return false;
}

int gpu_get_tex_mip_filter() {
  return 0;
}

int gpu_get_tex_mip_filter_ext(int sampler_index) {
  return 0;
}

int gpu_get_tex_mip_bias() {
  return 0;
}

int gpu_get_tex_mip_bias_ext(int sampler_index) {
  return 0;
}

int gpu_get_tex_min_mip() {
  return 0;
}

int gpu_get_tex_min_mip_ext(int sampler_index) {
  return 0;
}

int gpu_get_tex_max_mip() {
  return 0;
}

int gpu_get_tex_max_mip_ext(int sampler_index) {
  return 0;
}

int gpu_get_tex_max_aniso() {
  return 0;
}

int gpu_get_tex_max_aniso_ext(int sampler_index) {
  return 0;
}

int gpu_get_tex_mip_enable() {
  return 0;
}

int gpu_get_tex_mip_enable_ext(int sampler_index) {
  return 0;
}

void gpu_set_blendenable(bool enable) {
  draw_enable_alphablend(enable);
}

void gpu_set_ztestenable(bool enable) {
  d3d_set_hidden(enable);
}

void gpu_set_zfunc(int cmp_func) {
  d3d_set_depth_operator(cmp_func);
}

void gpu_set_zwriteenable(bool enable) {
  d3d_set_zwriteenable(enable);
}

void gpu_set_fog(bool enable, int color, double start, double end) {
  d3d_set_fog(enable, color, start, end);
}

void gpu_set_cullmode(int cullmode) {
  d3d_set_culling(cullmode);
}

void gpu_set_blendmode(int mode) {
  draw_set_blend_mode(mode);
}

void gpu_set_blendmode_ext(int src, int dest) {
  draw_set_blend_mode_ext(src, dest);
}

void gpu_set_blendmode_ext_sepalpha(int src, int dest, int alphasrc, int alphadest) {
  draw_set_blend_mode_ext(src, dest);
}

void gpu_set_colorwriteenable(bool red, bool green, bool blue, bool alpha) {
  draw_set_color_write_enable(red, green, blue, alpha);
}

void gpu_set_alphatestenable(bool enable) {
  draw_set_alpha_test(enable);
}

void gpu_set_alphatestref(int val) {
  draw_set_alpha_test_ref_value(val);
}

void gpu_set_texfilter(bool enable) {
  texture_set_interpolation(enable);
}

void gpu_set_texfilter_ext(int sampler_id, bool enable) {
  texture_set_interpolation_ext(sampler_id, enable);
}

void gpu_set_texrepeat(bool enable) {
  texture_set_repeat(enable);
}

void gpu_set_texrepeat_ext(int sampler_id, bool enable) {
  texture_set_repeat_ext(sampler_id, enable);
}

void gpu_set_tex_mip_filter(int filter) {

}

void gpu_set_tex_mip_filter_ext(int sampler_index, int filter) {

}

void gpu_set_tex_mip_bias(int bias) {

}

void gpu_set_tex_mip_bias_ext(int sampler_index, int bias) {

}

void gpu_set_tex_min_mip(int minmip) {

}

void gpu_set_tex_min_mip_ext(int sampler_index, int minmip) {

}

void gpu_set_tex_max_mip(int maxmip) {

}

void gpu_set_tex_max_mip_ext(int sampler_index, int maxmip) {

}

void gpu_set_tex_max_aniso(int maxaniso) {

}

void gpu_set_tex_max_aniso_ext(int sampler_index, int maxaniso) {

}

void gpu_set_tex_mip_enable(int setting) {

}

void gpu_set_tex_mip_enable_ext(int sampler_index, int setting) {

}

void gpu_push_state() {

}

void gpu_pop_state() {

}

int gpu_get_state() {
    return -1;
}

void gpu_set_state(int ds_map) {

}

} // namespace enigma_user
