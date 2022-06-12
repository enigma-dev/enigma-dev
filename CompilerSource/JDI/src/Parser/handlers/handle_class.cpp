/**
 * @file handle_class.cpp
 * @brief Source implementing a function to handle class and structure declarations.
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

#include <cstdio>
#include <Parser/context_parser.h>
#include <API/compile_settings.h>

using namespace jdi;

#define def_kind class
#define DEF_FLAG DEF_CLASS
#include <Parser/cclass_base.h>
#include <System/builtins.h>

int jdi::context_parser::handle_class_inheritance(definition_scope *scope, token_t& token, definition_class *recipient, unsigned default_protection) {
  do {
    unsigned iprotection = default_protection; //, ivirtuality = 0; TODO: mark inheritance as virtual
    token = read_next_token(scope);
    if (token.type == TT_DECFLAG && ((typeflag*) token.def) == builtin_flag__virtual) {
      token = read_next_token(scope);
    }
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
      return 1;
    }
    full_type ft = read_type(token, scope);
    if (!ft.def) {
      token.report_errorf(herr, "Expected class name to inherit before %s");
      return 1;
    }
    ft.reduce();
    if (ft.refs.size())
      token.report_warning(herr, "Extra qualifications in `" + ft.toString() + "' for ancestor class discarded");
    if (not(ft.def->flags & DEF_CLASS)) {
      if (ft.def->flags & DEF_TEMPPARAM) {
        definition_tempparam *tp = (definition_tempparam*)ft.def;
        tp->must_be_class = true;
      }
      else if (ft.def->flags & DEF_HYPOTHETICAL) {
        definition_hypothetical *tp = (definition_hypothetical*)ft.def;
        tp->required_flags |= DEF_CLASS;
      }
      else {
        token.report_errorf(herr, "Expected class name to inherit before %s");
        return 1;
      }
    }
    recipient->ancestors.push_back(definition_class::ancestor(iprotection, (definition_class*)ft.def));
  }
  while (token.type == TT_COMMA);

  return 0;
}

jdi::definition_class* jdi::context_parser::handle_class(definition_scope *scope, token_t& token, int inherited_flags)
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
  definition_class *nclass = nullptr; // The class definition. Will be null until allocation, unless the class was already declared.
  bool already_complete = false; // True if nclass is non-null, and the class is already implemented.
  bool will_redeclare = false; // True if this class is from another scope; so true if implementing this class will allocate it.
  unsigned incomplete = DEF_INCOMPLETE; // DEF_INCOMPLETE if this class has a body, zero otherwise.

  // TRUTH TABLE
  // nclass    already_complete   will_redeclare  Meaning
  // nullptr   *                  *               Nothing declared. Allocate new ASAP.
  // Non-null  False              False           Incomplete class in this scope; may be implemented.
  // Non-null  False              True            Incomplete class in another scope; can be implemented new in this scope.
  // Non-null  True               False           Complete class in this scope. MUST be used as a type, not implemented.
  // Non-null  True               True            Complete class in another scope; can be redeclared (reallocated and reimplemented) in this scope.

  if (get_location(nclass, will_redeclare, already_complete, token, classname, scope, this, herr))
    return nullptr;

  if (!nclass)
    if (not(nclass = insnew(scope,inherited_flags,classname,token,herr)))
      return nullptr;

  // Handle inheritance
  if (token.type == TT_COLON) {
    if (will_redeclare) {
      will_redeclare = false;
      if (not(nclass = insnew(scope,inherited_flags,classname,token,herr)))
        return nullptr;
    }
    else if (already_complete) {
      token.report_error(herr, "Attempting to add ancestors to previously defined class `" + classname + "'");
    }
    incomplete = 0;

    if (handle_class_inheritance(scope, token, nclass, protection))
      return nullptr;
  }

  // Handle implementation
  if (token.type == TT_LEFTBRACE)
  {
    incomplete = 0;
    if (will_redeclare) {
      will_redeclare = false;
      if (not(nclass = insnew(scope,inherited_flags,classname,token,herr)))
        return nullptr;
    }
    else if (already_complete) {
      token.report_error(herr, "Attempting to add members to previously defined class `" + classname + "'");
      FATAL_RETURN(nullptr);
    }
    nclass->flags &= ~DEF_INCOMPLETE;
    if (handle_scope(nclass, token, protection))
      FATAL_RETURN(nullptr);
    if (token.type != TT_RIGHTBRACE) {
      token.report_error(herr, "Expected closing brace to class `" + classname + "'");
      FATAL_RETURN(nullptr);
    }
    token = read_next_token(scope);
  }
  else // Sometimes, it isn't okay to not specify a structure body.
    if (!incomplete) { // The only way incomplete is zero in this instance is if it was set in : handler.
      token.report_errorf(herr, "Expected class body here (before %s) after parents named");
      FATAL_RETURN(nullptr);
    }

  nclass->flags |= incomplete;
  return nclass;
}

