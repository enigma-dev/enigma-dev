/** Copyright (C) 2010-2013 Alasdair Morrison, Robert B. Colton
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
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GSbackground.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Universal_System/nlpo2.h"
#include "Universal_System/backgroundstruct.h"
#include "Universal_System/spritestruct.h"

#define M_PI		3.14159265358979323846

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

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

namespace enigma_user {
  extern int room_width, room_height;
}
namespace enigma {
  extern size_t background_idmax;
}


#include <string.h> // needed for querying ARB extensions

namespace enigma_user
{

void draw_background(int back, gs_scalar x, gs_scalar y, int color, gs_scalar alpha)
{
	get_background(bck2d,back);

	const gs_scalar tbx=bck2d->texbordx,tby=bck2d->texbordy;
	draw_primitive_begin_texture(pr_trianglestrip, bck2d->texture);
	draw_vertex_texture_color(x,y,0,0,color,alpha);
	draw_vertex_texture_color(x+bck2d->width,y,tbx,0,color,alpha);
	draw_vertex_texture_color(x,y+bck2d->height, 0,tby,color,alpha);
	draw_vertex_texture_color(x+bck2d->width,y+bck2d->height, tbx,tby,color,alpha);
	draw_primitive_end();
}

void draw_background_stretched(int back, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, int color, gs_scalar alpha)
{
	get_background(bck2d, back);

	const gs_scalar tbx=bck2d->texbordx,tby=bck2d->texbordy;
	draw_primitive_begin_texture(pr_trianglestrip, bck2d->texture);
	draw_vertex_texture_color(x,y,0,0,color,alpha);
	draw_vertex_texture_color(x+width,y,tbx,0,color,alpha);
	draw_vertex_texture_color(x,y+height, 0,tby,color,alpha);
	draw_vertex_texture_color(x+width,y+height, tbx,tby,color,alpha);
	draw_primitive_end();
}

void draw_background_part(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, int color, gs_scalar alpha)
{
	get_background(bck2d, back);

	gs_scalar tbw = bck2d->width/(gs_scalar)bck2d->texbordx, tbh = bck2d->height/(gs_scalar)bck2d->texbordy,
	  tbx1 = left/tbw, tbx2 = tbx1 + width/tbw,
	  tby1 = top/tbh, tby2 = tby1 + height/tbh;

	draw_primitive_begin_texture(pr_trianglestrip, bck2d->texture);
	draw_vertex_texture_color(x,y,tbx1,tby1,color,alpha);
	draw_vertex_texture_color(x+width,y,tbx2,tby1,color,alpha);
	draw_vertex_texture_color(x,y+height,tbx1,tby2,color,alpha);
	draw_vertex_texture_color(x+width,y+height,tbx2,tby2,color,alpha);
	draw_primitive_end();
}

void draw_background_ext(int back, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int color, gs_scalar alpha)
{
    get_background(bck2d,back);

    rot *= M_PI/180;

    const gs_scalar
    tbx = bck2d->texbordx, tby = bck2d->texbordy,
    w = bck2d->width*xscale, h = bck2d->height*yscale,
    wsinrot = w*sin(rot), wcosrot = w*cos(rot);

    draw_primitive_begin_texture(pr_trianglestrip, bck2d->texture);

    gs_scalar
    ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
    ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);

	draw_vertex_texture_color(ulcx, ulcy, 0, 0,color,alpha);
	draw_vertex_texture_color(ulcx + wcosrot, ulcy - wsinrot, tbx, 0,color,alpha);

    const double mpr = 3*M_PI/2 + rot;
    ulcx += h * cos(mpr);
    ulcy -= h * sin(mpr);

	draw_vertex_texture_color(ulcx, ulcy, 0, tby,color,alpha);
	draw_vertex_texture_color(ulcx + wcosrot, ulcy - wsinrot, tbx, tby,color,alpha);
    draw_primitive_end();

}

void draw_background_stretched_ext(int back, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, int color, gs_scalar alpha)
{
	get_background(bck2d,back);

	const gs_scalar tbx=bck2d->texbordx, tby=bck2d->texbordy;

	draw_primitive_begin_texture(pr_trianglestrip, bck2d->texture);
	draw_vertex_texture_color(x,y,0,0,color,alpha);
	draw_vertex_texture_color(x+width,y,tbx,0,color,alpha);
	draw_vertex_texture_color(x,y+height,0,tby,color,alpha);
	draw_vertex_texture_color(x+width,y+height,tbx,tby,color,alpha);
	draw_primitive_end();
}

void draw_background_part_ext(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
	get_background(bck2d, back);

	gs_scalar tbw = bck2d->width/(gs_scalar)bck2d->texbordx, tbh = bck2d->height/(gs_scalar)bck2d->texbordy,
	  xvert1 = x, xvert2 = xvert1 + width*xscale,
	  yvert1 = y, yvert2 = yvert1 + height*yscale,
	  tbx1 = left/tbw, tbx2 = tbx1 + width/tbw,
	  tby1 = top/tbh, tby2 = tby1 + height/tbh;

	draw_primitive_begin_texture(pr_trianglestrip, bck2d->texture);
	draw_vertex_texture_color(xvert1,yvert1,tbx1,tby1,color,alpha);
	draw_vertex_texture_color(xvert2,yvert1,tbx2,tby1,color,alpha);
	draw_vertex_texture_color(xvert1,yvert2,tbx1,tby2,color,alpha);
	draw_vertex_texture_color(xvert2,yvert2,tbx2,tby2,color,alpha);
	draw_primitive_end();
}

void draw_background_general(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar alpha)
{
	get_background(bck2d, back);
    const gs_scalar
      tbx = bck2d->texbordx,  tby = bck2d->texbordy,
      tbw = bck2d->width/tbx, tbh = bck2d->height/tby,
      w = width*xscale, h = height*yscale;

    rot *= M_PI/180;
    const gs_scalar wcosrot = w*cos(rot), wsinrot = w*sin(rot);

    gs_scalar ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
          ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);

    draw_primitive_begin_texture(pr_trianglestrip, bck2d->texture);
	draw_vertex_texture_color(ulcx,ulcy,left/tbw,top/tbh,c1,alpha);
	draw_vertex_texture_color((ulcx + wcosrot), (ulcy - wsinrot), (left+width)/tbw,top/tbh, c2, alpha);

	ulcx += h * cos(3*M_PI/2 + rot);
	ulcy -= h * sin(3*M_PI/2 + rot);

	draw_vertex_texture_color(ulcx, ulcy, left/tbw, (top+height)/tbh,c3,alpha);
	draw_vertex_texture_color((ulcx + wcosrot), (ulcy - wsinrot), (left+width)/tbw,(top+height)/tbh,c4,alpha);
    draw_primitive_end();
}

void draw_background_tiled(int back, gs_scalar x, gs_scalar y, int color, gs_scalar alpha)
{
    get_background(bck2d,back);
    x = (x<0?0:bck2d->width)-fmod(x,bck2d->width);
    y = (y<0?0:bck2d->height)-fmod(y,bck2d->height);

    const gs_scalar tbx = bck2d->texbordx,tby=bck2d->texbordy;

    const int
    hortil = int (ceil((gs_scalar)room_width/((gs_scalar)bck2d->width))) + 1,
    vertil = int (ceil((gs_scalar)room_height/((gs_scalar)bck2d->height))) + 1;

    gs_scalar xvert1 = -x, xvert2 = xvert1 + bck2d->width, yvert1, yvert2;
    for (int i=0; i<hortil; ++i)
    {
        yvert1 = -y; yvert2 = yvert1 + bck2d->height;
        for (int c=0; c<vertil; ++c)
        {
			draw_primitive_begin_texture(pr_trianglestrip, bck2d->texture);
			draw_vertex_texture_color(xvert1,yvert1,0,0,color,alpha);
			draw_vertex_texture_color(xvert2,yvert1,tbx,0,color,alpha);
			draw_vertex_texture_color(xvert1,yvert2,0,tby,color,alpha);
			draw_vertex_texture_color(xvert2,yvert2,tbx,tby,color,alpha);
			draw_primitive_end();
            yvert1 = yvert2;
            yvert2 += bck2d->height;
        }
        xvert1 = xvert2;
        xvert2 += bck2d->width;
    }
}

void draw_background_tiled_ext(int back, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha, bool htiled, bool vtiled)
{
	get_background(bck2d,back);

	const gs_scalar
	  tbx = bck2d->texbordx, tby = bck2d->texbordy,
	  width_scaled = bck2d->width*xscale, height_scaled = bck2d->height*yscale;

    int hortil, vertil;
	if (htiled) {
		hortil = int(ceil((gs_scalar)room_width/((gs_scalar)width_scaled*tbx))) + 1;
		x = -((x<0?0:width_scaled)-fmod(x,width_scaled));
	} else {
		hortil = 1;
	}
	if (vtiled) {
		vertil = int(ceil((gs_scalar)room_height/((gs_scalar)height_scaled*tby))) + 1;
		y = -((y<0?0:height_scaled)-fmod(y,height_scaled));
	} else {
		vertil = 1;
	}

    gs_scalar xvert1 = x, xvert2 = xvert1 + width_scaled, yvert1, yvert2;
    for (int i=0; i<hortil; ++i)
    {
        yvert1 = y; yvert2 = yvert1 + height_scaled;
        for (int c=0; c<vertil; ++c)
        {
			draw_primitive_begin_texture(pr_trianglestrip, bck2d->texture);
			draw_vertex_texture_color(xvert1,yvert1,0,0,color,alpha);
			draw_vertex_texture_color(xvert2,yvert1,tbx,0,color,alpha);
			draw_vertex_texture_color(xvert1,yvert2,0,tby,color,alpha);
			draw_vertex_texture_color(xvert2,yvert2,tbx,tby,color,alpha);
			draw_primitive_end();
            yvert1 = yvert2;
            yvert2 += height_scaled;
        }
        xvert1 = xvert2;
        xvert2 += width_scaled;
    }
}

void draw_background_tiled_area(int back, gs_scalar x, gs_scalar y, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, int color, gs_scalar alpha)
{
	get_background(bck2d,back);

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

		draw_primitive_begin_texture(pr_trianglestrip, bck2d->texture);
		draw_vertex_texture_color(X,Y,left/sw*tbx,top/sh*tby,color,alpha);
		draw_vertex_texture_color(X+width,Y,(left+width)/sw*tbx,top/sh*tby,color,alpha);
		draw_vertex_texture_color(X,Y+height,left/sw*tbx,(top+height)/sh*tby,color,alpha);
        draw_vertex_texture_color(X+width,Y+height,(left+width)/sw*tbx,(top+height)/sh*tby,color,alpha);
		draw_primitive_end();
      }
      j = jj;
    }
}

void draw_background_tiled_area_ext(int back, gs_scalar x, gs_scalar y, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
	get_background(bck2d,back);

    const gs_scalar tbx=bck2d->texbordx,tby=bck2d->texbordy;
    gs_scalar sw,sh,i,j,jj,left,top,width,height,X,Y;
    sw = bck2d->width*xscale;
    sh = bck2d->height*yscale;

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

		draw_primitive_begin_texture(pr_trianglestrip, bck2d->texture);
		draw_vertex_texture_color(X,Y,left/sw*tbx,top/sh*tby,color,alpha);
		draw_vertex_texture_color(X+width,Y,(left+width)/sw*tbx,top/sh*tby,color,alpha);
		draw_vertex_texture_color(X,Y+height,left/sw*tbx,(top+height)/sh*tby,color,alpha);
        draw_vertex_texture_color(X+width,Y+height,(left+width)/sw*tbx,(top+height)/sh*tby,color,alpha);
		draw_primitive_end();
      }
      j = jj;
    }
}

double background_get_texture_width_factor(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->texbordx;
}

double background_get_texture_height_factor(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->texbordy;
}

void d3d_draw_background(int back, gs_scalar x, gs_scalar y, gs_scalar z)
{
	get_background(bck2d,back);

	const gs_scalar tbx=bck2d->texbordx,tby=bck2d->texbordy;
	draw_primitive_begin_texture(pr_trianglestrip, bck2d->texture);
	d3d_vertex_texture(x,y,z,0,0);
	d3d_vertex_texture(x+bck2d->width,y,z,tbx,0);
	d3d_vertex_texture(x,y+bck2d->height,z, 0,tby);
	d3d_vertex_texture(x+bck2d->width,y+bck2d->height,z, tbx,tby);
	draw_primitive_end();
}

}

