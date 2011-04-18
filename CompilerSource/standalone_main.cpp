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
#include <fstream>
#include <cstdlib>
#include <map>

using namespace std;
#define flushl '\n' << flush
#define flushs flush

#include "general/darray.h"

#include "externs/externs.h"
#include "syntax/syncheck.h"
    #include "parser/parser.h"
    #include "parser/object_storage.h"
    #include "compiler/compile.h"
    #include "cfile_parse/cfile_parse.h"
    #include "syntax/checkfile.h"

string fc(const char* fn);
my_string fca(const char* fn);

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

extern const char* establish_bearings(const char* compiler);
extern void print_definition(string);


extern  int ext_count;
extern  map<externs*, int> bigmap;

#include "cfile_parse/type_resolver.h"
#include "settings-parse/crawler.h"
#include "filesystem/file_find.h"

extern char getch();
extern int cfile_parse_main();
extern externs *enigma_type__var, *enigma_type__variant;

inline void povers(string n)
{
  if (find_extname(n,0xFFFFFFFF))
  {
    cout << ext_retriever_var->name;
    for (unsigned i = 0; i < ext_retriever_var->sparams.size; i++)
      cout << "   " << ext_retriever_var->sparams[i] << endl;
  }
}
int mainr(int argc, char *argv[])
{
  cout << "Grabbing locals" << endl;
    extensions::crawl_for_locals();
    string localstring = extensions::compile_local_string();
    cout << localstring << endl;
  cout << "Ass." << endl;
  
  //m_prog_loop_cfp();
  
  my_string EGMmain = fca("./CompilerSource/cfile_parse/auxilary.h");
  if (EGMmain == NULL) {
    cout << "ERROR: Failed to read main engine file\n";
    getchar(); return 1;
  } else cout << "Opened Engine file.\n";
  
  /*ofstream wto("freezway.txt",ios_base::out);
    wto << "This is what ENIGMA read for SHELL: \n";
    wto << EGMmain;
  wto.close();*/
  
  //EGMmain += "\n\n#include <map>\n\nstd::map<float,float> testmap;\n\n";
  //EGMmain = fc("./CompilerSource/cfile_parse/auxilary.h");
  
  //my_string EGMmain_e = EGMmain;
  
  clock_t cs = clock();
  timeval ts, tn; gettimeofday(&ts,NULL);
  pt a = parse_cfile(EGMmain);
  gettimeofday(&tn,NULL);
  clock_t ce = clock();
  
  if (a != pt(-1))
  {
    cout << "ERROR in parsing engine file: this is the worst thing that could have happened within the first few seconds of compile.\n";
    print_err_line_at(a);
    
    print_definition("__PTRDIFF_TYPE__");
    return 1;
  }
  
  //print_scope_members(&global_scope);
  
  cout << "Successfully parsed ENIGMA's engine (" << (((ce - cs) * 1000)/CLOCKS_PER_SEC) << "ms)\n";
  cout << "Name lookups: " << endl;
  povers("draw_sprite");
  povers("show_message");
  povers("max");
  povers("sin");
  povers("fork");
  povers("toString");
  povers("draw_triangle");
  povers("draw_text");
  povers("draw_primitive_begin");
  povers("room_goto");
  povers("printf");
  povers("draw_background_tiled_area_ext");
  povers("draw_roundrect_color");
  
  return 0;
  //cout << "Namespace std contains " << global_scope.members["std"]->members.size() << " items.\n";
  
  if (find_extname("var", EXTFLAG_TYPENAME))
    enigma_type__var = ext_retriever_var;
  if (find_extname("variant",EXTFLAG_TYPENAME))
    enigma_type__variant = ext_retriever_var;
  
  cout << "Initializing GML parser..." << endl;
  parser_init();
  cout << "Loading locals..." << endl;
  shared_locals_load();
  cout << "Getting data..." << endl;
  string pf = fc("./CompilerSource/cfile_parse/auxilary_gml.h");
  
  cout << "Checking syntax" << endl;
  a = syncheck::syntacheck(pf);
  if (a != pt(-1))
  {
    int line = 1, lp = 1;
    for (unsigned i=0; i<a; i++,lp++) {
      if (pf[i] =='\r')
        line++, lp = 0, i += pf[i+1] == '\n';
      else if (pf[i] == '\n') line++, lp = 0;
    }
    cout << "Line " << line << ", position " << lp << " (absolute " << a << "): " << syncheck::error <<  endl;
    return 0;
  }
  
  cout << "Syntax check completed with no error.\n";
  cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  
  parsed_object par;
  parsed_event ev(&par);
  string b = parser_main(pf,&ev);
  unsigned sc = 0;
  varray<string> empty;
  
  cout << "\nParsed to:\n" << b;
  parsed_object tglob; tglob.locals["gstr"] = dectrip("string");
  parser_secondary(ev.code,ev.synt, &tglob);
  ofstream fpsd("parse_output.txt",ios_base::out);
  print_to_file(ev.code,ev.synt,sc,empty,0,fpsd); fpsd.close();
  system("gedit parse_output.txt || notepad parse_output.txt");
  cout << "\n\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
  
  cout << "Locals declared:\n";
  for (deciter i = par.locals.begin(); i != par.locals.end(); i++)
  {
    cout << "  " << (i->second.type != ""? i->second.type : "var") << " " << i->second.prefix << " " << i->first << " " << i->second.suffix << "\n";
  }
  
  cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nOne more thing: Resolving types\n";
  cout << externs_name(exp_typeof("room_speed")) << endl;
  cout << externs_name(exp_typeof("var")) << endl;
  cout << externs_name(exp_typeof("*var")) << endl;
  cout << externs_name(exp_typeof("room")) << endl;
  cout << externs_name(exp_typeof("room + room")) << endl;
  cout << externs_name(exp_typeof("room + 1")) << endl;
  cout << externs_name(exp_typeof("room - 1")) << endl;
  cout << externs_name(exp_typeof("var[5]")) << endl;
  cout << externs_name(exp_typeof("testmap")) << endl;
  cout << externs_name(exp_typeof("testmap[5]")) << endl;
  cout << (find_extname("testmap",0xFFFFFFFF) ? externs_name(ext_retriever_var) : "wut") << endl;
  
  cout << "And just for the record, parsing the C stuff took " << (double(tn.tv_sec - ts.tv_sec) + double(tn.tv_usec - ts.tv_usec)/1000000.0) << " seconds" << endl;
  cout << "\n\n\n\n\n\n\n\n\nPress Enter to continue" << endl;
  
  getch();
  
  cout << "% Testing free % \n";
  cout << "Freeing global scope... "; global_scope.clear_all(); cout << "Done.\n";
  cout << "Clearing parser lists... "; cfp_clear_lists(); cout << "Done.\n";
  /*
  cout << "Free completed successfully (" << global_scope.members.size() << "): " << ext_count << " remaining\n";
  cout << "(Or, if you want a second opinion, there are apparently " << bigmap.size() << " remaining)\n";
  for (map<externs*,int>::iterator i = bigmap.begin(); i != bigmap.end(); i++)
    cout << i->first->name << " ("<<(i->second)<<"), "; cout << "end\n";*/
  
  
  return 0;
}


struct syntax_error {
  const char*err_str;
  int line, position;
  int absolute_index;
  void set(int x, int
  y,int a, string s);
};
const char* libInit(struct EnigmaCallbacks* ecs);
syntax_error *definitionsModified(const char*,const char*);

#include "OS_Switchboard.h"
#include "general/bettersystem.h"

int main(int argc, char* argv[])
{
  puts("Attempting to run");
  e_execp("gcc -E -x c++ -v blank.txt","");
  //e_exec("gcc -E -x c++ -v blank.txt");
  
  libInit(NULL);
  definitionsModified(NULL, ((string) "%e-yaml\n"
	       "---\n" 	 
	       "target-windowing: " +  (CURRENT_PLATFORM_ID==OS_WINDOWS ? "Win32" : CURRENT_PLATFORM_ID==OS_MACOSX ? "Cocoa" : "xlib")  + "\n" 	 
	       "target-graphics: OpenGL\n" 	 
	       "target-audio: OpenAL\n" 	 
	       ).c_str());
  mainr(argc,argv);
  getchar();
  return 0;
}
