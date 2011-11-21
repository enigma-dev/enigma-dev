/** Copyright (C) 2008-2011 Josh Ventura
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

#include "../Graphics_Systems/graphics_mandatory.h"
#include "../Collision_Systems/collision_mandatory.h"
#include "../Widget_Systems/widgets_mandatory.h"
#include "spritestruct.h"
#include "../libEGMstd.h"
#include "IMGloading.h"

namespace enigma {
  sprite** spritestructarray;
	extern size_t sprite_idmax;
	sprite::sprite(): texturearray(NULL) {}
  sprite::sprite(unsigned int x): texturearray(new unsigned int[x]) {}
}

int sprite_add(string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload, int x_offset, int y_offset)
{
	int width,height,fullwidth,fullheight;
	
	char *pxdata = enigma::load_bitmap(filename,&width,&height,&fullwidth,&fullheight);
	unsigned texture = enigma::graphics_create_texture(fullwidth, fullheight, pxdata);
	delete[] pxdata;
	
	//ns.pixeldata=(void**) malloc(sizeof(void*));
	//ns.pixeldata[0]=bitmapbuffer;
	enigma::sprite *ns = enigma::spritestructarray[enigma::sprite_idmax] = new enigma::sprite;
	ns->id = enigma::sprite_idmax;
	ns->subcount  = 1;
	ns->width     = width;
	ns->height    = height;
	ns->xoffset   = (int)x_offset;
	ns->yoffset   = (int)y_offset;
	ns->texbordx  = (double) width/fullwidth;
	ns->texbordy  = (double) height/fullheight;
	ns->texturearray[0] = texture;
	return enigma::sprite_idmax++;
}



/* These functions are primarily for use of the engine. Experienced users
 * who are familiar with C++ can make use of these, but they were made to
 * use at load time with data read from the executable. These both expect
 * RAW format, RGB only.
 */
#include "nlpo2.h"

namespace enigma
{
  //Allocates and zero-fills the array at game start
  void sprites_init()
  {
    spritestructarray = new sprite*[enigma::sprite_idmax+1];
    for (unsigned i = 0; i < enigma::sprite_idmax; i++)
      spritestructarray[i] = NULL;
  }
  
  //Adds an empty sprite to the list
  int sprite_new_empty(unsigned sprid, unsigned subc, int w, int h, int x, int y, int bbt, int bbb, int bbl, int bbr, bool pl, bool sm)
  {
    int fullwidth=nlpo2dc(w)+1,fullheight=nlpo2dc(h)+1;
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
    as->texbordx = (double)w/fullwidth;
    as->texbordy = (double)h/fullheight;
    
    as->texturearray = new unsigned int[subc];
    as->colldata = new void*[subc];
    
    if (enigma::sprite_idmax < sprid+1)
      enigma::sprite_idmax = sprid+1;
    
    return sprid;
  }

  #if COLLIGMA
  collCustom* generate_bitmask(unsigned char* pixdata,int x,int y,int w,int h)
  {
    collCustom* thenewmask = new collCustom(w,h,x,y,x,y);
    for(int xp=0;xp<w*h;xp++)
      collbitSet(*thenewmask,xp % w,xp/w,pixdata[xp*4+3]>0); //the width times the number of rows, the current column*4, then 3
    return thenewmask;
  }
  #endif
  
  
  //Adds a subimage to an existing sprite from the exe
  void sprite_set_subimage(int sprid, int imgindex, int x,int y, unsigned int w,unsigned int h,unsigned char*chunk)
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
    
    unsigned texture = graphics_create_texture(fullwidth,fullheight,imgpxdata);
    delete[] imgpxdata;
    
    enigma::sprite* sprstr = enigma::spritestructarray[sprid];
    
    sprstr->texturearray[imgindex] = texture;
    sprstr->colldata[imgindex] = collisionsystem_sprite_data_create(imgpxdata,x,y,w,h);
  }
}

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
#else
  #define get_sprite(spr,id,r) \
    const enigma::sprite *const spr = enigma::spritestructarray[id];
  #define get_spritev(spr,id) \
    const enigma::sprite *const spr = enigma::spritestructarray[id];
  #define get_sprite_null(spr,id,r) \
    const enigma::sprite *const spr = enigma::spritestructarray[id];
#endif

int sprite_get_width(int sprite)
{
	get_sprite(spr,sprite,0);
	return spr->width;
}

int sprite_get_height(int sprite)
{
	get_sprite(spr,sprite,0);
	return spr->height;
}


int sprite_get_bbox_bottom(int sprite)
{
	get_sprite(spr,sprite,0);
	return spr->bbox.bottom;
}
int sprite_get_bbox_left(int sprite)
{
	get_sprite(spr,sprite,0);
	return spr->bbox.left;
}
int sprite_get_bbox_right(int sprite)
{
	get_sprite(spr,sprite,0);
	return spr->bbox.right;
}
int sprite_get_bbox_top(int sprite)
{
	get_sprite(spr,sprite,0);
	return spr->bbox.top;
}

int sprite_get_bbox_bottom_relative(int sprite)
{
	get_sprite(spr,sprite,0);
	return spr->bbox_relative.bottom;
}
int sprite_get_bbox_left_relative(int sprite)
{
	get_sprite(spr,sprite,0);
	return spr->bbox_relative.left;
}
int sprite_get_bbox_right_relative(int sprite)
{
	get_sprite(spr,sprite,0);
	return spr->bbox_relative.right;
}
int sprite_get_bbox_top_relative(int sprite)
{
	get_sprite(spr,sprite,0);
	return spr->bbox_relative.top;
}

#ifdef DEBUG_MODE
  bbox_rect_t dummy_bbox;
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


//TODO: IMPLEMENT
//sprite_get_bbox_mode
//sprite_get_name
//sprite_get_precise
//sprite_get_preload
//sprite_get_smooth
//sprite_get_transparent

int sprite_get_number(int sprite)
{
  if (sprite == -1) return 0;
	get_sprite(spr,sprite,0);
	return spr->subcount;
}

int sprite_get_texture(int sprite,int subimage)
{
	get_sprite(spr,sprite,0);
	return spr->texturearray[subimage % spr->subcount];
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
