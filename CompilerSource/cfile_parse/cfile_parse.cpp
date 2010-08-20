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
#include <stdio.h>
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

bool in_false_conditional();

pt cfile_parse_macro();
int keyword_operator();
#include "handle_letters.h"

extern varray<string> include_directories;


#ifdef ENIGMA_PARSERS_DEBUG
  extern int tpcsval, total_alloc_count[15];
  #define TPDATA_CONSTRUCT(x) tpcsval = (total_alloc_count[x]++) * 100 + x
  #define REPORT_IF_ALLOCATED(x,y) if ((total_alloc_count[x]-1) == y)
#else
  #define TPDATA_CONSTRUCT(x)
#endif

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
    if (i) o = "::" + i->name + o;
    else { o = "NULL! " + o; break; }
  return o;
}

extern pt handle_skip();

int debug_ent() {
  cout << "Asses";
  return 0;
}

pt parse_cfile(my_string cftext)
{
  cferr="No error";
  while (!included_files.empty())
    included_files.pop();

  included_files.push(includings("SHELLmain.cpp","./ENIGMAsystem/SHELL/"));

  bool preprocallowed=1;

  cfile = cftext;
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
  skipto = skipto2 = skip_inc_on = 0;

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
  string id_to_handle;
  //bool rconcat = false; // This used to hold macros' hands, but ceased to ever be false after the system was rethought.

  for (;;)
  {
    if (pos >= len)
    {
      if (!cfstack.empty()) //If we're in a macro
      {
        //Move back down
        if (specializing)
          specialize_string += cfile.substr(specialize_start,pos-specialize_start);
        handle_macro_pop();
          if (specializing)
            specialize_start = pos;
        continue;
      }
      else break;
    }
    
    if (is_useless(cfile[pos]))
    {
      do if (cfile[pos]=='\r' or cfile[pos]=='\n')
        preprocallowed=true;
      while (is_useless(cfile[++pos]));
      continue;
    }
    
    
    //If it's a macro, deal with it here
    if (cfile[pos]=='#')
    {
      if (preprocallowed)
      {
        if (specializing)
          specialize_string += cfile.substr(specialize_start,pos-specialize_start);

        const pt a = cfile_parse_macro();

        if (specializing)
          specialize_start = pos;

        if (a != pt(-1)) return a;
          continue;
      }
      else
      {
        cferr = "Unexpected pound symbol at this point; must be at beginning of line for preprocessor";
        return pos;
      }
    }

    //Handle comments here, before conditionals
    //And before we disallow a preprocessor
    if (cfile[pos] == '/')
    {
      const pt hc = handle_comments();
      if (hc == pt(-2)) continue;
      if (hc != pt(-1)) return hc;
    }

    //Not a preprocessor
    preprocallowed = false;

    if (in_false_conditional())
      { pos++; continue; }

    if (skipto)
    {
      pt a = handle_skip(); // Too huge to display here. May need maintenance in the future anyway.
      if (a != pt(-1))
        return a;
      continue;
    }

    //if (rconcat and is_letterd(cfile[pos]))
    //  goto is_letter__the_block_below_this_next_block;

    if (handle_ids_next_iter)
    {
      //if (cfile[pos] != '#' or cfile[pos+1] != '#')
      //{
        bool at_scope_accessor = cfile[pos] == ':' and cfile[pos+1] == ':';
        bool at_template_param = cfile[pos] == '<';

        int diderrat = handle_identifiers(id_to_handle,fparam_named,at_scope_accessor,at_template_param);
        if (diderrat != -1) return id_would_err_at; //Discard diderrat until future use

        if (at_scope_accessor) pos += 2;
      //}
      //else { printf("ASSES!"); gets((char*)alloca(4)); rconcat = true, pos += 2; }
      handle_ids_next_iter = false;
      continue;
    }


    //First, let's check if it's a letter.
    //This implies it's one of three things...
    if (is_letter(cfile[pos]))
    {
      if (cfile[pos] == 'L' and (cfile[pos+1] == '"' or cfile[pos+1] == '\'')) {
        //rconcat = false;
        pos++; continue;
      }

      pt sp = id_would_err_at = pos;
      while (is_letterd(cfile[++pos]));

      string n = /*rconcat? id_to_handle + cfile.substr(sp,pos-sp) :*/ cfile.substr(sp,pos-sp); //This is the word we're looking at.

      //Macros get precedence. Check if it's one.
      const pt cm = handle_macros(n);
      if (cm == pt(-2)) continue;
      if (cm != pt(-1)) return cm;

      if (n == "__asm" or n == "__asm__") //now we have a problem
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
    //rconcat = false;

    //There is a select number of symbols we are supposed to encounter.
    //A digit is actually not one of them. Digits, most operators, etc,
    //will be skipped over when we see an = sign.

    //Here's a biggun: deal with semicolons
    //This is probably the symbol we will see most often
    if (cfile[pos] == ',' or cfile[pos] == ';')
    {
      if (last_named == LN_NOTHING)
        { pos++; continue; }

      if (last_named & LN_TYPEDEF) //Typedefing something
      {
        if (last_named != (LN_DECLARATOR | LN_TYPEDEF))
        {
          if (last_named == (LN_ENUM | LN_TYPEDEF))
            goto not_typedefing_anything_yet;
          cferr = "Invalid typedef: "+tostring(last_named & ~LN_TYPEDEF);
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
          if (last_type != it->second) {
            cferr = "Redeclaration of `"+last_identifier+"' as typedef at this point";
            return pos;
          }
          last_named_phase = DEC_FULL;
        }
        else
        {
          if (last_type == NULL)
          {
            cferr = "Program error: Type does not exist. An error should have been reported earlier.";
            return pos;
          }

          externs *n = new externs(last_identifier,last_type,current_scope,last_type->flags | EXTFLAG_TYPEDEF,0,refstack.dissociate());
          current_scope->members[last_identifier] = n;
          last_named_phase = DEC_FULL;
        }
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
              if (cfile[pos] == ';') {
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

                TPDATA_CONSTRUCT(1);
                tmplate_params[tpc++] = tpdata(last_identifier,last_type,last_type != NULL);
                last_named_phase = TMP_PSTART;
                pos++; continue;
              }

              TPDATA_CONSTRUCT(2);
              tmplate_params[tpc++] = tpdata(last_identifier,last_named_phase != TMP_DEFAULTED ? NULL : last_type);
              last_named_phase = TMP_PSTART;

            pos++; continue;

          //These can all be looked at the same at this point.
          case LN_CLASS: case LN_STRUCT: case LN_STRUCT_DD: case LN_UNION:
              if (cfile[pos] != ';')
              {
                if (last_named_phase == SP_PARENT_NAMED) {
                  last_named_phase = SP_COLON;
                  pos++; continue;
                }

                cferr="Expected ';' instead of ',' when not implemented";
                return pos;
              }
              if (last_named_phase != SP_IDENTIFIER) {
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
                  TPDATA_CONSTRUCT(1);
                  externs* enuScope = current_scope; //Store where we are
                  current_scope = current_scope->parent; //Move up a scope: ENUMs declare in two scopes
                  if (!ExtRegister(last_named,last_named_phase,last_identifier,flag_extern,refstack,builtin_type__int,tmplate_params,tpc,last_value))
                    return pos;
                  current_scope = enuScope; //Move back where we started and declare in that scope
                  if (!ExtRegister(last_named,last_named_phase,last_identifier,flag_extern,refstack,builtin_type__int,tmplate_params,tpc,last_value))
                    return pos;
                }
                else {
                  cferr = "Expected name of constant for enum";
                  return pos;
                }
                last_named_phase = EN_WAITING;
                last_identifier = "";
                pos++; continue;
              }
            break;
          case LN_NAMESPACE:
              if (cfile[pos] != ';') {
                cferr="Expected ';' for namespace declarations, not ','";
                return pos;
              }
              if (last_named_phase != NS_COMPLETE_ASSIGN) { //If it's not a complete assignment, shouldn't see ';'
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
                if (last_named_phase != USE_SINGLE_IDENTIFIER) {
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
                      pu->second->parameter_unify(last_type->refstack);
                    else {
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
          case LN_IMPLEMENT:
              //This isn't helpful at all.
              pos++;
              last_named = LN_NOTHING;
              last_named_phase = 0;
              tmplate_params_clear();
              tpc = -1;
              ihc = 0;
            continue;
          case LN_TYPENAME:
              cferr = "Unexpected symbol in expression";
            return pos;
          default:
              cferr = "This shouldn't have happened. Not that it's totally impossible, but it should be. (" + tostring(last_named) + ")";
            return pos;
        }

        //As a recap, we are in cfile_parse, we're at a semicolon or
        //comma, and we are not typedef'ing anything.

        externs *type_to_use = last_type;
        rf_stack refs_to_use = refstack.dissociate();

        if (type_to_use != NULL) //A case where it would be NULL is struct str;
          while (type_to_use->flags & EXTFLAG_TYPEDEF) // Get to the bottom of the typedefs
          {
            refs_to_use += type_to_use->refstack; // Each typedef level can implement new references
            if (type_to_use->type == NULL) break; // If we're at our end, stop
            type_to_use = type_to_use->type; // Go to the next level otherwise
          }

        if (last_identifier == "")
        {
          if (!last_type or !(last_type->flags & (EXTFLAG_CLASS | EXTFLAG_STRUCT))) {
            /*cferr = "Declaration doesn't declare anything.";
            return pos;*/ //We can't really trust this, as declarations such as "int:16;" are allowed in structs.
            //Let a truly ISO compliant compiler bitch about this. Ours is to read.
            //Useful debugging feature, though.
          }
          else if (last_named == LN_DECLARATOR and last_named_phase == DEC_FULL)
          {
            externs *cs = immediate_scope? immediate_scope:current_scope; // Someimes structs are redeclared, unimplemented, with defaulted template parameters
            if (cs->members.find(last_type->name) != cs->members.end())  // TODO: Remove ExtRegister from this block and manually set the template params.
            {
              last_identifier = last_type->name;

              last_type = NULL;
              last_named = LN_STRUCT;
              last_named_phase = SP_IDENTIFIER;
              TPDATA_CONSTRUCT(2);
              if (!ExtRegister(last_named,last_named_phase,last_identifier,flag_extern,refs_to_use,type_to_use,tmplate_params,tpc))
                return pos;
            }
          }
        }
        else {
          TPDATA_CONSTRUCT(3);
          if (!ExtRegister(last_named,last_named_phase,last_identifier,flag_extern,refs_to_use,type_to_use,tmplate_params,tpc))
          return pos;
        }

        last_identifier = "";

        flag_extern = 0;
        tmplate_params_clear();
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
        flag_extern = 0;
        tmplate_params_clear();
        tpc = -1;
      }

      pos++;
      immediate_scope = NULL;
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
    switch (cfile[pos])
    {
        //First off, the most common is likely to be a pointer indicator.
        case '*':
            {
              //type should be named
              if ((last_named | LN_TYPEDEF) != (LN_DECLARATOR | LN_TYPEDEF))
              {
                if ((last_named | LN_TYPEDEF) != (LN_CLASS  | LN_TYPEDEF)
                and (last_named | LN_TYPEDEF) != (LN_STRUCT | LN_TYPEDEF)
                and (last_named | LN_TYPEDEF) != (LN_UNION  | LN_TYPEDEF))
                {
                  if (last_named == LN_TEMPLATE or last_named == LN_TEMPARGS
                  or (last_named & ~LN_TYPEDEF) == LN_TYPENAME or last_named == LN_TYPENAME_P)
                  { pos++; continue; }
                  cferr = "Unexpected '*' ("+tostring(last_named);
                  return pos;
                }
                if (!extreg_deprecated_struct(false,last_identifier,last_named,last_named_phase,last_type))
                  return pos;
              }
              refstack += referencer('*');
              pos++; continue;
            }
          break;
        //First off, the most common is likely to be a pointer indicator.
        case '&':
            {
              //type should be named
              if ((last_named | LN_TYPEDEF) != (LN_DECLARATOR | LN_TYPEDEF))
              {
                if ((last_named | LN_TYPEDEF) != (LN_CLASS  | LN_TYPEDEF)
                and (last_named | LN_TYPEDEF) != (LN_STRUCT | LN_TYPEDEF)
                and (last_named | LN_TYPEDEF) != (LN_UNION  | LN_TYPEDEF))
                {
                  if (last_named == LN_TEMPLATE or last_named == LN_TEMPARGS
                  or (last_named & ~LN_TYPEDEF) == LN_TYPENAME or last_named == LN_TYPENAME_P)
                  { pos++; continue; }
                  cferr = "Unexpected '&' " + tostring(last_named);
                  return pos;
                }
                if (!extreg_deprecated_struct(false,last_identifier,last_named,last_named_phase,last_type))
                  return pos;
              }
              refstack += referencer('&');
              pos++; continue;
            }
          break;

        case '[':
            {
              //type should be named
              if ((last_named & ~LN_TYPEDEF) != LN_DECLARATOR)
              {
                cferr = "Unexpected '['";
                return pos;
              }
              refstack += referencer('[');
              skipto = ']'; skip_inc_on = '[';
              skippast = true; //I just want to ignore this
              pos++; continue;
            }
          break;

        case '(':
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
                  skippast = true;
                  last_named_phase = DEC_IDENTIFIER;
                  pos++; continue;
                }
                if (last_named_phase != DEC_FULL) {
                  if ((last_named &~ LN_TYPEDEF) != DEC_GENERAL_FLAG and (last_named &~ LN_TYPEDEF) != DEC_ATOMIC_FLAG
                  and (last_named &~ LN_TYPEDEF) != DEC_LONG and (last_named &~ LN_TYPEDEF) != DEC_LONGLONG) {
                    cferr = "Unexpected parenthesis in declaration";
                    return pos;
                  } last_named_phase = DEC_FULL;
                }
                if  (last_type->flags & (EXTFLAG_STRUCT | EXTFLAG_CLASS)
                and (~last_named & LN_TYPEDEF)
                and (last_type == (immediate_scope?immediate_scope:current_scope)
                     or (last_type->members.find(last_type->name) != last_type->members.end())
                     or (last_type->ancestors.size > 0 and last_type->ancestors[0]->name == last_type->name))
                ) { ///FIXME: Make this only execute if we're SURE this is a constructor...
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
                skippast =  true;
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
          break;

        case ')':
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
          break;

        case '{':
          {
            //Because :: can be used to move to a distant scope, we must leave a breadcrumb trail
            externs *push_scope = current_scope;
            bool skipping_to = false; //Also, if this is set to true (like for function implementation), nothing is pushed
            const int last_named_raw = last_named & ~LN_TYPEDEF;

            //Class/Namespace declaration.
            if (last_named_raw == LN_NAMESPACE or last_named_raw == LN_STRUCT or last_named_raw == LN_CLASS or last_named_raw == LN_UNION)
            {
              TPDATA_CONSTRUCT(4);
              if (!ExtRegister(last_named,last_named_phase,last_identifier,flag_extern=0,0,NULL,tmplate_params,tpc))
                return pos;
              current_scope = ext_retriever_var;
            }
            else if (last_named_raw == LN_STRUCT_DD)
            {
              current_scope = last_type;
              for (int i = 0; i < ihc; i++)
                current_scope->ancestors[current_scope->ancestors.size] = inheritance_types[i].parent;
              ihc = 0;
            }
            //Enum declaration.
            else if (last_named_raw == LN_ENUM)
            {
              TPDATA_CONSTRUCT(5);
              if (!ExtRegister(last_named,last_named_phase,last_identifier,flag_extern=0,0,NULL,tmplate_params,tpc))
                return pos;
              scope_stack.push(current_scope);
              current_scope = ext_retriever_var;
              last_identifier = "";
              last_named_phase = EN_WAITING;

              refstack.dump();
              tmplate_params_clear();
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
                TPDATA_CONSTRUCT(6);
                if (!ExtRegister(last_named,last_named_phase,last_identifier,0,refstack.dissociate(),last_type,tmplate_params,tpc))
                  return pos;

                //Skip the code: we don't need to know it ^_^
                skipto = '}'; skip_inc_on = '{';
                skippast = true; //Or remember it happened
                push_scope = NULL;
                skipping_to = 1;
              }
              else
              {
                if (last_type != NULL and last_type->flags & (EXTFLAG_STRUCT|EXTFLAG_CLASS|EXTFLAG_ENUM))
                {
                  //We're at a structure or something, and a type was given (not NULL)
                  //This means we're at "struct named_earlier <{> ... }"
                  if (!last_type->members.empty())
                  {
                    cferr = "Attempting to redeclare struct `"+last_type->name+"'";
                    return pos;
                  }
                  else
                  {
                    current_scope = last_type; //Move into it. Brilliantly simple; we already know it's parent is current_scope.
                    if (last_named & LN_TYPEDEF)
                      current_scope->flags |= EXTFLAG_PENDING_TYPEDEF; //Since we're implementing it now, make sure we're not typedefing it also ;_;
                    //TODO SOME DAY IN THE DISTANT, DISTANT FUTURE:
                    // This is where true template specialization should be handled; where same<tp,tp> is properly linked in memory
                    for (int i = 0; i < tpc; i++)
                      for (unsigned ii = 0; ii < current_scope->tempargs.size; ii++)
                      {
                        if (current_scope->tempargs[ii]->type == tmplate_params[i].def)
                          tmplate_params[i].def = NULL;
                      }
                  }
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
              else if (last_named == LN_IMPLEMENT)
              {
                skipto = '}';
                skip_inc_on = '{';
                skipping_to = true;
                skippast = true;
              }
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
            tmplate_params_clear();
            tpc = -1;
            pos++;

            continue;
          }
        break;

        case '}':
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
                //if (last_named != LN_DECLARATOR or !(current_scope->flags & EXTFLAG_ENUM))
                //{
                  //enum { a = 0 <here> }
                  if (((last_named & ~LN_TYPEDEF) != LN_ENUM)
                  or  (last_named_phase != EN_WAITING and last_named_phase != EN_DEFAULTED and last_named_phase != EN_CONST_IDENTIFIER)) {
                    cferr = "Unexpected closing brace at this point";
                    return pos;
                  }
                  if (last_named_phase != EN_WAITING) {
                    TPDATA_CONSTRUCT(7);
                  if (!ExtRegister(last_named,last_named_phase,last_identifier,flag_extern=0,refstack.dissociate(),builtin_type__int,tmplate_params,tpc,last_value))
                    return pos;
                  }
                /*}
                else
                {
                  cout << "This is reached; don't delete it.";
                  if (!ExtRegister(last_named,last_named_phase,last_identifier,flag_extern,refstack.dissociate(),last_type,tmplate_params,tpc))
                    return pos;
                }*/
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
              tmplate_params_clear();
              tpc = -1;

              pos++; continue;
            }
          break;

        //Two less freqent symbols now, heh.

        case '<':
            {
              //If we're not just past the word "template"
              if ((last_named & ~LN_TYPEDEF) != LN_TEMPLATE)
              {
                if ((last_named & ~LN_TYPEDEF) != LN_DECLARATOR)
                {
                  if (((last_named & ~LN_TYPEDEF) != LN_STRUCT and (last_named & ~LN_TYPEDEF) != LN_CLASS) or last_named_phase != SP_PARENT_NAMED or ihc == 0)
                  {
                    if ((last_named & ~LN_TYPEDEF) != LN_IMPLEMENT)
                    {
                      if ((last_named & ~LN_TYPEDEF) != LN_TYPENAME and (last_named & ~LN_TYPEDEF) != LN_TYPENAME_P)
                      {
                        if (last_type == NULL) {
                          cferr = "Unexpected symbol '<' should only occur directly following `template' token or type";
                          return pos;
                        }
                        if (!(last_type->flags & EXTFLAG_TEMPLATE))
                        {
                          if  (!refstack.is_function()){
                            cferr = "Unexpected symbol '<' [with ln == " + tostring(last_named) + "]";
                            return pos;
                          }
                          if (argument_type == NULL or !(argument_type->flags & EXTFLAG_TEMPLATE)) {
                            cferr = "Unexpected symbol '<' should only occur directly following `template' token or type, even in function parameters <_<\"";
                            return pos;
                          }
                        }
                      }
                      else
                      {
                        skipto = '>';
                        skip_inc_on = '<';
                        skippast = true;
                        specialize_start = pos;
                        specialize_string = "";
                        specializing = true;
                      }
                    }
                    else if (last_named_phase != IM_SCOPE) {
                      cferr = "Unexpected '<' in implementation";
                      return pos;
                    }
                  }
                }
                else //last_named == LN_DECLARATOR here
                {
                  if (last_named_phase != DEC_FULL)
                  {
                    if (last_named_phase == DEC_IDENTIFIER) //In a declarator at '<', but type is named...
                    {
                      if (refstack.is_function())
                      {
                        skipto = '>';
                        skip_inc_on = '<';
                        skippast = true;
                        pos++; continue;
                      }
                      if (find_extname(last_identifier,0xFFFFFFFF,0))
                      {
                        if (ext_retriever_var->is_function())
                        {
                          skipto = '>';
                          skip_inc_on = '<';
                          skippast = true;
                          pos++; continue;
                        }
                        if (ext_retriever_var->flags & (EXTFLAG_STRUCT|EXTFLAG_CLASS|EXTFLAG_NAMESPACE))
                        { //int scope<x>::func() {}
                          last_named = LN_IMPLEMENT;
                          last_named_phase = 0;
                          last_type = ext_retriever_var; //Already know type of what will be implemented. Good thing we just parsed it <_<
                          skipto = '>';
                          skip_inc_on = '<';
                          skippast = true;
                          pos++; continue;
                        }
                      }
                    }
                    cferr = "Unexpected '<' in declaration";
                    return pos;
                  }
                  if (tpc == -1) //tname<(*)...>
                    last_named = LN_TEMPARGS | (last_named & LN_TYPEDEF);
                }

              the_next_block_up:

                skipto = '>';
                skip_inc_on = '<';
                specializing = true;
                specialize_start = ++pos;
                specialize_string = "";
                skippast = true; //This will be handled at '>' anyway

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
              /*else {
                cferr = "Template parameters already named for this declaration";
                return pos;
              }*/

              //current_templates.push(last_type);
              last_named_phase = TMP_PSTART;
              pos++; continue;
            }
          break;

        case '>':
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
                if (last_named_phase != TMP_SIMPLE and last_named_phase != TMP_SIMPLE_DEFAULTED)
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
                  TPDATA_CONSTRUCT(3);
                  tmplate_params[tpc++] = tpdata(last_identifier, last_type, last_value, 1, last_named_phase == TMP_SIMPLE_DEFAULTED);
                }
              }
              else {
                TPDATA_CONSTRUCT(4);
                tmplate_params[tpc++] = tpdata(last_identifier,last_named_phase != TMP_DEFAULTED ? NULL : last_type);
              }

              last_named = LN_NOTHING;
              last_named_phase = 0;
              last_identifier = "";
              last_type = NULL;
              argument_type = NULL;
              pos++; continue;
            }
          break;

        case ':':
            {
              const int last_named_raw = last_named & ~LN_TYPEDEF;
              if (cfile[pos+1] == ':') //Handle '::'
              {
                if (last_named_raw == LN_DECLARATOR)
                {
                  if (last_named_phase == DEC_FULL)
                  {
                    immediate_scope = last_type;
                    last_named &= LN_TYPEDEF; //Set back to either TYPEDEF or NOTHING
                    last_named_phase = 0;
                    last_type = NULL;
                  }
                  else if (last_named_phase == DEC_IDENTIFIER and refstack.is_function())
                    immediate_scope = argument_type;
                  else {
                    cferr = "This error is an easter egg. You found it! Now go report it";
                    return pos;
                  }
                }
                else if (last_named_raw == LN_IMPLEMENT and last_named_phase == IM_SPECD)
                {
                  last_named = LN_DECLARATOR;
                  last_named_phase = DEC_FULL;
                  immediate_scope = argument_type; //We reused this; it's nothing to do with functions in this case.
                }
                else if (last_named_raw == LN_USING and last_named_phase == USE_SINGLE_IDENTIFIER)
                {
                  last_named_phase = USE_NOTHING;
                  immediate_scope = last_type;
                }
                else if ((last_named_raw == LN_STRUCT or last_named_raw == LN_CLASS or last_named_raw == LN_STRUCT_DD)
                     and (last_named_phase == SP_PARENT_NAMED))
                {
                  if (ihc != 0)
                    immediate_scope = inheritance_types[--ihc].parent;
                  else {
                    cferr = "Nothing makes sense anymore...";
                    return pos;
                  }
                  const int iht = inheritance_types[ihc].scopet;
                  last_named_phase =  (iht == ihdata::s_private)? SP_PRIVATE : ((iht == ihdata::s_protected)? SP_PROTECTED : SP_PUBLIC);
                }
                else if (((last_named_raw == LN_TYPENAME) or (last_named_raw == LN_TYPENAME_P)) and last_named_phase >= TN_GIVEN)
                {
                  last_named_phase = last_named_phase == TN_GIVEN? TN_NOTHING : TN_TEMPLATE;
                  immediate_scope = last_type;
                }
                else if (last_named_raw == LN_IMPLEMENT)
                {
                  immediate_scope = last_type;
                  last_named = LN_DECLARATOR;
                  last_type = NULL; //FIXME if necessary
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
                  if (refstack.is_function()) { //Constructors, mostly.
                    skipto = '{';
                    skipto2 = ';';
                    skippast = false; //This is where we'll register the variable. DON'T SKIP IT!
                  }
                  else if (last_named_phase == DEC_IDENTIFIER) {
                    skipto = ';';
                    skipto2 = ';';
                    skippast = false; //This is where we'll register the variable. DON'T SKIP IT!
                  }
                  else if (last_named_phase == DEC_FULL)
                  {
                    if (last_type->flags & (EXTFLAG_CLASS | EXTFLAG_STRUCT))
                    {
                      last_named = LN_STRUCT_DD;
                      last_named_phase = SP_COLON;
                      pos++; continue;
                    }
                    else {
                      skipto = ';';
                      skipto2 = ';';
                      skippast = false; //I may hate myself for this in the morning
                    }
                  }
                  else {
                    cferr = "Unexpected colon";
                    return pos;
                  }
                  pos++;
                  continue;
                }
                if (last_named_raw == LN_STRUCT or last_named_raw == LN_CLASS)
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
          break;

        case '=':
            {
              if (last_named == LN_TEMPLATE)
              {
                if (last_named_phase != TMP_IDENTIFIER and last_named_phase != TMP_SIMPLE)
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
                  skip_inc_on = '<';
                  specializing = true;
                  specialize_start = ++pos;
                  specialize_string = "";
                  skippast = true; //This will be handled at '>' anyway
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
                skippast = false;
                continue;
              }
              if ((last_named & ~LN_TYPEDEF) == LN_ENUM)
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
                skippast = false; //The specialization will be handled, but I want notified at the ';' or whatever.
                continue;
              }
              cferr = "I have no idea what to do with this '=' token. If you see that as a problem, report this error";
              return pos;
            }
          break;
        //...
        case '.':
            if (cfile[pos+1] == '.' and cfile[pos+2] == '.')
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
            else {
              cferr = "Unexpected '.'";
              return pos;
            }
          break;

        //The somewhat uncommon destructor token
        case '~':
            {
              if (last_named != LN_NOTHING) {
                cferr = "Token `~' should only occur to denote a destructor. Furthermore, `~' was fun to type";
                return pos;
              }
              last_named = LN_DESTRUCTOR;
              last_named_phase = 0;
              pos++; continue;
            }
          break;

        //extern "C"
        case '"':
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
          break;

        case '\\':
            {
              pos++;
              if (cfile[pos] == '\r' or cfile[pos] == '\n')
              { pos++; continue; }
              cferr = "Stray backslash in program";
              return pos-1;
            }
          break;
          
        case 0x0C:
            pos++;
          break;

        default:
            cferr = string("Unexpected symbol '")+cfile[pos]+"'";
          return pos;
    } //switch
  } //for


  /*
  string pname = "";
  if (last_typedef != NULL) pname=last_typedef->name;
  cout << '"' << last_typename << "\":" << pname << " \"" << last_identifier << "\"\r\n";*/
  return pt(-1);
}


int cfp_clear_lists()
{
  tmplate_params_clear();
  return 0;
}

