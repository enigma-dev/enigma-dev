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

#include "GScolors.h"
#include "GSprimitives.h"
#include "GSbackground.h"
#include "GStextures.h"
#include "GScolor_macros.h"
#include "Universal_System/nlpo2.h"
#include "Universal_System/Resources/backgrounds_internal.h"
#include "Universal_System/Resources/sprites_internal.h"
#include "Universal_System/math_consts.h"

#include <cstddef>
#include <math.h>
#include <string.h> // needed for querying ARB extensions

using enigma::Background;
using enigma::backgrounds;
using enigma::TexRect;

namespace enigma_user {
  extern int room_width, room_height;
}

namespace enigma_user
{

void draw_background(int back, gs_scalar x, gs_scalar y, int color, gs_scalar alpha)
{
  alpha=CLAMP_ALPHAF(alpha);
	const Background& bck2d = backgrounds.get(back);
  const TexRect& tr = bck2d.textureBounds;

	const gs_scalar tbx=tr.x,tby=tr.y,
                  tbw=tr.w,tbh=tr.h;
	draw_primitive_begin_texture(pr_trianglestrip, bck2d.textureID);
	draw_vertex_texture_color(x,y,tbx,tby,color,alpha);
	draw_vertex_texture_color(x+bck2d.width,y,tbx+tbw,tby,color,alpha);
	draw_vertex_texture_color(x,y+bck2d.height, tbx,tby+tbh,color,alpha);
	draw_vertex_texture_color(x+bck2d.width,y+bck2d.height, tbx+tbw,tby+tbh,color,alpha);
	draw_primitive_end();
}

void draw_background_stretched(int back, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, int color, gs_scalar alpha)
{
  alpha=CLAMP_ALPHAF(alpha);
	const Background& bck2d = backgrounds.get(back);
  const TexRect& tr = bck2d.textureBounds;

	const gs_scalar tbx=tr.x,tby=tr.y,
                  tbw=tr.w,tbh=tr.h;
	draw_primitive_begin_texture(pr_trianglestrip, bck2d.textureID);
	draw_vertex_texture_color(x,y,tbx,tby,color,alpha);
	draw_vertex_texture_color(x+width,y,tbx+tbw,tby,color,alpha);
	draw_vertex_texture_color(x,y+height, tbx,tby+tbh,color,alpha);
	draw_vertex_texture_color(x+width,y+height, tbx+tbw,tby+tbh,color,alpha);
	draw_primitive_end();
}

void draw_background_part(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, int color, gs_scalar alpha)
{
  alpha=CLAMP_ALPHAF(alpha);
	const Background& bck2d = backgrounds.get(back);
  const TexRect& tr = bck2d.textureBounds;

	const gs_scalar tbw = bck2d.width/(gs_scalar)tr.w, tbh = bck2d.height/(gs_scalar)tr.h,
            tbx = tr.x,tby = tr.y,
            tbx1 = tbx + left/tbw, tbx2 = tbx + tbx1 + width/tbw,
            tby1 = tby + top/tbh, tby2 = tby + tby1 + height/tbh;

	draw_primitive_begin_texture(pr_trianglestrip, bck2d.textureID);
	draw_vertex_texture_color(x,y,tbx1,tby1,color,alpha);
	draw_vertex_texture_color(x+width,y,tbx2,tby1,color,alpha);
	draw_vertex_texture_color(x,y+height,tbx1,tby2,color,alpha);
	draw_vertex_texture_color(x+width,y+height,tbx2,tby2,color,alpha);
	draw_primitive_end();
}

void draw_background_ext(int back, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int color, gs_scalar alpha)
{
  alpha=CLAMP_ALPHAF(alpha);
  const Background& bck2d = backgrounds.get(back);
  const TexRect& tr = bck2d.textureBounds;

  rot *= M_PI/180;

  const gs_scalar tbw = tr.w, tbh = tr.h,
                  tbx = tr.x, tby = tr.y,
                  w = bck2d.width*xscale, h = bck2d.height*yscale,
                  wsinrot = w*sin(rot), wcosrot = w*cos(rot);

  draw_primitive_begin_texture(pr_trianglestrip, bck2d.textureID);

  gs_scalar ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
            ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);

	draw_vertex_texture_color(ulcx, ulcy, tbx, tby,color,alpha);
	draw_vertex_texture_color(ulcx + wcosrot, ulcy - wsinrot, tbx+tbw, tby,color,alpha);

  const double mpr = 3*M_PI/2 + rot;
  ulcx += h * cos(mpr);
  ulcy -= h * sin(mpr);

	draw_vertex_texture_color(ulcx, ulcy, tbx, tby+tbh,color,alpha);
	draw_vertex_texture_color(ulcx + wcosrot, ulcy - wsinrot, tbx+tbw, tby+tbh,color,alpha);
  draw_primitive_end();
}

void draw_background_stretched_ext(int back, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, int color, gs_scalar alpha)
{
  alpha=CLAMP_ALPHAF(alpha);
	const Background& bck2d = backgrounds.get(back);
  const TexRect& tr = bck2d.textureBounds;

	const gs_scalar tbx=tr.x, tby=tr.y,
                  tbw=tr.w, tbh=tr.h;

	draw_primitive_begin_texture(pr_trianglestrip, bck2d.textureID);
	draw_vertex_texture_color(x,y,tbx,tby,color,alpha);
	draw_vertex_texture_color(x+width,y,tbx+tbw,tby,color,alpha);
	draw_vertex_texture_color(x,y+height,tbx,tby+tbh,color,alpha);
	draw_vertex_texture_color(x+width,y+height,tbx+tbw,tby+tbh,color,alpha);
	draw_primitive_end();
}

void draw_background_part_ext(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
  alpha=CLAMP_ALPHAF(alpha);
	const Background& bck2d = backgrounds.get(back);
  const TexRect& tr = bck2d.textureBounds;

	gs_scalar tbw = bck2d.width/(gs_scalar)tr.w, tbh = bck2d.height/(gs_scalar)tr.h,
	  tbx=tr.x, tby=tr.y,
	  xvert1 = x, xvert2 = xvert1 + width*xscale,
	  yvert1 = y, yvert2 = yvert1 + height*yscale,
	  tbx1 = tbx + left/tbw, tbx2 = tbx + tbx1 + width/tbw,
	  tby1 = tby + top/tbh, tby2 = tby + tby1 + height/tbh;

	draw_primitive_begin_texture(pr_trianglestrip, bck2d.textureID);
	draw_vertex_texture_color(xvert1,yvert1,tbx1,tby1,color,alpha);
	draw_vertex_texture_color(xvert2,yvert1,tbx2,tby1,color,alpha);
	draw_vertex_texture_color(xvert1,yvert2,tbx1,tby2,color,alpha);
	draw_vertex_texture_color(xvert2,yvert2,tbx2,tby2,color,alpha);
	draw_primitive_end();
}

void draw_background_general(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar alpha)
{
  alpha=CLAMP_ALPHAF(alpha);
	const Background& bck2d = backgrounds.get(back);
  const TexRect& tr = bck2d.textureBounds;
  
  const gs_scalar tbx = tr.x,  tby = tr.y,
                  tbw = tr.w,  tbh = tr.h,
                  ttbw = bck2d.width/tbw, ttbh = bck2d.height/tbh,
                  tbx1 = tbx+left/ttbw, tby1 = tby+top/ttbh,
                  tbx2 = tbx+(left+width)/ttbw, tby2 = tby+(top+height)/ttbh,
                  w = width*xscale, h = height*yscale;

  rot *= M_PI/180;
  const gs_scalar wcosrot = w*cos(rot), wsinrot = w*sin(rot);

  gs_scalar ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
            ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);

  draw_primitive_begin_texture(pr_trianglestrip, bck2d.textureID);
	draw_vertex_texture_color(ulcx, ulcy, tbx1, tby1,c1,alpha);
	draw_vertex_texture_color((ulcx + wcosrot), (ulcy - wsinrot), tbx2,tby1, c2, alpha);

	ulcx += h * cos(3*M_PI/2 + rot);
	ulcy -= h * sin(3*M_PI/2 + rot);

	draw_vertex_texture_color(ulcx, ulcy, tbx1, tby2,c3,alpha);
	draw_vertex_texture_color((ulcx + wcosrot), (ulcy - wsinrot), tbx2, tby2,c4,alpha);
  draw_primitive_end();
}

void draw_background_tiled(int back, gs_scalar x, gs_scalar y, int color, gs_scalar alpha)
{
  alpha=CLAMP_ALPHAF(alpha);
  const Background& bck2d = backgrounds.get(back); 
  const TexRect& tr = bck2d.textureBounds;
  
  x = (x<0?0:bck2d.width)-fmod(x,bck2d.width);
  y = (y<0?0:bck2d.height)-fmod(y,bck2d.height);

  const gs_scalar tbx = tr.x, tby = tr.y,
                  tbw = tr.w, tbh = tr.h;

  const int hortil = int (ceil((gs_scalar)room_width/((gs_scalar)bck2d.width))) + 1,
            vertil = int (ceil((gs_scalar)room_height/((gs_scalar)bck2d.height))) + 1;

  gs_scalar xvert1 = -x, xvert2 = xvert1 + bck2d.width, yvert1, yvert2;
  for (int i=0; i<hortil; ++i)
  {
    yvert1 = -y; yvert2 = yvert1 + bck2d.height;
    for (int c=0; c<vertil; ++c)
    {
      draw_primitive_begin_texture(pr_trianglestrip, bck2d.textureID);
      draw_vertex_texture_color(xvert1,yvert1,tbx,tby,color,alpha);
      draw_vertex_texture_color(xvert2,yvert1,tbx+tbw,tby,color,alpha);
      draw_vertex_texture_color(xvert1,yvert2,tbx,tby+tbh,color,alpha);
      draw_vertex_texture_color(xvert2,yvert2,tbx+tbw,tby+tbh,color,alpha);
      draw_primitive_end();
      yvert1 = yvert2;
      yvert2 += bck2d.height;
    }
    xvert1 = xvert2;
    xvert2 += bck2d.width;
  }
}

void draw_background_tiled_ext(int back, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha, bool htiled, bool vtiled)
{
  alpha=CLAMP_ALPHAF(alpha);
	const Background& bck2d = backgrounds.get(back); 
  const TexRect& tr = bck2d.textureBounds;

	const gs_scalar tbx = tr.x, tby = tr.y,
                  tbw = tr.w, tbh = tr.h,
                  width_scaled = bck2d.width*xscale, height_scaled = bck2d.height*yscale;

  int hortil, vertil;
	if (htiled) {
		hortil = int(ceil((gs_scalar)room_width/((gs_scalar)width_scaled*tbw))) + 1;
		x = -((x<0?0:width_scaled)-fmod(x,width_scaled));
	} else {
		hortil = 1;
	}
	if (vtiled) {
		vertil = int(ceil((gs_scalar)room_height/((gs_scalar)height_scaled*tbh))) + 1;
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
      draw_primitive_begin_texture(pr_trianglestrip, bck2d.textureID);
      draw_vertex_texture_color(xvert1,yvert1,tbx,tby,color,alpha);
      draw_vertex_texture_color(xvert2,yvert1,tbx+tbw,tby,color,alpha);
      draw_vertex_texture_color(xvert1,yvert2,tbx,tby+tbh,color,alpha);
      draw_vertex_texture_color(xvert2,yvert2,tbx+tbw,tby+tbh,color,alpha);
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
  alpha=CLAMP_ALPHAF(alpha);
	const Background& bck2d = backgrounds.get(back);
  const TexRect& tr = bck2d.textureBounds;

  const gs_scalar tbx=tr.x,tby=tr.y,
                  tbw=tr.w,tbh=tr.h;

  gs_scalar sw,sh,i,j,jj,left,top,width,height,X,Y, tbx1, tbx2, tby1, tby2;
  sw = bck2d.width;
  sh = bck2d.height;

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

      tbx1 = tbx+left/sw*tbw, tby1 = tby+top/sh*tbh,
      tbx2 = tbx+(left+width)/sw*tbw, tby2 = tby+(top+height)/sh*tbh;

      draw_primitive_begin_texture(pr_trianglestrip, bck2d.textureID);
      draw_vertex_texture_color(X,Y,tbx1,tby1,color,alpha);
      draw_vertex_texture_color(X+width,Y,tbx2,tby1,color,alpha);
      draw_vertex_texture_color(X,Y+height,tbx1,tby2,color,alpha);
          draw_vertex_texture_color(X+width,Y+height,tbx2,tby2,color,alpha);
      draw_primitive_end();
    }
    j = jj;
  }
}

void draw_background_tiled_area_ext(int back, gs_scalar x, gs_scalar y, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
  alpha=CLAMP_ALPHAF(alpha);
	const Background& bck2d = backgrounds.get(back);
  const TexRect& tr = bck2d.textureBounds;

  const gs_scalar tbx=tr.x,tby=tr.y,
                  tbw=tr.w,tbh=tr.h;

  gs_scalar sw,sh,i,j,jj,left,top,width,height,X,Y, tbx1, tbx2, tby1, tby2;
  sw = bck2d.width*xscale;
  sh = bck2d.height*yscale;

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

      tbx1 = tbx+left/sw*tbw, tby1 = tby+top/sh*tbh,
      tbx2 = tbx+(left+width)/sw*tbw, tby2 = tby+(top+height)/sh*tbh;

      draw_primitive_begin_texture(pr_trianglestrip, bck2d.textureID);
      draw_vertex_texture_color(X,Y,tbx1,tby1,color,alpha);
      draw_vertex_texture_color(X+width,Y,tbx2,tby1,color,alpha);
      draw_vertex_texture_color(X,Y+height,tbx1,tby2,color,alpha);
          draw_vertex_texture_color(X+width,Y+height,tbx2,tby2,color,alpha);
      draw_primitive_end();
    }
    j = jj;
  }
}

double background_get_texture_width_factor(int back) {
  const TexRect& tr = backgrounds.get(back).textureBounds;
  return tr.w;
}

double background_get_texture_height_factor(int back) {
  const TexRect& tr = backgrounds.get(back).textureBounds;
  return tr.h;
}

void d3d_draw_background(int back, gs_scalar x, gs_scalar y, gs_scalar z)
{
	const Background& bck2d = backgrounds.get(back);
  const TexRect& tr = bck2d.textureBounds;

	const gs_scalar tbx=tr.x,tby=tr.y,
                  tbw=tr.w,tbh=tr.h;
	draw_primitive_begin_texture(pr_trianglestrip, bck2d.textureID);
	d3d_vertex_texture(x,y,z,tbx,tby);
	d3d_vertex_texture(x+bck2d.width,y,z,tbx+tbw,tby);
	d3d_vertex_texture(x,y+bck2d.height,z, tbx,tby+tbh);
	d3d_vertex_texture(x+bck2d.width,y+bck2d.height,z, tbx+tbw,tby+tbh);
	draw_primitive_end();
}

}
