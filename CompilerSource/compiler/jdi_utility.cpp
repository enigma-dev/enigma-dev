/**
 * @file jdi_utility.cpp
 * @brief File implementing utility functions to pluck extra information from
 *        JDI constructs.
 * 
 * This file was written as a helper set in plugging JDI into the old parser.
 * 
 * @section License
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

// JDI removed - using clang_adapter instead
// #include <Storage/definition.h>
#include "languages/clang_definitions.h"  // Provides jdi:: typedefs
#include <languages/lang_CPP.h>
#include <clang-c/Index.h>
#include "backend/ideprint.h"
#include <sstream>
#include <iomanip>

using namespace jdi;

// Note: The old helper functions (visit_overload, iterate_overloads, referencers_varargs_at)
// are no longer needed - their functionality is implemented directly in:
// - definition_parameter_bounds() - handles parameter counting for all overloads
// - function_variadic_after() - handles variadic parameter detection

bool lang_CPP::is_variadic_function(jdi::definition *d) const {
  if (!definition_is_function(d)) return false;
  return function_variadic_after((jdi::definition_function*) d) != -1;
}

int lang_CPP::function_variadic_after(jdi::definition_function *func) const {
  if (!func) return -1;
  
  clang_adapter::ClangDefinitionFunction* cfunc = 
      dynamic_cast<clang_adapter::ClangDefinitionFunction*>(func);
  if (!cfunc) return -1;
  
  // Check all overloads for variadic
  for (const auto& overload_pair : cfunc->overloads) {
    const auto& overload = overload_pair.second;
    if (!overload) continue;  // Skip null overloads
    
    // Check for C-style variadic (e.g., printf) or ENIGMA varargs
    if (overload->is_variadic) {
      // Return the index of the variadic parameter (last parameter index)
      return overload->params.size() - 1;
    }
    
    // Check for varargs parameter (e.g., choose(const enigma::varargs& args))
    // Check parameter types to see if any is enigma::varargs
    for (size_t i = 0; i < overload->params.size(); ++i) {
      auto param = overload->params[i];
      if (!param) continue;
      
      clang_adapter::ClangDefinitionTyped* typed_param =
          dynamic_cast<clang_adapter::ClangDefinitionTyped*>(param);
      if (typed_param && typed_param->type) {
        std::string type_name = typed_param->type->name;
        // Check if type name contains "varargs" (handles "varargs", "enigma::varargs", etc.)
        if (type_name.find("varargs") != std::string::npos) {
          // Found varargs parameter - return its index
          return i;
        }
      }
    }
  }
  
  // Check template overloads
  for (const auto& overload : cfunc->template_overloads) {
    if (!overload) continue;  // Skip null overloads
    if (overload->is_variadic) {
      return overload->params.size() - 1;
    }
    
    // Skip detailed parameter inspection for template overloads to avoid crashes
  }
  
  return -1;
}

void lang_CPP::definition_parameter_bounds(definition *d, unsigned &min, unsigned &max) const {
  min = 0;  // Conservative: assume 0 minimum since we don't track default parameters
  max = (unsigned) SIZE_MAX;  // Default to unlimited - only restrict if we find specific overloads
  
  if (!d) {
    // If definition is null, allow any number of arguments (max already set to SIZE_MAX)
    return;
  }
  
  std::string func_name = d->name;
  
  if (!(d->flags & DEF_FUNCTION)) {
    cout << "Attempt to use " << func_name << " as function" << endl;
    // Keep max as SIZE_MAX to allow any arguments
    return;
  }
  
  clang_adapter::ClangDefinitionFunction* cfunc = 
      dynamic_cast<clang_adapter::ClangDefinitionFunction*>(d);
  if (!cfunc) {
    // If cast fails, the definition has DEF_FUNCTION flag but isn't a ClangDefinitionFunction
    // This suggests the function wasn't properly registered through clang_adapter
    // Try to find it in the parent scope if it's a scope member
    if (d->parent) {
      clang_adapter::ClangDefinitionScope* scope = 
          dynamic_cast<clang_adapter::ClangDefinitionScope*>(d->parent);
      if (scope) {
        clang_adapter::ClangDefinition* found = scope->find_local(d->name);
        if (found) {
          cfunc = dynamic_cast<clang_adapter::ClangDefinitionFunction*>(found);
        }
      }
    }
    
    if (!cfunc) {
      // Still not found - this function definition exists but isn't properly structured
      // This can happen if functions are added directly as ClangDefinition instead of ClangDefinitionFunction
      // For now, allow any number of arguments to avoid false errors
      // TODO: Ensure all functions are registered as ClangDefinitionFunction instances
      // TODO: Consider adding parameter info to base ClangDefinition for functions
      return;
    }
  }
  
  bool found_any_overload = false;
  max = 0;  // Reset max - we'll calculate it from overloads
  
  // Iterate all overloads to find min/max parameter counts
  for (const auto& overload_pair : cfunc->overloads) {
    const auto& overload = overload_pair.second;
    if (!overload) continue;  // Skip null overloads
    found_any_overload = true;
    
    // Safely access overload fields - if access fails, skip this overload
    unsigned param_count = 0;
    bool is_variadic = false;
    try {
      param_count = overload->params.size();
      is_variadic = overload->is_variadic;
    } catch (...) {
      // Overload object is corrupted, skip it
      continue;
    }
    
    if (is_variadic) {
      max = (unsigned) SIZE_MAX;  // Variadic means unlimited
      break;  // Once we find variadic, we're done
    } else {
      if (param_count > max) max = param_count;
    }
    // Note: We don't set min here because we don't have information about
    // which parameters have default values. Setting min=0 is conservative
    // and allows functions with defaulted parameters to be called with fewer args.
  }
  
  // Check template overloads (only if we haven't found a variadic overload)
  if (max != (unsigned) SIZE_MAX) {
    for (const auto& overload : cfunc->template_overloads) {
      if (!overload) continue;  // Skip null overloads
      found_any_overload = true;
      unsigned param_count = overload->params.size();
      
      // Check if any parameter type contains "varargs"
      bool has_varargs_param = false;
      for (const auto& param : overload->params) {
        if (!param) continue;
        
        // Safely access parameter name - use cursor first to avoid corruption issues
        std::string param_name;
        if (!clang_Cursor_isNull(param->cursor)) {
          CXString name_str = clang_getCursorSpelling(param->cursor);
          const char* name_cstr = clang_getCString(name_str);
          if (name_cstr) {
            param_name = name_cstr;
          }
          clang_disposeString(name_str);
        }
        // Fallback to param->name only if cursor didn't work
        if (param_name.empty()) {
          try {
            param_name = param->name;
          } catch (...) {
            param_name = "arg_unknown";
          }
        }
        if (param_name.empty()) {
          param_name = "arg_unknown";
        }
        
        // Check parameter name first
        if (param_name.find("varargs") != std::string::npos) {
          has_varargs_param = true;
          break;
        }
        
        // Check if parameter is typed and check its type name
        clang_adapter::ClangDefinitionTyped* typed_param = 
            dynamic_cast<clang_adapter::ClangDefinitionTyped*>(param);
        if (typed_param && typed_param->type) {
          std::string type_name = typed_param->type->name;
            if (type_name.find("varargs") != std::string::npos) {
              has_varargs_param = true;
              break;
            }
        }
      }
      
      if (overload->is_variadic || has_varargs_param) {
        max = (unsigned) SIZE_MAX;
        break;  // Once we find variadic, we're done
      } else {
        if (param_count > max) max = param_count;
      }
    }
  }
  
  // If no overloads found, set max back to unlimited to be safe
  if (!found_any_overload) {
    max = (unsigned) SIZE_MAX;
  }
}

bool lang_CPP::definition_is_function(definition *d) const {
  if (!d) return false;
  if (d->flags & DEF_FUNCTION) return true;
  // Check for template functions - template functions have both DEF_TEMPLATE and DEF_FUNCTION flags
  if (d->flags & DEF_TEMPLATE) {
    // A template function would have both flags set, or we can check if it's a function template
    // by checking if it has function-like characteristics (overloads)
    clang_adapter::ClangDefinitionFunction* cfunc = 
        dynamic_cast<clang_adapter::ClangDefinitionFunction*>(d);
    if (cfunc) return true;  // Template function
  }
  return false;
}

size_t lang_CPP::definition_overload_count(jdi::definition *d) const {
  if (!(d->flags & DEF_FUNCTION)) return 0;
  
  clang_adapter::ClangDefinitionFunction* cfunc = 
      dynamic_cast<clang_adapter::ClangDefinitionFunction*>(d);
  if (!cfunc) return 0;
  
  // Return total count of overloads + template overloads
  return cfunc->overloads.size() + cfunc->template_overloads.size();
}


#include "languages/lang_CPP.h"
definition* lang_CPP::find_typename(std::string_view name) const {
  definition* d = look_up(name);
  if (!d) return NULL;
  if (d->flags & DEF_TYPENAME) return d;
  return NULL;
}

bool lang_CPP::global_exists(string n) const {
  definition* d = look_up(n);
  return d;
}


void lang_CPP::quickmember_variable(jdi::definition_scope* scope, jdi::definition* type, string name) {
  if (!scope || !type) return;
  // JDI removed - ClangDefinitionTyped constructor signature
  // ClangDefinitionScope::members uses shared_ptr, so we need to use make_shared
  auto typed_def = std::make_shared<clang_adapter::ClangDefinitionTyped>(
      name, scope, jdi::DEF_TYPED, clang_getNullCursor(), type);
  scope->members[name] = typed_def;
}

// Static error handler instance - avoids creating multiple instances
// Note: A central error handler from context would be preferable, but the ClangDefinitionFunction
// constructor doesn't require an error handler parameter, so a static instance is used here.
static enigma::parsing::StdErrorHandler error_handler_instance;
void lang_CPP::quickmember_script(jdi::definition_scope* scope, string name) {
  if (!scope) return;
  
  clang_adapter::ClangDefinitionScope* cscope = 
      dynamic_cast<clang_adapter::ClangDefinitionScope*>(scope);
  if (!cscope) return;
  
  // Create a ClangDefinitionFunction with 16 defaulted variant parameters
  auto func_def = std::make_shared<clang_adapter::ClangDefinitionFunction>(
      name, cscope, jdi::DEF_FUNCTION, clang_getNullCursor());
  
  // Create an overload with 16 parameters of type variant
  auto overload = std::make_shared<clang_adapter::ClangDefinitionOverload>(
      name, cscope, jdi::DEF_OVERLOAD | jdi::DEF_FUNCTION, clang_getNullCursor());
  
  // Add 16 variant parameters (defaulted)
  for (int i = 0; i < 16; ++i) {
    // Use enigma_type__variant if available
    if (enigma_type__variant) {
      overload->params.push_back(enigma_type__variant);
    } else {
      // Fallback: create a temporary definition for variant type
      // This should rarely happen if initialization worked
      overload->params.push_back(nullptr);
    }
  }
  
  // Add overload to function (use a key based on parameter signature)
  // For scripts, we use an empty string as the signature key since all have same signature
  func_def->overloads[""] = overload;
  
  // Add function to scope
  cscope->members[name] = func_def;
}
