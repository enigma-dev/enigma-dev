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
#include <stdio.h>
using namespace std;

#include "../general/darray.h"
#include "cfile_pushing.h"

#include "macro_functions.h"
#include "../externs/externs.h"

includings::includings(string n,string p):name(n), path(p) {} 
stack<includings> included_files;

cfnode::cfnode(): scfile(cfile), spos(pos), slen(len) { }
cfnode::~cfnode()
{
  cfile = scfile;
  pos = spos;
  len = slen;
  id_would_err_at = pos;
}

unsigned int macrod = 0;
varray<string> inmacros;

stack<cfnode*> cfstack;
void handle_macro_pop()
{
  delete cfstack.top();
  cfstack.pop();
  
  if (macrod > 0)
    macrod--;
  else if (!included_files.empty()) {
    included_files.pop();
  }
}

#include <iostream>
unsigned int handle_macros(const string n)
{
  maciter t;
  //cout << "Find macro <"<<n<<">\n";
  
  if ((t=macros.find(n)) != macros.end())
  {
    bool recurs=0;
    for (unsigned int iii=0;iii<macrod;iii++)
       if (inmacros[iii]==n) { recurs=1; break; }
    if (!recurs)
    {
      string macrostr = t->second;
      
      if (t->second.argc != -1) //Expect ()
      {
        if (!macro_function_parse(cfile,n,pos,macrostr,t->second.args,t->second.argc,t->second.args_uat)) {
          cferr = macrostr;
          return pos;
        }
      }
      
      //const int cpos = position();
      cfstack.push(new cfnode);
      
      //Set everything
      cfile = macrostr;
      len = cfile.length();
      pos = 0;
      
      inmacros[macrod++] = n;
      return unsigned(-2);
    }
    //else puts("Recursing macro. This may be a problem.\r\n");
  }
  return unsigned(-1);
}
