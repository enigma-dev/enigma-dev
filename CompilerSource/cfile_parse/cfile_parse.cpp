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

#include "cfile_pushing.h"
#include "cfile_parse_constants.h"
#include "cfile_parse_calls.h"
#include "cparse_components.h"


#define encountered pos++;
#define quote while (cfile[pos]!='"') { pos++; if (cfile[pos]=='\\' and (cfile[pos+1]=='\\'||cfile[pos]=='"')) pos+=2; }
#define squote while (cfile[pos]!='\'') { pos++; if (cfile[pos]=='\\' and (cfile[pos+1]=='\\'||cfile[pos]=='\'')) pos+=2; }

bool in_false_conditional();
//extern void print_cfile_top(const unsigned);

unsigned int cfile_parse_macro();
int keyword_operator();
#include "handle_letters.h"

extern varray<string> include_directories;


string cferr_get_file()
{
  if (included_files.empty())
    return "";
  return "In file included from " + included_files.top().name + ": ";
}

string cferr_get_file_orfirstfile()
{
  if (included_files.empty())
    return "first file";
  return included_files.top().name;
}

string strace(externs *f)
{
  string o = "";
  if (f == &global_scope)
    return "Global Scope";
  for (externs* i=f; i != &global_scope; i=i->parent)
    o = "::" + i->name + o;
  return o;
}

char skipto, skipto2, skip_inc_on;
int parse_cfile(string cftext)
{
  cferr="No error";
  while (!included_files.empty())
    included_files.pop();

  bool preprocallowed=1;
  
  cfile_top = cfile = cftext;
  len = cfile.length();
  pos = 0;

  last_type = NULL;
  argument_type = NULL;
  last_identifier = "";
  last_named = LN_NOTHING;
  last_named_phase = 0;
  last_value = 0;

  int plevel = 0;/*
  int funclevel = -1;
  int fargs_named = 0;
  int fargs_count = 0;*/
  
  int fparam_named = 0;
  int fparam_defaulted = 0;
  
  skip_depth = 0;
  specialize_start = 0;
  specialize_string = "";
  specializing = false;
  skipto = 0, skipto2 = 0, skip_inc_on = 0;

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

  anoncount = 0;
  stack<externs*> scope_stack;
  //stack<externs*> current_templates;
  bool handle_ids_next_iter = 0;
  unsigned id_would_err_at = 0;
  string id_to_handle;

  for (;;)
  {
    cfile_top = cfile;
    if (!(pos<len))
    {
      if (!cfstack.empty()) //If we're in a macro
      {
        //Move back down
        if (specializing)
        {
          cout << "substr(" << specialize_start << "," << pos-specialize_start << ");" << endl;
          specialize_string += cfile.substr(specialize_start,pos-specialize_start);
        }
        handle_macro_pop();
          if (specializing)
            specialize_start = pos;
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
      if (preprocallowed)
      {
        if (specializing)
          specialize_string += cfile.substr(specialize_start,pos-specialize_start);
        
        const unsigned a = cfile_parse_macro();
        
        if (specializing)
          specialize_start = pos;
        
        if (a != unsigned(-1)) return a;
        continue;
      }
      else
      {
        cferr = "WHY?! WHHHHHHHHHHYYYYYYYYYYYYYYYYY!!?";
        return pos;
      }
    }
    
    //Handle comments here, before conditionals
    //And before we disallow a preprocessor
    const unsigned hc = handle_comments();
    if (hc == unsigned(-2)) continue;
    if (hc != unsigned(-1)) return hc;
    
    //Not a preprocessor
    preprocallowed = false;
    
    if (in_false_conditional())
      { pos++; continue; }
    
    if (skipto)
    {
      if (is_letter(cfile[pos]))
      {
        if (specializing and pos-specialize_start)
          specialize_string += cfile.substr(specialize_start,pos-specialize_start);
        
        //This segment is completely stolen from below and should be functionized ASAP
        string n = "";
        unsigned int sp = pos++;
        while (is_letterd(cfile[pos])) // move to the end of the word
        { //This is an odd case. If we reach the end of the file while we read, 
          if (++pos >= len) //it's a good idea to keep going
          {
            n += cfile.substr(sp,pos-sp);
            handle_macro_pop();
            sp=pos; if (pos >= len) break;
          }
        }
        n += cfile.substr(sp,pos-sp); //This is the word we're looking at.
        
        //This piece is also stolen from below, but is simple enough as-is
        const unsigned int cm = handle_macros(n);
        if (cm == unsigned(-2)) // Was a macro
        {
          if (specializing)
            specialize_start = pos;
          continue;
        }
        if (cm != unsigned(-1)) return cm;
        
        if (specializing) {
          specialize_start = pos;
          specialize_string += n;
        }
        continue;
      }
      if (skipto == ';' and skipto2 == ';')
      {
        if (cfile[pos] == ';' or cfile[pos] == ',')
        {
          if (skip_depth == 0)
            skipto = skipto2 = skip_inc_on = 0;
        }
        else if (cfile[pos] == '{')
          skip_depth++;
        else if (cfile[pos] == '}')
        {
          if (skip_depth == 0)
          {
            skipto = skipto2 = skip_inc_on = 0;
          }
          else
            skip_depth--;
        }
      }
      else
      {
        if (cfile[pos] == skipto or cfile[pos] == skipto2)
        {
          if (skip_depth == 0)
            skipto = skipto2 = skip_inc_on = 0;
          else if (cfile[pos] == skipto) //Only skip if the primary is met. For instance, in "s = '>'; s2 = ','; sincon = '<';", we want to dec only on >
            skip_depth--;
        }
        else if (cfile[pos] == skip_inc_on)
        {
          skip_depth++;
          /*if (specializing) {
            cferr = "Unexpected start of new template parameters in specialization of others";
            return pos;
          }*/
        }
      }
      
      if (skipto == 0)
      {
        if (specializing)
        {
          specialize_string += cfile.substr(specialize_start,pos-specialize_start);
          if ((last_named & ~LN_TYPEDEF) == LN_TEMPARGS)
          {
            last_named = LN_DECLARATOR | (last_named & LN_TYPEDEF);
            if (!access_specialization(last_type,specialize_string))
              return pos;
          }
          else if ((last_named & ~LN_TYPEDEF) == LN_TEMPLATE)
          {
            if (!access_specialization(last_type,specialize_string))
              return pos;
          }
          else if ((last_named & ~LN_TYPEDEF) == LN_DECLARATOR and last_named_phase == DEC_FULL)
          {
            last_named = LN_DECLARATOR | (last_named & LN_TYPEDEF);
            externs* ts = TemplateSpecialize(last_type,specialize_string);
            if (!ts)
              return pos;
            last_type = ts;
            pos++;
          }
          else if ((last_named & ~LN_TYPEDEF) == LN_ENUM)
          {
            last_named_phase = EN_DEFAULTED;
            last_value = evaluate_expression(specialize_string);
            if (rerrpos != -1) {
              cferr = rerr;
              return pos;
            }
            cout << "Evaluated \"" << specialize_string << "\" as " << last_value << endl;
          }
          else if ((last_named & ~LN_TYPEDEF) == LN_STRUCT
               or  (last_named & ~LN_TYPEDEF) == LN_CLASS)
          {
            if (last_named_phase == SP_PUBLIC or last_named_phase == SP_PRIVATE or last_named_phase == SP_PROTECTED)
            {
              if (ihc <= 0) {
                cferr = "Error in instantiating inherited template: this should have been reported earlier";
                return pos;
              }
              if (!access_specialization(inheritance_types[ihc-1].parent,specialize_string))
                return pos;
            }
          }
          else {
            cferr = "Nothing to do with freshly parsed string. This shouldn't happen. ln = " + tostring(last_named);
            return pos;
          }
          specializing = false;
          if (cfile[pos] != ';' and cfile[pos] != ',' and cfile[pos] != '{' and cfile[pos] != '}')
            pos++;
        }
        else if (cfile[pos] != ';' and cfile[pos] != ',' and cfile[pos] != '{')
          pos++;
      }
      else pos++;
      continue;
    }
    
    if (handle_ids_next_iter)
    {
      bool at_scope_accessor = cfile[pos] == ':' and cfile[pos+1] == ':';
      
      int diderrat = handle_identifiers(id_to_handle,last_identifier,last_named,last_named_phase,last_type,fparam_named,at_scope_accessor);
      if (diderrat != -1) return id_would_err_at; //Discard diderrat until future use
      
      if (at_scope_accessor) pos += 2;
      handle_ids_next_iter = false;
      continue;
    }
    
    
    //First, let's check if it's a letter.
    //This implies it's one of three things...
    if (is_letter(cfile[pos]))
    {
      string n = "";
      unsigned int sp = id_would_err_at = pos++;
      while (is_letterd(cfile[pos])) // move to the end of the word
      { //This is an odd case. If we reach the end of the file while we read, 
        if (++pos >= len) //it's a good idea to keep going
        {
          n += cfile.substr(sp,pos-sp);
          handle_macro_pop();
          sp = id_would_err_at = pos;
          if (pos >= len) break;
        }
      }
      n += cfile.substr(sp,pos-sp); //This is the word we're looking at.
      
      //Macros get precedence. Check if it's one.
      const unsigned int cm = handle_macros(n);
      if (cm == unsigned(-2)) continue;
      if (cm != unsigned(-1)) return cm;
      
      if (n=="__asm") //now we have a problem
      {
        while (is_useless(cfile[pos])) pos++;
        if (cfile[pos] != '(')
        {
          cferr = "Expected ( following inline-assembly token";
          return pos;
        }
        pos++;
        while (is_useless(cfile[pos])) pos++;
        if (cfile[pos] != '"')
        {
          cferr = "Expected string of assembly instructions";
          return pos;
        }
        pos++;
        while (pos<len and cfile[pos] != '"')
          { if (cfile[pos] == '\\') pos++; pos++; }
        if (cfile[pos] != '"')
        {
          cferr = "Expected string of assembly instructions";
          return pos;
        }
        pos++;
        while (is_useless(cfile[pos])) pos++;
        if (cfile[pos] != ')')
        {
          cferr = "Expected closing parenthesis after assembly string";
          return pos;
        }
        pos++;
      }
      else
      {
        handle_ids_next_iter = true;
        id_to_handle = n;
      }
      continue;
    }
    
    
    //There is a select number of symbols we are supposed to encounter.
    //A digit is actually not one of them. Digits, most operators, etc,
    //will be skipped over when we see an = sign.
    
    //Here's a biggun: deal with semicolons
    //This is probably the symbol we will see most often
    if (cfile[pos] == ',' or cfile[pos] == ';')
    {
      if (last_named == LN_NOTHING)
        { pos++; continue; }
      
      if ((last_named & LN_TYPEDEF) != 0)
      {
        if (last_named != (LN_DECLARATOR | LN_TYPEDEF))
        {
          cferr = "Invalid typedef";
          return pos;
        }
        
        //If we're in function params of a typedef
        if (refstack.currentsymbol() == '(' and !refstack.currentcomplete())
          goto not_typedefing_anything_yet;
        
        if (last_identifier == "")
        {
          cferr = "No definiendum in type definition";
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
        
        externs *n = new externs;
        
        n->type = last_type;
        n->name = last_identifier;
        n->flags = last_type->flags | EXTFLAG_TYPEDEF;
        n->refstack = refstack.dissociate();
        
        current_scope->members[last_identifier] = n;
        last_named_phase = DEC_FULL;
      }
      else //Not typedefing anything
      {
        not_typedefing_anything_yet:
        switch (last_named & ~LN_TYPEDEF)
        {
          case LN_DECLARATOR:
              //Can't error on last_named_phase != DEC_IDENTIFIER, or structs won't work
              if (refstack.currentsymbol() == '(' and !refstack.currentcomplete())
              {
                if (cfile[pos] == ';') {
                  cferr = "Expected closing parenthesis before ';'";
                  return pos;
                }
                if (fparam_named)
                {
                  if (!fparam_defaulted)
                    refstack.inc_current_min();
                  refstack.inc_current_max();
                  fparam_defaulted = 0;
                }
                pos++; continue;
              }
              last_named_phase = DEC_FULL; //reset to 4 for next identifier.
            break;
          case LN_TEMPLATE:
              if (cfile[pos] == ';')
              {
                cferr="Unterminating template declaration; expected '>' before ';'";
                return pos;
              }
              if (last_named_phase != TMP_IDENTIFIER and last_named_phase != TMP_DEFAULTED)
              {
                if (last_named != TMP_TYPENAME) {
                  cferr="Unexpected comma in template declaration";
                  return pos;
                }
                
                if (last_named_phase == TMP_SIMPLE and last_type == NULL) {
                  cferr = "Template parameter marked as a specific type, but arrived NULL";
                  return pos;
                }
                
                if (last_identifier == "")
                  last_identifier = last_named_phase == TMP_SIMPLE?"<type only>":"<not named>";
                
                tmplate_params[tpc++] = tpdata(last_identifier,last_type,last_type != NULL);
                last_named_phase = TMP_PSTART;
                pos++; continue;
              }
              
              tmplate_params[tpc++] = tpdata(last_identifier,last_named_phase != TMP_DEFAULTED ? NULL : last_type);
              last_named_phase = TMP_PSTART;
              
            pos++; continue;
          
          //These can all be looked at the same at this point.
          case LN_CLASS: case LN_STRUCT: case LN_UNION:
              if (cfile[pos] != ';') 
              {
                cferr="Expected ';' instead of ',' when not implemented";
                return pos;
              }
              if (last_named_phase != 1) {
                cferr="Expected only identifier when not implemented";
                return pos;
              }
            break;
          case LN_ENUM:
              if (cfile[pos] != ';' and last_named_phase == EN_IDENTIFIER)
              {
                cferr="Expected ';' instead of ',' when not enumerated";
                return pos;
              }
              if (last_named_phase != EN_IDENTIFIER)
              {
                if (cfile[pos] != ',') {
                  cferr = "Expected ',' or '}' before ';'";
                  return pos;
                }
                if (last_named_phase != EN_CONST_IDENTIFIER and last_named_phase != EN_DEFAULTED) {
                  cferr="Expected only identifier when not implemented";
                  return pos;
                }
                if (last_identifier != "") {
                  if (!ExtRegister(last_named,last_named_phase,last_identifier,refstack,builtin_type__int,tmplate_params,tpc,last_value))
                    return pos;
                }
                else {
                  cferr = "Expected name of constant for enum";
                  return pos;
                }
                last_named_phase = EN_WAITING;
                pos++; continue;
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
              if (last_named_phase != USE_NAMESPACE_IDENTIFIER)
              {
                if (last_named_phase != USE_SINGLE_IDENTIFIER)
                {
                  cferr = "Nothing to use";
                  return pos;
                }
              }
              {
                externs* using_scope = scope_get_using(current_scope);
                extiter pu = using_scope->members.find(last_type->name);
                if (pu != using_scope->members.end())
                {
                  if (pu->second != last_type)
                  {
                    if (pu->second->is_function() and last_type->is_function())
                    {
                      pu->second->parameter_unify(last_type->refstack);
                    }
                    else
                    {
                      cferr = "Using `" + last_type->name + "' conflicts with previous `using' directive";
                      return pos;
                    }
                  }
                } 
                else
                  using_scope->members[last_type->name] = last_type;
              }
              pos++;
              last_named = LN_NOTHING;
              last_named_phase = 0;
              last_identifier = "";
              
            continue;
          default:
              cferr = "WELL WHAT THE FUCK.";
            return pos;
        }
        
        externs *type_to_use = last_type;
        rf_stack refs_to_use = refstack.dissociate();
        
        if (type_to_use != NULL) //A case where it would be NULL is struct str;
        while (type_to_use->flags & EXTFLAG_TYPEDEF)
        {
          refs_to_use += type_to_use->refstack;
          if (type_to_use->type == NULL) break;
          type_to_use = type_to_use->type;
        }
        
        if (last_identifier != "")
        if (!ExtRegister(last_named,last_named_phase,last_identifier,refs_to_use,type_to_use,tmplate_params,tpc))
          return pos;
        tpc = -1;
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
        argument_type = NULL;
        refstack.dump();
        tpc = -1;
      }
      
      pos++;
      continue;
    }
    
    
    //The next thing we want to do is check we're not expecting an operator for the operator keyword.
    if (last_named==LN_OPERATOR and last_named_phase != OP_PARAMS)
    {
      int a=keyword_operator();
      if (a != -1) return a;
      continue;
    }
    
    //Now that we're sure we aren't in an "operator" expression,
    //We can check for the few symbols we expect to see.
    
    //First off, the most common is likely to be a pointer indicator.
    if (cfile[pos] == '*')
    {
      //type should be named
      if ((last_named | LN_TYPEDEF) != (LN_DECLARATOR | LN_TYPEDEF))
      {
        if ((last_named | LN_TYPEDEF) != (LN_CLASS  | LN_TYPEDEF)
        and (last_named | LN_TYPEDEF) != (LN_STRUCT | LN_TYPEDEF)
        and (last_named | LN_TYPEDEF) != (LN_UNION  | LN_TYPEDEF))
        {
          cferr = "Unexpected '*'";
          return pos;
        }
        if (!extreg_deprecated_struct(false,last_identifier,last_named,last_named_phase,last_type))
          return pos;
      }
      refstack += referencer('*');
      pos++; continue;
    }
    //First off, the most common is likely to be a pointer indicator.
    if (cfile[pos] == '&')
    {
      //type should be named
      if ((last_named | LN_TYPEDEF) != (LN_DECLARATOR | LN_TYPEDEF))
      {
        if ((last_named | LN_TYPEDEF) != (LN_CLASS  | LN_TYPEDEF)
        and (last_named | LN_TYPEDEF) != (LN_STRUCT | LN_TYPEDEF)
        and (last_named | LN_TYPEDEF) != (LN_UNION  | LN_TYPEDEF))
        {
          cferr = "Unexpected '&'";
          return pos;
        }
        if (!extreg_deprecated_struct(false,last_identifier,last_named,last_named_phase,last_type))
          return pos;
      }
      refstack += referencer('&');
      pos++; continue;
    }
    
    if (cfile[pos]=='[')
    {
      //type should be named
      if (last_named != LN_DECLARATOR)
      {
        cferr = "Unexpected '['";
        return pos;
      }
      refstack += referencer('[');
      skipto = ']'; skip_inc_on = '[';
      pos++; continue;
    }
    
    if (cfile[pos] == '(')
    {
      if ((last_named | LN_TYPEDEF) != (LN_DECLARATOR | LN_TYPEDEF))
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
      
      if (last_named_phase != DEC_IDENTIFIER)
      {
        if (last_named_phase == DEC_THROW) //int func() throw(<--You are here);
        {
          skipto = ')';
          skip_inc_on = '(';
          last_named_phase = DEC_IDENTIFIER;
          pos++; continue;
        }
        if (last_named_phase != DEC_FULL) {
          cferr = "Unexpected parenthesis in declaration";
          return pos;
        }
        if (last_type->flags & (EXTFLAG_STRUCT | EXTFLAG_CLASS)) {
          last_identifier = last_type->name;
          last_named_phase = DEC_IDENTIFIER;
        }
        //last_type can stay what it is
      }
      
      //<declarator> ( ... ) or <declarator> <identifier> ()
      if (refstack.currentsymbol() == '(' and !refstack.currentcomplete())
      { //Skip parenths inside function params
        skipto = ')';
        skip_inc_on = '(';
        last_named_phase = DEC_IDENTIFIER;
        pos++; continue;
      }
      else
      {
        refstack += referencer(last_named_phase == DEC_IDENTIFIER ? '(' : ')',0,last_named_phase != DEC_IDENTIFIER);
        argument_type = NULL;
        plevel++;
      }
      
      pos++; continue;
    }
    
    if (cfile[pos] == ')')
    {
      if (!(plevel > 0))
      {
        cferr = "Unexpected closing parenthesis: None open";
        return pos;
      }
      if (refstack.currentsymbol() == '(')
        if (fparam_named)
        {
          if (!fparam_defaulted)
            refstack.inc_current_min();
          refstack.inc_current_max();
          fparam_defaulted = 0;
        }
      
      plevel--;
      refstack--; //Move past previous parenthesis
      //last_named_phase=0;
      pos++; continue;
    }
    
    if (cfile[pos] == '{')
    {
      //Because :: can be used to move to a distant scope, we must leave a breadcrumb trail
      externs *push_scope = current_scope; 
      bool skipping_to = false; //Also, if this is set to true (like for function implementation), nothing is pushed
      const int last_named_raw = last_named & ~LN_TYPEDEF;
      
      //Class/Namespace declaration.
      if (last_named_raw == LN_NAMESPACE or last_named_raw == LN_STRUCT or last_named_raw == LN_CLASS or last_named_raw == LN_UNION)
      {
        if (!ExtRegister(last_named,last_named_phase,last_identifier,0,NULL,tmplate_params,tpc))
          return pos;
        current_scope = ext_retriever_var;
      }
      //Enum declaration.
      else if (last_named_raw == LN_ENUM)
      {
        if (!ExtRegister(last_named,last_named_phase,last_identifier,0,NULL,tmplate_params,tpc))
          return pos;
        scope_stack.push(current_scope);
        current_scope = ext_retriever_var;
        last_identifier = "";
        last_named_phase = EN_WAITING;
        
        refstack.dump();
        tpc = -1;
        pos++;
        
        continue;
      }
      //Function implementation.
      else if (last_named_raw == LN_DECLARATOR)
      {
        if (refstack.topmostsymbol() == '(') // Do not confuse with ')'
        {
          //Register the function in the current scope
          if (!ExtRegister(last_named,last_named_phase,last_identifier,refstack.dissociate(),last_type,tmplate_params,tpc))
            return pos;
          
          //Skip the code: we don't need to know it ^_^
          skipto = '}'; skip_inc_on = '{';
          push_scope = NULL;
          skipping_to = 1;
        }
        else
        {
          if (last_type != NULL and last_type->flags & (EXTFLAG_STRUCT|EXTFLAG_CLASS|EXTFLAG_ENUM))
          { //We're at a structure or something, and we're not NULL
            //if (last_type->parent == current_scope)
            if (!last_type->members.empty())
            {
              if ((last_type->flags & EXTFLAG_TEMPLATE) and specialize_string != "")
              {
                current_scope = TemplateSpecialize(last_type,specialize_string); //Parse specialize_string and 
                if (current_scope == NULL) return pos;
              }
              else {
                cferr = "Attempting to redeclare struct `"+last_type->name+"'";
                return pos;
              }
            }
            else
              current_scope = last_type; //Move into it. Brilliantly simple; we already know it's parent is current_scope.
          }
          else
          {
            cferr = (last_named_phase == DEC_FULL)?"Expected identifier between typename and '{'":
                    (last_named_phase == DEC_IDENTIFIER)?"Unepected symbol '{':  `"+last_identifier+"' does not name a function":
                    "Unexpected '{' in declaration.";
            return pos;
          }
        }
      }
      else
      {
        if (last_named == LN_NOTHING and last_named_phase == 99)
          last_named_phase = 0;
        else {
          cferr = "Unexpected opening brace at this point";
          return pos;
        }
      }
      
      last_identifier = "";
      last_named = LN_NOTHING;
      
      if (!skipping_to)
        scope_stack.push(push_scope);
      
      last_named_phase = 0;
      last_type = NULL;
      argument_type = NULL;
      refstack.dump();
      tpc = -1;
      pos++;
      
      continue;
    }
    
    if (cfile[pos] == '}')
    {
      if (scope_stack.empty())
      {
        cferr = "Unexpected closing brace at this point: none open";
        return pos;
      }
      
      externs* lscope = scope_stack.top();
      scope_stack.pop();
      if (lscope == NULL) { 
        pos++;
        continue;
      }
      
      if (last_named != LN_NOTHING)
      {
        if (last_named != LN_DECLARATOR or !(current_scope->flags & EXTFLAG_ENUM))
        {
          if (last_named != LN_ENUM or (last_named_phase != EN_WAITING
          and last_named_phase != EN_DEFAULTED and last_named_phase != EN_CONST_IDENTIFIER)) {
            cferr = "Unexpected closing brace at this point";
            return pos;
          }
          ExtRegister(last_named,last_named_phase,last_identifier,refstack.dissociate(),builtin_type__int,tmplate_params,tpc,last_value);
        }
        else
          ExtRegister(last_named,last_named_phase,last_identifier,refstack.dissociate(),last_type,tmplate_params,tpc);
      }
      
      if (current_scope->flags & EXTFLAG_TYPENAME)
      {
        last_named = LN_DECLARATOR; //if the scope we're popping serves as a typename
        last_named_phase = DEC_FULL;
      }
      else
      {
        last_named = LN_NOTHING;
        last_named_phase = 0;
      }
      
      if (current_scope->flags & EXTFLAG_PENDING_TYPEDEF)
      {
        last_named |= LN_TYPEDEF;
        current_scope->flags &= ~EXTFLAG_PENDING_TYPEDEF;
      }
      
      last_type = current_scope;
      current_scope = lscope;
      tpc = -1;
      
      pos++; continue;
    }
    
    //Two less freqent symbols now, heh.
    
    if (cfile[pos] == '<')
    {
      //If we're not just past the word "template"
      if ((last_named & ~LN_TYPEDEF) != LN_TEMPLATE)
      {
        if ((last_named & ~LN_TYPEDEF) != LN_DECLARATOR or last_named_phase != DEC_FULL)
        {
          if (((last_named & ~LN_TYPEDEF) != LN_STRUCT and (last_named & ~LN_TYPEDEF) != LN_CLASS) or last_named_phase != SP_PARENT_NAMED or ihc == 0)
          {
            if (last_type == NULL) {
              cferr = "Unexpected symbol '<' should only occur directly following `template' token or type";
              return pos;
            }
            if (!(last_type->flags & EXTFLAG_TEMPLATE))
            {
              if (!refstack.is_function() or argument_type == NULL or !(argument_type->flags & EXTFLAG_TEMPLATE)) {
                cferr = "Unexpected symbol '<' should only occur directly following `template' token or type, even in function parameters <_<";
                return pos;
              }
            }
          }
        }
        else if (tpc == -1) //tname<(*)...>
          last_named = LN_TEMPARGS | (last_named & LN_TYPEDEF);
        
    the_next_block_up:
          
        skipto = '>';
        skip_inc_on = '<';
        specializing = true;
        specialize_start = ++pos;
        specialize_string = "";
        
        continue;
      }
      if (last_named_phase != TMP_NOTHING)
      {
        if (last_named_phase != TMP_DEFAULTED) {
          cferr = "Unexpected token '<' in template parameters";
          return pos;
        }
        goto the_next_block_up;
      }
      
      if (tpc == -1) 
        tpc = 0;
      else { 
        cferr = "Template parameters already named for this declaration";
        return pos; 
      }
      
      //current_templates.push(last_type);
      last_named_phase = TMP_PSTART;
      pos++; continue;
    }
    
    if (cfile[pos] == '>')
    {
      if (last_named != LN_TEMPLATE)
      {
        if (last_named != LN_TEMPARGS)
        {
          cferr = "Unexpected symbol '>' should only occur as closing symbol of template parameters";
          return pos;
        }
        //Add to template args here
        last_named = LN_DECLARATOR;
        last_named_phase = DEC_FULL;
        //last_type = current_templates.top();
        //current_templates.pop();
        pos++; continue;
      }
      if (last_named_phase != TMP_IDENTIFIER and last_named_phase != TMP_DEFAULTED and (last_named_phase != TMP_TYPENAME? true : (last_identifier = "", false)))
      {
        if (last_named_phase != TMP_SIMPLE)
        {
          if (last_named_phase != TMP_PSTART/* or !current_templates.empty()*/) //Template <> is allowed
          { cferr = "Unexpected end of input before '>'";
            return pos; }
        }
        else
        {
          if (last_type == NULL) {
            cferr = "Type in template parameter appears to be NULL... Not sure why this would happen";
            return pos;
          }
          tmplate_params[tpc++] = tpdata(last_identifier, last_type, 1);
        }
      }
      else
        tmplate_params[tpc++] = tpdata(last_identifier,last_named_phase != TMP_DEFAULTED ? NULL : last_type);
      
      last_named = LN_NOTHING;
      last_named_phase = 0;
      last_type = NULL;
      argument_type = NULL;
      pos++; continue;
    }
    
    if (cfile[pos] == ':')
    {
      if (cfile[pos+1] == ':')
      {
        if (last_named == LN_DECLARATOR and last_named_phase == DEC_FULL)
        {
          immediate_scope = last_type;
          last_named = LN_NOTHING;
          last_named_phase = 0;
        }
        else
          immediate_scope = &global_scope;
        pos += 2;
        continue;
      }
      else
      {
        if (last_named == LN_DECLARATOR)
        {
          if (refstack.is_function()) {
            skipto = '{';
            skipto2 = ';'; //This shouldn't actually happen, but is included as a failsafe.
          }
          else {
            skipto = ';';
            skipto2 = ';';
          }
          pos++;
          continue;
        }
        if (last_named == LN_STRUCT or last_named == LN_CLASS)
        {
          if (last_named_phase != SP_IDENTIFIER and last_named_phase != SP_EMPTY) {
            cferr = "Colon already named in heritance expression";
            return pos;
          }
          last_named_phase = SP_COLON;
          pos++; continue;
        }
        if (last_named == LN_LABEL)
        {
          //TODO: Eventually this is to be used for public/private distinction
          last_named = LN_NOTHING;
          last_named_phase = 0;
          pos++; continue;
        }
        cferr = "Unexpected colon at this point";
        return pos;
      }
    }
    
    if (cfile[pos] == '=')
    {
      if (last_named == LN_TEMPLATE)
      {
        if (last_named_phase != TMP_IDENTIFIER)
        {
          /*cferr = "Expected identifier before '=' token";
          return pos;*/ //Somehow, this is ISO...
          last_identifier = "";
        }
        if (last_named_phase != TMP_SIMPLE) //"typename" was given, not a type like "bool"
          last_named_phase = TMP_EQUALS; //Thus, we're looking for a type to default to
        else
        {
          skipto = '>';
          skipto2 = ',';
          specializing = true;
          specialize_start = ++pos;
          specialize_string = "";
          continue;
        }
        pos++; continue;
      }
      if (last_named == LN_DECLARATOR)
      {
        if (fparam_named)
          fparam_defaulted = 1;
        skipto = ';';
        skipto2 = ';';
        continue;
      }
      if (last_named == LN_ENUM)
      {
        if (last_named_phase != EN_CONST_IDENTIFIER) {
          cferr = "Unexpected '=' in enum declaration";
          return pos;
        }
        
        skipto = ';';
        skipto2 = ';';
        specializing = true; //FIXME: Good luck.
        specialize_start = ++pos;
        specialize_string = "";
        continue;
      }
      cferr = "I have no idea what to do with this '=' token. If you see that as a problem, report this error";
      return pos;
    }
    
    //...
    if (cfile[pos] == '.' and cfile[pos+1] == '.' and cfile[pos+2] == '.')
    {
      if (last_named != LN_DECLARATOR or refstack.currentsymbol() != '(')
      {
        cferr = "Token `...' expected only in function parameters";
        return pos;
      }
      if (refstack.parameter_count_max() != (unsigned char)(-1))
        refstack.set_current_max((unsigned char)(-1));
      else
      {
        cferr = "Token `...' already named";
        return pos;
      }
      pos += 3;
      continue;
    }
    
    //The somewhat uncommon destructor token
    if (cfile[pos] == '~')
    {
      if (last_named != LN_NOTHING) {
        cferr = "Token `~' should only occur to denote a destructor. Furthermore, `~' was fun to type";
        return pos;
      }
      last_named = LN_DESTRUCTOR;
      last_named_phase = 0;
      pos++; continue;
    }
    
    //extern "C"
    if (cfile[pos] == '"')
    {
      if (last_named == LN_DECLARATOR and last_named_phase == 0) //assuming it's "extern" and not "unsigned", which'd behave the same
      if (cfile[++pos] == 'C')
      if (cfile[++pos] == '"')
      {
        last_named = LN_NOTHING;
        last_named_phase = 99;
        pos++; continue;
      }
      if (cfile[pos] == '+')
      if (cfile[++pos] == '+')
      if (cfile[++pos] == '"')
      {
        last_named = LN_NOTHING;
        last_named_phase = 99;
        pos++; continue;
      }
      cferr = "String literal not allowed here";
      return pos;
    }//Backslash. This is actually not that common.
    
    if (cfile[pos] == '\\')
    {
      pos++;
      if (cfile[pos] == '\r' or cfile[pos] == '\n')
      { pos++; continue; }
      cferr = "Stray backslash in program";
      return pos-1;
    }
    
    cferr = string("Unexpected symbol '")+cfile[pos]+"'";
    return pos;
  }
  
  
  
  /*
  string pname = "";
  if (last_typedef != NULL) pname=last_typedef->name;
  cout << '"' << last_typename << "\":" << pname << " \"" << last_identifier << "\"\r\n";*/
  return -1;
}

