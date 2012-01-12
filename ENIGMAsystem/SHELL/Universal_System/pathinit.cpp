/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Harijs Grînbergs                                         **
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

//#include <string>
#include <stdio.h>
//using namespace std;

#include "pathstruct.h"
#include "resinit.h"
//#include "../Platforms/platforms_mandatory.h"
//#include "../libEGMstd.h"

namespace enigma
{
  void exe_loadpaths(FILE *exe)
  {
    unsigned pathid, pointcount;
    bool smooth, closed;
    int x, y, speed, nullhere, precision;

    if (!fread(&nullhere,4,1,exe)) return;
    if (nullhere != *(int*)"PTH ")
      return;

    // Determine how many paths we have
    int pathcount;
    if (!fread(&pathcount,4,1,exe)) return;

    // Fetch the highest ID we will be using
    int path_highid, buf;
    if (!fread(&path_highid,4,1,exe)) return;
    paths_init();

    for (int i = 0; i < pathcount; i++)
    {
      if (!fread(&pathid, 4,1,exe)) return;
      if (!fread(&buf, 4,1,exe)) return;
      smooth = buf; //to fix int to bool issues
      if (!fread(&buf, 4,1,exe)) return;
      closed = buf;
      if (!fread(&precision, 4,1,exe)) return;

      if (!fread(&pointcount,4,1,exe)) return;

      new path(pathid, smooth, closed, precision, pointcount);
      for (unsigned ii=0;ii<pointcount;ii++)
      {
        if (!fread(&x, 4,1,exe)) return;
        if (!fread(&y, 4,1,exe)) return;
        if (!fread(&speed, 4,1,exe)) return;
        path_add_point(pathid, x, y, speed/100);
      }
      path_recalculate(pathid);
    }
  }
}
