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

#include <math.h>
#include <string>
#include "Universal_System/var4.h"
#include "libEGMstd.h"
#include "../General/GScolors.h"
#include "../General/GSfont.h"
#include "../General/GStextures.h"
#include "../General/GSprimitives.h"
#include "../General/GSsprite.h"


using namespace std;
#include "Universal_System/fontstruct.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

namespace enigma {
  static int currentfont = -1;
  extern size_t font_idmax;
}

using namespace enigma;
/*const int fa_left = 0;
const int fa_center = 1;
const int fa_right = 2;
const int fa_top = 0;
const int fa_middle = 1;
const int fa_bottom = 2;*/

unsigned halign = enigma_user::fa_left; //default alignment
unsigned valign = enigma_user::fa_top; //default alignment

namespace enigma_user
{

void draw_set_halign(unsigned align){
    halign = align;
}
void draw_set_valign(unsigned align){
    valign = align;
}

unsigned draw_get_halign(){
    return halign;
}
unsigned draw_get_valign(){
    return valign;
}

}

#ifdef DEBUG_MODE
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_font(fnt,id,r) \
    if (id < -1 or (size_t(id+1) > enigma::font_idmax) or !fontstructarray[id]) { \
      show_error("Cannot access font with id " + toString(id), false); \
      return r; \
    } const font *const fnt = fontstructarray[id];
  #define get_fontv(fnt,id) \
    if (id < -1 or (size_t(id+1) > enigma::font_idmax) or !fontstructarray[id]) { \
      show_error("Cannot access font with id " + toString(id), false); \
      return; \
    } const font *const fnt = fontstructarray[id];
  #define get_font_null(fnt,id,r) \
    if (id < -1 or (size_t(id+1) > enigma::font_idmax)) { \
      show_error("Cannot access font with id " + toString(id), false); \
      return r; \
    } const font *const fnt = fontstructarray[id];
#else
  #define get_font(fnt,id,r) \
    const font *const fnt = fontstructarray[id];
  #define get_fontv(fnt,id) \
    const font *const fnt = fontstructarray[id];
  #define get_font_null(fnt,id,r) \
    const font *const fnt = fontstructarray[id];
#endif

namespace enigma
{
  inline int get_space_width(const font *const fnt)
  {
    fontglyph &g = fnt->glyphs[(unsigned char)(' ' - fnt->glyphstart) % fnt->glyphcount];
    // Use the width of the space glyph when available,
    // else use the backup.
    // FIXME: Find out why the width is not available on Linux.
    return g.xs > 1 ? g.xs : fnt->height/3;
  }
}

///////////////////////////////////////////////////

namespace enigma_user
{

unsigned int string_width_line(variant vstr, int line)
{
  string str = toString(vstr);
  get_font(fnt,currentfont,0);
  int len = 0, cl = 0;
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == '\r'){
      if (cl == line)
        return len;
      cl += 1;
      len = 0;
      i += str[i+1] == '\n';
    } else if (str[i] == '\n'){
      if (cl == line)
        return len;
      cl += 1;
      len = 0;
    } else if (str[i] == ' ')
      len += fnt->height/3; // FIXME: what's GM do about this?
    else {
      len += fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount].xs;
    }
  }
  return len;
}

unsigned int string_width_ext_line(variant vstr, gs_scalar w, int line)
{
  string str = toString(vstr);
  get_font(fnt,currentfont,0);

  unsigned int width = 0, tw = 0; int cl = 0;
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == '\r')
      if (cl == line) return width; else width = 0, cl +=1, i += str[i+1] == '\n';
    else if (str[i] == '\n')
      if (cl == line) return width; else width = 0, cl +=1;
    else if (str[i] == ' '){
      width += fnt->height/3, tw = 0;
      for (unsigned c = i+1; c < str.length(); c++)
      {
        if (str[c] == ' ' or str[c] == '\r' or str[c] == '\n')
          break;
        tw += fnt->glyphs[(unsigned char)(str[c] - fnt->glyphstart) % fnt->glyphcount].xs;
      }
      if (width+tw >= unsigned(w) && w != -1)
        if (cl == line) return width; else width = 0, cl +=1; else;
    } else
      width += fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount].xs;
  }
  return width;
}

unsigned int string_width_ext_line_count(variant vstr, gs_scalar w)
{
  string str = toString(vstr);
  get_font(fnt,currentfont,0);

  unsigned int width = 0, tw = 0, cl = 1;
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == '\r')
      width = 0, cl +=1, i += str[i+1] == '\n';
    else if (str[i] == '\n')
      width = 0, cl +=1;
    else if (str[i] == ' '){
      width += fnt->height/3, tw = 0;
      for (unsigned c = i+1; c < str.length(); c++)
      {
        if (str[c] == ' ' or str[c] == '\r' or str[c] == '\n')
          break;
        tw += fnt->glyphs[(unsigned char)(str[c] - fnt->glyphstart) % fnt->glyphcount].xs;
      }
      if (width+tw >= unsigned(w) && w != -1)
        width = 0, cl +=1;
    } else
      width += fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount].xs;
  }
  return cl;
}

unsigned int string_width(variant vstr)
{
  string str = toString(vstr);
  get_font(fnt,currentfont,0);
  int mlen = 0, tlen = 0;
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == '\r' or str[i] == '\n')
      tlen = 0;
    else if (str[i] == ' ')
      tlen += fnt->height/3; // FIXME: what's GM do about this?
    else {
      tlen += fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount].xs;
      if (tlen > mlen) mlen = tlen;
    }
  }
  return mlen;
}

unsigned int string_height(variant vstr)
{
  string str = toString(vstr);
  get_font(fnt,currentfont,0);
  float hgt = fnt->height;
  for (unsigned i = 0; i < str.length(); i++)
    if (str[i] == '\r' or str[i] == '\n')
      hgt += fnt->height;
  return hgt;
}

unsigned int string_width_ext(variant vstr, gs_scalar sep, gs_scalar w) //here sep doesn't do anything, but I can't make it 'default = ""', because its the second argument
{
  string str = toString(vstr);
  get_font(fnt,currentfont,0);

  unsigned int width = 0, maxwidth = 0;
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == ' '){
        if (width >= unsigned(w) && w!=-1)
            (width>maxwidth ? maxwidth=width, width = 0 : width = 0);
        else
            width += fnt->height/3; // FIXME: what's GM do about this?
    } else {
        fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
        width += g.xs;
    }
  }
  return maxwidth;
}

unsigned int string_height_ext(variant vstr, gs_scalar sep, gs_scalar w)
{
  string str = toString(vstr);
  get_font(fnt,currentfont,0);

  unsigned int width = 0, tw = 0, height = fnt->height;
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == '\r' or str[i] == '\n')
      width = 0, height +=  (sep+2 ? fnt->height : sep);
    else if (str[i] == ' '){
      width += fnt->height/3;
      tw = 0;
      for (unsigned c = i+1; c < str.length(); c++)
      {
        if (str[c] == ' ' or str[i] == '\r' or str[i] == '\n')
          break;
        fontglyph &g = fnt->glyphs[(unsigned char)(str[c] - fnt->glyphstart) % fnt->glyphcount];
        tw += g.xs;
      }

      if (width+tw >= unsigned(w) && w != -1)
        height += (sep==-1 ? fnt->height : sep), width = 0, tw = 0;
    } else {
        fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
        width += g.xs;
    }
  }
  return height;
}

}

////////////////////////////////////////////////////

namespace enigma_user
{

void draw_text(gs_scalar x, gs_scalar y, variant vstr)
{
  string str = toString(vstr);
  get_fontv(fnt,currentfont);
  float yy = valign == fa_top ? y+fnt->yoffset : valign == fa_middle ? y +fnt->yoffset - string_height(str)/2 : y + fnt->yoffset - string_height(str);
  if (halign == fa_left){
      float xx = x;
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r')
          xx = x, yy += fnt->height, i += str[i+1] == '\n';
        else if (str[i] == '\n')
          xx = x, yy += fnt->height;
        else if (str[i] == ' ')
          xx += get_space_width(fnt);
        else
        {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
			draw_primitive_begin_texture(pr_trianglestrip, fnt->texture);
			draw_vertex_texture(xx + g.x,  yy + g.y, g.tx, g.ty);
			draw_vertex_texture(xx + g.x2, yy + g.y, g.tx2, g.ty);
			draw_vertex_texture(xx + g.x,  yy + g.y2, g.tx,  g.ty2);
			draw_vertex_texture(xx + g.x2, yy + g.y2, g.tx2, g.ty2);
			draw_primitive_end();
          xx += int(g.xs);
        }
      }
  } else {
      float xx = halign == fa_center ? x-float(string_width_line(str,0)/2) : x-float(string_width_line(str,0)), line = 0;
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r'){
          line +=1, yy += fnt->height, i += str[i+1] == '\n';
          xx = halign == fa_center ? x-float(string_width_line(str,line)/2) : x-float(string_width_line(str,line));
        } else if (str[i] == '\n'){
          line +=1, yy += fnt->height;
          xx = halign == fa_center ? x-float(string_width_line(str,line)/2) : x-float(string_width_line(str,line));
        } else if (str[i] == ' ')
          xx += get_space_width(fnt);
        else
        {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
			draw_primitive_begin_texture(pr_trianglestrip, fnt->texture);
			draw_vertex_texture(xx + g.x,  yy + g.y, g.tx, g.ty);
			draw_vertex_texture(xx + g.x2, yy + g.y, g.tx2, g.ty);
			draw_vertex_texture(xx + g.x,  yy + g.y2, g.tx,  g.ty2);
			draw_vertex_texture(xx + g.x2, yy + g.y2, g.tx2, g.ty2);
			draw_primitive_end();
          xx += float(g.xs);
        }
      }
  }
}


void draw_text_sprite(gs_scalar x, gs_scalar y, variant vstr, int sep, int lineWidth, int sprite, int firstChar, int scale)
{
  string str = toString(vstr);

  //Easy lookup of width/height, accounting for scale.
  int w = sprite_get_width(sprite)  * scale;
  int h = sprite_get_height(sprite) * scale;

  //Default line separation height is just the font sprite's height.
  if (sep == -1) {
    sep = h;
  }

  //Now, simply draw each letter via sub-images, accounting for the width if required. 
  int offX = 0;
  int offY = 0;
  for (size_t i=0; i<str.length(); i++) {
    //Draw, update.
    char c = str[i];
    int subIndex = c - firstChar;
    draw_sprite_stretched(sprite, subIndex, x+offX, y+offY, w, h);
    offX += w;

    //Wrap if we've passed a wrappable character. Note that, contrary to GM5's documentation,
    //line-wrapping for text_sprites does NOT occur after hyphens. Only spaces break the line.
    if (lineWidth!=-1 && c==' ') {
      if (offX+w  > lineWidth) {
        offX = 0;
        offY += sep;
      }
    }
  }
}


void draw_text_skewed(gs_scalar x, gs_scalar y, variant vstr, gs_scalar top, gs_scalar bottom)
{
  string str = toString(vstr);
  get_fontv(fnt,currentfont);
  gs_scalar yy = valign == fa_top ? y+fnt->yoffset : valign == fa_middle ? y +fnt->yoffset - string_height(str)/2 : y + fnt->yoffset - string_height(str);
  if (halign == fa_left){
      gs_scalar xx = x;
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r')
          xx = x, yy += fnt->height, i += str[i+1] == '\n';
        else if (str[i] == '\n')
          xx = x, yy += fnt->height;
        else if (str[i] == ' ')
          xx += get_space_width(fnt);
        else
        {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
		  
			draw_primitive_begin_texture(pr_trianglestrip, fnt->texture);
			draw_vertex_texture(xx + g.x + top,     yy + g.y + top, g.tx, g.ty);
			draw_vertex_texture(xx + g.x2 + top,    yy + g.y + top, g.tx2, g.ty);
			draw_vertex_texture(xx + g.x + bottom,  yy + g.y2 + bottom, g.tx,  g.ty2);
			draw_vertex_texture(xx + g.x2 + bottom, yy + g.y2 + bottom, g.tx2, g.ty2);
			draw_primitive_end();
		  
          xx += gs_scalar(g.xs);
        }
      }
  } else {
      gs_scalar xx = halign == fa_center ? x-gs_scalar(string_width_line(str,0)/2) : x-gs_scalar(string_width_line(str,0)), line = 0;
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r'){
          line +=1, yy += fnt->height, i += str[i+1] == '\n';
          xx = halign == fa_center ? x-gs_scalar(string_width_line(str,line)/2) : x-gs_scalar(string_width_line(str,line));
        } else if (str[i] == '\n'){
          line +=1, yy += fnt->height;
          xx = halign == fa_center ? x-gs_scalar(string_width_line(str,line)/2) : x-gs_scalar(string_width_line(str,line));
        } else if (str[i] == ' ')
          xx += get_space_width(fnt);
        else
        {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
			draw_primitive_begin_texture(pr_trianglestrip, fnt->texture);
			draw_vertex_texture(xx + g.x + top,     yy + g.y + top, g.tx, g.ty);
			draw_vertex_texture(xx + g.x2 + top,    yy + g.y + top, g.tx2, g.ty);
			draw_vertex_texture(xx + g.x + bottom,  yy + g.y2 + bottom, g.tx,  g.ty2);
			draw_vertex_texture(xx + g.x2 + bottom, yy + g.y2 + bottom, g.tx2, g.ty2);
			draw_primitive_end();
          xx += gs_scalar(g.xs);
        }
      }
  }
}

void draw_text_ext(gs_scalar x, gs_scalar y, variant vstr, gs_scalar sep, gs_scalar w)
{
  string str = toString(vstr);
  get_fontv(fnt,currentfont);

  gs_scalar yy = valign == fa_top ? y+fnt->yoffset : valign == fa_middle ? y + fnt->yoffset - string_height_ext(str,sep,w)/2 : y + fnt->yoffset - string_height_ext(str,sep,w);
  if (halign == fa_left){
      gs_scalar xx = x, width = 0, tw = 0;
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r')
          xx = x, yy += (sep+2 ? fnt->height : sep), i += str[i+1] == '\n';
        else if (str[i] == '\n')
          xx = x, yy += (sep+2 ? fnt->height : sep);
        else if (str[i] == ' '){
          xx += get_space_width(fnt), width = xx-x;
          tw = 0;
          for (unsigned c = i+1; c < str.length(); c++)
          {
            if (str[c] == ' ' or str[c] == '\r' or str[c] == '\n')
              break;
            fontglyph &g = fnt->glyphs[(unsigned char)(str[c] - fnt->glyphstart) % fnt->glyphcount];
            tw += g.xs;
          }

          if (width+tw >= w && w != -1)
            xx = x, yy += (sep==-1 ? fnt->height : sep), width = 0, tw = 0;
        } else {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
			draw_primitive_begin_texture(pr_trianglestrip, fnt->texture);
			draw_vertex_texture(xx + g.x,  yy + g.y, g.tx, g.ty);
			draw_vertex_texture(xx + g.x2, yy + g.y, g.tx2, g.ty);
			draw_vertex_texture(xx + g.x,  yy + g.y2, g.tx,  g.ty2);
			draw_vertex_texture(xx + g.x2, yy + g.y2, g.tx2, g.ty2);
			draw_primitive_end();
          xx += gs_scalar(g.xs);
        }
      }
  } else {
      gs_scalar xx = halign == fa_center ? x-gs_scalar(string_width_ext_line(str,w,0)/2) : x-gs_scalar(string_width_ext_line(str,w,0)), line = 0, width = 0, tw = 0;
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r')
          line += 1, xx = halign == fa_center ? x-gs_scalar(string_width_ext_line(str,w,line)/2) : x-gs_scalar(string_width_ext_line(str,w,line)), yy += (sep+2 ? fnt->height : sep), i += str[i+1] == '\n', width = 0;
        else if (str[i] == '\n')
          line += 1, xx = halign == fa_center ? x-gs_scalar(string_width_ext_line(str,w,line)/2) : x-gs_scalar(string_width_ext_line(str,w,line)), yy += (sep+2 ? fnt->height : sep), width = 0;
        else if (str[i] == ' '){
          xx += get_space_width(fnt), width += get_space_width(fnt), tw = 0;
          for (unsigned c = i+1; c < str.length(); c++)
          {
            if (str[c] == ' ' or str[c] == '\r' or str[c] == '\n')
              break;
            fontglyph &g = fnt->glyphs[(unsigned char)(str[c] - fnt->glyphstart) % fnt->glyphcount];
            tw += g.xs;
          }

          if (width+tw >= w && w != -1)
            line += 1, xx = halign == fa_center ? x-gs_scalar(string_width_ext_line(str,w,line)/2) : x-gs_scalar(string_width_ext_line(str,w,line)), yy += (sep==-1 ? fnt->height : sep), width = 0, tw = 0;
        } else {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
			draw_primitive_begin_texture(pr_trianglestrip, fnt->texture);
			draw_vertex_texture(xx + g.x,  yy + g.y, g.tx, g.ty);
			draw_vertex_texture(xx + g.x2, yy + g.y, g.tx2, g.ty);
			draw_vertex_texture(xx + g.x,  yy + g.y2, g.tx,  g.ty2);
			draw_vertex_texture(xx + g.x2, yy + g.y2, g.tx2, g.ty2);
			draw_primitive_end();
          xx += gs_scalar(g.xs);
          width += g.xs;
        }
      }
    }
}

void draw_text_transformed(gs_scalar x, gs_scalar y, variant vstr, gs_scalar xscale, gs_scalar yscale, double rot)
{
  string str = toString(vstr);
  get_fontv(fnt,currentfont);

  rot *= M_PI/180;

  const gs_scalar sv = sin(rot), cv = cos(rot),
    svx = sv*xscale, cvx = cv*xscale, svy = sv * yscale,
    cvy = cv*yscale, sw = get_space_width(fnt) * cvx, sh = fnt->height/3 * svx,
    chi = fnt->height * cvy, shi = fnt->height * svy;

  gs_scalar xx, yy, tmpx, tmpy, tmpsize;
  if (valign == fa_top)
    yy = y + fnt->yoffset * cvy, xx = x + fnt->yoffset * svy;
  else if (valign == fa_middle)
    tmpsize = string_height(str), yy = y + (fnt->yoffset - tmpsize/2) * cvy, xx = x + (fnt->yoffset - tmpsize/2) * svy;
  else
    tmpsize = string_height(str), yy = y + (fnt->yoffset - tmpsize) * cvy, xx = x + (fnt->yoffset - tmpsize) * svy;
  tmpx = xx, tmpy = yy;
  if (halign == fa_left){
      int lines = 0, w;
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r')
          lines += 1, xx = tmpx + lines * shi, yy = tmpy + lines * chi, i += str[i+1] == '\n';
        else if (str[i] == '\n')
          lines += 1, xx = tmpx + lines * shi, yy = tmpy + lines * chi;
        else if (str[i] == ' ')
          xx += sw,
          yy -= sh;
        else
        {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
          w = g.x2-g.x;
            const gs_scalar lx = xx + g.y * svy;
            const gs_scalar ly = yy + g.y * cvy;
		  
			draw_primitive_begin_texture(pr_trianglestrip, fnt->texture);
			draw_vertex_texture(lx, ly, g.tx, g.ty);
			draw_vertex_texture(lx + w * cvx, ly - w * svx, g.tx2, g.ty);
			draw_vertex_texture(xx + g.y2 * svy,  yy + g.y2 * cvy, g.tx,  g.ty2);
			draw_vertex_texture(xx + w * cvx + g.y2 * svy, yy - w * svx + g.y2 * cvy, g.tx2, g.ty2);
			draw_primitive_end();
		  
          xx += gs_scalar(g.xs) * cvx;
          yy -= gs_scalar(g.xs) * svx;
        }
      }
    } else {
      tmpsize = string_width_line(str,0);
      if (halign == fa_center)
        xx = tmpx-tmpsize/2 * cvx, yy = tmpy+tmpsize/2 * svx;
      else
        xx = tmpx-tmpsize * cvx, yy = tmpy+tmpsize * svx;
      int lines = 0, w;
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r'){
          lines += 1, tmpsize = string_width_line(str,lines), i += str[i+1] == '\n';
          if (halign == fa_center)
            xx = tmpx-tmpsize/2 * cvx + lines * shi, yy = tmpy+tmpsize/2 * svx + lines * chi;
          else
            xx = tmpx-tmpsize * cvx + lines * shi, yy = tmpy+tmpsize * svx + lines * chi;
        } else if (str[i] == '\n'){
          lines += 1, tmpsize = string_width_line(str,lines);
          if (halign == fa_center)
            xx = tmpx-tmpsize/2 * cvx + lines * shi, yy = tmpy+tmpsize/2 * svx + lines * chi;
          else
            xx = tmpx-tmpsize * cvx + lines * shi, yy = tmpy+tmpsize * svx + lines * chi;
        } else if (str[i] == ' ')
          xx += sw,
          yy -= sh;
        else
        {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
          w = g.x2-g.x;
            const gs_scalar lx = xx + g.y * svy;
            const gs_scalar ly = yy + g.y * cvy;
		  
		  	draw_primitive_begin_texture(pr_trianglestrip, fnt->texture);
			draw_vertex_texture(lx, ly, g.tx, g.ty);
			draw_vertex_texture(lx + w * cvx, ly - w * svx, g.tx2, g.ty);
			draw_vertex_texture(xx + g.y2 * svy,  yy + g.y2 * cvy, g.tx,  g.ty2);
			draw_vertex_texture(xx + w * cvx + g.y2 * svy, yy - w * svx + g.y2 * cvy, g.tx2, g.ty2);
			draw_primitive_end();
		  
          xx += gs_scalar(g.xs) * cvx;
          yy -= gs_scalar(g.xs) * svx;
        }
      }
    }
}

void draw_text_ext_transformed(gs_scalar x, gs_scalar y, variant vstr, gs_scalar sep, gs_scalar w, gs_scalar xscale, gs_scalar yscale, double rot)
{
  string str = toString(vstr);
  get_fontv(fnt,currentfont);

  rot *= M_PI/180;

  const gs_scalar sv = sin(rot), cv = cos(rot),
    svx = sv*xscale, cvx = cv*xscale, svy = sv * yscale,
    cvy = cv*yscale, sw = get_space_width(fnt) * cvx, sh = fnt->height/3 * svx,
    chi = fnt->height * cvy, shi = fnt->height * svy;

  gs_scalar xx, yy, tmpx, tmpy, wi, tmpsize;
  if (valign == fa_top)
    yy = y + fnt->yoffset * cvy, xx = x + fnt->yoffset * svy;
  else if (valign == fa_middle)
    tmpsize = string_height_ext(str,sep,w), yy = y + (fnt->yoffset - tmpsize/2) * cvy, xx = x + (fnt->yoffset - tmpsize/2) * svy;
  else
    tmpsize = string_height_ext(str,sep,w), yy = y + (fnt->yoffset - tmpsize) * cvy, xx = x + (fnt->yoffset - tmpsize) * svy;
  tmpx = xx, tmpy = yy;

  if (halign == fa_left){
      int lines = 0,width = 0, tw = 0;
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r')
          lines += 1, xx = tmpx + lines * shi, width = 0, yy = tmpy + lines * chi, i += str[i+1] == '\n';
        else if (str[i] == '\n')
          lines += 1, xx = tmpx + lines * shi, width = 0, yy = tmpy + lines * chi;
        else if (str[i] == ' '){
          xx += sw,
          yy -= sh;

          width += get_space_width(fnt);
          tw = 0;
          for (unsigned c = i+1; c < str.length(); c++)
          {
            if (str[c] == ' ' or str[c] == '\r' or str[c] == '\n')
              break;
            fontglyph &g = fnt->glyphs[(unsigned char)(str[c] - fnt->glyphstart) % fnt->glyphcount];
            tw += g.xs;
          }

          if (width+tw >= w && w != -1)
            lines += 1, xx = tmpx + lines * shi, yy = tmpy + lines * chi, width = 0, tw = 0;
        } else {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
          wi = g.x2-g.x;
            const gs_scalar lx = xx + g.y * svy;
            const gs_scalar ly = yy + g.y * cvy;
		  
		  	draw_primitive_begin_texture(pr_trianglestrip, fnt->texture);
			draw_vertex_texture(lx, ly, g.tx,  g.ty);
			draw_vertex_texture(lx + wi * cvx, ly - wi * svx, g.tx2, g.ty);
			draw_vertex_texture(xx + g.y2 * svy,  yy + g.y2 * cvy, g.tx,  g.ty2);
			draw_vertex_texture(xx + wi * cvx + g.y2 * svy, yy - wi * svx + g.y2 * cvy, g.tx2, g.ty2);
			draw_primitive_end();
		  
          xx += gs_scalar(g.xs) * cvx;
          yy -= gs_scalar(g.xs) * svx;
          width += gs_scalar(g.xs);
        }
      }
  } else {
      int lines = 0,width = 0, tw = 0;
      tmpsize = string_width_ext_line(str,w,0);
      if (halign == fa_center)
        xx = tmpx-tmpsize/2 * cvx, yy = tmpy+tmpsize/2 * svx;
      else
        xx = tmpx-tmpsize * cvx, yy = tmpy+tmpsize * svx;
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r'){
          lines += 1, tmpsize = string_width_ext_line(str,w,lines), width = 0, i += str[i+1] == '\n';
          if (halign == fa_center)
            xx = tmpx-tmpsize/2 * cvx + lines * shi, yy = tmpy+tmpsize/2 * svx + lines * chi;
          else
            xx = tmpx-tmpsize * cvx + lines * shi, yy = tmpy+tmpsize * svx + lines * chi;
        } else if (str[i] == '\n'){
          lines += 1, tmpsize = string_width_ext_line(str,w,lines), width = 0;
          if (halign == fa_center)
            xx = tmpx-tmpsize/2 * cvx + lines * shi, yy = tmpy+tmpsize/2 * svx + lines * chi;
          else
            xx = tmpx-tmpsize * cvx + lines * shi, yy = tmpy+tmpsize * svx + lines * chi;
        } else if (str[i] == ' '){
          xx += sw,
          yy -= sh;

          width += get_space_width(fnt);
          tw = 0;
          for (unsigned c = i+1; c < str.length(); c++)
          {
            if (str[c] == ' ' or str[c] == '\r' or str[c] == '\n')
              break;
            fontglyph &g = fnt->glyphs[(unsigned char)(str[c] - fnt->glyphstart) % fnt->glyphcount];
            tw += g.xs;
          }

          if (width+tw >= w && w != -1){
            lines += 1, tmpsize = string_width_ext_line(str,w,lines);
            if (halign == fa_center)
                xx = tmpx-tmpsize/2 * cvx + lines * shi, yy = tmpy+tmpsize/2 * svx + lines * chi;
            else
                xx = tmpx-tmpsize * cvx + lines * shi, yy = tmpy+tmpsize * svx + lines * chi;
            width = 0, tw = 0;
          }
        } else {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
          wi = g.x2-g.x;
            const gs_scalar lx = xx + g.y * svy;
            const gs_scalar ly = yy + g.y * cvy;

		  	draw_primitive_begin_texture(pr_trianglestrip, fnt->texture);
			draw_vertex_texture(lx, ly, g.tx,  g.ty);
			draw_vertex_texture(lx + wi * cvx, ly - wi * svx, g.tx2, g.ty);
			draw_vertex_texture(xx + g.y2 * svy,  yy + g.y2 * cvy, g.tx,  g.ty2);
			draw_vertex_texture(xx + wi * cvx + g.y2 * svy, yy - wi * svx + g.y2 * cvy, g.tx2, g.ty2);
			draw_primitive_end();
			
          xx += gs_scalar(g.xs) * cvx;
          yy -= gs_scalar(g.xs) * svx;
          width += gs_scalar(g.xs);
        }
      }
  }
}

void draw_text_transformed_color(gs_scalar x, gs_scalar y, variant vstr, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar a)
{
  string str = toString(vstr);
  get_fontv(fnt,currentfont);

  rot *= M_PI/180;

  const gs_scalar sv = sin(rot), cv = cos(rot),
    svx = sv*xscale, cvx = cv*xscale, svy = sv * yscale,
    cvy = cv*yscale, sw = get_space_width(fnt) * cvx, sh = fnt->height/3 * svx,
    chi = fnt->height * cvy, shi = fnt->height * svy;

  gs_scalar xx, yy, tmpx, tmpy, tmpsize;
  int hcol1 = c1, hcol2 = c1, hcol3 = c3, hcol4 = c4, width = 0;
  if (valign == fa_top)
    yy = y + fnt->yoffset * cvy, xx = x + fnt->yoffset * svy;
  else if (valign == fa_middle)
    tmpsize = string_height(str), yy = y + (fnt->yoffset - tmpsize/2) * cvy, xx = x + (fnt->yoffset - tmpsize/2) * svy;
  else
    tmpsize = string_height(str), yy = y + (fnt->yoffset - tmpsize) * cvy, xx = x + (fnt->yoffset - tmpsize) * svy;
  tmpx = xx, tmpy = yy;
  if (halign == fa_left){
      int lines = 0, w;
      tmpsize = string_width_line(str,0);
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r')
          lines += 1, width = 0, xx = tmpx + lines * shi, yy = tmpy + lines * chi, i += str[i+1] == '\n', tmpsize = string_width_line(str,lines);
        else if (str[i] == '\n')
          lines += 1, width = 0, xx = tmpx + lines * shi, yy = tmpy + lines * chi, tmpsize = string_width_line(str,lines);
        else if (str[i] == ' ')
          xx += sw, yy -= sh,
          width += get_space_width(fnt);
        else
        {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
          w = g.x2-g.x;
          const gs_scalar lx = xx + g.y * svy;
          const gs_scalar ly = yy + g.y * cvy;
          hcol1 = merge_color(c1,c2,(gs_scalar)(width)/tmpsize);
          hcol2 = merge_color(c1,c2,(gs_scalar)(width+g.xs)/tmpsize);
          hcol3 = merge_color(c4,c3,(gs_scalar)(width)/tmpsize);
          hcol4 = merge_color(c4,c3,(gs_scalar)(width+g.xs)/tmpsize);
		  
			draw_primitive_begin_texture(pr_trianglestrip, fnt->texture);
			draw_vertex_texture_color(lx, ly, g.tx,  g.ty, hcol1, a);
			draw_vertex_texture_color(lx + w * cvx, ly - w * svx, g.tx2, g.ty, hcol2, a);
			draw_vertex_texture_color(xx + g.y2 * svy,  yy + g.y2 * cvy, g.tx, g.ty2, hcol4, a);
			draw_vertex_texture_color(xx + w * cvx + g.y2 * svy, yy - w * svx + g.y2 * cvy, g.tx2, g.ty2, hcol3, a);
			draw_primitive_end();
		  
          xx += gs_scalar(g.xs) * cvx;
          yy -= gs_scalar(g.xs) * svx;
          width += gs_scalar(g.xs);
        }
      }
    } else {
      tmpsize = string_width_line(str,0);
      if (halign == fa_center)
        xx = tmpx-tmpsize/2 * cvx, yy = tmpy+tmpsize/2 * svx;
      else
        xx = tmpx-tmpsize * cvx, yy = tmpy+tmpsize * svx;
      int lines = 0, w;
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r'){
          lines += 1, tmpsize = string_width_line(str,lines), i += str[i+1] == '\n', width = 0;
          if (halign == fa_center)
            xx = tmpx-tmpsize/2 * cvx + lines * shi, yy = tmpy+tmpsize/2 * svx + lines * chi;
          else
            xx = tmpx-tmpsize * cvx + lines * shi, yy = tmpy+tmpsize * svx + lines * chi;
        } else if (str[i] == '\n'){
          lines += 1, tmpsize = string_width_line(str,lines), width = 0;
          if (halign == fa_center)
            xx = tmpx-tmpsize/2 * cvx + lines * shi, yy = tmpy+tmpsize/2 * svx + lines * chi;
          else
            xx = tmpx-tmpsize * cvx + lines * shi, yy = tmpy+tmpsize * svx + lines * chi;
        } else if (str[i] == ' ')
          xx += sw, yy -= sh,
          width += get_space_width(fnt);
        else
        {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
          w = g.x2-g.x;
            const gs_scalar lx = xx + g.y * svy;
            const gs_scalar ly = yy + g.y * cvy;
            hcol1 = merge_color(c1,c2,(gs_scalar)(width)/tmpsize);
            hcol2 = merge_color(c1,c2,(gs_scalar)(width+g.xs)/tmpsize);
            hcol3 = merge_color(c4,c3,(gs_scalar)(width)/tmpsize);
            hcol4 = merge_color(c4,c3,(gs_scalar)(width+g.xs)/tmpsize);
		  
			draw_primitive_begin_texture(pr_trianglestrip, fnt->texture);
			draw_vertex_texture_color(lx, ly, g.tx,  g.ty, hcol1, a);
			draw_vertex_texture_color(lx + w * cvx, ly - w * svx, g.tx2, g.ty, hcol2, a);
			draw_vertex_texture_color(xx + g.y2 * svy,  yy + g.y2 * cvy, g.tx, g.ty2, hcol4, a);
			draw_vertex_texture_color(xx + w * cvx + g.y2 * svy, yy - w * svx + g.y2 * cvy, g.tx2, g.ty2, hcol3, a);
			draw_primitive_end();
		  
          xx += gs_scalar(g.xs) * cvx;
          yy -= gs_scalar(g.xs) * svx;
          width += gs_scalar(g.xs);
        }
      }
    }
}

void draw_text_ext_transformed_color(gs_scalar x, gs_scalar y, variant vstr, gs_scalar sep, gs_scalar w, gs_scalar xscale, gs_scalar yscale, double rot,int c1, int c2, int c3, int c4, gs_scalar a)
{
  string str = toString(vstr);
  get_fontv(fnt,currentfont);

  rot *= M_PI/180;

  const gs_scalar sv = sin(rot), cv = cos(rot),
    svx = sv*xscale, cvx = cv*xscale, svy = sv * yscale,
    cvy = cv*yscale, sw = get_space_width(fnt) * cvx, sh = fnt->height/3 * svx,
    chi = fnt->height * cvy, shi = fnt->height * svy;

  gs_scalar xx, yy, tmpx, tmpy, tmpsize;
  int hcol1 = c1, hcol2 = c1, hcol3 = c3, hcol4 = c4, width = 0;
  if (valign == fa_top)
    yy = y + fnt->yoffset * cvy, xx = x + fnt->yoffset * svy;
  else if (valign == fa_middle)
    tmpsize = string_height_ext(str,sep,w), yy = y + (fnt->yoffset - tmpsize/2) * cvy, xx = x + (fnt->yoffset - tmpsize/2) * svy;
  else
    tmpsize = string_height_ext(str,sep,w), yy = y + (fnt->yoffset - tmpsize) * cvy, xx = x + (fnt->yoffset - tmpsize) * svy;
  tmpx = xx, tmpy = yy;
  if (halign == fa_left){
      int lines = 0, tw = 0, wi;
      tmpsize = string_width_ext_line(str,w,0);
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r')
          lines += 1, width = 0, xx = tmpx + lines * shi, yy = tmpy + lines * chi, i += str[i+1] == '\n', tmpsize = string_width_ext_line(str,w,lines);
        else if (str[i] == '\n')
          lines += 1, width = 0, xx = tmpx + lines * shi, yy = tmpy + lines * chi, tmpsize = string_width_ext_line(str,w,lines);
        else if (str[i] == ' '){
          xx += sw, yy -= sh,
          width += get_space_width(fnt);
          tw = 0;
          for (unsigned c = i+1; c < str.length(); c++)
          {
            if (str[c] == ' ' or str[c] == '\r' or str[c] == '\n')
              break;
            fontglyph &g = fnt->glyphs[(unsigned char)(str[c] - fnt->glyphstart) % fnt->glyphcount];
            tw += g.xs;
          }

          if (width+tw >= w && w != -1)
            lines += 1, xx = tmpx + lines * shi, yy = tmpy + lines * chi, width = 0, tmpsize = string_width_ext_line(str,w,lines);
        } else {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
          wi = g.x2-g.x;
            const gs_scalar lx = xx + g.y * svy;
            const gs_scalar ly = yy + g.y * cvy;
            hcol1 = merge_color(c1,c2,(gs_scalar)(width)/tmpsize);
            hcol2 = merge_color(c1,c2,(gs_scalar)(width+g.xs)/tmpsize);
            hcol3 = merge_color(c4,c3,(gs_scalar)(width)/tmpsize);
            hcol4 = merge_color(c4,c3,(gs_scalar)(width+g.xs)/tmpsize);
		  
		  	draw_primitive_begin_texture(pr_trianglestrip, fnt->texture);
			draw_vertex_texture_color(lx, ly, g.tx,  g.ty, hcol1, a);
			draw_vertex_texture_color(lx + wi * cvx, ly - wi * svx, g.tx2, g.ty, hcol2, a);
			draw_vertex_texture_color(xx + g.y2 * svy,  yy + g.y2 * cvy, g.tx,  g.ty2, hcol4, a);
			draw_vertex_texture_color(xx + wi * cvx + g.y2 * svy, yy - wi * svx + g.y2 * cvy, g.tx2, g.ty2, hcol3, a);
			draw_primitive_end();
		  
		  
          xx += gs_scalar(g.xs) * cvx;
          yy -= gs_scalar(g.xs) * svx;
          width += gs_scalar(g.xs);
        }
      }
    } else {
      tmpsize = string_width_ext_line(str,w,0);
      if (halign == fa_center)
        xx = tmpx-tmpsize/2 * cvx, yy = tmpy+tmpsize/2 * svx;
      else
        xx = tmpx-tmpsize * cvx, yy = tmpy+tmpsize * svx;
      int lines = 0, wi, tw = 0;
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r'){
          lines += 1, tmpsize = string_width_ext_line(str,w,lines), i += str[i+1] == '\n', width = 0;
          if (halign == fa_center)
            xx = tmpx-tmpsize/2 * cvx + lines * shi, yy = tmpy+tmpsize/2 * svx + lines * chi;
          else
            xx = tmpx-tmpsize * cvx + lines * shi, yy = tmpy+tmpsize * svx + lines * chi;
        } else if (str[i] == '\n'){
          lines += 1, tmpsize = string_width_ext_line(str,w,lines), width = 0;
          if (halign == fa_center)
            xx = tmpx-tmpsize/2 * cvx + lines * shi, yy = tmpy+tmpsize/2 * svx + lines * chi;
          else
            xx = tmpx-tmpsize * cvx + lines * shi, yy = tmpy+tmpsize * svx + lines * chi;
        } else if (str[i] == ' '){
          xx += sw, yy -= sh,
          width += get_space_width(fnt);
          tw = 0;
          for (unsigned c = i+1; c < str.length(); c++)
          {
            if (str[c] == ' ' or str[c] == '\r' or str[c] == '\n')
              break;
            fontglyph &g = fnt->glyphs[(unsigned char)(str[c] - fnt->glyphstart) % fnt->glyphcount];
            tw += g.xs;
          }

          if (width+tw >= w && w != -1){
            lines += 1, tmpsize = string_width_ext_line(str,w,lines);
            if (halign == fa_center)
                xx = tmpx-tmpsize/2 * cvx + lines * shi, yy = tmpy+tmpsize/2 * svx + lines * chi;
            else
                xx = tmpx-tmpsize * cvx + lines * shi, yy = tmpy+tmpsize * svx + lines * chi;
            width = 0;
          }
        } else {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
          wi = g.x2-g.x;
            const gs_scalar lx = xx + g.y * svy;
            const gs_scalar ly = yy + g.y * cvy;
            hcol1 = merge_color(c1,c2,(gs_scalar)(width)/tmpsize);
            hcol2 = merge_color(c1,c2,(gs_scalar)(width+g.xs)/tmpsize);
            hcol3 = merge_color(c4,c3,(gs_scalar)(width)/tmpsize);
            hcol4 = merge_color(c4,c3,(gs_scalar)(width+g.xs)/tmpsize);

		  	draw_primitive_begin_texture(pr_trianglestrip, fnt->texture);
			draw_vertex_texture_color(lx, ly, g.tx,  g.ty, hcol1, a);
			draw_vertex_texture_color(lx + wi * cvx, ly - wi * svx, g.tx2, g.ty, hcol2, a);
			draw_vertex_texture_color(xx + g.y2 * svy,  yy + g.y2 * cvy, g.tx,  g.ty2, hcol4, a);
			draw_vertex_texture_color(xx + wi * cvx + g.y2 * svy, yy - wi * svx + g.y2 * cvy, g.tx2, g.ty2, hcol3, a);
			draw_primitive_end();

          xx += gs_scalar(g.xs) * cvx;
          yy -= gs_scalar(g.xs) * svx;
          width += gs_scalar(g.xs);
        }
      }
    }
}

void draw_text_color(gs_scalar x, gs_scalar y,variant vstr,int c1,int c2,int c3,int c4,gs_scalar a)
{
  string str = toString(vstr);
  get_fontv(fnt,currentfont);

  gs_scalar yy = valign == fa_top ? y+fnt->yoffset : valign == fa_middle ? y +fnt->yoffset - string_height(str)/2 : y + fnt->yoffset - string_height(str);
  int hcol1 = c1, hcol2 = c1, hcol3 = c3, hcol4 = c4,  line = 0;
  gs_scalar tx1, tx2, sw = (gs_scalar)string_width_line(str, line);
  if (halign == fa_left){
      gs_scalar xx = x;
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r'){
          xx = x, yy += fnt->height, i += str[i+1] == '\n';
          line += 1;
          sw = (gs_scalar)string_width_line(str, line);
        } else if (str[i] == '\n'){
          xx = x, yy += fnt->height;
          line += 1;
          sw = (gs_scalar)string_width_line(str, line);
        } else if (str[i] == ' ')
          xx += get_space_width(fnt);
        else
        {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
          tx1 = (xx-x)/sw, tx2 = (xx+g.xs-x)/sw;
          hcol1 = merge_color(c1,c2,tx1);
          hcol2 = merge_color(c1,c2,tx2);
          hcol3 = merge_color(c4,c3,tx1);
          hcol4 = merge_color(c4,c3,tx2);

		  	draw_primitive_begin_texture(pr_trianglestrip, fnt->texture);
			draw_vertex_texture_color(xx + g.x,  yy + g.y, g.tx, g.ty, hcol1, a);
			draw_vertex_texture_color(xx + g.x2, yy + g.y, g.tx2, g.ty, hcol2, a);
			draw_vertex_texture_color(xx + g.x,  yy + g.y2, g.tx,  g.ty2, hcol4, a);
			draw_vertex_texture_color(xx + g.x2, yy + g.y2, g.tx2, g.ty2, hcol3, a);
			draw_primitive_end();
			
          xx += gs_scalar(g.xs);
        }
      }
  } else {
      gs_scalar xx = halign == fa_center ? x-sw/2 : x-sw, tmpx = xx;
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r'){
          yy += fnt->height, i += str[i+1] == '\n', line += 1,
          sw = (gs_scalar)string_width_line(str, line), xx = halign == fa_center ? x-sw/2 : x-sw, tmpx = xx;
        } else if (str[i] == '\n'){
          yy += fnt->height, line += 1, sw = (gs_scalar)string_width_line(str, line),
          xx = halign == fa_center ? x-sw/2 : x-sw, tmpx = xx;
        } else if (str[i] == ' ')
          xx += get_space_width(fnt);
        else
        {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
          tx1 = (xx-tmpx)/sw, tx2 = (xx+g.xs-tmpx)/sw;
          hcol1 = merge_color(c1,c2,tx1);
          hcol2 = merge_color(c1,c2,tx2);
          hcol3 = merge_color(c4,c3,tx1);
          hcol4 = merge_color(c4,c3,tx2);
		  
		  	draw_primitive_begin_texture(pr_trianglestrip, fnt->texture);
			draw_vertex_texture_color(xx + g.x,  yy + g.y, g.tx, g.ty, hcol1, a);
			draw_vertex_texture_color(xx + g.x2, yy + g.y, g.tx2, g.ty, hcol2, a);
			draw_vertex_texture_color(xx + g.x,  yy + g.y2, g.tx,  g.ty2, hcol4, a);
			draw_vertex_texture_color(xx + g.x2, yy + g.y2, g.tx2, g.ty2, hcol3, a);
			draw_primitive_end();
		  
          xx += gs_scalar(g.xs);
        }
      }
  }
}

void draw_text_ext_color(gs_scalar x, gs_scalar y,variant vstr,gs_scalar sep, gs_scalar w, int c1,int c2,int c3,int c4, gs_scalar a)
{
  string str = toString(vstr);
  get_fontv(fnt,currentfont);

  gs_scalar yy = valign == fa_top ? y+fnt->yoffset : valign == fa_middle ? y + fnt->yoffset - string_height_ext(str,sep,w)/2 : y + fnt->yoffset - string_height_ext(str,sep,w);
  gs_scalar width = 0, tw = 0, line = 0, sw = string_width_ext_line(str, w, line);
  int hcol1 = c1, hcol2 = c1, hcol3 = c3, hcol4 = c4;
  if (halign == fa_left){
      gs_scalar xx = x;
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r')
          xx = x, yy +=  (sep+2 ? fnt->height : sep), i += str[i+1] == '\n',  width = 0, line += 1, sw = string_width_ext_line(str, w, line);
        else if (str[i] == '\n')
          xx = x, yy += (sep+2 ? fnt->height : sep), width = 0, line += 1, sw = string_width_ext_line(str, w, line);
        else if (str[i] == ' '){
          xx += get_space_width(fnt);
          width = xx-x;
          tw = 0;
          for (unsigned c = i+1; c < str.length(); c++)
          {
            if (str[c] == ' ' or str[c] == '\r' or str[c] == '\n')
              break;
            fontglyph &g = fnt->glyphs[(unsigned char)(str[c] - fnt->glyphstart) % fnt->glyphcount];
            tw += g.xs;
          }

          if (width+tw >= w && w != -1)
            xx = x, yy += (sep==-1 ? fnt->height : sep), width = 0, line += 1, sw = string_width_ext_line(str, w, line);
        } else {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
          hcol1 = merge_color(c1,c2,(gs_scalar)(width)/sw);
          hcol2 = merge_color(c1,c2,(gs_scalar)(width+g.xs)/sw);
          hcol3 = merge_color(c4,c3,(gs_scalar)(width)/sw);
          hcol4 = merge_color(c4,c3,(gs_scalar)(width+g.xs)/sw);

		  	draw_primitive_begin_texture(pr_trianglestrip, fnt->texture);
			draw_vertex_texture_color(xx + g.x,  yy + g.y, g.tx, g.ty, hcol1, a);
			draw_vertex_texture_color(xx + g.x2, yy + g.y, g.tx2, g.ty, hcol2, a);
			draw_vertex_texture_color(xx + g.x,  yy + g.y2, g.tx,  g.ty2, hcol4, a);
			draw_vertex_texture_color(xx + g.x2, yy + g.y2, g.tx2, g.ty2, hcol3, a);
			draw_primitive_end();
			
          xx += gs_scalar(g.xs);
          width = xx-x;
        }
      }
  } else {
      gs_scalar xx = halign == fa_center ? x-sw/2 : x-sw, tmpx = xx;
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r')
          yy +=  (sep+2 ? fnt->height : sep), i += str[i+1] == '\n',  width = 0, line += 1, sw = string_width_ext_line(str, w, line), xx = halign == fa_center ? x-sw/2 : x-sw, tmpx = xx;
        else if (str[i] == '\n')
          yy += (sep+2 ? fnt->height : sep), width = 0, line += 1, sw = string_width_ext_line(str, w, line), xx = halign == fa_center ? x-sw/2 : x-sw, tmpx = xx;
        else if (str[i] == ' '){
          xx += get_space_width(fnt), width = xx-tmpx, tw = 0;
          for (unsigned c = i+1; c < str.length(); c++)
          {
            if (str[c] == ' ' or str[c] == '\r' or str[c] == '\n')
              break;
            fontglyph &g = fnt->glyphs[(unsigned char)(str[c] - fnt->glyphstart) % fnt->glyphcount];
            tw += g.xs;
          }

          if (width+tw >= w && w != -1)
            yy += (sep==-1 ? fnt->height : sep), width = 0, line += 1, sw = string_width_ext_line(str, w, line), xx = halign == fa_center ? x-sw/2 : x-sw, tmpx = xx;
        } else {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
          hcol1 = merge_color(c1,c2,(gs_scalar)(width)/sw);
          hcol2 = merge_color(c1,c2,(gs_scalar)(width+g.xs)/sw);
          hcol3 = merge_color(c4,c3,(gs_scalar)(width)/sw);
          hcol4 = merge_color(c4,c3,(gs_scalar)(width+g.xs)/sw);

		  	draw_primitive_begin_texture(pr_trianglestrip, fnt->texture);
			draw_vertex_texture_color(xx + g.x,  yy + g.y, g.tx, g.ty, hcol1, a);
			draw_vertex_texture_color(xx + g.x2, yy + g.y, g.tx2, g.ty, hcol2, a);
			draw_vertex_texture_color(xx + g.x,  yy + g.y2, g.tx,  g.ty2, hcol4, a);
			draw_vertex_texture_color(xx + g.x2, yy + g.y2, g.tx2, g.ty2, hcol3, a);
			draw_primitive_end();
			
          xx += gs_scalar(g.xs);
          width = xx-tmpx;
        }
      }
  }
}

unsigned int font_get_texture(int fnt)
{
  get_font_null(f,fnt,-1);
  return f ? f->texture : unsigned(-1);
}
unsigned int font_get_texture_width(int fnt)
{
  get_font_null(f,fnt,-1);
  return f ? f->twid: unsigned(-1);
}
unsigned int font_get_texture_height(int fnt)
{
  get_font_null(f,fnt,-1);
  return f ? f->thgt: unsigned(-1);
}

void draw_set_font(int fnt) {
  enigma::currentfont = fnt;
}

int draw_get_font() {
  return enigma::currentfont;
}

}

