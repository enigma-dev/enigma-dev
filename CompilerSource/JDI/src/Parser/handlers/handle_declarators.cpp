/**
 * @file handle_declarators.cpp
 * @brief Source implementing the parser function to handle standard declarations.
 *
 * This file's function will be referenced by every other function in the
 * parser. The efficiency of its implementation is of crucial importance.
 * If this file runs slow, so do the others in the parser.
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
#include <API/context.h>
#include <General/parse_basics.h>
#include <General/debug_macros.h>
#include <General/utils.h>
#include <Parser/is_potential_constructor.h>
#include <System/builtins.h>
#include <API/compile_settings.h>
#include <API/AST.h>
#include <cstdio>

#include "handle_function_impl.h"

using namespace jdi;
using std::make_unique;

static unsigned anon_count = 0;
namespace jdi {

definition *dangling_pointer = nullptr;

int context_parser::handle_declarators(definition_scope *scope, token_t& token,
                                       unsigned inherited_flags,
                                       definition* &res) {
  // Skip destructor tildes; log if we are a destructor
  bool dtor = token.type == TT_TILDE;
  const bool is_inline = token.type == TT_DECFLAG && token.content.view() == "inline";
  if (dtor) token = read_next_token(scope);

  // Outsource to read_fulltype, which will take care of the hard work for us.
  // When this function finishes, per its specification, our token will be set to the next relevant, non-referencer symbol.
  // This means an identifier if the syntax is correct.
  full_type tp = read_fulltype(token, scope);
  if (dtor) {
    if (tp.refs.name.empty() && tp.def == scope && !tp.flags &&
        tp.refs.size() == 1 && tp.refs.top().type == ref_stack::RT_FUNCTION) {
      tp.refs.name = "~" + scope->name;
      tp.def = builtin_type__void;
    } else {
      token.report_error(herr, "Junk destructor; remove tilde?");
      FATAL_RETURN(1);
    }
  }

  // Make sure we actually read a valid type.
  if (!tp.def) {
    if (token.type == TT_TILDE) {
      token = read_next_token(scope);
      full_type tp2 = read_fulltype(token, scope);
      if (!tp2.refs.name.empty() || tp2.def != scope || tp2.flags ||
          tp2.refs.size() != 1 || tp2.refs.top().type != ref_stack::RT_FUNCTION) {
        token.report_error(herr, "Junk destructor; remove tilde?");
        FATAL_RETURN(1);
      }
      tp2.refs.name = "~" + scope->name;
      tp2.flags |= tp.flags;
      tp2.def = builtin_type__void;
      tp.swap(tp2);
    }
    else if (token.type == TT_OPERATORKW) {
      full_type ft = read_operatorkw_cast_type(token, scope);
      if (!ft.def)
        return 1;
      res = scope->overload_function("(cast)", ft, inherited_flags,
                                     herr->at(token));
      return !res;
    }
    else if (is_inline && token.type == TT_NAMESPACE) {
      definition_scope *ns = handle_namespace(scope, token);
      if (!ns) return 1;
      scope->use_namespace(ns);
      if (token.type != TT_RIGHTBRACE) return 1;
      token.type = TT_SEMICOLON;
      return 0;
    } else {
      token.report_error(herr, "Declaration does not give a valid type");
      return 1;
    }
  }

  return handle_declarators(scope, token, tp, inherited_flags, res);
}

int context_parser::handle_declarators(
    definition_scope *scope, token_t& token, full_type &tp,
    unsigned inherited_flags, definition* &res) {
  // Make sure we do indeed find ourselves at an identifier to declare.
  if (tp.refs.name.empty()) {
    const bool potentialc = is_potential_constructor(scope, tp);
    if (potentialc and !(tp.flags & invalid_ctor_flags) and tp.refs.size() == 1 and tp.refs.top().type == ref_stack::RT_FUNCTION) {
      tp.refs.name = constructor_name;
      if (token.type == TT_COLON) {
        // TODO: When you have a place to store constructor data,
        handle_constructor_initializers(lex, token, scope, herr);
      }
    }
    else if (token.type == TT_COLON) {
      if (scope->flags & DEF_CLASS) {
        char anonname[32];
        sprintf(anonname,"<anonymousField%010d>",anon_count);
        tp.refs.name = anonname;
      }
      else
        token.report_warning(herr, "Declaration without name is meaningless outside of a class");
    }
    else if (token.type == TT_DEFINITION or token.type == TT_DECLARATOR) {
      definition *d = token.def;
      token = read_next_token(scope);
      rescope: {
        while (token.type == TT_SCOPE) {
          if (!(d->flags & DEF_SCOPE)) {
            token.report_error(herr, "Cannot access `" + d->name + "' as scope");
            FATAL_RETURN(1); break;
          }
          token = read_next_token((definition_scope*)d);
          if (token.type != TT_DEFINITION and token.type != TT_DECLARATOR) {
            if (token.type == TT_IDENTIFIER)
              token.report_errorf(herr, "Expected qualified-id before %s; `" + token.content.toString() + "' is not a member of `" + d->name + "'");
            else
              token.report_errorf(herr, "Expected qualified-id before %s");
            FATAL_RETURN(1); break;
          }
          d = token.def;
          token = read_next_token(scope);
        }
        if (token.type == TT_LESSTHAN and d->flags & DEF_TEMPLATE) {
          definition_template* temp = (definition_template*)d;
          arg_key k(temp->params.size());
          if (read_template_parameters(k, temp,token, scope))
            return 1;
          d = temp->instantiate(k, ErrorContext(herr, token));
          if (!d) return 1;
          token = read_next_token(scope);
          goto rescope;
        }
      }
      if (d and (d->flags & DEF_FUNCTION))
        read_referencers_post(tp.refs, token, d->parent);
      else
        read_referencers_post(tp.refs, token, scope);
      res = d; goto extra_loop;
    }
    else if (token.type == TT_COMMA) {
      if (tp.refs.name.empty()) {
        if (~scope->flags & DEF_CLASS)
          token.report_warning(herr, "Declaration without name is meaningless outside of a class");
        else
          token.report_warning(herr, "Declaration in class scope doesn't have a name");
        char buf[64];
        sprintf(buf, "<unnamed%08d>", anon_count++);
        tp.refs.name = buf;
      }
    }
    else
      return 0;
  }

  if (!tp.refs.ndef) {
    // Add it to our definitions map, without overwriting the existing member.
    decpair ins = ((definition_scope*)scope)->declare(tp.refs.name);
    if (ins.inserted) {  // If we successfully inserted
      insert_anyway:
      if (!tp.refs.empty() && tp.refs.top().type == ref_stack::RT_FUNCTION) {
        auto fun = make_unique<definition_function>(tp.refs.name, scope,
                                                    inherited_flags);
        res = fun->overload(tp, inherited_flags, herr->at(token));
        ins.def = std::move(fun);
      } else {
        ins.def = make_unique<definition_typed>(tp.refs.name, scope,
                                                tp.def, &tp.refs, tp.flags,
                                                DEF_TYPED | inherited_flags);
        res = ins.def.get();
      }
    } else {
      // Well, uh-oh. We didn't insert anything.
      // This is non-fatal, and will not leak, so no harm done.

      // If the original definition is a class
      if (ins.def->flags & (DEF_CLASS | DEF_UNION | DEF_ENUM)) {
        // Move that definition to the C structs list, so we can insert our
        // definition in its place.
        decpair cins = scope->declare_c_struct(tp.refs.name, std::move(ins.def));
        if (!cins.inserted && cins.def != ins.def) {
          herr->error(token) << "Attempt to redeclare " << PQuote(tp.refs.name)
                             << " failed due to name conflicts";
          FATAL_RETURN(1);
        }
        else goto insert_anyway;
      }
      // Handle function overloading
      else if (ins.def->flags & DEF_FUNCTION) {
        if (tp.refs.empty() or tp.refs.top().type != ref_stack::RT_FUNCTION) {
          token.report_error(herr, "Cannot declare `" + tp.refs.name + "' over existing function");
          return 4;
        }
        definition_function* func = (definition_function*) ins.def.get();
        res = func->overload(tp, inherited_flags, herr->at(token));
      }
      // More function overloading
      else if (!(ins.def->flags & DEF_TYPED)) {
        if (ins.def->flags & DEF_TEMPLATE and !tp.refs.empty() &&
            tp.refs.top().type == ref_stack::RT_FUNCTION) {
          auto func = make_unique<definition_function>(
              tp.refs.name, scope, tp.def, tp.refs, tp.flags,
              DEF_TYPED | DEF_FUNCTION | inherited_flags, token, herr);
          func->overload(cast_unique<definition_template>(std::move(ins.def)),
                         herr->at(token));
          ins.def = std::move(func);
          res = ins.def.get();
        }
        else {
          herr->error(token) << "Redeclaration of " << PQuote(tp.refs.name)
                             << " as a different kind of symbol";
          if (scope->parent) {
            herr->info(token) << "In scope " << PQuote(scope->name);
          } else {
            herr->info(token) << "At global scope";
          }
          //cerr << ins.def->toString() << endl;
          return 3;
        }
      } else {
        res = ins.def.get();
      }
    }
  }
  else {
    res = tp.refs.ndef;
    if (res->flags & DEF_FUNCTION) {
      if (tp.refs.empty() or tp.refs.top().type != ref_stack::RT_FUNCTION) {
        token.report_error(herr, "Cannot declare `" + tp.refs.name + "' over existing function");
        return 4;
      }
      definition_function* func = (definition_function*)res;
      res = func->overload(tp, inherited_flags, herr->at(token));
    }
    // cout << "Implementing " << res->name << std::endl;
  }

  extra_loop:
  for (;;)
  {
    switch (token.gloss_type()) {
      case GTT_EQUAL: {
          AST ast;
          token = read_next_token(scope);
          astbuilder->parse_expression(&ast, token, scope, precedence::comma);
        break;
      }
      case GTT_OPERATORMISC:
        if (token.type == TT_COMMA) {
          // Move past this comma
          token = read_next_token(scope);

          // Read a new type
          read_referencers(tp.refs, tp, token, scope);

          // Just hop into the error checking above and pass through the definition addition again.
          return handle_declarators(scope, token, tp, inherited_flags, res);
        } else if (token.type == TT_COLON) {
          definition *root = tp.def;
          while (root->flags & DEF_TYPED && (root = ((definition_typed*)root)->type));
          if (root != builtin_type__int) {
            token.report_error(herr, "Attempt to assign bit size in non-integer declaration");
            FATAL_RETURN(1);
          }
          AST bitcountexp;
          astbuilder->parse_expression(&bitcountexp, token = read_next_token(scope), scope, precedence::comma+1);
          value bc = bitcountexp.eval(ErrorContext(herr, token));
          if (bc.type != VT_INTEGER) {
            token.report_error(herr, "Bit count is not an integer");
            FATAL_RETURN(1);
          }
          // TODO: Store the bit count somewhere
          break;
        }
        token.report_error(herr, "Unexpected operator `" + token.content.toString() + "' at this point");
        return 5;

      case GTT_LITERAL:
          token.report_error(herr, "Expected initializer `=' here before literal.");
        return 5;

      case GTT_ASM: case GTT_OPERATORKW: case GTT_SEMICOLON:
      case GTT_DECLARATOR: case GTT_BRACKET: case GTT_MEMORYOP: case GTT_PREPROCESSOR:
      case GTT_CONSTRUCT: case GTT_TYPEOP: case GTT_VISIBILITYSPEC: case GTT_IDENTIFIER:
      case GTT_TEMPLATE: case GTT_USING: case GTT_ARITHMETIC: case GTT_RELATIVE_ASSIGN:
      case GTT_ANGLE: case GTT_ENDOFCODE: case GTT_INVALID: case GTT_CONTROL:
      default:
        return 0;
      }
  }

  return 0;
}

}  // namespace jdi

