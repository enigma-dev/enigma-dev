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

#include <string>
#include <string.h>
using namespace std;

#include "../Graphics_Systems/graphics_mandatory.h"
#include "spritestruct.h"
#include "../libEGMstd.h"

#include "backgroundstruct.h"

inline unsigned int nlpo2dc(unsigned int x) //Next largest power of two minus one
{
	x |= x>>1;
	x |= x>>2;
	x |= x>>4;
	x |= x>>8;
	return x | (x>>16);
}

namespace enigma
{
  background::background(): 
    tileset(false) {}
  background::background(const bool ts): 
    tileset(ts) {}
  background::background(int w,int h,unsigned tex,bool trans,bool smth,bool prel): 
    width(w), height(h), texture(tex), transparent(trans), smooth(smth), preload(prel), tileset(false) {}
  background::background(const bool ts,int w,int h,unsigned tex,bool trans,bool smth,bool prel): 
    width(w), height(h), texture(tex), transparent(trans), smooth(smth), preload(prel), tileset(ts) {}
  
  background_tileset::background_tileset(): 
    background(true) {}
  background_tileset::background_tileset(int tw, int th, int ho, int vo, int hs, int vs):
    background(true), tileWidth(tw), tileHeight(th), hOffset(ho), vOffset(vo), hSep(hs), vSep(vs) {}
  background_tileset::background_tileset(int w,int h,unsigned tex,bool trans,bool smth,bool prel,int tw, int th, int ho, int vo, int hs, int vs):
    background(true, w, h, tex, trans, smth, prel), tileWidth(tw), tileHeight(th), hOffset(ho), vOffset(vo), hSep(hs), vSep(vs) {}
  
  //Adds a subimage to an existing sprite from the exe
  void background_new(int bkgid, int w, int h, char* chunk, bool transparent, bool smoothEdges, bool preload, bool useAsTileset, int tileWidth, int tileHeight, int hOffset, int vOffset, int hSep, int vSep)
  {
    unsigned int fullwidth = nlpo2dc(w)+1, fullheight = nlpo2dc(h);
    char *imgpxdata = new char[4*fullwidth*fullheight+1], *imgpxptr = imgpxdata;
    unsigned int rowindex,colindex;
    for (rowindex = 0; rowindex < h; rowindex++)
    {
      for(colindex = 0; colindex < w; colindex++)
      {
        *imgpxptr++ = *chunk++;
        *imgpxptr++ = *chunk++;
        *imgpxptr++ = *chunk++;
        *imgpxptr++ = *chunk++;
      }
      memset(imgpxptr, 0, (fullwidth-colindex) << 2);
      imgpxptr += (fullwidth-colindex) << 2;
    }
    memset(imgpxptr,0,(fullheight-h) * fullwidth);
    
    unsigned texture = graphics_create_texture(fullwidth,fullheight,imgpxdata);
    delete[] imgpxdata;
    
    background* bkgstr = backgroundarray[bkgid] = useAsTileset ? new background(w,h,texture,transparent,smoothEdges,preload) : new background_tileset(w,h,texture,transparent,smoothEdges,preload,tileWidth, tileHeight, hOffset, vOffset, hSep, vSep);  
  }
}
