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

#include <string>
#include <stdio.h>
using namespace std;

#include "spritestruct.h"
#include "../Platforms/platforms_mandatory.h"
#include "../Graphics_Systems/graphics_mandatory.h"
#include "../Widget_Systems/widgets_mandatory.h"
#include "../libEGMstd.h"
#include "zlib.h"
#include "resinit.h"

namespace enigma
{
  void exe_loadsprs(FILE *exe)
  {
    int nullhere;
    unsigned sprid, width, height, bbt, bbb, bbl, bbr;
    int xorig, yorig;
    
    if (!fread(&nullhere,4,1,exe)) return;
    if (nullhere != *(int*)"SPR ")
      return;
    
    // Determine how many sprites we have
    int sprcount;
    if (!fread(&sprcount,4,1,exe)) return;
    
    // Fetch the highest ID we will be using
    int spr_highid;
    if (!fread(&spr_highid,4,1,exe)) return;
    sprites_init();
    
    for (int i = 0; i < sprcount; i++)
    {
      if (!fread(&sprid, 4,1,exe)) return;
      if (!fread(&width, 4,1,exe)) return;
      if (!fread(&height,4,1,exe)) return;
      if (!fread(&xorig, 4,1,exe)) return;
      if (!fread(&yorig, 4,1,exe)) return;
      if (!fread(&bbt, 4,1,exe)) return;
      if (!fread(&bbb, 4,1,exe)) return;
      if (!fread(&bbl, 4,1,exe)) return;
      if (!fread(&bbr, 4,1,exe)) return;
      
      int subimages;
      if (!fread(&subimages,4,1,exe)) return; //co//ut << "Subimages: " << subimages << endl;
      
      sprite_new_empty(sprid, subimages, width, height, xorig, yorig, bbt, bbb, bbl, bbr, 1,0);
      for (int ii=0;ii<subimages;ii++) 
      {
        int unpacked;
        if (!fread(&unpacked,4,1,exe)) return;
        unsigned int size;
        if (!fread(&size,4,1,exe)) return; //co//ut << "Alloc size: " << size << endl;
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
        //co//ut << "Adding subimage...\n";
        sprite_set_subimage(sprid, ii, xorig, yorig, width, height, pixels);
        //co//ut << "...done\n";
        
        delete[] pixels;
        if (!fread(&nullhere,4,1,exe)) return;
        
        if (nullhere)
        {
          show_error("Sprite load error: Null terminator expected",0);
          break;
        }
      }
    }
  }
}
