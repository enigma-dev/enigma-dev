/**
 * @file handle_union.cpp
 * @brief Source implementing a function to handle union declarations.
 *
 * This file is nearly identical to handle_class.cpp; there are only a few
 * differences in how unions are parsed (the lack of heredity being a key one).
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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 *
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include <cstdio>
#include <Parser/context_parser.h>
#include <API/compile_settings.h>

using namespace jdi;

#define def_kind union
#define DEF_FLAG DEF_UNION
#include <Parser/cclass_base.h>

definition_union* context_parser::handle_union(
    definition_scope *scope, token_t& token, int inherited_flags) {
  #ifdef DEBUG_MODE
  if (token.type != TT_UNION)
    token.report_error(herr, "PARSE ERROR: handle_union invoked with non-union token.");
  #endif

  token = read_next_token(scope);

  string classname;
  definition_union *nunion = nullptr; // The class definition. Will be null until allocation, unless the class was already declared.
  bool already_complete = false; // True if nunion is non-null, and the class is already implemented.
  bool will_redeclare = false; // True if this class is from another scope; so true if implementing this class will allocate it.
  unsigned incomplete = DEF_INCOMPLETE; // DEF_INCOMPLETE if this class has a body, zero otherwise.

  if (get_location(nunion, will_redeclare, already_complete, token, classname, scope, this, herr))
  return nullptr;

  if (!nunion)
    if (not(nunion = insnew(scope,inherited_flags,classname,token,herr)))
      return nullptr;

  if (token.type == TT_COLON) {
    token.report_error(herr, "Attempting to add ancestors to previously defined class `" + classname + "'");
    FATAL_RETURN(nullptr);
    do token = read_next_token(scope);
    while (token.type != TT_LEFTBRACE && token.type != TT_SEMICOLON && token.type != TT_ENDOFCODE);
  }

  if (token.type == TT_LEFTBRACE)
  {
    incomplete = 0;
    if (will_redeclare) {
      will_redeclare = false;
      if (not(nunion = insnew(scope,inherited_flags,classname,token,herr)))
        return nullptr;
    }
    else if (already_complete) {
      token.report_error(herr, "Attempting to add members to previously defined union `" + classname + "'");
    }
    if (handle_scope(nunion, token, 0)) FATAL_RETURN(nullptr);
    if (token.type != TT_RIGHTBRACE) {
      token.report_error(herr, "Expected closing brace to union `" + classname + "'");
      FATAL_RETURN(nullptr);
    }
    token = read_next_token(scope);
  }

  nunion->flags |= incomplete;
  return nunion;
}
