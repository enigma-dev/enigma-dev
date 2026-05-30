/**
 * @file handle_hypothetical.cpp
 * @brief Source implementing context_parser::handle_hypothetical.
 *
 * @section License
 *
 * Copyright (C) 2011-2014 Josh Ventura
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

#include <Parser/context_parser.h>
#include <API/compile_settings.h>

namespace jdi {

inline definition_hypothetical* handle_hypothetical_ast(
    unique_ptr<AST> a, definition_scope *scope, unsigned flags,
    ErrorContext errc) {
  definition_scope *temps;
  for (temps = scope; temps && !(temps->flags & DEF_TEMPLATE);
       temps = temps->parent);
  if (!temps || !(temps->flags & DEF_TEMPLATE)) {
    errc.error() << "Cannot infer type outside of template";
    return nullptr;
  }
  definition_template *temp = (definition_template*) temps;
  std::string n = "(?=" + a->toString() + ")";
  temp->dependents.emplace_back(
      std::make_unique<definition_hypothetical>(n, scope, flags, std::move(a)));
  return temp->dependents.back().get();
}

definition* context_parser::handle_dependent_tempinst(
    definition_scope *scope,  definition_template *temp, const arg_key &key,
    unsigned flags, ErrorContext errc) {
  if (scope->flags & DEF_TEMPLATE) {
    definition_template::specialization *spec = temp->find_specialization(key);
    if (spec && spec->spec_temp && spec->spec_temp->def) {
      return spec->spec_temp->def.get();
    }
    return temp->def.get();
  }
  unique_ptr<AST> a = AST::create_from_instantiation(temp, key);
  if (temp->def && (temp->def->flags & (DEF_CLASS | DEF_TYPENAME)))
    flags |= DEF_TYPENAME;
  return handle_hypothetical_ast(std::move(a), scope, flags, errc);
}

definition_hypothetical* context_parser::handle_hypothetical_access(
    definition_hypothetical *scope, string id, ErrorContext errc) {
  unique_ptr<AST> a = AST::create_from_access(scope, id, "::");
  return handle_hypothetical_ast(std::move(a), scope->parent, scope->flags,
                                 errc); // XXX: scope->flags, that & DEF_PRIVATE/whaever, or 0?
}

definition_hypothetical* context_parser::handle_hypothetical(
    definition_scope *scope, token_t &token, unsigned flags) {
  unique_ptr<AST> a = std::make_unique<AST>();
  if (astbuilder->parse_expression(a.get(), token, scope, precedence::scope)) {
    FATAL_RETURN(1);
  }
  return handle_hypothetical_ast(std::move(a), scope, flags, herr->at(token));
}

}  // namespace jdi
