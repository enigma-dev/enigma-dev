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

extern int establish_bearings();
extern void print_definition(string);


extern  int ext_count;
extern  map<externs*, int> bigmap;

#include "cfile_parse/type_resolver.h"
#include "settings-parse/crawler.h"
#include "filesystem/file_find.h"

extern char getch();
extern int cfile_parse_main();
int mainr(int argc, char *argv[])
{
  /*my_string atest = fca("test.txt");
  if (!atest) cout << "Open fail." << endl;
  else {
    cout << "a.length: " << atest.length() << endl;
    cout << '"' << atest << '"' << endl;
  }
  string attn = "NIGGAR TITTZ";
  atest = attn;
    cout << "a.length: " << atest.length() << endl;
    cout << '"' << atest << '"' << endl;
  return -1;*/
  
  if (establish_bearings()) {
    cout << "ERROR: Failed to locate the GCC" << endl;
    getchar(); return 1;
  }
  
  cparse_init();
  
  
  cout << "Grabbing locals" << endl;
    extensions::crawl_for_locals();
    string localstring = extensions::compile_local_string();
    cout << localstring << endl;
  cout << "Ass." << endl;
  
  //m_prog_loop_cfp();
  
  my_string EGMmain = fca(1 ? "./CompilerSource/cfile_parse/auxilary.h" : "./ENIGMAsystem/SHELL/SHELLmain.cpp");
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
  //cout << "Namespace std contains " << global_scope.members["std"]->members.size() << " items.\n";
  
  parser_init();
  string pf = fc("./CompilerSource/cfile_parse/auxilary_gml.h");
  
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
  parser_secondary(ev.code,ev.synt);
  ofstream fpsd("parse_output.txt",ios_base::out);
  print_to_file(ev.code,ev.synt,sc,empty,0,fpsd); fpsd.close();
  system("gedit parse_output.txt || notepad parse_output.txt");
  cout << "\n\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
  
  cout << "Locals declared:\n";
  for (deciter i = par.locals.begin(); i != par.locals.end(); i++)
  {
    cout << "  " << (i->second.type != ""? i->second.type : "var") << " " << i->second.prefix << " " << i->first << " " << i->second.suffix << "\n";
  }
  
  exp_typeof_init();
  
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

int main(int argc, char* argv[])
{
  mainr(argc,argv);
  getchar();
  return 0;
}
