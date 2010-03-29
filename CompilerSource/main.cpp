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

#include <time.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <map>

using namespace std;

#define until(x) while(!(x))

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
#include <windows.h>

int establish_bearings()
{
  //find us the GCC
  fclose(fopen("blank.txt","wb"));
  fclose(fopen("defines.txt","wb"));
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

#include "cfile_parse/cfile_pushing.h"

extern void print_err_line_at(unsigned a);
int main(int argc, char *argv[])
{
    cparse_init();
    
    if (establish_bearings()) {
      cout << "ERROR: Failed to locate the GCC";
      getchar(); return 1;
    }
    
    string EGMmain = fc("../ENIGMAsystem/SHELL/SHELLmain.cpp");
    if (EGMmain == "") {
      char d[600];
      GetCurrentDirectory(600,d);
      cout << "ERROR: Failed to read main engine file from " << d;
      getchar(); return 1;
    }
    
    clock_t cs = clock();
    unsigned a = parse_cfile(EGMmain);
    clock_t ce = clock();
    
    if (a != unsigned(-1)) {
      cout << "ERROR in parsing engine file: this is the worst thing that could have happened within the first few seconds of compile.\n";
      print_err_line_at(a);
      getchar(); return 1;
    }
    
    cout << "Successfully parsed ENIGMA's engine (" << (((ce - cs) * 1000)/CLOCKS_PER_SEC) << "ms)\n";
    cout << "Namespace std contains " << global_scope.members["std"]->members.size() << " items.\n";
    
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
    
    getchar();
    return 0;
    
    
    
    //Parse out some parameters
      string p1;
      if (!(argc>1)) { p1=""; }
      else      { p1=argv[1]; }
      double result=0; 
    
    if (p1[0]=='/' || p1[0]=='\\') p1[0] = '-';
    
    if (p1=="-r")
    {
      if (argc<3) {puts("Insufficient parameters"); return -11; }
      result = CompileEGMf(argv[2],argv[3]);
    }
    if (p1=="-b")
    {
      if (argc<3) {puts("Insufficient parameters"); return -11; }
      result = CompileEGMf(argv[2],argv[3],0,1);
    }
    if (p1=="-c")
    {
      if (argc<3) {puts("Insufficient parameters"); return -11; }

      string p3=argv[3];
      if (p3.length()<4) p3+=".exe";
      else if (p3.substr(p3.length()-4,4)!=".exe") p3+=".exe";

      result = CompileEGMf(argv[2],(char*)p3.c_str());
    }
    if (p1=="-d")
    {
      if (argc < 3) {puts("Insufficient parameters"); return -11; }
      result = CompileEGMf(argv[2],argv[3],1,0);
    }
    if (true or p1=="-s")
    {
      if (argc < 3) { return file_check("syntax.txt"); {puts("Insufficient parameters"); return -11; } }
      return file_check(argv[2]);
    }
    if (p1=="-?")
    {
      puts("ENIGMA Compiler\r\n\r\n");

      puts("-r in out | Compile 'in' to 'out' as fast as possible\r\n");
      puts("-b in chn | Compile 'in' with a room editor, saving room to 'chn'\r\n");
      puts("-c in out | Compile 'in' to 'out', no add-ons, full compile \r\n");
      puts("-d in out | Compile 'in' to 'out', including ENIGMA debugger\r\n");
    }
    if (p1=="") result = system("lateralgm-7-13.jar");


    return (int)result;
}
