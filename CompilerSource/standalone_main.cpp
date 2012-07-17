/** Copyright (C) 2008-2012 Josh Ventura
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>

using namespace std;
#define flushl '\n' << flush
#define flushs flush

#include "general/darray.h"


#include "syntax/syncheck.h"
  #include "parser/parser.h"
  #include "parser/object_storage.h"
  #include "compiler/compile.h"
  #include "syntax/checkfile.h"


int m_prog_loop_cfp();

#include <sys/time.h>

#ifdef _WIN32
 #include <windows.h>
 #define dllexport extern "C" __declspec(dllexport)
#else
 #define dllexport extern "C"
 #include <cstdio>
#endif

extern const char* establish_bearings(const char* compiler);
extern void print_definition(string);


extern  int ext_count;

#include "settings-parse/crawler.h"
#include "filesystem/file_find.h"

#include <Storage/definition.h>
#include <languages/lang_CPP.h>

extern char getch();
extern int cfile_parse_main();
extern jdi::definition *enigma_type__var, *enigma_type__variant, *enigma_type__varargs;

inline void povers(string n)
{
  jdi::definition *d = main_context->get_global()->look_up(n);
  if (d) cout << d->toString();
  else cout << n << ": Not found" << endl;
}

const char* libInit(struct EnigmaCallbacks* ecs);
void        libFree();
syntax_error *definitionsModified(const char*,const char*);

#include "OS_Switchboard.h"
#include "general/bettersystem.h"

int main(int argc, char* argv[])
{
  puts("Attempting to run");
  e_execp("gcc -E -x c++ -v blank.txt","");
  //e_exec("gcc -E -x c++ -v blank.txt");
  
  libInit(NULL);
  current_language->definitionsModified(NULL, ((string) "%e-yaml\n"
    "---\n" 	 
    "target-windowing: " +  (CURRENT_PLATFORM_ID==OS_WINDOWS ? "Win32" : CURRENT_PLATFORM_ID==OS_MACOSX ? "Cocoa" : "xlib")  + "\n" 	 
    "target-graphics: OpenGL\n" 	 
    "target-audio: OpenAL\n"
    "target-collision: BBox\n"
    /* Straight from LGM on Linux
    "treat-literals-as: 0\n"
    "sample-lots-of-radios: 0\n"
    "inherit-equivalence-from: 0\n"
    "sample-checkbox: on\n"
    "sample-edit: DEADBEEF\n"
    "sample-combobox: 0\n"
    "inherit-strings-from: 0\n"
    "inherit-escapes-from: 0\n"
    "inherit-increment-from: 0\n"
    " \n"
    "target-audio: OpenAL\n"
    "target-windowing: xlib\n"
    "target-compiler: gcc\n"
    "target-graphics: OpenGL\n"
    "target-widget: None\n"
    "target-collision: BBox\n"
    "target-networking: None\n"
    */
    ).c_str());
  libFree();
  //mainr(argc,argv);
  /*string in2 = fc("./CompilerSource/cfile_parse/auxilary_gml.h");
  int retval = syncheck::syntacheck(in2);
  int l=0,p=0;
  for (int i=0;i<retval;i++) { if (in2[i]=='\r') { l++; if (in2[i+1]=='\n') i++; p=i+1; } else if (in2[i]=='\n') { l++; p=i+1; } }
  printf("Line %d, position %d (absolute index %d): %s\r\n",(l)+1,(retval-p)+1,retval,syncheck::syerr.c_str());
  */
  //getchar();
  return 0;
}
