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
#include "GStextures_impl.h"
#include "Graphics_Systems/graphics_mandatory.h"

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

vector<Texture*> textures;

int graphics_duplicate_texture(int tex, bool mipmap) {
  unsigned w, h, fw, fh;
  w = textures[tex]->width;
  h = textures[tex]->height;
  fw = textures[tex]->fullwidth;
  fh = textures[tex]->fullheight;
  unsigned char* bitmap = graphics_copy_texture_pxdata(tex, &fw, &fh);
  unsigned dup_tex = graphics_create_texture(w, h, fw, fh, bitmap, mipmap);
  delete[] bitmap;

  return dup_tex;
}

void graphics_copy_texture(int source, int destination, int x, int y) {
  unsigned int sw, sh, sfw, sfh;
  sw = textures[source]->width;
  sh = textures[source]->height;
  sfw = textures[source]->fullwidth;
  sfh = textures[source]->fullheight;
  unsigned char* bitmap = graphics_copy_texture_pxdata(source, &sfw, &sfh);

  unsigned char* cropped_bitmap = new unsigned char[sw*sh*4];
  for (unsigned int i=0; i<sh; ++i){
    memcpy(cropped_bitmap+sw*i*4, bitmap+sfw*i*4, sw*4);
  }

  unsigned dw, dh;
  dw = textures[destination]->width;
  dh = textures[destination]->height;
  graphics_push_texture_pxdata(destination, x, y, (x+sw<=dw?sw:dw-x), (y+sh<=dh?sh:dh-y), cropped_bitmap);

  delete[] bitmap;
  delete[] cropped_bitmap;
}

void graphics_copy_texture_part(int source, int destination, int xoff, int yoff, int w, int h, int x, int y) {
  unsigned int sw, sh, sfw, sfh;
  sw = textures[source]->width;
  sh = textures[source]->height;
  sfw = textures[source]->fullwidth;
  sfh = textures[source]->fullheight;
  unsigned char* bitmap = graphics_copy_texture_pxdata(source, &sfw, &sfh);

  if (xoff+sw>sfw) sw = sfw-xoff;
  if (yoff+sh>sfh) sh = sfh-yoff;
  unsigned char* cropped_bitmap = new unsigned char[sw*sh*4];
  for (unsigned int i=0; i<sh; ++i){
    memcpy(cropped_bitmap+sw*i*4, bitmap+xoff*4+sfw*(i+yoff)*4, sw*4);
  }

  unsigned dw, dh;
  dw = textures[destination]->width;
  dh = textures[destination]->height;
  graphics_push_texture_pxdata(destination, x, y, (x+sw<=dw?sw:dw-x), (y+sh<=dh?sh:dh-y), cropped_bitmap);

  delete[] bitmap;
  delete[] cropped_bitmap;
}

void graphics_replace_texture_alpha_from_texture(int tex, int copy_tex) {
  unsigned fw, fh, size;
  fw = textures[tex]->fullwidth;
  fh = textures[tex]->fullheight;
  size = (fh<<(lgpp2(fw)+2))|2;
  unsigned char* bitmap = graphics_copy_texture_pxdata(tex, &fw, &fh);
  unsigned char* bitmap2 = graphics_copy_texture_pxdata(copy_tex, &fw, &fh);

  for (unsigned i = 3; i < size; i += 4) {
    bitmap[i] = (bitmap2[i-3] + bitmap2[i-2] + bitmap2[i-1])/3;
  }

  graphics_push_texture_pxdata(tex, fw, fh, bitmap);

  delete[] bitmap;
  delete[] bitmap2;
}

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
  unsigned w, h;
  unsigned char* rgbdata = enigma::graphics_copy_texture_pxdata(texid, &w, &h);

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

} // namespace enigma_user
