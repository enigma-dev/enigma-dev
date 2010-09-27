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

int module_write_backgrounds(EnigmaStruct *es, FILE *gameModule)
{
  // Now we're going to add backgrounds
  edbg << es->backgroundCount << " Adding Backgrounds to Game Module: " << flushl;

  //Magic Number
  fwrite("bkgn",4,1,gameModule);

  //Indicate how many
  int back_count = es->backgroundCount;
  fwrite(&back_count,4,1,gameModule);

  int back_maxid = 0;
  for (int i = 0; i < back_count; i++)
    if (es->backgrounds[i].id > back_maxid)
      back_maxid = es->backgrounds[i].id;
  fwrite(&back_maxid,4,1,gameModule);

  for (int i = 0; i < back_count; i++)
  {
      writei(es->backgrounds[i].id,gameModule); //id
    writei(es->backgrounds[i].backgroundImage.width, gameModule); // width
    writei(es->backgrounds[i].backgroundImage.height, gameModule); // height
    //need to add: transparent, smooth, preload, tileset, tileWidth, tileHeight, hOffset, vOffset, hSep, vSep

    const int sz = es->backgrounds[i].backgroundImage.dataSize;
    writei(sz, gameModule); // size
    fwrite(es->backgrounds[i].backgroundImage.data, 1, sz, gameModule); // data
  }

  edbg << "Done writing backgrounds." << flushl;
  return 0;
}
