/**
 * @file read_qualified_definition.cpp
 * @brief Source implementing the parser function to read an entire qualified-id.
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
#include <iostream>

using namespace jdi;
definition* jdi::context_parser::read_qualified_definition(token_t &token, definition_scope* scope)
{
  definition *res;
  if (token.type == TT_SCOPE)
  {
    res = ctex->get_global();
    token = lex->get_token();
    if (token.type == TT_IDENTIFIER)
    {
      res = ((definition_scope*)res)->get_local(token.content.toString());
      if (!res) {
        token.report_error(herr, "No `" + token.content.toString() + "' found in global scope");
        return nullptr;
      }
      if (!(res->flags & DEF_SCOPE)) {
        token = lex->get_token_in_scope(scope);
        return res;
      }
      token.def = res;
    }
    else return res;
  }
  else {
    if (token.type == TT_IDENTIFIER) {
      token.report_error(herr, "Expeceted qualified-id here; `" + token.content.toString() + "' is not declared");
      return nullptr;
    }
    #ifdef DEBUG_MODE
    if (!token.def) {
      for (const auto &sl : lex->detailed_position())
        std::cerr << "In " << sl.filename << ":" << sl.line << ":" << sl.pos << ":\n";
      herr->error(token)
          << "jdi::read_qualified_definition invoked with non-type token "
          << token;
      abort();
    }
    #endif
    res = token.def;
  }
  for (;;) {
    #ifdef DEBUG_MODE
    if (!token.def) {
      std::cerr << "Something in the program went wrong." << std::endl;
      abort();
    }
    #endif
    definition* tdef = token.def; // Reduce the type; full_type::reduce is more work than we need
    while (((tdef->flags & (DEF_TYPED | DEF_TYPENAME)) == (DEF_TYPED | DEF_TYPENAME)) && ((definition_typed*)tdef)->type)
      tdef = ((definition_typed*)tdef)->type;

    if (tdef->flags & DEF_TEMPLATE)
    {
      res = tdef;
      definition_template* dt = (definition_template*)res;
      if (dt->def && dt->def->flags & DEF_CLASS)
      {
        token = lex->get_token_in_scope(scope);
        if (token.type == TT_LESSTHAN)
        {
          arg_key k(dt->params.size());
          if (read_template_parameters(k, dt, token, scope))
            return FATAL_TERNARY(nullptr,res);
          if (k.is_dependent())
            res = handle_dependent_tempinst(scope, dt, k, 0, herr->at(token));
          else
            res =  dt->instantiate(k, ErrorContext(herr, token));
          if (token.type != TT_GREATERTHAN)
            token.report_errorf(herr, "Expected closing triangle bracket before %s");
          token = lex->get_token_in_scope(scope);
        }
        else {
          for (definition_scope* dsi = scope; dsi; dsi = dsi->parent)
          if (dt->def.get() == dsi ||
              ((dsi->flags & DEF_CLASS) &&
               ((definition_class*)dsi)->instance_of == dt)) {
            res = dsi;
          }
        }
      }
      else {
        herr->error(token) << "Template " << PQuote(token.def->name)
                           << " cannot be used as a type";
        return FATAL_TERNARY(nullptr, res);
      }
    }
    else if (tdef->flags & DEF_SCOPE)
    {
      definition_scope* as = (definition_scope*)tdef;
      token = lex->get_token_in_scope(scope);
      if (token.type != TT_SCOPE)
        break;
      token = lex->get_token();
      if (token.type != TT_IDENTIFIER) {
        if (token.type == TT_OPERATORKW) {
          res = token.def = as->look_up(read_operatorkw_name(token, scope));
          if (!token.def)
            return FATAL_TERNARY(nullptr,res);
          if (token.type != TT_SCOPE)
            break;
          continue;
        } else {
          token.report_errorf(herr, "Expected type or qualified-id before %s");
          return FATAL_TERNARY(nullptr,res);
        }
      }
      res = token.def = as->get_local(token.content.toString());
      if (!res) {
        token.report_errorf(herr, "Scope `" + as->name + "' contains no member `" + token.content.toString() + "'");
        return FATAL_TERNARY(nullptr,res);
      }
      continue;
    }
    else {
      token = lex->get_token_in_scope(scope);
      break;
    }

    if (token.type == TT_SCOPE) {
      #ifdef DEBUG_MODE
        if (!res) { token.report_error(herr, "Accessing nullptr scope..."); return nullptr; }
        if (!(res->flags & DEF_SCOPE)) { token.report_error(herr, "Accessing non-scope object " + res->name + "..."); return nullptr; }
      #endif
      token = lex->get_token();
      if (token.type != TT_IDENTIFIER) {
        if (token.type == TT_OPERATORKW) {
          string dname = read_operatorkw_name(token, scope);
          return ((definition_scope*)res)->get_local(dname);
        }
        if (token.type == TT_TILDE) {
          token = lex->get_token();
          if (token.type != TT_IDENTIFIER || token.content.toString() != res->name) {
            token.report_errorf(herr, "Expected class name following '~' before %s");
            return nullptr;
          }
          res = ((definition_scope*)res)->get_local("~" + res->name);
          token = lex->get_token_in_scope(scope);
          return res;
        }
        token.report_errorf(herr, "Expected variable name following `::' before %s");
        return nullptr;
      }

      // This loop checks token.def for things; we have to set it here.
      token.def = ((definition_scope*)res)->get_local(token.content.toString());
      if (!token.def) {
        token.report_error(herr, "Scope `" + res->name + "' does not contain `" + token.content.toString() + "'");
        return nullptr;
      }
      res = token.def;
    }
    else break;
  }
  return res;
}
