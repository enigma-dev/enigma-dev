/** Copyright (C) 2010-2013 Alasdair Morrison, Harijs Grinbergs, Robert B. Colton
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

#include <cstddef>

#include <math.h>
#include "../General/OpenGLHeaders.h"
#include "../General/GSbackground.h"
#include "../General/GLTextureStruct.h"
#include "Universal_System/backgroundstruct.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Universal_System/spritestruct.h"

#include "Universal_System/roomsystem.h"

#include "GL3binding.h"
#include "GL3shapes.h"

#define __GETR(x) (gs_scalar)(((x & 0x0000FF))/255.0)
#define __GETG(x) (gs_scalar)(((x & 0x00FF00) >> 8)/255.0)
#define __GETB(x) (gs_scalar)(((x & 0xFF0000) >> 16)/255.0)

#include "../General/GStextures.h"
#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_background(bck2d,back)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      show_error("Attempting to draw non-existing background " + toString(back), false);\
      return;\
    }\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
  #define get_backgroundnv(bck2d,back,r)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      show_error("Attempting to draw non-existing background " + toString(back), false);\
      return r;\
    }\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
#else
  #define get_background(bck2d,back)\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
  #define get_backgroundnv(bck2d,back,r)\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
#endif

namespace enigma {
  extern size_t background_idmax;
}

//#include <string.h> // needed for querying ARB extensions

namespace enigma_user
{

void draw_background(int back, gs_scalar x, gs_scalar y)
{
    get_background(bck2d,back);
    texture_use(GmTextures[bck2d->texture]->gltex);

    const gs_scalar tbx=bck2d->texbordx,tby=bck2d->texbordy,
                xvert1 = x, xvert2 = x+bck2d->width,
                yvert1 = y, yvert2 = y+bck2d->height;

    const gs_scalar data[4*8] = {
         xvert1, yvert1, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0,
         xvert2, yvert1, tbx, 0.0, 1.0, 1.0, 1.0, 1.0,
         xvert2, yvert2, tbx, tby, 1.0, 1.0, 1.0, 1.0,
         xvert1, yvert2, 0.0, tby, 1.0, 1.0, 1.0, 1.0
    };
    plane2D_rotated(data);
}

void draw_background_stretched(int back, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height)
{
    get_background(bck2d,back);
    texture_use(GmTextures[bck2d->texture]->gltex);

    const gs_scalar tbx=bck2d->texbordx,tby=bck2d->texbordy,
                xvert1 = x, xvert2 = x+width,
                yvert1 = y, yvert2 = y+height;

    const gs_scalar data[4*8] = {
         xvert1, yvert1, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0,
         xvert2, yvert1, tbx, 0.0, 1.0, 1.0, 1.0, 1.0,
         xvert2, yvert2, tbx, tby, 1.0, 1.0, 1.0, 1.0,
         xvert1, yvert2, 0.0, tby, 1.0, 1.0, 1.0, 1.0
    };
    plane2D_rotated(data);
}

void draw_background_part(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y)
{
    get_background(bck2d,back);
    texture_use(GmTextures[bck2d->texture]->gltex);

    const gs_scalar tbw = bck2d->width/(float)bck2d->texbordx, tbh = bck2d->height/(float)bck2d->texbordy,
          tbx1 = left/tbw, tbx2 = tbx1 + width/tbw,
          tby1 = top/tbh, tby2 = tby1 + height/tbh,
          xvert1 = x, xvert2 = x+width,
          yvert1 = y, yvert2 = y+height;

    const gs_scalar data[4*8] = {
         xvert1, yvert1, tbx1,tby1, 1.0, 1.0, 1.0, 1.0,
         xvert2, yvert1, tbx2,tby1, 1.0, 1.0, 1.0, 1.0,
         xvert2, yvert2, tbx2,tby2, 1.0, 1.0, 1.0, 1.0,
         xvert1, yvert2, tbx1,tby2, 1.0, 1.0, 1.0, 1.0
    };
    plane2D_rotated(data);
}

void draw_background_tiled(int back, gs_scalar x, gs_scalar y)
{
    get_background(bck2d,back);
    texture_use(GmTextures[bck2d->texture]->gltex);

    const gs_scalar tbx  = bck2d->texbordx, tby  = bck2d->texbordy,
    xoff = fmod(x,bck2d->width)-bck2d->width, yoff = fmod(y,bck2d->height)-bck2d->height;

    const int hortil = int(ceil((view_enabled ? int(view_xview[view_current] + view_wview[view_current]) : room_width) / (bck2d->width*tbx))) + 1,
    vertil = int(ceil((view_enabled ? int(view_yview[view_current] + view_hview[view_current]) : room_height) / (bck2d->height*tby))) + 1;

    gs_scalar xvert1 = xoff, xvert2 = xvert1 + bck2d->width, yvert1, yvert2;
    for (int i=0; i<hortil; i++)
    {
        yvert1 = yoff; yvert2 = yvert1 + bck2d->height;
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
            yvert2 += bck2d->height;
        }
        xvert1 = xvert2;
        xvert2 += bck2d->width;
    }
}

void draw_background_tiled_area(int back, gs_scalar x, gs_scalar y, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2)
{
    get_background(bck2d,back);
    texture_use(GmTextures[bck2d->texture]->gltex);

    const gs_scalar tbx=bck2d->texbordx,tby=bck2d->texbordy;
    gs_scalar sw,sh,i,j,jj,left,top,width,height,X,Y;
    sw = bck2d->width;
    sh = bck2d->height;

    i = x1-(fmod(x1,sw) - fmod(x,sw)) - sw*(fmod(x1,sw)<fmod(x,sw));
    j = y1-(fmod(y1,sh) - fmod(y,sh)) - sh*(fmod(y1,sh)<fmod(y,sh));
    jj = j;

    for(; i<=x2; i+=sw)
    {
      for(; j<=y2; j+=sh)
      {
        if(i <= x1) left = x1-i;
        else left = 0;
        X = i+left;

        if(j <= y1) top = y1-j;
        else top = 0;
        Y = j+top;

        if(x2 <= i+sw) width = ((sw)-(i+sw-x2)+1)-left;
        else width = sw-left;

        if(y2 <= j+sh) height = ((sh)-(j+sh-y2)+1)-top;
        else height = sh-top;

        const gs_scalar data[4*8] = {
                 X,Y, left/sw*tbx,top/sh*tby, 1.0, 1.0, 1.0, 1.0,
                 X+width,Y, (left+width)/sw*tbx,top/sh*tby, 1.0, 1.0, 1.0, 1.0,
                 X+width,Y+height, (left+width)/sw*tbx,(top+height)/sh*tby, 1.0, 1.0, 1.0, 1.0,
                 X,Y+height, left/sw*tbx,(top+height)/sh*tby, 1.0, 1.0, 1.0, 1.0
        };
        plane2D_rotated(data);

      }
      j = jj;
    }
}

void draw_background_ext(int back, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int blend, gs_scalar alpha)
{
    get_background(bck2d,back);
    texture_use(GmTextures[bck2d->texture]->gltex);

    rot *= M_PI/180;

    const gs_scalar w = bck2d->width*xscale, h = bck2d->height*yscale,
    tbx = bck2d->texbordx, tby = bck2d->texbordy,
    wsinrot = w*sin(rot), wcosrot = w*cos(rot);

    const gs_scalar ulcx = x - xscale * cos(rot) + yscale * cos(M_PI/2+rot),
    ulcy = y + xscale * sin(rot) - yscale * sin(M_PI/2+rot);
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

void draw_background_stretched_ext(int back, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, int blend, gs_scalar alpha)
{
    get_background(bck2d,back);
    texture_use(GmTextures[bck2d->texture]->gltex);

    const gs_scalar tbx=bck2d->texbordx, tby=bck2d->texbordy;

    const gs_scalar r = __GETR(blend), g = __GETG(blend), b = __GETB(blend);
    const gs_scalar data[4*8] = {
         x, y, 0.0, 0.0, r, g, b, alpha,
         x + width, y, tbx, 0.0, r, g, b, alpha,
         x + width, y + height, tbx, tby, r, g, b, alpha,
         x, y+height, 0.0, tby, r, g, b, alpha
    };
    plane2D_rotated(data);
}

void draw_background_part_ext(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int blend, gs_scalar alpha)
{
    get_background(bck2d,back);
      texture_use(GmTextures[bck2d->texture]->gltex);

    gs_scalar tbw = bck2d->width/(gs_scalar)bck2d->texbordx, tbh = bck2d->height/(gs_scalar)bck2d->texbordy,
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

void draw_background_tiled_ext(int back, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int blend, gs_scalar alpha)
{
    get_background(bck2d,back);
    texture_use(GmTextures[bck2d->texture]->gltex);

    const gs_scalar
    tbx = bck2d->texbordx, tby = bck2d->texbordy,
    width_scaled = bck2d->width*xscale, height_scaled = bck2d->height*yscale;

    x = width_scaled-fmod(x,width_scaled);
    y = height_scaled-fmod(y,height_scaled);

    const int
    hortil = int(ceil(room_width/(width_scaled*tbx))) + 1,
    vertil = int(ceil(room_height/(height_scaled*tby))) + 1;

    gs_scalar xvert1 = -x, xvert2 = xvert1 + width_scaled, yvert1, yvert2;
    const gs_scalar r = __GETR(blend), g = __GETG(blend), b = __GETB(blend);
    for (int i=0; i<hortil; i++)
    {
        yvert1 = -y; yvert2 = yvert1 + height_scaled;
        for (int c=0; c<vertil; c++)
        {
            const gs_scalar data[4*8] = {
                 xvert1,yvert1, 0.0,0.0, r, g, b, alpha,
                 xvert2,yvert1, tbx,0.0, r, g, b, alpha,
                 xvert2,yvert2, tbx,tby, r, g, b, alpha,
                 xvert1,yvert2, 0.0,tby, r, g, b, alpha
            };
            plane2D_rotated(data);

            yvert1 = yvert2;
            yvert2 += height_scaled;
        }
        xvert1 = xvert2;
        xvert2 += width_scaled;
    }
}

void draw_background_tiled_area_ext(int back, gs_scalar x, gs_scalar y, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar xscale, gs_scalar yscale, int blend, gs_scalar alpha)
{
    get_background(bck2d,back);
    texture_use(GmTextures[bck2d->texture]->gltex);

    const gs_scalar tbx=bck2d->texbordx,tby=bck2d->texbordy;
    gs_scalar sw,sh,i,j,jj,left,top,width,height,X,Y;
    sw = bck2d->width*xscale;
    sh = bck2d->height*yscale;

    i = x1-(fmod(x1,sw) - fmod(x,sw)) - sw*(fmod(x1,sw)<fmod(x,sw));
    j = y1-(fmod(y1,sh) - fmod(y,sh)) - sh*(fmod(y1,sh)<fmod(y,sh));
    jj = j;

    const gs_scalar r = __GETR(blend), g = __GETG(blend), b = __GETB(blend);
    for(; i<=x2; i+=sw)
    {
      for(; j<=y2; j+=sh)
      {
        if(i <= x1) left = x1-i;
        else left = 0;
        X = i+left;

        if(j <= y1) top = y1-j;
        else top = 0;
        Y = j+top;

        if(x2 <= i+sw) width = ((sw)-(i+sw-x2)+1)-left;
        else width = sw-left;

        if(y2 <= j+sh) height = ((sh)-(j+sh-y2)+1)-top;
        else height = sh-top;

        const gs_scalar data[4*8] = {
             X,Y, left/sw*tbx,top/sh*tby, r, g, b, alpha,
             X+width,Y, (left+width)/sw*tbx,top/sh*tby, r, g, b, alpha,
             X+width,Y+height, (left+width)/sw*tbx,(top+height)/sh*tby, r, g, b, alpha,
             X,Y+height, left/sw*tbx,(top+height)/sh*tby, r, g, b, alpha
        };
        plane2D_rotated(data);
      }
      j = jj;
    }
}

void draw_background_general(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar a1, gs_scalar a2, gs_scalar a3, gs_scalar a4)
{
    get_background(bck2d,back);
    texture_use(GmTextures[bck2d->texture]->gltex);

    const gs_scalar
      tbx = bck2d->texbordx,  tby = bck2d->texbordy,
      tbw = bck2d->width/tbx, tbh = bck2d->height/tby,
      w = width*xscale, h = height*yscale;

    rot *= M_PI/180;
    const gs_scalar wcosrot = w*cos(rot), wsinrot = w*sin(rot);

    gs_scalar ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
          ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);

    gs_scalar ulcx2 = ulcx + h * cos(3*M_PI/2 + rot),
    ulcy2 = ulcy - h * sin(3*M_PI/2 + rot);

    const gs_scalar data[4*8] = {
         ulcx,ulcy, left/tbw,top/tbh, __GETR(c1), __GETG(c1), __GETB(c1), a1,
         (ulcx + wcosrot), (ulcy - wsinrot), (left+width)/tbw,top/tbh, __GETR(c2), __GETG(c2), __GETB(c2), a2,
         (ulcx2 + wcosrot), (ulcy2 - wsinrot), (left+width)/tbw,(top+height)/tbh, __GETR(c3), __GETG(c3), __GETB(c3), a3,
         ulcx2,ulcy2, left/tbw,(top+height)/tbh, __GETR(c4), __GETG(c4), __GETB(c4), a4
    };
    plane2D_rotated(data);
}

int background_get_texture(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->texture;
}

int background_get_width(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->width;
}

int background_get_height(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->height;
}

double background_get_texture_width_factor(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->texbordx;
}

double background_get_texture_height_factor(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->texbordy;
}

}

