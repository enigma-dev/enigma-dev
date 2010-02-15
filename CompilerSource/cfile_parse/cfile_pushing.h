
/*********************************************************************************\
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
\*********************************************************************************/

#include <stack>
#include <string>
using namespace std;

#include "cparse_shared.h"

struct includings { 
  string name, path;
  includings(string n,string p);
};
extern stack<includings> included_files;


struct cfnode
{
  string scfile;
  unsigned spos;
  unsigned slen;
  cfnode();
  ~cfnode();
};

//extern unsigned int macrod=0;
//extern varray<string> inmacros;

extern stack<cfnode*> cfstack;
void handle_macro_pop();
unsigned int handle_macros(const string n);

extern varray<string> include_directories;
extern unsigned int include_directory_count;
