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

/**
  @file    object.h
  @summary This file outlines the most basic local variables needed by different systems
*/

#ifndef _object_h
#define _object_h

const int self =   -1;
const int other =  -2;
const int all =    -3;
const int noone =  -4;
const int global = -5;
const int local =  -7;

#include "var4.h"

namespace enigma
{
    extern int maxid;
    extern int instancecount;
    extern int id_current;

    extern double newinst_x, newinst_y;
    extern int newinst_obj, newinst_id;

    struct object_basic
    {
      const unsigned id;
      const int object_index;
      
      virtual void unlink();
      virtual variant myevent_draw();
      virtual variant myevent_roomend();
      virtual variant myevent_destroy();
      
      object_basic();
      object_basic(int uid, int uoid);
      virtual ~object_basic();
    };
}

namespace enigma
{
    void constructor(object_basic* instance);
}

#endif //_object_h
