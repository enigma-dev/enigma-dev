/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 IsmAvatar <IsmAvatar@gmail.com>                          **
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

int module_write_paths(EnigmaStruct *es, FILE *gameModule)
{
  // Now we're going to add paths
  edbg << es->pathCount << " Adding Fonts to Game Module: " << flushl;

  //Magic Number
  fwrite("PTH ",4,1,gameModule);

  //Indicate how many
  int path_count = es->pathCount;
  fwrite(&path_count,4,1,gameModule);

  int path_maxid = 0;
  for (int i = 0; i < path_count; i++)
    if (es->paths[i].id > path_maxid)
      path_maxid = es->paths[i].id;
  fwrite(&path_maxid,4,1,gameModule);

  for (int i = 0; i < path_count; i++)
  {
    writei(es->paths[i].id,gameModule); //id
    
    writei(es->paths[i].smooth,gameModule);
    writei(es->paths[i].closed,gameModule);
    writei(es->paths[i].precision,gameModule);
    // possibly snapX/Y?

    // Track how many path points we're copying
    int pointCount = es->paths[i].pointCount;

    for (int ii = 0; ii < pointCount; ii++)
    {
      writei(es->paths[i].points[ii].x,gameModule);
      writei(es->paths[i].points[ii].y,gameModule);
      writei(es->paths[i].points[ii].speed,gameModule);
    }
  }

  edbg << "Done writing paths." << flushl;
  return 0;
}
