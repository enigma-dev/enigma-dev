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
using namespace std;

#include "pathstruct.h"
//#include "../Platforms/platforms_mandatory.h"
//#include "../libEGMstd.h"

namespace enigma
{
  void exe_loadpaths(FILE *exe)
  {
    unsigned pathid, pointcount;
    bool smooth, closed;
    int x, y, speed, nullhere, precision;

    fread(&nullhere,4,1,exe);
    if (nullhere != *(int*)"PTH ")
      return;

    // Determine how many paths we have
    int pathcount;
    fread(&pathcount,4,1,exe);

    // Fetch the highest ID we will be using
    int path_highid, buf;
    fread(&path_highid,4,1,exe);
    paths_init();

    for (int i = 0; i < pathcount; i++)
    {
      fread(&pathid, 4,1,exe);
      fread(&buf, 4,1,exe);
      smooth = buf; //to fix int to bool issues
      fread(&buf, 4,1,exe);
      closed = buf;
      fread(&precision, 4,1,exe);

      fread(&pointcount,4,1,exe);

      new path(pathid, smooth, closed, precision, pointcount);
      for (int ii=0;ii<pointcount;ii++)
      {
        fread(&x, 4,1,exe);
        fread(&y, 4,1,exe);
        fread(&speed, 4,1,exe);
        path_add_point(pathid, x, y, speed/100);
      }
      path_recalculate(pathid);
    }
  }
}
