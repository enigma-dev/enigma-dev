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
#include <stdio.h>
using namespace std;

#include "backgroundstruct.h"
#include "../Graphics_Systems/graphics_mandatory.h"
#include "../Widget_Systems/widgets_mandatory.h"
#include "../Platforms/platforms_mandatory.h"
#include "../libEGMstd.h"
#include "zlib.h"
#include "resinit.h"


namespace enigma
{
  void exe_loadbackgrounds(FILE *exe)
  {
    int nullhere;
	  unsigned bkgid, width, height,transparent,smoothEdges,preload,useAsTileset,tileWidth,tileHeight,hOffset,vOffset,hSep,vSep;
    
    if (!fread(&nullhere,4,1,exe) or nullhere != *(int*)"BKG ")
      return;
    
    // Determine how many backgrounds we have
    int bkgcount;
    if (!fread(&bkgcount,4,1,exe))
      return;
    
	  
	  // Fetch the highest ID we will be using
	  int bkg_highid;
	  if (!fread(&bkg_highid,4,1,exe))
	    return;
	  
	  printf("highestid: %d", bkg_highid);
	  
	  backgrounds_init();
	  
	  for (int i = 0; i < bkgcount; i++)
	  {
		  int unpacked;
		  if (!fread(&bkgid, 4,1,exe)) return;
		  if (!fread(&width, 4,1,exe)) return;
		  printf("width: %d", width);
		  if (!fread(&height,4,1,exe)) return;
		  printf("height: %d", height);
		  
		  if (!fread(&transparent,4,1,exe)) return;
		  printf("transparent: %d", transparent);
		  if (!fread(&smoothEdges,4,1,exe)) return;
		  printf("smoothEdges: %d", smoothEdges);
		  if (!fread(&preload,4,1,exe)) return;
		  printf("preload: %d", preload);
		  if (!fread(&useAsTileset,4,1,exe)) return;
		  printf("useAsTileset: %d", useAsTileset);
		  if (!fread(&tileWidth,4,1,exe)) return;
		  printf("tileWidth: %d", tileWidth);
		  if (!fread(&tileHeight,4,1,exe)) return;
		  printf("tileHeight: %d", tileHeight);
		  if (!fread(&hOffset,4,1,exe)) return;
		  printf("hOffset: %d", hOffset);
		  if (!fread(&vOffset,4,1,exe)) return;
		  printf("vOffset: %d", vOffset);
		  if (!fread(&hSep,4,1,exe)) return;
		  printf("hSep: %d", hSep);
		  if (!fread(&vSep,4,1,exe)) return;
		  printf("vSep: %d", vSep);
		  
		 
		  //need to add: transparent, smooth, preload, tileset, tileWidth, tileHeight, hOffset, vOffset, hSep, vSep
		  
		  unpacked = width*height*4;
		  
		  unsigned int size;
		  if (!fread(&size,4,1,exe)){}; 
		  printf("Alloc size: %d", size);
		  
		  unsigned char* cpixels=new unsigned char[size+1];
		  if (!cpixels)
		  {  
			  show_error("Failed to load background: Cannot allocate enough memory "+toString(unpacked),0);
			  break;
		  }
		  unsigned int sz2=fread(cpixels,1,size,exe);
		  if (size!=sz2) {
			  show_error("Failed to load background: Data is truncated before exe end. Read "+toString(sz2)+" out of expected "+toString(size),0);
			  return;
		  }
		  unsigned char* pixels=new unsigned char[unpacked+1];
		  if (zlib_decompress(cpixels,size,unpacked,pixels) != unpacked)
		  {
			  show_error("Background load error: Background does not match expected size",0);
			  continue;
		  }
		  delete[] cpixels;
		 
		  printf("Adding background: %d", i);
		  background_new(bkgid, width, height, pixels, false, false, true, false, 32, 32, 0, 0, 1,1);
		  
		  delete[] pixels;
	  }
  }
}
