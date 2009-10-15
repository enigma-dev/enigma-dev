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
#include "../general/implicit_stack.h"

using namespace std;

#include "value.h"
#include "../externs/externs.h"
#include "expression_evaluator.h"


#define is_letter(x)  ((x>='a' && x<='z') || (x>='A' && x<='Z') || (x=='_'))
  #define is_digit(x)   (x>='0' && x<='9')
  #define is_letterd(x) (is_letter(x) || is_digit(x))
  #define is_unary(x)   (x=='!' || x=='~' || x=='+' || x=='-' || x=='&' || x=='*')
  #define is_linker(x)  (x=='+' || x=='-' || x=='*' || x=='/' || x=='=' || x=='!' || x=='~' || x=='&' || x=='|' || x=='^' || x=='.')
#define is_useless(x) (x==' ' || x=='\r' || x=='\n' || x=='\t')

extern string cferr;

typedef implicit_stack<string> iss;
typedef implicit_stack<unsigned int> isui;

inline void move_newline_a(string &cfile,unsigned int &pos,const unsigned int len)
{
  while (cfile[pos]!='\r' and cfile[pos]!='\n' and pos<len)
  {
    pos++;
    if (cfile[pos]=='\\' and pos+1<len)
    {
      pos++;
      if (cfile[pos]=='\\')
      pos++;
      else
      {
        if (cfile[pos]=='\r') { pos++; if (cfile[pos]=='\n') pos++; }
        else if (cfile[pos]=='\n') pos++;
      }
    }
  }
}

#define move_newline() move_newline_a(cfile,pos,len)

bool fnf = 0;
string fc(const char* fn)
{
    fnf = 0;
    FILE *pt = fopen(fn,"rb");
    if (pt==NULL)
    {
      fnf=1;
      return "";
    }
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

struct flow_stack
{
  struct n
  {
    n* prev;
    bool value;
    n(): prev(NULL) {}
    n(bool x): prev(NULL), value(x) {}
    n(n* p,bool x): prev(p), value(x) {}
  } *top;
  flow_stack(): top(NULL) {}
  void push(bool x) { top = new n(top,(top == NULL) ? x : top->value and x); }
  bool pop() { if (top==NULL) return 1; n*d=top; bool r=top->value; top=top->prev; delete d; return r; }
  bool empty() { return top == NULL; }
  bool topval() { return top->value; }
  bool invert_top() { return top->value = !top->value; }
  void set_top(bool x) { top->value = x; }
} flowstack;

bool in_false_conditional()
{
  if (flowstack.empty()) return 0;
  return flowstack.topval() == 0;
}


typedef implicit_stack<string> iss;
typedef implicit_stack<unsigned int> isui;

#define cfile c_file()
#define pos position()
#define len cfile_length()
unsigned int cfile_parse_macro(iss &c_file,isui &position,isui &cfile_length)
{
  pos++;
  while(is_useless(cfile[pos])) pos++;

  unsigned int poss=pos;
  if (!is_letter(cfile[pos])) { cferr="Preprocessor directive expected"; return pos; }
  while (is_letterd(cfile[pos])) pos++;
  string next=cfile.substr(poss,pos-poss);

  if (next=="error")
  {
    int poss=pos;
    move_newline();
    cferr=cfile.substr(poss,pos-poss+1);
    return pos;
  }
  //Define, Undefine
  {
    if (next=="define")
    {
      while (is_useless(cfile[pos])) pos++;
      
      const unsigned poss=pos;
      if (!is_letter(cfile[pos])) { cferr="Identifier expected for #define"; return pos; }
      while (is_letterd(cfile[pos])) pos++;
      
      //unsigned int flags=0;
      
      string defiendum=cfile.substr(poss,pos-poss);
      if (cfile[pos]=='(') //macro function
      {
        int lvl=1, an=0,ac=0;
        pos++;
        while (lvl>0 and pos<len)
        {
          if (cfile[pos]=='(') lvl++;
          else if (cfile[pos]==')') lvl--;
          else if (cfile[pos]==',') { if (an==0) { cferr="Identifier expected before ',' symbol"; return pos; } ac++; an=0; }
          else if (is_letter(cfile[pos]))
          {
            if (an) { cferr="Symbol ',' expected"; return pos; }
            while (is_letterd(cfile[pos])) pos++; pos--;
            an=1; if (ac==0) ac=1;
          }
          else if (!is_useless(cfile[pos])) { cferr="Unexpected symbol in macro parameters"; return pos; }
          pos++;
        }
      }
      
      while (is_useless(cfile[pos])) pos++;
      
      const unsigned poss2=pos;
      move_newline();
      
      string defiens=cfile.substr(poss2,pos-poss2);
      
      /*cout << "Define \"" << defiendum << "\" as \"" << defiens << "\"\r\n";
      if (current_scope->members.find(defiendum) != current_scope->members.end())
        current_scope->members.erase(defiendum);*/
      macros[defiendum]=defiens;
    }
    if (next=="undef")
    {
      const unsigned poss=pos;
      if (!is_letter(cfile[pos])) { cferr="Identifier expected for #define"; return pos; }
      while (is_letterd(cfile[pos])) pos++;
      
      macros.erase(cfile.substr(poss,pos-poss));
    }
  }
  //Including things
  {
    if (next=="include")
    {
      while (cfile[pos]==' ' or cfile[pos]=='\t') pos++;
      
      const char c = cfile[pos];
      if (cfile[pos] != '"' and cfile[pos] != '<')
      {
        cferr = "Expcted filename to include set in quotes or <>";
        return pos;
      }
      
      const int spos = ++pos;
      while (cfile[pos++] != '"');
      string file = cfile.substr(spos,pos-spos-1);
      move_newline();
      
      if (c == '"')
      {
        const string ins = fc(file.c_str());
        if (fnf)
        {
          cferr = "Failed to include " + file + ": File not found";
          return pos;
        }
        c_file.push();
        position.push();
        cfile_length.push();
        cfile = ins;
        len = cfile.length();
        pos = 0;
      }
    }
    if (next=="import")
    {
      move_newline();
    }
    if (next=="using")
    {
      move_newline();
    }
  }
  //Conditionals/Flow
  {
    if (next=="if" or next=="ifdef" or next=="ifndef" or next=="else" or next=="elif" or next=="endif")
    {
      while (cfile[pos]==' ' or cfile[pos]=='\t') pos++;
      const int spos = pos;
      move_newline();
      string exp = cfile.substr(spos,pos-spos);
      if (next[0] == 'i')
      {
        if (next.length() == 2)
          flowstack.push(evaluate_expression(exp));
        else if (next.length() == 5)
          flowstack.push(macros.find(exp) != macros.end());
        else if (next.length() == 6)
          flowstack.push(macros.find(exp) == macros.end());
      }
      else
      {
        if (next.length() == 4)
        {
          if (next[3]=='e') //else
            flowstack.invert_top();
          else //elif
            flowstack.set_top(evaluate_expression(exp));
        }
        else
          flowstack.pop();
      }
    }
  }
  if (next=="line")
  {
    cferr="#line is unimplemented for reasons of sanity.";
    return pos;
  }
  if (next=="pragma")
  {
    move_newline();
  }
  return (unsigned)-1;
}
