/**
 * @file jdi_utility.cpp
 * @brief File implementing utility functions using clang instead of JDI
 * 
 * This file provides clang-based implementations of function analysis utilities.
 * 
 * @section License
 * Copyright (C) 2011-2012 Josh Ventura
 * This file is part of ENIGMA.
 * 
 * ENIGMA is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
**/

#include "languages/clang_definitions.h"
#include "languages/lang_CPP.h"
#include <clang-c/Index.h>

using namespace jdi;
using namespace clang_adapter;

// Helper to check if a parameter is varargs type
static bool is_varargs_type(ClangDefinition* param, jdi::definition* varargs_t) {
  if (!param || !varargs_t) return false;
  // Check if parameter name matches varargs
  if (param->name == "varargs") return true;
  // Check if it's a typed definition with the varargs type
  ClangDefinitionTyped* typed = dynamic_cast<ClangDefinitionTyped*>(param);
  if (typed && typed->type == varargs_t) return true;
  return false;
}

bool lang_CPP::is_variadic_function(jdi::definition *d) const {
  if (!definition_is_function(d)) return false;
  return function_variadic_after((jdi::definition_function*) d) != -1;
}

int lang_CPP::function_variadic_after(jdi::definition_function *func) const {
  ClangDefinitionFunction* clang_func = static_cast<ClangDefinitionFunction*>(func);
  
  for (const auto &overload_pair : clang_func->overloads) {
    ClangDefinitionOverload *ov = static_cast<ClangDefinitionOverload*>(overload_pair.second.get());
    if (ov->is_variadic) {
      // Find the position of varargs parameter
      for (size_t i = 0; i < ov->params.size(); ++i) {
        if (is_varargs_type(ov->params[i], enigma_type__varargs)) {
          return i;
        }
      }
      // If variadic but no explicit varargs type, return last param index
      return ov->params.size() > 0 ? ov->params.size() - 1 : 0;
    }
  }
  return -1;
}

void lang_CPP::definition_parameter_bounds(definition *d, unsigned &min, unsigned &max) const {
  min = (unsigned) SIZE_MAX;
  max = 0;
  
  if (!(d->flags & DEF_FUNCTION)) {
    cout << "Attempt to use " << d->name << " as function" << endl;
    return;
  }
  
  ClangDefinitionFunction* func = static_cast<ClangDefinitionFunction*>(d);
  
  for (const auto &overload_pair : func->overloads) {
    ClangDefinitionOverload *ov = static_cast<ClangDefinitionOverload*>(overload_pair.second.get());
    unsigned local_min = 0;
    unsigned local_max = ov->params.size();
    
    if (ov->is_variadic) {
      max = (unsigned)-1;  // Variadic means unlimited
    } else {
      if (min > local_min) min = local_min;
      if (max < local_max) max = local_max;
    }
  }
  
  for (const auto &template_overload : func->template_overloads) {
    ClangDefinitionOverload *ov = static_cast<ClangDefinitionOverload*>(template_overload.get());
    unsigned local_min = 0;
    unsigned local_max = ov->params.size();
    
    if (ov->is_variadic) {
      max = (unsigned)-1;
    } else {
      if (min > local_min) min = local_min;
      if (max < local_max) max = local_max;
    }
  }
}

bool lang_CPP::definition_is_function(definition *d) const {
  if (d->flags & DEF_FUNCTION) return true;
  if (d->flags & DEF_TEMPLATE) {
    // For templates, we'd need to check the template definition
    // For now, just check if it has function-like characteristics
    return false;  // Simplified
  }
  return false;
}

size_t lang_CPP::definition_overload_count(jdi::definition *d) const {
  if (!(d->flags & DEF_FUNCTION)) return 0;
  ClangDefinitionFunction *df = static_cast<ClangDefinitionFunction*>(d);
  return df->overloads.size() + df->template_overloads.size();
}


#include "languages/lang_CPP.h"
jdi::definition* lang_CPP::find_typename(std::string_view name) const {
  jdi::definition* d = look_up(name);
  if (!d) return NULL;
  if (d->flags & DEF_TYPENAME) return d;
  return NULL;
}

bool lang_CPP::global_exists(string n) const {
  jdi::definition* d = look_up(n);
  return d;
}

void lang_CPP::quickmember_variable(jdi::definition_scope* scope, jdi::definition* type, string name) {
  ClangDefinitionScope* clang_scope = static_cast<ClangDefinitionScope*>(scope);
  auto def = std::make_unique<ClangDefinitionTyped>(
    name, clang_scope, jdi::DEF_TYPED, clang_getNullCursor(), 
    static_cast<ClangDefinition*>(type));
  clang_scope->members[name] = std::move(def);
}

enigma::parsing::StdErrorHandler hackybaby;  // TODO: FIXME: This should be using a central error handler...
void lang_CPP::quickmember_script(jdi::definition_scope* scope, string name) {
  ClangDefinitionScope* clang_scope = static_cast<ClangDefinitionScope*>(scope);
  auto def = std::make_unique<ClangDefinitionFunction>(
    name, clang_scope, jdi::DEF_FUNCTION, clang_getNullCursor());
  
  // Create a default overload with 16 variant parameters
  auto overload = std::make_unique<ClangDefinitionOverload>(
    name, clang_scope, jdi::DEF_FUNCTION | jdi::DEF_OVERLOAD, clang_getNullCursor());
  
  // Add 16 variant parameters
  for (int i = 0; i < 16; ++i) {
    std::string param_name = "argument" + std::to_string(i);
    auto param = std::make_unique<ClangDefinitionTyped>(
      param_name, clang_scope, jdi::DEF_TYPED, clang_getNullCursor(), 
      static_cast<ClangDefinition*>(enigma_type__variant));
    // Store param pointer before moving
    ClangDefinition* param_ptr = param.get();
    clang_scope->members[param_name] = std::move(param);
    overload->params.push_back(param_ptr);
  }
  
  def->overloads[name] = std::move(overload);
  clang_scope->members[name] = std::move(def);
}
