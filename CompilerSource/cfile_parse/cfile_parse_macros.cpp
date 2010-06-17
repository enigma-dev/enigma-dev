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
#include <stack>
#include <string>
#include <iostream>
#include <cstdio>
#include "../general/darray.h"
#include "../general/implicit_stack.h"

using namespace std;

#include "value.h"
#include "../externs/externs.h"
#include "expression_evaluator.h"
#include "../general/parse_basics.h"
#include "cfile_pushing.h"

inline void move_newline()
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
    n* prev; //Next level down, obviously
    bool value; //If current level is true or false
    bool used; //If it has been true before, meaning an else has been used/no longer can be anyway
    n(): prev(NULL),value(0),used(0) {}
    n(bool x): prev(NULL), value(x), used(x)  {}
    n(n* p,bool x): prev(p), value(x), used(x) {}
    n(n* p,bool x,bool pre): prev(p), value(x), used(x or pre) {}
  } *top;
  flow_stack(): top(NULL) {}
  void push(bool x) { top = (top) ? new n(top,top->value and x,!top->value) : new n(top,x,0); }
  bool push_false() { if (!top) return 0; if (!top->value) top = new n(top,0,1); return !top->value; }
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

extern void print_definition(string);
pt cfile_parse_macro()
{
  while(is_useless_macros(cfile[++pos]));
  
  pt poss=pos;
  if (!is_letter(cfile[pos])) {
    cferr="Preprocessor directive expected";
    return pos;
  }
  while (is_letterd(cfile[pos])) pos++;
  string next=cfile.substr(poss,pos-poss);
  
  while (cfile[pos] == ' ' or cfile[pos] == '\t') pos++;
  
  if (!in_false_conditional())
  {
    if (next=="error")
    {
      pt poss=pos;
      move_newline();
      cferr="#error "+cfile.substr(poss,pos-poss+1);
      return pos;
    }
    //Define, Undefine
    {
      if (next=="define")
      {
        while (cfile[pos] == ' ' or cfile[pos] == '\t') pos++;
        
        const pt poss=pos;
        if (!is_letter(cfile[pos])) { cferr="Identifier expected for #define"; return pos; }
        while (is_letterd(cfile[pos])) pos++;
        
        //unsigned int flags=0;
        int arg_count = -1;
        int args_unlimited_at = -1;
        varray<string> args;
        
        string definiendum=cfile.substr(poss,pos-poss);
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
              
              const pt spos = pos;
              while (is_letterd(cfile[pos])) pos++;
              an=1; args[arg_count++] = cfile.substr(spos,pos-spos);
              continue;
            }
            else if (cfile[pos] == '.' and cfile[pos+1] == '.' and cfile[pos+2] == '.')
            {
              if (args_unlimited_at != -1){
                cferr = "Unexpected ellipsis in function parameters: already given";
                return pos;
              }
              args_unlimited_at = arg_count-an;
              pos += 2;
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
        
        const pt poss2=pos;
        move_newline();
        
        string definiens=cfile.substr(poss2,pos-poss2);
        
        /*cout << "Define \"" << definiendum << "\" as \"" << definiens << "\"\r\n";
        if (current_scope->members.find(definiendum) != current_scope->members.end())
          current_scope->members.erase(definiendum);*/
        macro_type *t = &macros[definiendum];
        *t = definiens;
        if (arg_count != -1)
        {
          t->assign_func(definiendum);
          for (int i=0; i<arg_count; i++)
            t->addarg(args[i]);
          if (args_unlimited_at != -1)
            t->set_unltd_args(args_unlimited_at);
        }
      }
      if (next=="undef")
      {
        const pt poss=pos;
        if (!is_letter(cfile[pos])) { cferr="Identifier expected for #undef"; return pos; }
        while (is_letterd(cfile[pos])) pos++;

        const string uw = cfile.substr(poss,pos-poss);
        if (uw != "__attribute__") //_mingw.h is a little bitch
          macros.erase(uw);
      }
    }
    //Including things
    {
      if (next=="include" or next=="include_next")
      {
        while (cfile[pos]==' ' or cfile[pos]=='\t') pos++;
        
        const char c = cfile[pos];
        if (pos<len and cfile[pos] != '"' and cfile[pos] != '<')
        {
          cferr = "Expcted filename to include set in quotes or in <>";
          return pos;
        }
        
        //Isolate filename
        const int spos = ++pos; //Start after opening symbol
        const char ce = c=='"'? c:'>'; //Get end char
        
        while (pos < len and cfile[pos++] != ce);
        string file = cfile.substr(spos,pos-spos-1);
        move_newline();
        
        //Find the file and include it
        string ins;
        string include_from;
        if (c == '"')
        {
          string qpath;
          for (unsigned i = file.length(); i; i--)
            if (file[i] == '\\' or file[i] == '/')
            {
              qpath = file.substr(0,i+1);
              file.erase(0,i+1);
              break;
            }
          if (included_files.empty())
            include_from = qpath;
          else
            include_from = included_files.top().path + qpath;
          
          ins = fc( (include_from+file).c_str() );
          if (fnf) {
            cferr = "Failed to include `" + file + "' from " + include_from + ": File not found";
            return pos;
          }
        }
        else
        {
          if (!include_directory_count) {
            cferr = "Failed to include " + file + ": Search path is empty!!!111111!!112";
            return pos;
          }
          
          // Loop through the search path, seeking the header
          for (unsigned int i = 0; i < include_directory_count; i++)
          {
            include_from = include_directories[i];
            ins = fc( (include_from+file).c_str() );
            if (!fnf) break;
          }
          
          if (fnf) {
            cferr = "Failed to include " + file + " from " + include_from + ": File not found";
            return pos;
          }
        }
        
        cfstack.push(new cfnode);
        cfile = ins;
        len = cfile.length();
        pos = 0;
        
        included_files.push(includings(file,include_from));
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
    // This is my beautiful debugging suite during development of the parser.
      #ifdef ENIGMA_PARSERS_DEBUG
        const pt sp = pos;
      #endif
    move_newline();
      #ifdef ENIGMA_PARSERS_DEBUG
        const string pc = cfile.substr(sp,pos-sp);
        if (pc == "debug_entry_point" and !in_false_conditional())
          cout << "#pragma: debug_entry_point\r\n";
        if (pc == "debug_entry_point_unconditional")
          cout << "#pragma: debug_entry_point\r\n";
        if (pc == "skiptell")
          cout << "#pragma: skipto status: ['"<<skipto<<"', '"<<skipto2<<"' : " << specializing << "]\r\n";
        if (pc == "toggledebug")
          cout << "#pragma: toggledebug = " << (cfile_debug = !cfile_debug) << "\r\n";
        if (pc == "ihtell")
          cout << "#pragma: ihc = " << ihc << "\r\n";
        if (pc == "tptell")
        {
          cout << "#pragma: tpc = " << tpc << " Current scope: " << current_scope->tempargs.size << "\r\n";
          for (int i=0; i<tpc; i++) cout << "  " << tmplate_params[i].name << endl;
          for (unsigned i=0; i<current_scope->tempargs.size; i++) cout << "  " << current_scope->tempargs[i]->name << endl;
        }
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
        if (pc.substr(0,10) == "printinfo ")
        {
          cout << "Define " << pc.substr(10) << ": " << endl;
          print_definition(pc.substr(10));
          fflush(stdout);
        }
        if (pc == "printlast")
        {
          cout <<
          "Last named: " << last_named << endl <<
          "Last named phase: " << last_named_phase << endl <<
          "Last identifier: " << last_identifier << endl <<
          "Last type: " << (void*)last_type << (last_type? ": " + strace(last_type) : "") << endl <<
          "Immediate scope: " << immediate_scope << (immediate_scope? ": " + strace(immediate_scope) : "") << endl <<
          "In false conditional: " << in_false_conditional() << endl <<
          endl;
          fflush(stdout);
        }
        if (pc.substr(0,10) == "tracescope")
        {
          string o;
          cout << "Tracing scope (" << (pc.length()>11?pc.substr(11):"no additional info") << "): ";
          
          string lnm;
          for (externs* i=immediate_scope?immediate_scope:current_scope; i != &global_scope; i=i->parent)
          {
            if (i == NULL) {
              cout << "ERROR! Trace wound up at NULL! ";
              break;
            }
            
            if (lnm != "")
            {
              extiter ii = i->members.find(lnm);
              if (ii == i->members.end())
                cout << "ERROR! Scope `" << i->name << "' offers no route back to `" << lnm << "'  ";
            }
            o = "::" + i->name + o;
            lnm = i->name;
          }
          if (lnm != "")
          {
            extiter ii = global_scope.members.find(lnm);
            if (ii == global_scope.members.end())
              cout << "ERROR! Global scope offers no route back to `" << lnm << "'  " << o << endl << endl;
            else cout << o << endl<< "Confirmed global scope traces back to `" << lnm << "'" << endl << endl;
          } else cout << endl << "At global scope." << endl << endl;

          fflush(stdout);
        }
      #endif
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
        if (!flowstack.push_false()) //If this expression will be evaluated as false anyway, just push it that way...
        {
          if (next.length() == 2) //#if
          {
            //We're allowed to cheat like this because there are no fucking strings.
            size_t p;
            for (p = exp.find("/*"); p != string::npos; p = exp.find("/*")) {
              size_t p2 = exp.find("*/",p+2);
              exp.erase(p,p2-p+1);
            } if ((p = exp.find("//")) != string::npos) exp.erase(p);
            flowstack.push(evaluate_expression(exp));
            if (rerrpos != -1) { cferr = "In #if expression at position " + tostring(rerrpos) + ": " + rerr; return spos+rerrpos; }
          }
          else if (next.length() == 5) { //ifdef
            int e = 0; while (is_letterd(exp[++e]));
            flowstack.push(macros.find(exp.substr(0,e)) != macros.end());
          }
          else if (next.length() == 6) { //ifndef
            int e = 0; while (is_letterd(exp[++e]));
            flowstack.push(macros.find(exp.substr(0,e)) == macros.end());
          }
        }
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
  
  
  return pt(-1);
}
