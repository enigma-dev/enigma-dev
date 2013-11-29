/** Copyright (C) 2013 Robert B. Colton
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

#include <cmath>
#include <cstdlib>
#include <string>
using std::string;

#include "Direct3D9Headers.h"
#include "../General/GScolors.h"
#include "../General/GSsprite.h"
#include "../General/GStextures.h"
#include "../General/GSprimitives.h"

#include "Universal_System/nlpo2.h"
#include "Universal_System/spritestruct.h"
#include "Universal_System/instance_system.h"
#include "Universal_System/graphics_object.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)


#ifdef DEBUG_MODE
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
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

#include "Direct3D9Headers.h"
#include "Bridges/General/DX9Context.h"
#include "DX9TextureStruct.h"

namespace enigma {
  D3DCOLOR get_currentcolor();
}

namespace enigma_user
{

void draw_sprite(int spr,int subimg, gs_scalar x, gs_scalar y)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
	
	const float tbx = spr2d->texbordxarray[usi], tby = spr2d->texbordyarray[usi],
			xvert1 = x-spr2d->xoffset, xvert2 = xvert1 + spr2d->width,
			yvert1 = y-spr2d->yoffset, yvert2 = yvert1 + spr2d->height;

	draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
	draw_vertex_texture(xvert1,yvert1,0,0);
	draw_vertex_texture(xvert2,yvert1,tbx,0);
	draw_vertex_texture(xvert1,yvert2, 0,tby);
	draw_vertex_texture(xvert2,yvert2, tbx,tby);
	draw_primitive_end();
}

void draw_sprite_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int color, gs_scalar alpha)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

	rot *= M_PI/180;

    const float
    w = spr2d->width*xscale, h = spr2d->height*yscale,
    tbx = spr2d->texbordxarray[usi], tby = spr2d->texbordyarray[usi],
    wsinrot = w*sin(rot), wcosrot = w*cos(rot);
	
	draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
	float
		ulcx = x - xscale * spr2d->xoffset * cos(rot) + yscale * spr2d->yoffset * cos(M_PI/2+rot),
		ulcy = y + xscale * spr2d->xoffset * sin(rot) - yscale * spr2d->yoffset * sin(M_PI/2+rot);
	draw_vertex_texture_color(ulcx,ulcy, 0,0, color, alpha);
	draw_vertex_texture_color(ulcx + wcosrot, ulcy - wsinrot, tbx, 0, color, alpha);
	const double mpr = 3*M_PI/2 + rot;
    ulcx += h * cos(mpr);
    ulcy -= h * sin(mpr);
	draw_vertex_texture_color(ulcx,ulcy, 0,tby, color, alpha);
	draw_vertex_texture_color(ulcx + wcosrot, ulcy - wsinrot, tbx,tby, color, alpha);
	draw_primitive_end();
}

void draw_sprite_pos(int spr, int subimg, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar alpha)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

    const float tbx = spr2d->texbordxarray[usi], tby = spr2d->texbordyarray[usi];

	draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
	draw_vertex_texture_color(x1,y1,0,0,draw_get_color(),alpha);
	draw_vertex_texture_color(x2,y1,tbx,0,draw_get_color(),alpha);
	draw_vertex_texture_color(x1,y2,0,tby,draw_get_color(),alpha);
	draw_vertex_texture_color(x2,y2,tbx,tby,draw_get_color(),alpha);
	draw_primitive_end();
}

void draw_sprite_part(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
	
	float tbw = spr2d->width/(float)spr2d->texbordxarray[usi], tbh = spr2d->height/(float)spr2d->texbordyarray[usi],
	  tbx1 = left/tbw, tbx2 = tbx1 + width/tbw,
	  tby1 = top/tbh, tby2 = tby1 + height/tbh;
	
	draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
	draw_vertex_texture(x,y,tbx1,tby1);
	draw_vertex_texture(x+width,y,tbx2,tby1);
	draw_vertex_texture(x,y+height,tbx1,tby2);
	draw_vertex_texture(x+width,y+height,tbx2,tby2);
	draw_primitive_end();
}

void draw_sprite_part_offset(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
	
	float tbw = spr2d->width/spr2d->texbordxarray[usi], tbh = spr2d->height/spr2d->texbordyarray[usi],
	  xvert1 = x-spr2d->xoffset, xvert2 = xvert1 + spr2d->width,
	  yvert1 = y-spr2d->yoffset, yvert2 = yvert1 + spr2d->height,
	  tbx1 = left/tbw, tbx2 = tbx1 + width/tbw,
	  tby1 = top/tbh, tby2 = tby1 + height/tbh;
	
	draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
	draw_vertex_texture(xvert1,yvert1,tbx1,tby1);
	draw_vertex_texture(xvert2,yvert1,tbx2,tby1);
	draw_vertex_texture(xvert1,yvert2,tbx1,tby2);
	draw_vertex_texture(xvert2,yvert2,tbx2,tby2);
	draw_primitive_end();
}

void draw_sprite_part_ext(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

	float tbw = spr2d->width/(float)spr2d->texbordxarray[usi], tbh = spr2d->height/(float)spr2d->texbordyarray[usi],
	  xvert1 = x, xvert2 = xvert1 + width*xscale,
	  yvert1 = y, yvert2 = yvert1 + height*yscale,
	  tbx1 = left/tbw, tbx2 = tbx1 + width/tbw,
	  tby1 = top/tbh, tby2 = tby1 + height/tbh;

	draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
	draw_vertex_texture_color(xvert1,yvert1,tbx1,tby1,color,alpha);
	draw_vertex_texture_color(xvert2,yvert1,tbx2,tby1,color,alpha);
	draw_vertex_texture_color(xvert1,yvert2,tbx1,tby2,color,alpha);
	draw_vertex_texture_color(xvert2,yvert2,tbx2,tby2,color,alpha);
	draw_primitive_end();
}

void draw_sprite_general(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar a1, gs_scalar a2, gs_scalar a3, gs_scalar a4)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

	const float
	  tbx = spr2d->texbordxarray[usi],  tby = spr2d->texbordyarray[usi],
	  tbw = spr2d->width/tbx, tbh = spr2d->height/tby,
	  w = width*xscale, h = height*yscale;

    rot *= M_PI/180;
    const float wcosrot = w*cos(rot), wsinrot = w*sin(rot);

    draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);

    float
    ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
    ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);

	draw_vertex_texture_color(ulcx, ulcy, left/tbw, top/tbh, c1, a1);
	draw_vertex_texture_color((ulcx + wcosrot), (ulcy - wsinrot), (left+width)/tbw, top/tbh, c2, a2);
	
    ulcx += h * cos(3*M_PI/2 + rot);
    ulcy -= h * sin(3*M_PI/2 + rot);

	draw_vertex_texture_color((ulcx + wcosrot), (ulcy - wsinrot), (left+width)/tbw, (top+height)/tbh, c3, a3);
	draw_vertex_texture_color(ulcx, ulcy, left/tbw, (top+height)/tbh, c4, a4);

    draw_primitive_end();
}

void draw_sprite_stretched(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

    const float tbx = spr2d->texbordxarray[usi], tby = spr2d->texbordyarray[usi],
                xvert1 = x-spr2d->xoffset, xvert2 = xvert1 + width,
                yvert1 = y-spr2d->yoffset, yvert2 = yvert1 + height;
	
	draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
	draw_vertex_texture(xvert1,yvert1,0,0);
	draw_vertex_texture(xvert2,yvert1,tbx,0);
	draw_vertex_texture(xvert1,yvert2, 0,tby);
	draw_vertex_texture(xvert2,yvert2, tbx,tby);
	draw_primitive_end();
}

void draw_sprite_stretched_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, int color, gs_scalar alpha)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

    const float tbx = spr2d->texbordxarray[usi], tby = spr2d->texbordyarray[usi],
                xvert1 = x-spr2d->xoffset, xvert2 = xvert1 + width,
                yvert1 = y-spr2d->yoffset, yvert2 = yvert1 + height;

	draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
	draw_vertex_texture_color(xvert1,yvert1,0,0,color,alpha);
	draw_vertex_texture_color(xvert2,yvert1,tbx,0,color,alpha);
	draw_vertex_texture_color(xvert1,yvert2, 0,tby,color,alpha);
	draw_vertex_texture_color(xvert2,yvert2, tbx,tby,color,alpha);
	draw_primitive_end();
}

}

// These two leave a bad taste in my mouth because they depend on views, which should be removable.
// However, for now, they stay.

#include <string>
using std::string;
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"

namespace enigma_user
{

void draw_sprite_tiled(int spr, int subimg, gs_scalar x, gs_scalar y)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

    const float
    tbx  = spr2d->texbordxarray[usi], tby  = spr2d->texbordyarray[usi],
    xoff = fmod(spr2d->xoffset+x,spr2d->width)-spr2d->width, yoff = fmod(spr2d->yoffset+y,spr2d->height)-spr2d->height;

    const int
    hortil = int(ceil((view_enabled ? int(view_xview[view_current] + view_wview[view_current]) : room_width) / (spr2d->width*tbx))) + 1,
    vertil = int(ceil((view_enabled ? int(view_yview[view_current] + view_hview[view_current]) : room_height) / (spr2d->height*tby))) + 1;
	
    float xvert1 = xoff, xvert2 = xvert1 + spr2d->width, yvert1, yvert2;
    for (int i=0; i<hortil; i++)
    {
        yvert1 = yoff; yvert2 = yvert1 + spr2d->height;
        for (int c=0; c<vertil; c++)
        {
			draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
			draw_vertex_texture(xvert1,yvert1,0,0);
			draw_vertex_texture(xvert2,yvert1,tbx,0);
			draw_vertex_texture(xvert1,yvert2, 0,tby);
			draw_vertex_texture(xvert2,yvert2, tbx,tby);
			draw_primitive_end();
            yvert1 = yvert2;
            yvert2 += spr2d->height;
        }
        xvert1 = xvert2;
        xvert2 += spr2d->width;
    }
}

void draw_sprite_tiled_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

    const float
    tbx  = spr2d->texbordxarray[usi], tby  = spr2d->texbordyarray[usi],
    width_scaled = spr2d->width*xscale, height_scaled = spr2d->height*yscale,
    xoff = fmod(spr2d->xoffset*xscale+x,width_scaled)-width_scaled, yoff = fmod(spr2d->yoffset*yscale+y,height_scaled)-height_scaled;

    const int
    hortil = int(ceil((view_enabled ? int(view_xview[view_current] + view_wview[view_current]) : room_width) / (width_scaled*tbx))) + 1,
    vertil = int(ceil((view_enabled ? int(view_yview[view_current] + view_hview[view_current]) : room_height) / (height_scaled*tby))) + 1;

    float xvert1 = xoff, xvert2 = xvert1 + width_scaled, yvert1, yvert2;
    for (int i=0; i<hortil; i++)
    {
        yvert1 = yoff; yvert2 = yvert1 + height_scaled;
        for (int c=0; c<vertil; c++)
        {
			draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
			draw_vertex_texture(xvert1,yvert1,0,0);
			draw_vertex_texture(xvert2,yvert1,tbx,0);
			draw_vertex_texture(xvert1,yvert2, 0,tby);
			draw_vertex_texture(xvert2,yvert2, tbx,tby);
			draw_primitive_end();
            yvert1 = yvert2;
            yvert2 += height_scaled;
       }
       xvert1 = xvert2;
       xvert2 += width_scaled;
    }
}

int sprite_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload, int xorig, int yorig) {

}

int sprite_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig) {
	return sprite_create_from_screen(x, y, w, h, removeback, smooth, true, xorig, yorig);
}

void sprite_add_from_screen(int id, int x, int y, int w, int h, bool removeback, bool smooth) {

}

}

