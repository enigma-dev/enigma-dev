/**
 * @file handle_enum.cpp
 * @brief Source implementing a function to handle enumeration declarations.
 * 
 * @section License
 * 
 * Copyright (C) 2011 Josh Ventura
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
#include <Parser/bodies.h>
#include <System/builtins.h>
#include <General/debug_macros.h>
#include <API/compile_settings.h>

static unsigned anon_count = 1111111;
jdi::definition_enum* jdip::context_parser::handle_enum(definition_scope *scope, token_t& token, int inherited_flags)
{
  dbg_assert(token.type == TT_ENUM);
  
  token = read_next_token(scope);
  
  string classname;
  definition_enum *nenum = NULL; // The enum definition. Will be NULL until allocation, unless the enum was already declared.
  bool already_complete = false; // True if nenum is non-NULL, and the enum is already implemented.
  bool will_redeclare = false; // True if this enum is from another scope; so true if implementing this enum will allocate it.
  unsigned incomplete = DEF_INCOMPLETE; // DEF_INCOMPLETE if this enum has a body, zero otherwise.
  
  if (token.type == TT_IDENTIFIER) {
    classname = string((const char*)token.extra.content.str, token.extra.content.len);
    token = read_next_token(scope);
  }
  else if (token.type == TT_DECLARATOR) {
    nenum = (jdi::definition_enum*)token.extra.def;
    classname = nenum->name;
    if (not(nenum->flags & DEF_CLASS)) {
      if (nenum->parent == scope)
        token.report_error(herr, "Attempt to redeclare `" + classname + "' as enum in this scope");
      nenum = NULL;
    }
    else {
      will_redeclare = nenum->parent != scope;
      already_complete = not(nenum->flags & DEF_INCOMPLETE);
    }
    token = read_next_token(scope);
  }
  else {
    char buf[32];
    sprintf(buf, "<anonymousEnum%08d>", anon_count++);
    classname = buf;
  }
  
  #ifdef DEBUG_MODE
    #define derr(x) token.report_error(herr, x);
  #else
    #define derr(x)
  #endif
  
  #define insnew() { \
    pair<definition_scope::defiter, bool> dins = scope->members.insert(pair<string,definition*>(classname,NULL)); \
    if (!dins.second) { derr("Enum `" + classname + "' instantiated inadvertently during parse by another thread. Freeing."); delete dins.first->second; } \
    dins.first->second = nenum = new definition_enum(classname,scope, DEF_ENUM | DEF_TYPENAME | inherited_flags); \
  }
  
  if (!nenum)
    insnew();
  
  if (token.type == TT_COLON) {
    if (will_redeclare) {
      will_redeclare = false;
      insnew();
    }
    else if (already_complete) {
      token.report_error(herr, "Attempting to define type of previously defined enum `" + classname + "'");
    }
    incomplete = 0;
     
    token = read_next_token(scope);
    full_type ft = read_type(lex, token, scope, this, herr); // TODO: Check type for errors.
    nenum->type = ft.def;
    nenum->modifiers = ft.flags;
  }
  else nenum->type = builtin_declarators["int"]->def;
  
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
    if (token.type != TT_IDENTIFIER)
      { token.report_error(herr, "Expected identifier at this point"); token = read_next_token(scope); continue; }
    string cname((const char*)token.extra.content.str, token.extra.content.len);
    
    token = read_next_token(scope);
    if (token.type == TT_OPERATOR) {
      if (token.extra.content.len != 1 or token.extra.content.str[0] != '=') {
        token.report_error(herr, "Expected assignment operator `=' here before secondary operator");
      }
      token = read_next_token(scope);
      AST ast;
      if (ast.parse_expression(token,lex,herr)) {
        token.report_error(herr, "Expected const expression here");
        continue;
      }
      render_ast(ast, "enum_values");
      value v = ast.eval();
      if (v.type != VT_INTEGER)
        token.report_error(herr, "Expected integer result from expression; " + string(v.type == VT_DOUBLE? "floating point": v.type == VT_STRING? "string": "invalid") + " type given"),
        this_value = value(++this_value.val.i);
      else this_value = v;
    }
    else
      this_value = value(++this_value.val.i);
    
    pair<definition_scope::defiter, bool> cins = nenum->constants.insert(pair<string,definition*>(cname,NULL));
    if (cins.second) { // If a new definition key was created, then allocate a new enum representation for it.
      pair<definition_scope::defiter, bool> sins = scope->members.insert(pair<string,definition*>(cname,cins.first->second));
      if (sins.second)
        cins.first->second = sins.first->second = new definition_valued(cname, nenum, nenum->type, nenum->modifiers, this_value);
      else
        token.report_error(herr, "Declatation of constant `" + classname + "' in enumeration conflicts with definition in parent scope");
    }
    else
      token.report_error(herr, "Redeclatation of constant `" + classname + "' in enumeration");
    
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
