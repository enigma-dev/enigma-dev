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

}

}

