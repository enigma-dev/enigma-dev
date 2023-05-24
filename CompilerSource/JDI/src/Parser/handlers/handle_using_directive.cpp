/**
 * @file  handle_using_directive.cpp
 * @brief Source implementing handling for `using` directives.
 *
 * @section License
 *
 * Copyright (C) 2011-2022 Josh Ventura
 * This file is part of JustDefineIt.
 *
 * JustDefineIt is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License, or
 * (at your option) any later version.
 *
 * JustDefineIt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for details.
 *
 * You should have received a copy of the GNU General Public License
 * along with JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include <Parser/context_parser.h>

jdi::definition *jdi::context_parser::handle_using_directive(
    definition_scope *scope, token_t& token) {
  // Skip "using"
  token = read_next_token(scope);
  if (token.type == TT_NAMESPACE) {
    token = lex->get_token_in_scope(scope);
    if (token.type == TT_DEFINITION) {
      definition *d = read_qualified_definition(token, scope);
      if (!d) {
        token.report_errorf(herr, "Expected namespace-name following `namespace' token");
        return nullptr;
      } else {
        if (d->flags & DEF_NAMESPACE)
          scope->use_namespace((definition_scope*) d);
        else
          token.report_error(herr, "Expected namespace-name following `namespace' token");
      }
      if (token.type == TT_SEMICOLON) {
        token = read_next_token(scope);
      } else {
        token.report_errorf(herr, "Expected semicolon before %s");
        return nullptr;
      }
      return d;
    } else {
      token.report_errorf(herr, "Expected namespace to use before %s");
      return nullptr;
    }
  } else if (token.type == TT_IDENTIFIER) {
    token_t alias_token = token;
    std::string alias = token.content.toString();
    token = lex->get_token_in_scope(scope);
    if (token.type != TT_EQUAL) {
      herr->error(token) << "Expected `=` for `using` directive";
      return nullptr;
    }
    token = lex->get_token_in_scope(scope);
    full_type tp = read_type(token, scope);
    decpair ins = scope->declare(alias);
    if (ins.inserted) {  // If we successfully inserted
      ins.def = std::make_unique<definition_typed>(
          alias, scope, tp.def, &tp.refs, tp.flags, DEF_TYPED);
    } else {
      herr->error(alias_token)
          << "Redefinition of symbol `" << alias << "` as `using` alias";
      return nullptr;
    }
    if (token.type != TT_SEMICOLON) {
      herr->error(token)
          << "Expected `;` following `using` directive before " << token;
    }
    return ins.def.get();
  } else {
    definition *usedef = read_qualified_definition(token, scope);
    if (usedef) {
      scope->use_general(usedef->name, usedef);
    } else {
      token.report_errorf(herr, "Using directive does not specify an object");
    }
    if (token.type != TT_SEMICOLON) {
      herr->error(token) << "Expected semicolon before " << token
                         << " to terminate using directive";
    }
    return usedef;
  }
}
