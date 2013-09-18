/** Copyright (C) 2008-2013 Josh Ventura, Harijs Grinbergs, Robert B. Colton
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

#include "../General/OpenGLHeaders.h"
#include "../General/GSsprite.h"
#include "../General/GStextures.h"
#include "../General/GLTextureStruct.h"

#include "GL3binding.h"
#include "GL3shapes.h"

#include "Universal_System/spritestruct.h"
#include "Universal_System/instance_system.h"
#include "Universal_System/graphics_object.h"

#define __GETR(x) (gs_scalar)(((x & 0x0000FF))/255.0)
#define __GETG(x) (gs_scalar)(((x & 0x00FF00) >> 8)/255.0)
#define __GETB(x) (gs_scalar)(((x & 0xFF0000) >> 16)/255.0)


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

namespace enigma
{
    extern unsigned int globalVBO_texture;
}

namespace enigma_user
{

bool sprite_exists(int spr) {
    return (unsigned(spr) < enigma::sprite_idmax) and bool(enigma::spritestructarray[spr]);
}

void draw_sprite(int spr, int subimg, gs_scalar x, gs_scalar y)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    texture_use(GmTextures[spr2d->texturearray[usi]]->gltex);

    const gs_scalar tbx = spr2d->texbordxarray[usi], tby = spr2d->texbordyarray[usi],
                xvert1 = x-spr2d->xoffset, xvert2 = xvert1 + spr2d->width,
                yvert1 = y-spr2d->yoffset, yvert2 = yvert1 + spr2d->height;

    const gs_scalar data[4*8] = {
         xvert1, yvert1, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0,
         xvert2, yvert1, tbx, 0.0, 1.0, 1.0, 1.0, 1.0,
         xvert2, yvert2, tbx, tby, 1.0, 1.0, 1.0, 1.0,
         xvert1, yvert2, 0.0, tby, 1.0, 1.0, 1.0, 1.0
    };
    plane2D_rotated(data);
}

void draw_sprite_pos(int spr, int subimg, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar alpha)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    texture_use(GmTextures[spr2d->texturearray[usi]]->gltex);

    const gs_scalar tbx = spr2d->texbordxarray[usi], tby = spr2d->texbordyarray[usi];

    const gs_scalar data[4*8] = {
         x1, y1, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0,
         x2, y1, tbx, 0.0, 1.0, 1.0, 1.0, 1.0,
         x2, y2, tbx, tby, 1.0, 1.0, 1.0, 1.0,
         x1, y2, 0.0, tby, 1.0, 1.0, 1.0, 1.0
    };
    plane2D_rotated(data);
}

void draw_sprite_stretched(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    texture_use(GmTextures[spr2d->texturearray[usi]]->gltex);

    const gs_scalar tbx = spr2d->texbordxarray[usi], tby = spr2d->texbordyarray[usi],
                xvert1 = x-spr2d->xoffset, xvert2 = xvert1 + width,
                yvert1 = y-spr2d->yoffset, yvert2 = yvert1 + height;

    const gs_scalar data[4*8] = {
         xvert1, yvert1, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0,
         xvert2, yvert1, tbx, 0.0, 1.0, 1.0, 1.0, 1.0,
         xvert2, yvert2, tbx, tby, 1.0, 1.0, 1.0, 1.0,
         xvert1, yvert2, 0.0, tby, 1.0, 1.0, 1.0, 1.0
    };
    plane2D_rotated(data);
}

void draw_sprite_part(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    texture_use(GmTextures[spr2d->texturearray[usi]]->gltex);

    const gs_scalar tbw = spr2d->width/(float)spr2d->texbordxarray[usi], tbh = spr2d->height/(float)spr2d->texbordyarray[usi],
          tbx1 = left/tbw, tbx2 = tbx1 + width/tbw,
          tby1 = top/tbh, tby2 = tby1 + height/tbh;

    const gs_scalar data[4*8] = {
         x, y, tbx1, tby1, 1.0, 1.0, 1.0, 1.0,
         x+width, y, tbx2, tby1, 1.0, 1.0, 1.0, 1.0,
         x+width, y+height, tbx2, tby2, 1.0, 1.0, 1.0, 1.0,
         x, y+height, tbx1, tby2, 1.0, 1.0, 1.0, 1.0
    };
    plane2D_rotated(data);
}

void draw_sprite_part_offset(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    texture_use(GmTextures[spr2d->texturearray[usi]]->gltex);

    const gs_scalar tbw = spr2d->width/spr2d->texbordxarray[usi], tbh = spr2d->height/spr2d->texbordyarray[usi],
          xvert1 = x-spr2d->xoffset, xvert2 = xvert1 + spr2d->width,
          yvert1 = y-spr2d->yoffset, yvert2 = yvert1 + spr2d->height,
          tbx1 = left/tbw, tbx2 = tbx1 + width/tbw,
          tby1 = top/tbh, tby2 = tby1 + height/tbh;

    const gs_scalar data[4*8] = {
         xvert1, yvert1, tbx1, tby1, 1.0, 1.0, 1.0, 1.0,
         xvert2, yvert1, tbx2, tby1, 1.0, 1.0, 1.0, 1.0,
         xvert2, yvert2, tbx2, tby2, 1.0, 1.0, 1.0, 1.0,
         xvert1, yvert2, tbx1, tby2, 1.0, 1.0, 1.0, 1.0
    };
    plane2D_rotated(data);
}

void draw_sprite_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int blend, gs_scalar alpha)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    texture_use(GmTextures[spr2d->texturearray[usi]]->gltex);

    rot *= M_PI/180;

    const gs_scalar w = spr2d->width*xscale, h = spr2d->height*yscale,
    tbx = spr2d->texbordxarray[usi], tby = spr2d->texbordyarray[usi],
    wsinrot = w*sin(rot), wcosrot = w*cos(rot);

    const gs_scalar ulcx = x - xscale * spr2d->xoffset * cos(rot) + yscale * spr2d->yoffset * cos(M_PI/2+rot),
    ulcy = y + xscale * spr2d->xoffset * sin(rot) - yscale * spr2d->yoffset * sin(M_PI/2+rot);
    const double mpr = 3*M_PI/2 + rot;
    const gs_scalar ulcx2 = ulcx + h * cos(mpr),
    ulcy2 = ulcy - h * sin(mpr);

    const gs_scalar r = __GETR(blend), g = __GETG(blend), b = __GETB(blend);
    const gs_scalar data[4*8] = {
         ulcx, ulcy, 0.0, 0.0, r, g, b, alpha,
         ulcx + wcosrot, ulcy - wsinrot, tbx, 0.0, r, g, b, alpha,
         ulcx2 + wcosrot, ulcy2 - wsinrot, tbx, tby, r, g, b, alpha,
         ulcx2, ulcy2, 0.0, tby, r, g, b, alpha
    };
    plane2D_rotated(data);
}

void draw_sprite_part_ext(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int blend, gs_scalar alpha)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    texture_use(GmTextures[spr2d->texturearray[usi]]->gltex);

    const gs_scalar tbw = spr2d->width/(float)spr2d->texbordxarray[usi], tbh = spr2d->height/(float)spr2d->texbordyarray[usi],
          xvert1 = x, xvert2 = xvert1 + width*xscale,
          yvert1 = y, yvert2 = yvert1 + height*yscale,
          tbx1 = left/tbw, tbx2 = tbx1 + width/tbw,
          tby1 = top/tbh, tby2 = tby1 + height/tbh;

    const gs_scalar r = __GETR(blend), g = __GETG(blend), b = __GETB(blend);
    const gs_scalar data[4*8] = {
         xvert1,yvert1, tbx1,tby1, r, g, b, alpha,
         xvert2,yvert1, tbx2,tby1, r, g, b, alpha,
         xvert2,yvert2, tbx2,tby2, r, g, b, alpha,
         xvert1,yvert2, tbx1,tby2, r, g, b, alpha
    };
    plane2D_rotated(data);
}

void draw_sprite_general(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar a1, gs_scalar a2, gs_scalar a3, gs_scalar a4)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    texture_use(GmTextures[spr2d->texturearray[usi]]->gltex);

    const gs_scalar
    tbw = spr2d->width/spr2d->texbordxarray[usi], tbh = spr2d->height/spr2d->texbordyarray[usi],
    w = width*xscale, h = height*yscale;

    rot *= M_PI/180;
    const gs_scalar wcosrot = w*cos(rot), wsinrot = w*sin(rot);

    const gs_scalar ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
    ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);

    const gs_scalar ulcx2 = ulcx + h * cos(3*M_PI/2 + rot),
    ulcy2 = ulcy - h * sin(3*M_PI/2 + rot);

    const gs_scalar data[4*8] = {
         ulcx,ulcy, left/tbw,top/tbh, __GETR(c1), __GETG(c1), __GETB(c1), a1,
         ulcx + wcosrot, ulcy - wsinrot, (left+width)/tbw,top/tbh, __GETR(c2), __GETG(c2), __GETB(c2), a2,
         ulcx2 + wcosrot, ulcy2 - wsinrot, (left+width)/tbw,(top+height)/tbh, __GETR(c3), __GETG(c3), __GETB(c3), a3,
         ulcx2,ulcy2, left/tbw,(top+height)/tbh, __GETR(c4), __GETG(c4), __GETB(c4), a4
    };
    plane2D_rotated(data);
}

void draw_sprite_stretched_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, int blend, gs_scalar alpha)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    texture_use(GmTextures[spr2d->texturearray[usi]]->gltex);

    const gs_scalar tbx = spr2d->texbordxarray[usi], tby = spr2d->texbordyarray[usi],
                xvert1 = x-spr2d->xoffset, xvert2 = xvert1 + width,
                yvert1 = y-spr2d->yoffset, yvert2 = yvert1 + height;

    const gs_scalar r = __GETR(blend), g = __GETG(blend), b = __GETB(blend);
    const gs_scalar data[4*8] = {
         xvert1, yvert1, 0.0, 0.0, r, g, b, alpha,
         xvert2, yvert1, tbx, 0.0, r, g, b, alpha,
         xvert2, yvert2, tbx, tby, r, g, b, alpha,
         xvert1, yvert2, 0.0, tby, r, g, b, alpha
    };
    plane2D_rotated(data);
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
    texture_use(GmTextures[spr2d->texturearray[usi]]->gltex);

    const gs_scalar tbx  = spr2d->texbordxarray[usi], tby  = spr2d->texbordyarray[usi],
    xoff = fmod(spr2d->xoffset+x,spr2d->width)-spr2d->width, yoff = fmod(spr2d->yoffset+y,spr2d->height)-spr2d->height;

    const int hortil = int(ceil((view_enabled ? int(view_xview[view_current] + view_wview[view_current]) : room_width) / (spr2d->width*tbx))) + 1,
    vertil = int(ceil((view_enabled ? int(view_yview[view_current] + view_hview[view_current]) : room_height) / (spr2d->height*tby))) + 1;

    gs_scalar xvert1 = xoff, xvert2 = xvert1 + spr2d->width, yvert1, yvert2;
    for (int i=0; i<hortil; i++)
    {
        yvert1 = yoff; yvert2 = yvert1 + spr2d->height;
        for (int c=0; c<vertil; c++)
        {
            const gs_scalar data[4*8] = {
                 xvert1, yvert1, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0,
                 xvert2, yvert1, tbx, 0.0, 1.0, 1.0, 1.0, 1.0,
                 xvert2, yvert2, tbx, tby, 1.0, 1.0, 1.0, 1.0,
                 xvert1, yvert2, 0.0, tby, 1.0, 1.0, 1.0, 1.0
            };
            plane2D_rotated(data);

            yvert1 = yvert2;
            yvert2 += spr2d->height;
        }
        xvert1 = xvert2;
        xvert2 += spr2d->width;
    }
}

void draw_sprite_tiled_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int blend, gs_scalar alpha)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;
    texture_use(GmTextures[spr2d->texturearray[usi]]->gltex);

    const gs_scalar
    tbx  = spr2d->texbordxarray[usi], tby  = spr2d->texbordyarray[usi],
    width_scaled = spr2d->width*xscale, height_scaled = spr2d->height*yscale,
    xoff = fmod(spr2d->xoffset*xscale+x,width_scaled)-width_scaled, yoff = fmod(spr2d->yoffset*yscale+y,height_scaled)-height_scaled;


    const int
    hortil = int(ceil((view_enabled ? int(view_xview[view_current] + view_wview[view_current]) : room_width) / (width_scaled*tbx))) + 1,
    vertil = int(ceil((view_enabled ? int(view_yview[view_current] + view_hview[view_current]) : room_height) / (height_scaled*tby))) + 1;

    gs_scalar xvert1 = xoff, xvert2 = xvert1 + width_scaled, yvert1, yvert2;
    const gs_scalar r = __GETR(blend), g = __GETG(blend), b = __GETB(blend);
    for (int i=0; i<hortil; i++)
    {
        yvert1 = yoff; yvert2 = yvert1 + height_scaled;
        for (int c=0; c<vertil; c++)
        {
            const gs_scalar data[4*8] = {
                 xvert1, yvert1, 0.0, 0.0, r, g, b, alpha,
                 xvert2, yvert1, tbx, 0.0, r, g, b, alpha,
                 xvert2, yvert2, tbx, tby, r, g, b, alpha,
                 xvert1, yvert2, 0.0, tby, r, g, b, alpha
            };
            plane2D_rotated(data);
            yvert1 = yvert2;
            yvert2 += height_scaled;
       }
       xvert1 = xvert2;
       xvert2 += width_scaled;
    }
}

}

