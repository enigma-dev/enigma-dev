/** Copyright (C) 2010-2011 Josh Ventura
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

#include "rectpack.h"

#include <algorithm>
#include <stdlib.h>

namespace enigma {
namespace rect_packer {
    bool pack_rectangles(std::vector<pvrect>& rects, unsigned& width, unsigned &height) {

      // insert the metrics and calculate a minimum (power of 2) texture size
      width = 64, height = 64; // starter size for our texture
      rectpnode* rootNode = new rectpnode(0, 0, width, height);
      for (std::vector<pvrect>::reverse_iterator ii = rects.rbegin(); ii != rects.rend();) {
        size_t index = rects.size() - 1 - std::distance(rects.rbegin(), ii);
        rectpnode* node = rninsert(rootNode, index, rects.data());

        if (node) {
          rncopy(node, rects.data(), index);
          ii++;
        } else {
          width > height ? height <<= 1 : width <<= 1,
          rootNode = expand(rootNode, width, height);
          if (!width or !height) {
            delete rootNode;
            return false;
          }
        }
      }
      
      delete rootNode;
      return true;
    }
    
    // This is a container of which you will allocate and refer to precisely one instance.
    // It is best to use new and keep a reference to it, in case an insert() leads to an overflow,
    // in which case you may want to point to a new, bigger box created with expand().
    rectpnode::rectpnode(): x(0),y(0), c(-1) { child[0] = NULL, child[1] = NULL; }
    rectpnode::rectpnode(int xx,int yy,int w,int h,rectpnode* c1,rectpnode* c2):
        x(xx), y(yy), wid(w), hgt(h), c(-1) { child[0] = c1, child[1]=c2; }

    rectpnode::~rectpnode() {
      delete child[0];
      delete child[1];
    }
    
    void rectpnode::rect(int xx, int yy, int w, int h) { x=xx, y=yy, wid=w, hgt=h; }
    
    // Copies the content of a element `c` of pvrect array `boxes` into container `h`
    void rncopy(rectpnode *h, pvrect *boxes, unsigned int c)
    {
      boxes[c].x = h->x,   boxes[c].y = h->y;
      boxes[c].w = h->wid, boxes[c].h = h->hgt;
    }
    
    // Inserts a new node into container `who` using metrics obtained from `boxes`[`c`]
    rectpnode *rninsert(rectpnode* who, unsigned int c, pvrect* boxes)
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
        
        if ((int)boxes[c].w > who->wid or (int)boxes[c].h > who->hgt) //doesn't fit
          return NULL;

        if (who->wid - boxes[c].w < 2 and who->hgt - boxes[c].h < 2) //tight squeeze; too tight to fit a glyph inside, I promise.
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
    
    // Automatically allocates a container around existing container `who`, returning it.
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
