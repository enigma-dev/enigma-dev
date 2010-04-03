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

#include <string>
using namespace std;
#include "../general/darray.h"

varray<string> //In int *foobar [32]
  varnames,       //this is "foobar"
    varname_prefix, //this is "*"
    varname_types,  //this is "int"
    varname_suffix; //this is "[32]"
varray<string>
  withs,
  globals,
    globalstars,
    globaltags,
    globalarrays,
  fnames;

varray<string>
  localn,
  localt;
int varcount,fcount,withcount,globalcount,globalscrcount,localcount;


int varname_onlists(string vname)
{
  return 1;
}
int varname_oninheriteds(string vname)
{
  return 1;
}
int varname_onnotinheriteds(string vname)
{
  return 1;
}
