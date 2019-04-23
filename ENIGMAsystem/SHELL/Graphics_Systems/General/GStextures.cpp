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

namespace enigma {

vector<Texture*> textures;

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

} // namespace enigma_user
