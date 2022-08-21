/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2014 Seth N. Hetu                                             **
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


#include <string>
#include <iostream>
#include <cstdio>
using namespace std;
#include "darray.h"

#include "general/parse_basics_old.h"
#include "compiler/output_locals.h"
#include "languages/language_adapter.h"

#include "settings.h"


typedef size_t pt; //Use size_t as our pos type; helps on 64bit systems.
map<string,char> edl_tokens; // Logarithmic lookup, with token.
typedef map<string,char>::iterator tokiter;

int scope_braceid = 0;
extern string tostring(int);

#include <Storage/definition.h>
static jdi::definition_scope *current_scope;

int dropscope()
{
  if (current_scope != main_context->get_global())
  current_scope = current_scope->parent;
  return 0;
}
int quickscope()
{
  jdi::definition_scope* ns = new jdi::definition_scope("{}",current_scope,jdi::DEF_NAMESPACE);
  current_scope->members["{}"+tostring(scope_braceid++)].reset(ns);
  current_scope = ns;
  return 0;
}
int initscope(string name)
{
  scope_braceid = 0;
  main_context->get_global()->members[name] = std::make_unique<jdi::definition_scope>(name,main_context->get_global(),jdi::DEF_NAMESPACE);
  current_scope = reinterpret_cast<jdi::definition_scope*>(main_context->get_global()->members[name].get());
  return 0;
}
int quicktype(unsigned flags, string name)
{
  current_scope->members[name] = std::make_unique<jdi::definition>(name,current_scope,flags | jdi::DEF_TYPENAME);
  return 0;
}

#include <API/context.h>
#include <System/macros.h>
#include <System/lex_cpp.h>

//Has conditional popping based on an attribute pushed with the value
struct stackif
{
  stackif* prev;
  char value,popifc;
  
  stackif(void):prev(NULL) { }
  stackif(char v):prev(NULL),value(v) { }
  stackif(stackif* p,char v,char i):prev(p),value(v),popifc(i) { }
  
  stackif* push(char v,char i)
  {
    stackif* r = new stackif(this,v,i);
    return r;
  }
  stackif* popif(char i)
  {
    if (prev == NULL or popifc != i)
      return this;

    stackif* r = prev;
    delete this;
    return r;
  }

  operator char()
  {
    return value;
  }
};


/**
  This part outputs somewhat well-formatted code.

  @func likesaspace(char c, char d)
    @return returns whether or not the two characters (as seen
    in syntax string) @param c and @param d should be separated
    by a space in the outputted code.

  @func print_the_fucker(string code,string synt)
  @summary Outputs the code in a well-formatted fashion.

**/

inline bool likesaspace(char c,char d)
{
  if (c==d)
    return 0;
  if (c==' ' or c=='{' or c=='}' or c==':' or c==';')
    return 0;
  if (d==' ' or d=='{' or d=='}' or d==':' or d==';' or d==',')
    return 0;
  if (c=='=' and !is_letterd(d))
    return 0;
  if (d=='=' and !is_letterd(c))
    return 0;
  if ((c=='(' or c=='[') and is_letterd(d))
    return 0;
  if ((d=='(' or d=='[' or d==']' or d==')') and is_letterd(c))
    return 0;
  if (c == '-' and d == '>')
    return 0;
  if (c == '.' or d == '.')
    return 0;
  return 1;
}

#include <fstream>

const char * indent_chars   =  "\n                                \
                                                                  \
                                                                  \
                                                                  \
                                                                  ";
static inline string string_settings_escape(
    string n, const setting::CompatibilityOptions &compat_opts) {
  if (!compat_opts.use_cpp_strings) {
    if (!n.length()) return "\"\"";
    if (n[0] == '\'' and n[n.length()-1] == '\'')
      n[0] = n[n.length() - 1] = '"';
  } else {
    if (!n.length()) {
      return "\'\'";
    }
  }
  for (size_t pos = 1; pos < n.length()-1; pos++)
  {
    switch (n[pos])
    {
      case '\\':
          if (n[pos+1] == '#') // Backslashes can only escape number signs
            n.erase(pos,1); // It happens to be escaping this one
          else
            n.insert(pos++,1,'\\'); // Ordinary backslash; escape it for C++.
        break;
      case '#':
          n[pos] = 'n'; // Newlines are expressed with #
          n.insert(pos++,1,'\\');
        break;
      case '\n': // Newlines are allowed in strings in GML, but not in C
          n[pos] = 'n';
          n.insert(pos++,1,'\\');
        break;
      case '\r': // Handle odd cases
          n[pos] = '\\';
          if (n[++pos] == '\n')
            n[pos] = 'n';
          else n.insert(pos,1,'n');
        break;
      case '"':
          n.insert(pos++,1,'\\');
        break;
    } 
  }
  return n;
}

void print_to_file(const enigma::parsing::ParseContext &ctex, string code,string synt,const unsigned int strc, const varray<string> &string_in_code,int indentmin_b4,ofstream &of) {
  //FILE* of = fopen("/media/HP_PAVILION/Documents and Settings/HP_Owner/Desktop/parseout.txt","w+b");
  FILE* of_ = fopen("/home/josh/Desktop/parseout.txt","ab");
  if (of_) { fprintf(of_,"%s\n%s\n\n\n",code.c_str(), synt.c_str()); fclose(of_); }
  //if (of == NULL) return;

  const int indentmin = indentmin_b4 + 1;
  int indc = 0,tind = 0,pars = 0;
  unsigned str_ind = 0;

  const pt len = code.length();
  for (pt pos = 0; pos < len; pos++)
  {
    switch (synt[pos])
    {
      case '{':
          tind = 0;
          of.write(indent_chars,indentmin+indc);
          if (indc < 256) indc+=2;
          of << '{';
          of.write(indent_chars,indentmin+indc);
        break;
      case '}':
          tind = 0;
          indc -= 2;
          of.write(indent_chars,indentmin+indc);
          of << '}';
          of.write(indent_chars,indentmin+indc);
        break;
      case ';':
        if (pars)
        {
          of << code[pos];
          of << ' ';
          break;
        }
        if (tind) tind = 0;
        case ':':
          of << code[pos];
          of.write(indent_chars,indentmin+indc+tind);
        break;
      case '(':
          if (pars) pars++;
          of << '(';
        break;
      case ')':
          if (pars) pars--;
          of << ')';
          if (pars == 1)
          {
            pars = 0;
            if (synt[pos+1] != '{' and synt[pos+1] != ';')
            {
              tind+=2;
              of.write(indent_chars,indentmin+indc+tind);
            }
          }
        break;
      case '"': case '\'':
          if (pars) pars--;
            if (str_ind >= strc) cout << "What the string literal.\n";
            of << string_settings_escape(string_in_code[str_ind], ctex.compatibility_opts).c_str();
            str_ind++;
            if (synt[pos+1] == '+' and synt[pos+2] == '"')
              synt[pos+1] = code[pos+1] = ' ';
        break;
      case 's':
      case 'f':
          pars = 1;
          of << code[pos];
        break;

      default:
          of << code[pos];
          if (likesaspace(synt[pos],synt[pos+1]))
            of << ' ';
        break;
    }
  }
  /*if (system("\"/media/HP_PAVILION/Documents and Settings/HP_Owner/Desktop/parseout.txt\""))
  if (system("gedit \"/media/HP_PAVILION/Documents and Settings/HP_Owner/Desktop/parseout.txt\""))
    printf("zomg fnf\r\n");*/
}

int parser_fix_templates(string &code,pt pos,pt spos,string *synt)
{
  cout << "qass: " << pos << " <" << ((synt && code.length()) == (synt && synt->length()) ? "equivalent" : "UNEQUAL") << "> [" << (pos > code.length()) << "]";
  pt epos = pos;
  int a2i = 0;
  
  if (code[--epos] == '>')
  {
    --epos;
    int ac = 0; bool ga = false; // argument count, given argument = whether or not anything other than whitespace was specified for the arg type
    for (int tbc = 1; tbc > 0; epos--) // track triangle bracket count 
    {
      if (code[epos] == '<')
        { tbc--; continue; }
      tbc += (code[epos] == '>');
      ac +=  (code[epos] == ',') and ga and (tbc == 1);
      ga |=  (code[epos] != '<'  and code[epos] != '>' and !is_useless(code[epos]));
      ga &=  (code[epos] != ',');
    }
    a2i = ac + ga;
  }
  
  pt sp2 = spos;
  while (code[sp2] != '<' and sp2 < epos)
    if (code[sp2++] == ' ') spos = sp2;
  
  cout << " <" << ((synt && code.length()) == (synt && synt->length()) ? "equivalent" : "UNEQUAL") << "> [" << (pos > code.length()) << "]";
  cout << "ass: " << spos << ", " << epos << ": " << code.length() << endl;
  string ptname = code.substr(spos,epos-spos+1); // Isolate the potential template's name
  jdi::definition* a = current_language->look_up(ptname);
  if (!a) return 0;
  
  if (a->flags & jdi::DEF_TEMPLATE)
  {
    jdi::definition_template *tmp = (jdi::definition_template*)a;
    int tmc = tmp->params.size() - 1;
    for (int i = tmc; i >= 0; i--)
      if (tmp->params[i]->default_assignment) tmc = i;
    a2i = tmc - a2i;
    string iseg;
    for (int i = 0; i < a2i;)
      iseg += (++i < a2i) ? "variant," : "variant";
    if (code[pos-1] == '>')
    {
      if (code[pos-2] == ',' or code[pos-2] == '<')
      {
        if (iseg.length())
        {
          code.insert(pos-1, iseg);
          synt && (synt->insert(pos-1, string(iseg.length(),'t')),   true);
          return iseg.length();
        }
        else if (code[pos-2] == ',')
        {
          code.erase(pos-2,1);
          synt->erase(pos-2,1);
          return -1;
        }
      }
      else if (iseg.length())
      {
        code.insert(pos-1, ","+iseg),
        synt && (synt->insert(pos-1, string(iseg.length()+1,'t')), true);
        return iseg.length() + 1;
      }
    }
    else
    {
      code.insert(pos, "<"+iseg+">"),
      synt && (synt->insert(pos,   string(iseg.length()+2,'t')),   true);
      return iseg.length() + 2;
    }
  }
  return 0;
}
