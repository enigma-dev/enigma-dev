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
#include "Universal_System/image_formats.h"
#include "Universal_System/nlpo2.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <string>
#include <iostream>

using enigma::Sprite;
using enigma::sprites;
using enigma::nlpo2dc;
using enigma::TexRect;
using enigma::Color;
using enigma::RawImage;

namespace {

Sprite sprite_add_helper(std::string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload, int x_offset, int y_offset, bool mipmap) {
  unsigned int width, height, fullwidth, fullheight;
  unsigned char *pxdata = enigma::image_load(
      filename, &width, &height, &fullwidth, &fullheight, &imgnumb, false);
  
  unsigned cellwidth = width / imgnumb;
  Sprite ns(cellwidth, height, x_offset, y_offset);
  ns.SetBBox(0, 0, cellwidth, height);

  if (pxdata == NULL) {
    DEBUG_MESSAGE("ERROR - Failed to append sprite to index!", MESSAGE_TYPE::M_ERROR);
    return ns;
  }

  // If sprite transparent, set the alpha to zero for pixels that should be
  // transparent from lower left pixel color
  if (transparent) {
    Color c = enigma::image_get_pixel_color(pxdata, width, height, 0, height - 1);
    enigma::image_swap_color(pxdata, width, height, c, Color {0, 0, 0, 0});
  }
  
  std::vector<RawImage> rawSubimages = enigma::image_split(pxdata, width, height, imgnumb);
  for (const RawImage& i : rawSubimages) {
    ns.AddSubimage(i.pxdata, i.w, i.h, ((precise) ? enigma::ct_precise : enigma::ct_bbox), i.pxdata, mipmap);
  }
  
  delete[] pxdata;
  
  return ns;
}

}

namespace enigma_user {

int sprite_get_width(int sprid) {
  return sprites.get(sprid).width;
}

int sprite_get_height(int sprid) {
  return sprites.get(sprid).height;
}

gs_scalar sprite_get_texture_width_factor(int sprid, int subimg) {
  return sprites.get(sprid).GetTextureRect(subimg).w;
}

gs_scalar sprite_get_texture_height_factor(int sprid, int subimg) {
  return sprites.get(sprid).GetTextureRect(subimg).h;
}

int sprite_get_bbox_bottom(int sprid) {
  return sprites.get(sprid).bbox.bottom();
}

int sprite_get_bbox_left(int sprid) {
  return sprites.get(sprid).bbox.left();
}

int sprite_get_bbox_right(int sprid) {
  return sprites.get(sprid).bbox.right();
}

int sprite_get_bbox_top(int sprid) {
  return sprites.get(sprid).bbox.top();
}

int sprite_get_bbox_mode(int sprid) {
  return sprites.get(sprid).bbox_mode;
}

int sprite_get_bbox_bottom_relative(int sprid) {
  const Sprite& spr = sprites.get(sprid);
  return spr.bbox.bottom() - spr.yoffset;
}

int sprite_get_bbox_left_relative(int sprid) {
  const Sprite& spr = sprites.get(sprid);
  return spr.bbox.left() - spr.xoffset;
}

int sprite_get_bbox_right_relative(int sprid) {
  const Sprite& spr = sprites.get(sprid);
  return spr.bbox.right() - spr.xoffset;
}

int sprite_get_bbox_top_relative(int sprid) {
  const Sprite& spr = sprites.get(sprid);
  return spr.bbox.top() - spr.yoffset;
}

int sprite_get_number(int sprid) {
  return sprites.get(sprid).SubimageCount();
}

int sprite_get_texture(int sprid, int subimage) {
  return sprites.get(sprid).GetTexture(subimage);
}

int sprite_get_xoffset(int sprid) {
  return sprites.get(sprid).xoffset;
}

int sprite_get_yoffset(int sprid) {
  return sprites.get(sprid).yoffset;
}

int sprite_add(std::string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload, int x_offset, int y_offset, bool mipmap) {
  return sprites.add(sprite_add_helper(filename, imgnumb, precise, transparent, smooth, preload, x_offset, y_offset, mipmap));
}

int sprite_add(std::string filename, int imgnumb, bool transparent, bool smooth, int x_offset, int y_offset, bool mipmap) {
  return sprite_add(filename, imgnumb, false, transparent, smooth, false, x_offset, y_offset, mipmap);
}

bool sprite_replace(int ind, std::string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload,
                    int x_offset, int y_offset, bool free_texture, bool mipmap) {
  if (free_texture) sprites.get(ind).FreeTextures();
  return (sprites.replace(ind, sprite_add_helper(filename, imgnumb, precise, transparent, smooth, preload, x_offset, y_offset, mipmap)) != -1);
}

bool sprite_replace(int ind, std::string filename, int imgnumb, bool transparent, bool smooth, int x_offset, int y_offset,
                    bool free_texture, bool mipmap) {
  return sprite_replace(ind, filename, imgnumb, false, transparent, smooth, false, x_offset, y_offset, free_texture, mipmap);
}

bool sprite_exists(int spr) {
  return sprites.exists(spr);
}

void sprite_delete(int ind, bool free_texture = true) {
  if (free_texture) sprites.get(ind).FreeTextures();
  sprites.destroy(ind);
}

int sprite_duplicate(int ind) {
  return sprites.duplicate(ind);  
}

void sprite_assign(int ind, int copy_sprite, bool free_texture = true) {
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
  uvs[1] = s.textureBounds.right();
  uvs[2] = s.textureBounds.right();
  uvs[3] = s.textureBounds.bottom();
  return uvs;
}

void sprite_save(int ind, unsigned subimg, std::string fname) {
  const Sprite& spr = sprites.get(ind);
  
  unsigned w, h;
  unsigned char* rgbdata =
      enigma::graphics_copy_texture_pixels(spr.GetSubimage(subimg).textureID, &w, &h);

  enigma::image_save(fname, rgbdata, spr.width, spr.height, w, h, false);

  delete[] rgbdata;
}

//void sprite_set_precise(int ind, bool precise); //FIXME: We don't support this yet
//void sprite_save_strip(int ind, std::string fname); //FIXME: We don't support this yet
}
