/** Copyright (C) 2013 Robert B. Colton
*** Copyright (C) 2014 Seth N. Hetu
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

#include "GSsurface.h"
#include "GSsurface_impl.h"
#include "GSprimitives.h"
#include "GScolor_macros.h"
#include "Graphics_Systems/graphics_mandatory.h"

#include "Universal_System/image_formats.h"
#include "Universal_System/nlpo2.h"
#include "Universal_System/Resources/sprites_internal.h"
#include "Universal_System/Resources/backgrounds_internal.h"
#include "Collision_Systems/collision_types.h"
#include "Universal_System/math_consts.h"

#include <stdio.h> //for file writing (surface_save)
#include <cstddef>
#include <iostream>
#include <math.h>

using namespace std;
using enigma::Sprite;
using enigma::sprites;

namespace enigma {

vector<BaseSurface*> surfaces;

} // namespace enigma

namespace enigma_user {

extern int room_width, room_height/*, sprite_idmax*/;

bool surface_exists(int id)
{
  return !((id < 0) or (size_t(id) > enigma::surfaces.size()) or (enigma::surfaces[id] == NULL));
}

int surface_get_texture(int id)
{
  get_surfacev(surf,id,-1);
  return ((enigma::BaseSurface&)surf).texture;
}

int surface_get_width(int id)
{
  get_surfacev(surf,id,-1);
  return ((enigma::BaseSurface&)surf).width;
}

int surface_get_height(int id)
{
  get_surfacev(surf,id,-1);
  return ((enigma::BaseSurface&)surf).height;
}

void draw_surface(int id, gs_scalar x, gs_scalar y, int color, gs_scalar alpha)
{
  alpha=CLAMP_ALPHAF(alpha);
	int w=surface_get_width(id);
	int h=surface_get_height(id);

	draw_primitive_begin_texture(pr_trianglestrip, surface_get_texture(id));
	draw_vertex_texture_color(x,y,0,0,color,alpha);
	draw_vertex_texture_color(x+w,y,1,0,color,alpha);
	draw_vertex_texture_color(x,y+h,0,1,color,alpha);
	draw_vertex_texture_color(x+w,y+h,1,1,color,alpha);
	draw_primitive_end();
}

void draw_surface_ext(int id,gs_scalar x, gs_scalar y,gs_scalar xscale, gs_scalar yscale,double rot,int color,gs_scalar alpha)
{
    alpha=CLAMP_ALPHAF(alpha);
    const gs_scalar w=surface_get_width(id)*xscale, h=surface_get_height(id)*yscale;
    rot *= M_PI/180;

    gs_scalar ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
          ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);

	draw_primitive_begin_texture(pr_trianglestrip, surface_get_texture(id));
	draw_vertex_texture_color(ulcx,ulcy,0,0,color,alpha);
	draw_vertex_texture_color(ulcx + w*cos(rot), ulcy - w*sin(rot),1,0,color,alpha);
	ulcx += h * cos(3*M_PI/2 + rot);
	ulcy -= h * sin(3*M_PI/2 + rot);
	draw_vertex_texture_color(ulcx,ulcy,0,1,color,alpha);
	draw_vertex_texture_color(ulcx + w*cos(rot), ulcy - w*sin(rot),1,1,color,alpha);
	draw_primitive_end();
}

void draw_surface_stretched(int id, gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, int color, gs_scalar alpha)
{
  alpha=CLAMP_ALPHAF(alpha);
	draw_primitive_begin_texture(pr_trianglestrip, surface_get_texture(id));
	draw_vertex_texture_color(x,y,0,0,color,alpha);
	draw_vertex_texture_color(x+w,y,1,0,color,alpha);
	draw_vertex_texture_color(x,y+h,0,1,color,alpha);
	draw_vertex_texture_color(x+w,y+h,1,1,color,alpha);
	draw_primitive_end();
}

void draw_surface_stretched_ext(int id, gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, int color, gs_scalar alpha)
{
  alpha=CLAMP_ALPHAF(alpha);
	draw_primitive_begin_texture(pr_trianglestrip, surface_get_texture(id));
	draw_vertex_texture_color(x,y,0,0,color,alpha);
	draw_vertex_texture_color(x+w,y,1,0,color,alpha);
	draw_vertex_texture_color(x,y+h,0,1,color,alpha);
	draw_vertex_texture_color(x+w,y+h,1,1,color,alpha);
	draw_primitive_end();
}

void draw_surface_part(int id, gs_scalar left, gs_scalar top, gs_scalar w, gs_scalar h, gs_scalar x, gs_scalar y, int color, gs_scalar alpha)
{
  alpha=CLAMP_ALPHAF(alpha);
	const gs_scalar tbw=surface_get_width(id),tbh=surface_get_height(id);

	draw_primitive_begin_texture(pr_trianglestrip, surface_get_texture(id));
	draw_vertex_texture_color(x,y,left/tbw,top/tbh,color,alpha);
	draw_vertex_texture_color(x+w,y,(left+w)/tbw,top/tbh,color,alpha);
	draw_vertex_texture_color(x,y+h,left/tbw,(top+h)/tbh,color,alpha);
	draw_vertex_texture_color(x+w,y+h,(left+w)/tbw,(top+h)/tbh,color,alpha);
	draw_primitive_end();
}

void draw_surface_part_ext(int id, gs_scalar left, gs_scalar top, gs_scalar w, gs_scalar h, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale,int color, gs_scalar alpha)
{
  alpha=CLAMP_ALPHAF(alpha);
	const gs_scalar tbw = surface_get_width(id), tbh = surface_get_height(id);
	draw_primitive_begin_texture(pr_trianglestrip, surface_get_texture(id));
	draw_vertex_texture_color(x,y,left/tbw,top/tbh,color,alpha);
	draw_vertex_texture_color(x+w*xscale,y,(left+w)/tbw,top/tbh,color,alpha);
	draw_vertex_texture_color(x,y+h*yscale,left/tbw,(top+h)/tbh,color,alpha);
	draw_vertex_texture_color(x+w*xscale,y+h*yscale,(left+w)/tbw,(top+h)/tbh,color,alpha);
	draw_primitive_end();
}

void draw_surface_general(int id, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar alpha)
{
  alpha=CLAMP_ALPHAF(alpha);
	const gs_scalar tbw = surface_get_width(id), tbh = surface_get_height(id),
	  w = width*xscale, h = height*yscale;

    rot *= M_PI/180;

    gs_scalar ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
          ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);

    draw_primitive_begin_texture(pr_trianglestrip, surface_get_texture(id));
	draw_vertex_texture_color(ulcx,ulcy,left/tbw,top/tbh,c1,alpha);
	draw_vertex_texture_color((ulcx + w*cos(rot)), (ulcy - w*sin(rot)), (left+width)/tbw,top/tbh, c2, alpha);

      ulcx += h * cos(3*M_PI/2 + rot);
      ulcy -= h * sin(3*M_PI/2 + rot);

	draw_vertex_texture_color((ulcx + w*cos(rot)), (ulcy - w*sin(rot)), (left+width)/tbw,(top+height)/tbh, c4, alpha);
	draw_vertex_texture_color(ulcx, ulcy, left/tbw, (top+height)/tbh, c3, alpha);
    draw_primitive_end();
}

void draw_surface_tiled(int id, gs_scalar x, gs_scalar y, int color, gs_scalar alpha)
{
  alpha=CLAMP_ALPHAF(alpha);
	const gs_scalar tbw = surface_get_width(id), tbh = surface_get_height(id);
	x=surface_get_width(id)-fmod(x,surface_get_width(id));
	y=surface_get_height(id)-fmod(y,surface_get_height(id));
	const int hortil= int (ceil(room_width/(surface_get_width(id)))),
			  vertil= int (ceil(room_height/(surface_get_height(id))));


	for (int i=0; i<hortil; i++)
	{
		for (int c=0; c<vertil; c++)
		{
			draw_primitive_begin_texture(pr_trianglestrip, surface_get_texture(id));
			draw_vertex_texture_color(i*tbw-x,c*tbh-y,0,0,color,alpha);
			draw_vertex_texture_color((i+1)*tbw-x,c*tbh-y,1,0,color,alpha);
			draw_vertex_texture_color(i*tbw-x,(c+1)*tbh-y,0,1,color,alpha);
			draw_vertex_texture_color((i+1)*tbw-x,(c+1)*tbh-y,1,1,color,alpha);
			draw_primitive_end();
		}
	}
}

void draw_surface_tiled_ext(int id, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
    alpha=CLAMP_ALPHAF(alpha);
    const gs_scalar w=surface_get_width(id)*xscale, h=surface_get_height(id)*yscale;
    const int hortil= int (ceil(room_width/(surface_get_width(id)))),
        vertil= int (ceil(room_height/(surface_get_height(id))));
    x=w-fmod(x,w);
    y=h-fmod(y,h);

    for (int i=0; i<hortil; i++)
    {
      for (int c=0; c<vertil; c++)
      {
		draw_primitive_begin_texture(pr_trianglestrip, surface_get_texture(id));
		draw_vertex_texture_color(i*w-x,c*h-y,0,0,color,alpha);
		draw_vertex_texture_color((i+1)*w-x,c*h-y,1,0,color,alpha);
		draw_vertex_texture_color(i*w-x,(c+1)*h-y,0,1,color,alpha);
		draw_vertex_texture_color((i+1)*w-x,(c+1)*h-y,1,1,color,alpha);
		draw_primitive_end();
      }
    }
}

void draw_surface_tiled_area(int id, gs_scalar x, gs_scalar y, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, int color, gs_scalar alpha)
{
    alpha=CLAMP_ALPHAF(alpha);
    gs_scalar sw,sh,i,j,jj,left,top,width,height,X,Y;
    sw = surface_get_width(id);
    sh = surface_get_height(id);

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

		draw_primitive_begin_texture(pr_trianglestrip, surface_get_texture(id));
		draw_vertex_texture_color(X,Y,left/sw,top/sh,color,alpha);
		draw_vertex_texture_color(X+width,Y,(left+width)/sw,top/sh,color,alpha);
		draw_vertex_texture_color(X,Y+height,left/sw,(top+height)/sh,color,alpha);
		draw_vertex_texture_color(X+width,Y+height,(left+width)/sw,(top+height)/sh,color,alpha);
		draw_primitive_end();
      }
      j = jj;
    }
}

void draw_surface_tiled_area_ext(int id, gs_scalar x, gs_scalar y, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
    alpha=CLAMP_ALPHAF(alpha);
    gs_scalar sw,sh,i,j,jj,left,top,width,height,X,Y;
    sw = surface_get_width(id)*xscale;
    sh = surface_get_height(id)*yscale;

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

		draw_primitive_begin_texture(pr_trianglestrip, surface_get_texture(id));
		draw_vertex_texture_color(X,Y,left/sw,top/sh,color,alpha);
		draw_vertex_texture_color(X+width,Y,(left+width)/sw,top/sh,color,alpha);
		draw_vertex_texture_color(X,Y+height,left/sw,(top+height)/sh,color,alpha);
		draw_vertex_texture_color(X+width,Y+height,(left+width)/sw,(top+height)/sh,color,alpha);
		draw_primitive_end();
      }
      j = jj;
    }
}

//////////////////////////////////////SAVE TO FILE AND CTEATE SPRITE FUNCTIONS/////////

int surface_getpixel(int id, int x, int y)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surf,id,-1);
  const enigma::BaseSurface& base = ((enigma::BaseSurface&)surf);
  unsigned char *surfbuf=enigma::graphics_copy_texture_pixels(base.texture,x,y,1,1);
  int ret = surfbuf[2] + (surfbuf[1] << 8) + (surfbuf[0] << 16);
  delete[] surfbuf;
  return ret;
}

int surface_getpixel_ext(int id, int x, int y)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surf,id,-1);
  const enigma::BaseSurface& base = ((enigma::BaseSurface&)surf);
  unsigned char *surfbuf=enigma::graphics_copy_texture_pixels(base.texture,x,y,1,1);
  int ret = surfbuf[2] + (surfbuf[1] << 8) + (surfbuf[0] << 16) + (surfbuf[3] << 24);
  delete[] surfbuf;
  return ret;
}

int surface_getpixel_alpha(int id, int x, int y)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surf,id,-1);
  const enigma::BaseSurface& base = ((enigma::BaseSurface&)surf);
  unsigned char *surfbuf=enigma::graphics_copy_texture_pixels(base.texture,x,y,1,1);
  int ret = surfbuf[3];
  delete[] surfbuf;
  return ret;
}

int surface_save(int id, string filename)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surf,id,-1);
  const enigma::BaseSurface& base = ((enigma::BaseSurface&)surf);
  const gs_scalar w=base.width,h=base.height;
  unsigned fw=0, fh=0;

  unsigned char *surfbuf=enigma::graphics_copy_texture_pixels(base.texture,&fw,&fh);

  int ret = enigma::image_save(filename, surfbuf, w, h, fw, fh, false);
  delete[] surfbuf;
  return ret;
}

int surface_save_part(int id, string filename, unsigned x, unsigned y, unsigned w, unsigned h)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surf,id,-1);
  const enigma::BaseSurface& base = ((enigma::BaseSurface&)surf);

  unsigned char *surfbuf=enigma::graphics_copy_texture_pixels(base.texture,x,y,w,h);

  int ret = enigma::image_save(filename, surfbuf, w, h, w, h, false);
  delete[] surfbuf;
  return ret;
}

int background_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, bool preload)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surf,id,-1);
  const enigma::BaseSurface& base = ((enigma::BaseSurface&)surf);

  unsigned char *surfbuf = enigma::graphics_copy_texture_pixels(base.texture,x,y,w,h);
  
  if (removeback) {
    enigma::Color c = enigma::image_get_pixel_color(surfbuf, w, h, 0, h - 1);
    enigma::image_swap_color(surfbuf, w, h, c, enigma::Color {0, 0, 0, 0});
  }
  
  enigma::RawImage img = enigma::image_pad(surfbuf, w, h, enigma::nlpo2dc(w)+1, enigma::nlpo2dc(h)+1);
  enigma::Background bkg(w, h, enigma::graphics_create_texture(w, h, img.w, img.h, img.pxdata, false));
  
  delete[] surfbuf;

  return enigma::backgrounds.add(std::move(bkg));
}

int sprite_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, bool preload, int xorig, int yorig)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surf,id,-1);
  const enigma::BaseSurface& base = ((enigma::BaseSurface&)surf);

  unsigned char *surfbuf=enigma::graphics_copy_texture_pixels(base.texture,x,y,w,h);
  
  Sprite spr(w, h, xorig, yorig);
  spr.AddSubimage(surfbuf, w, h, enigma::ct_precise, surfbuf); //TODO: Support toggling of precise.
  
  delete[] surfbuf;
  
  return sprites.add(std::move(spr));
}

int sprite_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig)
{
  return sprite_create_from_surface(id, x, y, w, h, removeback, smooth, true, xorig, yorig);
}

void sprite_add_from_surface(int ind, int id, int x, int y, int w, int h, bool removeback, bool smooth)
{
  draw_batch_flush(batch_flush_deferred);

  get_surface(surf,id);
  const enigma::BaseSurface& base = ((enigma::BaseSurface&)surf);

  unsigned char *surfbuf=enigma::graphics_copy_texture_pixels(base.texture,x,y,w,h);

  Sprite& spr = sprites.get(ind);
  spr.AddSubimage(surfbuf, w, h, enigma::ct_precise, surfbuf); //TODO: Support toggling of precise.
  
  delete[] surfbuf;
}

void surface_copy_part(int destination, gs_scalar x, gs_scalar y, int source, int xs, int ys, int ws, int hs)
{
  draw_batch_flush(batch_flush_deferred);

  get_surface(ssurf,source);
  get_surface(dsurf,destination);
  const enigma::BaseSurface& sbase = ((enigma::BaseSurface&)ssurf),
                             dbase = ((enigma::BaseSurface&)dsurf);

  unsigned char *surfbuf=enigma::graphics_copy_texture_pixels(sbase.texture,xs,ys,ws,hs);
  enigma::graphics_push_texture_pixels(dbase.texture, x, y, ws, hs, surfbuf);
  delete[] surfbuf;
}

void surface_copy(int destination, gs_scalar x, gs_scalar y, int source)
{
  draw_batch_flush(batch_flush_deferred);

  get_surface(ssurf,source);
  get_surface(dsurf,destination);
  const enigma::BaseSurface& sbase = ((enigma::BaseSurface&)ssurf),
                             dbase = ((enigma::BaseSurface&)dsurf);
  unsigned sw = 0, sh = 0;

  unsigned char *surfbuf=enigma::graphics_copy_texture_pixels(sbase.texture,&sw,&sh);
  enigma::graphics_push_texture_pixels(dbase.texture, x, y, sw, sh, surfbuf);
  delete[] surfbuf;
}

} // namespace enigma_user
