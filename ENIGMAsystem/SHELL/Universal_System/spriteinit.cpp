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

#include "../Graphics_Systems/OpenGL/GSspriteadd.h"
#include "../Platforms/platforms_mandatory.h"
#include "../libEGMstd.h"
#include "compression.h"

using namespace std;

inline string tostring(int x)
{
  char a[20];
  sprintf(a,"%d",x);
  return a;
}

namespace enigma
{
  void exe_loadsprs()
  {
    int nullhere;
    char exename[1025];
    windowsystem_write_exename(exename);
    FILE* exe=fopen(exename,"rb");
    if (!exe) {
      show_error("Resource load fail: exe unopenable",0);
      return;
    }
    
    // Read the magic number so we know we're looking at our own data
    fseek(exe,-8,SEEK_END);
    char str_quad[4];
    fread(str_quad,1,4,exe);
    if (str_quad[0] != 's' or str_quad[1] != 'p' or str_quad[2] != 'r' or str_quad[3] != 'n') {
      printf("No resource data in exe\n");
      sprite_safety_override();
      return;
    }
    
    // Get where our resources are located in the module
    int pos;
    fread(&pos,4,1,exe);
    unsigned sprid, width, height;
    int xorig, yorig;
    
    // Go to the start of the resource data
    fseek(exe,pos,SEEK_SET);
    fread(&nullhere,4,1,exe);
    if(nullhere) return;
    
    // Determine how many sprites we have
    int sprcount;
    fread(&sprcount,4,1,exe);
    
    // Fetch the highest ID we will be using
    int spr_highid;
    fread(&spr_highid,4,1,exe);
    sprites_allocate_initial(spr_highid);
    
    for (int i=0;i<sprcount;i++)
    {
      fread(&sprid,4,1,exe);
      fread(&width,4,1,exe);
      fread(&height,4,1,exe);
      fread(&xorig,4,1,exe);
      fread(&yorig,4,1,exe);
      
      int subimages;
      fread(&subimages,4,1,exe); cout << "Subimages: " << subimages << endl;
      
      
      new_sprexe(sprid,subimages,width,height,xorig,yorig,1,0);
      for (int ii=0;ii<subimages;ii++) 
      {
        int unpacked;
        fread(&unpacked,1,4,exe);
        unsigned int size;
        fread(&size,4,1,exe); cout << "Alloc size: " << size << endl;
        unsigned char* cpixels=new unsigned char[size+1];
        if (!cpixels)
        {  //FIXME: Uncomment these when tostring is available
          show_error("Failed to load sprite: Cannot allocate enough memory "+tostring(unpacked),0);
          break;
        }
        unsigned int sz2=fread(cpixels,1,size,exe);
        if (size!=sz2)
        {
          show_error("Failed to load sprite: Data is truncated before exe end. Read "+tostring(sz2)+" out of expected "+tostring(size),0);
          goto break2;
        }
        unsigned char* pixels=new unsigned char[unpacked+1];
        if (zlib_decompress(cpixels,size,unpacked,pixels) != unpacked)
        {
          show_error("Sprite load error: Sprite does not match expected size",0);
          continue;
        }
        delete[] cpixels;
        cout << "Adding subimage...\n";
        sprexe(sprid, xorig, yorig, width, height, pixels);
        
        delete[] pixels;
        fread(&nullhere,1,4,exe);
        
        if (nullhere)
        {
          show_error("Sprite load error: Null terminator expected",0);
          break;
        }
      }
    }
    break2:
    #if SHOWERRORS
      fread(&nullhere,1,4,exe);
      if(nullhere) show_error("Sprite load error: Final null terminator expected, world cannot end",0);
    #endif
    fclose(exe);
  }
}
