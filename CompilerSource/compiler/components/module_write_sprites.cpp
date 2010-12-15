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

#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

#include "../../externs/externs.h"
#include "../../syntax/syncheck.h"
#include "../../parser/parser.h"

#include "../../backend/EnigmaStruct.h" //LateralGM interface structures
#include "../../parser/object_storage.h"
#include "../compile_common.h"

#include "../../backend/ideprint.h"

inline void writei(int x, FILE *f) {
  fwrite(&x,4,1,f);
}

int module_write_sprites(EnigmaStruct *es, FILE *gameModule)
{
  // Now we're going to add sprites
  edbg << es->spriteCount << " Adding Sprites to Game Module: " << flushl;
  
  //Magic Number
  fwrite("sprn",4,1,gameModule);
  
  //Indicate how many
  int sprite_count = es->spriteCount;
  fwrite(&sprite_count,4,1,gameModule);
  
  int sprite_maxid = 0;
  for (int i = 0; i < sprite_count; i++)
    if (es->sprites[i].id > sprite_maxid)
      sprite_maxid = es->sprites[i].id;
  fwrite(&sprite_maxid,4,1,gameModule);
  
  for (int i = 0; i < sprite_count; i++)
  {
    writei(es->sprites[i].id,gameModule); //id
    
    // Track how many subImages we're copying
    int subCount = es->sprites[i].subImageCount;
    
    int swidth = 0, sheight = 0;
    for (int ii = 0; ii < subCount; ii++)
    {
      if (!swidth and !sheight) {
        swidth =  es->sprites[i].subImages[ii].image.width;
        sheight = es->sprites[i].subImages[ii].image.height;
      }
      else if (swidth != es->sprites[i].subImages[ii].image.width or sheight != es->sprites[i].subImages[ii].image.height) {
        user << "Subimages of sprite `" << es->sprites[i].name << "' vary in dimensions; do not want." << flushl;
        return 14;
      }
    }
    if (!(swidth and sheight and subCount)) {
      user << "Subimages of sprite `" << es->sprites[i].name << "' have zero size." << flushl;
      return 14;
    }
    
    writei(swidth, gameModule); //width
    writei(sheight,gameModule); //height
    writei(es->sprites[i].originX,gameModule); //xorig
    writei(es->sprites[i].originY,gameModule); //yorig
    
    writei(subCount,gameModule); //subimages
    
    for (int ii = 0;ii < subCount; ii++)
    {
      //strans = es->sprites[i].subImages[ii].transColor, fwrite(&idttrans,4,1,exe); //Transparent color
      writei(swidth * sheight * 4,gameModule); //size when unpacked
      writei(es->sprites[i].subImages[ii].image.dataSize,gameModule); //size when unpacked
      fwrite(es->sprites[i].subImages[ii].image.data, 1, es->sprites[i].subImages[ii].image.dataSize, gameModule); //sprite data
      writei(0,gameModule);
    }
  }
 
  edbg << "Done writing sprites." << flushl;
  return 0;
}
