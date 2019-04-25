/** Copyright (C) 2008-2013, Josh Ventura
*** Copyright (C) 2013-2014,2019 Robert B. Colton
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

#include "GStextures.h"
#include "GSstdraw.h"
#include "GStextures_impl.h"
#include "Graphics_Systems/graphics_mandatory.h"

#include "Universal_System/image_formats.h"

namespace enigma {

vector<Texture*> textures;
Sampler samplers[8];

} // namespace enigma

namespace enigma_user {

int texture_add(string filename, bool mipmap) {
  unsigned int w, h, fullwidth, fullheight;
  int img_num;

  unsigned char *pxdata = enigma::image_load(filename,&w,&h,&fullwidth,&fullheight,&img_num,false);
  if (pxdata == NULL) { printf("ERROR - Failed to append sprite to index!\n"); return -1; }
  unsigned texture = enigma::graphics_create_texture(w, h, fullwidth, fullheight, pxdata, mipmap);
  delete[] pxdata;

  return texture;
}

void texture_save(int texid, string fname) {
  unsigned w = 0, h = 0;
  unsigned char* rgbdata = enigma::graphics_get_texture_pixeldata(texid, &w, &h);

  string ext = enigma::image_get_format(fname);

  enigma::image_save(fname, rgbdata, w, h, w, h, false);

  delete[] rgbdata;
}

void texture_delete(int texid) {
  enigma::graphics_delete_texture(texid); // delete the peer
  delete enigma::textures[texid];         // now delete the user object
  enigma::textures[texid] = NULL;         // GM ids are forever!
}

bool texture_exists(int texid) {
  return (texid >= 0 && size_t(texid) < enigma::textures.size() && enigma::textures[texid] != nullptr);
}

void texture_preload(int texid)
{
  // Deprecated in ENIGMA and GM: Studio, all textures are automatically preloaded.
}

gs_scalar texture_get_width(int texid) {
  return enigma::textures[texid]->width / enigma::textures[texid]->fullwidth;
}

gs_scalar texture_get_height(int texid)
{
  return enigma::textures[texid]->height / enigma::textures[texid]->fullheight;
}

gs_scalar texture_get_texel_width(int texid)
{
  return 1.0/enigma::textures[texid]->width;
}

gs_scalar texture_get_texel_height(int texid)
{
  return 1.0/enigma::textures[texid]->height;
}

void texture_set_stage(int stage, int texid) {
  if (enigma::samplers[stage].texture == texid) return;
  enigma::draw_set_state_dirty();
  enigma::samplers[stage].texture = texid;
}

int texture_get_stage(int stage) {
  return enigma::samplers[stage].texture;
}

void texture_reset() {
  if (enigma::samplers[0].texture == -1) return;
  enigma::draw_set_state_dirty();
  enigma::samplers[0].texture = -1;
}

void texture_set_interpolation_ext(int sampler, bool enable) {
  enigma::draw_set_state_dirty();
  enigma::samplers[sampler].interpolate = enable;
}

void texture_set_repeat_ext(int sampler, bool repeat) {
  enigma::draw_set_state_dirty();
  enigma::samplers[sampler].wrapu = repeat;
  enigma::samplers[sampler].wrapv = repeat;
  enigma::samplers[sampler].wrapw = repeat;
}

void texture_set_wrap_ext(int sampler, bool wrapu, bool wrapv, bool wrapw) {
  enigma::draw_set_state_dirty();
  enigma::samplers[sampler].wrapu = wrapu;
  enigma::samplers[sampler].wrapv = wrapv;
  enigma::samplers[sampler].wrapw = wrapw;
}

void texture_set_border_ext(int sampler, int r, int g, int b, double a) {
  enigma::draw_set_state_dirty();
}

void texture_set_filter_ext(int sampler, int filter) {
  enigma::draw_set_state_dirty();
}

void texture_set_lod_ext(int sampler, double minlod, double maxlod, int maxlevel) {
  enigma::draw_set_state_dirty();
}

void texture_anisotropy_filter(int sampler, gs_scalar levels) {
  enigma::draw_set_state_dirty();
}

} // namespace enigma_user
