/**
 * @file handle_enum.cpp
 * @brief Source implementing a function to handle enumeration declarations.
 * 
 * @section License
 * 
 * Copyright (C) 2011-2013 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include <cstdio>
#include <API/AST.h>
#include <Parser/context_parser.h>
#include <System/builtins.h>
#include <General/debug_macros.h>
#include <API/compile_settings.h>

using namespace jdip;

#define def_kind enum
#define DEF_FLAG DEF_ENUM
#include <Parser/cclass_base.h>

jdi::definition_enum* jdip::context_parser::handle_enum(definition_scope *scope, token_t& token, int inherited_flags)
{
  dbg_assert(token.type == TT_ENUM);
  
  token = read_next_token(scope);
  
  string classname;
  definition_enum *nenum = NULL; // The enum definition. Will be NULL until allocation, unless the enum was already declared.
  bool already_complete = false; // True if nenum is non-NULL, and the enum is already implemented.
  bool will_redeclare = false; // True if this enum is from another scope; so true if implementing this enum will allocate it.
  unsigned incomplete = DEF_INCOMPLETE; // DEF_INCOMPLETE if this enum has a body, zero otherwise.
  
  if (get_location(nenum, will_redeclare, already_complete, token, classname, scope, this, herr))
    return NULL;
  
  if (!nenum)
    if (not(nenum = insnew(scope,inherited_flags,classname,token,herr)))
      return NULL;
  
  if (token.type == TT_COLON) {
    if (will_redeclare) {
      will_redeclare = false;
      if (not(nenum = insnew(scope,inherited_flags,classname,token,herr)))
      return NULL;
    }
    else if (already_complete) {
      token.report_error(herr, "Attempting to define type of previously defined enum `" + classname + "'");
    }
    incomplete = 0;
     
    token = read_next_token(scope);
    full_type ft = read_fulltype(token, scope); // TODO: Check type for errors.
    nenum->type = ft.def;
    nenum->modifiers = ft.flags;
  }
  else nenum->type = builtin_type__int;
  
  if (token.type != TT_LEFTBRACE) {
    return nenum;
  }
  
  value this_value = 0L;
  token = read_next_token(scope);
  while (token.type != TT_RIGHTBRACE) {
    if (token.type == TT_ENDOFCODE) {
      token.report_error(herr, "Expected closing brace to enum `" + classname + "'");
      return FATAL_ERRORS_T(NULL, nenum);
    }
    string cname;
    if (token.type != TT_IDENTIFIER) {
        if (token.type != TT_DECLARATOR && token.type != TT_DEFINITION) {
        token.report_errorf(herr, "Expected identifier for constant name before %s"); token = read_next_token(scope);
        FATAL_RETURN(NULL);
        continue;
      }
      cname = token.def->name;
    }
    else
      cname = token.content.toString();
    
    token = read_next_token(scope);
    
    AST *ast = NULL;
    if (token.type == TT_OPERATOR) {
      if (token.content.len != 1 or token.content.str[0] != '=') {
        token.report_error(herr, "Expected assignment operator `=' here before secondary operator");
      }
      token = read_next_token(scope);
      ast = new AST();
      if (astbuilder->parse_expression(ast, token, scope, precedence::comma + 1)) {
        token.report_error(herr, "Expected const expression here");
        continue;
      }
      render_ast(*ast, "enum_values");
      value v = ast->eval(error_context(herr, token));
      if (v.type != VT_INTEGER && v.type != VT_DEPENDENT) {
        #ifdef DEBUG_MODE
          token.report_error(herr, "Expected integer result from expression; " + string(v.type == VT_DOUBLE? "floating point": v.type == VT_STRING? "string": "invalid") + " type given (expression: " + ast->expression + ")");
        #else
          token.report_error(herr, "Expected integer result from expression; " + string(v.type == VT_DOUBLE? "floating point": v.type == VT_STRING? "string": "invalid") + " type given");
        #endif
        this_value = value(++this_value.val.i);
      }
      else {
        this_value = v;
        if (v.type != VT_DEPENDENT) {
          delete ast;
          ast = NULL;
        }
      }
    }
    
    pair<definition_scope::defiter, bool> cins = nenum->members.insert(pair<string,definition*>(cname,NULL));
    if (cins.second) { // If a new definition key was created, then allocate a new enum representation for it.
      definition_valued *dv = new definition_valued(cname, nenum, nenum->type, nenum->modifiers, 0, this_value);
      nenum->constants.push_back(definition_enum::const_pair(dv, ast));
      scope->use_general(cname, cins.first->second = dv);
    }
    else
      token.report_error(herr, "Redeclatation of constant `" + classname + "' in enumeration");
    
    ++this_value.val.i;
    
    if (token.type == TT_COMMA)
      token = read_next_token(scope);
    else {
      if (token.type != TT_RIGHTBRACE)
        token.report_error(herr, "Expected right brace here to terminate enumeration");
      break;
    }
  }
  token = read_next_token(scope);
  
  nenum->flags |= incomplete;
  return nenum;
}
