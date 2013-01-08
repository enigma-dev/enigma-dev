/**
  @file  module_write_sprites.cpp
  @brief Implements the method in the C++ plugin that writes sprites to a game's
         resource section.
  
  @section License
    Copyright (C) 2008-2013 Josh Ventura
    This file is a part of the ENIGMA Development Environment.

    ENIGMA is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, version 3 of the license or any later version.

    This application and its source code is distributed AS-IS, WITHOUT ANY WARRANTY; 
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

    You should have recieved a copy of the GNU General Public License along
    with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;


#include "syntax/syncheck.h"
#include "parser/parser.h"

#include "backend/EnigmaStruct.h" //LateralGM interface structures
#include "parser/object_storage.h"
#include "compiler/compile_common.h"

#include "backend/ideprint.h"

inline void writei(int x, FILE *f) {
  fwrite(&x,4,1,f);
}

#include "languages/lang_CPP.h"
int lang_CPP::resource_writer_cpp::module_write_sprites(compile_context &ctex)
{
  // Now we're going to add sprites
  edbg << ctex.es->spriteCount << " Adding Sprites to Game Module: " << flushl;
  
  //Magic Number
  fwrite("SPR ",4,1,gameModule);
  
  //Indicate how many
  int sprite_count = ctex.es->spriteCount;
  fwrite(&sprite_count,4,1,gameModule);
  
  int sprite_maxid = 0;
  for (int i = 0; i < sprite_count; i++)
    if (ctex.es->sprites[i].id > sprite_maxid)
      sprite_maxid = ctex.es->sprites[i].id;
  fwrite(&sprite_maxid,4,1,gameModule);
  
  for (int i = 0; i < sprite_count; i++)
  {
    writei(ctex.es->sprites[i].id,gameModule); //id
    
    // Track how many subImages we're copying
    int subCount = ctex.es->sprites[i].subImageCount;
    
    int swidth = 0, sheight = 0;
    for (int ii = 0; ii < subCount; ii++)
    {
      if (!swidth and !sheight) {
        swidth =  ctex.es->sprites[i].subImages[ii].image.width;
        sheight = ctex.es->sprites[i].subImages[ii].image.height;
      }
      else if (swidth != ctex.es->sprites[i].subImages[ii].image.width or sheight != ctex.es->sprites[i].subImages[ii].image.height) {
        user << "Subimages of sprite `" << ctex.es->sprites[i].name << "' vary in dimensions; do not want." << flushl;
        return 14;
      }
    }
    if (!(swidth and sheight and subCount)) {
      user << "Subimages of sprite `" << ctex.es->sprites[i].name << "' have zero size." << flushl;
      return 14;
    }
    
    writei(swidth, gameModule); //width
    writei(sheight,gameModule); //height
    writei(ctex.es->sprites[i].originX,gameModule); //xorig
    writei(ctex.es->sprites[i].originY,gameModule); //yorig
    writei(ctex.es->sprites[i].bbTop,gameModule);    //BBox Top
    writei(ctex.es->sprites[i].bbBottom,gameModule); //BBox Bottom
    writei(ctex.es->sprites[i].bbLeft,gameModule);   //BBox Left
    writei(ctex.es->sprites[i].bbRight,gameModule);  //BBox Right
    writei(ctex.es->sprites[i].shape,gameModule);  //Mask shape
    
    writei(subCount,gameModule); //subimages
    
    for (int ii = 0;ii < subCount; ii++)
    {
      //strans = ctex.es->sprites[i].subImages[ii].transColor, fwrite(&idttrans,4,1,exe); //Transparent color
      writei(swidth * sheight * 4,gameModule); //size when unpacked
      writei(ctex.es->sprites[i].subImages[ii].image.dataSize,gameModule); //size when unpacked
      fwrite(ctex.es->sprites[i].subImages[ii].image.data, 1, ctex.es->sprites[i].subImages[ii].image.dataSize, gameModule); //sprite data
      writei(0,gameModule);
    }
  }
 
  edbg << "Done writing sprites." << flushl;
  return 0;
}
