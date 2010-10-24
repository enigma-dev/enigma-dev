/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include <list>
#include <string>
#include <string.h>
#include <stdio.h>
using namespace std;

#include "../Graphics_Systems/graphics_mandatory.h"
#include "../libEGMstd.h"

#include "spritestruct.h"
#include "fontstruct.h"

namespace enigma
{
  font **fontstructarray = NULL;
  static size_t fontcount = 0;

  int font_new(unsigned char gs, unsigned char gc) // Creates a new font, allocating 'gc' glyphs
  {
    font *ret = new font;
    ret->glyphstart = gs;
    ret->glyphcount = gc;
    ret->glyphs = new fontglyph[gc];
    ret->height = 0;
    
    font **fsan = new font*[fontcount+1];
    for (unsigned i = 0; i < fontcount; i++)
      fsan[i] = fontstructarray[i];
    delete fontstructarray;
    fontstructarray = fsan;
    
    fontstructarray[fontcount] = ret;
    return fontcount++;
  }
  
  namespace rect_packer
  {
    struct pvrect {
      int x,y,w,h, placed;
      pvrect(): x(0),y(0),w(1),h(1),placed(-1) {}
      pvrect(int a,int b,int c,int d,int e):
        x(a), y(b), w(c), h(d), placed(e) {}
    };
    
    struct rectpnode
    {
      rectpnode* child[2];
      int x,y,wid,hgt;
      int c;
      rectpnode(): x(0),y(0), c(-1) { child[0] = NULL, child[1] = NULL; }
      rectpnode(int xx,int yy,int w,int h,rectpnode* c1=NULL,rectpnode* c2=NULL):
        x(xx), y(yy), wid(w), hgt(h), c(-1) { child[0] = c1, child[1]=c2; }
      void rect(int xx, int yy, int w, int h) { x=xx, y=yy, wid=w, hgt=h; }
    };
    
    void rncopy(rectpnode *h, pvrect *boxes, unsigned char c)
    {
      boxes[c].x = h->x,   boxes[c].y = h->y;
      boxes[c].w = h->wid, boxes[c].h = h->hgt;
    }
    
    rectpnode *rninsert(rectpnode* who, unsigned char c, pvrect* boxes)
    {
      rectpnode *newNode;
      if (who->child[0]) // Already split
      {
        newNode = rninsert(who->child[0], c, boxes);
        if (newNode != NULL) return newNode;
        return rninsert(who->child[1], c, boxes); //First failed; try second
      }
      else
      {
        if (who->c != -1) //already a rect here
          return NULL;
        
        if (boxes[c].w > who->wid or boxes[c].h > who->hgt) //doesn't fit
          return NULL;

        if (who->wid - boxes[c].w < 2 and who->hgt - boxes[c].h < 2) //tight squeeze
          return (who->c = c, who);
        
        // split node in two
        who->child[0] = new rectpnode;
        who->child[1] = new rectpnode;
        
        // horizontal or vertical split?
        const int dw = who->wid - boxes[c].w;
        const int dh = who->hgt - boxes[c].h;
        
        if (dw > dh) // Split horizontally
          who->child[0]->rect(who->x,            who->y, boxes[c].w,          who->hgt),
          who->child[1]->rect(who->x+boxes[c].w, who->y, who->wid-boxes[c].w, who->hgt);
        else // Split vertically
          who->child[0]->rect(who->x, who->y,            who->wid, boxes[c].h),
          who->child[1]->rect(who->x, who->y+boxes[c].h, who->wid, who->hgt-boxes[c].h);
            
        return rninsert(who->child[0], c, boxes);
      }
    }
    
    rectpnode *expand(rectpnode* who, int w, int h)
    {
      rectpnode *ret = who;
      if (h > who->hgt) // Need more vertical space
        ret = new rectpnode(who->x, who->y, who->wid, h, who,
              new rectpnode(who->x, who->y + who->hgt, who->wid, h - who->hgt, NULL, NULL));
      if (w > who->wid) // Need more horizontal space
        ret = new rectpnode(ret->x, ret->y, w, ret->hgt, ret,
              new rectpnode(ret->x + who->wid, ret->y, w-ret->wid, ret->hgt, NULL, NULL));
      return ret;
    }
  }
}

int font_add_sprite(int spr, unsigned char first, bool prop, int sep)
{
  enigma::sprite *sspr = enigma::spritestructarray[spr];
  if (!sspr) return -1;
  
  unsigned char gcount = sspr->subcount;
  int idfont = enigma::font_new(first, gcount);
  enigma::font *font = enigma::fontstructarray[idfont];
  
  // Now, what we'll implement is a packing algorithm.
  // This algorithm will try to fit as many glyphs as possible into
  // a square space based on the max height of the font.
  
  unsigned char* glyphdata[gcount];
  enigma::rect_packer::pvrect glyphmetrics[gcount];
  int glyphx[gcount], glyphy[gcount];
  
  int gwm = sspr->width, // Glyph width max: sprite width
      ghm = sspr->height, // Glyph width max: sprite width
      gtw = int((double)sspr->width / sspr->texbordy);
  
  font->height = ghm;
  
  for (int i = 0; i < gcount; i++)
  {
    unsigned char* data = enigma::graphics_get_texture_rgba(sspr->texturearray[i]);
    glyphdata[i] = data;
    
    // Here we calculate the bbox
    if (!prop)
       glyphmetrics[i].x = gwm, glyphmetrics[i].y = ghm,
       glyphmetrics[i].w = 0,   glyphmetrics[i].h = 0;
    else
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
    font->glyphs[i].x = glyphmetrics[i].x; // Save these metrics while x and y are still relative to each glyph
    font->glyphs[i].y = glyphmetrics[i].y;
    font->glyphs[i].x2 = glyphmetrics[i].w; // And while w and h are still the right and bottom edge coordinates
    font->glyphs[i].y2 = glyphmetrics[i].h;
    
    glyphmetrics[i].w -= glyphmetrics[i].x; // Fix width and height to be such
    glyphmetrics[i].h -= glyphmetrics[i].y; // instead of right and bottom
    glyphx[i] = glyphmetrics[i].x, glyphy[i] = glyphmetrics[i].y;
    glyphmetrics[i].placed = -1;
    
    font->glyphs[i].xs = glyphmetrics[i].w + sep; // This is just user-specified for sprite-loaded fonts
  }
  
  list<unsigned int> boxes;
  for (int i = 0; i < gcount; i++)
    boxes.push_back((glyphmetrics[i].w * glyphmetrics[i].h << 8) + i);
  boxes.sort();
  
  int w = 256, h = 256;
  enigma::rect_packer::rectpnode *rectplane = new enigma::rect_packer::rectpnode(0,0,w,h);
  for (list<unsigned int>::reverse_iterator i = boxes.rbegin(); i != boxes.rend() and w and h; )
  {
    printf("Add rectangle %d, which is of size %d x %d, to main cell of size %d x %d (%d, %d)\n", *i & 255, glyphmetrics[*i & 255].x, glyphmetrics[*i & 255].y, rectplane->wid, rectplane->hgt, w, h);
    enigma::rect_packer::rectpnode *nn = enigma::rect_packer::rninsert(rectplane, *i & 0xFF, glyphmetrics);
    if (nn)
      enigma::rect_packer::rncopy(nn, glyphmetrics, *i & 0xFF),
      i++;
    else
    {
      w > h ? h <<= 1 : w <<= 1,
      rectplane = enigma::rect_packer::expand(rectplane, w, h);
      printf("Expanded to %d by %d\n", w, h);
      if (!w or !h) return -1;
    }
  }
  
  int bigtex[w*h];
  for (int i = 0; i < gcount; i++)
  {
    for (int yy = 0; yy < glyphmetrics[i].h; yy++)
      for (int xx = 0; xx < glyphmetrics[i].w; xx++)
        bigtex[w*(glyphmetrics[i].y + yy) + glyphmetrics[i].x + xx] = glyphdata[i][gtw*(glyphy[i] + yy) + xx + glyphx[i]];
    delete[] glyphdata[i];
    
    font->glyphs[i].tx = glyphmetrics[i].x / double(w);
    font->glyphs[i].ty = glyphmetrics[i].y / double(h);
    font->glyphs[i].tx2 = (glyphmetrics[i].x + glyphmetrics[i].w) / double(w);
    font->glyphs[i].ty2 = (glyphmetrics[i].y + glyphmetrics[i].h) / double(h);
  }
  return idfont;
}
