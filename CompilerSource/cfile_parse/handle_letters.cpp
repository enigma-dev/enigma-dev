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

#include <stack>
#include <string>
#include <iostream>
using namespace std;
#include "../general/darray.h"
#include "../general/implicit_stack.h"
#include "../externs/externs.h"
#include "cfile_parse_constants.h"

unsigned int macrod=0;
varray<string> inmacros;

#include "macro_functions.h"

typedef implicit_stack<string> iss;
typedef implicit_stack<unsigned int> isui;

struct includings { string name; string path; };
extern stack<includings> included_files;
extern externs* builtin_type__int;

extern string cfile_top;
extern string cferr_get_file_orfirstfile();
int get_line(string& cfile,const unsigned int pos)
{
  int iline=0;
    for (unsigned int i=0; i<pos; i++)
    {
      if (cfile[i]=='\n')
        iline++;
    }
  return iline;
}

void handle_macro_pop(iss &c_file,isui &position,isui &cfile_length)
{
  //const int pos = position();
  
  c_file.pop();
  position.pop();
  cfile_length.pop();
  if (macrod > 0)
    macrod--;
  else if (!included_files.empty())
  {
    //print_cfile_top(pos);
    cfile_top=c_file();
    included_files.pop();
  }
}

extern string cferr;
string tostring(int val);
#include "../general/parse_basics.h"
bool ExtRegister(unsigned int last,string name,rf_stack refs,externs *type,varray<tpdata> &tparams, int tpc = 0);

//struct a { struct a *b; }, not struct a { a* b }
bool extreg_deprecated_struct(bool idnamed,string &last_identifier,int &last_named,int & last_named_phase, externs *&last_type)
{
  if (last_identifier == "")
  {
    cferr = "Identifier expected";
    return 0;
  }
  if (!find_extname(last_identifier,EXTFLAG_TYPENAME))
  {
    if (current_scope->members.find(last_identifier) != current_scope->members.end())
    {
      cferr = "`"+last_identifier+"' does not name a type.";
      return 0;
    }
    rf_stack NO_REFS;
    varray<tpdata> EMPTY;
    ExtRegister(LN_STRUCT,last_identifier,NO_REFS,NULL,EMPTY,0);
  }
  last_named = LN_DECLARATOR;
  last_named_phase = idnamed?DEC_IDENTIFIER:DEC_FULL;
  last_type = ext_retriever_var;
  return 1;
}

unsigned int handle_macros(const string n,iss &c_file,isui &position,isui &cfile_length)
{
  maciter t;
  if ((t=macros.find(n)) != macros.end())
  {
    bool recurs=0;
    for (unsigned int iii=0;iii<macrod;iii++)
    if (inmacros[iii]==n) { recurs=1; break; }
    if (!recurs)
    {
      string macrostr = t->second;
      
      #define cfile c_file()
      #define pos position()
        if (t->second.argc != -1) //Expect ()
        {
          if (!macro_function_parse(cfile,pos,macrostr,t->second.args,t->second.argc))
          {
            cferr = macrostr;
            return pos;
          }
        }
      #undef cfile
      #undef pos
      
      //const int cpos = position();
      
      //Push everything
      c_file.push();
      position.push();
      cfile_length.push();
      
      //print_cfile_top(cpos);
      cfile_top = c_file();
      
      //Set everything
      position() = 0;
      c_file() = macrostr;
      cfile_length() = c_file().length();
      
      inmacros[macrod++] = n;
      return unsigned(-2);
    }
  }
  return unsigned(-1);
}

extern bool is_tflag(string x);
int handle_identifiers(const string n,string &last_identifier,unsigned int &pos,int &last_named,int &last_named_phase,externs* &last_type,int &fparam_named)
{
  //it's not a macro, so the next thing we'll check for is keyword
  if (n=="struct" or n=="class")
  {
    //Struct can only really follow typedef.
    if (last_named != LN_NOTHING)
    {
      if (last_named != LN_TYPEDEF)
      {
        if (last_named != LN_DECLARATOR or !(last_named_phase == 0 or refstack.currentsymbol() == '('))
        {
          cferr="Unexpected `"+n+"' token";
          return pos;
        }
      }
      else
      {
        last_named |= LN_STRUCT;
      }
    }
    else last_named = LN_STRUCT;
    return -1;
  }
  if (n=="enum")
  {
    //Same with enum
    if (last_named != LN_NOTHING)
    {
      if (last_named != LN_TYPEDEF)
      {
        cferr="Unexpected `enum' token";
        return pos;
      }
      else
        last_named |= LN_ENUM;
    }
    else
      last_named = LN_ENUM;
    return -1;
  }
  if (n=="typedef")
  {
    //Typedef can't follow anything
    if (last_named != LN_NOTHING)
    {
      cferr="Unexpected `struct' token";
      return pos;
    }
    last_named = LN_TYPEDEF;
    return -1;
  }
  if (n=="extern")
  { //This doesn't tell us anything useful unless the next token is "C"
    if (last_named != LN_NOTHING)
    {
      if (last_named != LN_DECLARATOR)
      {
        cferr = "Unexpected `extern' token at this point";
        return pos;
      }
      if (last_named_phase != DEC_LONG 
      and last_named_phase != DEC_LONGLONG 
      and last_named_phase != DEC_GENERAL_FLAG
      and last_named_phase != DEC_NOTHING_YET)
      {
        cferr = "Unexpected `extern' token at this point";
        return pos;
      }
      return -1;
    }
    last_named = LN_DECLARATOR;
    last_named_phase = 0;
    return -1;
  }
  if (n=="union")
  {
    //Union can only really follow typedef, if it's not on its own.
    if (last_named != LN_NOTHING)
    {
      if (last_named != LN_TYPEDEF)
      {
        cferr="Unexpected `union' token";
        return pos;
      }
      else
      {
        last_named |= LN_UNION;
      }
    }
    else last_named = LN_UNION;
    return -1;
  }
  if (n=="namespace")
  {
    //Namespace can only follow using, when it's not on its own.
    if (last_named != LN_NOTHING)
    {
      if (last_named != LN_USING or last_named_phase != USE_NOTHING)
      {
        cferr="Unexpected `namespace' token";
        return pos;
      }
      last_named_phase = USE_NAMESPACE; //using namespace...
    }
    else
      last_named = LN_NAMESPACE; //namespace...
    return -1;
  }
  if (n=="explicit")
  { //This is for GCC to know, and us to just be okay with.
    return -1;
  }
  if (n=="operator")
  {
    if (last_named != LN_DECLARATOR)
    {
      cferr="Expected declarator before `operator' token";
      return pos;
    }
    if (last_named_phase<1)
    {
      cferr="Declarator before `operator' token names no type";
      return pos;
    }
    last_named=LN_OPERATOR;
    last_named_phase=0;
    cout << last_named << " = " << last_named_phase << "\r\n";
    return -1;
  }
  if (n=="new")
  {
    //New must only follow keyword "operator" or an = outside of functions
    //In the case of =, it will be skipped anyway. Check for "operator".
    if (last_named != LN_OPERATOR)
    {
      if (last_named != LN_NOTHING)
        cferr="Expect `new' token only after `operator' token or as initializer";
      else
        cferr="Expected identifier before `new' token";
      return pos;
    }
    last_named_phase = OP_NEWORDELETE;
    last_identifier = "operator new";
    return -1;
  }
  if (n=="delete")
  {
    //Delete must only follow keyword "operator" outside of functions
    if (last_named != LN_OPERATOR)
    {
      cferr="Expect `delete' token only after `operator' token";
      return pos;
    }
    last_named_phase = OP_NEWORDELETE;
    last_identifier = "operator delete";
    return -1;
  }
  if (n=="template")
  {
    if (last_named != LN_NOTHING)
    {
      cferr = "Unexpected `template' token";
      return pos;
    }
    last_named = LN_TEMPLATE;
    last_named_phase = 0;
    return -1;
  }
  if (n=="typename")
  {
    if (last_named != LN_TEMPLATE or last_named_phase != TMP_PSTART)
    {
      cferr = "Unexpected `typename' token";
      return pos;
    }
    last_named_phase = TMP_TYPENAME;
    return -1;
  }
  if (n=="inline")
  {
    return -1;
  }
  if (n == "throw")
  {
    if (last_named != LN_DECLARATOR or (refstack.currentsymbol() != '(' and refstack.currentsymbol() != ')'))
    { cferr = "Unexpected `throw' token"; return pos; }
    last_named_phase = DEC_THROW;
    return -1;
  }
  if (n=="const" or n=="__const") //or for that matter, if n fucking= ____const__
    return -1; //Put something here if const ever fucking matters
  if (n=="class")
  if (n=="friend")
  if (n=="private")
  if (n=="public" or n=="protected" or n=="using")
  if (n=="virtual")
  if (n=="mutable")
  return -1;
  
  //Next, check if it's a type name.
  //If flow allows, this should be moved before the keywords section.
  
  //Check if it's a modifier
  if (is_tflag(n))
  {
    //last_typename += n + " "; Why bother
    last_type = builtin_type__int;
    
    if (last_named==LN_NOTHING)
    {
      last_named = LN_DECLARATOR;
      if (n=="long")
        last_named_phase = DEC_LONG;
      else
        last_named_phase = DEC_GENERAL_FLAG;
      return -1;
    }
    
    if ((last_named | LN_TYPEDEF) == (LN_DECLARATOR | LN_TYPEDEF))
    {
      if (last_named_phase != DEC_FULL)
      {
        if (last_named_phase != DEC_IDENTIFIER)
        {
          if (n=="long")
          {
            if (last_named_phase == DEC_NOTHING_YET
            or  last_named_phase == DEC_GENERAL_FLAG)
              last_named_phase = DEC_LONG;
            else if (last_named_phase == DEC_LONG)
              last_named_phase = DEC_LONGLONG;
            else
            {
              if (last_named_phase == DEC_LONGLONG)
                cferr="Type is too long for GCC";
              else
                cferr="Unexpected `long' modifier at this point";
              return pos;
            }
          }
          else if (last_named_phase == 0)
            last_named_phase = DEC_GENERAL_FLAG;
        }
        else
        {
          if (refstack.currentsymbol() != '(')
          {
            cferr = "Expected ';' before new declaration";
            return pos;
          }
          fparam_named = 1;
          return -1;
        }
      }
      else if ((last_named | LN_TYPEDEF) == (LN_TEMPLATE | LN_TYPEDEF) and (last_named_phase == TMP_EQUALS or last_named_phase == TMP_DEFAULTED))
      {
        last_named_phase = TMP_DEFAULTED;
        return -1;
      }
      else
      {
        cferr="Unexpected declarator at this point";
        return pos;
      }

      return -1;
    }
    if (last_named==LN_TYPEDEF) //if typedef is single, phase==0
    {
      last_named=LN_DECLARATOR | LN_TYPEDEF;
      if (n=="long")
        last_named_phase = DEC_LONG;
      else
        last_named_phase = DEC_GENERAL_FLAG;
      return -1;
    }
    
    if ((last_named | LN_TYPEDEF) == (LN_TEMPLATE | LN_TYPEDEF) and (last_named_phase == TMP_EQUALS or last_named_phase == TMP_DEFAULTED))
    {
      last_named_phase = TMP_DEFAULTED;
      return -1;
    }
    
    cferr="Unexpected declarator at this point";
    return pos;
  }
  
  //Check if it's a primitive or anything user defined that serves as a type.
  if (find_extname(n,EXTFLAG_TYPENAME))
  {
    if (last_named == LN_NOTHING or last_named == LN_TYPEDEF)
    {
      last_type = ext_retriever_var;
      last_named |= LN_DECLARATOR;
      last_named_phase = DEC_FULL;
      return -1;
    }
    
    //If we're declaring a variable by type
    if ((last_named | LN_TYPEDEF) == (LN_DECLARATOR | LN_TYPEDEF))
    {
      if (last_named_phase != DEC_FULL)
      {
        if (last_named_phase != DEC_IDENTIFIER)
        {
          last_type = ext_retriever_var;
          last_named_phase = DEC_FULL;
        }
        else if (refstack.currentsymbol() != '(') //int a b, as opposed to int a(b)
        {
          cferr = "Expected ';' before new declaration; old declaration is of type " + (last_type?last_type->name:string("NULL")) + " as " + last_identifier;
          return pos;
        }
        fparam_named = 1;
        return -1;
      } 
      //If error, or if it was declared in this scope
      else if (ext_retriever_var == NULL or ext_retriever_var->parent == current_scope)
      {
        cferr = ext_retriever_var == NULL ? "Type unimplemented in this scope" : "Two types named in declaration";
        return pos;
      }
      //If we made it this far, we are redeclaring something in this scope that is different in higher scopes
      //These next segments of elses are skipped, and the variable is treated like new.
    }
    //Check if we're declaring a new struct or something instead
    else //Last isn't a declarator. Maybe it's a struct or something?
    if ((last_named | LN_TYPEDEF) == (LN_STRUCT | LN_TYPEDEF)
    or  (last_named | LN_TYPEDEF) == (LN_ENUM   | LN_TYPEDEF)
    or  (last_named | LN_TYPEDEF) == (LN_CLASS  | LN_TYPEDEF))
    {
      //We're dealing with struct structid
      //If we're not right after "struct" (or the like) or are capable of redeclaring it in this scope
      if (last_named_phase != SP_EMPTY)
      {
        cferr = "Structure already identified, expected undeclared identifier";
        return pos;
      }
      //This shouldn't really happen
      if (ext_retriever_var == NULL)
      {
        cferr = "Cannot work with NULL type `"+n+"': This error should not occur";
        return pos;
      }
      //If this structid can be redeclared in this scope
      if (ext_retriever_var->parent == current_scope)
      {
        //Report that we're now a declarator (or typedef declarator) and return
        last_named = LN_DECLARATOR | (last_named & LN_TYPEDEF);
        last_named_phase = DEC_FULL;
        last_type = ext_retriever_var;
        return -1;
      }
      //Past this point, the type will be redeclared in this scope.
    }
    //Not declaring var by type, see if we're giving a template parameter a default value
    else if ((last_named | LN_TYPEDEF) == (LN_TEMPLATE | LN_TYPEDEF) and (last_named_phase == TMP_EQUALS or last_named_phase == TMP_DEFAULTED))
    {
      last_named_phase = TMP_DEFAULTED;
      last_type = ext_retriever_var;
      return -1;
    }
    //Not declaring by type or giving default template value
    else //Note: This else is here because the above will need to pass this block     //struct a;
    {    //in the case of the current type being redeclared as scalar in this scope   //namespace b { int a; }
      cferr = "Unexpected declarator at this point";
      return pos;
    }
  }

  //Here's the big part
  //We now assume that what was named is a regular identifier.
  //This means we do a lot of error checking here.
  if (last_named == LN_NOTHING) //what we have here is a standalone identifier.
  {
    if (!(current_scope != &global_scope and current_scope->flags & EXTFLAG_ENUM))
    {
      cferr="Expected type name or keyword before identifier";
      return pos;
    }
    else //Standalone identifer is okay in an enum
    {
      last_named = LN_DECLARATOR;
      last_named_phase = DEC_IDENTIFIER;
      last_type = builtin_type__int;
      last_identifier = n;
      return unsigned(-1);
    }
  }
  if (last_named == LN_TYPEDEF) //plain typedef, not typedef | declarator
  {
    cferr="Type definition does not specify a type";
    return pos;
  }

  //bool is_td = last_named & LN_TYPEDEF;
  switch (last_named & ~LN_TYPEDEF)
  {
    case LN_DECLARATOR:
        if (last_named_phase == DEC_IDENTIFIER)
        {
          if (refstack.currentsymbol() != '(')
          {
            cferr="Expected ',' or ';' before identifier";
            return pos;
          }
          fparam_named = 1;
          return -1;
        }
        last_named_phase = DEC_IDENTIFIER;
      break;
    case LN_TEMPLATE:
        if (last_named_phase == TMP_TYPENAME)
        {
          last_named_phase = TMP_IDENTIFIER;
          break;
        }
    case LN_CLASS:
    case LN_STRUCT:
    case LN_UNION:
        if (last_named_phase != SP_EMPTY) //Probably shouldn't be an identifier here
        {
          if (last_named_phase != SP_IDENTIFIER) //Shouldn't be an identifier here
          {
            cferr="Unexpected identifier in declaration";
            return pos;
          }
          //struct a { struct a b; }
          if (!extreg_deprecated_struct(true,last_identifier,last_named,last_named_phase,last_type))
            return pos;
          break;
        }
        last_named_phase = 1;
      break;
    case LN_ENUM:
        if (last_named_phase == 1 or last_named_phase == 3)
        {
          cferr="Expected '{' before identifier";
          return pos;
        }
        if (last_named_phase == 0)
          last_named_phase = 1;
        else if (last_named_phase == 2)
          last_named_phase = 3;
      break;
    case LN_NAMESPACE:
        if (last_named_phase == NS_NOTHING)
          last_named_phase = NS_IDENTIFIER;
        else 
        {
          if (last_named_phase == NS_EQUALS)
          cferr = "Nonexisting namspace given in synonymous namespace definition";
          else cferr = "Unexpected identifier in namespace definition";
          return pos;
        }
      break;
    case LN_OPERATOR:
        cferr = "Unexpected identifier in operator statement";
      return pos;
    case LN_USING:
        if (last_named_phase == USE_NAMESPACE_IDENTIFIER)
        {
          cferr = "Namespace to use already specified";
          return pos;
        }
        if (last_named_phase == USE_SINGLE_IDENTIFIER)
        {
          cferr = "Identifier to use already specified";
          return pos;
        }
        if (last_named_phase == USE_NAMESPACE)
        {
          if (!find_extname(n,LN_NAMESPACE))
          {
            cferr = "Expected namespace identifier following `namespace' keyword";
            return pos;
          }
          last_named_phase = USE_NAMESPACE_IDENTIFIER;
          break;
        }
        last_named_phase = USE_SINGLE_IDENTIFIER;
      break;
    default:
      cferr = "Unspecified Error. This shouldn't happen...";
      return pos;
      //last_named = LN_IDENTIFIER;
      //last_named_phase = 0;
  }

  last_identifier = n;
  return -1;
}
