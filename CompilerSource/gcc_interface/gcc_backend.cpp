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

#include <time.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <map>

using namespace std;
#define flushl (fflush(stdout), "\n")

#include "general/darray.h"

#include "externs/externs.h"
#include "syntax/syncheck.h"
    #include "parser/parser.h"
    #include "compiler/compile.h"
    #include "cfile_parse/cfile_parse.h"
    #include "syntax/checkfile.h"

string fc(const char* fn);

#include <sys/time.h>

#ifdef _WIN32
 #include <windows.h>
 #define dllexport extern "C" __declspec(dllexport)
#else
 #define dllexport extern "C"
 #include <cstdio>
#endif

bool init_found_gcc = false;
bool init_load_successful = false;
varray<string> include_directories;

//Find us the GCC, get info about it and ourself
int establish_bearings()
{
  // Clear some files
  fclose(fopen("blank.txt","wb"));
  fclose(fopen("defines.txt","wb"));
  fclose(fopen("searchpaths.txt","wb"));
  
  // See if we've been down this road before
  string GCC_location =
  
  if (system("cpp -dM -x c++ -E  blank.txt > defines.txt"))
  {
    fclose(fopen("defines.txt","wb"));
    if (system("C:/MinGW/bin/cpp -dM -x c++ -E blank.txt > defines.txt"))
      return 1;
  }
  string defs = fc("defines.txt");
  if (defs == "")
    return 1;
  
  unsigned a = parse_cfile(defs);
  if (a != unsigned(-1)) {
    cout << "Highly unlikely error. Borderline impossible, but stupid things can happen when working with files.\n\n";
    return 1;
  }
  
  cout << "Successfully loaded GCC definitions\n";
  cout << "Undefining _GLIBCXX_EXPORT_TEMPLATE\n";
  macros["_GLIBCXX_EXPORT_TEMPLATE"] = "0";
  return 0;
}

dllexport int gccDefinePath(const char* gccPath)
{
  return 0;
}
