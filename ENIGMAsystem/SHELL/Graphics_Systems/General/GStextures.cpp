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
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/image_formats.h"

#include <string.h> // for memcpy

namespace {

inline unsigned int lgpp2(unsigned int x) { // Trailing zero count. lg for perfect powers of two
  x =  (x & -x) - 1;
  x -= ((x >> 1) & 0x55555555);
  x =  ((x >> 2) & 0x33333333) + (x & 0x33333333);
  x =  ((x >> 4) + x) & 0x0f0f0f0f;
  x += x >> 8;
  return (x + (x >> 16)) & 63;
}

} // namespace anonymous

namespace enigma {

vector<std::unique_ptr<Texture>> textures;
Sampler samplers[8];

int graphics_duplicate_texture(int tex, bool mipmap) {
  unsigned w = textures[tex]->width, h = textures[tex]->height,
           fw = textures[tex]->fullwidth, fh = textures[tex]->fullheight;

  unsigned char* bitmap = graphics_copy_texture_pixels(tex, &fw, &fh);
  unsigned dup_tex = graphics_create_texture(w, h, fw, fh, bitmap, mipmap);
  delete[] bitmap;

  return dup_tex;
}

void graphics_copy_texture(int source, int destination, int x, int y) {
  unsigned sw = textures[source]->width, sh = textures[source]->height,
           sfw = textures[source]->fullwidth, sfh = textures[source]->fullheight;
  unsigned char* bitmap = graphics_copy_texture_pixels(source, &sfw, &sfh);

  unsigned char* cropped_bitmap = new unsigned char[sw*sh*4];
  for (unsigned int i=0; i<sh; ++i){
    memcpy(cropped_bitmap+sw*i*4, bitmap+sfw*i*4, sw*4);
  }

  unsigned dw = textures[destination]->width, dh = textures[destination]->height;
  graphics_push_texture_pixels(destination, x, y, (x+sw<=dw?sw:dw-x), (y+sh<=dh?sh:dh-y), cropped_bitmap);

  delete[] bitmap;
  delete[] cropped_bitmap;
}

void graphics_copy_texture_part(int source, int destination, int xoff, int yoff, int w, int h, int x, int y) {
  unsigned sw = textures[source]->width, sh = textures[source]->height,
           sfw = textures[source]->fullwidth, sfh = textures[source]->fullheight;
  unsigned char* bitmap = graphics_copy_texture_pixels(source, &sfw, &sfh);

  if (xoff+sw>sfw) sw = sfw-xoff;
  if (yoff+sh>sfh) sh = sfh-yoff;
  unsigned char* cropped_bitmap = new unsigned char[sw*sh*4];
  for (unsigned int i=0; i<sh; ++i){
    memcpy(cropped_bitmap+sw*i*4, bitmap+xoff*4+sfw*(i+yoff)*4, sw*4);
  }

  unsigned dw = textures[destination]->width, dh = textures[destination]->height;
  graphics_push_texture_pixels(destination, x, y, (x+sw<=dw?sw:dw-x), (y+sh<=dh?sh:dh-y), cropped_bitmap);

  delete[] bitmap;
  delete[] cropped_bitmap;
}

void graphics_replace_texture_alpha_from_texture(int tex, int copy_tex) {
  unsigned fw = textures[tex]->fullwidth, fh = textures[tex]->fullheight;
  unsigned size = (fh<<(lgpp2(fw)+2))|2;
  unsigned char* bitmap = graphics_copy_texture_pixels(tex, &fw, &fh);
  unsigned char* bitmap2 = graphics_copy_texture_pixels(copy_tex, &fw, &fh);

  for (unsigned i = 3; i < size; i += 4) {
    bitmap[i] = (bitmap2[i-3] + bitmap2[i-2] + bitmap2[i-1])/3;
  }

  graphics_push_texture_pixels(tex, fw, fh, bitmap);

  delete[] bitmap;
  delete[] bitmap2;
}

} // namespace enigma

namespace enigma_user {

int texture_add(string filename, bool mipmap) {
  unsigned int w, h, fullwidth, fullheight;
  int img_num;

  unsigned char *pxdata = enigma::image_load(filename,&w,&h,&fullwidth,&fullheight,&img_num,false);
  if (pxdata == NULL) { DEBUG_MESSAGE("ERROR - Failed to append sprite to index!", MESSAGE_TYPE::M_ERROR); return -1; }
  unsigned texture = enigma::graphics_create_texture(w, h, fullwidth, fullheight, pxdata, mipmap);
  delete[] pxdata;

  return texture;
}

void texture_save(int texid, string fname) {
  unsigned w = 0, h = 0;
  unsigned char* rgbdata = enigma::graphics_copy_texture_pixels(texid, &w, &h);

  string ext = enigma::image_get_format(fname);

  enigma::image_save(fname, rgbdata, w, h, w, h, false);

  delete[] rgbdata;
}

void texture_delete(int texid) {
  enigma::graphics_delete_texture(texid); // delete the peer
  enigma::textures[texid] = nullptr;         // GM ids are forever!
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

void texture_set_enabled(bool enable){}

void texture_set_blending(bool enable){}

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
