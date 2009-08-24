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

#include <map>
#include <string>
#include <iostream>
#include "../general/darray.h"

using namespace std;

#include "../externs/externs.h"
#include "expev_macros.h"
#include "value.h"

value evaluate_expression(string exp);
extern string rerr; extern int rerrpos;
void cparse_init();

extern string cferr;
int parse_cfile(string cfile);


string file_contents(string file)
{
  FILE *f = fopen(file.c_str(),"rb");
  if (f==NULL) return "";

  fseek(f,0,SEEK_END);
  size_t sz = ftell(f);
  char a[sz+1];
  fseek(f,0,SEEK_SET);
  sz = fread(a,sz,1,f);

  fclose(f);
  a[sz]=0;

  return a;
}


void print_scope_members(externs* gscope, int indent)
{
  for (extiter i=gscope->members.begin();i!=gscope->members.end();i++)
  {
    bool comma=0;
    string indstr(indent,' ');

    cout << indstr << i->second->name << ":  ";

    if (i->second->type != NULL)
      cout << i->second->type->name << "  ";

    if (i->second->flags&EXTFLAG_FUNCTION)
    {
      cout << " function (" << i->second->fargs.size << " parameters)"/* returning "<<(i->second)[0].macrotext*/;
      comma=1;
    } /*else cout << " " << (i->second)[0].macrotext;*/

    if (i->second->flags&EXTFLAG_TEMPLATE)
    {
      cout << " template with " << i->second->targs.size << " parameters";
      comma=1;
    }

    if (i->second->flags & EXTFLAG_TYPENAME)
    {
      if (comma)
      cout << ", serves as typename";
      else
      cout << " Serves as typename";
    }

    if (i->second->flags & EXTFLAG_NAMESPACE) cout << " namespace";
    if (i->second->flags & EXTFLAG_STRUCT) cout << " : struct";
    if (i->second->flags & EXTFLAG_CLASS) cout << " : class";

    if (!i->second->members.empty())
    {
      cout << "\r\n" << indstr << "{\r\n";
      print_scope_members(i->second,indent+2);
      cout << indstr << "};\r\n";
    }

    cout << "\r\n";
  }
}

int cfile_parse_main()
{
  cparse_init();

  string fc=file_contents("C:\\cfile.h");
  time_t ts = clock();
  int a=parse_cfile(fc);
  time_t te = clock();
  cout << "Parse time: " << ((te-ts) * 1000) / CLOCKS_PER_SEC << " milliseconds";
  return 0;
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
    cout<<"  "<<i->second<<"\r\n";

  print_scope_members(&global_scope, 0);

  //system("pause");

  #if 0
      while (1)
      {
        string p;
        char ghere[2048];
        value val;
        cin.getline(ghere,2048);
        p=ghere;
        if (p=="x") return 0;
        if (p=="c" or p=="cls") system("cls");
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
  #endif


  return 0;
}
