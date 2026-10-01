/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2013 Robert B. Colton
*** Copyright (C) 2015 Harijs Grinbergs
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

#include "sprites_internal.h"
#include "Universal_System/compliance.h"
#include "Universal_System/image_formats.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GScolor_macros.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <algorithm>
#include <climits>
#include <string>
#include <vector>

using enigma::Sprite;
using enigma::sprites;
using enigma::TexRect;
using enigma::Color;
using enigma::RawImage;

namespace {

// GM8 smooth edges: each transparent pixel, in row order, takes 32 off the alpha of
// the pixels around it that have at least 32 left (as OpenGMK's process_image).
void smooth_edges(RawImage& img) {
  const int w = img.w, h = img.h;
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++) {
      if (img.pxdata[4 * (y * w + x) + 3]) continue;
      for (int ny = std::max(y - 1, 0); ny < std::min(y + 2, h); ny++)
        for (int nx = std::max(x - 1, 0); nx < std::min(x + 2, w); nx++) {
          unsigned char &a = img.pxdata[4 * (ny * w + nx) + 3];
          if (a >= 32) a -= 32;
        }
    }
}

// Grows [l, r] x [t, b] (inclusive) to the image's visible pixels.
void grow_bbox(const RawImage& img, int& l, int& t, int& r, int& b) {
  for (unsigned y = 0; y < img.h; y++)
    for (unsigned x = 0; x < img.w; x++)
      if (img.pxdata[4 * (y * img.w + x) + 3]) {
        l = std::min(l, int(x)), r = std::max(r, int(x));
        t = std::min(t, int(y)), b = std::max(b, int(y));
      }
}

Sprite sprite_add_helper(std::string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload, int x_offset, int y_offset, bool mipmap) {
  std::vector<RawImage> imgs = enigma::image_load(filename);
  
  if (imgs.empty()) {
    DEBUG_MESSAGE("ERROR - Failed to append sprite to index!", MESSAGE_TYPE::M_ERROR);
    return Sprite();
  }
  
  unsigned cellwidth = ((imgs.size() > 1) ? imgs[0].w : imgs[0].w / imgnumb);
  Sprite ns(cellwidth, imgs[0].h, x_offset, y_offset);
  int l = INT_MAX, t = INT_MAX, r = -1, b = -1;
  auto add = [&](RawImage& i) {
    if (smooth) smooth_edges(i);
    grow_bbox(i, l, t, r, b);
    ns.AddSubimage(i, ((precise) ? enigma::ct_precise : enigma::ct_bbox), i.pxdata, mipmap);
  };
  
  if (imgs.size() == 1 && imgnumb > 1) {
    if (transparent) enigma::image_remove_color(imgs[0]);
  
    std::vector<RawImage> rawSubimages = enigma::image_split(imgs[0], imgnumb);
    for (RawImage& i : rawSubimages) add(i);
  } else {
    for (RawImage& i : imgs) {
      if (transparent) enigma::image_remove_color(i);
      add(i);
    }
  }
  // right and bottom are inclusive, as for compiled sprites; nothing visible keeps the full cell.
  if (r < 0) l = 0, t = 0, r = cellwidth - 1, b = imgs[0].h - 1;
  ns.SetBBox(l, t, r - l, b - t);
  
  return ns;
}

}

namespace enigma_user {

int sprite_get_width(int sprid) {
  if (!sprites.exists(sprid)) return -1;
  const Sprite &spr = sprites.get(sprid);
  // GM8 reports a sprite with no subimages as 1x1.
  if (spr.SubimageCount() == 0 && enigma::gm8_compliance()) return 1;
  return spr.width;
}

int sprite_get_height(int sprid) {
  if (!sprites.exists(sprid)) return -1;
  const Sprite &spr = sprites.get(sprid);
  // GM8 reports a sprite with no subimages as 1x1.
  if (spr.SubimageCount() == 0 && enigma::gm8_compliance()) return 1;
  return spr.height;
}

gs_scalar sprite_get_texture_width_factor(int sprid, int subimg) {
  const auto& spr2d = sprites.get(sprid);
  if (!spr2d.SubimageCount()) return 0;
  return spr2d.GetTextureRect(spr2d.ModSubimage(subimg)).w;
}

gs_scalar sprite_get_texture_height_factor(int sprid, int subimg) {
  const auto& spr2d = sprites.get(sprid);
  if (!spr2d.SubimageCount()) return 0;
  return spr2d.GetTextureRect(spr2d.ModSubimage(subimg)).h;
}

int sprite_get_bbox_bottom(int sprid) {
  if (!sprites.exists(sprid)) return -1;
  return sprites.get(sprid).bbox.bottom();
}

int sprite_get_bbox_left(int sprid) {
  if (!sprites.exists(sprid)) return -1;
  return sprites.get(sprid).bbox.left();
}

int sprite_get_bbox_right(int sprid) {
  if (!sprites.exists(sprid)) return -1;
  return sprites.get(sprid).bbox.right();
}

int sprite_get_bbox_top(int sprid) {
  if (!sprites.exists(sprid)) return -1;
  return sprites.get(sprid).bbox.top();
}

int sprite_get_bbox_mode(int sprid) {
  if (!sprites.exists(sprid)) return -1;
  return sprites.get(sprid).bbox_mode;
}

int sprite_get_bbox_bottom_relative(int sprid) {
  if (!sprites.exists(sprid)) return -1;
  const Sprite& spr = sprites.get(sprid);
  return spr.bbox.bottom() - spr.yoffset;
}

int sprite_get_bbox_left_relative(int sprid) {
  if (!sprites.exists(sprid)) return -1;
  const Sprite& spr = sprites.get(sprid);
  return spr.bbox.left() - spr.xoffset;
}

int sprite_get_bbox_right_relative(int sprid) {
  if (!sprites.exists(sprid)) return -1;
  const Sprite& spr = sprites.get(sprid);
  return spr.bbox.right() - spr.xoffset;
}

int sprite_get_bbox_top_relative(int sprid) {
  if (!sprites.exists(sprid)) return -1;
  const Sprite& spr = sprites.get(sprid);
  return spr.bbox.top() - spr.yoffset;
}

int sprite_get_number(int sprid) {
  if (!sprites.exists(sprid)) return -1;
  return sprites.get(sprid).SubimageCount();
}

int sprite_get_texture(int sprid, int subimage) {
  const auto& spr2d = sprites.get(sprid);
  if (!spr2d.SubimageCount()) return -1;
  return spr2d.GetTexture(spr2d.ModSubimage(subimage));
}

int sprite_get_xoffset(int sprid) {
  if (!sprites.exists(sprid)) return -1;
  return sprites.get(sprid).xoffset;
}

int sprite_get_yoffset(int sprid) {
  if (!sprites.exists(sprid)) return -1;
  return sprites.get(sprid).yoffset;
}

int sprite_add(std::string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload, int x_offset, int y_offset, bool mipmap) {
  return sprites.add(sprite_add_helper(filename, imgnumb, precise, transparent, smooth, preload, x_offset, y_offset, mipmap));
}

int sprite_add(std::string filename, int imgnumb, bool transparent, bool smooth, int x_offset, int y_offset, bool mipmap) {
  return sprite_add(filename, imgnumb, true, transparent, smooth, false, x_offset, y_offset, mipmap);
}

bool sprite_replace(int ind, std::string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload,
                    int x_offset, int y_offset, bool free_texture, bool mipmap) {
  if (free_texture) sprites.get(ind).FreeTextures();
  return (sprites.replace(ind, sprite_add_helper(filename, imgnumb, precise, transparent, smooth, preload, x_offset, y_offset, mipmap)) != -1);
}

bool sprite_replace(int ind, std::string filename, int imgnumb, bool transparent, bool smooth, int x_offset, int y_offset,
                    bool free_texture, bool mipmap) {
  return sprite_replace(ind, filename, imgnumb, true, transparent, smooth, false, x_offset, y_offset, free_texture, mipmap);
}

bool sprite_exists(int spr) {
  return sprites.exists(spr);
}

void sprite_delete(int ind, bool free_texture) {
  if (free_texture) sprites.get(ind).FreeTextures();
  sprites.destroy(ind);
}

int sprite_duplicate(int ind) {
  return sprites.duplicate(ind);  
}

void sprite_assign(int ind, int copy_sprite, bool free_texture) {
  if (free_texture) sprites.get(ind).FreeTextures();
  Sprite copy = sprites.get(copy_sprite);
  sprites.assign(ind, std::move(copy));
}

// NOTE: I doubt this method actually works properly on sprites of differing sizes
// testing needed 
void sprite_merge(int ind, int copy_sprite) {
  Sprite& spr1 = sprites.get(ind);
  const Sprite& spr2 = sprites.get(copy_sprite);
  
  // FIXME: this will break when we add functionality for removing subimages
  for (size_t i = 0; i < spr2.SubimageCount(); ++i) {
    spr1.AddSubimage(spr2.GetSubimage(i));
  }
   
  spr1.width = std::max(spr1.width, spr2.width);
  spr1.height = std::max(spr1.height, spr2.height);
}

void sprite_set_offset(int ind, int xoff, int yoff) {
  Sprite& spr = sprites.get(ind);
  spr.xoffset = xoff;
  spr.yoffset = yoff;
}

// FIXME: free_texture unused
void sprite_set_alpha_from_sprite(int ind, int copy_sprite, bool free_texture) {

  Sprite& spr = sprites.get(ind);
  const Sprite& spr_copy = sprites.get(copy_sprite);
  
  // FIXME: this will break when we add functionality for removing subimages
  for (size_t i = 0; i < spr.SubimageCount(); i++)
    enigma::graphics_replace_texture_alpha_from_texture(spr.GetTexture(i), spr_copy.GetTexture(i % spr_copy.SubimageCount()));
}


// FIXME: This sets the mode but the mode is currently meaningless
void sprite_set_bbox_mode(int ind, int mode) {
  sprites.get(ind).bbox_mode = mode;
}

void sprite_set_bbox(int ind, int left, int top, int right, int bottom) {
  enigma::BoundingBox& bb = sprites.get(ind).bbox; 
  bb.x = left;
  bb.y = top;
  bb.w = right - left;
  bb.h = bottom - top;
}

//FIXME: This only updates the bbox currently
void sprite_collision_mask(int ind, bool sepmasks, int mode, int left, int top, int right, int bottom, int kind,
                           unsigned char tolerance) {
  sprite_set_bbox(ind, left, top, right, bottom);
}

var sprite_get_uvs(int ind, int subimg) {
  var uvs;
  uvs[4] = 0;

  const enigma::Subimage& s = sprites.get(ind).GetSubimage(subimg);

  uvs[0] = s.textureBounds.left();
  uvs[1] = s.textureBounds.top();
  uvs[2] = s.textureBounds.right();
  uvs[3] = s.textureBounds.bottom();
  return uvs;
}

void sprite_save(int ind, unsigned subimg, std::string fname) {
  const Sprite& spr = sprites.get(ind);
  
  if (spr.SubimageCount() <= subimg) {
    DEBUG_MESSAGE("Requested subimage: " + std::to_string(subimg) + " out of range. Sprite: " 
                 +  std::to_string(ind) + " only has " + std::to_string(spr.SubimageCount()) 
                 + " subimages.", MESSAGE_TYPE::M_USER_ERROR);
    return;
  }
  
  unsigned w, h;
  unsigned char* rgbdata =
      enigma::graphics_copy_texture_pixels(spr.GetSubimage(subimg).textureID, &w, &h);

  enigma::image_save(fname, rgbdata, spr.width, spr.height, w, h, false);

  delete[] rgbdata;
}

//void sprite_set_precise(int ind, bool precise); //FIXME: We don't support this yet
//void sprite_save_strip(int ind, std::string fname); //FIXME: We don't support this yet

int sprite_create_color(unsigned w, unsigned h, int col) {
  RawImage img(new unsigned char[w * h * 4], w, h);
  std::fill((unsigned*)(img.pxdata), (unsigned*)(img.pxdata) + w * h, (COL_GET_R(col) | (COL_GET_G(col) << 8) | (COL_GET_B(col) << 16) | 255 << 24));

  Sprite s(w, h, 0, 0);
  s.SetBBox(0, 0, w, h);
  s.AddSubimage(img, enigma::ct_bbox, img.pxdata, false);
  return sprites.add(std::move(s));
}

bool sprite_textures_equal(int id1, int subimg1, int id2, int subimg2) {
  // Note: this will need to be amended to use textures_regions_equal when atlas support is implemented
  return textures_equal(sprite_get_texture(id1, subimg1), sprite_get_texture(id2, subimg2));
}

uint32_t sprite_get_pixel(int id, int subimg, unsigned x, unsigned y) {
  // Note: this will need to be amended when atlas support is implemented
  return texture_get_pixel(sprite_get_texture(id, subimg), x, y);
}
}
