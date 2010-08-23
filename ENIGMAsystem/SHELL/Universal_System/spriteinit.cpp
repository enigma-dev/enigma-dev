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
#include <stdio.h>
#include <iostream>
using namespace std;

#include "../Graphics_Systems/graphics_mandatory.h"
#include "../Platforms/platforms_mandatory.h"
#include "../libEGMstd.h"
#include "compression.h"

namespace enigma
{
  void exe_loadsprs(FILE *exe)
  {
    int nullhere;
    unsigned sprid, width, height;
    int xorig, yorig;
    
    fread(&nullhere,4,1,exe);
    if (nullhere != *(int*)"sprn")
      return;
    
    // Determine how many sprites we have
    int sprcount;
    fread(&sprcount,4,1,exe);
    
    // Fetch the highest ID we will be using
    int spr_highid;
    fread(&spr_highid,4,1,exe);
    sprites_init();
    
    for (int i = 0; i < sprcount; i++)
    {
      fread(&sprid, 4,1,exe);
      fread(&width, 4,1,exe);
      fread(&height,4,1,exe);
      fread(&xorig, 4,1,exe);
      fread(&yorig, 4,1,exe);
      
      int subimages;
      fread(&subimages,4,1,exe); cout << "Subimages: " << subimages << endl;
      
      
      sprite_new_empty(sprid,subimages,width,height,xorig,yorig,1,0);
      for (int ii=0;ii<subimages;ii++) 
      {
        int unpacked;
        fread(&unpacked,1,4,exe);
        unsigned int size;
        fread(&size,4,1,exe); cout << "Alloc size: " << size << endl;
        unsigned char* cpixels=new unsigned char[size+1];
        if (!cpixels)
        {  //FIXME: Uncomment these when tostring is available
          show_error("Failed to load sprite: Cannot allocate enough memory "+toString(unpacked),0);
          break;
        }
        unsigned int sz2=fread(cpixels,1,size,exe);
        if (size!=sz2) {
          show_error("Failed to load sprite: Data is truncated before exe end. Read "+toString(sz2)+" out of expected "+toString(size),0);
          return;
        }
        unsigned char* pixels=new unsigned char[unpacked+1];
        if (zlib_decompress(cpixels,size,unpacked,pixels) != unpacked)
        {
          show_error("Sprite load error: Sprite does not match expected size",0);
          continue;
        }
        delete[] cpixels;
        cout << "Adding subimage...\n";
        sprite_add_subimage(sprid, xorig, yorig, width, height, pixels);
        
        delete[] pixels;
        fread(&nullhere,1,4,exe);
        
        if (nullhere)
        {
          show_error("Sprite load error: Null terminator expected",0);
          break;
        }
      }
    }
  }
}
