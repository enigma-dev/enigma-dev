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

#include <stdlib.h>
#include "rectpack.h"

namespace enigma
{
  namespace rect_packer
  {
    // This is a simple rectangle to be mass-instantiated; you should probably allocate a large array statically, one per glyph.
    pvrect::pvrect(): x(0),y(0),w(1),h(1),placed(-1) {}
    pvrect::pvrect(int a,int b,int c,int d,int e):
        x(a), y(b), w(c), h(d), placed(e) {}
    
    // This is a container of which you will allocate and refer to precisely one instance.
    // It is best to use new and keep a reference to it, in case an insert() leads to an overflow,
    // in which case you may want to point to a new, bigger box created with expand().
    rectpnode::rectpnode(): x(0),y(0), c(-1) { child[0] = NULL, child[1] = NULL; }
    rectpnode::rectpnode(int xx,int yy,int w,int h,rectpnode* c1,rectpnode* c2):
        x(xx), y(yy), wid(w), hgt(h), c(-1) { child[0] = c1, child[1]=c2; }
    void rectpnode::rect(int xx, int yy, int w, int h) { x=xx, y=yy, wid=w, hgt=h; }
    
    // Copies the content of a element `c` of pvrect array `boxes` into container `h`
    void rncopy(rectpnode *h, pvrect *boxes, unsigned char c)
    {
      boxes[c].x = h->x,   boxes[c].y = h->y;
      boxes[c].w = h->wid, boxes[c].h = h->hgt;
    }
    
    // Inserts a new node into container `who` using metrics obtained from `boxes`[`c`]
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
