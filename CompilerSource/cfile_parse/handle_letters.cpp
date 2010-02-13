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

#include "cfile_pushing.h"
#include "macro_functions.h"

extern externs* builtin_type__int;

extern string cfile_top;
extern string cferr_get_file_orfirstfile();
int get_line()
{
  int iline=0;
    for (unsigned int i=0; i<pos; i++)
    {
      if (cfile[i]=='\n')
        iline++;
    }
  return iline;
}

#include "../general/parse_basics.h"

bool ExtRegister(unsigned int last,unsigned phase,string name,rf_stack refs,externs *type,varray<tpdata> &tparams, int tpc = -1, long long last_value = 0);

//struct a { struct a *b; }, not struct a { a *b }
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
    if (!ExtRegister(LN_STRUCT,last_named_phase,last_identifier,NO_REFS,NULL,EMPTY,tpc = -1))
      return 0;
  }
  last_named = LN_DECLARATOR;
  last_named_phase = idnamed?DEC_IDENTIFIER:DEC_FULL;
  last_type = ext_retriever_var;
  return 1;
}

int handle_identifiers(const string n,int &fparam_named,bool at_scope_accessor,bool at_template_param)
{
  //it's not a macro, so the next thing we'll check for is keyword
  if (n == "class")
    cout << "oh oh.";
  switch (switch_hash(n))
  {
    case SH_STRUCT: case SH_CLASS:
        if (n=="struct" or n=="class")
        {
          //Struct can only really follow typedef.
          if (last_named != LN_NOTHING)
          {
            if (last_named != LN_TYPEDEF)
            {
              if (last_named != LN_DECLARATOR or !(last_named_phase == 0 or refstack.currentsymbol() == '('))
              {
                if (last_named != LN_TEMPLATE or last_named_phase != TMP_PSTART)
                {
                  cferr="Unexpected `"+n+"' token";
                  return pos;
                }
                last_named_phase = TMP_TYPENAME;
                return -1;
              }
            }
            else //last_named == ln_typedef
              last_named |= LN_STRUCT;
          }
          else last_named = LN_STRUCT;
          return -1;
        }
      break;
    case SH_ENUM: if (n=="enum")
        {
          //Same with enum
          if (last_named != LN_NOTHING)
          {
            if (last_named != LN_TYPEDEF)
            {
              cferr="Unexpected `enum' token";
              return pos;
            }
            last_named |= LN_ENUM;
          }
          else
            last_named = LN_ENUM;
          last_named_phase = EN_NOTHING;
          return -1;
        }
      break;
    case SH_TYPEDEF: //case SH_THROW:
        if (n=="typedef")
        {
          //Typedef can't follow anything
          if (last_named != LN_NOTHING)
          {
            cferr="Unexpected `typedef' token";
            return pos;
          }
          last_named = LN_TYPEDEF;
          return -1;
        }
        if (n == "throw")
        {
          if (last_named != LN_DECLARATOR or (refstack.currentsymbol() != '(' and refstack.currentsymbol() != ')'))
          { cferr = "Unexpected `throw' token"; return pos; }
          last_named_phase = DEC_THROW;
          return -1;
        }
      break;
      break;
    case SH_EXTERN: if (n=="extern")
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
              cferr = "Stray `extern' token at this point, not expected after DECID_" + tostring(last_named_phase);
              return pos;
            }
            return -1;
          }
          last_named = LN_DECLARATOR;
          last_named_phase = 0;
          return -1;
        }
      break;
    case SH_UNION: 
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
    //break;
    //case SH_USING: 
       if (n=="using")
        {
          if (last_named != LN_NOTHING) {
            cferr = "Unexpecting `using' token";
            return pos;
          }
          last_named = LN_USING;
          last_named_phase = USE_NOTHING;
          return -1;
        }
      break;
    case SH_NAMESPACE: if (n=="namespace")
        {
          //Namespace can only follow using, when it's not on its own.
          if (last_named != LN_NOTHING)
          {
            if (last_named != LN_USING or last_named_phase != USE_NOTHING)
            {
              cferr="Unexpected `namespace' token: "+tostring(last_named);
              return pos;
            }
            last_named_phase = USE_NAMESPACE; //using namespace...
          }
          else
            last_named = LN_NAMESPACE; //namespace...
          return -1;
        }
      break;
    case SH_EXPLICIT: if (n=="explicit")
        { //This is for GCC to know, and us to just be okay with.
          return -1;
        }
      break;
    case SH_OPERATOR: if (n=="operator")
        {
          if (!(current_scope->flags & (EXTFLAG_CLASS | EXTFLAG_STRUCT)))
          {
            skipto = '{';
            skipto2 = ';';
            skippast = false; //Need to know if we hit '{' so we can begin move toward '}'
            
            last_named = LN_IMPLEMENT;
            last_named_phase = 0;
            last_identifier = "";
            tpc = ihc = 0;
            
            return -1;
          }
          if (last_named != LN_DECLARATOR or last_named_phase < 1)
          {
            //cferr="Expected declarator before `operator' token";
            last_named = LN_OPERATOR;
            last_named_phase = OP_CAST;
            last_identifier = "operator";
            return -1;
          }
          last_named = LN_OPERATOR;
          last_named_phase = 0;
          //cout << last_named << " = " << last_named_phase << "\r\n";
          return -1;
        }
      break;
    case SH_NEW: if (n=="new")
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
      break;
    case SH_DELETE: if (n=="delete")
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
      break;
    case SH_TEMPLATE: if (n=="template")
        {
          if (last_named != LN_NOTHING)
          {
            if (last_named == LN_DECLARATOR and last_named_phase == 0) //extern
            { //extern template: This is garbage.
              skipto = ';';
              skipto2 = ';';
              skippast = false; //; will clear everything anyway
              last_named = LN_NOTHING;
              return -1;
            }
            if ((((last_named & ~LN_TYPEDEF) == LN_TYPENAME) or (last_named == LN_TYPENAME_P)) and last_named_phase == TN_NOTHING) { // typename tp::template
              last_named_phase = TN_TEMPLATE;
              return -1; 
            }
            cferr = "Unexpected `template' token: "+tostring(last_named);
            return pos;
          }
          last_named = LN_TEMPLATE;
          last_named_phase = 0;
          return -1;
        }
      break;
    case SH_TYPENAME: if (n=="typename")
        {
          if (last_named != LN_TEMPLATE or last_named_phase != TMP_PSTART)
          {
            if (((last_named & ~LN_TYPEDEF) == LN_DECLARATOR) and (last_named_phase > DEC_NOTHING_YET and last_named_phase < DEC_FULL))
              last_named &= LN_TYPEDEF;
            if (last_named == LN_TYPEDEF or last_named == LN_NOTHING) //Plain old typedef... Nothing else named yet; or just nothing at all
            {
              last_named |= LN_TYPENAME;
              last_named_phase = TN_NOTHING;
              return -1;
            }
            if (((last_named & ~LN_TYPEDEF) == LN_DECLARATOR) and (last_named_phase == DEC_IDENTIFIER) and refstack.is_function())
            {
              last_named = LN_TYPENAME_P; //This relies on a truth I don't plan to document elsewhere:
              last_named_phase = TN_NOTHING;  //You can't typedef a usable function.
              return -1;
            }
            cferr = "Unexpected `typename' token";
            return pos;
          }
          last_named_phase = TMP_TYPENAME;
          return -1;
        }
      break;
    case SH_INLINE: if (n=="inline")
      return -1;
    case SH_CONST: case SH___CONST:
        if (n=="const" or n=="__const") //or for that matter, if n fucking= ____const__
          return -1; //Put something here if const ever fucking matters
      break;
    case SH_FRIEND: if (n=="friend")
        {
          if (last_named or last_named_phase) {
            cferr = "Unexpected `friend' token";
            return pos;
          }
          skipto = ';';
          skipto2 = 0;
          skippast = true;
          return -1;
        }
      break;
    case SH_PRIVATE: case SH_PUBLIC: case SH_PROTECTED: 
        if (n=="private" or n=="protected" or n=="public")
        {
          if (last_named == LN_STRUCT or last_named == LN_CLASS or last_named == LN_STRUCT_DD)
          {
            if (last_named_phase != SP_COLON) {
              cferr = "Unexpected `" + n + "' token in structure declaration";
              return pos;
            }
            switch (n[2]) {
              case 'i': last_named_phase = SP_PRIVATE;   break;
              case 'o': last_named_phase = SP_PROTECTED; break;
              case 'b': last_named_phase = SP_PUBLIC;    break;
            }
          }
          else
          {
            if (last_named != LN_NOTHING) {
              cferr = "What the hell is this doing here?";
              return pos;
            }
            last_named = LN_LABEL;
            //last_named_phase == LBL_
          }
          return -1;
        }
      break;
    case SH_VIRTUAL: if (n=="virtual")
      return -1;
    case SH_MUTABLE: if (n=="mutable")
      return -1;
  }
  
  //This is the end of the reserved words.
  //Now we make sure we're not accessing something from this id's scope, meaning this id must be a scope of some sort
  if (at_scope_accessor)
  {
    if (!find_extname(n,EXTFLAG_CLASS | EXTFLAG_STRUCT | EXTFLAG_NAMESPACE))
    {
      if (immediate_scope == NULL)
        for (externs *cscope = current_scope; cscope; cscope = cscope->parent)
          for (unsigned i = 0; i < cscope->tempargs.size; i++)
            if (cscope->tempargs[i]->name == n)
            {
              immediate_scope = cscope->tempargs[i];
              return -1;
            }
      
      //This happens so rarely as to barely justify its existence
      //And to fully justify a second pass.
      if (find_extname(n,EXTFLAG_TYPEDEF))
      {
        while (ext_retriever_var->type and (ext_retriever_var->flags & EXTFLAG_TYPEDEF))
        {
          ext_retriever_var = ext_retriever_var->type;
          if (ext_retriever_var->flags & (EXTFLAG_CLASS | EXTFLAG_STRUCT | EXTFLAG_HYPOTHETICAL)) {
            immediate_scope = ext_retriever_var;
            return -1;
          }
          //If it's discernably a template
          if (ext_retriever_var->flags & EXTFLAG_TYPEDEF and (ext_retriever_var->type == NULL or (ext_retriever_var->flags & EXTFLAG_DEFAULTED))){
            immediate_scope = ext_retriever_var;
            return -1;
          }
        }
      }
      
      cferr = "Cannot access `" + n + "' as scope from `" + (immediate_scope?immediate_scope->name:current_scope->name) + "'";
      return pos;
    }
    immediate_scope = ext_retriever_var;
    return -1;
  }
  
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
        last_named_phase = tflag_atomic(n) ? DEC_ATOMIC_FLAG : DEC_GENERAL_FLAG; //See section on bindingly atomic flags
      return -1;
    }
    
    if ((last_named | LN_TYPEDEF) == (LN_DECLARATOR | LN_TYPEDEF) 
    or (last_named | LN_TYPEDEF) == (LN_TEMPARGS | LN_TYPEDEF))
    {
      if (last_named_phase != DEC_FULL)
      {
        if (last_named_phase != DEC_IDENTIFIER)
        {
          if (n=="long")
          {
            if (last_named_phase == DEC_NOTHING_YET
            or  last_named_phase == DEC_GENERAL_FLAG
            or  last_named_phase == DEC_ATOMIC_FLAG)
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
            last_named_phase = tflag_atomic(n) ? DEC_ATOMIC_FLAG : DEC_GENERAL_FLAG;
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
      else if ((last_named & ~LN_TYPEDEF) == LN_TEMPLATE) //template<int> can be used in specialization
      {
        last_named_phase = (last_named_phase == TMP_EQUALS or last_named_phase == TMP_DEFAULTED)?TMP_DEFAULTED:TMP_SIMPLE;
        return -1;
      }
      else
      {
        cferr="Unexpected declarator at this point.";
        return pos;
      }

      return -1;
    }
    if (last_named == LN_TYPEDEF) //if typedef is single, phase==0
    {
      last_named=LN_DECLARATOR | LN_TYPEDEF;
      if (n=="long")
        last_named_phase = DEC_LONG;
      else
        last_named_phase = tflag_atomic(n) ? DEC_ATOMIC_FLAG : DEC_GENERAL_FLAG;
      return -1;
    }
    
    if ((last_named | LN_TYPEDEF) == (LN_TEMPLATE | LN_TYPEDEF) and (last_named_phase == TMP_EQUALS or last_named_phase == TMP_DEFAULTED))
    {
      last_named_phase = TMP_DEFAULTED;
      return -1;
    }
    
    if ((last_named | LN_TYPEDEF) == (LN_OPERATOR | LN_TYPEDEF))
    {
      if (last_named_phase != OP_CAST) {
        cferr = "Unexpected type name in operator expression";
        return pos;
      }
      last_identifier += " " + n;
      return -1;
    }
    
    cferr="Unexpected declarator at this point...";
    return pos;
  }
  
  
  //Check if it's a primitive or anything user defined that serves as a type.
  if (find_extname(n,EXTFLAG_TYPENAME,0))
  {
    //cout << n << " is type \n";
    if (last_named == LN_NOTHING or last_named == LN_TYPEDEF)
    {
      last_type = ext_retriever_var;
      last_named |= LN_DECLARATOR;
      last_named_phase = DEC_FULL;
      return -1;
    }
    
    //If we're declaring a variable by type
    if ((last_named & ~LN_TYPEDEF) == LN_DECLARATOR or (last_named & ~LN_TYPEDEF) == LN_TEMPARGS)
    {
      //Single EXTFLAG_TYPENAME implies atomic type.
      //We are fully qualified if either phase == DEC_FULL, Or if it's DEC_ATOMIC_TYPE/LONG/LONGLONG and we're not an atomic type.
      const bool atat = last_named_phase == DEC_ATOMIC_FLAG or last_named_phase == DEC_LONG or last_named_phase == DEC_LONGLONG;
      if (last_named_phase != DEC_FULL and !(atat and ext_retriever_var->flags != EXTFLAG_TYPENAME))
      {
        //In the case that both of those are false, we assume this will finish it
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
        else argument_type = ext_retriever_var;
        fparam_named = 1;
        return -1;
      } 
      //If error, or if it was declared in this scope
      else if (ext_retriever_var == NULL) {
        cferr = "Type unimplemented in this scope";
        return pos;
      }
      else if (ext_retriever_var->parent == current_scope)
      {
        if (!at_template_param or !(ext_retriever_var->flags & (EXTFLAG_STRUCT | EXTFLAG_CLASS))) {
          cferr = "Two types named in declaration: `"+ext_retriever_var->name+"' cannot be declared in this scope";
          return pos;
        }
        
        //Sometimes STL programmers like to say typename a b::c; which is fuck-annoying.
        //Also, we have no use for int a::b() {}
        //In either case, we'll skip.
        
        //This tells us nothing new, there's no need to parse it.
        //This will cause the parser to skip to the nearest ";" or "{", the latter case resulting in a skip to "}"
        skipto = '{';
        skipto2 = ';';
        skippast = false; //Need to know if we hit '{' so we can begin move toward '}'
        
        last_named = LN_IMPLEMENT;
        last_named_phase = 0;
        last_identifier = "";
        
        return -1;
      }
      //If we made it this far, we are redeclaring something in this scope that is different in higher scopes
      //These next segments of elses are skipped, and the variable is treated like new.
    }
    //Check if we're declaring a new struct
    else //Last isn't a declarator
    if ((last_named &~ LN_TYPEDEF) == LN_STRUCT
    or  (last_named &~ LN_TYPEDEF) == LN_CLASS
    or  (last_named &~ LN_TYPEDEF) == LN_STRUCT_DD)
    {
      //We're dealing with struct structid
      //If we're not right after "struct" (or the like) or are capable of redeclaring it in this scope
      if (last_named_phase != SP_EMPTY)
      {
        if (last_named_phase != SP_PRIVATE and last_named_phase != SP_PROTECTED and last_named_phase != SP_PUBLIC)
        {
          //This is sad, really, but it's standard...
          if (last_named_phase == SP_COLON)
            last_named_phase = SP_PUBLIC;
          else {
            cferr = "Structure already identified, expected undeclared identifier";
            return pos;
          }
        }
        inheritance_types[ihc++] = ihdata(ext_retriever_var,last_named_phase == SP_PUBLIC ? ihdata::s_public : last_named_phase == SP_PRIVATE ? ihdata::s_private : ihdata::s_protected);
        last_named_phase = SP_PARENT_NAMED;
        return -1;
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
    else if ((last_named | LN_TYPEDEF) == (LN_TEMPLATE | LN_TYPEDEF))
    {
      if (last_named_phase == TMP_EQUALS or last_named_phase == TMP_DEFAULTED)
      {
        last_named_phase = TMP_DEFAULTED;
        last_type = ext_retriever_var;
        return -1;
      }
      else if (last_named_phase == TMP_PSTART) //Plain old type, to be passed a constant expression
      {
        last_named_phase = TMP_SIMPLE;
        last_type = ext_retriever_var;
        return -1;
      }
      else if (last_named_phase != TMP_TYPENAME) { //template <typename string> is fine
        cferr = "Unexpected type in template parameters: " + tostring(last_named_phase);
        return pos;
      }
      //Else skip to the end of this huge block down to the part where we handle identifiers.
    }
    else if (last_named == LN_USING) //last chance... hopefully we're using this
    {
      if (last_named_phase != USE_NOTHING) {
        cferr = "Unexpected typename in `using' statement";
        return pos;
      }
      last_named_phase = USE_SINGLE_IDENTIFIER;
      last_type = ext_retriever_var;
      return -1;
    }
    else if (last_named == LN_DESTRUCTOR and (ext_retriever_var->flags & (EXTFLAG_CLASS | EXTFLAG_STRUCT)))
    {
      last_named = LN_DECLARATOR;
      last_named_phase = DEC_IDENTIFIER;
      last_type = ext_retriever_var;
      last_identifier = "~" + n;
      return unsigned(-1);
    }
    else if ((last_named | LN_TYPEDEF) == (LN_ENUM   | LN_TYPEDEF))
    {
      /*cferr = "Unexpected type name in enumeration";
      return pos;*/
    }
    else if ((last_named | LN_TYPEDEF) == (LN_OPERATOR | LN_TYPEDEF))
    {
      if (last_named_phase != OP_CAST) {
        cferr = "Unexpected type name in enumeration";
        return pos;
      }
      externs* ttu = ext_retriever_var;
      while ((ttu->flags & EXTFLAG_TYPEDEF) and ttu->type)
        ttu = ttu->type;
      last_identifier += " " + strace(ttu);
      return -1;
    }
    else if ((last_named & ~LN_TYPEDEF) == LN_TYPENAME or last_named == LN_TYPENAME_P)
    {
      last_type = ext_retriever_var;
      last_named = LN_DECLARATOR | (last_named & LN_TYPEDEF);
      last_named_phase = last_named == LN_TYPENAME_P ? DEC_IDENTIFIER : DEC_FULL;
      return -1;
    }
    //Not declaring by type or giving default template value
    else //Note: This else is here because the above will need to pass this block     //struct a;
    {    //in the case of the current type being redeclared as scalar in this scope   //namespace b { int a; }
      cferr = "Unexpected declarator at this point";
      cout << "\nTracing path to `" << ext_retriever_var->name << "': " <<strace(ext_retriever_var);
      cout << endl << ext_retriever_var->flags << " flags\n";// on iteration " << iteration << endl;
      return pos;
    }
  }
  
  
  
  //Here's the big part
  //We now assume that what was named is a regular identifier.
  //This means we do a lot of error checking here.
  if (last_named == LN_NOTHING) //what we have here is a standalone identifier.
  {
    if (!(current_scope != &global_scope and (current_scope->flags & EXTFLAG_ENUM))) {
      cferr = "Expected type name or keyword before identifier";
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
  if (last_named == LN_TYPEDEF) { //plain typedef, not typedef | declarator
    cferr = "Type definition does not specify a type";
    return pos;
  }
  
  //Find what preceded this identifier.
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
          last_named_phase = TMP_IDENTIFIER;
      break;
    case LN_CLASS:
    case LN_STRUCT:
    case LN_UNION: //Class, struct, or union
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
        last_named_phase = SP_IDENTIFIER;
      break;
    case LN_ENUM:
        if (last_named_phase == EN_IDENTIFIER or last_named_phase == EN_CONST_IDENTIFIER) {
          cferr="Expected '{' or ',' before identifier";
          return pos;
        }
        
        if (last_named_phase == EN_NOTHING)
          last_named_phase = EN_IDENTIFIER;
        else if (last_named_phase == EN_WAITING)
          last_named_phase = EN_CONST_IDENTIFIER;
        else {
          cferr = "Unexpected identifier in enumeration";
          return pos;
        }
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
          if (!find_extname(n,EXTFLAG_NAMESPACE)) {
            cferr = "Expected namespace identifier following `namespace' keyword";
            return pos;
          }
          last_named_phase = USE_NAMESPACE_IDENTIFIER;
          last_type = ext_retriever_var;
          break;
        }
        if (!find_extname(n,0xFFFFFFFF)) {
          cferr = "Cannot use `" + n + "' from `"+(immediate_scope?strace(immediate_scope):"current scope")+"': undeclared";
          return pos;
        }
        
        last_named_phase = USE_SINGLE_IDENTIFIER;
        last_type = ext_retriever_var;
      break;
    case LN_TEMPARGS:
        cferr = "Unexpected identifier in template parameters";
      return pos;
    case LN_IMPLEMENT:
        cferr = "Unexpected identifier in implementation";
      return pos;
    case LN_TYPENAME:
    case LN_TYPENAME_P:
        if (last_named_phase == TN_NOTHING)
        {
          varray<tpdata> empty;
          last_type = ExtRegister(LN_TYPEDEF | LN_DECLARATOR,DEC_IDENTIFIER,n,0,NULL,empty,0,0) ? ext_retriever_var : NULL;
          last_named = LN_DECLARATOR | (last_named & LN_TYPEDEF); last_named_phase = last_named == LN_TYPENAME_P ? DEC_IDENTIFIER : DEC_FULL;
          if (last_type)
          {
            last_type->flags &= ~EXTFLAG_PENDING_TYPEDEF;
            last_type->flags |= EXTFLAG_TYPENAME | EXTFLAG_TYPEDEF | EXTFLAG_HYPOTHETICAL | EXTFLAG_STRUCT;
            return -1;
          }
          return pos;
        }
        else //if (last_named_phase == TN_TEMPLATE)
        {
          varray<tpdata> single;
          single[0] = tpdata("",builtin_type__int,0,true,true);
          last_type = ExtRegister(LN_TYPEDEF | LN_DECLARATOR,DEC_IDENTIFIER,n,0,NULL,single,1,0) ? ext_retriever_var : NULL;
          last_named = LN_DECLARATOR | (last_named & LN_TYPEDEF); last_named_phase = last_named == LN_TYPENAME_P ? DEC_IDENTIFIER : DEC_FULL;
          if (last_type)
          {
            last_type->flags &= ~EXTFLAG_PENDING_TYPEDEF;
            last_type->flags |= EXTFLAG_TYPENAME | EXTFLAG_TYPEDEF | EXTFLAG_HYPOTHETICAL | EXTFLAG_STRUCT;
            return -1;
          }
          return pos;
        }
      return pos;
    default:
        cferr = "Unspecified Error. This shouldn't happen... Result: last_named = "+tostring(last_named);
      return pos;
      //last_named = LN_IDENTIFIER;
      //last_named_phase = 0;
  }

  last_identifier = n;
  return -1;
}
