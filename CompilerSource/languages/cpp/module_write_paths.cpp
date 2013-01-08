/**
  @file  module_write_backgrounds.cpp
  @brief Implements the method in the C++ plugin that writes backgrounds to a game's
         resource section.
  
  @section License
    Copyright (C) 2011 IsmAvatar <IsmAvatar@gmail.com>
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

int lang_CPP::resource_writer_cpp::module_write_paths(compile_context &ctex)
{
  // Now we're going to add paths
  edbg << ctex.es->pathCount << " Adding Paths to Game Module: " << flushl;

  //Magic Number
  fwrite("PTH ",4,1,gameModule);

  //Indicate how many
  int path_count = ctex.es->pathCount;
  fwrite(&path_count,4,1,gameModule);

  int path_maxid = 0;
  for (int i = 0; i < path_count; i++)
    if (ctex.es->paths[i].id > path_maxid)
      path_maxid = ctex.es->paths[i].id;
  fwrite(&path_maxid,4,1,gameModule);

  for (int i = 0; i < path_count; i++)
  {
    writei(ctex.es->paths[i].id,gameModule); //id

    writei(ctex.es->paths[i].smooth,gameModule);
    writei(ctex.es->paths[i].closed,gameModule);
    writei(ctex.es->paths[i].precision,gameModule);
    // possibly snapX/Y?

    // Track how many path points we're copying
    int pointCount = ctex.es->paths[i].pointCount;
    writei(pointCount,gameModule);

    for (int ii = 0; ii < pointCount; ii++)
    {
      writei(ctex.es->paths[i].points[ii].x,gameModule);
      writei(ctex.es->paths[i].points[ii].y,gameModule);
      writei(ctex.es->paths[i].points[ii].speed,gameModule);
    }
  }

  edbg << "Done writing paths." << flushl;
  return 0;
}
