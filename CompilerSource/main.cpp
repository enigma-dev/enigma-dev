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

void print_ext_data(externs *ext,int indent);
void print_scope_members(externs* gscope, int indent);

char getch()
{
  int c, first;
  first = c = getchar();
  while (c != '\n' && c != EOF)
  c = getchar();
  return first;
}
string getst()
{
  string res;
  int c, first;
  first = c = getchar();
  while (c != '\n' && c != EOF)
  {
    res += char(c);
    c = getchar();
  }
  return res;
}

#include "general/implicit_stack.h"
extern string cfile_top;
int main(int argc, char *argv[])
{
      cparse_init();
      string cftp = fc("./cfile_parse/parsein.h");
      
      time_t ts = clock();
        int a = parse_cfile(cftp);
      time_t te = clock();
      
      
      if (a != -1)
      {
        int line=0,pos=0;
        for (int i=0; i<a; i++,pos++)
        {
          if (cfile_top[i]=='\r')
            i++;
          if (cfile_top[i]=='\n')
            line++, pos=0;
        }
        printf("%sLine %d, position %d: %s\r\n",cferr_get_file().c_str(),line+1,pos,cferr.c_str());
        const int margin = 100;
        const unsigned int 
          begin = (signed)a-(margin/2)<0?0:a-(margin/2), 
          end = (a+unsigned(margin/2)>cfile_top.length())?cfile_top.length():a+(margin/2);
        cout << "code snippet: " << cfile_top.substr(begin,end-begin).insert((a-begin<end)?a-begin:end,"<<>>") << endl;
        cout << "------------------------------------------------\r\n\r\n";
      }
      else
      cout << "No error.\r\nParse time: " << (((te-ts) * 1000.0) / CLOCKS_PER_SEC)
           << " milliseconds\r\n++++++++++++++++++++++++++++++++++++++++++++++++\r\n\r\n";

      cout << "Macros ("<<macros.size()<<") [+]\r\nVariables [+]\r\n";
      
      cout << ">>";
      char c = getch();
      while (c != '\r' and c != '\n')
      {
        if (c == '+')
        {
          cout << "Macros ("<<macros.size()<<"):\r\n";
            for (maciter i=macros.begin(); i!=macros.end();i++)
            {
              cout<<"  "<<i->first;
              if (i->second.argc != -1)
              {
                cout << "(";
                for (int ii = 0; ii<i->second.argc; ii++)
                  cout << i->second.args[ii] << (ii<i->second.argc-1 ? ",":"");
                cout << ")";
              }
              cout<<": "<<(string)i->second<<"\r\n";
            }
          cout<<"\r\nVariables:\r\n";
            print_scope_members(&global_scope, 2);
        }
        if (c == 'c') system("cls ^ clear");
        if (c == 'd')
        {
          cout << "Define: ";
          string n = getst();
          maciter a = macros.find(n);
          if (a != macros.end())
          {
            cout << "  #define " << n;
            if (a->second.argc != -1) {
              cout << "("; for (int i=0; i<a->second.argc; i++) 
              cout << a->second.args[i] << (i+1<a->second.argc ? ", " : ")"); }
            cout << " " << (string)a->second << endl;
          }
          else
          {
            extiter a = global_scope.members.find(n);
            if (a != global_scope.members.end())
              print_ext_data(a->second, 2);
            else cout << "Not found: " << n << endl;
          }
        }
        if (c == 't')
        {
          cout << "Trace: ";
          string n = getst();
          extiter a = global_scope.members.find(n);
          if (a != global_scope.members.end())
          {
            for (externs* i=a->second; i!=&global_scope; i=i->parent)
            cout << i->name << "::";
          }
          else cout << "Not found: " << n << endl;
        }
        if (c == 'm')
        {
          string search_terms;
          cout << "Search in Macros: ";
          cin >> search_terms;
          
          getch(); bool f = 0;
          for (maciter i = macros.begin(); i != macros.end(); i++)
            if (string(i->second).find(search_terms) != string::npos)
            { cout << "Found " << i->first << ". (N/Q)"; char a = getch(); f=1; if (a != 'N' and a != 'n') break; }
          if (!f) cout << "Not found.";
        }
        if (c == 'e') cout << cferr << endl;
        if (c == 'p') system("PAUSE");
        cout << ">>";
        c = getch();
      }
    
    return 0;
    
      parser_init();
      string b = parser_main(fc("/media/HP_PAVILION/Documents and Settings/HP_Owner/Desktop/parsein.txt"));
    
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
