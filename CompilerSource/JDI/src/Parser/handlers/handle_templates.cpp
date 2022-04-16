/**
 * @file  handle_templates.cpp
 * @brief Source implementing a function to handle a template<> declaration.
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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include <Parser/context_parser.h>
#include <API/AST.h>
#include <API/compile_settings.h>
#include <Parser/handlers/handle_function_impl.h>
#include <General/parse_basics.h>
#include <Parser/is_potential_constructor.h>
#include <cstdio>

namespace jdi {

using std::make_unique;

#if FATAL_ERRORS
#define ERROR_CODE 1
#else
#define ERROR_CODE 0
#endif

template<typename T, typename U>
bool operator==(unique_ptr<T> const& x, U *y) { return x.get() == y; }
template<typename T, typename U>
bool operator==(T *x, unique_ptr<U> const& y) { return x == y.get(); }

static inline bool in_template(definition *d) {
  for (definition* s = d; s; s = s->parent)
    if (s->flags & DEF_TEMPLATE)
      return true;
  return false;
}

int context_parser::read_specialization_parameters(
    arg_key &argk, definition_template *basetemp,
    definition_template::specialization *spec, token_t &token) {
  definition_template *temp = spec->spec_temp.get();
  argk.mirror_types(basetemp);
  size_t args_given = 0;
  for (;; ++args_given) {
    token = read_next_token(temp);
    if (token.type == TT_GREATERTHAN)
      break;
    if (token.type == TT_SEMICOLON || token.type == TT_LEFTBRACE) {
      herr->error(token)
          <<"Expected closing angle bracket to template parameters before "
          << token;
      break;
    }

    if (token.type == TT_COMMA) continue;

    if (args_given >= basetemp->params.size()) {
      herr->error(token)
          << "Too many parameters for template " << PQuote(basetemp->name);
      return 1;
    }

    if ((argk[args_given].type == arg_key::AKT_VALUE) &&
        (token.type == TT_DEFINITION or token.type == TT_DECLARATOR) &&
        (token.def->flags & DEF_TEMPPARAM)) {
      for (size_t i = 0; i < temp->params.size(); ++i) {
        if (temp->params[i] == token.def) {
          if (~temp->params[i]->flags & DEF_TYPENAME) {
            spec->key.arg_inds[i][++spec->key.arg_inds[i][0]] = args_given;
            argk[args_given].val() = VT_DEPENDENT;
            token = read_next_token(temp);
            goto handled_argk; // break 2;
          } else {
            herr->error(token)
                << "Type mismatch in passing parameter " << args_given
                << " to template specialization: real-valued parameter expected";
          }
        }
      }
    }

    if (read_template_parameter(argk, args_given, basetemp, token, temp)) {
      return 1;
    }

    if (argk[args_given].type == arg_key::AKT_FULLTYPE) {
      definition *def = argk[args_given].ft().def;
      for (size_t i = 0; i < temp->params.size(); ++i) {
        if (temp->params[i] == def) {
          spec->key.arg_inds[i][++spec->key.arg_inds[i][0]] = args_given;
          argk[args_given].ft().def = arg_key::abstract;
        }
      }
    }

    handled_argk:

    if (token.type == TT_GREATERTHAN) {
      ++args_given;
      break;
    }
    if (token.type != TT_COMMA) {
      herr->error(token)
          << "Comma or closing triangle bracket expected here before " << token;
      break;
    }
  }
  if (check_read_template_parameters(argk, args_given, basetemp,
                                     herr->at(token))) {
    return 1;
  }
  return 0;
}

int context_parser::handle_template(definition_scope *scope, token_t& token,
                                    unsigned inherited_flags) {
  token = read_next_token(scope);
  if (token.type != TT_LESSTHAN) {
    herr->error(token) << "Expected opening angle bracket following "
                       << PQuote("template") << " token";
    return ERROR_CODE;
  }
  token = read_next_token(scope);

  unique_ptr<definition_template> temp =
      make_unique<definition_template>("", scope,
                                       DEF_TEMPLATE | inherited_flags);

  for (;;) {
    string pname; // The name given to this parameter
    unsigned dtpflags = DEF_TEMPPARAM | DEF_DEPENDENT;

    if (token.type == TT_TYPENAME || token.type == TT_CLASS ||
        token.type == TT_STRUCT) {
      token = lex->get_token();
      if (token.type == TT_IDENTIFIER) {
        pname = token.content.toString();
        token = read_next_token(temp.get());
      }
      dtpflags |= DEF_TYPENAME;
    }
    else if (token.gloss_type() == GTT_DECLARATOR) {
      full_type fts = read_fulltype(token, temp.get());
      pname = fts.refs.name;
    }
    else {
      if (token.type == TT_GREATERTHAN) break;
      token.report_errorf(herr, "Expected '>' token here before %s");
      FATAL_RETURN(1);
      break;
    }

    unique_ptr<AST> ast;
    if (token.type == TT_EQUAL) {
      token = read_next_token(temp.get());
      ast = make_unique<AST>();
      ast->set_use_for_templates(true);
      astbuilder->parse_expression(ast.get(), token, temp.get(),
                                   precedence::comma + 1);
    }
    if (auto dtn = make_unique<definition_tempparam>(
                       pname, temp.get(), std::move(ast), dtpflags)) {
      temp->use_general(pname, dtn.get());
      temp->params.push_back(std::move(dtn));
    }
    if (token.type == TT_GREATERTHAN)
      break;
    if (token.type != TT_COMMA)
      herr->error(token) << "Expected '>' or ',' before " << token;
    token = read_next_token(temp.get());
  }

  token = read_next_token(temp.get());

  // ===========================================================================
  // =====: Handle template class definitions :=================================
  // ===========================================================================

  if (token.type == TT_CLASS || token.type == TT_STRUCT) {
    unsigned protection = token.type == TT_CLASS? DEF_PRIVATE : 0;
    token = read_next_token(scope);
    definition_class *tclass;

    if (token.type == TT_IDENTIFIER) {
      regular_identifier:
      temp->name = token.content.toString();
      // Store a temporary of the proper type. Read "if" as "with".
      if (auto uc = make_unique<definition_class>(temp->name, temp.get(),
                                                  DEF_CLASS | DEF_TYPENAME)) {
        tclass = uc.get();
        temp->def = std::move(uc);
      }

      /* Avoid UB problems from argument order */ {
        const string &temp_name = temp->name;
        scope->declare(temp_name, std::move(temp));
      }

      token = read_next_token(scope);
      regular_template_class:

      if (token.type == TT_COLON) {
        if (handle_class_inheritance(tclass, token, tclass, protection))
          return 1;
      }

      if (token.type != TT_LEFTBRACE) {
        if (token.type == TT_SEMICOLON) {
          tclass->flags |= DEF_INCOMPLETE;
          return 0;
        }
        herr->error(token)
            << "Opening brace for class body expected before " << token;
        return 1;
      }

      if (handle_scope(tclass, token, protection))
        FATAL_RETURN(1);

      if (token.type != TT_RIGHTBRACE) {
        herr->error(token)
            << "Expected closing brace to class body before " << token;
        FATAL_RETURN(1);
      }

      return 0;
    }
    else if (token.type == TT_DEFINITION) {
      if (token.def->parent != scope)
        goto regular_identifier;

      if (!((token.def->flags & DEF_TEMPLATE) &&
            (((definition_template*) token.def)->def->flags & DEF_CLASS))) {
        herr->error(token)
            << "Expected class name for specialization; "
            << PQuote(token.def->name) << " does not name a template class";
        return 1;
      }

      definition_template *basetemp = (definition_template*) token.def;

      token = read_next_token(temp.get());
      if (token.type != TT_LESSTHAN) {
        if (basetemp->def &&
            basetemp->def->has_all_flags(DEF_CLASS | DEF_INCOMPLETE)) {
          if (basetemp->params.size() != temp->params.size()) {
            herr->error(token)
                << "Argument count differs from forward declaration; was "
                << basetemp->params.size() << "; now " << temp->params.size();
            return 1;
          }
          for (size_t i = 0; i < basetemp->params.size(); ++i) {
            if (basetemp->params[i]->flags != temp->params[i]->flags) {
              herr->error(token) << "Parameter " << i
                                 << " of declaration differs from forward";
              return 1;
            }
            if (!temp->params[i]->name.empty() &&
                basetemp->params[i]->name != temp->params[i]->name) {
              auto it = basetemp->using_general.find(basetemp->params[i]->name);
              if (it != basetemp->using_general.end()) {
                if (it->second == basetemp->params[i]) {
                  basetemp->using_general.erase(it);
                } else {
                  herr->warning(token)
                      << "Template parameter "
                      << PQuote(basetemp->params[i]->name)
                      << " in forward declaration's using is not mapped to "
                      << PQuote(temp->params[i]->name);
                }
              } else if (!basetemp->params[i]->name.empty()) {
                herr->warning(token)
                    << "Template parameter " << PQuote(basetemp->params[i]->name)
                    << " not found in forward declaration's using; "
                    << "cannot be renamed to " << PQuote(temp->params[i]->name);
              }
              basetemp->params[i]->name = temp->params[i]->name;
              basetemp->use_general(basetemp->params[i]->name,
                                    basetemp->params[i].get());
            }
          }
          tclass = (definition_class*) basetemp->def.get();
          temp.reset();
          goto regular_template_class;
        }
        herr->error(token)
            << "Expected opening angle bracket for template definition before "
            << token;
        return 1;
      }  // if (token.type != TT_LESSTHAN)

      // =======================================================================
      // =: Handle specialization. :============================================
      // =======================================================================

      arg_key argk(basetemp->params.size());
      auto spec =
          make_unique<definition_template::specialization>(basetemp,
                                                           std::move(temp));
      read_specialization_parameters(argk, basetemp, spec.get(), token);

      spec->filter = argk;
      definition_template::speclist &slist = basetemp->specializations;

      definition_template *extemp = nullptr;
      definition_template::specialization *exspec = nullptr;
      for (auto it = slist.begin(); it != slist.end(); ++it) {
        if ((*it)->key.same_as(spec->key)) {
          exspec = it->get();
          if (~exspec->spec_temp->flags & DEF_INCOMPLETE) {
            herr->error(token)
                << "Cannot specialize template " << PQuote(temp->name)
                << ": specialization by this type already exists.";
            return 1;
          }

          // XXX: Maybe replace this with something to specifically delete
          // template parameters, just in case? Could anything else be used?
          // I'm thinking not.
          exspec->spec_temp->using_general.clear();
          for (auto pi = temp->params.begin(); pi != temp->params.end(); ++pi) {
            exspec->spec_temp->use_general((*pi)->name, pi->get());
          }
          extemp = exspec->spec_temp.get();
          break;
        }
      }
      if (!exspec) {
        exspec = spec.get();
        slist.push_back(std::move(spec));
      }
      if (!extemp) {
        extemp = exspec->spec_temp.get();
      }

      extemp->name = basetemp->name + "<" + argk.toString() + ">";
      if (auto uc = make_unique<definition_class>(extemp->name, extemp,
                                                  DEF_CLASS | DEF_TYPENAME)) {
        tclass = uc.get();
        extemp->def = std::move(uc);
      }
      tclass->instance_of = basetemp;

      token = read_next_token(scope);
      goto regular_template_class;
    }

    herr->error(token) << "Expected class name here before " << token;
    return 1;
  }

  // ===========================================================================
  // =====: Handle template function definitions :==============================
  // ===========================================================================

  int funcflags = 0;
  if (token.gloss_type() == GTT_DECLARATOR) {
    full_type funcrefs = read_fulltype(token, temp.get());
    if (!funcrefs.def) {
      if (token.type == TT_OPERATORKW) {
        funcflags = funcrefs.flags;
        if (!funcrefs.refs.empty()) {
          herr->error(token)
              << "Internal error: references attatched to typeless expression";
          return 1;
        }
        goto template_cast_operator;
      }
      herr->error(token)
          << "Expected return type for template function at this point";
      return 1;
    }

    if (funcrefs.refs.empty() ||
        funcrefs.refs.top().type != ref_stack::RT_FUNCTION) {
      if (token.type == TT_DEFINITION && in_template(token.def)) {
        // We don't need to know the definition, just skip it.
        // TODO: Actually store this
        read_qualified_definition(token, scope);
        if (token.type == TT_EQUAL) { // We don't need to know the value, either; we just need to skip it.
          token = read_next_token(scope);
          AST a; astbuilder->parse_expression(&a, token, scope, precedence::comma); // Read and discard; kind of a hack, but it's safe.
        }
        if (token.type != TT_SEMICOLON) {
          if (token.type == TT_LEFTPARENTH) {
            // We're implementing a template function within a different class
            read_referencers_post(funcrefs.refs, token, temp.get());
            if (token.type == TT_LEFTBRACE) {
              // TODO: Is this actually happening, ever? Store this.
              delete_function_implementation(
                  handle_function_implementation(lex, token, temp.get(), herr));
            } else {
              herr->error(token) << "I have no idea where I am. :(";
            }
            return 0;
          }
          herr->error(token)
              << "Expected semicolon following template member definition "
              << "before " << token;
          FATAL_RETURN(1);
        }
        return 0;
      }
      if (funcrefs.refs.ndef && funcrefs.refs.ndef->flags & DEF_HYPOTHETICAL) {
        // TODO: Do some error checking here to make sure whatever's being
        // declared in that AST is a member of the template. Mark it non-extern.
      }
      else if (funcrefs.refs.ndef && (funcrefs.refs.ndef->flags & DEF_TYPED)) {
        definition_typed* dt = (definition_typed*)funcrefs.refs.ndef;
        if (dt->modifiers & builtin_flag__static->mask) {
          dt->flags &= ~DEF_INCOMPLETE; // TODO: Make structures OR static members by DEF_INCOMPLETE on creation; add error checking here
        } else {
          herr->error(token) << "Definition of non-static member "
                             << PQuote(dt->name);
        }
      } else {
        herr->error(token)
            << "Definition in template must be a function; "
            << PQuote(funcrefs.def->name + " " + funcrefs.refs.name)
            << " is not a function";
      }
      return 1;
    }

    string funcname = funcrefs.refs.name;
    if (funcname.empty()) {
      if ((funcrefs.refs.ndef && funcrefs.refs.ndef->name == constructor_name) || is_potential_constructor(scope, funcrefs)) {
        funcname = constructor_name;
        if (token.type == TT_COLON) {
          // TODO: Actually store this. And the other one. :P
          handle_constructor_initializers(lex, token, scope, herr);
        }
      }
      else if (!funcrefs.refs.ndef) {
        token.report_error(herr, "Template functions must have names");
        return 1;
      }
    }

    definition_function *func = nullptr;
    definition *maybe = funcrefs.refs.ndef;
    if (!maybe) maybe = scope->find_local(funcname);
    if (maybe) {
      if (!(maybe->flags & DEF_FUNCTION)) {
        herr->error(token) << "Cannot redeclare " << maybe->kind() << " "
                           << PQuote(funcname) << " as function in this scope";
        return 1;
      } else {
        func = (definition_function*)maybe;
      }
    }

    definition_overload *tovr;
    if (auto tu = make_unique<definition_overload>(funcname, scope,
            funcrefs.def, funcrefs.refs, funcrefs.flags, DEF_FUNCTION)) {
      tovr = tu.get();
      temp->def = std::move(tu);
    }
    if (!func) {
      auto nfun = make_unique<definition_function>(funcname, scope);
      func = nfun.get();
      scope->declare(funcname, std::move(nfun));
    }
    definition_scope *tscope = temp.get();
    func->overload(std::move(temp), herr->at(token));

    if (token.type == TT_COLON) {
      herr->error(token) << "Unexpected colon; " << PQuote(funcname)
                         << " is not a constructor";
      return 1;
    }

    if (token.type == TT_LEFTBRACE) {
      tovr->implementation = (handle_function_implementation(lex, token, tscope,
                                                             herr));
    } else if (token.type != TT_SEMICOLON) {
      herr->error(token)
          << "Expected template function body or semicolon before" << token;
      FATAL_RETURN(1);
    }
    return 0;
  }
  else if (token.type == TT_OPERATORKW) {
    template_cast_operator:
    full_type ft = read_operatorkw_cast_type(token, temp.get());
    auto ovr = make_unique<definition_overload>(
                   "(cast)", scope, ft.def, ft.refs, ft.flags,
                   DEF_FUNCTION | inherited_flags | funcflags);
    if (token.type == TT_LEFTBRACE)
      ovr->implementation = handle_function_implementation(lex, token, scope,
                                                           herr);
    else ovr->flags |= DEF_INCOMPLETE;
    temp->def = std::move(ovr);
    scope->overload_function("(cast)", std::move(temp),
                             inherited_flags | funcflags, herr->at(token));
    return 0;
  }
  else if (token.type == TT_TEMPLATE) {
    // FIXME: OH MY FUCKING WHAT
    // These templates are used when declaring functions which have templates as parameters,
    handle_template(temp.get(), token, inherited_flags);
    return 0;
  }
  else if (token.type == TT_FRIEND) {
    if (scope->flags & DEF_CLASS) {
      token = read_next_token(scope);
      if (token.type == TT_CLASS) {
        token = read_next_token(scope);
        read_qualified_definition(token, temp.get());
      }
      else {
        read_fulltype(token, temp.get());
      }
      if (token.type != TT_SEMICOLON) {
        herr->error(token)
            << "Expected semicolon to close friend statement before " << token;
      }
      return 0; // TODO: store this friend somewhere
    }
    else {
      herr->error(token) << "Unexpected `friend' statement: not in a class";
      return 1;
    }
  }
  else {
    herr->error(token) <<
      "Expected class or function declaration following template clause before "
      << token;
    return ERROR_CODE;
  }
  return 0;
}

int context_parser::handle_template_extern(
    definition_scope *scope, token_t &token, unsigned inherited_flags) {
  // TODO: idk, do something with this?
  herr->error(token) << "Unsupported: extern template";
  while (token.type != TT_ENDOFCODE and token.type != TT_SEMICOLON)
    token = read_next_token(scope);
  (void) inherited_flags;
  return 0;
}

}  // namespace jdi
