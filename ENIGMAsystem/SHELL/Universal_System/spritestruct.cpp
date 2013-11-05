/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2013 Robert B. Colton
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

#include <string>
#include <cstring>
using namespace std;

#include "Graphics_Systems/graphics_mandatory.h"
#include "Collision_Systems/collision_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "spritestruct.h"
#include "graphics_object.h"
#include "Universal_System/instance_system.h"
#include "libEGMstd.h"
#include "image_formats.h"
#include "estring.h"

#ifdef DEBUG_MODE
  #include "../Widget_Systems/widgets_mandatory.h"
  #define get_sprite(spr,id,r) \
    if (id < -1 or size_t(id) > enigma::sprite_idmax or !enigma::spritestructarray[id]) { \
      show_error("Cannot access sprite with id " + toString(id), false); \
      return r; \
    } const enigma::sprite *const spr = enigma::spritestructarray[id];
  #define get_spritev(spr,id) \
    if (id < -1 or size_t(id) > enigma::sprite_idmax or !enigma::spritestructarray[id]) { \
      show_error("Cannot access sprite with id " + toString(id), false); \
      return; \
    } const enigma::sprite *const spr = enigma::spritestructarray[id];
  #define get_sprite_null(spr,id,r) \
    if (id < -1 or size_t(id) > enigma::sprite_idmax) { \
      show_error("Cannot access sprite with id " + toString(id), false); \
      return r; \
    } const enigma::sprite *const spr = enigma::spritestructarray[id];
  #define get_sprite_mutable(spr,id,r) \
    if (id < -1 or size_t(id) > enigma::sprite_idmax or !enigma::spritestructarray[id]) { \
      show_error("Cannot access sprite with id " + toString(id), false); \
      return r; \
    } enigma::sprite *const spr = enigma::spritestructarray[id];
  #define get_spritev_mutable(spr,id) \
    if (id < -1 or size_t(id) > enigma::sprite_idmax or !enigma::spritestructarray[id]) { \
      show_error("Cannot access sprite with id " + toString(id), false); \
      return; \
    } enigma::sprite *const spr = enigma::spritestructarray[id];
#else
  /// Retrieve a sprite by the given index, doing bound/null checking if in debug mode and bailing with result r if an error occurs.
  #define get_sprite(spr,id,r) \
    const enigma::sprite *const spr = enigma::spritestructarray[id];
  /// Retrieve a sprite by the given index, doing bound/null checking if in debug mode and bailing with no result if an error occurs.
  #define get_spritev(spr,id) \
    const enigma::sprite *const spr = enigma::spritestructarray[id];
  /// Retrieve a sprite by the given index, doing bounds checking (but not null checking) if in debug mode and bailing with result r if an error occurs.
  #define get_sprite_null(spr,id,r) \
    const enigma::sprite *const spr = enigma::spritestructarray[id];
  /// Retrieve a sprite (and a lock thereto) by the given index, doing bound/null checking if in debug mode and bailing with result r if an error occurs.
  #define get_sprite_mutable(spr,id,r) \
    enigma::sprite *const spr = enigma::spritestructarray[id];
  /// Retrieve a sprite (and a lock thereto) by the given index, doing bound/null checking if in debug mode and bailing with no result if an error occurs.
  #define get_spritev_mutable(spr,id) \
    enigma::sprite *const spr = enigma::spritestructarray[id];
#endif

namespace enigma {
  sprite** spritestructarray;
  extern size_t sprite_idmax;
  sprite::sprite() {}
  sprite::sprite(int x) {}
}

namespace enigma_user
{

int sprite_add(string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload, int x_offset, int y_offset)
{
    enigma::spritestructarray_reallocate();
    enigma::sprite *spr = enigma::spritestructarray[enigma::sprite_idmax] = new enigma::sprite();
    enigma::sprite_add_to_index(spr, filename, imgnumb, precise, transparent, smooth, x_offset, y_offset);
    return enigma::sprite_idmax++;
}

int sprite_add(string filename, int imgnumb, bool transparent, bool smooth, int x_offset, int y_offset)
{
    return sprite_add(filename, imgnumb, false, transparent, smooth, true, x_offset, y_offset);
}

bool sprite_replace(int ind, string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload, int x_offset, int y_offset, bool free_texture)
{
    get_sprite_mutable(spr,ind,false);
    if (free_texture)
        for (int ii = 0; ii < spr->subcount; ii++)
            enigma::graphics_delete_texture(spr->texturearray[ii]);

    spr->texturearray.clear();
    spr->texbordxarray.clear();
    spr->texbordyarray.clear();
    enigma::sprite_add_to_index(spr, filename, imgnumb, precise, transparent, smooth, x_offset, y_offset);
    return true;
}

bool sprite_replace(int ind, string filename, int imgnumb, bool transparent, bool smooth, int x_offset, int y_offset, bool free_texture)
{
    return sprite_replace(ind, filename, imgnumb, false, transparent, smooth, true, x_offset, y_offset, free_texture);
}

void sprite_save(int ind, unsigned subimg, string fname) {
	get_spritev_mutable(spr,ind);
	unsigned char* rgbdata = enigma::graphics_get_texture_rgba(spr->texturearray[subimg]);
	
    string ext = enigma::image_get_format(fname);
	if (ext == ".bmp") {
		unsigned char* data = enigma::image_reverse_scanlines(rgbdata, spr->width, spr->height, 4);
		enigma::image_save(fname, data, spr->width, spr->height, spr->width, spr->height);
		delete[] data;
	} else {
		enigma::image_save(fname, rgbdata, spr->width, spr->height, spr->width, spr->height);
	}
	
	delete[] rgbdata;
}

void sprite_save_strip(int ind, string fname) {

}

void sprite_delete(int ind, bool free_texture)
{
    get_spritev_mutable(spr,ind);
    if (free_texture)
        for (int ii = 0; ii < spr->subcount; ii++)
            enigma::graphics_delete_texture(spr->texturearray[ii]);

    delete enigma::spritestructarray[ind];
    enigma::spritestructarray[ind] = NULL;
}

int sprite_duplicate(int copy_sprite)
{
    get_sprite_mutable(spr_copy,copy_sprite,-1);
    enigma::spritestructarray_reallocate();
    enigma::sprite *spr = enigma::spritestructarray[enigma::sprite_idmax] = new enigma::sprite();
    spr->id = enigma::sprite_idmax;
    enigma::sprite_add_copy(spr, spr_copy);
    return enigma::sprite_idmax++;
}

void sprite_assign(int ind, int copy_sprite, bool free_texture)
{
    get_spritev_mutable(spr,ind);
    get_spritev_mutable(spr_copy,copy_sprite);
    if (free_texture)
        for (int ii = 0; ii < spr->subcount; ii++)
            enigma::graphics_delete_texture(spr->texturearray[ii]);

    spr->texturearray.clear();
    spr->texbordxarray.clear();
    spr->texbordyarray.clear();
    enigma::sprite_add_copy(spr, spr_copy);
}

void sprite_set_alpha_from_sprite(int ind, int copy_sprite, bool free_texture)
{
    get_spritev_mutable(spr,ind);
    get_spritev_mutable(spr_copy,copy_sprite);
    for (int i = 0; i < spr->subcount; i++)
        enigma::graphics_replace_texture_alpha_from_texture(spr->texturearray[i], spr_copy->texturearray[i % spr_copy->subcount]);
}

void sprite_merge(int ind, int copy_sprite)
{
    get_spritev_mutable(spr,ind);
    get_spritev_mutable(spr_copy,copy_sprite);
    int i = 0, j = 0, t_subcount = spr->subcount + spr_copy->subcount;
    while (j < spr_copy->subcount)
    {
        spr->texturearray.push_back(enigma::graphics_duplicate_texture(spr_copy->texturearray[j]));
        spr->texbordxarray.push_back(spr_copy->texbordxarray[j]);
        spr->texbordyarray.push_back(spr_copy->texbordyarray[j]);
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
#include "nlpo2.h"

namespace enigma
{
  size_t spritestructarray_actualsize = 0; // INVARIANT: Should always be equal to the actual size of spritestructarray.
  //Allocates and zero-fills the array at game start
  void sprites_init()
  {
    spritestructarray_actualsize = sprite_idmax+1;
    spritestructarray = new sprite*[spritestructarray_actualsize];
    for (unsigned i = 0; i < sprite_idmax; i++)
      spritestructarray[i] = NULL;
  }

    void spritestructarray_reallocate()
    {
        if (spritestructarray_actualsize > sprite_idmax + 2) { // Ignore reallocation request if array is big enough.
            return;
        }
        sprite** spriteold = spritestructarray;
        spritestructarray_actualsize = (sprite_idmax + 2)*2; // Grow by doubling size, minimizing needed reallocations.
        spritestructarray = new sprite*[spritestructarray_actualsize];
        for (size_t i = 0; i < sprite_idmax; i++) spritestructarray[i] = spriteold[i];
        delete[] spriteold;
    }

    //Adds an empty sprite to the list
    int sprite_new_empty(unsigned sprid, unsigned subc, int w, int h, int x, int y, int bbt, int bbb, int bbl, int bbr, bool pl, bool sm)
    {
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

    void sprite_add_to_index(sprite *ns, string filename, int imgnumb, bool precise, bool transparent, bool smooth, int x_offset, int y_offset)
    {
        unsigned int width, height,fullwidth, fullheight;

        unsigned char *pxdata = image_load(filename, &width, &height, &fullwidth, &fullheight);
        
        // If sprite transparent, set the alpha to zero for pixels that should be transparent from lower left pixel color
        if (pxdata && transparent)
        {
          int t_pixel_r = pxdata[(height-1)*width*4]; 
          int t_pixel_g = pxdata[(height-1)*width*4+1]; 
          int t_pixel_b = pxdata[(height-1)*width*4+2];
          unsigned int ih, iw;
          for (ih = 0; ih <= height - 1; ih++)
          {
            int tmp = ih*width*4;
            for (iw=0; iw < width; iw++)
            {
              if (pxdata[tmp] == t_pixel_r && pxdata[tmp+1] == t_pixel_g && pxdata[tmp+2] == t_pixel_b)
                pxdata[tmp+3] = 0;
              
              tmp+=4;
            }
          }
        }
        
        int cellwidth =width/imgnumb;

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

        unsigned char* pixels=new unsigned char[cellwidth*height*4]();
        for (int ii=0;ii<imgnumb;ii++) 
        {
                int ih,iw;
                int xcelloffset=ii*cellwidth*4;
                for(ih = height - 1; ih >= 0; ih--)
                {
                        int tmp = ih*fullwidth*4+xcelloffset;
                        int tmpcell = ih*cellwidth*4;
                        for (iw=0; iw < cellwidth; iw++)
                        {
                                pixels[tmpcell+3] = pxdata[tmp+3];
                                pixels[tmpcell+2] = pxdata[tmp+2];
                                pixels[tmpcell+1] = pxdata[tmp+1];
                                pixels[tmpcell] = pxdata[tmp];
                                tmp+=4;
                                tmpcell+=4;
                        }
                }
                unsigned texture = graphics_create_texture(cellwidth, fullheight, pixels, false);
                ns->texturearray.push_back(texture);
                ns->texbordxarray.push_back((double) 1.0);//width/fullwidth;
                ns->texbordyarray.push_back((double) height/fullheight);
				
                collision_type coll_type = precise ? ct_precise : ct_bbox;
                ns->colldata.push_back(get_collision_mask(ns,(unsigned char*)pixels,coll_type));
        }
        delete[] pixels;
        delete[] pxdata;
    }

    void sprite_add_copy(sprite *spr, sprite *spr_copy)
    {
        spr->subcount  = spr_copy->subcount;
        spr->width     = spr_copy->width;
        spr->height    = spr_copy->height;
        spr->xoffset   = spr_copy->xoffset;
        spr->yoffset   = spr_copy->yoffset;

        for (int i = 0; i < spr->subcount; i++)
        {
            spr->texturearray.push_back(graphics_duplicate_texture(spr_copy->texturearray[i]));
            spr->texbordxarray.push_back(spr_copy->texbordxarray[i]);
            spr->texbordyarray.push_back(spr_copy->texbordyarray[i]);
        }
    }

  //Sets the subimage
  void sprite_set_subimage(int sprid, int imgindex, unsigned int w, unsigned int h, unsigned char* chunk, unsigned char* collision_data, collision_type ct)
  {
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

    unsigned texture = graphics_create_texture(fullwidth,fullheight,imgpxdata,false);

    sprite* sprstr = spritestructarray[sprid];

    sprstr->texturearray.push_back(texture);
    sprstr->texbordxarray.push_back((double) w/fullwidth);
    sprstr->texbordyarray.push_back((double) h/fullheight);
    sprstr->colldata.push_back(get_collision_mask(sprstr,collision_data,ct));

    delete[] imgpxdata;
  }
  
  //Appends a subimage
  void sprite_add_subimage(int sprid, unsigned int w, unsigned int h, unsigned char* chunk, unsigned char* collision_data, collision_type ct)
  {
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

    unsigned texture = graphics_create_texture(fullwidth,fullheight,imgpxdata,false);

    sprite* sprstr = spritestructarray[sprid];

    sprstr->texturearray.push_back(texture);
    sprstr->texbordxarray.push_back((double) w/fullwidth);
    sprstr->texbordyarray.push_back((double) h/fullheight);
    sprstr->colldata.push_back(get_collision_mask(sprstr,collision_data,ct));
	
	sprstr->subcount += 1;

    delete[] imgpxdata;
  }
}

namespace enigma_user
{

int sprite_get_width(int sprite)
{
  get_sprite(spr,sprite,32);
  return spr->width;
}

int sprite_get_height(int sprite)
{
  get_sprite(spr,sprite,32);
  return spr->height;
}

double sprite_get_texture_width_factor(int sprite, int subimg)
{
  get_sprite(spr,sprite,32);
  return spr->texbordxarray[subimg];
}

double sprite_get_texture_height_factor(int sprite, int subimg)
{
  get_sprite(spr,sprite,32);
  return spr->texbordyarray[subimg];
}

int sprite_get_bbox_bottom(int sprite)
{
  get_sprite(spr,sprite,32);
  return spr->bbox.bottom;
}
int sprite_get_bbox_left(int sprite)
{
  get_sprite(spr,sprite,0);
  return spr->bbox.left;
}
int sprite_get_bbox_right(int sprite)
{
  get_sprite(spr,sprite,32);
  return spr->bbox.right;
}
int sprite_get_bbox_top(int sprite)
{
  get_sprite(spr,sprite,0);
  return spr->bbox.top;
}

}

int sprite_get_bbox_bottom_relative(int sprite)
{
  get_sprite(spr,sprite,32);
  return spr->bbox_relative.bottom;
}
int sprite_get_bbox_left_relative(int sprite)
{
  get_sprite(spr,sprite,0);
  return spr->bbox_relative.left;
}
int sprite_get_bbox_right_relative(int sprite)
{
  get_sprite(spr,sprite,32);
  return spr->bbox_relative.right;
}
int sprite_get_bbox_top_relative(int sprite)
{
  get_sprite(spr,sprite,0);
  return spr->bbox_relative.top;
}

#ifdef DEBUG_MODE
  bbox_rect_t dummy_bbox = {32,0,32,0};
#endif
const bbox_rect_t &sprite_get_bbox(int sprite)
{
  get_sprite(spr,sprite,dummy_bbox);
  return spr->bbox;
}
const bbox_rect_t &sprite_get_bbox_relative(int sprite)
{
  get_sprite(spr,sprite,dummy_bbox);
  return spr->bbox_relative;
}

namespace enigma_user
{

int sprite_get_number(int sprite)
{
  if (sprite == -1) return 0;
  get_sprite(spr,sprite,0);
  return spr->subcount;
}

int sprite_get_texture(int sprite,int subimage)
{
  get_sprite(spr,sprite,0);
    const int usi = subimage >= 0 ? (subimage % spr->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr->subcount;
  return spr->texturearray[usi];
}

int sprite_get_xoffset(int sprite)
{
  get_sprite(spr,sprite,0);
  return spr->xoffset;
}

int sprite_get_yoffset(int sprite)
{
  get_sprite(spr,sprite,0);
  return spr->yoffset;
}

void sprite_set_offset(int ind, int xoff, int yoff)
{
    get_spritev_mutable(spr,ind);
    spr->xoffset = xoff;
    spr->yoffset = yoff;
}

void sprite_set_bbox_mode(int ind, int mode) {
  get_spritev_mutable(spr,ind);
}

void sprite_set_bbox(int ind, int left, int top, int right, int bottom)
{
    get_spritev_mutable(spr,ind);
    spr->bbox.left = left;
    spr->bbox.top = top;
    spr->bbox.right = right;
    spr->bbox.bottom = bottom;
    spr->bbox_relative.left = left - spr->xoffset;
    spr->bbox_relative.top = top - spr->yoffset;
    spr->bbox_relative.right = right - spr->xoffset;
    spr->bbox_relative.bottom = bottom - spr->yoffset;
}

void sprite_collision_mask(int ind, bool sepmasks, int mode, int left, int right, int top, int bottom, int kind, unsigned char tolerance) {
	sprite_set_bbox(ind, left, top, right, bottom);
}

void sprite_set_precise(int ind, bool precise) {
  get_spritev_mutable(spr,ind);
}

}

