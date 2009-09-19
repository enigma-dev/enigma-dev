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

string cferr;

#include "cfile_parse_constants.h"
#include "cfile_parse_calls.h"

#define encountered pos++;
#define quote while (cfile[pos]!='"') { pos++; if (cfile[pos]=='\\' and (cfile[pos+1]=='\\'||cfile[pos]=='"')) pos+=2; }
#define squote while (cfile[pos]!='\'') { pos++; if (cfile[pos]=='\\' and (cfile[pos+1]=='\\'||cfile[pos]=='\'')) pos+=2; }

string tostring(int val);

unsigned int cfile_parse_macro(string& cfile,unsigned int& pos,const unsigned int len);
int keyword_operator(string& cfile,unsigned int &pos,int &last_named,int &last_named_phase,string &last_identifier);
#include "handle_letters.h"

int parse_cfile(string cftext)
{
  cferr="No error";

  bool preprocallowed=1;

  implicit_stack<string> c_file;
  #define cfile c_file()
  cfile = cftext;
  implicit_stack<unsigned int> position;
  #define pos position()
  pos = 0;
  implicit_stack<unsigned int> cfile_length;
  #define len cfile_length()
  len = cfile.length();

  externs *last_type = NULL;
  string last_identifier="";
  int last_named=LN_NOTHING;
  int last_named_phase=0;

  int plevel=0;/*
  int funclevel=-1;
  int fargs_named=0;
  int fargs_count=0;*/

  unsigned int *debugpos;
  debugpos = &pos;
  
  int skip_depth = 0;
  char skipto = 0, skipto2 = 0, skip_inc_on = 0;

  /*
    Okay, we have to break this into sections.
    We're going to blindly check what we are looking at RIGHT NOW.
    THEN we're going to see how that fits together with what we already know.

    In other words, this parser does not use a token tree.
    So, everything in this parser is handled in one pass.
    The closest thing to a token tree in this parser is a
    linked list I use for reference tracing.

    If you have a problem with this, please go to hell.
    I don't want to hear about it.
  */

  int anoncount = 0;

  for (;;)
  {
    if (!(pos<len))
    {
      if (c_file.ind>0) //If we're in a macro
      {
        //Move back down
        handle_macro_pop(c_file,position,cfile_length);
        continue;
      }
      else break;
    }
    
    
    if (is_useless(cfile[pos]))
    {
      if (cfile[pos]=='\r' or cfile[pos]=='\n')
        preprocallowed=true;
      pos++; continue;
    }
    
    
    //If it's a macro, deal with it here
    if (cfile[pos]=='#' and preprocallowed)
    {
      cfile_parse_macro(cfile,pos,len);
      continue;
    }
    
    //Not a preprocessor
    preprocallowed = false;
    
    if (skipto)
    {
      if (cfile[pos] == skipto or cfile[pos] == skipto2)
      {
        if (skip_depth == 0)
          skipto = skipto2 = skip_inc_on = 0;
        else
          skip_depth--;
      }
      else if (cfile[pos] == skip_inc_on)
        skip_depth++;
      
      pos++;
      continue;
    }
    
    
    //First, let's check if it's a letter.
    //This implies it's one of three things...
    if (is_letter(cfile[pos]))
    {
      unsigned int sp = pos;
      while (is_letterd(cfile[++pos])); // move to the end of the word
      string n = cfile.substr(sp,pos-sp); //This is the word we're looking at.
      
      //Macros get precedence. Check if it's one.
      if (handle_macros(n,c_file,position,cfile_length))
        continue;
        
      int diderrat = handle_identifiers(n,cferr,last_identifier,pos,last_named,last_named_phase,last_type);
      //cout << last_named << ":" << last_named_phase << "  ->  ";
      if (diderrat != -1)
        return diderrat;
      //cout << last_named << ":" << last_named_phase << "\r\n";
      continue;
    }


    //There is a select number of symbols we are supposed to encounter.
    //A digit is actually not one of them. Digits, most operators, etc,
    //will be skipped over when we see an = sign.

    //The symbol we will see most often is probably the semicolon.
    if (cfile[pos] == ',' or cfile[pos] == ';')
    {
      if (last_named == LN_NOTHING)
        { pos++; continue; }

      if ((last_named & LN_TYPEDEF) != 0)
      {
        if (last_identifier == "")
        {
          cferr = "No defiendum in type definition";
          return pos;
        }
        extiter it = current_scope->members.find(last_identifier);
        if (it != current_scope->members.end())
        {
          cferr = "Redeclaration of `"+last_identifier+"' as typedef at this point";
          return pos;
        }

        if (last_type == NULL)
        {
          cferr = "Program error: Type does not exist. An error should have been reported earlier.";
          return pos;
        }
        current_scope->members[last_identifier] = last_type;
      }
      else
      {
        switch (last_named)
        {
          case LN_DECLARATOR:
              //Can't error on last_named_phase != DEC_IDENTIFIER, or structs won't work
              if (refstack.currentsymbol() == '(')
              {
                pos++;
                continue;
              }
              last_named_phase = DEC_FULL; //reset to 4 for next identifier.
            break;
          case LN_TEMPLATE:
              cferr="Unused template declaration: Identifier expected before ;";
              return pos;
            break;
          //These can all be looked at the same at this point.
          case LN_CLASS: case LN_STRUCT:
          case LN_UNION: case LN_ENUM:
              if (cfile[pos] != ';')
              {
                cferr="Expected ';' instead of ',' when not implemented";
                return pos;
              }
              if (last_named_phase != 1)
              {
                cferr="Expected only identifier when not implemented";
                return pos;
              }
            break;
          case LN_NAMESPACE:
              if (cfile[pos] != ';')
              {
                cferr="Expected ';' for namespace declarations, not ','";
                return pos;
              }
              if (last_named_phase != NS_COMPLETE_ASSIGN) //If it's not a complete assignment, shouldn't see ';'
              {
                cferr = "Cannot define empty namespace; expect '=' and namespace identifier before ';'";
                return pos;
              }
            break;
          case LN_OPERATOR:
              if (last_named_phase == 0)
                cferr = "Expected overloadable operator before ';'";
              else cferr = "Expected parameters to operator overload at this point";
            return pos;
          case LN_USING:
              if (last_named_phase != 2)
              {
                cferr = "Nothing to use";
                return pos;
              }
            break;
        }
        
        if (last_identifier != "")
        if (!ExtRegister(last_named,last_identifier,refstack.dissociate(),last_type))
          return pos;
      }

      if (cfile[pos] == ';') //If it was ';' and not ','
      {
        if (plevel > 0)
        { //No semicolons in parentheses.
          cferr="Expected closing parentheses before ';'";
          return pos;
        }
        //Change of plans. Dump everything.
        last_named = LN_NOTHING;
        last_named_phase = 0;
        last_identifier = "";
        last_type = NULL;
        refstack.dump();
      }

      pos++;
      continue;
    }

    //You're looking at another symbol we have to watch out for right now.
    //I'm talking of / in comments, for those who don't like riddles.
    if (cfile[pos]=='/')
    {
      pos++;
      if (cfile[pos]=='/')
      {
        while (cfile[pos] != '\r' and cfile[pos] != '\n' and (pos++)<len);
        continue;
      }
      if (cfile[pos]=='*')
      {
        int spos=pos;
        pos+=2;

        while ((cfile[pos] != '/' or cfile[pos-1] != '*') and (pos++)<len);
        if (pos>=len)
        {
          cferr="Unterminating comment";
          return spos;
        }

        pos++;
        continue;
      }
      pos--;
    }

    //The next thing we want to do is check we're not expecting an operator for the operator keyword.
    if (last_named==LN_OPERATOR and last_named_phase != OP_PARAMS)
    {
      int a=keyword_operator(cfile,pos,last_named,last_named_phase,last_identifier);
      if (a != -1) return a;
      continue;
    }

    //Now that we're sure we aren't in an "operator" expression,
    //We can check for the few symbols we expect to see.

    //First off, the most common is likely to be a pointer indicator.
    if (cfile[pos]=='*')
    {
      //type should be named
      if ((last_named | LN_TYPEDEF) != (LN_DECLARATOR | LN_TYPEDEF))
      {
        cferr = "Unexpected '*'";
        return pos;
      }
      refstack += referencer('*');
      pos++; continue;
    }
    
    if (cfile[pos]=='[')
    {
      //type should be named
      if (last_named != LN_DECLARATOR)
      {
        cferr = "Unexpected '*'";
        return pos;
      }
      refstack += referencer('[');
      skipto = ']'; skip_inc_on = '[';
      pos++; continue;
    }

    if (cfile[pos] == '(')
    {
      if (last_named != LN_DECLARATOR)
      {
        if (last_named != LN_OPERATOR or last_named_phase != OP_PARAMS)
        {
          cferr = "Unexpected parenthesis at this point";
          return pos;
        }
        
        last_named = LN_DECLARATOR;
        last_named_phase = DEC_IDENTIFIER;
      }
      
      //In a declaration
      
      //<declarator> ( ... ) or <declarator> <identifier> ()
      refstack += referencer(last_named_phase == DEC_IDENTIFIER ? '(':')',0,last_named_phase != DEC_IDENTIFIER);
      plevel++;
      
      pos++; continue;
    }

    if (cfile[pos] == ')')
    {
      pos++;
      if (!(plevel > 0))
      {
        cferr = "Unexpected closing parenthesis: None open";
        return pos;
      }
      plevel--;
      refstack--;
      //last_named_phase=0;
      continue;
    }

    if (cfile[pos] == '{')
    {
      if (last_named == LN_NAMESPACE or (last_named | LN_TYPEDEF) == (LN_STRUCT | LN_TYPEDEF)
      or  (last_named | LN_TYPEDEF) == (LN_CLASS | LN_TYPEDEF))
      {
        unsigned int tflags = (last_named==LN_NAMESPACE)?EXTFLAG_NAMESPACE:EXTFLAG_TYPENAME | ((last_named==LN_STRUCT)?EXTFLAG_STRUCT:EXTFLAG_CLASS);
        if (last_identifier != "")
        {
          extiter addin = current_scope->members.find(last_identifier);
          if (addin != current_scope->members.end())
          {
            if (last_named != LN_NAMESPACE)
            {
              cferr = "Redeclaration of " + last_identifier + " at this point";
              return pos;
            }
            current_scope = addin->second;
          }
          else
            current_scope = current_scope->members[last_identifier] = new externs(last_identifier,current_scope,tflags);
        }
        else
          current_scope = current_scope->members["<anonymous"+tostring(anoncount++)+">"] = new externs(last_identifier,current_scope,tflags);
      }
      else
      {
        if (last_named == LN_DECLARATOR and refstack.nextsymbol() == '(') // Do not confuse with ')'
        {
          //Function implementation.
          
          //Register the function in the current scope
          if (!ExtRegister(last_named,last_identifier,refstack.dissociate(),last_type))
            return pos;
          
          //Skip the code: we don't need to know it ^_^
          skipto = '}'; skip_inc_on = '{';
        }
        else
        {
          cferr = "Expected scope declaration before '{'";
          return pos;
        }
      }

      last_identifier = "";
      last_named = LN_NOTHING;
      last_named_phase = 0;
      last_type = NULL;
      refstack.dump();
      pos++;

      continue;
    }

    if (cfile[pos] == '}')
    {
      if (current_scope == &global_scope)
      {
        cferr = "Unexpected closing brace at this point";
        return pos;
      }
      if (current_scope->flags & EXTFLAG_TYPENAME)
        last_named = LN_DECLARATOR; //if the scope we're popping serves as a typename
      else
        last_named = LN_NOTHING;//Not sure what to do with enum {}
      last_named_phase = 0;

      pos++;
      last_type = current_scope;

      current_scope = current_scope->parent;
      continue;
    }
    
    cferr = "Unknown symbol";
    return pos;
  }



  /*
  string pname = "";
  if (last_typedef != NULL) pname=last_typedef->name;
  cout << '"' << last_typename << "\":" << pname << " \"" << last_identifier << "\"\r\n";*/
  return -1;
}

