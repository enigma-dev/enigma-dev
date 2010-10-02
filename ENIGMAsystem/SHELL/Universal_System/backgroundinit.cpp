/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>                      **
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
#include <stdio.h>
using namespace std;

#include "backgroundstruct.h"
#include "../Graphics_Systems/graphics_mandatory.h"
#include "../Platforms/platforms_mandatory.h"
#include "../libEGMstd.h"
#include "compression.h"


namespace enigma
{
  void exe_loadbackgrounds(FILE *exe)
  {
    int nullhere;
	  unsigned bkgid, width, height,transparent,smoothEdges,preload,useAsTileset,tileWidth,tileHeight,hOffset,vOffset,hSep,vSep;
    
    fread(&nullhere,4,1,exe);
    if (nullhere != *(int*)"bkgn")
      return;
    
    // Determine how many backgrounds we have
    int bkgcount;
    fread(&bkgcount,4,1,exe);
    
	  
	  
	  // Fetch the highest ID we will be using
	  int bkg_highid;
	  fread(&bkg_highid,4,1,exe);
	  
	  printf("highestid: %d", bkg_highid);
	  
	  backgrounds_init();
	  
	  for (int i = 0; i < bkgcount; i++)
	  {
		  int unpacked;
		  fread(&bkgid, 4,1,exe);
		  fread(&width, 4,1,exe);
		  printf("width: %d", width);
		  fread(&height,4,1,exe);
		  printf("height: %d", height);
		  
		  fread(&transparent,4,1,exe);
		  printf("transparent: %d", transparent);
		  fread(&smoothEdges,4,1,exe);
		  printf("smoothEdges: %d", smoothEdges);
		  fread(&preload,4,1,exe);
		  printf("preload: %d", preload);
		  fread(&useAsTileset,4,1,exe);
		  printf("useAsTileset: %d", useAsTileset);
		  fread(&tileWidth,4,1,exe);
		  printf("tileWidth: %d", tileWidth);
		  fread(&tileHeight,4,1,exe);
		  printf("tileHeight: %d", tileHeight);
		  fread(&hOffset,4,1,exe);
		  printf("hOffset: %d", hOffset);
		  fread(&vOffset,4,1,exe);
		  printf("vOffset: %d", vOffset);
		  fread(&hSep,4,1,exe);
		  printf("hSep: %d", hSep);
		  fread(&vSep,4,1,exe);
		  printf("vSep: %d", vSep);
		  
		 
		  //need to add: transparent, smooth, preload, tileset, tileWidth, tileHeight, hOffset, vOffset, hSep, vSep
		  
		  unpacked = width*height*4;
		  
		  unsigned int size;
		  fread(&size,4,1,exe); 
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
