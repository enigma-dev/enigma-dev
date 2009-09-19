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

string fc(const char* fn)
{
    FILE *pt = fopen(fn,"rb");
    if (pt==NULL) { cout << "fail\r\n"; return ""; }
    else
    {
      fseek(pt,0,SEEK_END);
      size_t sz = ftell(pt);
      fseek(pt,0,SEEK_SET);

      char a[sz+1];
      sz = fread(a,1,sz,pt);
      a[sz] = 0;
      return a;
    }
};

void print_scope_members(externs* gscope, int indent);

int main(int argc, char *argv[])
{
      cparse_init();
      string cftp = fc("./cfile_parse/parsein.h");

      int a = parse_cfile(cftp);
      if (a != -1)
      {
        int line=0,pos=0;
        for (int i=0; i<a; i++,pos++)
        {
          if (cftp[i]=='\r')
            i++;
          if (cftp[i]=='\n')
            line++, pos=0;
        }
        printf("Line %d, position %d: %s\r\n",line+1,pos,cferr.c_str());
      }

      cout << "Macros:\r\n";
      for (maciter i=macros.begin(); i!=macros.end();i++)
        cout<<"  "<<i->second<<"\r\n";

      print_scope_members(&global_scope, 0);

    getchar();
    return 0;

      parser_init();
      string b = parser_main(fc("/media/HP_PAVILION/Documents and Settings/HP_Owner/Desktop/parsein.txt"));

    getchar();
    return 0;



    string p1;
    if (!(argc>1)) { p1=""; }
    else      { p1=argv[1]; }
    double result=0;


    if (p1[0]=='/' || p1[0]=='\\') p1[0]='-';

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
      if (argc<3) {puts("Insufficient parameters"); return -11; }
      result = CompileEGMf(argv[2],argv[3],1,0);
    }
    if (true or p1=="-s")
    {
      if (argc<3) { return file_check("syntax.txt"); {puts("Insufficient parameters"); return -11; } }
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
