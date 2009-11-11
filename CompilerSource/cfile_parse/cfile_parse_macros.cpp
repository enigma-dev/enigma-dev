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
#include <stack>
#include <string>
#include <iostream>
#include "../general/darray.h"
#include "../general/implicit_stack.h"

using namespace std;

#include "value.h"
#include "../externs/externs.h"
#include "expression_evaluator.h"
#include "../general/parse_basics.h"

extern string cferr;
extern string tostring(int x);
struct includings
{
  string name;
  string path; 
  includings(string n,string p):name(n), path(p) {} 
};
extern stack<includings> included_files;

typedef implicit_stack<string> iss;
typedef implicit_stack<unsigned int> isui;

inline void move_newline_a(string &cfile,unsigned int &pos,const unsigned int len)
{
  bool cancomment = 1;
  while (cfile[pos]!='\r' and cfile[pos]!='\n' and pos<len)
  {
    if (cfile[pos]=='\\' and pos+1<len)
    {
      pos++;
      if (cfile[pos]=='\\') pos++;
      else
      {
        if (cfile[pos]=='\r') pos++;
        if (cfile[pos]=='\n') pos++;
      }
    }
    else if (cfile[pos]=='/' and cancomment)
    {
      if (cfile[pos+1]=='/')
        cancomment = 0;
      else if (cfile[pos+1]=='*') 
      {
        pos+=2;
        if (pos<len) pos++;
        while (pos<len and cfile[pos] != '/' and cfile[pos-1] != '*') pos++;
        continue;
      }
    }
    pos++;
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
      fclose(pt);
      
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
    bool used;
    n(): prev(NULL),value(0),used(0) {}
    n(bool x): prev(NULL), value(x), used(x)  {}
    n(n* p,bool x): prev(p), value(x), used(x) {}
    n(n* p,bool x,bool pre): prev(p), value(x), used(x or pre) {}
  } *top;
  flow_stack(): top(NULL) {}
  void push(bool x) { top = (top) ? new n(top,top->value and x,!top->value) : new n(top,x,0); }
  bool pop() { if (top==NULL) return 1; n*d=top; bool r=top->value; top=top->prev; delete d; return r; }
  bool empty() { return top == NULL; }
  bool topval() { if (top) return top->value; return 0; }
  bool invert_top() { if (top and (top->value or !top->used)) { top->used=1; return top->value = !top->value; } return 0; }
  bool topused() { if (top) return top->used; return 0; }
  void set_top(bool x) { top->value = x; top->used |= x; }
} flowstack;

bool in_false_conditional()
{
  if (flowstack.empty()) return 0;
  return flowstack.topval() == 0;
}

extern string cfile_top;

typedef implicit_stack<string> iss;
typedef implicit_stack<unsigned int> isui;

extern varray<string> include_directories;
extern unsigned int include_directory_count;

#define cfile c_file()
#define pos position()
#define len cfile_length()
unsigned int cfile_parse_macro(iss &c_file,isui &position,isui &cfile_length)
{
  pos++;
  while(is_useless_macros(cfile[pos])) pos++;

  unsigned int poss=pos;
  if (!is_letter(cfile[pos])) { cferr="Preprocessor directive expected"; return pos; }
  while (is_letterd(cfile[pos])) pos++;
  string next=cfile.substr(poss,pos-poss);
  
  while (cfile[pos] == ' ' or cfile[pos] == '\t') pos++;
  
  if (!in_false_conditional())
  {
    if (next=="error")
    {
      int poss=pos;
      move_newline();
      cferr="#error "+cfile.substr(poss,pos-poss+1);
      return pos;
    }
    //Define, Undefine
    {
      if (next=="define")
      {
        while (cfile[pos] == ' ' or cfile[pos] == '\t') pos++;
        
        const unsigned poss=pos;
        if (!is_letter(cfile[pos])) { cferr="Identifier expected for #define"; return pos; }
        while (is_letterd(cfile[pos])) pos++;
        
        //unsigned int flags=0;
        int arg_count = -1;
        varray<string> args;
        
        string defiendum=cfile.substr(poss,pos-poss);
        if (cfile[pos]=='(') //macro function
        {
          pos++;
          bool an = 0;
          arg_count = 0;
          while (pos<len)
          {
            if (cfile[pos] == ')') { pos++; break; }
            else if (cfile[pos]==',') { if (an==0) { cferr="Identifier expected before ',' symbol"; return pos; } an=0; }
            else if (is_letter(cfile[pos]))
            {
              if (an) { cferr="Symbol ',' expected"; return pos; }
              
              const unsigned spos = pos;
              while (is_letterd(cfile[pos])) pos++;
              an=1; args[arg_count++] = cfile.substr(spos,pos-spos);
              continue;
            }
            
            else if (!is_useless_macros(cfile[pos]))
            {
              if (is_useless(cfile[pos]))
                cferr = "Unexpected newline in macro parameters";
              else
                cferr = string("Unexpected symbol '")+cfile[pos]+"' in macro parameters";
              return pos;
            }
            pos++;
          }
        }
        
        while (is_useless_macros(cfile[pos])) pos++;
        
        const unsigned poss2=pos;
        move_newline();
        
        string defiens=cfile.substr(poss2,pos-poss2);
        
        /*cout << "Define \"" << defiendum << "\" as \"" << defiens << "\"\r\n";
        if (current_scope->members.find(defiendum) != current_scope->members.end())
          current_scope->members.erase(defiendum);*/
        macro_type *t = &macros[defiendum];
        *t = defiens;
        if (arg_count != -1)
        {
          t->assign_func();
          for (int i=0; i<arg_count; i++)
            t->addarg(args[i]);
        }
      }
      if (next=="undef")
      {
        const unsigned poss=pos;
        if (!is_letter(cfile[pos])) { cferr="Identifier expected for #undef"; return pos; }
        while (is_letterd(cfile[pos])) pos++;
        
        const string uw = cfile.substr(poss,pos-poss);
        if (uw != "__attribute__") //_mingw.h is a little bitch
        macros.erase(uw);
      }
    }
    //Including things
    {
      if (next=="include")
      {
        while (cfile[pos]==' ' or cfile[pos]=='\t') pos++;
        
        const char c = cfile[pos];
        if (pos<len and cfile[pos] != '"' and cfile[pos] != '<')
        {
          cferr = "Expcted filename to include set in quotes or in <>";
          return pos;
        }
        
        //Isolate filename
        const int spos = ++pos;
        const char ce = c=='"'? c:'>';
        while (cfile[pos++] != ce);
        string file = cfile.substr(spos,pos-spos-1);
        move_newline();
        
        //Find the file and include it
        string ins;
        string include_from;
        if (c == '"')
        {
          if (included_files.empty())
            include_from = "";
          else
            include_from = included_files.top().path;
          
          ins = fc( (include_from+file).c_str() );
          if (fnf)
          {
            cferr = "Failed to include " + file + " from " + include_from + ": File not found";
            return pos;
          }
        }
        else
        {
          for (unsigned int i = 0; i < include_directory_count; i++)
          {
            include_from = include_directories[i];
            ins = fc( (include_from+file).c_str() );
            if (!fnf) break;
          }
          if (fnf)
          {
            cferr = "Failed to include " + file + " from " + include_from + ": File not found";
            return pos;
          }
        }
        
        int iline=0;
        for (unsigned int i=0; i<pos; i++)
        {
          if (cfile[i]=='\n')
            iline++;
        }
        
        c_file.push();
        position.push();
        cfile_length.push();
        
        
        cfile_top = cfile = ins;
        len = cfile.length();
        pos = 0;
        
        included_files.push(includings(file,include_from));
        
        cout << "Including file " << file << " from line " << iline << endl;
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
    
    if (next=="line")
    {
      cferr="#line is unimplemented for reasons of sanity.";
      return pos;
    }
  } //end if (!in_false_conditional())
  
  if (next=="pragma") //Visit this even in a false conditional for print and debug
  {
    const unsigned sp = pos;
    move_newline();
    const string pc = cfile.substr(sp,pos-sp);
    if (pc == "debug_entry_point" and !in_false_conditional())
      cout << "#pragma: debug_entry_point\r\n";
    if (pc == "debug_entry_point_unconditional")
      cout << "#pragma: debug_entry_point\r\n";
    if (pc.substr(0,8) == "println " and !in_false_conditional())
    {
      cout << "Debug output: " << pc.substr(8) << endl;
      fflush(stdout);
    }
    if (pc.substr(0,22) == "println_unconditional ")
    {
      cout << "Unconditional debug output: " << pc.substr(22) << endl;
      fflush(stdout);
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
        {
          flowstack.push(evaluate_expression(exp));
          if (rerrpos != -1) { cferr = "In #if expression at position " + tostring(rerrpos) + ": " + rerr; return spos+rerrpos; }
        }
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
          {
            flowstack.set_top(flowstack.topval() == 0 and !flowstack.topused() and evaluate_expression(exp));
            if (rerrpos != -1) { cferr = "In #if expression at position " + tostring(rerrpos) + ": " + rerr; return spos+rerrpos; }
          }
        }
        else //endif
          flowstack.pop();
      }
    }
    else if (in_false_conditional())
    {
      move_newline();
    }
  }
  
  
  return (unsigned)-1;
}
