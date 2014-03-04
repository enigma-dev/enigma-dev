/**
  @file  main.cpp
  @brief Implements the main DLL API calls.
  
  @section License
    Copyright (C) 2008-2013 Josh Ventura
    This file is a part of the ENIGMA Development Environment.

    ENIGMA is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, version 3 of the license or any later version.

    This application and its source code is distributed AS-IS, WITHOUT ANY WARRANTY; 
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

    You should have recieved a copy of the GNU General Public License along
    with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <time.h>
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <stdio.h>
#include <map>

using namespace std;
#define flushl '\n' << flush
#define flushs flush

#include "general/darray.h"

#include "syntax/syncheck.h"
#include "parser/parser.h"
#include "compiler/compile.h"
#include "syntax/checkfile.h"
#include "OS_Switchboard.h"

int m_prog_loop_cfp();

#include <sys/time.h>

#ifdef _WIN32
 #include <windows.h>
 #define dllexport extern "C" __declspec(dllexport)
   #define DECLARE_TIME() clock_t cs, ce
   #define START_TIME() cs = clock()
   #define STOP_TIME() ce = clock()
   #define PRINT_TIME() (((ce - cs) * 1000)/CLOCKS_PER_SEC)
#else
 #define dllexport extern "C"
 #include <cstdio>
   #define DECLARE_TIME()  timeval ts, tn
   #define START_TIME() gettimeofday(&ts,NULL);
   #define STOP_TIME() gettimeofday(&tn,NULL);
   #define PRINT_TIME() ((double(tn.tv_sec - ts.tv_sec) + double(tn.tv_usec - ts.tv_usec)/1000000.0)*1000)
#endif

extern void print_err_line_at(size_t a);

#include "gcc_interface/gcc_backend.h"
#include "backend/JavaCallbacks.h"

#ifdef NOT_A_DLL
#  undef dllexport
#  define dllexport 
#endif

#include "languages/lang_CPP.h"
#include <System/builtins.h>
#include <API/jdi.h>

dllexport const char* libInit(EnigmaCallbacks* ecs);
dllexport void libFree();
dllexport syntax_error *definitionsModified(const char* wscode, const char* targetYaml);
dllexport syntax_error *syntaxCheck(int script_count, const char* *script_names, const char* code);

dllexport const char* libInit(EnigmaCallbacks* ecs)
{
  if (ecs)
  {
    cout << "Linking up to IDE" << endl;
    ide_dia_open     = ecs->dia_open;
    ide_dia_add      = ecs->dia_add;
    ide_dia_clear    = ecs->dia_clear;
    ide_dia_progress = ecs->dia_progress;
    ide_dia_progress_text = ecs->dia_progress_text;
    
    ide_output_redirect_file = ecs->output_redirect_file;
    ide_output_redirect_reset = ecs->output_redirect_reset;
  }
  else cout << "IDE Not Found. Continuing without graphical output." << endl;
  
  cout << "Implementing JDI basics" << endl;
  jdi::initialize();
  jdi::builtin->output_types();
  jdi::builtin->add_macro("true","1"); // Temporary, or permanent, fix for true/false in ENIGMA
  jdi::builtin->add_macro("false","0"); // Added because polygone is a bitch
  cout << endl << endl;
  
  cout << "Choosing language... Eenie, meenie, minie, C++" << endl;
  current_language_name = "CPP";
  current_language = languages[current_language_name] = new lang_CPP();
  
  cout << "Reading GCC builtins" << endl;
  const char* a = current_language->establish_bearings("Compilers/" CURRENT_PLATFORM_NAME "/gcc.ey");
  if (a)
    cout << "ERROR: " << a << endl << "See scrollback for more information.\n";
  
  cout << "Creating parse context" << endl;
  main_context = new jdi::context;
  
  return a;
}

dllexport void libFree() {
  delete main_context;
  delete current_language;
  jdi::clean_up();
}


#include "OS_Switchboard.h"
#include "settings-parse/crawler.h"
#include "settings-parse/parse_ide_settings.h"
#include "parser/object_storage.h"
#include "compiler/jdi_utility.h"

extern void print_definition(string n);

#include "languages/language_adapter.h"

dllexport syntax_error *definitionsModified(const char* wscode, const char* targetYaml)
{
  current_language->definitionsModified(wscode, targetYaml);
  return &ide_passback_error;
}

dllexport syntax_error *syntaxCheck(int script_count, const char* *script_names, const char* code)
{
  cout << "Well, shucks; started." << endl;
  
  //First, we make a space to put our scripts.
  jdi::using_scope globals_scope("<ENIGMA Resources>", main_context->get_global());
  
  cout << "Checkpoint." << endl;
  for (int i = 0; i < script_count; i++)
    quickmember_script(main_context, &globals_scope,script_names[i]);
  
  cout << "Starting syntax check." << endl;
  definition_scope fakecode, fakelocal, fakeglobal;
  EDL_AST ast(main_context, &fakecode, &fakelocal, &fakeglobal);
  bool serr = ast.parse_edl(code);
  // NEWPARSER: TODO: FIXME: Give a list of errors, somehow
  return &ide_passback_error;
}
