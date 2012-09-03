/**
 * @file handle_hypothetical.cpp
 * @brief Source implementing context_parser::handle_hypothetical.
 * 
 * @section License
 * 
 * Copyright (C) 2011-2012 Josh Ventura
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
#include <API/compile_settings.h>

namespace jdip {
  definition_hypothetical* handle_hypothetical(lexer *lex, definition_scope *scope, token_t& token, unsigned flags, error_handler *herr) {
    // Verify that we're in a template<> statement.
    definition_scope* temps;
    for (temps = scope; temps and not (temps->flags & (DEF_TEMPLATE | DEF_TEMPSCOPE)); temps = temps->parent);
    if (!temps) {
      token.report_errorf(herr, "Invalid use of `typename' keyword: must be in a template");
      return NULL;
    }
    
    // We are in a template<> declaration. Insert our hypothetical 
    definition_template* temp = temps->flags & DEF_TEMPLATE? (definition_template*)temps : (definition_template*)((definition_tempscope*)temps)->source;
    if (!temp->flags & DEF_TEMPLATE) {
      token.report_error(herr, "`" + temp->name + "' is not a template");
    }
    
    AST *a = new AST();
    if (a->parse_expression(token, lex, scope, precedence::scope, herr))
      { FATAL_RETURN(1); }
    
    definition_hypothetical* h = new definition_hypothetical("<dependent member>", scope, flags, a);
    temp->dependents.push_back(h);
    return h;
  }
}
