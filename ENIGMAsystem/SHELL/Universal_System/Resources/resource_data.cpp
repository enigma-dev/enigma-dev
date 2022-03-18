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
#include "Universal_System/var4.h"
#include "resource_data.h"
using namespace std;

static map<string,int> resources;
namespace enigma
{
  extern size_t script_idmax;
  void map_resource_ids(nameid_pair* n)
  {
    for (nameid_pair* i = n; i->id != -1; i++)
    {
      //TODO: IMPLEMENT
    }
  }
}

namespace enigma_user
{

int resource_get_id(string name)
{
  return -1;
}

evariant script_execute(int scr, evariant arg0, evariant arg1, evariant arg2, evariant arg3, evariant arg4, evariant arg5, evariant arg6, evariant arg7, evariant arg8, evariant arg9, evariant arg10, evariant arg11, evariant arg12, evariant arg13, evariant arg14, evariant arg15)
{
  enigma::callable_script &i = enigma::callable_scripts[scr];
  switch (i.argnum)
  {
    case 0:  return ((evariant(*)())i.base)                                                                                                                                  ();
    case 1:  return ((evariant(*)(evariant))i.base)                                                                                                                           (arg0);
    case 2:  return ((evariant(*)(evariant,evariant))i.base)                                                                                                                   (arg0,arg1);
    case 3:  return ((evariant(*)(evariant,evariant,evariant))i.base)                                                                                                           (arg0,arg1,arg2);
    case 4:  return ((evariant(*)(evariant,evariant,evariant,evariant))i.base)                                                                                                   (arg0,arg1,arg2,arg3);
    case 5:  return ((evariant(*)(evariant,evariant,evariant,evariant,evariant))i.base)                                                                                           (arg0,arg1,arg2,arg3,arg4);
    case 6:  return ((evariant(*)(evariant,evariant,evariant,evariant,evariant,evariant))i.base)                                                                                   (arg0,arg1,arg2,arg3,arg4,arg5);
    case 7:  return ((evariant(*)(evariant,evariant,evariant,evariant,evariant,evariant,evariant))i.base)                                                                           (arg0,arg1,arg2,arg3,arg4,arg5,arg6);
    case 8:  return ((evariant(*)(evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant))i.base)                                                                   (arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7);
    case 9:  return ((evariant(*)(evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant))i.base)                                                           (arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8);
    case 10: return ((evariant(*)(evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant))i.base)                                                   (arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9);
    case 11: return ((evariant(*)(evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant))i.base)                                           (arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10);
    case 12: return ((evariant(*)(evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant))i.base)                                   (arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11);
    case 13: return ((evariant(*)(evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant))i.base)                           (arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12);
    case 14: return ((evariant(*)(evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant))i.base)                   (arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12,arg13);
    case 15: return ((evariant(*)(evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant))i.base)           (arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12,arg13,arg14);
    case 16: return ((evariant(*)(evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant,evariant))i.base)   (arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12,arg13,arg14,arg15);
    default: return 0;
  }
}

bool script_exists(int script)
{
    return (script >= 0 && size_t(script) < enigma::script_idmax && bool(enigma::callable_scripts[script].base));
}

}
