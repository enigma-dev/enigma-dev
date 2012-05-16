/**
 * @file handle_namespace.cpp
 * @brief Source implementing a function to handle the namespace keyword.
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

#include <Parser/bodies.h>

int jdip::context_parser::handle_namespace(definition_scope *scope, token_t& token)
{
  token = read_next_token( scope);
  if (token.type != TT_IDENTIFIER) {
    token.report_error(herr, "Expected namespace name here.");
    return 1;
  }
  
  // Copy the name and ensure it's a member of this scope.
  string nsname((const char*)token.extra.content.str, token.extra.content.len);
  pair<definition_scope::defiter, bool> dins = scope->members.insert(pair<string,definition*>(nsname,NULL));
  
  definition_scope *nscope;
  if (dins.second) // If a new definition key was created, then allocate a new namespace representation for it.
    dins.first->second = nscope = new definition_scope(nsname,scope,DEF_NAMESPACE);
  else {
    nscope = (definition_scope*)dins.first->second;
    if (not(dins.first->second->flags & DEF_NAMESPACE)) {
      token.report_error(herr, "Attempting to redeclare `" + nsname + "' as a namespace");
      return 1;
    }
  }
  
  token = read_next_token(scope);
  if (token.type != TT_LEFTBRACE) {
    token.report_error(herr, "Expected opening brace for namespace definition.");
    return 1;
  }
  if (handle_scope(nscope, token)) return 1;
  if (token.type != TT_RIGHTBRACE) {
    token.report_error(herr, "Expected closing brace to namespace `" + nsname + "'");
    return 1;
  }
  return 0;
}
