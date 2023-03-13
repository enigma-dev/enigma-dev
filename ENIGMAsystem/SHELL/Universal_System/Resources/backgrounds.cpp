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

#include "backgrounds_internal.h"
#include "Universal_System/image_formats.h"
#include "Universal_System/nlpo2.h"
#include "Graphics_Systems/General/GScolor_macros.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Widget_Systems/widgets_mandatory.h"

using enigma::Background;
using enigma::backgrounds;
using enigma::nlpo2;
using enigma::TexRect;
using enigma::Color;
using enigma::RawImage;
using enigma::image_pad;
using enigma::graphics_create_texture;
using enigma::image_load;

namespace {

Background background_add_helper(std::string filename, bool transparent, bool smooth, bool preload, bool mipmap) {
  unsigned fullwidth, fullheight;
  std::vector<RawImage> imgs = image_load(filename);
      
  Background nb;
  nb.isTileset = false;
  
  if (imgs.empty()) {
    DEBUG_MESSAGE("ERROR - Failed to append background to index!", MESSAGE_TYPE::M_ERROR);
    return nb;
  }
  
  nb.width = imgs[0].w; 
  nb.height = imgs[0].h;

  // If background is transparent, set the alpha to zero for pixels that should be
  // transparent from lower left pixel color
  if (transparent) enigma::image_remove_color(imgs[0]);

  nb.textureID = graphics_create_texture(imgs[0], mipmap, &fullwidth, &fullheight);
  nb.textureBounds = TexRect(0, 0, static_cast<gs_scalar>(imgs[0].w) / fullwidth, static_cast<gs_scalar>(imgs[0].h) / fullheight);

  return nb;
}

}

namespace enigma_user {
  
int background_add(std::string filename, bool transparent, bool smooth, bool preload, bool mipmap) {
  return backgrounds.add(std::move(background_add_helper(filename, transparent, smooth, preload, mipmap)));
}

int background_create_color(unsigned w, unsigned h, int col, bool preload) {
  unsigned int fullwidth = nlpo2(w), fullheight = nlpo2(h);
  RawImage img(new unsigned char[fullwidth * fullheight * 4], fullwidth, fullheight);
  std::fill((unsigned*)(img.pxdata), (unsigned*)(img.pxdata) + fullwidth * fullheight, (COL_GET_R(col) | (COL_GET_G(col) << 8) | (COL_GET_B(col) << 16) | 255 << 24));
  int textureID = graphics_create_texture(img, false);
  return backgrounds.add(std::move(Background(w, h, fullwidth, fullheight, textureID)));
}

bool background_replace(int back, std::string filename, bool transparent, bool smooth, bool preload, bool free_texture,
                        bool mipmap) {
  backgrounds.get(back).FreeTexture();
  return (backgrounds.replace(back, background_add_helper(filename, transparent, smooth, preload, mipmap)) != -1);
}

void background_save(int back, std::string fname) {
  const Background& bkg = backgrounds.get(back);
  
  if (bkg.textureID < 0) {
    DEBUG_MESSAGE("Requested background: " + std::to_string(back) +
                 + " has no loaded texture.", MESSAGE_TYPE::M_USER_ERROR);
    return;
  }
  
  unsigned w, h;
  unsigned char* rgbdata =
      enigma::graphics_copy_texture_pixels(bkg.textureID, &w, &h);

  enigma::image_save(fname, rgbdata, bkg.width, bkg.height, w, h, false);

  delete[] rgbdata;
}

void background_delete(int back, bool free_texture) {
  backgrounds.get(back).FreeTexture();
  backgrounds.destroy(back);
}

int background_duplicate(int back) {
  return backgrounds.duplicate(back);
}

void background_assign(int back, int copy_background, bool free_texture) {
  backgrounds.get(back).FreeTexture();
  Background copy = backgrounds.get(copy_background);
  backgrounds.assign(back, std::move(copy));
}

bool background_exists(int back) {
  return backgrounds.exists(back);
}

// FIXME: free_texture unused
void background_set_alpha_from_background(int back, int copy_background, bool free_texture) {
  enigma::graphics_replace_texture_alpha_from_texture(backgrounds.get(back).textureID, backgrounds.get(copy_background).textureID);
}

int background_get_texture(int back) {
  return backgrounds.get(back).textureID;
}

int background_get_width(int back) {
  return backgrounds.get(back).width;
}

int background_get_height(int back) {
  return backgrounds.get(back).height;
}

var background_get_uvs(int back) {
  var uvs;
  uvs[4] = 0;

  const Background& b = backgrounds.get(back);

  uvs[0] = b.textureBounds.left();
  uvs[1] = b.textureBounds.top();
  uvs[2] = b.textureBounds.right();
  uvs[3] = b.textureBounds.bottom();
  return uvs;
}

bool background_textures_equal(int id1, int id2) {
  // Note: this will need to be amended to use textures_regions_equal when atlas support is implemented
  return textures_equal(background_get_texture(id1), background_get_texture(id2));
}

uint32_t background_get_pixel(int bkgID, unsigned x, unsigned y) {
  // Note: this will need to be amended when atlas support is implemented
  return texture_get_pixel(background_get_texture(bkgID), x, y);
}

}  // namespace enigma_user
