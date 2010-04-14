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

int m_prog_loop_cfp();

#include <sys/time.h>

#ifdef _WIN32
 #include <windows.h>
 #define dllexport extern "C" __declspec(dllexport)
#else
 #define dllexport extern "C"
 #include <cstdio>
#endif

extern void clear_ide_editables();
extern void print_err_line_at(unsigned a);
#include "cfile_parse/cfile_pushing.h"

extern int establish_bearings();
extern void print_definition(string);

extern int cfile_parse_main();
int main(int argc, char *argv[])
{
  cparse_init();
  
  if (establish_bearings()) {
    cout << "ERROR: Failed to locate the GCC";
    getchar(); return 1;
  }
  
  string EGMmain = fc("./ENIGMAsystem/SHELL/SHELLmain.cpp");
  if (EGMmain == "") {
    cout << "ERROR: Failed to read main engine file\n";
    getchar(); return 1;
  }
  
  clock_t cs = clock();
  unsigned a = parse_cfile(EGMmain);
  clock_t ce = clock();
  
  if (a != unsigned(-1)) {
    cout << "ERROR in parsing engine file: this is the worst thing that could have happened within the first few seconds of compile.\n";
    print_err_line_at(a);
    
    print_definition("__PTRDIFF_TYPE__");
    return 1;
  }
  
  cout << "Successfully parsed ENIGMA's engine (" << (((ce - cs) * 1000)/CLOCKS_PER_SEC) << "ms)\n";
  //cout << "Namespace std contains " << global_scope.members["std"]->members.size() << " items.\n";
  
  parser_init();
  string pf = fc("C:/Users/Josh/ENIGMA/trunk/CompilerSource/cfile_parse/auxilary_gml.h");
  
  a = syncheck::syntacheck(pf);
  if (a != unsigned(-1))
  {
    int line = 1, lp = 1;
    for (unsigned i=0; i<a; i++,lp++) {
      if (pf[i] =='\r')
        line++, lp = 0, i += pf[i+1] == '\n';
      else if (pf[i] == '\n') line++, lp = 0;
    }
    cout << "Line " << line << ", position " << lp << " (absolute " << a << "): " << syncheck::error <<  endl;
  }
  else
  {
    cout << "Syntax check completed with no error.\n";
    
    string b = parser_main(pf);
    cout << endl << endl << endl << endl << b << endl;
  }
  
  return 0;
}
