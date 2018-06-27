/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2013-2014 Robert B. Colton
*** Copyright (C) 2015-2016 Harijs Grinbergs
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

#include "Graphics_Systems/graphics_mandatory.h"

#include "Collision_Systems/collision_mandatory.h"

#include "image_formats.h"
#include "sprites_internal.h"

using enigma::Sprite;

namespace enigma {

ResourceVec<Sprite> sprites("sprite");

void Sprite::freeTextures() {
  for (unsigned i = 0; i < subimages.size(); ++i) enigma::graphics_delete_texture(subimages[i].textureID);
}

size_t Sprite::subcount() { return subimages.size(); }

SubImage::SubImage(const SubImage &copy) {
  textureID = graphics_duplicate_texture(copy.textureID);
  x = copy.x;
  y = copy.y;
  w = copy.w;
  h = copy.h;
  for (void *c : copy.collisionData) collisionData.push_back(c);
}

inline void lowerLeftToTransparent(unsigned char *pxdata, unsigned w, unsigned h) {
  int idx = (h - 1) * w * 4;
  int color[3];
  color[0] = pxdata[idx];
  color[1] = pxdata[idx + 1];
  color[2] = pxdata[idx + 2];
  for (unsigned i = 0; i < w * h * 4; i += 4) {
    idx = (h - 1) * w * 4;
    int color2[3];
    color2[0] = pxdata[idx];
    color2[1] = pxdata[idx + 1];
    color2[2] = pxdata[idx + 2];

    if (color == color2) pxdata[idx + 3] = 0;
  }
}

inline std::array<bbox_rect_t, 2> calculateBBox(unsigned w, unsigned h, int xoffset, int yoffset) {
  // FIXME: Calculate and assign correct bbox values.
  std::array<bbox_rect_t, 2> b;

  b[0].bottom = h;
  b[0].left = 0;
  b[0].top = 0;
  b[0].right = w;

  b[1].bottom = b[0].bottom - yoffset;
  b[1].left = b[0].left - xoffset;
  b[1].top = b[0].top - yoffset;
  b[1].right = b[0].right - xoffset;

  return b;
}

inline void addTexture(Sprite *spr, unsigned char *pxdata, unsigned fullWidth, unsigned fullHeight,
                       unsigned targetWidth, unsigned targetHeight, unsigned imgCount, bool precise, bool mipmap) {
  unsigned fullcellwidth = nlpo2dc(fullWidth) + 1;

  unsigned char *pixels = new unsigned char[fullcellwidth * fullHeight * 4]();
  for (unsigned ii = 0; ii < imgCount; ii++) {
    unsigned ih, iw;
    unsigned xcelloffset = ii * targetWidth * 4;
    for (ih = 0; ih < targetHeight; ih++) {
      unsigned tmp = ih * fullWidth * 4 + xcelloffset;
      unsigned tmpcell = ih * fullcellwidth * 4;
      for (iw = 0; iw < targetWidth; iw++) {
        pixels[tmpcell + 0] = pxdata[tmp + 0];
        pixels[tmpcell + 1] = pxdata[tmp + 1];
        pixels[tmpcell + 2] = pxdata[tmp + 2];
        pixels[tmpcell + 3] = pxdata[tmp + 3];
        tmp += 4;
        tmpcell += 4;
      }
    }

    SubImage s;
    s.textureID = graphics_create_texture(targetWidth, targetHeight, fullcellwidth, fullHeight, pixels, mipmap);
    s.x = 0;
    s.y = 0;
    s.w = static_cast<double>(targetWidth) / fullcellwidth;
    s.h = static_cast<double>(targetHeight) / fullHeight;

    collision_type coll_type = precise ? ct_precise : ct_bbox;
    s.collisionData.push_back(get_collision_mask(spr, static_cast<unsigned char *>(pixels), coll_type));

    spr->subimages.push_back(std::move(s));
  }

  delete pixels;
}

int sprite_add_subimage(unsigned char *pxdata, unsigned w, unsigned h, int id, int imgCount, bool precise,
                        bool transparent, bool smooth, int xoffset, int yoffset, bool mipmap) {
#ifdef DEBUG_MODE
  if (pxdata == nullptr) {
    printf("ERROR - Failed to append sprite to index!\n");
    return -1;
  }
#endif

  unsigned fullwidth = nlpo2dc(w) + 1;
  unsigned fullheight = nlpo2dc(h) + 1;

  if (transparent) enigma::lowerLeftToTransparent(pxdata, fullwidth, fullheight);

  unsigned cellwidth = w / imgCount;

  Sprite *spr;
  get_resi(sprites, id, spr);

  spr->bbox = enigma::calculateBBox(cellwidth, h, xoffset, yoffset);
  spr->width = cellwidth;
  spr->height = h;
  spr->xoffset = xoffset;
  spr->yoffset = yoffset;
  spr->smooth = smooth;

  addTexture(spr, pxdata, fullwidth, fullheight, cellwidth, h, imgCount, precise, mipmap);

  delete pxdata;

  return spr->subimages.size() - 1;
}

bool sprite_set(int index, unsigned char *pxdata, unsigned w, unsigned h, int imgnumb, bool precise, bool transparent,
                bool smooth, bool preload, int xoffset, int yoffset, bool mipmap) {
  sprites.assign(index,
                 std::move(make_unique<Sprite>(precise, transparent, smooth, preload, xoffset, yoffset, mipmap)));

  if (imgnumb == 0)
    return true;
  else
    return (sprite_add_subimage(pxdata, w, h, sprites.size() - 1, imgnumb, precise, transparent, smooth, xoffset,
                                yoffset, mipmap) >= 0);
}

int sprite_add(unsigned char *pxdata, unsigned w, unsigned h, int imgnumb, bool precise, bool transparent, bool smooth,
               bool preload, int xoffset, int yoffset, bool mipmap) {
  int id = sprites.size();

  sprites.push_back(nullptr);
  if (!sprite_set(id, pxdata, w, h, imgnumb, precise, transparent, smooth, preload, xoffset, yoffset, mipmap)) {
    sprites.pop_back();
    return -1;
  }

  return id;
}

bbox_rect_t dummy_bbox = bbox_rect_t(32, 0, 32, 0);

const bbox_rect_t &sprite_get_bbox(int sprid) {
  enigma::Sprite *spr;
  get_res(sprites, sprid, spr);
  if (!spr) return dummy_bbox;

  return spr->bbox[0];
}

const bbox_rect_t &sprite_get_bbox_relative(int sprid) {
  Sprite *spr;
  get_res(sprites, sprid, spr);
  if (!spr) return dummy_bbox;

  return spr->bbox[1];
}

}  // namespace enigma

using enigma::sprites;

namespace enigma_user {

int sprite_add_subimage(std::string filename, int id, int imgCount, bool precise, bool transparent, bool smooth,
                        int xoffset, int yoffset, bool mipmap) {
  unsigned width, height;
  int imgnumb;
  unsigned char *pxdata = enigma::image_load(filename, &width, &height, nullptr, nullptr, &imgnumb, false);
  return enigma::sprite_add_subimage(pxdata, width, height, id, imgCount, precise, transparent, smooth, xoffset,
                                     yoffset, mipmap);
}

bool sprite_set(int index, std::string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload,
                int xoffset, int yoffset, bool mipmap) {
  sprites.assign(index,
                 std::move(make_unique<Sprite>(precise, transparent, smooth, preload, xoffset, yoffset, mipmap)));
  sprite_add_subimage(filename, sprites.size() - 1, imgnumb, precise, transparent, smooth, xoffset, yoffset, mipmap);

  return true;
}

int sprite_add(std::string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload,
               int xoffset, int yoffset, bool mipmap) {
  int id = sprites.size();

  sprites.push_back(nullptr);
  if (!sprite_set(id, filename, imgnumb, precise, transparent, smooth, preload, xoffset, yoffset, mipmap)) {
    sprites.pop_back();
    return -1;
  }

  return id;
}

int sprite_add(string filename, int imgnumb, bool transparent, bool smooth, int x_offset, int y_offset, bool mipmap) {
  return sprite_add(filename, imgnumb, false, transparent, smooth, true, x_offset, y_offset, mipmap);
}

bool sprite_replace(int ind, string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload,
                    int x_offset, int y_offset, bool free_texture, bool mipmap) {
  Sprite *spr;
  get_resb(sprites, ind, spr);

  if (free_texture) spr->freeTextures();

  sprite_set(ind, filename, imgnumb, precise, transparent, smooth, preload, x_offset, y_offset, mipmap);

  return true;
}

bool sprite_replace(int ind, string filename, int imgnumb, bool transparent, bool smooth, int x_offset, int y_offset,
                    bool free_texture, bool mipmap) {
  return sprite_replace(ind, filename, imgnumb, false, transparent, smooth, true, x_offset, y_offset, free_texture,
                        mipmap);
}

bool sprite_exists(int spr) { return sprites.exists(spr); }

void sprite_save(int ind, unsigned subimg, string fname) {
  Sprite *spr;
  get_resv(sprites, ind, spr);

  unsigned w, h;
  unsigned char *rgbdata = enigma::graphics_get_texture_pixeldata(spr->subimages[subimg].textureID, &w, &h);

  enigma::image_save(fname, rgbdata, spr->width, spr->height, w, h, false);

  delete[] rgbdata;
}

void sprite_delete(int ind, bool free_texture) {
  Sprite *spr;
  get_resv(sprites, ind, spr);
  if (free_texture) spr->freeTextures();
  sprites.remove(ind);
}

void sprite_duplicate(int ind) { sprites.duplicate(ind); }

void sprite_assign(int ind, int copy_sprite, bool free_texture) {
  Sprite *spr;
  get_resv(sprites, ind, spr);
  sprites.assign(copy_sprite, make_unique<Sprite>(*spr));
}

void sprite_set_alpha_from_sprite(int ind, int copy_sprite, bool free_texture) {
  Sprite *spr;
  get_resv(sprites, ind, spr);

  Sprite *spr2;
  get_resv(sprites, ind, spr2);

  for (unsigned i = 0; i < spr->subimages.size(); i++)
    enigma::graphics_replace_texture_alpha_from_texture(spr->subimages[i].textureID,
                                                        spr2->subimages[i % spr2->subimages.size()].textureID);
  if (free_texture) spr->freeTextures();
}

void sprite_merge(int ind, int copy_sprite) {
  Sprite *spr, *spr_copy;
  get_resv(sprites, ind, spr);
  get_resv(sprites, copy_sprite, spr_copy);

  for (unsigned j = 0; j < spr_copy->subimages.size(); ++j) {
    spr->subimages.push_back(spr_copy->subimages[j]);
  }

  spr->width = (spr->width > spr_copy->width) ? spr->width : spr_copy->width;
  spr->height = (spr->height > spr_copy->height) ? spr->height : spr_copy->height;
}

int sprite_get_width(int sprid) {
  enigma::Sprite *spr;
  get_res(sprites, sprid, spr);
  if (!spr) return 32;

  return spr->width;
}

int sprite_get_height(int sprid) {
  enigma::Sprite *spr;
  get_res(sprites, sprid, spr);
  if (!spr) return 32;

  return spr->height;
}

double sprite_get_texture_width_factor(int sprid, int subimg) {
  enigma::Sprite *spr;
  get_res(sprites, sprid, spr);
  if (!spr) return 32;

  return spr->subimages[subimg].w;
}

double sprite_get_texture_height_factor(int sprid, int subimg) {
  enigma::Sprite *spr;
  get_res(sprites, sprid, spr);
  if (!spr) return 32;

  return spr->subimages[subimg].h;
}

int sprite_get_bbox_bottom(int sprid) {
  enigma::Sprite *spr;
  get_res(sprites, sprid, spr);
  if (!spr) return 32;

  return spr->bbox[0].bottom;
}

int sprite_get_bbox_left(int sprid) {
  enigma::Sprite *spr;
  get_resi(sprites, sprid, spr);

  return spr->bbox[0].left;
}

int sprite_get_bbox_right(int sprid) {
  enigma::Sprite *spr;
  get_res(sprites, sprid, spr);
  if (!spr) return 32;

  return spr->bbox[0].right;
}

int sprite_get_bbox_top(int sprid) {
  enigma::Sprite *spr;
  get_resi(sprites, sprid, spr);

  return spr->bbox[0].top;
}

int sprite_get_bbox_bottom_relative(int sprid) {
  enigma::Sprite *spr;
  get_res(sprites, sprid, spr);
  if (!spr) return 32;

  return spr->bbox[1].bottom;
}
int sprite_get_bbox_left_relative(int sprid) {
  enigma::Sprite *spr;
  get_resi(sprites, sprid, spr);

  return spr->bbox[1].left;
}

int sprite_get_bbox_right_relative(int sprid) {
  enigma::Sprite *spr;
  get_res(sprites, sprid, spr);
  if (!spr) return 32;

  return spr->bbox[1].right;
}

int sprite_get_bbox_top_relative(int sprid) {
  enigma::Sprite *spr;
  get_resi(sprites, sprid, spr);
  return spr->bbox[1].top;
}

int sprite_get_number(int sprid) {
  enigma::Sprite *spr;
  get_resi(sprites, sprid, spr);

  return spr->subcount();
}

int sprite_get_texture(int sprid, int subimage) {
  enigma::Sprite *spr;
  get_resi(sprites, sprid, spr);
  return spr->subimages[subimage].textureID;
}

int sprite_get_xoffset(int sprid) {
  enigma::Sprite *spr;
  get_resi(sprites, sprid, spr);
  return spr->xoffset;
}

int sprite_get_yoffset(int sprid) {
  enigma::Sprite *spr;
  get_resi(sprites, sprid, spr);
  return spr->yoffset;
}

void sprite_set_offset(int sprid, int xoff, int yoff) {
  enigma::Sprite *spr;
  get_resv(sprites, sprid, spr);
  spr->xoffset = xoff;
  spr->yoffset = yoff;
}

void sprite_set_bbox_mode(int sprid, int mode) {
  enigma::Sprite *spr;
  get_resv(sprites, sprid, spr);
  spr->bbox_mode = mode;
}

int sprite_get_bbox_mode(int sprid) {
  enigma::Sprite *spr;
  get_resi(sprites, sprid, spr);
  return spr->bbox_mode;
}

void sprite_set_bbox(int sprid, int left, int top, int right, int bottom) {
  enigma::Sprite *spr;
  get_resv(sprites, sprid, spr);
  spr->bbox[0].left = left;
  spr->bbox[0].top = top;
  spr->bbox[0].right = right;
  spr->bbox[0].bottom = bottom;
  spr->bbox[1].left = left - spr->xoffset;
  spr->bbox[1].top = top - spr->yoffset;
  spr->bbox[1].right = right - spr->xoffset;
  spr->bbox[1].bottom = bottom - spr->yoffset;
}

void sprite_collision_mask(int ind, bool sepmasks, int mode, int left, int top, int right, int bottom, int kind,
                           unsigned char tolerance) {
  sprite_set_bbox(ind, left, top, right, bottom);
}

var sprite_get_uvs(int sprid, int subimg) {
  var uvs;
  uvs[4] = 0;

  enigma::Sprite *spr;
  get_res(sprites, sprid, spr);
  if (!spr) return uvs;

  uvs[0] = spr->subimages[subimg].x;
  uvs[1] = spr->subimages[subimg].y;
  uvs[2] = spr->subimages[subimg].x + spr->subimages[subimg].w;
  uvs[3] = spr->subimages[subimg].y + spr->subimages[subimg].h;
  return uvs;
}

}  // namespace enigma_user