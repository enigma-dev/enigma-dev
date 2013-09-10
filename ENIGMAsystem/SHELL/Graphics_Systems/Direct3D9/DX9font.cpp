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
#include "Direct3D9Headers.h"
#include "Universal_System/var4.h"
#include "libEGMstd.h"
#include "../General/GScolors.h"
#include "../General/GSfont.h"
#include "../General/GStextures.h"
#include "DX9binding.h"

using namespace std;
#include "Universal_System/fontstruct.h"

#include "Bridges/General/DX9Device.h"
#include "DX9TextureStruct.h"

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

namespace enigma {
  D3DCOLOR get_currentcolor();
}

////////////////////////////////////////////////////

namespace enigma_user
{

void draw_text(gs_scalar x, gs_scalar y, variant vstr)
{
  #ifdef CODEBLOX
    return;
  #endif
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
			D3DXVECTOR3 pos(xx + g.x, yy + g.y, 0);
			tagRECT rect;
			rect.left = g.tx * fnt->twid; rect.top = g.ty * fnt->thgt; rect.right = g.tx2 * fnt->twid; rect.bottom = g.ty2 * fnt->thgt;
			dsprite->Draw(GmTextures[fnt->texture]->gTexture, &rect, NULL, &pos, enigma::get_currentcolor());
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
			D3DXVECTOR3 pos(xx + g.x, yy + g.y, 0);
			tagRECT rect;
			rect.left = g.tx * fnt->twid; rect.top = g.ty * fnt->thgt; rect.right = g.tx2 * fnt->twid; rect.bottom = g.ty2 * fnt->thgt;
			dsprite->Draw(GmTextures[fnt->texture]->gTexture, &rect, NULL, &pos, enigma::get_currentcolor());
          xx += float(g.xs);
        }
      }
  }
}

void draw_text_skewed(gs_scalar x, gs_scalar y, variant vstr, gs_scalar top, gs_scalar bottom)
{

}

void draw_text_ext(gs_scalar x, gs_scalar y, variant vstr, gs_scalar sep, gs_scalar w)
{
  string str = toString(vstr);
  get_fontv(fnt,currentfont);

  float yy = valign == fa_top ? y+fnt->yoffset : valign == fa_middle ? y + fnt->yoffset - string_height_ext(str,sep,w)/2 : y + fnt->yoffset - string_height_ext(str,sep,w);

  if (halign == fa_left){
      float xx = x, width = 0, tw = 0;
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
			D3DXVECTOR3 pos(xx + g.x, yy + g.y, 0);
			tagRECT rect;
			rect.left = g.tx * fnt->twid; rect.top = g.ty * fnt->thgt; rect.right = g.tx2 * fnt->twid; rect.bottom = g.ty2 * fnt->thgt;
			dsprite->Draw(GmTextures[fnt->texture]->gTexture, &rect, NULL, &pos, enigma::get_currentcolor());
          xx += float(g.xs);
        }
      }
  } else {
      float xx = halign == fa_center ? x-float(string_width_ext_line(str,w,0)/2) : x-float(string_width_ext_line(str,w,0)), line = 0, width = 0, tw = 0;
      for (unsigned i = 0; i < str.length(); i++)
      {
        if (str[i] == '\r')
          line += 1, xx = halign == fa_center ? x-float(string_width_ext_line(str,w,line)/2) : x-float(string_width_ext_line(str,w,line)), yy += (sep+2 ? fnt->height : sep), i += str[i+1] == '\n', width = 0;
        else if (str[i] == '\n')
          line += 1, xx = halign == fa_center ? x-float(string_width_ext_line(str,w,line)/2) : x-float(string_width_ext_line(str,w,line)), yy += (sep+2 ? fnt->height : sep), width = 0;
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
            line += 1, xx = halign == fa_center ? x-float(string_width_ext_line(str,w,line)/2) : x-float(string_width_ext_line(str,w,line)), yy += (sep==-1 ? fnt->height : sep), width = 0, tw = 0;
        } else {
          fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
            D3DXVECTOR3 pos(xx + g.x, yy + g.y, 0);
			tagRECT rect;
			rect.left = g.tx * fnt->twid; rect.top = g.ty * fnt->thgt; rect.right = g.tx2 * fnt->twid; rect.bottom = g.ty2 * fnt->thgt;
			dsprite->Draw(GmTextures[fnt->texture]->gTexture, &rect, NULL, &pos, enigma::get_currentcolor());
          xx += float(g.xs);
          width += g.xs;
        }
      }
    }
}

void draw_text_transformed(gs_scalar x, gs_scalar y, variant vstr, gs_scalar xscale, gs_scalar yscale, double rot)
{

}

void draw_text_ext_transformed(gs_scalar x, gs_scalar y, variant vstr, gs_scalar sep, gs_scalar w, gs_scalar xscale, gs_scalar yscale, double rot)
{

}

void draw_text_transformed_color(gs_scalar x, gs_scalar y, variant vstr, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, double a)
{

}

void draw_text_ext_transformed_color(gs_scalar x, gs_scalar y, variant vstr, gs_scalar sep, gs_scalar w, gs_scalar xscale, gs_scalar yscale, double rot,int c1, int c2, int c3, int c4, double a)
{

}

void draw_text_color(gs_scalar x, gs_scalar y,variant vstr,int c1,int c2,int c3,int c4,double a)
{

}

void draw_text_ext_color(gs_scalar x, gs_scalar y,variant vstr,gs_scalar sep, gs_scalar w, int c1,int c2,int c3,int c4,double a)
{

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

