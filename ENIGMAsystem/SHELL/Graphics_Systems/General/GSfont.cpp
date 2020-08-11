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

#include "libEGMstd.h"

#include "GScolors.h"
#include "GSfont.h"
#include "GStextures.h"
#include "GSprimitives.h"
#include "GSsprite.h"

#include "Universal_System/var4.h"
#include "Universal_System/math_consts.h"
#include "Universal_System/Resources/fonts_internal.h"
#include "Universal_System/Resources/sprites.h"

#include <cmath>
#include <string>
#include <stdint.h>

using namespace std;
using namespace enigma::fonts;

namespace enigma {
  class CurrentFont {
    int _id;
  public:
    CurrentFont(int id=-1): _id(id) {} // << handle currentfont init
    void operator=(const int& other) { _id = other; } // << handle currentfont assign
    operator int() { // << cast back to int when passing it to function
      if (sprite_fonts.exists(_id)) return _id;
      return -1; // << use default left font when currentfont not exist
    }
  } currentfont;
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

const string unicodeAnds = "\x1F\x1F\x1F\x1F\x1F\x1F\x1F\x1F\x1F\x1F\x1F\x1F\x1F\x1F\x1F\x1F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x07\x07\x07\x07\x03\x03\x01";

static uint32_t getUnicodeCharacter(const string str, size_t& pos) {
  uint32_t character = 0;
  if (str[pos] & 0x80) {
    character = (str[pos] & unicodeAnds[(str[pos] >> 1) & 0x1F]);
    for (size_t ii = 1; ii <= 6; ii++) {
      if ((str[pos + ii] & 0xC0) != 0x80) { pos += ii - 1; break; }
      character <<= 6;
      character |= (str[pos + ii] & 0x3F);
    }
  } else {
    character = (uint32_t)str[pos];
  }
  return character;
}

namespace enigma_user {

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

namespace enigma {
  inline float get_space_width(const SpriteFont& fnt) {
    GlyphMetrics g = findGlyph(fnt, ' ');
    // Use the width of the space glyph when available,
    // else use the backup.
    // FIXME: Find out why the width is not available on Linux.
    if (!g.empty()) {
      return g.xs > 1 ? g.xs : fnt.lineHeight/3;
    } else {
      return fnt.lineHeight/3;
    }
  }
}

///////////////////////////////////////////////////

namespace enigma_user {

double string_char_width(variant vstr)
{
  string str = toString(vstr);
  const SpriteFont& fnt = sprite_fonts[currentfont];
  size_t i = 0;
  uint32_t character = getUnicodeCharacter(str, i);
  if (character == ' ') {
    return get_space_width(fnt);
  }
  GlyphMetrics g = findGlyph(fnt, character);
  if (g.empty()) {
    return get_space_width(fnt);
  } else {
    return g.xs;
  }
}

unsigned int string_width(variant vstr)
{
  string str = toString(vstr);
  const SpriteFont& fnt = sprite_fonts[currentfont];
  float mlen = 0, tlen = 0, slen = get_space_width(fnt);
  for (size_t i = 0; i < str.length(); i++)
  {
    uint32_t character = getUnicodeCharacter(str, i);
    if (character == '\r' or character == '\n') {
      tlen = 0;
    } else {
      GlyphMetrics g = findGlyph(fnt, character);
      if (character == ' ' or g.empty()) {
        tlen += slen;
      } else {
        tlen += g.xs;
      }
      if (tlen > mlen) mlen = tlen;
    }
  }
  return ceil(mlen);
}

unsigned int string_height(variant vstr)
{
  string str = toString(vstr);
  const SpriteFont& fnt = sprite_fonts[currentfont];
  float hgt = fnt.lineHeight;
  for (size_t i = 0; i < str.length(); i++)
    if (str[i] == '\r' or str[i] == '\n')
      hgt += fnt.lineHeight;
  return hgt;
}

unsigned int string_width_ext(variant vstr, gs_scalar sep, gs_scalar w) //here sep doesn't do anything, but I can't make it 'default = ""', because its the second argument
{
  string str = toString(vstr);
  const SpriteFont& fnt = sprite_fonts[currentfont];

  float width = 0, maxwidth = 0, slen = get_space_width(fnt);
  for (size_t i = 0; i < str.length(); i++)
  {
    uint32_t character = getUnicodeCharacter(str, i);

    GlyphMetrics g = findGlyph(fnt, character);
    if (character == ' ' or g.empty()) {
        if (width >= w && w!=-1) {
          (width>maxwidth ? maxwidth=width, width = 0 : width = 0);
        } else {
          width += slen;
        }
    } else {
      width += g.xs;
    }
    if (width > maxwidth) maxwidth = width;
  }
  return ceil(maxwidth);
}

unsigned int string_height_ext(variant vstr, gs_scalar sep, gs_scalar w)
{
  string str = toString(vstr);
  const SpriteFont& fnt = sprite_fonts[currentfont];

  float width = 0, tw = 0, height = fnt.lineHeight, slen = get_space_width(fnt);
  for (size_t i = 0; i < str.length(); i++)
  {
    uint32_t character = getUnicodeCharacter(str, i);
    if (character == '\r' or character == '\n') {
      width = 0, height +=  (sep+2 ? fnt.lineHeight : sep);
    } else {
      GlyphMetrics g = findGlyph(fnt, character);
      if (character == ' ' or g.empty()) {
        width += slen;
      }

      tw = 0;
      for (size_t c = i+1; c < str.length(); c++) {
        character = getUnicodeCharacter(str, c);
        if (character == ' ' or character == '\r' or character == '\n')
          break;
        tw += g.xs;
      }

      if (width+tw >= w && w != -1) {
        height += (sep==-1 ? fnt.lineHeight : sep), width = 0, tw = 0;
      } else {
        width += g.xs;
      }
    }
  }
  return ceil(height);
}

unsigned int string_width_line(variant vstr, int line)
{
  string str = toString(vstr);
  const SpriteFont& fnt = sprite_fonts[currentfont];
  float len = 0, cl = 0, slen = get_space_width(fnt);
  for (size_t i = 0; i < str.length(); i++)
  {
    uint32_t character = getUnicodeCharacter(str, i);
    if (character == '\r') {
      if (cl == line)
        return ceil(len);
      cl += 1;
      len = 0;
      i += (str[i+1] == '\n');
    } else if (character == '\n') {
      if (cl == line)
        return ceil(len);
      cl += 1;
      len = 0;
    } else {
      GlyphMetrics g = findGlyph(fnt, character);
      if (character == ' ' or g.empty())
        len += slen;
      else {
        len += g.xs;
      }
    }
  }
  return (cl != line ? 0 : ceil(len));
}

//TODO: These next functions can be rewritten to get rid of Schlemiel the Painter's algorithm happening in the second for loop
unsigned int string_width_ext_line(variant vstr, gs_scalar w, int line)
{
  string str = toString(vstr);
  const SpriteFont& fnt = sprite_fonts[currentfont];

  float width = 0, tw = 0; int cl = 0, slen = get_space_width(fnt);
  for (size_t i = 0; i < str.length(); i++)
  {
    uint32_t character = getUnicodeCharacter(str, i);
    if (character == '\r') {
      if (cl == line) return ceil(width); else {width = 0; cl +=1; i += str[i+1] == '\n';}
    } else if (character == '\n') {
      if (cl == line) return ceil(width); else width = 0, cl +=1;
    } else {
      GlyphMetrics g = findGlyph(fnt, character);
      if ((character == ' ' or g.empty()) && w != -1) {
        width += slen, tw = 0;
        for (size_t c = i+1; c < str.length(); c++)
        {
          uint32_t ct = getUnicodeCharacter(str, c);
          if (ct == ' ' or ct == '\r' or ct == '\n')
            break;
          GlyphMetrics gt = findGlyph(fnt, ct);
          tw += (!gt.empty() ? g.xs : slen);
        }
        if (width+tw >= w){
          if (cl == line) {
            return ceil(width);
          }
          width = 0;
          cl += 1;
        }
      } else {
        width += (!g.empty() ? g.xs:slen);
      }
    }
  }
  return  (cl != line ? 0 : ceil(width));
}

unsigned int string_width_ext_line_count(variant vstr, gs_scalar w)
{
  string str = toString(vstr);
  const SpriteFont& fnt = sprite_fonts[currentfont];

  float width = 0, tw = 0, slen = get_space_width(fnt);
  unsigned int cl = 1;
  for (size_t i = 0; i < str.length(); i++)
  {
    uint32_t character = getUnicodeCharacter(str, i);
    if (character == '\r') {
      width = 0, cl +=1, i += str[i+1] == '\n';
    } else if (character == '\n') {
      width = 0, cl +=1;
    } else {
      GlyphMetrics g = findGlyph(fnt, character);
      if ((character == ' ' or g.empty()) && w != -1){
        width += slen, tw = 0;
        for (size_t c = i+1; c < str.length(); c++)
        {
          uint32_t ct = getUnicodeCharacter(str, c);
          if (ct == ' ' or ct == '\r' or ct == '\n')
            break;
          GlyphMetrics gt = findGlyph(fnt, ct);
          tw += (!gt.empty() ? g.xs : slen);
        }
        if (width+tw >= w)
        width = 0, cl +=1;
      } else {
        width += (!g.empty() ? g.xs : slen);
      }
    }
  }
  return cl;
}

}

////////////////////////////////////////////////////

namespace enigma_user
{

void draw_text(gs_scalar x, gs_scalar y, variant vstr)
{
  string str = toString(vstr);
  const SpriteFont& fnt = sprite_fonts[currentfont];
  gs_scalar yy = valign == fa_top ? y+fnt.yOffset : valign == fa_middle ? y + fnt.yOffset - string_height(str)/2 : y + fnt.yOffset - string_height(str);
  float slen = get_space_width(fnt);
  if (halign == fa_left){
      gs_scalar xx = x;
      for (size_t i = 0; i < str.length(); i++)
      {
        uint32_t character = getUnicodeCharacter(str, i);

        if (character == '\r') {
          xx = x, yy += fnt.lineHeight, i += str[i+1] == '\n';
        } else if (character == '\n') {
          xx = x, yy += fnt.lineHeight;
        } else {
          GlyphMetrics g = findGlyph(fnt, character);
          if (character == ' ' or g.empty()) {
            xx += slen;
          } else {
            draw_primitive_begin_texture(pr_trianglestrip, fnt.texture.ID);
            draw_vertex_texture(xx + g.x,  yy + g.y, g.tx, g.ty);
            draw_vertex_texture(xx + g.x2, yy + g.y, g.tx2, g.ty);
            draw_vertex_texture(xx + g.x,  yy + g.y2, g.tx,  g.ty2);
            draw_vertex_texture(xx + g.x2, yy + g.y2, g.tx2, g.ty2);
            draw_primitive_end();
            xx += gs_scalar(g.xs);
          }
        }
      }
  } else {
      gs_scalar xx = halign == fa_center ? x-gs_scalar(string_width_line(str,0)/2) : x-gs_scalar(string_width_line(str,0)), line = 0;
      for (size_t i = 0; i < str.length(); i++)
      {
        uint32_t character = getUnicodeCharacter(str, i);

        if (character == '\r') {
          line +=1, yy += fnt.lineHeight, i += str[i+1] == '\n';
          xx = halign == fa_center ? x-gs_scalar(string_width_line(str,line)/2) : x-gs_scalar(string_width_line(str,line));
        } else if (character == '\n') {
          line +=1, yy += fnt.lineHeight;
          xx = halign == fa_center ? x-gs_scalar(string_width_line(str,line)/2) : x-gs_scalar(string_width_line(str,line));
        } else {
          GlyphMetrics g = findGlyph(fnt, character);
          if (character == ' ' or g.empty()) {
            xx += slen;
          } else {
            draw_primitive_begin_texture(pr_trianglestrip, fnt.texture.ID);
            draw_vertex_texture(xx + g.x,  yy + g.y, g.tx, g.ty);
            draw_vertex_texture(xx + g.x2, yy + g.y, g.tx2, g.ty);
            draw_vertex_texture(xx + g.x,  yy + g.y2, g.tx,  g.ty2);
            draw_vertex_texture(xx + g.x2, yy + g.y2, g.tx2, g.ty2);
            draw_primitive_end();
            xx += gs_scalar(g.xs);
          }
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
  //Line breaking can occur either before the current word (if it won't fit) or after the
  // current character (if it's a space).
  //Note that, contrary to GM5's documentation, line-wrapping for text_sprites does NOT occur
  // after hyphens. Only spaces break the line.
  int offX = 0;
  int offY = 0;
  char prev_c = '\0'; //Most recent character drawn.
  for (size_t i = 0; i<str.length(); i++) {
    //Fetch the next character to be drawn.
    uint32_t c = getUnicodeCharacter(str, i);

    //Handle newline wrapping immediately.
    if (c=='\n') {
      offX = 0;
      offY += sep + h;
      prev_c = c;
      continue;
    }

    //A line break can occur here if we are starting a new word that won't fit.
    if (lineWidth!=-1 && prev_c==' ' && c!= ' ') {
      //Assume a space at str[str.length()]
      size_t word_len = str.find(' ', i+1);
      if (word_len == string::npos) {
        word_len = str.length();
      }

      //Break if the next word is too long.
      if (offX + w * int(word_len - i) > lineWidth) {
        offX = 0;
        offY += sep;
      }
    }

    //Draw, update.
    int subIndex = c - firstChar;
    draw_sprite_stretched(sprite, subIndex, x+offX, y+offY, w, h);
    offX += w;

    //Wrap if we've passed a wrappable character.
    if (lineWidth != -1 && c == ' ') {
      if (offX+w  > lineWidth) {
        offX = 0;
        offY += sep;
      }
    }

    //Next
    prev_c = c;
  }
}


void draw_text_skewed(gs_scalar x, gs_scalar y, variant vstr, gs_scalar top, gs_scalar bottom)
{
  string str = toString(vstr);
  const SpriteFont& fnt = sprite_fonts[currentfont];
  gs_scalar yy = valign == fa_top ? y + fnt.yOffset : valign == fa_middle ? y + fnt.yOffset - string_height(str)/2 : y + fnt.yOffset - string_height(str);
  float slen = get_space_width(fnt);
  if (halign == fa_left){
    gs_scalar xx = x;
    for (size_t i = 0; i < str.length(); i++)
    {
      uint32_t character = getUnicodeCharacter(str, i);
      if (character == '\r') {
        xx = x, yy += fnt.lineHeight, i += str[i+1] == '\n';
      } else if (character == '\n') {
        xx = x, yy += fnt.lineHeight;
      } else {
        GlyphMetrics g = findGlyph(fnt, character);
        if (character == ' ' or g.empty()) {
          xx += slen;
        } else {
          draw_primitive_begin_texture(pr_trianglestrip, fnt.texture.ID);
          draw_vertex_texture(xx + g.x + top,     yy + g.y + top, g.tx, g.ty);
          draw_vertex_texture(xx + g.x2 + top,    yy + g.y + top, g.tx2, g.ty);
          draw_vertex_texture(xx + g.x + bottom,  yy + g.y2 + bottom, g.tx,  g.ty2);
          draw_vertex_texture(xx + g.x2 + bottom, yy + g.y2 + bottom, g.tx2, g.ty2);
          draw_primitive_end();

          xx += gs_scalar(g.xs);
        }
      }
    }
  } else {
    gs_scalar xx = halign == fa_center ? x-gs_scalar(string_width_line(str,0)/2) : x-gs_scalar(string_width_line(str,0)), line = 0;
    for (size_t i = 0; i < str.length(); i++)
    {
      uint32_t character = getUnicodeCharacter(str, i);
      if (character == '\r') {
        line +=1, yy += fnt.lineHeight, i += str[i+1] == '\n';
        xx = halign == fa_center ? x-gs_scalar(string_width_line(str,line)/2) : x-gs_scalar(string_width_line(str,line));
      } else if (character == '\n') {
        line +=1, yy += fnt.lineHeight;
        xx = halign == fa_center ? x-gs_scalar(string_width_line(str,line)/2) : x-gs_scalar(string_width_line(str,line));
      } else {
        GlyphMetrics g = findGlyph(fnt, character);
        if (character == ' ' or g.empty()) {
          xx += slen;
        } else {
          draw_primitive_begin_texture(pr_trianglestrip, fnt.texture.ID);
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
}

void draw_text_ext(gs_scalar x, gs_scalar y, variant vstr, gs_scalar sep, gs_scalar w)
{
  string str = toString(vstr);
  const SpriteFont& fnt = sprite_fonts[currentfont];

  gs_scalar yy = valign == fa_top ? y+fnt.yOffset : valign == fa_middle ? y + fnt.yOffset - string_height_ext(str,sep,w)/2 : y + fnt.yOffset - string_height_ext(str,sep,w);
  float slen = get_space_width(fnt);
  if (halign == fa_left){
    gs_scalar xx = x, width = 0, tw = 0;
    for (size_t i = 0; i < str.length(); i++)
    {
      uint32_t character = getUnicodeCharacter(str, i);
      if (character == '\r') {
        xx = x, yy += (sep+2 ? fnt.lineHeight : sep), i += str[i+1] == '\n';
      } else if (character == '\n') {
            xx = x, yy += (sep+2 ? fnt.lineHeight : sep);
      } else {
        GlyphMetrics g = findGlyph(fnt, character);
        if (character == ' ' or g.empty()) {
          xx += slen, width = xx-x;
          tw = 0;
          for (size_t c = i+1; c < str.length(); c++)
          {
            character = getUnicodeCharacter(str, c);
            if (character == ' ' or character == '\r' or character == '\n')
              break;
            g = findGlyph(fnt, character);
            tw += (!g.empty()?g.xs:slen);
          }
          if (width+tw >= w && w != -1)
          xx = x, yy += (sep==-1 ? fnt.lineHeight : sep), width = 0, tw = 0;
        } else {
          draw_primitive_begin_texture(pr_trianglestrip, fnt.texture.ID);
          draw_vertex_texture(xx + g.x,  yy + g.y, g.tx, g.ty);
          draw_vertex_texture(xx + g.x2, yy + g.y, g.tx2, g.ty);
          draw_vertex_texture(xx + g.x,  yy + g.y2, g.tx,  g.ty2);
          draw_vertex_texture(xx + g.x2, yy + g.y2, g.tx2, g.ty2);
          draw_primitive_end();
          xx += gs_scalar(g.xs);
        }
      }
    }
  } else {
    gs_scalar xx = halign == fa_center ? x-gs_scalar(string_width_ext_line(str,w,0)/2) : x-gs_scalar(string_width_ext_line(str,w,0)), line = 0, width = 0, tw = 0;
    for (size_t i = 0; i < str.length(); i++)
    {
      uint32_t character = getUnicodeCharacter(str, i);
      if (character == '\r') {
        line += 1, xx = halign == fa_center ? x-gs_scalar(string_width_ext_line(str,w,line)/2) : x-gs_scalar(string_width_ext_line(str,w,line)), yy += (sep+2 ? fnt.lineHeight : sep), i += str[i+1] == '\n', width = 0;
      } else if (character == '\n') {
        line += 1, xx = halign == fa_center ? x-gs_scalar(string_width_ext_line(str,w,line)/2) : x-gs_scalar(string_width_ext_line(str,w,line)), yy += (sep+2 ? fnt.lineHeight : sep), width = 0;
      } else {
        GlyphMetrics g = findGlyph(fnt, character);
        if (character == ' ' or g.empty()) {
          xx += slen, width += slen, tw = 0;
          for (size_t c = i+1; c < str.length(); c++)
          {
            character = getUnicodeCharacter(str, c);
            if (character == ' ' or character == '\r' or character == '\n')
              break;
            g = findGlyph(fnt, character);
            tw += (!g.empty() ? g.xs : slen);
          }

          if (width+tw >= w && w != -1)
            line += 1, xx = halign == fa_center ? x-gs_scalar(string_width_ext_line(str,w,line)/2) : x-gs_scalar(string_width_ext_line(str,w,line)), yy += (sep==-1 ? fnt.lineHeight : sep), width = 0, tw = 0;
        } else {
          draw_primitive_begin_texture(pr_trianglestrip, fnt.texture.ID);
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
}

void draw_text_transformed(gs_scalar x, gs_scalar y, variant vstr, gs_scalar xscale, gs_scalar yscale, double rot)
{
  string str = toString(vstr);
  const SpriteFont& fnt = sprite_fonts[currentfont];

  rot *= M_PI/180;

  const gs_scalar sv = sin(rot), cv = cos(rot),
    svx = sv*xscale, cvx = cv*xscale, svy = sv * yscale,
    cvy = cv*yscale, sw = get_space_width(fnt) * cvx, sh = fnt.lineHeight/3 * svx,
    chi = fnt.lineHeight * cvy, shi = fnt.lineHeight * svy;

  gs_scalar xx, yy, tmpx, tmpy, tmpsize;
  if (valign == fa_top)
    yy = y + fnt.yOffset * cvy, xx = x + fnt.yOffset * svy;
  else if (valign == fa_middle)
    tmpsize = string_height(str), yy = y + (fnt.yOffset - tmpsize/2) * cvy, xx = x + (fnt.yOffset - tmpsize/2) * svy;
  else
    tmpsize = string_height(str), yy = y + (fnt.yOffset - tmpsize) * cvy, xx = x + (fnt.yOffset - tmpsize) * svy;
  tmpx = xx, tmpy = yy;
  if (halign == fa_left){
      int lines = 0, w;
      for (size_t i = 0; i < str.length(); i++)
      {
        uint32_t character = getUnicodeCharacter(str, i);
        if (character == '\r') {
          lines += 1, xx = tmpx + lines * shi, yy = tmpy + lines * chi, i += str[i+1] == '\n';
        } else if (character == '\n') {
          lines += 1, xx = tmpx + lines * shi, yy = tmpy + lines * chi;
        } else {
          GlyphMetrics g = findGlyph(fnt, character);
          if (character == ' ' or g.empty()) {
            xx += sw,
            yy -= sh;
          } else {
            w = g.x2-g.x;
            const gs_scalar lx = xx + g.y * svy;
            const gs_scalar ly = yy + g.y * cvy;

            draw_primitive_begin_texture(pr_trianglestrip, fnt.texture.ID);
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
    } else {
      tmpsize = string_width_line(str,0);
      if (halign == fa_center)
        xx = tmpx-tmpsize/2 * cvx, yy = tmpy+tmpsize/2 * svx;
      else
        xx = tmpx-tmpsize * cvx, yy = tmpy+tmpsize * svx;
      int lines = 0, w;
      for (size_t i = 0; i < str.length(); i++)
      {
        uint32_t character = getUnicodeCharacter(str, i);
        if (character == '\r') {
          lines += 1, tmpsize = string_width_line(str,lines), i += str[i+1] == '\n';
          if (halign == fa_center)
            xx = tmpx-tmpsize/2 * cvx + lines * shi, yy = tmpy+tmpsize/2 * svx + lines * chi;
          else
            xx = tmpx-tmpsize * cvx + lines * shi, yy = tmpy+tmpsize * svx + lines * chi;
        } else if (character == '\n') {
          lines += 1, tmpsize = string_width_line(str,lines);
          if (halign == fa_center)
            xx = tmpx-tmpsize/2 * cvx + lines * shi, yy = tmpy+tmpsize/2 * svx + lines * chi;
          else
            xx = tmpx-tmpsize * cvx + lines * shi, yy = tmpy+tmpsize * svx + lines * chi;
        } else {
          GlyphMetrics g = findGlyph(fnt, character);
          if (character == ' ' or g.empty()) {
              xx += sw,
            yy -= sh;
          } else {
            w = g.x2-g.x;
            const gs_scalar lx = xx + g.y * svy;
            const gs_scalar ly = yy + g.y * cvy;

            draw_primitive_begin_texture(pr_trianglestrip, fnt.texture.ID);
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
}

void draw_text_ext_transformed(gs_scalar x, gs_scalar y, variant vstr, gs_scalar sep, gs_scalar w, gs_scalar xscale, gs_scalar yscale, double rot)
{
  string str = toString(vstr);
  const SpriteFont& fnt = sprite_fonts[currentfont];

  rot *= M_PI/180;

  const gs_scalar sv = sin(rot), cv = cos(rot),
    svx = sv*xscale, cvx = cv*xscale, svy = sv * yscale,
    cvy = cv*yscale, sw = get_space_width(fnt) * cvx, sh = fnt.lineHeight/3 * svx,
    chi = fnt.lineHeight * cvy, shi = fnt.lineHeight * svy;

  gs_scalar xx, yy, tmpx, tmpy, wi, tmpsize;
  if (valign == fa_top)
    yy = y + fnt.yOffset * cvy, xx = x + fnt.yOffset * svy;
  else if (valign == fa_middle)
    tmpsize = string_height_ext(str,sep,w), yy = y + (fnt.yOffset - tmpsize/2) * cvy, xx = x + (fnt.yOffset - tmpsize/2) * svy;
  else
    tmpsize = string_height_ext(str,sep,w), yy = y + (fnt.yOffset - tmpsize) * cvy, xx = x + (fnt.yOffset - tmpsize) * svy;
  tmpx = xx, tmpy = yy;

  if (halign == fa_left){
      int lines = 0,width = 0, tw = 0;
      for (size_t i = 0; i < str.length(); i++)
      {
        uint32_t character = getUnicodeCharacter(str, i);
        if (character == '\r') {
          lines += 1, xx = tmpx + lines * shi, width = 0, yy = tmpy + lines * chi, i += str[i+1] == '\n';
        } else if (character == '\n') {
          lines += 1, xx = tmpx + lines * shi, width = 0, yy = tmpy + lines * chi;
        } else {
          GlyphMetrics g = findGlyph(fnt, character);
          if (character == ' ' or g.empty()) {
            xx += sw,
            yy -= sh;

            width += sw;
            tw = 0;
            for (size_t c = i+1; c < str.length(); c++)
            {
              character = getUnicodeCharacter(str, c);
              if (character == ' ' or character == '\r' or character == '\n')
                break;
              g = findGlyph(fnt, character);
              tw += (!g.empty() ? g.xs : sw);
            }

            if (width+tw >= w && w != -1)
            lines += 1, xx = tmpx + lines * shi, yy = tmpy + lines * chi, width = 0, tw = 0;
          } else {
            wi = g.x2-g.x;
            const gs_scalar lx = xx + g.y * svy;
            const gs_scalar ly = yy + g.y * cvy;

            draw_primitive_begin_texture(pr_trianglestrip, fnt.texture.ID);
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
  } else {
      int lines = 0,width = 0, tw = 0;
      tmpsize = string_width_ext_line(str,w,0);
      if (halign == fa_center)
        xx = tmpx-tmpsize/2 * cvx, yy = tmpy+tmpsize/2 * svx;
      else
        xx = tmpx-tmpsize * cvx, yy = tmpy+tmpsize * svx;
      for (size_t i = 0; i < str.length(); i++)
      {
        uint32_t character = getUnicodeCharacter(str, i);
        if (character == '\r') {
          lines += 1, tmpsize = string_width_ext_line(str,w,lines), width = 0, i += str[i+1] == '\n';
          if (halign == fa_center)
            xx = tmpx-tmpsize/2 * cvx + lines * shi, yy = tmpy+tmpsize/2 * svx + lines * chi;
          else
            xx = tmpx-tmpsize * cvx + lines * shi, yy = tmpy+tmpsize * svx + lines * chi;
        } else if (character == '\n') {
          lines += 1, tmpsize = string_width_ext_line(str,w,lines), width = 0;
          if (halign == fa_center)
            xx = tmpx-tmpsize/2 * cvx + lines * shi, yy = tmpy+tmpsize/2 * svx + lines * chi;
          else
            xx = tmpx-tmpsize * cvx + lines * shi, yy = tmpy+tmpsize * svx + lines * chi;
        } else {
          GlyphMetrics g = findGlyph(fnt, character);
          if (character == ' ' or g.empty()) {
            xx += sw,
            yy -= sh;

            width += sw;
            tw = 0;
            for (size_t c = i+1; c < str.length(); c++)
            {
              character = getUnicodeCharacter(str, c);
              if (character == ' ' or character == '\r' or character == '\n')
                break;
              g = findGlyph(fnt, character);
              tw += (!g.empty() ? g.xs : sw);
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
              wi = g.x2-g.x;
              const gs_scalar lx = xx + g.y * svy;
              const gs_scalar ly = yy + g.y * cvy;

              draw_primitive_begin_texture(pr_trianglestrip, fnt.texture.ID);
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
}

void draw_text_transformed_color(gs_scalar x, gs_scalar y, variant vstr, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar a)
{
  string str = toString(vstr);
  const SpriteFont& fnt = sprite_fonts[currentfont];

  rot *= M_PI/180;

  const gs_scalar sv = sin(rot), cv = cos(rot),
    svx = sv*xscale, cvx = cv*xscale, svy = sv * yscale,
    cvy = cv*yscale, sw = get_space_width(fnt) * cvx, sh = fnt.lineHeight/3 * svx,
    chi = fnt.lineHeight * cvy, shi = fnt.lineHeight * svy;

  gs_scalar xx, yy, tmpx, tmpy, tmpsize;
  int hcol1 = c1, hcol2 = c1, hcol3 = c3, hcol4 = c4, width = 0;
  if (valign == fa_top)
    yy = y + fnt.yOffset * cvy, xx = x + fnt.yOffset * svy;
  else if (valign == fa_middle)
    tmpsize = string_height(str), yy = y + (fnt.yOffset - tmpsize/2) * cvy, xx = x + (fnt.yOffset - tmpsize/2) * svy;
  else
    tmpsize = string_height(str), yy = y + (fnt.yOffset - tmpsize) * cvy, xx = x + (fnt.yOffset - tmpsize) * svy;
  tmpx = xx, tmpy = yy;
  if (halign == fa_left){
      int lines = 0, w;
      tmpsize = string_width_line(str,0);
      for (size_t i = 0; i < str.length(); i++)
      {
        uint32_t character = getUnicodeCharacter(str, i);
        if (character == '\r') {
          lines += 1, width = 0, xx = tmpx + lines * shi, yy = tmpy + lines * chi, i += str[i+1] == '\n', tmpsize = string_width_line(str,lines);
        } else if (character == '\n') {
          lines += 1, width = 0, xx = tmpx + lines * shi, yy = tmpy + lines * chi, tmpsize = string_width_line(str,lines);
        } else {
          GlyphMetrics g = findGlyph(fnt, character);
          if (character == ' ' or g.empty()) {
            xx += sw, yy -= sh,
            width += sw;
          } else {
            w = g.x2-g.x;
            const gs_scalar lx = xx + g.y * svy;
            const gs_scalar ly = yy + g.y * cvy;
            hcol1 = merge_color(c1,c2,(gs_scalar)(width)/tmpsize);
            hcol2 = merge_color(c1,c2,(gs_scalar)(width+g.xs)/tmpsize);
            hcol3 = merge_color(c4,c3,(gs_scalar)(width)/tmpsize);
            hcol4 = merge_color(c4,c3,(gs_scalar)(width+g.xs)/tmpsize);

            draw_primitive_begin_texture(pr_trianglestrip, fnt.texture.ID);
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
    } else {
      tmpsize = string_width_line(str,0);
      if (halign == fa_center)
        xx = tmpx-tmpsize/2 * cvx, yy = tmpy+tmpsize/2 * svx;
      else
        xx = tmpx-tmpsize * cvx, yy = tmpy+tmpsize * svx;
      int lines = 0, w;
      for (size_t i = 0; i < str.length(); i++)
      {
        uint32_t character = getUnicodeCharacter(str, i);
        if (character == '\r') {
          lines += 1, tmpsize = string_width_line(str,lines), i += str[i+1] == '\n', width = 0;
          if (halign == fa_center)
            xx = tmpx-tmpsize/2 * cvx + lines * shi, yy = tmpy+tmpsize/2 * svx + lines * chi;
          else
            xx = tmpx-tmpsize * cvx + lines * shi, yy = tmpy+tmpsize * svx + lines * chi;
        } else if (character == '\n') {
          lines += 1, tmpsize = string_width_line(str,lines), width = 0;
          if (halign == fa_center)
            xx = tmpx-tmpsize/2 * cvx + lines * shi, yy = tmpy+tmpsize/2 * svx + lines * chi;
          else
            xx = tmpx-tmpsize * cvx + lines * shi, yy = tmpy+tmpsize * svx + lines * chi;
        } else {
          GlyphMetrics g = findGlyph(fnt, character);
          if (character == ' ' or g.empty()) {
            xx += sw, yy -= sh,
            width += sw;
          } else {
            w = g.x2-g.x;
            const gs_scalar lx = xx + g.y * svy;
            const gs_scalar ly = yy + g.y * cvy;
            hcol1 = merge_color(c1,c2,(gs_scalar)(width)/tmpsize);
            hcol2 = merge_color(c1,c2,(gs_scalar)(width+g.xs)/tmpsize);
            hcol3 = merge_color(c4,c3,(gs_scalar)(width)/tmpsize);
            hcol4 = merge_color(c4,c3,(gs_scalar)(width+g.xs)/tmpsize);

            draw_primitive_begin_texture(pr_trianglestrip, fnt.texture.ID);
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
}

void draw_text_ext_transformed_color(gs_scalar x, gs_scalar y, variant vstr, gs_scalar sep, gs_scalar w, gs_scalar xscale, gs_scalar yscale, double rot,int c1, int c2, int c3, int c4, gs_scalar a)
{
  string str = toString(vstr);
  const SpriteFont& fnt = sprite_fonts[currentfont];
  
  rot *= M_PI/180;

  const gs_scalar sv = sin(rot), cv = cos(rot),
    svx = sv*xscale, cvx = cv*xscale, svy = sv * yscale,
    cvy = cv*yscale, sw = get_space_width(fnt) * cvx, sh = fnt.lineHeight/3 * svx,
    chi = fnt.lineHeight * cvy, shi = fnt.lineHeight * svy;

  gs_scalar xx, yy, tmpx, tmpy, tmpsize;
  int hcol1 = c1, hcol2 = c1, hcol3 = c3, hcol4 = c4, width = 0;
  if (valign == fa_top)
    yy = y + fnt.yOffset * cvy, xx = x + fnt.yOffset * svy;
  else if (valign == fa_middle)
    tmpsize = string_height_ext(str,sep,w), yy = y + (fnt.yOffset - tmpsize/2) * cvy, xx = x + (fnt.yOffset - tmpsize/2) * svy;
  else
    tmpsize = string_height_ext(str,sep,w), yy = y + (fnt.yOffset - tmpsize) * cvy, xx = x + (fnt.yOffset - tmpsize) * svy;
  tmpx = xx, tmpy = yy;
  if (halign == fa_left){
      int lines = 0, tw = 0, wi;
      tmpsize = string_width_ext_line(str,w,0);
      for (size_t i = 0; i < str.length(); i++)
      {
        uint32_t character = getUnicodeCharacter(str, i);
        if (character == '\r') {
          lines += 1, width = 0, xx = tmpx + lines * shi, yy = tmpy + lines * chi, i += str[i+1] == '\n', tmpsize = string_width_ext_line(str,w,lines);
        } else if (character == '\n') {
          lines += 1, width = 0, xx = tmpx + lines * shi, yy = tmpy + lines * chi, tmpsize = string_width_ext_line(str,w,lines);
        } else {
          GlyphMetrics g = findGlyph(fnt, character);
          if (character == ' ' or g.empty()) {
            xx += sw, yy -= sh,
            width += sw;
            tw = 0;
            for (size_t c = i+1; c < str.length(); c++)
            {
              character = getUnicodeCharacter(str, c);
              if (character == ' ' or character == '\r' or character == '\n')
                break;
              g = findGlyph(fnt, character);
              tw += (!g.empty() ? g.xs : sw);
            }

            if (width+tw >= w && w != -1)
            lines += 1, xx = tmpx + lines * shi, yy = tmpy + lines * chi, width = 0, tmpsize = string_width_ext_line(str,w,lines);
          } else {
            wi = g.x2-g.x;
            const gs_scalar lx = xx + g.y * svy;
            const gs_scalar ly = yy + g.y * cvy;
            hcol1 = merge_color(c1,c2,(gs_scalar)(width)/tmpsize);
            hcol2 = merge_color(c1,c2,(gs_scalar)(width+g.xs)/tmpsize);
            hcol3 = merge_color(c4,c3,(gs_scalar)(width)/tmpsize);
            hcol4 = merge_color(c4,c3,(gs_scalar)(width+g.xs)/tmpsize);

            draw_primitive_begin_texture(pr_trianglestrip, fnt.texture.ID);
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
    } else {
      tmpsize = string_width_ext_line(str,w,0);
      if (halign == fa_center)
        xx = tmpx-tmpsize/2 * cvx, yy = tmpy+tmpsize/2 * svx;
      else
        xx = tmpx-tmpsize * cvx, yy = tmpy+tmpsize * svx;
      int lines = 0, wi, tw = 0;
      for (size_t i = 0; i < str.length(); i++)
      {
        uint32_t character = getUnicodeCharacter(str, i);
        if (character == '\r') {
          lines += 1, tmpsize = string_width_ext_line(str,w,lines), i += str[i+1] == '\n', width = 0;
          if (halign == fa_center)
            xx = tmpx-tmpsize/2 * cvx + lines * shi, yy = tmpy+tmpsize/2 * svx + lines * chi;
          else
            xx = tmpx-tmpsize * cvx + lines * shi, yy = tmpy+tmpsize * svx + lines * chi;
        } else if (character == '\n') {
          lines += 1, tmpsize = string_width_ext_line(str,w,lines), width = 0;
          if (halign == fa_center)
            xx = tmpx-tmpsize/2 * cvx + lines * shi, yy = tmpy+tmpsize/2 * svx + lines * chi;
          else
            xx = tmpx-tmpsize * cvx + lines * shi, yy = tmpy+tmpsize * svx + lines * chi;
        } else {
          GlyphMetrics g = findGlyph(fnt, character);
          if (character == ' ' or g.empty()) {
            xx += sw, yy -= sh,
            width += sw;
            tw = 0;
            for (size_t c = i+1; c < str.length(); c++)
            {
              character = getUnicodeCharacter(str, c);
              if (character == ' ' or character == '\r' or character == '\n')
                break;
              g = findGlyph(fnt, character);
              tw += (!g.empty() ? g.xs : sw);
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
            wi = g.x2-g.x;
            const gs_scalar lx = xx + g.y * svy;
            const gs_scalar ly = yy + g.y * cvy;
            hcol1 = merge_color(c1,c2,(gs_scalar)(width)/tmpsize);
            hcol2 = merge_color(c1,c2,(gs_scalar)(width+g.xs)/tmpsize);
            hcol3 = merge_color(c4,c3,(gs_scalar)(width)/tmpsize);
            hcol4 = merge_color(c4,c3,(gs_scalar)(width+g.xs)/tmpsize);

            draw_primitive_begin_texture(pr_trianglestrip, fnt.texture.ID);
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
}

void draw_text_color(gs_scalar x, gs_scalar y,variant vstr,int c1,int c2,int c3,int c4,gs_scalar a)
{
  string str = toString(vstr);
  const SpriteFont& fnt = sprite_fonts[currentfont];

  float slen = get_space_width(fnt);
  gs_scalar yy = valign == fa_top ? y+fnt.yOffset : valign == fa_middle ? y +fnt.yOffset - string_height(str)/2 : y + fnt.yOffset - string_height(str);
  int hcol1 = c1, hcol2 = c1, hcol3 = c3, hcol4 = c4,  line = 0;
  gs_scalar tx1, tx2, sw = (gs_scalar)string_width_line(str, line);
  if (halign == fa_left){
      gs_scalar xx = x;
      for (size_t i = 0; i < str.length(); i++)
      {
        uint32_t character = getUnicodeCharacter(str, i);
        if (character == '\r') {
          xx = x, yy += fnt.lineHeight, i += str[i+1] == '\n';
          line += 1;
          sw = (gs_scalar)string_width_line(str, line);
        } else if (character == '\n') {
          xx = x, yy += fnt.lineHeight;
          line += 1;
          sw = (gs_scalar)string_width_line(str, line);
        } else {
          GlyphMetrics g = findGlyph(fnt, character);
          if (character == ' ' or g.empty()) {
            xx += slen;
          } else {
            tx1 = (xx-x)/sw, tx2 = (xx+g.xs-x)/sw;
            hcol1 = merge_color(c1,c2,tx1);
            hcol2 = merge_color(c1,c2,tx2);
            hcol3 = merge_color(c4,c3,tx1);
            hcol4 = merge_color(c4,c3,tx2);

            draw_primitive_begin_texture(pr_trianglestrip, fnt.texture.ID);
            draw_vertex_texture_color(xx + g.x,  yy + g.y, g.tx, g.ty, hcol1, a);
            draw_vertex_texture_color(xx + g.x2, yy + g.y, g.tx2, g.ty, hcol2, a);
            draw_vertex_texture_color(xx + g.x,  yy + g.y2, g.tx,  g.ty2, hcol4, a);
            draw_vertex_texture_color(xx + g.x2, yy + g.y2, g.tx2, g.ty2, hcol3, a);
            draw_primitive_end();

            xx += gs_scalar(g.xs);
          }
        }
      }
  } else {
      gs_scalar xx = halign == fa_center ? x-sw/2 : x-sw, tmpx = xx;
      for (size_t i = 0; i < str.length(); i++)
      {
        uint32_t character = getUnicodeCharacter(str, i);
        if (character == '\r') {
          yy += fnt.lineHeight, i += str[i+1] == '\n', line += 1,
          sw = (gs_scalar)string_width_line(str, line), xx = halign == fa_center ? x-sw/2 : x-sw, tmpx = xx;
        } else if (character == '\n') {
          yy += fnt.lineHeight, line += 1, sw = (gs_scalar)string_width_line(str, line),
          xx = halign == fa_center ? x-sw/2 : x-sw, tmpx = xx;
        } else {
          GlyphMetrics g = findGlyph(fnt, character);
          if (character == ' ' or g.empty()) {
            xx += slen;
          } else {
            tx1 = (xx-tmpx)/sw, tx2 = (xx+g.xs-tmpx)/sw;
            hcol1 = merge_color(c1,c2,tx1);
            hcol2 = merge_color(c1,c2,tx2);
            hcol3 = merge_color(c4,c3,tx1);
            hcol4 = merge_color(c4,c3,tx2);

            draw_primitive_begin_texture(pr_trianglestrip, fnt.texture.ID);
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
}

void draw_text_ext_color(gs_scalar x, gs_scalar y,variant vstr,gs_scalar sep, gs_scalar w, int c1,int c2,int c3,int c4, gs_scalar a)
{
  string str = toString(vstr);
  const SpriteFont& fnt = sprite_fonts[currentfont];

  gs_scalar yy = valign == fa_top ? y+fnt.yOffset : valign == fa_middle ? y + fnt.yOffset - string_height_ext(str,sep,w)/2 : y + fnt.yOffset - string_height_ext(str,sep,w);
  gs_scalar width = 0, tw = 0, line = 0, sw = string_width_ext_line(str, w, line);
  float slen = get_space_width(fnt);
  int hcol1 = c1, hcol2 = c1, hcol3 = c3, hcol4 = c4;
  if (halign == fa_left){
      gs_scalar xx = x;
      for (size_t i = 0; i < str.length(); i++)
      {
        uint32_t character = getUnicodeCharacter(str, i);
        if (character == '\r') {
          xx = x, yy +=  (sep+2 ? fnt.lineHeight : sep), i += str[i+1] == '\n',  width = 0, line += 1, sw = string_width_ext_line(str, w, line);
        } else if (character == '\n') {
          xx = x, yy += (sep+2 ? fnt.lineHeight : sep), width = 0, line += 1, sw = string_width_ext_line(str, w, line);
        } else {
          GlyphMetrics g = findGlyph(fnt, character);
          if (character == ' ' or g.empty()) {
            xx += slen;
            width = xx-x;
            tw = 0;
            for (size_t c = i+1; c < str.length(); c++)
            {
              character = getUnicodeCharacter(str, c);
              if (character == ' ' or character == '\r' or character == '\n')
                break;
              g = findGlyph(fnt, character);
              tw += (!g.empty() ? g.xs : slen);
            }

            if (width+tw >= w && w != -1)
            xx = x, yy += (sep==-1 ? fnt.lineHeight : sep), width = 0, line += 1, sw = string_width_ext_line(str, w, line);
          } else {
            hcol1 = merge_color(c1,c2,(gs_scalar)(width)/sw);
            hcol2 = merge_color(c1,c2,(gs_scalar)(width+g.xs)/sw);
            hcol3 = merge_color(c4,c3,(gs_scalar)(width)/sw);
            hcol4 = merge_color(c4,c3,(gs_scalar)(width+g.xs)/sw);

            draw_primitive_begin_texture(pr_trianglestrip, fnt.texture.ID);
            draw_vertex_texture_color(xx + g.x,  yy + g.y, g.tx, g.ty, hcol1, a);
            draw_vertex_texture_color(xx + g.x2, yy + g.y, g.tx2, g.ty, hcol2, a);
            draw_vertex_texture_color(xx + g.x,  yy + g.y2, g.tx,  g.ty2, hcol4, a);
            draw_vertex_texture_color(xx + g.x2, yy + g.y2, g.tx2, g.ty2, hcol3, a);
            draw_primitive_end();

            xx += gs_scalar(g.xs);
            width = xx-x;
          }
        }
      }
  } else {
      gs_scalar xx = halign == fa_center ? x-sw/2 : x-sw, tmpx = xx;
      for (size_t i = 0; i < str.length(); i++)
      {
        uint32_t character = getUnicodeCharacter(str, i);
        if (character == '\r') {
          yy +=  (sep+2 ? fnt.lineHeight : sep), i += str[i+1] == '\n',  width = 0, line += 1, sw = string_width_ext_line(str, w, line), xx = halign == fa_center ? x-sw/2 : x-sw, tmpx = xx;
        } else if (character == '\n') {
          yy += (sep+2 ? fnt.lineHeight : sep), width = 0, line += 1, sw = string_width_ext_line(str, w, line), xx = halign == fa_center ? x-sw/2 : x-sw, tmpx = xx;
        } else {
          GlyphMetrics g = findGlyph(fnt, character);
          if (character == ' ' or g.empty()) {
            xx += slen, width = xx-tmpx, tw = 0;
            for (size_t c = i+1; c < str.length(); c++)
            {
              character = getUnicodeCharacter(str, c);
              if (character == ' ' or character == '\r' or character == '\n')
                break;
              g = findGlyph(fnt, character);
              tw += (!g.empty() ? g.xs : slen);
            }
            if (width+tw >= w && w != -1)
            yy += (sep==-1 ? fnt.lineHeight : sep), width = 0, line += 1, sw = string_width_ext_line(str, w, line), xx = halign == fa_center ? x-sw/2 : x-sw, tmpx = xx;
          } else {
          hcol1 = merge_color(c1,c2,(gs_scalar)(width)/sw);
            hcol2 = merge_color(c1,c2,(gs_scalar)(width+g.xs)/sw);
            hcol3 = merge_color(c4,c3,(gs_scalar)(width)/sw);
            hcol4 = merge_color(c4,c3,(gs_scalar)(width+g.xs)/sw);

            draw_primitive_begin_texture(pr_trianglestrip, fnt.texture.ID);
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
}

unsigned font_get_texture(int id) {
  const SpriteFont& fnt = sprite_fonts.get(id);
  return fnt.texture.ID;
}

unsigned font_get_texture_width(int id) {
  const SpriteFont& fnt = sprite_fonts.get(id);
  return fnt.texture.width;
}

unsigned font_get_texture_height(int id) {
  const SpriteFont& fnt = sprite_fonts.get(id);
  return fnt.texture.height;
}

unsigned font_height(int id) {
  const SpriteFont& fnt = sprite_fonts.get(id);
  return fnt.lineHeight;
}

void draw_set_font(int fnt) {
  if (fnt == -1 || sprite_fonts.exists(fnt))
    enigma::currentfont = fnt;
  #ifdef DEBUG_MODE
  else
    DEBUG_MESSAGE("Requested font asset " + std::to_string(fnt) + " does not exist.", MESSAGE_TYPE::M_USER_ERROR);
  #endif
  
}

int draw_get_font() {
  return enigma::currentfont;
}

}