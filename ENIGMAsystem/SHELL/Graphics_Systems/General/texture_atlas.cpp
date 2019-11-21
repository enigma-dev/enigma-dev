/** Copyright (C) 2015 Harijs Grinbergs
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

#include <algorithm>    // std::sort

#include "texture_atlas.h"
#include "texture_atlas_internal.h"

#include "Universal_System/Resources/background_internal.h"
#include "Universal_System/Resources/fonts_internal.h"
#include "Universal_System/Resources/sprites_internal.h"

#include "Universal_System/nlpo2.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "rectpacker/rectpack.h"

#ifdef DEBUG_MODE
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_sprite(spr,id) \
    if (id < -1 or size_t(id) > enigma::sprite_idmax or !enigma::spritestructarray[id]) { \
      DEBUG_MESSAGE("Cannot access sprite with id " + toString(id), MESSAGE_TYPE::M_USER_ERROR); \
      return; \
    } enigma::sprite *const spr = enigma::spritestructarray[id];
#else
  #define get_sprite(spr,id) \
    enigma::sprite *const spr = enigma::spritestructarray[id];
#endif

using std::unordered_map;
using std::vector;

namespace enigma {
  struct TextureAtlasRect{
    unsigned int metric = 0;
    unsigned int size = 0;
    TextureAtlasRect(unsigned int m, unsigned int s) : metric(m), size(s) {}
    const bool operator<(TextureAtlasRect const &rhs) const{
      return this->size < rhs.size;
    }
  };

  unordered_map<unsigned int, texture_atlas> texture_atlas_array;
  size_t texture_atlas_idmax = 0;

  bool textures_pack(int ta, bool free_textures){
    // Implement packing algorithm.
    vector<texture_element> &textures = texture_atlas_array[ta].textures;
    vector<enigma::rect_packer::pvrect> metrics(textures.size()); //This means every resource has only one image
    for (unsigned int i = 0; i < textures.size(); i++){ //This adds the rest of the images
      switch (textures[i].type){
        case 0: { //Add all sprite subimages
          enigma::sprite *sspr = enigma::spritestructarray[textures[i].id];
          for (int s = 0; s < sspr->subcount; s++){
            metrics.emplace_back();
          }
        } break;
        case 2: { //Add font glyps
          //We remove one as it's the font resource itself, but we only need glyps
          metrics.erase(metrics.begin());
          enigma::SpriteFont *fnt = &enigma::sprite_fonts[textures[i].id];
          for (size_t g = 0; g < fnt->glyphRanges.size(); g++) {
            enigma::fontglyphrange& fgr = fnt->glyphRanges[g];
            for (size_t s = 0; s < fgr.glyphs.size(); s++){
              metrics.emplace_back();
            }
          }
        } break;
        default: break; //We do nothing for the rest
      }
    }

    unsigned int counter = 0;
    for (unsigned int i = 0; i < textures.size(); i++){
      // TODO: This should maybe crop the images so it's totally fit (this can be done on compile time by LGM or compiler even, but it's possible the user has loaded the image at runtime)
      switch (textures[i].type){
        case 0: { //Metrics all sprite subimages
          enigma::sprite *sspr = enigma::spritestructarray[textures[i].id];
          for (int s = 0; s < sspr->subcount; s++){
            metrics[counter].w = sspr->width, metrics[counter].h = sspr->height;
            counter++;
          }
        } break;
        case 1: { //Metrics for backgrounds
          enigma::background *bkg = enigma::backgroundstructarray[textures[i].id];
          metrics[counter].w = bkg->width, metrics[counter].h = bkg->height;
          counter++;
        } break;
        case 2: { //Metrics for font glyps
          enigma::SpriteFont *fnt = &enigma::sprite_fonts[textures[i].id];
          for (size_t g = 0; g < fnt->glyphRanges.size(); g++) {
            enigma::fontglyphrange& fgr = fnt->glyphRanges[g];
            for (size_t s = 0; s < fgr.glyphs.size(); s++){
              metrics[counter].w = fgr.glyphs[s].x2-fgr.glyphs[s].x, metrics[counter].h = fgr.glyphs[s].y2-fgr.glyphs[s].y;
              counter++;
            }
          }
        } break;
        default: break; //We do nothing for the rest (there shouldn't be any "rest")
      }
    }

    vector<TextureAtlasRect> boxes;
    boxes.reserve(metrics.size());
    for (unsigned int i = 0; i < metrics.size(); i++){
      boxes.emplace_back(i,metrics[i].w * metrics[i].h);
    }
    std::sort(boxes.begin(), boxes.end());

    unsigned int w = 64, h = 64; //Minimum atlas size
    if (texture_atlas_array[ta].width > 0 && texture_atlas_array[ta].height > 0){
      w = texture_atlas_array[ta].width, h = texture_atlas_array[ta].height;
    }
    enigma::rect_packer::rectpnode *rectplane = new enigma::rect_packer::rectpnode(0,0,w,h);
    unsigned int max_textures = 0;
    for (auto i = boxes.rbegin(); i != boxes.rend();){
      unsigned int indx = (*i).metric;
      enigma::rect_packer::rectpnode *nn = enigma::rect_packer::rninsert(rectplane, indx, &metrics.front());
      if (nn){
        enigma::rect_packer::rncopy(nn, &metrics.front(), indx),
        i++;
        max_textures++;
      } else {
        if (texture_atlas_array[ta].width != -1 && texture_atlas_array[ta].height != -1){
          break; //We are already at max size
        }
        w > h ? h <<= 1 : w <<= 1,
        rectplane = enigma::rect_packer::expand(rectplane, w, h);
        if (!w or !h) return false;
      }
    }

    if (texture_atlas_array[ta].width == -1 || texture_atlas_array[ta].height == -1){
      texture_atlas_array[ta].width = w;
      texture_atlas_array[ta].height = h;
      if (enigma::texture_atlas_array[ta].texture != -1){
        graphics_delete_texture(texture_atlas_array[ta].texture);
      }
      texture_atlas_array[ta].texture = graphics_create_texture(w, h, w, h, nullptr, false);
    }

    counter = 0;
    for (unsigned int i = 0; i < textures.size(); i++){
      switch (textures[i].type){
        case 0: { //Copy textures for all sprite subimages
          enigma::sprite *sspr = enigma::spritestructarray[textures[i].id];
          for (int s = 0; s < sspr->subcount; s++){
            enigma::graphics_copy_texture(sspr->texturearray[s], enigma::texture_atlas_array[ta].texture, metrics[counter].x, metrics[counter].y);
            if (free_textures == true){
              enigma::graphics_delete_texture(sspr->texturearray[s]);
            }
            sspr->texturearray[s] = enigma::texture_atlas_array[ta].texture;
            sspr->texturexarray[s] = (double)metrics[counter].x/(double)(enigma::texture_atlas_array[ta].width);
            sspr->textureyarray[s] = (double)metrics[counter].y/(double)(enigma::texture_atlas_array[ta].height);
            sspr->texturewarray[s] = (double)sspr->width/(double)(enigma::texture_atlas_array[ta].width);
            sspr->textureharray[s] = (double)sspr->height/(double)(enigma::texture_atlas_array[ta].height);
            counter++;
            if (counter > max_textures) { return false; }
          }
        } break;
        case 1: { //Copy textures for all the backgrounds
          enigma::background *bkg = enigma::backgroundstructarray[textures[i].id];
          enigma::graphics_copy_texture(bkg->texture, enigma::texture_atlas_array[ta].texture, metrics[counter].x, metrics[counter].y);
          if (free_textures == true){
            enigma::graphics_delete_texture(bkg->texture);
          }
          bkg->texture = enigma::texture_atlas_array[ta].texture;
          bkg->texturex = (double)metrics[counter].x/(double)(enigma::texture_atlas_array[ta].width);
          bkg->texturey = (double)metrics[counter].y/(double)(enigma::texture_atlas_array[ta].height);
          bkg->texturew = (double)bkg->width/(double)(enigma::texture_atlas_array[ta].width);
          bkg->textureh = (double)bkg->height/(double)(enigma::texture_atlas_array[ta].height);

          counter++;
          if (counter > max_textures) { return false; }
        } break;
        case 2: { //Copy textures for all font glyps
          ///This sometimes draws cut of letters - need to investigate!
          enigma::SpriteFont *fnt = &enigma::sprite_fonts[textures[i].id];
          for (size_t g = 0; g < fnt->glyphRanges.size(); g++) {
            enigma::fontglyphrange& fgr = fnt->glyphRanges[g];
            for (size_t s = 0; s < fgr.glyphs.size(); s++){
              double tix, tiy; //Calculate texture position in image space (pixels) instead of normalized 0-1. We sadly don't hold this information, but maybe we should
              tix = (double)fgr.glyphs[s].tx*(double)fnt->twid;
              tiy = (double)fgr.glyphs[s].ty*(double)fnt->thgt;

              int gw = fgr.glyphs[s].x2-fgr.glyphs[s].x;
              int gh = fgr.glyphs[s].y2-fgr.glyphs[s].y;

              enigma::graphics_copy_texture_part(fnt->texture, enigma::texture_atlas_array[ta].texture, tix, tiy, gw, gh, metrics[counter].x, metrics[counter].y);

              fgr.glyphs[s].tx = (double)metrics[counter].x/(double)(enigma::texture_atlas_array[ta].width);
              fgr.glyphs[s].ty = (double)metrics[counter].y/(double)(enigma::texture_atlas_array[ta].height);
              fgr.glyphs[s].tx2 = (double)(metrics[counter].x+gw)/(double)(enigma::texture_atlas_array[ta].width);
              fgr.glyphs[s].ty2 = (double)(metrics[counter].y+gh)/(double)(enigma::texture_atlas_array[ta].height);

              counter++;
              if (counter > max_textures) { return false; }
            }
          }
          if (free_textures == true){
            enigma::graphics_delete_texture(fnt->texture);
          }
          fnt->texture = enigma::texture_atlas_array[ta].texture;
        } break;
        default: break; //We do nothing for the rest
      }
    }
    return true;
  }
}

namespace enigma_user {
  int texture_atlas_create(int w, int h){
    size_t id = enigma::texture_atlas_idmax;

    bool found_empty = false;
    for (unsigned int i=0; i<enigma::texture_atlas_idmax; ++i){ //Find first empty slot
      if (enigma::texture_atlas_array.find(i) == enigma::texture_atlas_array.end()){
        id = i;
        found_empty = true;
        break;
      }
    }
    if (found_empty == false){ enigma::texture_atlas_idmax++; }

    enigma::texture_atlas_array.emplace(id,enigma::texture_atlas());

    if (w != -1 && h != -1){ //If we set the size manually
      unsigned int fullwidth = enigma::nlpo2dc(w)+1, fullheight = enigma::nlpo2dc(h)+1; //We only take power of two
      enigma::texture_atlas_array[id].width = fullwidth;
      enigma::texture_atlas_array[id].height = fullheight;
      enigma::texture_atlas_array[id].texture = enigma::graphics_create_texture(fullwidth, fullheight, fullwidth, fullheight, nullptr, false);
    }else{
      enigma::texture_atlas_array[id].width = -1;
      enigma::texture_atlas_array[id].height = -1;
      enigma::texture_atlas_array[id].texture = -1;
    }

    return id;
  }

  void texture_atlas_delete(int id){
    ///TODO: NEEDS ERROR CHECKING
    enigma::graphics_delete_texture(enigma::texture_atlas_array[id].texture);
    enigma::texture_atlas_array.erase(id);
  }

  //Manually add sprite at position (NOT RECOMMENDED)
  void texture_atlas_add_sprite_position(int tp, int sprid, int subimg, int x, int y, bool free_texture){
    ///TODO: NEEDS ERROR CHECKING
    get_sprite(spr, sprid);
    enigma::graphics_copy_texture(spr->texturearray[subimg], enigma::texture_atlas_array[tp].texture, x, y);
    if (free_texture == true){
      enigma::graphics_delete_texture(spr->texturearray[subimg]);
    }
    spr->texturearray[subimg] = enigma::texture_atlas_array[tp].texture;
    spr->texturexarray[subimg] = (double)x/(double)(enigma::texture_atlas_array[tp].width);
    spr->textureyarray[subimg] = (double)y/(double)(enigma::texture_atlas_array[tp].height);
    spr->texturewarray[subimg] = (double)spr->width/(double)(enigma::texture_atlas_array[tp].width);
    spr->textureharray[subimg] = (double)spr->height/(double)(enigma::texture_atlas_array[tp].height);
  }
}
