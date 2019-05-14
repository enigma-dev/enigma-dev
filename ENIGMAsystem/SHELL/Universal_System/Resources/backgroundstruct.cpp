/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>
*** Copyright (C) 2013-2014 Robert B. Colton
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

#include "background_internal.h"
#include "Universal_System/image_formats.h"
#include "libEGMstd.h"
#include "Universal_System/nlpo2.h"

#include "Graphics_Systems/graphics_mandatory.h"

#include <cstring>
#include <string>

namespace enigma {
background **backgroundstructarray;
extern size_t background_idmax;
}  // namespace enigma

/* These functions are primarily for use of the engine. Experienced users
 * who are familiar with C++ can make use of these, but they were made to
 * use at load time with data read from the executable. These both expect
 * RAW format, RGB only.
 */
namespace enigma {
background::background() : tileset(false) {}
background::background(bool ts) : tileset(ts) {}
background::background(int w, int h, int tex, bool trans, bool smth, bool prel)
    : width(w), height(h), texture(tex), transparent(trans), smooth(smth), preload(prel), tileset(false) {}
background::background(bool ts, int w, int h, int tex, bool trans, bool smth, bool prel)
    : width(w), height(h), texture(tex), transparent(trans), smooth(smth), preload(prel), tileset(ts) {}

background_tileset::background_tileset() : background(true) {}
background_tileset::background_tileset(int tw, int th, int ho, int vo, int hs, int vs)
    : background(true), tileWidth(tw), tileHeight(th), hOffset(ho), vOffset(vo), hSep(hs), vSep(vs) {}
background_tileset::background_tileset(int w, int h, int tex, bool trans, bool smth, bool prel, int tw, int th, int ho,
                                       int vo, int hs, int vs)
    : background(true, w, h, tex, trans, smth, prel),
      tileWidth(tw),
      tileHeight(th),
      hOffset(ho),
      vOffset(vo),
      hSep(hs),
      vSep(vs) {}

size_t backgroundstructarray_actualsize =
    0;  // INVARIANT: Should always be equal to the actual size of spritestructarray.

//Allocates and zero-fills the array at game start
void backgrounds_init() {
  backgroundstructarray_actualsize = background_idmax + 1;
  backgroundstructarray = new background *[backgroundstructarray_actualsize];
  for (unsigned i = 0; i < background_idmax; i++) backgroundstructarray[i] = NULL;
}

//Adds a subimage to an existing sprite from the exe
void background_new(int bkgid, unsigned w, unsigned h, unsigned char *chunk, bool transparent, bool smoothEdges,
                    bool preload, bool useAsTileset, int tileWidth, int tileHeight, int hOffset, int vOffset, int hSep,
                    int vSep) {
  unsigned int fullwidth = enigma::nlpo2dc(w) + 1, fullheight = enigma::nlpo2dc(h) + 1;
  char *imgpxdata = new char[4 * fullwidth * fullheight + 1], *imgpxptr = imgpxdata;
  unsigned int rowindex, colindex;
  for (rowindex = 0; rowindex < h; rowindex++) {
    for (colindex = 0; colindex < w; colindex++) {
      *imgpxptr++ = *chunk++;
      *imgpxptr++ = *chunk++;
      *imgpxptr++ = *chunk++;
      *imgpxptr++ = *chunk++;
    }
    memset(imgpxptr, 0, (fullwidth - colindex) << 2);
    imgpxptr += (fullwidth - colindex) << 2;
  }
  memset(imgpxptr, 0, (fullheight - h) * fullwidth);

  int texture = graphics_create_texture(w, h, fullwidth, fullheight, imgpxdata, false);
  delete[] imgpxdata;

  backgroundstructarray[bkgid] = useAsTileset
                                     ? new background(w, h, texture, transparent, smoothEdges, preload)
                                     : new background_tileset(w, h, texture, transparent, smoothEdges, preload,
                                                              tileWidth, tileHeight, hOffset, vOffset, hSep, vSep);
  background *bak = backgroundstructarray[bkgid];
  bak->texturew = (double)w / fullwidth;
  bak->textureh = (double)h / fullheight;
}

void background_add_to_index(background *bak, std::string filename, bool transparent, bool smoothEdges, bool preload,
                             bool mipmap) {
  unsigned int w, h, fullwidth, fullheight;
  int img_numb;

  unsigned char *pxdata = image_load(filename, &w, &h, &fullwidth, &fullheight, &img_numb, false);
  if (pxdata == NULL) {
    printf("ERROR - Failed to append background to index!\n");
    return;
  }

  // If background is transparent, set the alpha to zero for pixels that should be transparent from lower left pixel color
  if (transparent) {
    int t_pixel_b = pxdata[(h - 1) * fullwidth * 4];
    int t_pixel_g = pxdata[(h - 1) * fullwidth * 4 + 1];
    int t_pixel_r = pxdata[(h - 1) * fullwidth * 4 + 2];
    unsigned int ih, iw;
    for (ih = 0; ih < h; ih++) {
      int tmp = ih * fullwidth * 4;
      for (iw = 0; iw < w; iw++) {
        if (pxdata[tmp] == t_pixel_b && pxdata[tmp + 1] == t_pixel_g && pxdata[tmp + 2] == t_pixel_r)
          pxdata[tmp + 3] = 0;

        tmp += 4;
      }
    }
  }

  unsigned texture = graphics_create_texture(w, h, fullwidth, fullheight, pxdata, mipmap);
  delete[] pxdata;

  bak->width = w;
  bak->height = h;
  bak->transparent = transparent;
  bak->smooth = smoothEdges;
  bak->preload = preload;
  bak->tileset = false;
  bak->texturew = (double)w / fullwidth;
  bak->textureh = (double)h / fullheight;
  bak->texture = texture;
}

void background_add_copy(background *bak, background *bck_copy) {
  bak->width = bck_copy->width;
  bak->height = bck_copy->height;
  bak->transparent = bck_copy->transparent;
  bak->smooth = bck_copy->smooth;
  bak->preload = bck_copy->preload;
  bak->tileset = bck_copy->tileset;
  bak->texturew = bck_copy->texturew;
  bak->textureh = bck_copy->textureh;
  bak->texture = graphics_duplicate_texture(bck_copy->texture);
}

void backgroundstructarray_reallocate() {
  if (backgroundstructarray_actualsize > background_idmax + 2) {  // Ignore reallocation request if array is big enough.
    return;
  }
  background **backgroundold = backgroundstructarray;
  backgroundstructarray_actualsize =
      (background_idmax + 2) * 2;  // Grow by doubling size, minimizing needed reallocations.
  backgroundstructarray = new background *[backgroundstructarray_actualsize];
  for (size_t i = 0; i < background_idmax; i++) backgroundstructarray[i] = backgroundold[i];
  delete[] backgroundold;
}
}  // namespace enigma

namespace enigma_user {
int background_add(std::string filename, bool transparent, bool smooth, bool preload, bool mipmap) {
  enigma::backgroundstructarray_reallocate();
  enigma::background *bck = enigma::backgroundstructarray[enigma::background_idmax] = new enigma::background;
  enigma::background_add_to_index(bck, filename, transparent, smooth, preload, mipmap);
  return enigma::background_idmax++;
}

int background_create_color(unsigned w, unsigned h, int col, bool preload) {
  int full_width = enigma::nlpo2dc(w) + 1, full_height = enigma::nlpo2dc(h) + 1;

  unsigned sz = full_width * full_height;
  unsigned char *surfbuf = new unsigned char[sz * 4];

  for (unsigned int i = 0; i + 3 < sz * 4; i += 4) {
    surfbuf[i] = COL_GET_R(col);
    surfbuf[i + 1] = COL_GET_G(col);
    surfbuf[i + 2] = COL_GET_B(col);
    surfbuf[i + 3] = 255;
  }

  enigma::backgroundstructarray_reallocate();
  int bckid = enigma::background_idmax;
  enigma::background_new(bckid, w, h, surfbuf, false, false, preload, false, 0, 0, 0, 0, 0, 0);
  delete[] surfbuf;
  enigma::background_idmax++;
  return bckid;
}

bool background_replace(int back, std::string filename, bool transparent, bool smooth, bool preload, bool free_texture,
                        bool mipmap) {
  get_backgroundnv(bck, back, false);
  if (free_texture) enigma::graphics_delete_texture(bck->texture);

  enigma::background_add_to_index(bck, filename, transparent, smooth, preload, mipmap);
  return true;
}

void background_save(int back, std::string fname) {
  get_background(bck, back);
  unsigned w, h;
  unsigned char *rgbdata = enigma::graphics_copy_texture_pixels(bck->texture, &w, &h);

  std::string ext = enigma::image_get_format(fname);

  enigma::image_save(fname, rgbdata, bck->width, bck->height, w, h, false);

  delete[] rgbdata;
}

void background_delete(int back, bool free_texture) {
  get_background(bck, back);
  if (free_texture) enigma::graphics_delete_texture(bck->texture);

  delete enigma::backgroundstructarray[back];
  enigma::backgroundstructarray[back] = NULL;
}

int background_duplicate(int back) {
  get_backgroundnv(bck_copy, back, -1);
  enigma::backgroundstructarray_reallocate();
  enigma::background *bck = enigma::backgroundstructarray[enigma::background_idmax] = new enigma::background;
  enigma::background_add_copy(bck, bck_copy);
  return enigma::background_idmax++;
}

void background_assign(int back, int copy_background, bool free_texture) {
  get_background(bck, back);
  get_background(bck_copy, copy_background);
  if (free_texture) enigma::graphics_delete_texture(bck->texture);

  enigma::background_add_copy(bck, bck_copy);
}

bool background_exists(int back) {
  return unsigned(back) < enigma::background_idmax && bool(enigma::backgroundstructarray) &&
         bool(enigma::backgroundstructarray[back]);
}

void background_set_alpha_from_background(int back, int copy_background, bool free_texture) {
  get_background(bck, back);
  get_background(bck_copy, copy_background);
  enigma::graphics_replace_texture_alpha_from_texture(bck->texture, bck_copy->texture);
}

int background_get_texture(int backId) {
  get_backgroundnv(bck2d, backId, -1);
  return bck2d->texture;
}

int background_get_width(int backId) {
  get_backgroundnv(bck2d, backId, -1);
  return bck2d->width;
}

int background_get_height(int backId) {
  get_backgroundnv(bck2d, backId, -1);
  return bck2d->height;
}

var background_get_uvs(int backId) {
  var uvs;
  uvs[4] = 0;
  get_backgroundnv(bck2d, backId, uvs);

  uvs[0] = bck2d->texturex;
  uvs[1] = bck2d->texturey;
  uvs[2] = bck2d->texturex + bck2d->texturew;
  uvs[3] = bck2d->texturey + bck2d->textureh;
  return uvs;
}
}  // namespace enigma_user
