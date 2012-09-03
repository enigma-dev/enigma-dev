/**
 * @file handle_union.cpp
 * @brief Source implementing a function to handle union declarations.
 *
 * This file is nearly identical to handle_class.cpp; there are only a few
 * differences in how unions are parsed (the lack of heredity being a key one).
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
#include <Parser/bodies.h>
#include <API/compile_settings.h>

using namespace jdip;

static inline definition_union* insnew(definition_scope *const &scope, int inherited_flags, const string& classname, const token_t &token, error_handler* const& herr, context *ct) {
  definition_union* nclass = NULL;
  decpair dins = scope->declare(classname);
  if (!dins.inserted) {
    if (dins.def->flags & DEF_TYPENAME) {
      token.report_error(herr, "Union `" + classname + "' instantiated inadvertently during parse by another thread. Freeing.");
      delete ~dins.def;
    }
    else {
      dins = ct->declare_c_struct(classname);
      if (dins.inserted)
        goto my_else;
      if (dins.def->flags & DEF_UNION)
        nclass = (definition_union*)dins.def;
      else {
        #if FATAL_ERRORS
          return NULL;
        #else
          delete ~dins.def;
          goto my_else;
        #endif
      }
    }
  } else { my_else:
    dins.def = nclass = new definition_union(classname,scope, DEF_UNION | DEF_TYPENAME | inherited_flags);
  }
  return nclass;
}

static unsigned anon_count = 1;
jdi::definition_union* jdip::context_parser::handle_union(definition_scope *scope, token_t& token, int inherited_flags)
{
  #ifdef DEBUG_MODE
  if (token.type != TT_UNION)
    token.report_error(herr, "PARSE ERROR: handle_union invoked with non-union token.");
  #endif
  
  token = read_next_token(scope);
  
  string classname;
  definition_union *nclass = NULL; // The class definition. Will be NULL until allocation, unless the class was already declared.
  bool already_complete = false; // True if nclass is non-NULL, and the class is already implemented.
  bool will_redeclare = false; // True if this class is from another scope; so true if implementing this class will allocate it.
  unsigned incomplete = DEF_INCOMPLETE; // DEF_INCOMPLETE if this class has a body, zero otherwise.
  
  // TRUTH TABLE
  // nclass    already_complete   will_redeclare  Meaning40.8686675
  // NULL      *                  *               Nothing declared. Allocate new ASAP.
  // Non-NULL  False              False           Incomplete class in this scope; may be implemented.
  // Non-NULL  False              True            Incomplete class in another scope; can be implemented new in this scope.
  // Non-NULL  True               False           Complete class in this scope. MUST be used as a type, not implemented.
  // Non-NULL  True               True            Complete class in another scope; can be redeclared (reallocated and reimplemented) in this scope.
  
  if (token.type == TT_IDENTIFIER || token.type == TT_DEFINITION) {
    classname = token.content.toString();
    token = read_next_token(scope);
  }
  else if (token.type == TT_DECLARATOR) {
    nclass = (jdi::definition_union*)token.def;
    classname = nclass->name;
    if (not(nclass->flags & DEF_UNION)) {
      if (nclass->parent == scope)
        token.report_error(herr, "Attempt to redeclare `" + classname + "' as union in this scope");
      nclass = NULL;
    }
    else {
      will_redeclare = nclass->parent != scope;
      already_complete = not(nclass->flags & DEF_INCOMPLETE);
    }
    token = read_next_token(scope);
  }
  else {
    char buf[32];
    sprintf(buf, "<anonymousUnion%08d>", anon_count++);
    classname = buf;
  }
  
  if (!nclass)
    if (not(nclass = insnew(scope,inherited_flags,classname,token,herr,this)))
      return NULL;
  
  if (token.type == TT_COLON) {
    token.report_error(herr, "Attempting to add ancestors to previously defined class `" + classname + "'");
    FATAL_RETURN(NULL);
    do token = read_next_token(scope);
    while (token.type != TT_LEFTBRACE && token.type != TT_SEMICOLON && token.type != TT_ENDOFCODE);
  }
  
  if (token.type == TT_LEFTBRACE)
  {
    incomplete = 0;
    if (will_redeclare) {
      will_redeclare = false;
      if (not(nclass = insnew(scope,inherited_flags,classname,token,herr,this)))
        return NULL;
    }
    else if (already_complete) {
      token.report_error(herr, "Attempting to add members to previously defined union `" + classname + "'");
    }
    if (handle_scope(nclass, token, 0)) FATAL_RETURN(NULL);
    if (token.type != TT_RIGHTBRACE) {
      token.report_error(herr, "Expected closing brace to union `" + classname + "'");
      FATAL_RETURN(NULL);
    }
    token = read_next_token(scope);
  }
  
  nclass->flags |= incomplete;
  return nclass;
}
