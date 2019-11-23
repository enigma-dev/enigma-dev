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

#include "Universal_System/estring.h"
#include "Universal_System/Object_Tiers/graphics_object.h"
#include "Universal_System/image_formats.h"
#include "libEGMstd.h"
#include "sprites_internal.h"

#include "Collision_Systems/collision_mandatory.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Universal_System/Instances/instance_system.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <cstring>
#include <string>

using std::string;

#define get_current_instance() \
    ((enigma::object_graphics*) enigma::instance_event_iterator->inst)

bool get_sprite(enigma::sprite* &spr, int id)
{
#ifdef DEBUG_MODE
    if (id < -1 || size_t(id) > enigma::sprite_idmax || !enigma::spritestructarray[id]) {
        DEBUG_MESSAGE("Cannot access sprite with id " + toString(id), MESSAGE_TYPE::M_ERROR);
        return false;
    }
#endif
    spr = enigma::spritestructarray[id];
    return true;
}

namespace enigma {
  sprite** spritestructarray;
  extern size_t sprite_idmax;
  sprite::sprite() {}
  sprite::sprite(int x) {}
    
bool get_sprite_mtx(enigma::sprite* &spr, int id) {
  bool rtn = get_sprite(spr, id);
  if (rtn) {
    // TODO: Lock a lock by reference and allow it to be timely destructed and released
  }
  return rtn;
}
}

namespace enigma_user
{

int sprite_add(string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload, int x_offset, int y_offset, bool mipmap)
{
    enigma::spritestructarray_reallocate();
    enigma::sprite *spr = enigma::spritestructarray[enigma::sprite_idmax] = new enigma::sprite();
    enigma::sprite_add_to_index(spr, filename, imgnumb, precise, transparent, smooth, x_offset, y_offset, mipmap);
    return enigma::sprite_idmax++;
}

int sprite_add(string filename, int imgnumb, bool transparent, bool smooth, int x_offset, int y_offset, bool mipmap)
{
    return sprite_add(filename, imgnumb, false, transparent, smooth, true, x_offset, y_offset, mipmap);
}

bool sprite_replace(int ind, string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload, int x_offset, int y_offset, bool free_texture, bool mipmap)
{
    enigma::sprite *spr;
    if (!get_sprite_mtx(spr, ind))
        return false;

  if (free_texture) {
    for (int ii = 0; ii < spr->subcount; ii++) {
      enigma::graphics_delete_texture(spr->texturearray[ii]);
    }
  }

    spr->texturearray.clear();
    spr->texturexarray.clear();
    spr->textureyarray.clear();
    spr->texturewarray.clear();
    spr->textureharray.clear();
  enigma::sprite_add_to_index(spr, filename, imgnumb, precise, transparent,
      smooth, x_offset, y_offset, mipmap);
    return true;
}

bool sprite_replace(int ind, string filename, int imgnumb, bool transparent, bool smooth, int x_offset, int y_offset, bool free_texture, bool mipmap)
{
  return sprite_replace(ind, filename, imgnumb, false, transparent, smooth,
      true, x_offset, y_offset, free_texture, mipmap);
}

bool sprite_exists(int spr) {
    return (unsigned(spr) < enigma::sprite_idmax) and bool(enigma::spritestructarray[spr]);
}

void sprite_save(int ind, unsigned subimg, string fname) {
    enigma::sprite *spr;
    if (!get_sprite_mtx(spr, ind))
        return;

  unsigned w, h;
  unsigned char* rgbdata =
      enigma::graphics_copy_texture_pixels(spr->texturearray[subimg], &w, &h);

  enigma::image_save(fname, rgbdata, spr->width, spr->height, w, h, false);

  delete[] rgbdata;
}

void sprite_save_strip(int ind, string fname) {

}

void sprite_delete(int ind, bool free_texture)
{
    enigma::sprite* spr;
    if (!get_sprite_mtx(spr, ind))
        return;

    if (free_texture)
        for (int ii = 0; ii < spr->subcount; ii++)
            enigma::graphics_delete_texture(spr->texturearray[ii]);

    delete enigma::spritestructarray[ind];
    enigma::spritestructarray[ind] = NULL;
}

int sprite_duplicate(int copy_sprite)
{
    enigma::sprite* spr_copy;
    if (!get_sprite_mtx(spr_copy, copy_sprite))
        return -1;

    enigma::spritestructarray_reallocate();
    enigma::sprite *spr = enigma::spritestructarray[enigma::sprite_idmax] = new enigma::sprite();
    spr->id = enigma::sprite_idmax;
    enigma::sprite_add_copy(spr, spr_copy);
    return enigma::sprite_idmax++;
}

void sprite_assign(int ind, int copy_sprite, bool free_texture)
{
    enigma::sprite *spr, *spr_copy;
    if (!get_sprite_mtx(spr, ind))
        return;
    if (!get_sprite_mtx(spr_copy, copy_sprite))
        return;

    if (free_texture)
        for (int ii = 0; ii < spr->subcount; ii++)
            enigma::graphics_delete_texture(spr->texturearray[ii]);

    spr->texturearray.clear();
    spr->texturexarray.clear();
    spr->textureyarray.clear();
    spr->texturewarray.clear();
    spr->textureharray.clear();
    enigma::sprite_add_copy(spr, spr_copy);
}

void sprite_set_alpha_from_sprite(int ind, int copy_sprite, bool free_texture)
{
    enigma::sprite *spr, *spr_copy;
    if (!get_sprite_mtx(spr, ind))
        return;
    if (!get_sprite_mtx(spr_copy, copy_sprite))
        return;

    for (int i = 0; i < spr->subcount; i++)
        enigma::graphics_replace_texture_alpha_from_texture(spr->texturearray[i], spr_copy->texturearray[i % spr_copy->subcount]);
}

void sprite_merge(int ind, int copy_sprite)
{
    enigma::sprite *spr, *spr_copy;
    if (!get_sprite_mtx(spr, ind))
        return;
    if (!get_sprite_mtx(spr_copy, copy_sprite))
        return;

    int i = 0, j = 0, t_subcount = spr->subcount + spr_copy->subcount;
    while (j < spr_copy->subcount)
    {
        spr->texturearray.push_back(enigma::graphics_duplicate_texture(spr_copy->texturearray[j]));
        spr->texturexarray.push_back(spr_copy->texturexarray[j]);
        spr->textureyarray.push_back(spr_copy->textureyarray[j]);
        spr->texturewarray.push_back(spr_copy->texturewarray[j]);
        spr->textureharray.push_back(spr_copy->textureharray[j]);
        i++; j++;
    }
    spr->subcount = t_subcount;
    spr->width    = (spr->width > spr_copy->width)?spr->width:spr_copy->width;
    spr->height   = (spr->height > spr_copy->height)?spr->height:spr_copy->height;
}

}

/* These functions are primarily for use of the engine. Experienced users
 * who are familiar with C++ can make use of these, but they were made to
 * use at load time with data read from the executable. These both expect
 * RAW format, RGB only.
 */
#include "Universal_System/nlpo2.h"

namespace enigma
{
  // INVARIANT: Should always be equal to the actual size of spritestructarray.
  size_t spritestructarray_actualsize = 0;

  //Allocates and zero-fills the array at game start
  void sprites_init() {
    spritestructarray_actualsize = sprite_idmax+1;
    spritestructarray = new sprite*[spritestructarray_actualsize];
    for (unsigned i = 0; i < sprite_idmax; i++)
      spritestructarray[i] = NULL;
  }

  void spritestructarray_reallocate() {
    if (spritestructarray_actualsize > sprite_idmax + 2) {
      return; // Ignore reallocation request if array is big enough.
        }
        sprite** spriteold = spritestructarray;
    // Grow by doubling size, minimizing needed reallocations.
    spritestructarray_actualsize = (sprite_idmax + 2) * 2;
        spritestructarray = new sprite*[spritestructarray_actualsize];
    for (size_t i = 0; i < sprite_idmax; i++) {
      spritestructarray[i] = spriteold[i];
    }
        delete[] spriteold;
    }

    //Adds an empty sprite to the list
  int sprite_new_empty(unsigned sprid, unsigned subc, int w, int h,
      int x, int y, int bbt, int bbb, int bbl, int bbr, bool pl, bool sm) {
        sprite *as = new sprite(subc);
        spritestructarray[sprid] = as;

        as->id = sprid;
        as->subcount = subc;
        as->width  = w;
        as->height = h;
        as->bbox.bottom  = bbb;
          as->bbox.left  = bbl;
          as->bbox.top   = bbt;
          as->bbox.right = bbr;
        as->bbox_relative.bottom  = bbb - y;
          as->bbox_relative.left  = bbl - x;
          as->bbox_relative.top   = bbt - y;
          as->bbox_relative.right = bbr - x;
        as->xoffset = x;
        as->yoffset = y;

        if (sprite_idmax < sprid+1)
          sprite_idmax = sprid+1;

        return sprid;
    }
    
  void sprite_add_to_index_from_buffer(sprite *ns, unsigned char *pxdata, int imgnumb,
      bool precise, bool transparent, bool smooth, int x_offset, int y_offset, bool mipmap, unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight) {
   if (pxdata == NULL) {
      DEBUG_MESSAGE("ERROR - Failed to append sprite to index!", MESSAGE_TYPE::M_ERROR);
      return;
    }

    // If sprite transparent, set the alpha to zero for pixels that should be
    // transparent from lower left pixel color
        if (transparent)
        {
          int t_pixel_b = pxdata[(height-1)*fullwidth*4];
          int t_pixel_g = pxdata[(height-1)*fullwidth*4+1];
          int t_pixel_r = pxdata[(height-1)*fullwidth*4+2];
          unsigned int ih, iw;
      for (ih = 0; ih < height; ih++) {
            int tmp = ih*fullwidth*4;
            for (iw = 0; iw < width; iw++)
            {
          if (pxdata[tmp] == t_pixel_b
              && pxdata[tmp+1] == t_pixel_g
              && pxdata[tmp+2] == t_pixel_r) {
                pxdata[tmp+3] = 0;
          }

              tmp+=4;
            }
          }
        }

        unsigned cellwidth = width/imgnumb;
    unsigned fullcellwidth = nlpo2dc(cellwidth) + 1;

        ns->id = sprite_idmax;
        ns->subcount  = imgnumb;
        ns->width     = cellwidth;
        ns->height    = height;
        // FIXME: Calculate and assign correct bbox values.
        int bbb = height;
        int bbl = 0;
        int bbt = 0;
        int bbr = cellwidth;
        ns->bbox.bottom  = bbb;
          ns->bbox.left  = bbl;
          ns->bbox.top   = bbt;
          ns->bbox.right = bbr;
        ns->bbox_relative.bottom  = bbb - y_offset;
          ns->bbox_relative.left  = bbl - x_offset;
          ns->bbox_relative.top   = bbt - y_offset;
          ns->bbox_relative.right = bbr - x_offset;
        ns->xoffset   = (int)x_offset;
        ns->yoffset   = (int)y_offset;
        ns->smooth = smooth;

        unsigned char* pixels=new unsigned char[fullcellwidth*fullheight*4]();
        for (int ii = 0; ii < imgnumb; ii++)
        {
      unsigned ih,iw;
      unsigned xcelloffset = ii * cellwidth * 4;
      for (ih = 0; ih < height; ih++)
      {
        unsigned tmp = ih * fullwidth * 4 + xcelloffset;
        unsigned tmpcell = ih * fullcellwidth * 4;
        for (iw = 0; iw < cellwidth; iw++)
        {
          pixels[tmpcell+0] = pxdata[tmp+0];
          pixels[tmpcell+1] = pxdata[tmp+1];
          pixels[tmpcell+2] = pxdata[tmp+2];
          pixels[tmpcell+3] = pxdata[tmp+3];
          tmp += 4;
          tmpcell += 4;
        }
      }
      unsigned texture = graphics_create_texture(
          cellwidth, height, fullcellwidth, fullheight, pixels, mipmap);
      ns->texturearray.push_back(texture);
          ns->texturexarray.push_back((double) 0);
          ns->textureyarray.push_back((double) 0);
          ns->texturewarray.push_back((double) cellwidth/fullcellwidth);
          ns->textureharray.push_back((double) height/fullheight);

      collision_type coll_type = precise ? ct_precise : ct_bbox;
      ns->colldata.push_back(get_collision_mask(ns,(unsigned char*)pixels,coll_type));
        }
        delete[] pixels;
  }

  void sprite_add_to_index(sprite *ns, string filename, int imgnumb,
      bool precise, bool transparent, bool smooth, int x_offset, int y_offset,
      bool mipmap) {

        unsigned int width, height, fullwidth, fullheight;
    unsigned char *pxdata = image_load(
        filename, &width, &height, &fullwidth, &fullheight, &imgnumb, false);
      
      if (pxdata == NULL) {
        DEBUG_MESSAGE("ERROR - Failed to append sprite to index!", MESSAGE_TYPE::M_ERROR);
        return;
      }
      
      sprite_add_to_index_from_buffer(ns, pxdata, imgnumb, precise, transparent, smooth, x_offset, y_offset, mipmap, width, height, fullwidth, fullheight);
      delete[] pxdata;
    }

  void sprite_add_copy(sprite *spr, sprite *spr_copy) {
        spr->subcount  = spr_copy->subcount;
        spr->width     = spr_copy->width;
        spr->height    = spr_copy->height;
        spr->xoffset   = spr_copy->xoffset;
        spr->yoffset   = spr_copy->yoffset;
        spr->bbox.bottom  = spr_copy->bbox.bottom;
        spr->bbox.left  = spr_copy->bbox.left;
        spr->bbox.top   = spr_copy->bbox.top;
        spr->bbox.right = spr_copy->bbox.right;
        spr->bbox_relative.bottom  = spr_copy->bbox_relative.bottom;
        spr->bbox_relative.left  = spr_copy->bbox_relative.left;
        spr->bbox_relative.top   = spr_copy->bbox_relative.top;
        spr->bbox_relative.right = spr_copy->bbox_relative.right;
        spr->smooth = spr_copy->smooth;


        for (int i = 0; i < spr->subcount; i++)
        {
            spr->texturearray.push_back(graphics_duplicate_texture(spr_copy->texturearray[i]));
            spr->texturexarray.push_back(spr_copy->texturexarray[i]);
            spr->textureyarray.push_back(spr_copy->textureyarray[i]);
            spr->texturewarray.push_back(spr_copy->texturewarray[i]);
            spr->textureharray.push_back(spr_copy->textureharray[i]);
        }
    }

  //Sets the subimage
  void sprite_set_subimage(int sprid, int imgindex,
      unsigned int w, unsigned int h, unsigned char* chunk,
      unsigned char* collision_data, collision_type ct) {
    unsigned int fullwidth = nlpo2dc(w)+1, fullheight = nlpo2dc(h)+1;
    char *imgpxdata = new char[4*fullwidth*fullheight+1], *imgpxptr = imgpxdata;
    unsigned int rowindex,colindex;
    for (rowindex = 0; rowindex < h; rowindex++)
    {
      for(colindex = 0; colindex < w; colindex++)
      {
        *imgpxptr++ = *chunk++;
        *imgpxptr++ = *chunk++;
        *imgpxptr++ = *chunk++;
        *imgpxptr++ = *chunk++;
      }
      memset(imgpxptr, 0, (fullwidth-colindex) << 2);
      imgpxptr += (fullwidth-colindex) << 2;
    }
    memset(imgpxptr,0,(fullheight-h) * fullwidth);

    unsigned texture =
        graphics_create_texture(w, h, fullwidth, fullheight, imgpxdata, false);

    sprite* sprstr = spritestructarray[sprid];

    sprstr->texturearray.push_back(texture);
    sprstr->texturexarray.push_back(0.0);
    sprstr->textureyarray.push_back(0.0);
    sprstr->texturewarray.push_back((double) w/fullwidth);
    sprstr->textureharray.push_back((double) h/fullheight);
    sprstr->colldata.push_back(get_collision_mask(sprstr,collision_data,ct));

    delete[] imgpxdata;
  }

  //Appends a subimage
  void sprite_add_subimage(int sprid, unsigned int w, unsigned int h,
      unsigned char* chunk, unsigned char* collision_data, collision_type ct) {
  unsigned int fullwidth = nlpo2dc(w)+1, fullheight = nlpo2dc(h)+1;
    char *imgpxdata = new char[4*fullwidth*fullheight+1], *imgpxptr = imgpxdata;
    unsigned int rowindex,colindex;
    for (rowindex = 0; rowindex < h; rowindex++)
    {
      for(colindex = 0; colindex < w; colindex++)
      {
        *imgpxptr++ = *chunk++;
        *imgpxptr++ = *chunk++;
        *imgpxptr++ = *chunk++;
        *imgpxptr++ = *chunk++;
      }
      memset(imgpxptr, 0, (fullwidth-colindex) << 2);
      imgpxptr += (fullwidth-colindex) << 2;
    }
    memset(imgpxptr,0,(fullheight-h) * fullwidth);

  unsigned texture =
      graphics_create_texture(w, h, fullwidth,fullheight,imgpxdata,false);

    sprite* sprstr = spritestructarray[sprid];

    sprstr->texturearray.push_back(texture);
    sprstr->texturexarray.push_back(0.0);
    sprstr->textureyarray.push_back(0.0);
    sprstr->texturewarray.push_back((double) w/fullwidth);
    sprstr->textureharray.push_back((double) h/fullheight);
    sprstr->colldata.push_back(get_collision_mask(sprstr,collision_data,ct));

  sprstr->subcount += 1;

    delete[] imgpxdata;
  }

  bbox_rect_t dummy_bbox = {32,0,32,0};

  const bbox_rect_t &sprite_get_bbox(int sprid)
  {
    sprite *spr;
    if (!get_sprite(spr,sprid))
      return dummy_bbox;

    return spr->bbox;
  }

  const bbox_rect_t &sprite_get_bbox_relative(int sprid)
  {
    sprite *spr;
    if (!get_sprite(spr,sprid))
      return dummy_bbox;

    return spr->bbox_relative;
  }
}

namespace enigma_user
{

int sprite_get_width(int sprid)
{
  enigma::sprite *spr;
  if (!get_sprite(spr,sprid))
    return 32;

  return spr->width;
}

int sprite_get_height(int sprid)
{
  enigma::sprite *spr;
  if (!get_sprite(spr,sprid))
    return 32;

  return spr->height;
}

double sprite_get_texture_width_factor(int sprid, int subimg)
{
  enigma::sprite *spr;
  if (!get_sprite(spr,sprid))
    return 32;

  return spr->texturewarray[subimg];
}

double sprite_get_texture_height_factor(int sprid, int subimg)
{
  enigma::sprite *spr;
  if (!get_sprite(spr,sprid))
    return 32;

  return spr->textureharray[subimg];
}

int sprite_get_bbox_bottom(int sprid)
{
  enigma::sprite *spr;
  if (!get_sprite(spr,sprid))
    return 32;

  return spr->bbox.bottom;
}
int sprite_get_bbox_left(int sprid)
{
  enigma::sprite *spr;
  if (!get_sprite(spr,sprid))
    return 0;

  return spr->bbox.left;
}
int sprite_get_bbox_right(int sprid)
{
  enigma::sprite *spr;
  if (!get_sprite(spr,sprid))
    return 32;

  return spr->bbox.right;
}
int sprite_get_bbox_top(int sprid)
{
  enigma::sprite *spr;
  if (!get_sprite(spr,sprid))
    return 0;

  return spr->bbox.top;
}

int sprite_get_bbox_bottom_relative(int sprid)
{
  enigma::sprite *spr;
  if (!get_sprite(spr,sprid))
    return 32;

  return spr->bbox_relative.bottom;
}
int sprite_get_bbox_left_relative(int sprid)
{
  enigma::sprite *spr;
  if (!get_sprite(spr,sprid))
    return 0;

  return spr->bbox_relative.left;
}

int sprite_get_bbox_right_relative(int sprid)
{
  enigma::sprite *spr;
  if (!get_sprite(spr,sprid))
    return 32;

  return spr->bbox_relative.right;
}
int sprite_get_bbox_top_relative(int sprid)
{
  enigma::sprite *spr;
  if (!get_sprite(spr,sprid))
    return 0;

  return spr->bbox_relative.top;
}

int sprite_get_number(int sprid)
{
  if (sprid == -1) return 0;

  enigma::sprite *spr;
  if (!get_sprite(spr,sprid))
    return 0;

  return spr->subcount;
}

int sprite_get_texture(int sprid,int subimage)
{
  enigma::sprite *spr;
  if (!get_sprite(spr,sprid))
    return 0;

  const int usi = subimage >= 0
      ? (subimage % spr->subcount)
      : int(get_current_instance()->image_index) % spr->subcount;
  return spr->texturearray[usi];
}

int sprite_get_xoffset(int sprid)
{
  enigma::sprite *spr;
  if (!get_sprite(spr,sprid))
    return 0;

  return spr->xoffset;
}

int sprite_get_yoffset(int sprid)
{
  enigma::sprite *spr;
  if (!get_sprite(spr,sprid))
    return 0;

  return spr->yoffset;
}

void sprite_set_offset(int ind, int xoff, int yoff)
{
  enigma::sprite *spr;
  if (!get_sprite_mtx(spr,ind))
    return;

  spr->xoffset = xoff;
  spr->yoffset = yoff;
}

void sprite_set_bbox_mode(int ind, int mode) {
  enigma::sprite *spr;
  if (!get_sprite_mtx(spr,ind))
    return;

  spr->bbox_mode = mode;
}

int sprite_get_bbox_mode(int ind) {
  enigma::sprite *spr;
  if (!get_sprite_mtx(spr,ind))
    return 0;

  return spr->bbox_mode;
}


void sprite_set_bbox(int ind, int left, int top, int right, int bottom)
{
  enigma::sprite *spr;
  if (!get_sprite_mtx(spr,ind))
    return;

  spr->bbox.left = left;
  spr->bbox.top = top;
  spr->bbox.right = right;
  spr->bbox.bottom = bottom;
  spr->bbox_relative.left = left - spr->xoffset;
  spr->bbox_relative.top = top - spr->yoffset;
  spr->bbox_relative.right = right - spr->xoffset;
  spr->bbox_relative.bottom = bottom - spr->yoffset;
}

void sprite_collision_mask(int ind, bool sepmasks, int mode,
    int left, int top, int right, int bottom, int kind,
    unsigned char tolerance) {
  sprite_set_bbox(ind, left, top, right, bottom);
}

var sprite_get_uvs(int ind, int subimg){
  var uvs;
  uvs[4] = 0;

  enigma::sprite *spr;
  if (!get_sprite(spr,ind))
    return uvs;

  uvs[0] = spr->texturexarray[subimg];
  uvs[1] = spr->textureyarray[subimg];
  uvs[2] = spr->texturexarray[subimg] + spr->texturewarray[subimg];
  uvs[3] = spr->textureyarray[subimg] + spr->textureharray[subimg];
  return uvs;
}

}
