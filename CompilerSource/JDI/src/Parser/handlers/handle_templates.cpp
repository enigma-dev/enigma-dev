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

using namespace jdip;
using namespace jdi;

#if FATAL_ERRORS
#define ERROR_CODE 1
#else
#define ERROR_CODE 0
#endif

static inline bool in_template(definition *d) {
  for (definition* s = d; s; s = s->parent)
    if (s->flags & DEF_TEMPLATE)
      return true;
  return false;
}

int context_parser::handle_template(definition_scope *scope, token_t& token, unsigned inherited_flags)
{
  token = read_next_token(scope);
  if (token.type != TT_LESSTHAN) {
    token.report_error(herr, "Expected opening triangle bracket following `template' token");
    return ERROR_CODE;
  }
  token = read_next_token(scope);
  
  definition_template* temp = new definition_template("", scope, DEF_TEMPLATE | inherited_flags);
  
  for (;;) {
    string pname; // The name given to this parameter
    unsigned dtpflags = DEF_TEMPPARAM | DEF_DEPENDENT;
    
    definition_tempparam* dtn;
    if (token.type == TT_TYPENAME || token.type == TT_CLASS || token.type == TT_STRUCT) {
      token = lex->get_token(herr);
      if (token.type == TT_IDENTIFIER) {
        pname = token.content.toString();
        token = read_next_token(temp);
      }
      dtpflags |= DEF_TYPENAME;
    }
    else if (token.type == TT_DECFLAG || token.type == TT_DECLARATOR || token.type == TT_DECLTYPE) {
      full_type fts = read_fulltype(token, temp);
      pname = fts.refs.name;
    }
    else {
      if (token.type == TT_GREATERTHAN) break;
      token.report_errorf(herr, "Expected '>' token here before %s");
      FATAL_RETURN((delete temp, 1));
      break;
    }
    
    AST *ast = NULL;
    if (token.type == TT_OPERATOR) {
      if (token.content.len != 1 or *token.content.str != '=') {
        token.report_error(herr, "Unexpected operator here; value must be denoted by '='");
        FATAL_RETURN((void(delete temp),1));
      }
      token = read_next_token(temp);
      ast = new AST();
      ast->set_use_for_templates(true);
      astbuilder->parse_expression(ast, token, temp, precedence::comma+1);
    }
    dtn = new definition_tempparam(pname, temp, ast, dtpflags);
    
    temp->params.push_back(dtn);
    if (pname.empty()) {
      char nname[32];
      sprintf(nname, "<templateParam%03u>", (unsigned)temp->params.size());
    }
    else
      temp->use_general(pname, dtn);
    if (token.type == TT_GREATERTHAN)
      break;
    if (token.type != TT_COMMA)
      token.report_errorf(herr, "Expected '>' or ',' before %s");
    token = read_next_token(temp);
  }
  
  token = read_next_token(temp);
  
  // ========================================================================================================================================
  // =====: Handle template class definitions :==============================================================================================
  // ========================================================================================================================================
  
  if (token.type == TT_CLASS || token.type == TT_STRUCT)
  {
    unsigned protection = token.type == TT_CLASS? DEF_PRIVATE : 0;
    token = read_next_token(scope);
    definition_class *tclass;
    
    if (token.type == TT_IDENTIFIER) {
      regular_identifier:
      temp->name = token.content.toString();
      tclass = new definition_class(temp->name, temp, DEF_CLASS | DEF_TYPENAME);
      temp->def = tclass;
    
      scope->declare(temp->name, temp);
      
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
        token.report_errorf(herr, "Opening brace for class body expected before %s");
        return 1;
      }
      
      if (handle_scope(tclass, token, protection))
        FATAL_RETURN(1);
      
      if (token.type != TT_RIGHTBRACE) {
        token.report_errorf(herr, "Expected closing brace to class body before %s");
        FATAL_RETURN(1);
      }
      
      return 0;
    }
    else if (token.type == TT_DEFINITION) {
      if (token.def->parent != scope)
        goto regular_identifier;
      
      if (not((token.def->flags & DEF_TEMPLATE) && (((definition_template*)token.def)->def->flags & DEF_CLASS))) {
        token.report_error(herr, "Expected class name for specialization; `" + token.def->name + "' does not name a template class");
        delete temp;
        return 1;
      }
      
      definition_template *basetemp = (definition_template*)token.def;
      
      token = read_next_token(temp);
      
      if (token.type != TT_LESSTHAN) {
        if (basetemp->def && (basetemp->def->flags & (DEF_CLASS | DEF_INCOMPLETE)) == (DEF_CLASS | DEF_INCOMPLETE)) {
          if (basetemp->params.size() != temp->params.size()) {
            token.report_error(herr, "Argument count differs from forward declaration");
            delete temp;
            return 1;
          }
          for (size_t i = 0; i < basetemp->params.size(); ++i) {
            if (basetemp->params[i]->flags != temp->params[i]->flags) {
              token.report_error(herr, "Parameter " + value((long)i).toString() + " of declaration differs from forward");
              delete temp;
              return 1;
            }
            if (!temp->params[i]->name.empty() && basetemp->params[i]->name != temp->params[i]->name) {
              definition_scope::defiter it = basetemp->using_general.find(basetemp->params[i]->name);
              if (it != basetemp->using_general.end())
                if (it->second == basetemp->params[i])
                  basetemp->using_general.erase(it);
                else token.report_warning(herr, "Template parameter `" + basetemp->params[i]->name
                                          + "' in forward declaration's using is not mapped to `" + temp->params[i]->name + "'");
              else if (!basetemp->params[i]->name.empty())
                token.report_warning(herr, "Template parameter `" + basetemp->params[i]->name
                                     + "' not found in forward declaration's using; cannot be renamed to `" + temp->params[i]->name + "'");
              
              basetemp->params[i]->name = temp->params[i]->name;
              basetemp->use_general(basetemp->params[i]->name, basetemp->params[i]);
            }
          }
          delete temp;
          temp = basetemp;
          tclass = (definition_class*)temp->def;
          goto regular_template_class;
        }
        token.report_errorf(herr, "Expected opening triangle bracket for template definition before %s");
        delete temp;
        return 1;
      }
      
      arg_key argk(basetemp->params.size());
      definition_template::specialization *spec = new definition_template::specialization(basetemp->params.size(), temp->params.size(), temp);
      argk.mirror_types(basetemp);
      size_t args_given = 0;
      for (;;++args_given)
      {
        token = read_next_token(temp);
        if (token.type == TT_GREATERTHAN)
          break;
        if (token.type == TT_SEMICOLON || token.type == TT_LEFTBRACE) {
          token.report_errorf(herr, "Expected closing triangle bracket to template parameters before %s");
          break;
        }
        
        if (token.type == TT_COMMA) continue;
        
        if (args_given >= basetemp->params.size()) {
          token.report_error(herr, "Too many parameters for template `" + basetemp->name + "'");
          delete temp; return 1;
        }
        
        if ((argk[args_given].type == arg_key::AKT_VALUE)
        and (token.type == TT_DEFINITION or token.type == TT_DECLARATOR) and (token.def->flags & DEF_TEMPPARAM))
        {
          for (size_t i = 0; i < temp->params.size(); ++i) if (temp->params[i] == token.def) {
            if (~temp->params[i]->flags & DEF_TYPENAME) {
              spec->key.arg_inds[i][++spec->key.arg_inds[i][0]] = args_given;
              argk[args_given].val() = VT_DEPENDENT;
              token = read_next_token(temp);
              goto handled_argk; // break 2;
            }
            else
              token.report_error(herr, "Type mismatch in passing parameter " + parse_bacics::visible::toString(args_given) + " to template specialization: real-valued parameter expected");
          }
        }
        
        if (read_template_parameter(argk, args_given, basetemp, token, temp)) {
          delete temp; // XXX: is this needed?
          return 1;
        }
        
        if (argk[args_given].type == arg_key::AKT_FULLTYPE) {
          definition *def = argk[args_given].ft().def;
          for (size_t i = 0; i < temp->params.size(); ++i) if (temp->params[i] == def) {
            spec->key.arg_inds[i][++spec->key.arg_inds[i][0]] = args_given;
            argk[args_given].ft().def = arg_key::abstract;
          }
        }
        
        handled_argk:
        
        if (token.type == TT_GREATERTHAN) {
          ++args_given;
          break;
        }
        if (token.type != TT_COMMA) {
          token.report_errorf(herr, "Comma or closing triangle bracket expected here before %s");
          break;
        }
      }
      if (check_read_template_parameters(argk, args_given, basetemp, token, herr)) {
        delete temp; // XXX: Needed?
        return 1;
      }
      
      // cout << "Specialization key: " << argk.toString() << endl;
      spec->filter = argk;
      definition_template::speclist &slist = basetemp->specializations;
      
      for (definition_template::speclist::iterator it = slist.begin(); it != slist.end(); ++it)
        if ((*it)->key.same_as(spec->key))
        {
          delete spec;
          spec = *it;
          
          if (~spec->spec_temp->flags & DEF_INCOMPLETE) {
            token.report_error(herr, "Cannot specialize template: specialization by this type already exists.");
            delete temp;
            return 1;
          }
          
          spec->spec_temp->using_general.clear(); // XXX: Maybe replace this with something to specifically delete template parameters, just in case? Could anything else be used? I'm thinking not.
          for (definition_template::piterator pit = temp->params.begin(); pit != temp->params.end(); ++pit)
            spec->spec_temp->use_general((*pit)->name, *pit);
          
          delete temp;
          temp = spec->spec_temp;
          break;
        }
      slist.push_back(spec);
      
      temp->name = basetemp->name + "<" + argk.toString() + ">";
      tclass = new definition_class(temp->name, temp, DEF_CLASS | DEF_TYPENAME);
      temp->def = tclass;
      tclass->instance_of = basetemp;
      
      token = read_next_token(scope);
      goto regular_template_class;
    }
    
    token.report_errorf(herr, "Expected class name here before %s");
    delete temp;
    return 1;
  }
  
  // ========================================================================================================================================
  // =====: Handle template function definitions :===========================================================================================
  // ========================================================================================================================================
  
  int funcflags = 0;
  if (token.type == TT_DECLARATOR || token.type == TT_DECFLAG || token.type == TT_DECLTYPE || token.type == TT_DEFINITION || token.type == TT_TYPENAME)
  {
    full_type funcrefs = read_fulltype(token, temp);
    if (!funcrefs.def) {
      if (token.type == TT_OPERATORKW) {
        funcflags = funcrefs.flags;
        if (!funcrefs.refs.empty()) {
          token.report_error(herr, "Program error: references attatched to typeless expression");
          return 1;
        }
        goto template_cast_operator;
      }
      token.report_error(herr, "Expected return type for template function at this point");
      delete temp;
      return 1;
    }
    
    if (funcrefs.refs.empty() || funcrefs.refs.top().type != ref_stack::RT_FUNCTION) {
      if (token.type == TT_DEFINITION && in_template(token.def)) {
        read_qualified_definition(token, scope); // We don't need to know the definition, just skip it. If we were a compiler, we'd need to know this. :P
        if (token.type == TT_OPERATOR && token.content.len == 1 && *token.content.str == '=') { // We don't need to know the value, either; we just need to skip it.
          token = read_next_token(scope);
          AST a; astbuilder->parse_expression(&a, token, scope, precedence::comma); // Read and discard; kind of a hack, but it's safe.
        }
        if (token.type != TT_SEMICOLON) {
          if (token.type == TT_LEFTPARENTH) // We're implementing a template function within a different class
          {
            read_referencers_post(funcrefs.refs, token, temp);
            if (token.type == TT_LEFTBRACE)
              delete_function_implementation(handle_function_implementation(lex, token, temp, herr));
            else
              token.report_error(herr, "I have no idea where I am.");
            delete temp;
            return 0;
          }
          token.report_errorf(herr, "Expected semicolon following template member definition before %s");
          FATAL_RETURN((void(delete temp), 1));
        }
        delete temp; // We're done with temp.
        return 0;
      }
      if (funcrefs.refs.ndef && funcrefs.refs.ndef->flags & DEF_HYPOTHETICAL) {
        // TODO: Do some error checking here to make sure whatever's being declared in that AST is a member of the template. Mark it non-extern.
      }
      else if (funcrefs.refs.ndef && (funcrefs.refs.ndef->flags & DEF_TYPED)) {
          definition_typed* dt = (definition_typed*)funcrefs.refs.ndef;
        if (dt->modifiers & builtin_flag__static)
          dt->flags &= ~DEF_INCOMPLETE; // TODO: Make structures OR static members by DEF_INCOMPLETE on creation; add error checking here
        else
          token.report_error(herr, "Definition of non-static member `" + dt->name + "'");
      }
      else
        token.report_errorf(herr, "Definition in template must be a function; `" + funcrefs.def->name + " " + funcrefs.refs.name + "' is not a function (at %s)");
      delete temp;
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
        delete temp;
        return 1;
      }
    }
    
    definition_function *func = NULL;
    definition *maybe = funcrefs.refs.ndef;
    if (!maybe)
      maybe = scope->find_local(funcname);
    if (maybe)
      if (!(maybe->flags & DEF_FUNCTION)) {
        token.report_error(herr, "Cannot redeclare " + maybe->kind() + " `" + funcname + "' as function in this scope");
        delete temp;
        return 1;
      }
      else func = (definition_function*)maybe;
    else {}
    
    definition_overload *tovr = new definition_overload(funcname, scope, funcrefs.def, funcrefs.refs, funcrefs.flags, DEF_FUNCTION);
    temp->def = tovr;
    
    if (!func)
      scope->declare(funcname, func = new definition_function(funcname, scope));
    func->overload(temp, herr);
    
    if (token.type == TT_COLON) {
      token.report_error(herr, "Unexpected colon; `" + funcname + "' is not a constructor");
      return 1;
    }
    
    if (token.type == TT_LEFTBRACE)
      tovr->implementation = (handle_function_implementation(lex, token, temp, herr));
    else if (token.type != TT_SEMICOLON) {
      token.report_errorf(herr, "Expected template function body or semicolon before %s");
      FATAL_RETURN(1);
    }
  }
  else if (token.type == TT_OPERATORKW) {
    template_cast_operator:
    full_type ft = read_operatorkw_cast_type(token, temp);
    definition_overload *ovr = new definition_overload("(cast)", scope, ft.def, ft.refs, ft.flags, DEF_FUNCTION | inherited_flags | funcflags);
    if (!ovr)
      return 1;
    if (token.type == TT_LEFTBRACE)
      ovr->implementation = handle_function_implementation(lex, token, scope, herr);
    else ovr->flags |= DEF_INCOMPLETE;
    temp->def = ovr;
    scope->overload_function("(cast)", temp, inherited_flags | funcflags, token, herr);
    return 0;
  }
  else if (token.type == TT_TEMPLATE) {
    // FIXME: OH MY FUCKING WHAT
    // These templates are used when declaring functions which have templates as parameters,
    handle_template(temp, token, inherited_flags);
    delete temp;
    return 0;
  }
  else if (token.type == TT_FRIEND) {
    if (scope->flags & DEF_CLASS) {
      token = read_next_token(scope);
      if (token.type == TT_CLASS) {
        read_qualified_definition(token, temp);
        delete temp;
        if (token.type != TT_SEMICOLON)
          token.report_errorf(herr, "Expected semicolon to close friend statement before %s");
        return 0; // TODO: store this friend somewhere
      }
      else {
        read_fulltype(token, temp);
        delete temp;
        if (token.type != TT_SEMICOLON)
          token.report_errorf(herr, "Expected semicolon to close friend statement before %s");
        return 0; // TODO: store this friend somewhere
      }
      delete temp;
    }
    else {
      token.report_error(herr, "Unexpected `friend' statement: not in a class");
      delete temp;
      return 1;
    }
  }
  else {
    token.report_errorf(herr, "Expected class or function declaration following template clause before %s");
    delete temp;
    return ERROR_CODE;
  }
  
  
  return 0;
}

int jdip::context_parser::handle_template_extern(definition_scope *scope, token_t &token, unsigned inherited_flags) {
  while (token.type != TT_ENDOFCODE and token.type != TT_SEMICOLON)
    token = read_next_token(scope);
  (void)inherited_flags;
  return 0;
}
