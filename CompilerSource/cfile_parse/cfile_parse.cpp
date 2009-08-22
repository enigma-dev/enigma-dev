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
#include "darray.h"
#include "implicit_stack.h"

using namespace std;

#include "value.h"
#include "externs.h"
#include "expression_evaluator.h"

string cferr;

#include "cfile_parse_constants.h"
#include "cfile_parse_calls.h"

#define encountered pos++;
#define quote while (cfile[pos]!='"') { pos++; if (cfile[pos]=='\\' and (cfile[pos+1]=='\\'||cfile[pos]=='"')) pos+=2; }
#define squote while (cfile[pos]!='\'') { pos++; if (cfile[pos]=='\\' and (cfile[pos+1]=='\\'||cfile[pos]=='\'')) pos+=2; }


unsigned int cfile_parse_macro(string& cfile,unsigned int& pos,const unsigned int len);
extern inline int keyword_operator(string& cfile,unsigned int &pos,int &last_named,int &last_named_phase,string &last_identifier);
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
  
  unsigned int macrod=0;
  varray<string> inmacros;
  
  string last_typename="";
  string last_identifier="";
  int last_named=LN_NOTHING;
  int last_named_phase=0;
  
  externs *last_typedef = NULL;
  
  int plevel=0;
  int funclevel=-1;
  int fargs_named=0;
  int fargs_count=0;
  
  unsigned int *debugpos;
  debugpos = &pos;
  
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
  
  for (;;)
  {
    if (!(pos<len))
    {
      if (c_file.ind>0) //If we're in a macro
      {
        //Move back down
        c_file.pop();
        position.pop();
        cfile_length.pop();
        macrod--;
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
    if (cfile[pos]=='#')
    {
      cfile_parse_macro(cfile,pos,len);
      continue;
    }
    
    //Not a preprocessor
    preprocallowed = false;
    
    //First, let's check if it's a letter.
    //This implies it's one of three things...
    if (is_letter(cfile[pos]))
    {
      unsigned int sp = pos;
      while (is_letterd(cfile[++pos])); // move to the end of the word
      string n = cfile.substr(sp,pos-sp); //This is the word we're looking at.
      
      //Macros get precedence. Check if it's one.
      maciter t;
      if ((t=macros.find(n)) != macros.end())
      {
        bool recurs=0;
        for (unsigned int iii=0;iii<macrod;iii++)
        if (inmacros[iii]==n) { recurs=1; break; }
        if (!recurs)
        {
          c_file.push();
          position.push();
          cfile_length.push();
          pos = 0;
          cfile = t->second;
          len = cfile.length();
          inmacros[macrod++]=n;
          continue;
        }
      }
      
      //it's not a macro, so the next thing we'll check for is keyword
      if (n=="struct")
      {
        //Struct can only really follow typedef.
        if (last_named != LN_NOTHING)
        {
          if (last_named != LN_TYPEDEF)
          {
            cferr="Unexpected `struct' token";
            return pos;
          }
          else
          {
            last_named |= LN_STRUCT;
          }
        }
        else last_named = LN_STRUCT;
        continue;
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
        continue;
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
        continue;
      }
      if (n=="extern")
      { //This doesn't tell us anything useful.
        continue;
      }
      if (n=="union")
      {
        //Struct can only really follow typedef.
        if (last_named != LN_NOTHING)
        {
          if (last_named != LN_TYPEDEF)
          {
            cferr="Unexpected `struct' token";
            return pos;
          }
          else
          {
            last_named |= LN_UNION;
          }
        }
        else last_named = LN_UNION;
        continue;
      }
      if (n=="namespace")
      {
        //Namespace can't follow anything.
        if (last_named != LN_NOTHING)
        {
          if (last_named != LN_USING or last_named_phase != 0)
          {
            cferr="Unexpected `namespace' token";
            return pos;
          }
          last_named_phase = 1; //using namespace...
        }
        else
          last_named = LN_NAMESPACE; //namespace...
        continue;
      }
      if (n=="explicit")
      { //This is for GCC to know, and us to just be okay with.
        continue;
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
        continue;
      }
      if (n=="new")
      {
        //New must only follow keyword "operator" or an = outside of functions
        if (last_named != LN_OPERATOR)
        {
          if (last_named != LN_NOTHING)
            cferr="Expect `new' token only after `operator' token or as initializer";
          else
            cferr="Expected identifier before `new' token";
          return pos;
        }
        last_named_phase=3;
        last_identifier="operator new";
        continue;
      }
      if (n=="delete")
      {
        //Delete must only follow keyword "operator" outside of functions
        if (last_named != LN_OPERATOR)
        {
          cferr="Expect `delete' token only after `operator' token";
          return pos;
        }
        last_named_phase=3;
        last_identifier="operator delete";
        continue;
      }
      if (n=="template")
      if (n=="typename")
      if (n=="class")
      if (n=="friend")
      if (n=="private")
      if (n=="public" or n=="protected" or n=="using")
      if (n=="inline")
      if (n=="virtual")
      if (n=="mutable")
      continue;
      
      //Next, check if it's a type name.
      //If flow allows, this should be moved before the keywords section.
      
      //Check if it's a modifier
      if (is_tflag(n))
      {
        last_typename += n + " ";
        last_typedef = global_scope.members.find("int")->second;
        if (last_named==LN_NOTHING)
        {
          last_named=LN_DECLARATOR;
          if (n=="long")
            last_named_phase = 2;
          else
            last_named_phase = 1;
          continue;
        }
        if ((last_named | LN_TYPEDEF) == (LN_DECLARATOR | LN_TYPEDEF))
        {
          if (last_named_phase != 4)
          {
            if (n=="long")
            {
              if (last_named_phase == 0)
                last_named_phase = 2;
              else if (last_named_phase == 2)
                last_named_phase = 3;
              else
              {
                if (last_named_phase == 3)
                  cferr="Type is too long for GCC";
                else
                  cferr="Unexpected `long' modifier at this point";
                return pos;
              }
            }
            else if (last_named_phase == 0)
              last_named_phase = 1;
          }
          else
          {
            cferr="Unexpected declarator at this point";
            return pos;
          }
          
          continue;
        }
        if (last_named==LN_TYPEDEF) //if typedef is single, phase==0
        {
          last_named=LN_DECLARATOR | LN_TYPEDEF;
          if (n=="long")
            last_named_phase = 2;
          else
            last_named_phase = 1;
          continue;
        }
        
        cferr="Unexpected declarator at this point";
        return pos;
      }
      
      
      //Check if it's a primitive or anything user defined that serves as a type.
      if (find_extname(n,EXTFLAG_TYPENAME))
      {
        last_typename += n + " ";
        last_typedef = ext_retriever_var;
        if (last_named == LN_NOTHING)
        {
          last_named=LN_DECLARATOR;
          last_named_phase=4;
          continue;
        }
        if ((last_named | LN_TYPEDEF) == (LN_DECLARATOR | LN_TYPEDEF))
        {
          if (last_named_phase != 4)
          last_named_phase=4;
          else
          {
            cferr = "Two types named in declaration";
            return pos;
          }
          continue;
        }
        if (last_named == LN_TYPEDEF)
        {
          last_named |= LN_DECLARATOR;
          last_named_phase = 4;
          continue;
        }
        cferr = "Unexpected declarator at this point";
        return pos;
      }
      
      //Here's the big part
      //We now assume that the named is an identifier.
      //This means we do a lot of error checking here.
      if (last_named == LN_NOTHING)
      {
        cferr="Expected type name or keyword before identifier";
        return pos;
      }
      if (last_named == LN_TYPEDEF)
      {
        cferr="Type definition does not specify a type";
        return pos;
      }
      
      bool is_td = last_named & LN_TYPEDEF;
      switch (last_named & ~LN_TYPEDEF)
      {
        case LN_DECLARATOR:
            if (last_named_phase == 5)
            {
              cferr="Expected ',' or ';' before identifier";
              return pos;
            }
            last_named_phase = 5;
          break;
        case LN_TEMPLATE:
            if (last_named_phase != 2)
            {
              cferr="Unexpected identifier in template declaration";
              return pos;
            }
            last_named_phase=3;
          break;
        case LN_CLASS:
            if (last_named_phase != 0)
            {
              cferr="Unexpected identifier in class declaration";
              return pos;
            }
            last_named_phase = 1;
          break;
        case LN_STRUCT:
            if (last_named_phase != 0)
            {
              cferr="Unexpected identifier in struct declaration";
              return pos;
            }
            last_named_phase = 1;
          break;
        case LN_UNION:
             if (last_named_phase != 0)
            {
              cferr="Unexpected identifier in union declaration";
              return pos;
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
            if (last_named_phase == 0)
              last_named_phase = 1;
            else
            {
              cferr = "Unexpected identifier in namespace definition";
              return pos;
            }
          break;
        case LN_OPERATOR:
            cferr = "Unexpected identifier in operator statement";
          return pos;
        case LN_USING:
            if (last_named_phase == 2)
            {
              cferr = "Namespace to use already specified";
              return pos;
            }
            if (last_named_phase == 3)
            {
              cferr = "Identifier to use already specified";
              return pos;
            }
            if (last_named_phase == 1)
            {
              if (!find_extname(n,LN_NAMESPACE))
              {
                cferr = "Expected namespace identifier following `namespace' keyword";
                return pos;
              }
              last_named_phase = 2;
              break;
            }
            last_named_phase = 3;
          break;
      }
      
      last_named = LN_IDENTIFIER;
      last_named_phase = 0;
      last_identifier = n;
      
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
        
        if (last_typedef==NULL)
        {
          cferr = "Program error: Type does not exist. An error should have been reported earlier.";
          return pos;
        }
        current_scope->members[last_identifier] = last_typedef;
      }
      else 
      {
        switch (last_named)
        {
          case LN_DECLARATOR:
              last_named_phase = 4; //reset to 4 for next identifier.
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
              if (last_named_phase != 3)
              {
                cferr = "Cannot define empty namespace; expect `= namespaceid' before ';'";
                return pos;
              }
            break;
          case LN_OPERATOR:
              if (last_named_phase == 0)
                cferr = "Expected overloadable operator at this point";
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
        
        if (!ExtRegister(last_named,last_identifier,last_typedef))
          return pos;
      }
        
      if (cfile[pos] == ';')
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
        last_typedef = NULL;
        last_typename = "";
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
      if (last_named != LN_DECLARATOR)
      {
        if (last_named != LN_STRUCT 
        and last_named != LN_CLASS 
        and last_named != LN_UNION)
        {
          cferr="Expected type id before '*' symbol";
          return pos;
        }
        else
        {
          if (last_named_phase != 1)
          {
            cferr = "Instantiation should only follow object name or definition";
            return pos;
          }
          if (!ExtRegister(last_named,last_identifier))
            return pos;
        }
      }
    }
    
    if (cfile[pos] == '(')
    {
      if (last_named != LN_IDENTIFIER)
      {
        if (last_named != LN_OPERATOR or last_named_phase != OP_PARAMS)
        {
          if (last_named != LN_DECLARATOR)
          {
            if (last_named != LN_NOTHING)
            {
              cferr = "Unexpected parenthesis at this point";
              return pos;
            }
          }
          plevel++;
          pos++;
          continue;
        }
        
      }
      
      last_named = LN_IDENTIFIER;
      last_named_phase = 0;
      
      if (funclevel==-1)
      {
        funclevel=plevel;
        fargs_named=1;
        fargs_count=0;
      }
      plevel++;
      pos++;
      continue;
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
      if (plevel < funclevel)
        funclevel = -1;
      last_named_phase=0;
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

