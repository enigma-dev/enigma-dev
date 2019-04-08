/** Copyright (C) 2013 Robert B. Colton, 2013-2015 Harijs Grinbergs
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
#include "GScolor_macros.h"
#include "GSsprite.h"
#include "GStextures.h"
#include "GSprimitives.h"

#include "Universal_System/nlpo2.h"
#include "Universal_System/sprites_internal.h"
#include "Universal_System/sprites.h"
#include "Universal_System/instance_system.h"
#include "Universal_System/graphics_object.h"
#include "Universal_System/math_consts.h"

#include <cmath>
#include <cstdlib>
#include <string>
using std::string;

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

namespace enigma_user
{

void draw_sprite(int spr,int subimg, gs_scalar x, gs_scalar y, int color, gs_scalar alpha)
{
    alpha=CLAMP_ALPHAF(alpha);
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

  const gs_scalar tbx = spr2d->texturexarray[usi], tby = spr2d->textureyarray[usi],
    tbw = spr2d->texturewarray[usi], tbh = spr2d->textureharray[usi],
    xvert1 = x-spr2d->xoffset, xvert2 = xvert1 + spr2d->width,
    yvert1 = y-spr2d->yoffset, yvert2 = yvert1 + spr2d->height;

  draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
  draw_vertex_texture_color(xvert1,yvert1,tbx,tby,color,alpha);
  draw_vertex_texture_color(xvert2,yvert1,tbx+tbw,tby,color,alpha);
  draw_vertex_texture_color(xvert1,yvert2,tbx,tby+tbh,color,alpha);
  draw_vertex_texture_color(xvert2,yvert2,tbx+tbw,tby+tbh,color,alpha);
  draw_primitive_end();
}

void draw_sprite_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int color, gs_scalar alpha)
{
    alpha=CLAMP_ALPHAF(alpha);
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

  rot *= M_PI/180.0;

  const gs_scalar
  w = spr2d->width*xscale, h = spr2d->height*yscale,
  tbx = spr2d->texturexarray[usi], tby = spr2d->textureyarray[usi],
  tbw = spr2d->texturewarray[usi], tbh = spr2d->textureharray[usi],
  wsinrot = w*sin(rot), wcosrot = w*cos(rot);

  draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
  gs_scalar
    ulcx = x - xscale * spr2d->xoffset * cos(rot) + yscale * spr2d->yoffset * cos(M_PI/2+rot),
    ulcy = y + xscale * spr2d->xoffset * sin(rot) - yscale * spr2d->yoffset * sin(M_PI/2+rot);
  draw_vertex_texture_color(ulcx,ulcy, tbx,tby, color, alpha);
  draw_vertex_texture_color(ulcx + wcosrot, ulcy - wsinrot, tbx+tbw, tby, color, alpha);
  const double mpr = 3*M_PI/2 + rot;
    ulcx += h * cos(mpr);
    ulcy -= h * sin(mpr);
  draw_vertex_texture_color(ulcx,ulcy, tbx,tby+tbh, color, alpha);
  draw_vertex_texture_color(ulcx + wcosrot, ulcy - wsinrot, tbx+tbw,tby+tbh, color, alpha);
  draw_primitive_end();
}

///NOTE(harijs) - This function fits in as a priest in candy shop. It's the only sprite drawing function that uses bound draw color and we cannot add a color argument before the alpha, because YYG had it this way
///Maybe we should forget about GM compatibility once again and just modify this so it fits the others
void draw_sprite_pos(int spr, int subimg, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar alpha)
{
    alpha=CLAMP_ALPHAF(alpha);
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

  const gs_scalar tbx = spr2d->texturexarray[usi], tby = spr2d->textureyarray[usi],
  tbw = spr2d->texturewarray[usi], tbh = spr2d->textureharray[usi];

  draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
  draw_vertex_texture_color(x1,y1,tbx,tby,draw_get_color(),alpha);
  draw_vertex_texture_color(x2,y1,tbx+tbw,tby,draw_get_color(),alpha);
  draw_vertex_texture_color(x1,y2,tbx,tby+tbh,draw_get_color(),alpha);
  draw_vertex_texture_color(x2,y2,tbx+tbw,tby+tbh,draw_get_color(),alpha);
  draw_primitive_end();
}

void draw_sprite_part(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, int color, gs_scalar alpha)
{
    alpha=CLAMP_ALPHAF(alpha);
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

  const gs_scalar tbw = spr2d->width/(gs_scalar)spr2d->texturewarray[usi], tbh = spr2d->height/(gs_scalar)spr2d->textureharray[usi],
    tbx1 = spr2d->texturexarray[usi]+left/tbw, tbx2 = spr2d->texturexarray[usi]+tbx1 + width/tbw,
    tby1 = spr2d->textureyarray[usi]+top/tbh, tby2 = spr2d->textureyarray[usi]+tby1 + height/tbh;

  draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
  draw_vertex_texture_color(x,y,tbx1,tby1,color,alpha);
  draw_vertex_texture_color(x+width,y,tbx2,tby1,color,alpha);
  draw_vertex_texture_color(x,y+height,tbx1,tby2,color,alpha);
  draw_vertex_texture_color(x+width,y+height,tbx2,tby2,color,alpha);
  draw_primitive_end();
}

void draw_sprite_part_offset(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, int color, gs_scalar alpha)
{
    alpha=CLAMP_ALPHAF(alpha);
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

  const gs_scalar tbw = spr2d->width/spr2d->texturewarray[usi], tbh = spr2d->height/spr2d->textureharray[usi],
    xvert1 = x-spr2d->xoffset, xvert2 = xvert1 + spr2d->width,
    yvert1 = y-spr2d->yoffset, yvert2 = yvert1 + spr2d->height,
    tbx1 = spr2d->texturexarray[usi]+left/tbw, tbx2 = spr2d->texturexarray[usi]+tbx1 + width/tbw,
    tby1 = spr2d->textureyarray[usi]+top/tbh, tby2 = spr2d->textureyarray[usi]+tby1 + height/tbh;

  draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
  draw_vertex_texture_color(xvert1,yvert1,tbx1,tby1,color,alpha);
  draw_vertex_texture_color(xvert2,yvert1,tbx2,tby1,color,alpha);
  draw_vertex_texture_color(xvert1,yvert2,tbx1,tby2,color,alpha);
  draw_vertex_texture_color(xvert2,yvert2,tbx2,tby2,color,alpha);
  draw_primitive_end();
}

void draw_sprite_part_ext(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
    alpha=CLAMP_ALPHAF(alpha);
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

  const gs_scalar tbw = spr2d->width/(gs_scalar)spr2d->texturewarray[usi], tbh = spr2d->height/(gs_scalar)spr2d->textureharray[usi],
    xvert1 = x, xvert2 = xvert1 + width*xscale,
    yvert1 = y, yvert2 = yvert1 + height*yscale,
    tbx1 = spr2d->texturexarray[usi]+left/tbw, tbx2 = spr2d->texturexarray[usi]+tbx1 + width/tbw,
    tby1 = spr2d->textureyarray[usi]+top/tbh, tby2 = spr2d->textureyarray[usi]+tby1 + height/tbh;

  draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
  draw_vertex_texture_color(xvert1,yvert1,tbx1,tby1,color,alpha);
  draw_vertex_texture_color(xvert2,yvert1,tbx2,tby1,color,alpha);
  draw_vertex_texture_color(xvert1,yvert2,tbx1,tby2,color,alpha);
  draw_vertex_texture_color(xvert2,yvert2,tbx2,tby2,color,alpha);
  draw_primitive_end();
}

void draw_sprite_general(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar alpha)
{
    alpha=CLAMP_ALPHAF(alpha);
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

  const gs_scalar
    tbx = spr2d->texturexarray[usi],  tby = spr2d->textureyarray[usi],
    tbw = spr2d->width/spr2d->texturewarray[usi], tbh = spr2d->height/spr2d->textureharray[usi],
    w = width*xscale, h = height*yscale;

  rot *= M_PI/180;
  const gs_scalar wcosrot = w*cos(rot), wsinrot = w*sin(rot);

  draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);

  gs_scalar
    ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
    ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);

  draw_vertex_texture_color(ulcx, ulcy, tbx+left/tbw, tby+top/tbh, c1, alpha);
  draw_vertex_texture_color((ulcx + wcosrot), (ulcy - wsinrot), tbx+(left+width)/tbw, tby+top/tbh, c2, alpha);

  ulcx += h * cos(3*M_PI/2 + rot);
  ulcy -= h * sin(3*M_PI/2 + rot);

  draw_vertex_texture_color((ulcx + wcosrot), (ulcy - wsinrot), tbx+(left+width)/tbw, tby+(top+height)/tbh, c4, alpha);
  draw_vertex_texture_color(ulcx, ulcy, tbx+left/tbw, tby+(top+height)/tbh, c3, alpha);

  draw_primitive_end();
}

void draw_sprite_stretched(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, int color, gs_scalar alpha)
{
    alpha=CLAMP_ALPHAF(alpha);
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

  const gs_scalar tbx = spr2d->texturexarray[usi], tby = spr2d->textureyarray[usi],
              tbw = spr2d->texturewarray[usi], tbh = spr2d->textureharray[usi],
              xvert1 = x, xvert2 = xvert1 + width,
              yvert1 = y, yvert2 = yvert1 + height;

  draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
  draw_vertex_texture_color(xvert1,yvert1,tbx,tby,color,alpha);
  draw_vertex_texture_color(xvert2,yvert1,tbx+tbw,tby,color,alpha);
  draw_vertex_texture_color(xvert1,yvert2,tbx,tby+tbh,color,alpha);
  draw_vertex_texture_color(xvert2,yvert2,tbx+tbw,tby+tbh,color,alpha);
  draw_primitive_end();
}

void draw_sprite_stretched_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, int color, gs_scalar alpha)
{
  draw_sprite_stretched(spr, subimg, x, y, width, height, color, alpha);
}

void d3d_draw_sprite(int spr,int subimg, gs_scalar x, gs_scalar y, gs_scalar z)
{
  get_spritev(spr2d,spr);
  const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

  const gs_scalar tbx = spr2d->texturexarray[usi], tby = spr2d->textureyarray[usi],
      tbw = spr2d->texturewarray[usi], tbh = spr2d->textureharray[usi],
      xvert1 = x-spr2d->xoffset, xvert2 = xvert1 + spr2d->width,
      yvert1 = y-spr2d->yoffset, yvert2 = yvert1 + spr2d->height;

  draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
  d3d_vertex_texture(xvert1,yvert1,z,tbx,tby);
  d3d_vertex_texture(xvert2,yvert1,z,tbx+tbw,tby);
  d3d_vertex_texture(xvert1,yvert2,z,tbx,tby+tbh);
  d3d_vertex_texture(xvert2,yvert2,z,tbx+tbw,tby+tbh);
  draw_primitive_end();
}

//Draw padded
void draw_sprite_padded(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, int color, gs_scalar alpha)
{
  alpha=CLAMP_ALPHAF(alpha);
  get_spritev(spr2d,spr);
  const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

  //Order x1,y1,x2,y2 correctly
  if (x1>x2) {gs_scalar tx = x2; x2 = x1, x1 = tx;}
  if (y1>y2) {gs_scalar ty = y2; y2 = y1, y1 = ty;}

  //Minimum w = left+right and minimum h = top+bottom
  gs_scalar w = x2-x1, h = y2-y1;
  if (w<left+right) x2 = x1+left+right, w = x2-x1;
  if (h<top+bottom) y2 = y1+top+bottom, h = y2-y1;

  const gs_scalar midw = w-left-right, midh = h-top-bottom;
  const gs_scalar midtw = spr2d->width-left-right, midth = spr2d->height-bottom-top;
  const gs_scalar tbx = spr2d->texturexarray[usi], tby = spr2d->textureyarray[usi];
  const gs_scalar tbw = spr2d->width/(gs_scalar)spr2d->texturewarray[usi], tbh = spr2d->height/(gs_scalar)spr2d->textureharray[usi];
  const gs_scalar tbl = left/tbw, tbt = top/tbh, tbr = right/tbw, tbb = bottom/tbh, tbmw = midtw/tbw, tbmh = midth/tbh;

  //Draw top-left corner
  gs_scalar xvert1 = x1, xvert2 = xvert1 + left,
            yvert1 = y1, yvert2 = yvert1 + top;

  draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
  draw_vertex_texture_color(xvert1,yvert1,tbx,tby,color,alpha);
  draw_vertex_texture_color(xvert2,yvert1,tbx+tbl,tby,color,alpha);
  draw_vertex_texture_color(xvert1,yvert2,tbx,tby+tbt,color,alpha);
  draw_vertex_texture_color(xvert2,yvert2,tbx+tbl,tby+tbt,color,alpha);

  //Draw left side
  xvert1 = x1, xvert2 = xvert1 + left,
  yvert1 = y1 + top, yvert2 = yvert1 + midh;

  draw_vertex_texture_color(xvert1,yvert1,tbx,tby+tbt,color,alpha);
  draw_vertex_texture_color(xvert2,yvert1,tbx+tbl,tby+tbt,color,alpha);
  draw_vertex_texture_color(xvert1,yvert2,tbx,tby+tbt+tbmh,color,alpha);
  draw_vertex_texture_color(xvert2,yvert2,tbx+tbl,tby+tbt+tbmh,color,alpha);

  //Draw bottom-left corner
  xvert1 = x1, xvert2 = xvert1 + left,
  yvert1 = y1 + top + midh, yvert2 = yvert1 + bottom;

  draw_vertex_texture_color(xvert1,yvert1,tbx,tby+tbt+tbmh,color,alpha);
  draw_vertex_texture_color(xvert2,yvert1,tbx+tbl,tby+tbt+tbmh,color,alpha);
  draw_vertex_texture_color(xvert1,yvert2,tbx,tby+tbt+tbmh+tbb,color,alpha);
  draw_vertex_texture_color(xvert2,yvert2,tbx+tbl,tby+tbt+tbmh+tbb,color,alpha);
  draw_primitive_end();

  //Draw top
  xvert1 = x1 + left, xvert2 = xvert1 + midw,
  yvert1 = y1, yvert2 = yvert1 + top;

  draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
  draw_vertex_texture_color(xvert1,yvert1,tbx+tbl,tby,color,alpha);
  draw_vertex_texture_color(xvert2,yvert1,tbx+tbl+tbmw,tby,color,alpha);
  draw_vertex_texture_color(xvert1,yvert2,tbx+tbl,tby+tbt,color,alpha);
  draw_vertex_texture_color(xvert2,yvert2,tbx+tbl+tbmw,tby+tbt,color,alpha);

  //Draw middle
  xvert1 = x1 + left, xvert2 = xvert1 + midw,
  yvert1 = y1 + top, yvert2 = yvert1 + midh;

  draw_vertex_texture_color(xvert1,yvert1,tbx+tbl,tby+tbt,color,alpha);
  draw_vertex_texture_color(xvert2,yvert1,tbx+tbl+tbmw,tby+tbt,color,alpha);
  draw_vertex_texture_color(xvert1,yvert2,tbx+tbl,tby+tbt+tbmh,color,alpha);
  draw_vertex_texture_color(xvert2,yvert2,tbx+tbl+tbmw,tby+tbt+tbmh,color,alpha);

  //Draw bottom
  xvert1 = x1 + left, xvert2 = xvert1 + midw,
  yvert1 = y1 + midh + top, yvert2 = yvert1 + bottom;

  draw_vertex_texture_color(xvert1,yvert1,tbx+tbl,tby+tbt+tbmh,color,alpha);
  draw_vertex_texture_color(xvert2,yvert1,tbx+tbl+tbmw,tby+tbt+tbmh,color,alpha);
  draw_vertex_texture_color(xvert1,yvert2,tbx+tbl,tby+tbt+tbmh+tbb,color,alpha);
  draw_vertex_texture_color(xvert2,yvert2,tbx+tbl+tbmw,tby+tbt+tbmh+tbb,color,alpha);
  draw_primitive_end();

  //Draw top-right corner
  xvert1 = x1 + midw + left, xvert2 = xvert1 + right,
  yvert1 = y1, yvert2 = yvert1 + top;

  draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
  draw_vertex_texture_color(xvert1,yvert1,tbx+tbl+tbmw,tby,color,alpha);
  draw_vertex_texture_color(xvert2,yvert1,tbx+tbl+tbmw+tbr,tby,color,alpha);
  draw_vertex_texture_color(xvert1,yvert2,tbx+tbl+tbmw,tby+tbt,color,alpha);
  draw_vertex_texture_color(xvert2,yvert2,tbx+tbl+tbmw+tbr,tby+tbt,color,alpha);

  //Draw right side
  xvert1 = x1 + midw + left, xvert2 = xvert1 + right,
  yvert1 = y1 + top, yvert2 = yvert1 + midh;

  draw_vertex_texture_color(xvert1,yvert1,tbx+tbl+tbmw,tby+tbt,color,alpha);
  draw_vertex_texture_color(xvert2,yvert1,tbx+tbl+tbmw+tbr,tby+tbt,color,alpha);
  draw_vertex_texture_color(xvert1,yvert2,tbx+tbl+tbmw,tby+tbt+tbmh,color,alpha);
  draw_vertex_texture_color(xvert2,yvert2,tbx+tbl+tbmw+tbr,tby+tbt+tbmh,color,alpha);

  //Draw bottom-right corner
  xvert1 = x1 + midw + left, xvert2 = xvert1 + right,
  yvert1 = y1 + top + midh, yvert2 = yvert1 + bottom;

  draw_vertex_texture_color(xvert1,yvert1,tbx+tbl+tbmw,tby+tbt+tbmh,color,alpha);
  draw_vertex_texture_color(xvert2,yvert1,tbx+tbl+tbmw+tbr,tby+tbt+tbmh,color,alpha);
  draw_vertex_texture_color(xvert1,yvert2,tbx+tbl+tbmw,tby+tbt+tbmh+tbb,color,alpha);
  draw_vertex_texture_color(xvert2,yvert2,tbx+tbl+tbmw+tbr,tby+tbt+tbmh+tbb,color,alpha);
  draw_primitive_end();
}

}

// These two leave a bad taste in my mouth because they depend on views, which should be removable.
// However, for now, they stay. - Josh

#include <string>
using std::string;
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"

namespace enigma_user
{

void draw_sprite_tiled(int spr, int subimg, gs_scalar x, gs_scalar y, int color, gs_scalar alpha)
{
    alpha=CLAMP_ALPHAF(alpha);
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

  x = ((spr2d->xoffset+x)<0?0:spr2d->width)-fmod(spr2d->xoffset+x,spr2d->width);
  y = ((spr2d->yoffset+y)<0?0:spr2d->height)-fmod(spr2d->yoffset+y,spr2d->height);

  const gs_scalar tbx  = spr2d->texturexarray[usi], tby  = spr2d->textureyarray[usi],
                  tbw  = spr2d->texturewarray[usi], tbh  = spr2d->textureharray[usi];

  const int
  hortil = int(ceil((view_enabled ? (gs_scalar)(view_xview[view_current] + view_wview[view_current]) : (gs_scalar)room_width) / ((gs_scalar)spr2d->width))) + 1,
  vertil = int(ceil((view_enabled ? (gs_scalar)(view_yview[view_current] + view_hview[view_current]) : (gs_scalar)room_height) / ((gs_scalar)spr2d->height))) + 1;

  gs_scalar xvert1 = -x, xvert2 = xvert1 + spr2d->width, yvert1, yvert2;
  for (int i=0; i<hortil; ++i)
  {
    yvert1 = -y; yvert2 = yvert1 + spr2d->height;
    for (int c=0; c<vertil; ++c)
    {
      draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
      draw_vertex_texture_color(xvert1,yvert1,tbx,tby,color,alpha);
      draw_vertex_texture_color(xvert2,yvert1,tbx+tbw,tby,color,alpha);
      draw_vertex_texture_color(xvert1,yvert2,tbx,tby+tbh,color,alpha);
      draw_vertex_texture_color(xvert2,yvert2,tbx+tbw,tby+tbh,color,alpha);
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
    alpha=CLAMP_ALPHAF(alpha);
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

  const gs_scalar
  tbx  = spr2d->texturexarray[usi], tby  = spr2d->textureyarray[usi],
  tbw  = spr2d->texturewarray[usi], tbh  = spr2d->textureharray[usi],
  width_scaled = spr2d->width*xscale, height_scaled = spr2d->height*yscale;

  x = ((spr2d->xoffset*xscale+x)<0?0:width_scaled)-fmod(spr2d->xoffset*xscale+x,width_scaled);
  y = ((spr2d->yoffset*yscale+y)<0?0:height_scaled)-fmod(spr2d->yoffset*yscale+y,height_scaled);

  const int
  hortil = int(ceil((view_enabled ? (gs_scalar)(view_xview[view_current] + view_wview[view_current]) : (gs_scalar)room_width) / (width_scaled))) + 1,
  vertil = int(ceil((view_enabled ? (gs_scalar)(view_yview[view_current] + view_hview[view_current]) : (gs_scalar)room_height) / (height_scaled))) + 1;

  gs_scalar xvert1 = -x, xvert2 = xvert1 + width_scaled, yvert1, yvert2;
  for (int i=0; i<hortil; ++i)
  {
    yvert1 = -y; yvert2 = yvert1 + height_scaled;
    for (int c=0; c<vertil; ++c)
    {
      draw_primitive_begin_texture(pr_trianglestrip, spr2d->texturearray[usi]);
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

}
