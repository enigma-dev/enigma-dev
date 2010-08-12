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
#define flushl '\n' << flush
#define flushs flush

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
extern void print_err_line_at(pt a);
#include "cfile_parse/cfile_pushing.h"

extern int establish_bearings();

#include "backend/JavaCallbacks.h"

dllexport int libInit(EnigmaCallbacks* ecs)
{
  cout << "Linking up to IDE";
  ide_dia_open     = ecs->dia_open;
  ide_dia_add      = ecs->dia_add;
  ide_dia_clear    = ecs->dia_clear;
  ide_dia_progress = ecs->dia_progress;
  ide_dia_progress_text = ecs->dia_progress_text;
  
  int a = establish_bearings();
  if (a) {
    cout << "ERROR: See scrollback for information.\n";
    return a;
  }
  return 0;
}

struct syntax_error {
  const char*err_str;
  int line, position;
  int absolute_index;
  void set(int x, int
  y,int a, string s);
} ide_passback_error;
string error_sstring;


void syntax_error::set(int x, int y, int a, string s)
{
  error_sstring = s;
  err_str = error_sstring.c_str();
  line = x, position = y;
  absolute_index = a;
}

const char* heaping_pile_of_dog_shit = "\
             /\n\
            |    |\n\
             \\    \\\n\
      |       |    |\n\
       \\     /    /     \\\n\
    \\   |   |    |      |\n\
     | /     /\\   \\    /\n\
    / |     /# \\   |  |\n\
   |   \\   *    `      \\\n\
    \\    /   =  # `     |\n\
     |  | #     ___/   /\n\
    /   _`---^^^   `. |\n\
   |  .*     #  =    | \\\n\
     |  =   #      __/\n\
    .\\____-------^^  `.\n\
   /      #         #  \\\n\
  |   =          =     |\n\
  \\___    #     #___--^\n\
      ^^^^^^^^^^^\n\n";

extern void print_definition(string n);
static bool firstpass = true;
dllexport syntax_error *whitespaceModified(const char* wscode)
{
  cout << "Clearing IDE editables... " << flushs;
    clear_ide_editables();
  cout << "Done." << flushl;
  
  cout << "Creating swap." << flushl;
  externs oldglobal; map<string,macro_type> oldmacs; // These will essentially garbage collect at the end of this call
  oldglobal.members.swap(global_scope.members);
  oldmacs.swap(macros);
  
  cout << "Initializing global scope.";
  cparse_init();
  
  cout << "Dumping whiteSpace definitions...";
  FILE *of = fopen("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_whitespace.h","wb");
  if (of) fputs(wscode,of), fclose(of);
  
  cout << "Opening ENIGMA for parse..." << flushl;
  string EGMmain = fc("ENIGMAsystem/SHELL/SHELLmain.cpp");
  if (EGMmain == "")
  {
    /*char d[600];
    GetCurrentDirectory(600,d);*/
    cout << "ERROR: Failed to read main engine";
    ide_passback_error.set(0,0,0,"ENIGMAsystem/SHELL/SHELLmain.cpp: File not found; parse cannot continue");
    if (!firstpass) oldglobal.members.swap(global_scope.members), oldmacs.swap(macros); // Restore the original... unless there wasn't one.
    firstpass = false; return &ide_passback_error;
  }
  
  clock_t cs = clock();
  pt a = parse_cfile(EGMmain);
  clock_t ce = clock();
  
  if (a != pt(-1)) {
    cout << "ERROR in parsing engine file: this is the worst thing that could have happened within the first few seconds of compile.\n";
    cout << heaping_pile_of_dog_shit;
    print_definition("__GNUC_PREREQ");
    print_definition("__builtin_huge_val");
    print_err_line_at(a);
    ide_passback_error.set(0,0,0,"Parse failed; details in stdout. Bite me.");
    if (!firstpass) oldglobal.members.swap(global_scope.members), oldmacs.swap(macros);
    firstpass = false;
    return &ide_passback_error;
  }
  firstpass = false;
  
  cout << "Successfully parsed ENIGMA's engine (" << (((ce - cs) * 1000)/CLOCKS_PER_SEC) << "ms)\n"
  << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  //cout << "Namespace std contains " << global_scope.members["std"]->members.size() << " items.\n";
  
  cout << "Initializing EDL Parser...\n";
  
  parser_init();
  
  cout << "Grabbing locals...\n";
  
  shared_locals_load();
  
  cout << " Done.\n";
  
  return &ide_passback_error;
};

void quickmember_script(externs* scope, string name);
dllexport syntax_error *syntaxCheck(int script_count, const char* *script_names, const char* code)
{
  //First, we make a space to put our scripts.
  globals_scope = scope_get_using(&global_scope);
  globals_scope = globals_scope->members["ENIGMA Resources"] = new externs("ENIGMA Resources",NULL,globals_scope,EXTFLAG_NAMESPACE);
  
  shared_locals_load();
  
  for (int i = 0; i < script_count; i++)
    quickmember_script(globals_scope,script_names[i]);
  
  ide_passback_error.absolute_index = syncheck::syntacheck(code);
  error_sstring = syncheck::error;
  
  ide_passback_error.err_str = error_sstring.c_str();
  
  if (ide_passback_error.absolute_index != -1)
  {
    int line = 1, lp = 1;
    for (int i=0; i<ide_passback_error.absolute_index; i++,lp++) {
      if (code[i] =='\r')
        line++, lp = 0, i += code[i+1] == '\n';
      else if (code[i] == '\n') line++, lp = 0;
    }
    
    ide_passback_error.line = line;
    ide_passback_error.position = lp;
  }
  cout << "In checking code\n" << code << "\n\nat position " << ide_passback_error.absolute_index << "\n\n";
  cout << endl << "Line " << ide_passback_error.line << ", position " << ide_passback_error.position << ": " << ide_passback_error.err_str << endl<< endl;
  return &ide_passback_error;
}
