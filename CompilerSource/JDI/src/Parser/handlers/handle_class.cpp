/**
 * @file handle_class.cpp
 * @brief Source implementing a function to handle class and structure declarations.
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

#include <cstdio>
#include <Parser/bodies.h>
#include <API/compile_settings.h>


using namespace jdip;
#define alloc_class() new definition_class(classname,scope, DEF_CLASS | DEF_TYPENAME | inherited_flags)
static inline definition_class* insnew(definition_scope *const &scope, int inherited_flags, const string& classname, const token_t &token, error_handler* const& herr, context *ct) {
  definition_class* nclass = NULL;
  decpair dins = scope->declare(classname);
  if (!dins.inserted) {
    if (dins.def->flags & DEF_TYPENAME) { // This error is displayed because if the class existed earlier when we were checking, we'd have gotten a different token.
      token.report_error(herr, "Class `" + classname + "' instantiated inadvertently during parse by another thread. Freeing.");
      delete ~dins.def;
    }
    else {
      dins = ct->declare_c_struct(classname);
      if (dins.inserted)
        goto my_else;
      if (dins.def->flags & DEF_CLASS)
        nclass = (definition_class*)dins.def;
      else {
        #if FATAL_ERRORS
          return NULL;
        #else
          delete ~dins.def;
          goto my_else;
        #endif
      }
    }
  } else { my_else:
    dins.def = nclass = alloc_class();
  }
  return nclass;
}

static unsigned anon_count = 1;
jdi::definition_class* jdip::context_parser::handle_class(definition_scope *scope, token_t& token, int inherited_flags)
{
  unsigned protection = 0;
  if (token.type == TT_CLASS)
     protection = DEF_PRIVATE;
  #ifdef DEBUG_MODE
  else if (token.type != TT_STRUCT)
    token.report_error(herr, "PARSE ERROR: handle_class invoked with non-class, non-struct token.");
  #endif
  
  token = read_next_token(scope);
  
  string classname;
  definition_class *nclass = NULL; // The class definition. Will be NULL until allocation, unless the class was already declared.
  bool already_complete = false; // True if nclass is non-NULL, and the class is already implemented.
  bool will_redeclare = false; // True if this class is from another scope; so true if implementing this class will allocate it.
  unsigned incomplete = DEF_INCOMPLETE; // DEF_INCOMPLETE if this class has a body, zero otherwise.
  
  // TRUTH TABLE
  // nclass    already_complete   will_redeclare  Meaning
  // NULL      *                  *               Nothing declared. Allocate new ASAP.
  // Non-NULL  False              False           Incomplete class in this scope; may be implemented.
  // Non-NULL  False              True            Incomplete class in another scope; can be implemented new in this scope.
  // Non-NULL  True               False           Complete class in this scope. MUST be used as a type, not implemented.
  // Non-NULL  True               True            Complete class in another scope; can be redeclared (reallocated and reimplemented) in this scope.
  
  definition *dulldef = NULL;
  if (token.type == TT_IDENTIFIER) {
    classname = token.content.toString();
    token = read_next_token(scope);
  }
  else if (token.type == TT_DEFINITION) {
    classname = token.content.toString();
    dulldef = token.def;
    token = read_next_token(scope);
  }
  else if (token.type == TT_DECLARATOR) {
    nclass = (jdi::definition_class*)token.def;
    classname = nclass->name;
    if (not(nclass->flags & DEF_CLASS)) {
      if (nclass->parent == scope) {
        pair<definition_scope::defiter, bool> dins = c_structs.insert(pair<string,definition*>(classname, NULL));
        if (dins.second)
          dins.first->second = nclass = alloc_class();
        else {
          if (dins.first->second->flags & DEF_CLASS)
            nclass = (definition_class*)dins.first->second;
          else {
            token.report_error(herr, "Attempt to redeclare `" + classname + "' as class in this scope");
            FATAL_RETURN(NULL);
          }
        }
      }
      else
        nclass = NULL;
    }
    else {
      will_redeclare = nclass->parent != scope;
      already_complete = not(nclass->flags & DEF_INCOMPLETE);
    }
    token = read_next_token(scope);
  }
  else {
    char buf[32];
    sprintf(buf, "<anonymousClass%08d>", anon_count++);
    classname = buf; // I love std::string. Even if I'm lazy for it.
  }
  
  // Handle template access and specialization **before** we go allocating classes.
  if (token.type == TT_LESSTHAN)
  {
    // We'd better have read a definition earlier, and it'd better have been a template.
    if (not(dulldef and (dulldef->flags & DEF_TEMPLATE))) {
      token.report_error(herr, "Unexpected '<' token; `" + classname + "' is not a template type");
      //cerr << dulldef << ": " << (dulldef? dulldef->name : "no definition by that name") << endl;
      return NULL;
    }
    // Now, we might be specializing the template, or we might just be instantiating it.
    // If we are in a template<> statement, we have to be specializing. Otherwise, we're not.
    if (scope->flags & DEF_TEMPSCOPE) {
      definition_template *temp = (definition_template*)dulldef;
      definition_tempscope *ts = (definition_tempscope*)scope;
      arg_key k(temp->params.size());
      if (read_template_parameters(k, temp, lex, token, scope, this, herr))
        return NULL;
      if (token.type != TT_GREATERTHAN) {
        token.report_errorf(herr, "Expected closing triangle bracket here before %s");
        FATAL_RETURN(NULL);
      }
      definition_template *spec = temp->specialize(k, ts);
      if (spec->def) {
        if (not(spec->def->flags & DEF_CLASS)) {
          token.report_error(herr, "Template `" + temp->name + "' does not name a class");
          return NULL;
        }
        nclass = (definition_class*)spec->def;
        nclass->parent = ts;
        already_complete = not(nclass->flags & DEF_INCOMPLETE);
      }
      else {
        spec->def = nclass = new definition_class(temp->name, ts, temp->def->flags);
        already_complete = false;
      }
      will_redeclare = false;
      token = read_next_token(scope);
    }
  }
  else if (dulldef)
  {
    definition_scope *nts = scope; // Find a non-tempscope scope
    while (nts and nts->flags & DEF_TEMPSCOPE)
      nts = nts->parent;
    if (dulldef->parent == nts)
    {
      if (dulldef->flags & DEF_TEMPLATE)
      {
        definition_template* temp = (definition_template*)dulldef; // temp = existing template
        if (temp->def->flags & DEF_CLASS) { // If the existing template is indeed a class
          nclass = (definition_class*)temp->def; // Set our class, nclass, to the existing class
          if (scope->flags & DEF_TEMPSCOPE) { // If that scope is a tempscope,
            ((definition_tempscope*)scope)->referenced = true; // Denote that we've referenced it,
            
            if (temp->params.size() != scope->using_general.size()) { // Make sure our parameter counts are the same
              token.report_error(herr, "Template parameter mismatch in class definition");
              scope->using_general.clear();
            }
            else {
              scope->using_general.clear(); // Drop anything we might still be referring to before we toast this bitch
              definition_template *const ntemp = (definition_template*)((definition_tempscope*)scope)->source;
              for (size_t i = 0; i < temp->params.size(); ++i) // Replace what we've dropped with the old, likely-to-be-referenced stuff
                scope->use_general(ntemp->params[i]->name, temp->params[i]);
            }
            
            nclass->parent = scope; // Make sure it can actually use this scope's using scope
            delete ((definition_tempscope*)scope)->source; // But instead of actually referencing it, just delete it
            ((definition_tempscope*)scope)->source = temp; // Replace it with our existing one
            // Be advised that this entire process is to prevent leaks while avoiding the possibility
            // of deleting a class that is already pointed to by other classes; this is why we delete
            // our new class rather than merging the old one into it and deleting the old.
          }
        }
        else {
          token.report_error(herr, "Cannot redeclare template `" + dulldef->name + "' as class in this scope");
          return NULL;
        }
      }
      else {
        token.report_error(herr, "Cannot redeclare `" + dulldef->name + "' as class in this scope");
        return NULL;
      }
    }
  }
  
  if (!nclass)
    if (not(nclass = insnew(scope,inherited_flags,classname,token,herr,this)))
      return NULL;
  
  // Handle inheritance
  if (token.type == TT_COLON) {
    if (will_redeclare) {
      will_redeclare = false;
      if (not(nclass = insnew(scope,inherited_flags,classname,token,herr,this)))
        return NULL;
    }
    else if (already_complete) {
      token.report_error(herr, "Attempting to add ancestors to previously defined class `" + classname + "'");
    }
    incomplete = 0;
    do {
      unsigned iprotection = protection;
      token = read_next_token(scope);
      if (token.type == TT_PUBLIC)
        iprotection = 0,
        token = read_next_token(scope);
      else if (token.type == TT_PRIVATE)
        iprotection = DEF_PRIVATE,
        token = read_next_token(scope);
      else if (token.type == TT_PROTECTED)
        iprotection = DEF_PRIVATE,
        token = read_next_token(scope);
      if (token.type != TT_DECLARATOR and token.type != TT_DEFINITION) {
        string err = "Ancestor class name expected";
        if (token.type == TT_DECLARATOR) err += "; `" + token.def->name + "' does not name a class";
        if (token.type == TT_IDENTIFIER) err += "; `" + token.content.toString() + "' does not name a type";
        token.report_error(herr, err);
        return NULL;
      }
      full_type ft = read_type(lex, token, scope, this, herr);
      if (!ft.def) {
        token.report_errorf(herr, "Expected class name before %s");
        return NULL;
      }
      if (not(ft.def->flags & DEF_CLASS)) {
        if (ft.def->flags & DEF_TYPENAME) {
          definition* def = scope;
          while (def and ~def->flags & DEF_TEMPSCOPE) def = def->parent;
          if (!def) {
            token.report_error(herr, "Invalid use of `typename'");
            return NULL;
          }
          else {
            definition_hypothetical *h = new definition_hypothetical(ft.def->name, ft.def->parent, ft.def->flags, new AST(ft.def));
            ((definition_template*)((definition_tempscope*)def)->source)->dependents.push_back(h);
            ft.def = h;
          }
        }
        else {
          token.report_errorf(herr, "Expected class name before %s");
          return NULL;
        }
      }
      else {
        if (ft.flags or ft.refs.size())
          token.report_error(herr, "Extra qualifiers to inherited class ignored");
      }
      nclass->ancestors.push_back(definition_class::ancestor(iprotection, (definition_class*)ft.def));
    }
    while (token.type == TT_COMMA);
  }
  
  // Handle implementation
  if (token.type == TT_LEFTBRACE)
  {
    incomplete = 0;
    if (will_redeclare) {
      will_redeclare = false;
      if (not(nclass = insnew(scope,inherited_flags,classname,token,herr,this)))
        return NULL;
    }
    else if (already_complete) {
      token.report_error(herr, "Attempting to add members to previously defined class `" + classname + "'");
      FATAL_RETURN(NULL);
    }
    nclass->flags &= ~DEF_INCOMPLETE;
    if (handle_scope(nclass, token, protection))
      FATAL_RETURN(NULL);
    if (token.type != TT_RIGHTBRACE) {
      token.report_error(herr, "Expected closing brace to class `" + classname + "'");
      FATAL_RETURN(NULL);
    }
    token = read_next_token(scope);
  }
  else // Sometimes, it isn't okay to not specify a structure body.
    if (!incomplete) { // The only way incomplete is zero in this instance is if it was set in : handler.
      token.report_errorf(herr, "Expected class body here (before %s) after parents named");
      FATAL_RETURN(NULL);
    }
  
  nclass->flags |= incomplete;
  return nclass;
}

