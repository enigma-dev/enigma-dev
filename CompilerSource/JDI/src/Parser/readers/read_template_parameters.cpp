/**
 * @file read_template_parameters.cpp
 * @brief Source implementing a function to handle class and structure declarations.
 *
 * @section License
 *
 * Copyright (C) 2012-2013 Josh Ventura
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

#include <API/AST.h>
#include <Parser/context_parser.h>
#include <API/compile_settings.h>
#include <cstdio>

namespace jdi {

int context_parser::read_template_parameter(
    arg_key &argk, size_t argnum, definition_template *temp, token_t &token,
    definition_scope *scope) {
  if (argnum < temp->params.size() &&
      temp->params[argnum]->flags & DEF_TYPENAME) {
    full_type ft = read_fulltype(token, scope);
    if (ft.def)
      argk[argnum].ft().swap(ft);
  }
  else
  {
    AST a;
    a.set_use_for_templates(true);
    static int iv = 0; ++iv;
    astbuilder->parse_expression(&a, token, scope, precedence::comma+1);
    if (argnum < temp->params.size())
    {
      argk.put_value(argnum, a.eval(ErrorContext(herr, token)));
      if (argk[argnum].val().type != VT_INTEGER) {
        if (argk[argnum].val().type == VT_DEPENDENT) {
          argk[argnum].val() = VT_DEPENDENT;
          auto ah = std::make_unique<AST>();
          ah->swap(a);
          argk[argnum].av().ast = std::move(ah);
        }
        else {
          herr->error(token)
              << "Expression must give integer result (value returned: "
              << argk[argnum].val() << "): " << iv;
          render_ast_nd(a, "ass");
          FATAL_RETURN(1); argk[argnum].val() = long(0);
        }
      }
    }
  }
  return 0;
}

int check_read_template_parameters(arg_key &argk, size_t args_given,
                                   definition_template *temp,
                                   ErrorContext errc) {
  if (args_given > temp->params.size()) {
    errc.error() << "Too many template parameters provided to templat "
                 << PQuote(temp->name) << ": wanted " << temp->params.size()
                 << ", got " << args_given;
    FATAL_RETURN(1);
  }

  if (args_given < temp->params.size()) {
    remap_set n;

    for (size_t i = 0; i < args_given; ++i) {
      if (argk[i].type == arg_key::AKT_FULLTYPE) {
        n[temp->params[i].get()] =
          new definition_typed(temp->params[i]->name, temp, argk[i].ft(),
                               DEF_TYPENAME | DEF_TYPED);
      }
      else if (argk[i].type == arg_key::AKT_VALUE) {
        n[temp->params[i].get()] = new definition_valued(
            temp->params[i]->name, temp, temp->params[i]->integer_type.def,
            temp->params[i]->integer_type.flags, DEF_VALUED, argk[i].val());
      } else {
        errc.error() << "Internal error: type of template parameter unknown.";
      }
    }

    for (size_t i = args_given; i < temp->params.size(); ++i) {
      if (temp->params[i]->default_assignment) {
        AST nast(*temp->params[i]->default_assignment, true);
        nast.remap(n, errc);
        if (temp->params[i]->flags & DEF_TYPENAME)
          argk.put_type(i, nast.coerce(errc));
        else
          argk.put_value(i, nast.eval(errc));
      } else {
        errc.error() << "Parameter " << i << " is not defaulted";
      }
    }

    for (remap_iter rit = n.begin(); rit != n.end(); ++rit)
      delete rit->second;
  }

  int bad_params = 0;
  for (size_t i = 0; i < temp->params.size(); ++i) {
    if (argk[i].type == arg_key::AKT_FULLTYPE) {
      if (!argk[i].ft().def)
        ++bad_params;
    }
    else if (argk[i].type == arg_key::AKT_VALUE) {
      if (argk[i].val().type == VT_NONE)
        ++bad_params;
    }
  }

  if (bad_params) {
    errc.error() << "Insufficient parameters to " << PQuote(temp->name) << "; "
                 << bad_params << " more required";
    for (size_t i = 0; i < temp->params.size(); ++i) {
      if ((argk[i].type == arg_key::AKT_FULLTYPE && !argk[i].ft().def) ||
         ((argk[i].type == arg_key::AKT_VALUE &&
           argk[i].val().type == VT_NONE))) {
        errc.error()
            << "Missing parameter " << i << ": parameter is not defaulted";
      }
    }
    FATAL_RETURN(1);
  }

  return 0;
}

int context_parser::read_template_parameters(
    arg_key &argk, definition_template *temp, token_t &token,
    definition_scope *scope) {
  argk.mirror_types(temp);
  size_t args_given = 0;
  for (;;++args_given)
  {
    token = lex->get_token_in_scope(scope);
    if (token.type == TT_GREATERTHAN)
      break;
    if (token.type == TT_SEMICOLON || token.type == TT_LEFTBRACE) {
      herr->error(token)
          << "Expected closing angle bracket to template parameters before "
          << token;
      break;
    }

    if (token.type == TT_COMMA) continue;

    if (read_template_parameter(argk, args_given, temp, token, scope))
      return 1;

    if (token.type == TT_GREATERTHAN) {
      ++args_given;
      break;
    }
    if (token.type != TT_COMMA) {
      herr->error(token) << "Comma expected here before " << token;
      break;
    }
  }

  return check_read_template_parameters(argk, args_given, temp, herr->at(token));
}

}  // namespace jdi
