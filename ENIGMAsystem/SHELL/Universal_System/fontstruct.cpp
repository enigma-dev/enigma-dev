/** Copyright (C) 2008-2011 Josh Ventura
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

#include <list>
#include <string>
#include <string.h>
#include <stdio.h>
#include <vector>
using namespace std;

#include "Graphics_Systems/graphics_mandatory.h"
#include "libEGMstd.h"

#include "spritestruct.h"
#include "fontstruct.h"
#include "rectpack.h"

namespace enigma
{
  font **fontstructarray = NULL;
  extern size_t font_idmax;

  int font_new(unsigned char gs, unsigned char gc) // Creates a new font, allocating 'gc' glyphs
  {
    font *ret = new font;
    ret->glyphstart = gs - 1; //TODO: For some reason the glyphs are off by 1, -1 is temporary fix.
    ret->glyphcount = gc;
    ret->glyphs = new fontglyph[gc];
    ret->height = 0;

    font **fsan = new font*[font_idmax+2];
    font ** const fold = fontstructarray - 1;
    if (fontstructarray and fold) {
      for (unsigned i = 0; i <= font_idmax; i++)
        fsan[i] = fold[i];
      delete[] fold;
    }
    fontstructarray = fsan + 1;
    fontstructarray[font_idmax] = ret;
    return font_idmax++;
  }

  int font_pack(enigma::font *font , int spr, unsigned char gcount, bool prop, int sep)
  {
      // Implement packing algorithm.
      // This algorithm will try to fit as many glyphs as possible into
      // a square space based on the max height of the font.

      enigma::sprite *sspr = enigma::spritestructarray[spr];
      unsigned char* glyphdata[gcount]; // Raw font image data
      std::vector<enigma::rect_packer::pvrect> glyphmetrics(gcount);
      int glyphx[gcount], glyphy[gcount];

      int gwm = sspr->width, // Glyph width max: sprite width
          ghm = sspr->height, // Glyph height max: sprite height
          gtw = 0;

      font->height = ghm;

      for (int i = 0; i < gcount; i++)
      {
        gtw =  int((double)sspr->width / sspr->texbordyarray[i]);
        unsigned char* data = enigma::graphics_get_texture_rgba(sspr->texturearray[i]);
        glyphdata[i] = data;

        // Here we calculate the bbox
        if (!prop)
           glyphmetrics[i].x = 0,   glyphmetrics[i].y = 0,
           glyphmetrics[i].w = gwm-1, glyphmetrics[i].h = ghm-1;
        else
        {
          glyphmetrics[i].x = gwm, glyphmetrics[i].y = ghm,
          glyphmetrics[i].w = 0,   glyphmetrics[i].h = 0;
          for (int bx = 0; bx < gwm; bx++)
          for (int by = 0; by < ghm; by++)
          {
            if (data[(by*gtw + bx)<<2]) // If this pixel isn't completely transparent
            {
              if (bx < glyphmetrics[i].x) glyphmetrics[i].x = bx;
              if (bx > glyphmetrics[i].w) glyphmetrics[i].w = bx; // Treat width as right for now
              if (by < glyphmetrics[i].y) glyphmetrics[i].y = by;
              if (by > glyphmetrics[i].h) glyphmetrics[i].h = by; // Treat height as bottom for now
            }
          }
          if (glyphmetrics[i].x > glyphmetrics[i].w)
            glyphmetrics[i].x = 0, glyphmetrics[i].y = 0,
            glyphmetrics[i].w = 0, glyphmetrics[i].h = 0;
        }
        font->glyphs[i].x = glyphmetrics[i].x; // Save these metrics while x and y are still relative to each glyph
        font->glyphs[i].y = glyphmetrics[i].y;
        font->glyphs[i].x2 = glyphmetrics[i].w + 1; // And while w and h are still the right and bottom edge coordinates
        font->glyphs[i].y2 = glyphmetrics[i].h + 1;

        font->glyphs[i].xs = glyphmetrics[i].w + sep; // This is just user-specified for sprite-loaded fonts

        glyphmetrics[i].w -= glyphmetrics[i].x - 1; // Fix width and height to be such
        glyphmetrics[i].h -= glyphmetrics[i].y - 1; // instead of right and bottom
        glyphx[i] = glyphmetrics[i].x, glyphy[i] = glyphmetrics[i].y;
        glyphmetrics[i].placed = -1;
      }

      list<unsigned int> boxes;
      for (int i = 0; i < gcount; i++)
        boxes.push_back((glyphmetrics[i].w * glyphmetrics[i].h << 8) + i);
      boxes.sort();

	  //NOTE: This was hardcoded with 64x64 now it starts with the size of the first glyph, maybe should be fixed properly?
      int w = glyphmetrics[0].w, h = glyphmetrics[0].h ;
      enigma::rect_packer::rectpnode *rectplane = new enigma::rect_packer::rectpnode(0,0,w,h);
      for (list<unsigned int>::reverse_iterator i = boxes.rbegin(); i != boxes.rend() and w and h; )
      {
        enigma::rect_packer::rectpnode *nn = enigma::rect_packer::rninsert(rectplane, *i & 0xFF, &glyphmetrics.front());
        if (nn)
          enigma::rect_packer::rncopy(nn, &glyphmetrics.front(), *i & 0xFF),
          i++;
        else
        {
          w > h ? h <<= 1 : w <<= 1,
          rectplane = enigma::rect_packer::expand(rectplane, w, h);
          printf("Expanded to %d by %d\n", w, h);
          if (!w or !h) return false;
        }
      }

      int bigtex[w*h];
      for (int i = 0; i < gcount; i++)
      {
        // Copy the font glyph image into the big texture we just allocated
        for (int yy = 0; yy < glyphmetrics[i].h; yy++)
          for (int xx = 0; xx < glyphmetrics[i].w; xx++)
            bigtex[w*(glyphmetrics[i].y + yy) + glyphmetrics[i].x + xx] = ((unsigned int*)glyphdata[i])[gtw*(glyphy[i] + yy) + xx + glyphx[i]];
        delete[] glyphdata[i]; // Delete the image data we just copied

        font->glyphs[i].tx = glyphmetrics[i].x / double(w);
        font->glyphs[i].ty = glyphmetrics[i].y / double(h);
        font->glyphs[i].tx2 = (glyphmetrics[i].x + glyphmetrics[i].w) / double(w);
        font->glyphs[i].ty2 = (glyphmetrics[i].y + glyphmetrics[i].h) / double(h);
      }

      font->texture = enigma::graphics_create_texture(w,h,bigtex,true);
      font->twid = w;
      font->thgt = h;
      font->yoffset = 0;

      return true;
  }
}

namespace enigma_user
{

bool font_get_bold(int fnt)
{
    return enigma::fontstructarray[fnt]->bold;
}

bool font_get_italic(int fnt)
{
    return enigma::fontstructarray[fnt]->italic;
}

unsigned char font_get_first(int fnt)
{
    return enigma::fontstructarray[fnt]->glyphstart;
}

unsigned char font_get_last(int fnt)
{
    return enigma::fontstructarray[fnt]->glyphstart + enigma::fontstructarray[fnt]->glyphcount;
}

string font_get_fontname(int fnt)
{
    return enigma::fontstructarray[fnt]->fontname;
}

void font_delete(int fnt)
{
    delete enigma::fontstructarray[fnt];
    enigma::fontstructarray[fnt] = NULL;
}

bool font_exists(int fnt)
{
    return unsigned(fnt) < enigma::font_idmax && bool(enigma::fontstructarray[fnt]);
}

int font_add(string name, int size, bool bold, bool italic, unsigned char first, unsigned char last)
{
  int res = enigma::font_new(first, last-first);
  enigma::font *fnt = enigma::fontstructarray[res];
  fnt->name = name;
  fnt->fontsize = size;
  fnt->bold = bold;
  fnt->italic = italic;
  fnt->glyphstart = first;
  fnt->glyphcount = last-first;
  return res;
}

bool font_replace(int ind, string name, int size, bool bold, bool italic, unsigned char first, unsigned char last)
{
  enigma::font *fnt = enigma::fontstructarray[ind];
  fnt->name = name;
  fnt->fontsize = size;
  fnt->bold = bold;
  fnt->italic = italic;
  fnt->glyphstart = first;
  fnt->glyphcount = last-first;
  return true;
}

bool font_replace_sprite(int ind, int spr, unsigned char first, bool prop, int sep)
{
  enigma::sprite *sspr = enigma::spritestructarray[spr];
  if (!sspr) return false;

  unsigned char gcount = sspr->subcount;
  enigma::font *font = enigma::fontstructarray[ind];
  delete[] font->glyphs;
  font->glyphstart = first - 1; //TODO: For some reason the glyphs are off by 1, -1 is temporary fix.
  font->glyphcount = gcount;
  font->glyphs = new enigma::fontglyph[gcount];
  return enigma::font_pack(font, spr, gcount, prop, sep);
}

int font_add_sprite(int spr, unsigned char first, bool prop, int sep)
{
  enigma::sprite *sspr = enigma::spritestructarray[spr];
  if (!sspr) return -1;

  unsigned char gcount = sspr->subcount;
  int idfont = enigma::font_new(first, gcount);
  enigma::font *font = enigma::fontstructarray[idfont];
  if (!enigma::font_pack(font, spr, gcount, prop, sep)) return -1;
  return idfont;
}

}

