/**
  @file  module_write_backgrounds.cpp
  @brief Implements the method in the C++ plugin that writes backgrounds to a game's
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
#include "languages/lang_CPP.h"

inline void writei(int x, FILE *f) {
  fwrite(&x,4,1,f);
}

int lang_CPP::resource_writer_cpp::module_write_backgrounds(compile_context &ctex)
{
  edbg << ctex.es->backgroundCount << " Adding Backgrounds to Game Module: " << flushl;

  //Magic Number
  fwrite("BKG ",4,1,gameModule);
  
  //Indicate how many
  int back_count = ctex.es->backgroundCount;
  fwrite(&back_count,4,1,gameModule);
  
  int back_maxid = 0;
  for (int i = 0; i < back_count; i++)
    if (ctex.es->backgrounds[i].id > back_maxid)
      back_maxid = ctex.es->backgrounds[i].id;
  fwrite(&back_maxid,4,1,gameModule);
  
  for (int i = 0; i < back_count; i++)
  {
    writei(ctex.es->backgrounds[i].id,gameModule); //id
    writei(ctex.es->backgrounds[i].backgroundImage.width, gameModule); // width
    writei(ctex.es->backgrounds[i].backgroundImage.height, gameModule); // height


    writei(ctex.es->backgrounds[i].transparent,gameModule);
    writei(ctex.es->backgrounds[i].smoothEdges,gameModule);
    writei(ctex.es->backgrounds[i].preload,gameModule);
    writei(ctex.es->backgrounds[i].useAsTileset,gameModule);
    writei(ctex.es->backgrounds[i].tileWidth,gameModule);
    writei(ctex.es->backgrounds[i].tileHeight,gameModule);
    writei(ctex.es->backgrounds[i].hOffset,gameModule);
    writei(ctex.es->backgrounds[i].vOffset,gameModule);
    writei(ctex.es->backgrounds[i].hSep,gameModule);
    writei(ctex.es->backgrounds[i].vSep,gameModule);

    const int sz = ctex.es->backgrounds[i].backgroundImage.dataSize;
    writei(sz, gameModule); // size
    fwrite(ctex.es->backgrounds[i].backgroundImage.data, 1, sz, gameModule); // data
  }

  edbg << "Done writing backgrounds." << flushl;
  return 0;
}
