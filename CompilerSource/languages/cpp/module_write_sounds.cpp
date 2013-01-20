/**
  @file  module_write_sounds.cpp
  @brief Implements the method that writes sounds to the resource section of a game
         built with the C++ plugin.
  
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

int lang_CPP::resource_writer_cpp::module_write_sounds(compile_context &ctex)
{
  // Now we're going to add sounds
  edbg << ctex.es->soundCount << " Sounds:" << flushl;
  for (int i = 0; i < ctex.es->soundCount; i++) {
    edbg << " " << ctex.es->sounds[i].name << flushl;
    fflush(stdout);
  }
  
  //Magic number
  fwrite("SND ",4,1,gameModule);
  
  //Indicate how many
  int sound_count = ctex.es->soundCount;
  fwrite(&sound_count,4,1,gameModule);
  
  int sound_maxid = 0;
  for (int i = 0; i < sound_count; i++)
    if (ctex.es->sounds[i].id > sound_maxid)
      sound_maxid = ctex.es->sounds[i].id;
  fwrite(&sound_maxid,4,1,gameModule);
  
  for (int i = 0; i < sound_count; i++)
  {
    unsigned sndsz = ctex.es->sounds[i].size;
    if (!sndsz) {
      user << "Sound `" << ctex.es->sounds[i].name << "' has no size. It will be omitted from the game." << flushl;
      continue;
    }
    
    writei(ctex.es->sounds[i].id, gameModule); // id
    writei(sndsz, gameModule); // Size
    fwrite(ctex.es->sounds[i].data, 1, sndsz, gameModule); // Sound data
  }
 
  edbg << "Done writing sounds." << flushl;
  return 0;
}
