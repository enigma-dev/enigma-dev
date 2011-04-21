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

#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <sys/time.h>
#include "../general/darray.h"

using namespace std;

#include "../externs/externs.h"
#include "expev_macros.h"
#include "value.h"

value evaluate_expression(string exp);
extern string rerr; extern int rerrpos;
void cparse_init();

extern string cferr;
#include "cfile_parse.h"


string file_contents(string file)
{
  FILE *f = fopen(file.c_str(),"rb");
  if (f==NULL) return "";

  fseek(f,0,SEEK_END);
  size_t sz = ftell(f);
  char a[sz+1];
  fseek(f,0,SEEK_SET);
  sz = fread(a,1,sz,f);

  fclose(f);
  a[sz]=0;

  return a;
}


void print_ext_data(externs *ext,int indent, int depth);

void print_scope_members(externs* gscope, int indent,int depth)
{
  if (depth--)
  for (extiter i=gscope->members.begin();i!=gscope->members.end();i++)
    print_ext_data(i->second,indent,depth);
}

extern string strace(externs*);
void print_ext_data(externs *ext,int indent,int depth)
{
  bool comma=0;
  string indstr(indent,' ');
  
  if (ext == NULL) {
    cout <<  indstr << "error\n";
    return;
  }
  cout << indstr << ext->name << ":  ";
  
  if (ext->is_function())
  {
    const int pcn = ext->parameter_count_min();
    cout << "Function with " << pcn;
    if (pcn != ext->parameter_count_max())
      cout << " to " << ext->parameter_count_max();
    cout << " parameters, returning ";
  }

  if (ext->type != NULL)
    cout << ext->type->name << "  ";

  if (ext->ancestors.size)
  {
    cout << " derived from ";
    string nc;
    for (unsigned i = 0; i < ext->ancestors.size; i++) {
      cout << nc << strace(ext->ancestors[i]);
      nc = ", ";
    }
    cout << "; ";
  }


  if (ext->flags & EXTFLAG_TEMPLATE)
  {
    cout << "Template [" << ext->tempargs.size << "]<";
    char comma = 0;
    for (unsigned ii=0; ii < ext->tempargs.size; ii++)
    {
      if (comma != 0) cout << comma;
      if (ext->tempargs[ii]->flags & EXTFLAG_TYPEDEF)
      {
        cout << "typename " << ext->tempargs[ii]->name;
        if (ext->tempargs[ii]->type != NULL)
          cout << "=" << ext->tempargs[ii]->type->name;
      }
      else
      {
        cout << ext->tempargs[ii]->name;
        if (ext->tempargs[ii]->flags & EXTFLAG_VALUED)
          cout << "=" << ext->tempargs[ii]->value_of;
      }
      comma = ',';
    }
    cout << "> ";
  }

  if (ext->flags & EXTFLAG_VALUED)
  {
    cout << " valued as " << ext->value_of;
    comma = ',';
  }

  if (ext->flags & EXTFLAG_TYPENAME)
  {
    if (comma)
      cout << ", serves as typename";
    else
      cout << " Serves as typename";
  }

  if (ext->flags & EXTFLAG_NAMESPACE) cout << " namespace";
  if (ext->flags & EXTFLAG_STRUCT) cout << " : struct";
  if (ext->flags & EXTFLAG_CLASS) cout << " : class";

  if (ext->flags & EXTFLAG_TYPEDEF)
    cout << " typedef'd as " << strace(ext->type) << " ";
  else
  if (!ext->members.empty())
  {
    cout << "\r\n" << indstr << "{\r\n";
    print_scope_members(ext,indent+2,depth);
    cout << indstr << "};\r\n";
  }
  else
  if (ext->flags & (EXTFLAG_STRUCT | EXTFLAG_CLASS | EXTFLAG_NAMESPACE))
    cout << " {}; ";

  if (!ext->refstack.empty())
  {
    cout << "Dereference path: ";
    for (rf_node *ii = ext->refstack.last; ii != NULL; ii = ii->prev)
      if (ii->ref.symbol == '(') cout << "(params = " << ii->ref.count << ")";
      else if (ii->ref.symbol == ')') cout << "</group>";
      else if (ii->ref.symbol == '[') cout << "[]";
      else cout << ii->ref.symbol;
  }

  cout << "\r\n";
}

#include "cparse_shared.h"

extern my_string fca(const char *fn);
int cfile_parse_main()
{
  cparse_init();

  my_string fc = fca("./ENIGMAsystem/SHELL/SHELLmain.cpp");
  if (fc == NULL) cout << "FAILOOS.";
  
  cout << fc;
  
  time_t ts = clock();
  int a = parse_cfile(fc);
  time_t te = clock();
  cout << "Parse time: " << ((te-ts) * 1000) / CLOCKS_PER_SEC << " milliseconds";
  
  if (a != -1)
  {
    int line=0,pos=0;
    for (int i=0; i<a; i++,pos++)
    {
      if (fc[i]=='\r')
        i++;
      if (fc[i]=='\n')
        line++, pos=0;
    }
    printf("Line %d, position %d: %s\r\n",line+1,pos,cferr.c_str());
  }
  
  cout << "Macros:\r\n";
  for (maciter i=macros.begin(); i!=macros.end();i++)
    cout<<"  "<<(string)i->second<<"\r\n";
  
  print_scope_members(&global_scope, 0);
  
  //system("pause");
  
  return 0;
}

string fc(const char* fn);

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

#include "cfile_parse.h"
#include "../general/implicit_stack.h"

void print_err_line_at(pt a)
{
  int line=0,pos=0;
  for (int i=0; i<(signed)a; i++,pos++)
  {
    if (i >= cfile.length()) {
      line = -1, pos = a;
      break;
    }
    if (cfile[i]=='\r')
      i++;
    if (cfile[i]=='\n')
      line++, pos=0;
  }
  printf("%sLine %d, position %d: %s\r\n",cferr_get_file().c_str(),line+1,pos,cferr.c_str());
  if (line >= 0)
  {
    const int margin = 100;
    const pt
      begin = (a < (margin/2))?0:a-(margin/2),
      end = (a+pt(margin/2)>cfile.length())?cfile.length():a+(margin/2);
    cout << "code snippet: " << cfile.substr(begin,end-begin).insert((a-begin<end)?a-begin:end,"<<>>") << endl;
  }
  cout << "Code snippet unavailable; possibly heavily buried in untraceable macros or instantiations." << endl;
  cout << "------------------------------------------------\r\n\r\n";
}

void print_definition(string n)
{
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
    extiter a;
    string seg;
    externs *isco = &global_scope;
    for (pt i = 0; i<n.length();)
    {
      const pt is = i;
      while (i<n.length() and n[i] != ':') i++;

      seg = n.substr(is,i-is);
      a = isco->members.find(seg);
      if (i<n.length() and a != isco->members.end())
        isco = a->second;
      while (i<n.length() and n[i] == ':') i++;
    }
    if (a != isco->members.end())
      print_ext_data(a->second, 2,1000);
    else cout << "Not found: " << seg << endl;
  }
}

int test_exp_eval()
{
  while (1)
  {
    string p;
    char ghere[2048];
    value val;
    cin.getline(ghere,2048);
    p=ghere; int wret;
    if (p=="x") return 0;
    if (p=="c" or p=="cls") wret = system("cls");
    else
    {
      val=evaluate_expression(p);
      #if USETYPE_INT
        if (val.type==RTYPE_INT)
          cout<<val.real.i;
      #endif
      #if USETYPE_DOUBLE
        if (val.type==RTYPE_DOUBLE)
          cout<<val.real.d;
      #endif
      #if USETYPE_UINT
        if (val.type==RTYPE_UINT)
          cout<<val.real.u;
      #endif
      #if USETYPE_STRING
        if (val.type==RTYPE_STRING)
          cout << '"' << val.str << '"';
      #endif
      cout << endl;
      if (rerr != "") cout <<"error at "<<rerrpos<<": "<<rerr << "\r\n\r\n";
    }
  }
}


int m_prog_loop_cfp()
{
  my_string cftp = fca(0 ? "./ENIGMAsystem/SHELL/SHELLmain.cpp" : "./CompilerSource/cfile_parse/parsein.h");

  #ifdef linux
    timeval ts; gettimeofday(&ts,NULL);
  #else
    time_t ts = clock();
  #endif
  
  int a = parse_cfile(cftp);
  int wret;

  #ifdef linux
    timeval te; gettimeofday(&te,NULL);
    double tel = (te.tv_sec*1000.0 + te.tv_usec) - (ts.tv_sec*1000.0 + ts.tv_usec);
  #else
    time_t te = clock();
    double tel = (((te-ts) * 1000.0) / CLOCKS_PER_SEC);
  #endif


  if (a != -1)
    print_err_line_at(a);
  else
  cout << "No error.\r\nParse time: " << tel
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
    if (c == 'c') wret = system("cls || clear");
    if (c == 'd')
    {
      cout << "Define: ";
      string n = getst();
      print_definition(n);
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
    if (c == 'e') print_err_line_at(a);
    if (c == 'p') wret = system("PAUSE");
    if (c == '>') cout << cfile << endl;
    cout << ">>";
    c = getch();
  }

  return 0;
}
