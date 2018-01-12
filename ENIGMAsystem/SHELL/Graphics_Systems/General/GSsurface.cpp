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

#include <cstddef>
#include <iostream>
#include <math.h>

using namespace std;

#include <stdio.h> //for file writing (surface_save)
#include "Universal_System/nlpo2.h"
#include "Universal_System/sprites_internal.h"
#include "Universal_System/background_internal.h"
#include "Collision_Systems/collision_types.h"
#include "Universal_System/math_consts.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

//Note that this clamps between 0 and 1, not 0 and 255
#define clamp_alpha(alpha) (alpha <= 0 ? 0: alpha >= 1? 1: alpha)

namespace enigma_user {
extern int room_width, room_height/*, sprite_idmax*/;
}

#include "../General/GSprimitives.h"
#include "../General/GSsurface.h"


namespace enigma_user
{

void draw_surface(int id, gs_scalar x, gs_scalar y, int color, gs_scalar alpha)
{
  alpha=clamp_alpha(alpha);
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
    alpha=clamp_alpha(alpha);
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
  alpha=clamp_alpha(alpha);
	draw_primitive_begin_texture(pr_trianglestrip, surface_get_texture(id));
	draw_vertex_texture_color(x,y,0,0,color,alpha);
	draw_vertex_texture_color(x+w,y,1,0,color,alpha);
	draw_vertex_texture_color(x,y+h,0,1,color,alpha);
	draw_vertex_texture_color(x+w,y+h,1,1,color,alpha);
	draw_primitive_end();
}

void draw_surface_stretched_ext(int id, gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, int color, gs_scalar alpha)
{
  alpha=clamp_alpha(alpha);
	draw_primitive_begin_texture(pr_trianglestrip, surface_get_texture(id));
	draw_vertex_texture_color(x,y,0,0,color,alpha);
	draw_vertex_texture_color(x+w,y,1,0,color,alpha);
	draw_vertex_texture_color(x,y+h,0,1,color,alpha);
	draw_vertex_texture_color(x+w,y+h,1,1,color,alpha);
	draw_primitive_end();
}

void draw_surface_part(int id, gs_scalar left, gs_scalar top, gs_scalar w, gs_scalar h, gs_scalar x, gs_scalar y, int color, gs_scalar alpha)
{
  alpha=clamp_alpha(alpha);
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
  alpha=clamp_alpha(alpha);
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
  alpha=clamp_alpha(alpha);
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
  alpha=clamp_alpha(alpha);
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
    alpha=clamp_alpha(alpha);
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
    alpha=clamp_alpha(alpha);
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
    alpha=clamp_alpha(alpha);
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

}
