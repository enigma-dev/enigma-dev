/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>
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

#include <string>
#include <cstring>
using namespace std;

#include "../Graphics_Systems/graphics_mandatory.h"
#include "../libEGMstd.h"

#include "backgroundstruct.h"


namespace enigma {
	background** backgroundstructarray;
	extern size_t background_idmax;
}


/* These functions are primarily for use of the engine. Experienced users
 * who are familiar with C++ can make use of these, but they were made to
 * use at load time with data read from the executable. These both expect
 * RAW format, RGB only.
 */
#include "nlpo2.h"

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
  void background_new(int bkgid, unsigned w, unsigned h, unsigned char* chunk, bool transparent, bool smoothEdges, bool preload, bool useAsTileset, int tileWidth, int tileHeight, int hOffset, int vOffset, int hSep, int vSep)
  {
    unsigned int fullwidth = nlpo2dc(w)+1, fullheight = nlpo2dc(h)+1;
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
    
   backgroundstructarray[bkgid] = useAsTileset ? new background(w,h,texture,transparent,smoothEdges,preload) : new background_tileset(w,h,texture,transparent,smoothEdges,preload,tileWidth, tileHeight, hOffset, vOffset, hSep, vSep);  
	  enigma::background *bak = backgroundstructarray[bkgid];
	  bak->texbordx  = (double) w/fullwidth;
	  bak->texbordy  = (double) h/fullheight;
	  
  }
	
	//Allocates and zero-fills the array at game start
	void backgrounds_init()
	{
		backgroundstructarray = new background*[enigma::background_idmax+1];
		for (unsigned i = 0; i < enigma::background_idmax; i++)
			backgroundstructarray[i] = NULL;
	}
}
