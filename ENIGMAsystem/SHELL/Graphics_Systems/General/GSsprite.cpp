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
#include "Universal_System/Resources/sprites_internal.h"
#include "Universal_System/Resources/sprites.h"
#include "Universal_System/Instances/instance_system.h"
#include "Universal_System/Object_Tiers/graphics_object.h"
#include "Universal_System/math_consts.h"

#include <cmath>
#include <cstdlib>
#include <string>

using std::string;
using namespace enigma_user;
using enigma::Sprite;
using enigma::sprites;
using enigma::TexRect;

inline int get_usi(const Sprite& spr2d, int subimg) { return subimg >= 0 ? (subimg % spr2d.SubimageCount()) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d.SubimageCount(); }
// VD: see https://yal.cc/2d-pivot-points/ for math
inline gs_scalar rotx(gs_scalar qx, gs_scalar qy, gs_scalar rx, gs_scalar ry) { return ((qx) * rx - (qy) * ry); }
inline gs_scalar roty(gs_scalar qx, gs_scalar qy, gs_scalar rx, gs_scalar ry) { return ((qx) * ry + (qy) * rx); }

// implementation:
namespace enigma
{

void draw_sprite_pos_raw(const Sprite& spr2d, int subimg, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar color, gs_scalar alpha)
{
  alpha = CLAMP_ALPHAF(alpha);
  int usi = get_usi(spr2d, subimg);

  const TexRect& texRect = spr2d.GetTextureRect(usi); 
  
  gs_scalar
    tx = texRect.x, tw = texRect.w,
    ty = texRect.y, th = texRect.h;

  draw_primitive_begin_texture(pr_trianglestrip, spr2d.GetTexture(usi));
  draw_vertex_texture_color(x1,y1, tx,    ty,    color,alpha);
  draw_vertex_texture_color(x2,y2, tx+tw, ty,    color,alpha);
  draw_vertex_texture_color(x4,y4, tx,    ty+th, color,alpha);
  draw_vertex_texture_color(x3,y3, tx+tw, ty+th, color,alpha);
  draw_primitive_end();
}

void draw_sprite_pos_part_raw(const Sprite& spr2d, int subimg,
  gs_scalar px, gs_scalar py, gs_scalar pw, gs_scalar ph,
  gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4,
  gs_scalar color, gs_scalar alpha
) {
  alpha = CLAMP_ALPHAF(alpha);
  int usi = get_usi(spr2d, subimg);

  const TexRect& texRect = spr2d.GetTextureRect(usi); 
  
  gs_scalar
    tbx = texRect.x, tbw = (gs_scalar)spr2d.width  / (gs_scalar)texRect.w,
    tby = texRect.y, tbh = (gs_scalar)spr2d.height / (gs_scalar)texRect.h,
    tx1 = tbx + px / tbw, tx2 = tx1 + pw / tbw,
    ty1 = tby + py / tbh, ty2 = ty1 + ph / tbh;

  draw_primitive_begin_texture(pr_trianglestrip, spr2d.GetTexture(usi));
  draw_vertex_texture_color(x1,y1, tx1,ty1, color,alpha);
  draw_vertex_texture_color(x2,y2, tx2,ty1, color,alpha);
  draw_vertex_texture_color(x4,y4, tx1,ty2, color,alpha);
  draw_vertex_texture_color(x3,y3, tx2,ty2, color,alpha);
  draw_primitive_end();
}

}

namespace enigma_user {

// standard (see GSsprite.h for default parameter rvalues):
void draw_sprite_pos(int spr, int subimg, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar alpha)
{
  // VD: points are ordered TL, TR, BR, BL
  const Sprite& spr2d = sprites.get(spr);
  enigma::draw_sprite_pos_raw(spr2d, subimg, x1, y1, x2, y2, x3, y3, x4, y4, 0xFFFFFF, alpha);
}

void draw_sprite(int spr,int subimg, gs_scalar x, gs_scalar y, int color, gs_scalar alpha)
{
  const Sprite& spr2d = sprites.get(spr);
  gs_scalar
    x1 = x - spr2d.xoffset, x2 = x1 + spr2d.width,
    y1 = y - spr2d.yoffset, y2 = y1 + spr2d.height;
  enigma::draw_sprite_pos_raw(spr2d,subimg, x1,y1, x2,y1, x2,y2, x1,y2, color, alpha);
}

void draw_sprite_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int color, gs_scalar alpha)
{
  const Sprite& spr2d = sprites.get(spr);
  rot *= M_PI / -180.0;
  gs_scalar
    rx = cos(rot), ry = sin(rot),
    x1 = -xscale * spr2d.xoffset, x2 = x1 + xscale * spr2d.width,
    y1 = -yscale * spr2d.yoffset, y2 = y1 + yscale * spr2d.height;
  enigma::draw_sprite_pos_raw(spr2d,subimg,
    x + rotx(x1, y1, rx, ry), y + roty(x1, y1, rx, ry),
    x + rotx(x2, y1, rx, ry), y + roty(x2, y1, rx, ry),
    x + rotx(x2, y2, rx, ry), y + roty(x2, y2, rx, ry),
    x + rotx(x1, y2, rx, ry), y + roty(x1, y2, rx, ry),
    color, alpha
  );
}

void draw_sprite_part(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, int color, gs_scalar alpha)
{
  const Sprite& spr2d = sprites.get(spr);
  enigma::draw_sprite_pos_part_raw(spr2d,subimg, left,top,width,height, x,y, x+width,y, x+width,y+height, x,y+height, color,alpha);
}

void draw_sprite_part_ext(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
  const Sprite& spr2d = sprites.get(spr);
  gs_scalar x2 = x + xscale * width, y2 = y + yscale * height;
  enigma::draw_sprite_pos_part_raw(spr2d,subimg, left,top,width,height, x,y, x2,y, x2,y2, x,y2, color,alpha);
}

void draw_sprite_general(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar alpha)
{
  const Sprite& spr2d = sprites.get(spr);
  alpha = CLAMP_ALPHAF(alpha);
  int usi = get_usi(spr2d, subimg);
  rot *= M_PI / -180.0;
  
  const TexRect& texRect = spr2d.GetTextureRect(usi);
  
  gs_scalar
    rx = cos(rot), ry = sin(rot),
    x1 = 0, x2 = x1 + xscale * width,
    y1 = 0, y2 = y1 + yscale * height,
    tbx = texRect.x, tbw = (gs_scalar)spr2d.width  / (gs_scalar)texRect.w,
    tby = texRect.y, tbh = (gs_scalar)spr2d.height / (gs_scalar)texRect.h,
    tx1 = tbx + left / tbw, tx2 = tx1 + width / tbw,
    ty1 = tby + top / tbh, ty2 = ty1 + height / tbh;
  // VD: EGM's color blending is for some reason softer and I can't figure out why
  draw_primitive_begin_texture(pr_trianglestrip, spr2d.GetTexture(usi));
  draw_vertex_texture_color(x + rotx(x1, y1, rx, ry), y + roty(x1, y1, rx, ry), tx1,ty1, c1,alpha);
  draw_vertex_texture_color(x + rotx(x2, y1, rx, ry), y + roty(x2, y1, rx, ry), tx2,ty1, c2,alpha);
  draw_vertex_texture_color(x + rotx(x1, y2, rx, ry), y + roty(x1, y2, rx, ry), tx1,ty2, c4,alpha);
  draw_vertex_texture_color(x + rotx(x2, y2, rx, ry), y + roty(x2, y2, rx, ry), tx2,ty2, c3,alpha);
  draw_primitive_end();
}

void draw_sprite_stretched(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, int color, gs_scalar alpha)
{
  const Sprite& spr2d = sprites.get(spr);
  gs_scalar x2 = x + width, y2 = y + height;
  enigma::draw_sprite_pos_raw(spr2d,subimg, x,y, x2,y, x2,y2, x,y2, color, alpha);
}

void draw_sprite_stretched_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, int color, gs_scalar alpha)
{
  draw_sprite_stretched(spr, subimg, x, y, width, height, color, alpha);
}

// non-standard:
void draw_sprite_part_offset(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, int color, gs_scalar alpha)
{
  alpha=CLAMP_ALPHAF(alpha);
  const Sprite& spr2d = sprites.get(spr);
  int usi = get_usi(spr2d, subimg);
  
  const TexRect& texRect = spr2d.GetTextureRect(usi);

  const gs_scalar tbw = spr2d.width/texRect.w, tbh = spr2d.height/texRect.h,
    xvert1 = x-spr2d.xoffset, xvert2 = xvert1 + spr2d.width,
    yvert1 = y-spr2d.yoffset, yvert2 = yvert1 + spr2d.height,
    tbx1 = texRect.x+left/tbw, tbx2 = texRect.x+tbx1 + width/tbw,
    tby1 = texRect.y+top/tbh,  tby2 = texRect.y+tby1 + height/tbh;

  draw_primitive_begin_texture(pr_trianglestrip, spr2d.GetTexture(usi));
  draw_vertex_texture_color(xvert1,yvert1,tbx1,tby1,color,alpha);
  draw_vertex_texture_color(xvert2,yvert1,tbx2,tby1,color,alpha);
  draw_vertex_texture_color(xvert1,yvert2,tbx1,tby2,color,alpha);
  draw_vertex_texture_color(xvert2,yvert2,tbx2,tby2,color,alpha);
  draw_primitive_end();
}

void d3d_draw_sprite(int spr,int subimg, gs_scalar x, gs_scalar y, gs_scalar z)
{
  const Sprite& spr2d = sprites.get(spr);
  int usi = get_usi(spr2d, subimg);

  const TexRect& texRect = spr2d.GetTextureRect(usi);

  const gs_scalar tbx = texRect.x, tby = texRect.y,
      tbw = texRect.w, tbh = texRect.h,
      xvert1 = x-spr2d.xoffset, xvert2 = xvert1 + spr2d.width,
      yvert1 = y-spr2d.yoffset, yvert2 = yvert1 + spr2d.height;

  draw_primitive_begin_texture(pr_trianglestrip, spr2d.GetTexture(usi));
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
  const Sprite& spr2d = sprites.get(spr);
  int usi = get_usi(spr2d, subimg);

  //Order x1,y1,x2,y2 correctly
  if (x1>x2) {gs_scalar tx = x2; x2 = x1, x1 = tx;}
  if (y1>y2) {gs_scalar ty = y2; y2 = y1, y1 = ty;}

  //Minimum w = left+right and minimum h = top+bottom
  gs_scalar w = x2-x1, h = y2-y1;
  if (w<left+right) x2 = x1+left+right, w = x2-x1;
  if (h<top+bottom) y2 = y1+top+bottom, h = y2-y1;

  const TexRect& texRect = spr2d.GetTextureRect(usi);

  const gs_scalar midw = w-left-right, midh = h-top-bottom;
  const gs_scalar midtw = spr2d.width-left-right, midth = spr2d.height-bottom-top;
  const gs_scalar tbx = texRect.x, tby = texRect.y;
  const gs_scalar tbw = spr2d.width/(gs_scalar)texRect.w, tbh = spr2d.height/(gs_scalar)texRect.h;
  const gs_scalar tbl = left/tbw, tbt = top/tbh, tbr = right/tbw, tbb = bottom/tbh, tbmw = midtw/tbw, tbmh = midth/tbh;

  //Draw top-left corner
  gs_scalar xvert1 = x1, xvert2 = xvert1 + left,
            yvert1 = y1, yvert2 = yvert1 + top;

  draw_primitive_begin_texture(pr_trianglestrip, spr2d.GetTexture(usi));
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

  draw_primitive_begin_texture(pr_trianglestrip, spr2d.GetTexture(usi));
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

  draw_primitive_begin_texture(pr_trianglestrip, spr2d.GetTexture(usi));
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
  const Sprite& spr2d = sprites.get(spr);
  int usi = get_usi(spr2d, subimg);

  x = ((spr2d.xoffset+x)<0?0:spr2d.width)-fmod(spr2d.xoffset+x,spr2d.width);
  y = ((spr2d.yoffset+y)<0?0:spr2d.height)-fmod(spr2d.yoffset+y,spr2d.height);
  
  const TexRect& texRect = spr2d.GetTextureRect(usi);
  
  gs_scalar
    tx = texRect.x, tw = texRect.w,
    ty = texRect.y, th = texRect.h;

  const int
  hortil = int(ceil((view_enabled ? (gs_scalar)(view_xview[view_current] + view_wview[view_current]) : (gs_scalar)room_width) / ((gs_scalar)spr2d.width))) + 1,
  vertil = int(ceil((view_enabled ? (gs_scalar)(view_yview[view_current] + view_hview[view_current]) : (gs_scalar)room_height) / ((gs_scalar)spr2d.height))) + 1;

  gs_scalar xvert1 = -x, xvert2 = xvert1 + spr2d.width, yvert1, yvert2;
  for (int i=0; i<hortil; ++i)
  {
    yvert1 = -y; yvert2 = yvert1 + spr2d.height;
    for (int c=0; c<vertil; ++c)
    {
      draw_primitive_begin_texture(pr_trianglestrip, spr2d.GetTexture(usi));
      draw_vertex_texture_color(xvert1,yvert1,tx,ty,color,alpha);
      draw_vertex_texture_color(xvert2,yvert1,tx+tw,ty,color,alpha);
      draw_vertex_texture_color(xvert1,yvert2,tx,ty+th,color,alpha);
      draw_vertex_texture_color(xvert2,yvert2,tx+tw,ty+th,color,alpha);
      draw_primitive_end();
      yvert1 = yvert2;
      yvert2 += spr2d.height;
    }
    xvert1 = xvert2;
    xvert2 += spr2d.width;
  }
}

void draw_sprite_tiled_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
    alpha=CLAMP_ALPHAF(alpha);
    const Sprite& spr2d = sprites.get(spr);
    int usi = get_usi(spr2d, subimg);

  const TexRect& texRect = spr2d.GetTextureRect(usi);
  
  const gs_scalar
    tx = texRect.x, tw = texRect.w,
    ty = texRect.y, th = texRect.h,
    width_scaled = spr2d.width*xscale, height_scaled = spr2d.height*yscale;

  x = ((spr2d.xoffset*xscale+x)<0?0:width_scaled)-fmod(spr2d.xoffset*xscale+x,width_scaled);
  y = ((spr2d.yoffset*yscale+y)<0?0:height_scaled)-fmod(spr2d.yoffset*yscale+y,height_scaled);

  const int
  hortil = int(ceil((view_enabled ? (gs_scalar)(view_xview[view_current] + view_wview[view_current]) : (gs_scalar)room_width) / (width_scaled))) + 1,
  vertil = int(ceil((view_enabled ? (gs_scalar)(view_yview[view_current] + view_hview[view_current]) : (gs_scalar)room_height) / (height_scaled))) + 1;

  gs_scalar xvert1 = -x, xvert2 = xvert1 + width_scaled, yvert1, yvert2;
  for (int i=0; i<hortil; ++i)
  {
    yvert1 = -y; yvert2 = yvert1 + height_scaled;
    for (int c=0; c<vertil; ++c)
    {
      draw_primitive_begin_texture(pr_trianglestrip, spr2d.GetTexture(usi));
      draw_vertex_texture_color(xvert1,yvert1,tx,ty,color,alpha);
      draw_vertex_texture_color(xvert2,yvert1,tx+tw,ty,color,alpha);
      draw_vertex_texture_color(xvert1,yvert2,tx,ty+th,color,alpha);
      draw_vertex_texture_color(xvert2,yvert2,tx+tw,ty+th,color,alpha);
      draw_primitive_end();
      yvert1 = yvert2;
      yvert2 += height_scaled;
    }
    xvert1 = xvert2;
    xvert2 += width_scaled;
  }
}

}
