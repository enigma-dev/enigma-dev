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

#include <map>
#include "var4.h"
#include "resource_data.h"
using namespace std;

static map<string,int> resources;
namespace enigma
{
  void map_resource_ids(nameid_pair* n)
  {
    for (nameid_pair* i = n; i->id != -1; i++)
    {
      //TODO: IMPLEMENT
    }
  }
}

int resource_get_id(string name)
{
  return -1;
}

variant script_execute(int scr, variant arg0, variant arg1, variant arg2, variant arg3, variant arg4, variant arg5, variant arg6, variant arg7, variant arg8, variant arg9, variant arg10, variant arg11, variant arg12, variant arg13, variant arg14, variant arg15)
{
  enigma::callable_script &i = enigma::callable_scripts[scr];
  switch (i.argnum)
  {
    case 0:  return ((variant(*)())i.base)                                                                                                                                  ();
    case 1:  return ((variant(*)(variant))i.base)                                                                                                                           (arg0);
    case 2:  return ((variant(*)(variant,variant))i.base)                                                                                                                   (arg0,arg1);
    case 3:  return ((variant(*)(variant,variant,variant))i.base)                                                                                                           (arg0,arg1,arg2);
    case 4:  return ((variant(*)(variant,variant,variant,variant))i.base)                                                                                                   (arg0,arg1,arg2,arg3);
    case 5:  return ((variant(*)(variant,variant,variant,variant,variant))i.base)                                                                                           (arg0,arg1,arg2,arg3,arg4);
    case 6:  return ((variant(*)(variant,variant,variant,variant,variant,variant))i.base)                                                                                   (arg0,arg1,arg2,arg3,arg4,arg5);
    case 7:  return ((variant(*)(variant,variant,variant,variant,variant,variant,variant))i.base)                                                                           (arg0,arg1,arg2,arg3,arg4,arg5,arg6);
    case 8:  return ((variant(*)(variant,variant,variant,variant,variant,variant,variant,variant))i.base)                                                                   (arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7);
    case 9:  return ((variant(*)(variant,variant,variant,variant,variant,variant,variant,variant,variant))i.base)                                                           (arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8);
    case 10: return ((variant(*)(variant,variant,variant,variant,variant,variant,variant,variant,variant,variant))i.base)                                                   (arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9);
    case 11: return ((variant(*)(variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant))i.base)                                           (arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10);
    case 12: return ((variant(*)(variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant))i.base)                                   (arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11);
    case 13: return ((variant(*)(variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant))i.base)                           (arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12);
    case 14: return ((variant(*)(variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant))i.base)                   (arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12,arg13);
    case 15: return ((variant(*)(variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant))i.base)           (arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12,arg13,arg14);
    case 16: return ((variant(*)(variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant,variant))i.base)   (arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12,arg13,arg14,arg15);
  }
  return 0;
}
