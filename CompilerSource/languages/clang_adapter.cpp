/**
 * @file clang_adapter.cpp
 * @brief Implementation of clang adapter
 */

#include "clang_adapter.h"
#include "parsing/macros.h"
#include <clang-c/Index.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>

namespace clang_adapter {

// Helper to convert cursor kind to flags
unsigned int cursor_kind_to_flags(CXCursorKind kind) {
  unsigned int flags = 0;
  
  switch (kind) {
    case CXCursor_Namespace:
      flags |= jdi::DEF_NAMESPACE | jdi::DEF_SCOPE;
      break;
    case CXCursor_StructDecl:
    case CXCursor_ClassDecl:
    case CXCursor_ClassTemplate:
      flags |= jdi::DEF_CLASS | jdi::DEF_SCOPE | jdi::DEF_TYPENAME;
      break;
    case CXCursor_UnionDecl:
      flags |= jdi::DEF_UNION | jdi::DEF_SCOPE | jdi::DEF_TYPENAME;
      break;
    case CXCursor_EnumDecl:
      flags |= jdi::DEF_ENUM | jdi::DEF_TYPENAME;
      break;
    case CXCursor_FunctionDecl:
    case CXCursor_CXXMethod:
    case CXCursor_FunctionTemplate:
      flags |= jdi::DEF_FUNCTION;
      break;
    case CXCursor_TypedefDecl:
      flags |= jdi::DEF_TYPENAME | jdi::DEF_TYPED;
      break;
    case CXCursor_VarDecl:
    case CXCursor_FieldDecl:
      flags |= jdi::DEF_TYPED;
      break;
    case CXCursor_TemplateTypeParameter:
    case CXCursor_NonTypeTemplateParameter:
      flags |= jdi::DEF_TEMPLATE | jdi::DEF_TEMPPARAM;
      break;
    default:
      break;
  }
  
  return flags;
}

// Helper to get cursor name
std::string get_cursor_name(CXCursor cursor) {
  CXString name = clang_getCursorSpelling(cursor);
  std::string result = clang_getCString(name);
  clang_disposeString(name);
  return result;
}

// Get qualified name
std::string get_qualified_name(CXCursor cursor) {
  CXString name = clang_getCursorDisplayName(cursor);
  std::string result = clang_getCString(name);
  clang_disposeString(name);
  return result;
}

// Get type spelling (string representation of a type)
std::string get_type_spelling(CXType type) {
  CXString type_str = clang_getTypeSpelling(type);
  std::string result = clang_getCString(type_str);
  clang_disposeString(type_str);
  return result;
}

// Helper function to check if a scope is in a specific namespace
static bool is_in_namespace(ClangDefinitionScope* scope, const std::string& namespace_name) {
  if (namespace_name.empty()) {
    return true;  // Empty filter means print all
  }
  
  // Traverse up the parent chain to find the namespace
  ClangDefinitionScope* current = scope;
  while (current) {
    if (current->name == namespace_name && (current->flags & jdi::DEF_NAMESPACE)) {
      return true;
    }
    current = current->parent;
  }
  return false;
}

// ClangContext implementation
ClangContext::ClangContext() : index_(nullptr), tu_(nullptr), namespace_filter_("") {
  index_ = clang_createIndex(0, 0);
  global_scope_ = std::make_shared<ClangDefinitionScope>("", nullptr, jdi::DEF_SCOPE, clang_getNullCursor());
}

ClangContext::~ClangContext() {
  if (tu_) {
    clang_disposeTranslationUnit(tu_);
  }
  if (index_) {
    clang_disposeIndex(index_);
  }
  // macro_token_strings_storage_ will be destroyed here, but any macros
  // that were returned by get_macros() should have been destroyed by then
}

void ClangContext::add_include_dir(const std::string& dir) {
  include_dirs_.push_back(dir);
}

void ClangContext::add_define(const std::string& name, const std::string& value) {
  if (value.empty()) {
    defines_.push_back("-D" + name);
  } else {
    defines_.push_back("-D" + name + "=" + value);
  }
}

std::vector<const char*> ClangContext::build_args() {
  static std::vector<std::string> system_include_strings;
  static std::vector<const char*> system_include_args;
  
  // Add C++ standard and explicitly use libc++
  std::vector<const char*> args;
  args.push_back("-std=c++17");
  args.push_back("-stdlib=libc++");
  
  // Get system include paths (only compute once)
  if (system_include_strings.empty()) {
    #ifdef __APPLE__
      // Try to get SDK path
      std::string sdk_path = "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk";
      FILE* pipe = popen("xcrun --show-sdk-path 2>/dev/null", "r");
      if (pipe) {
        char buffer[512];
        if (fgets(buffer, sizeof(buffer), pipe)) {
          std::string result = buffer;
          // Remove trailing newline
          if (!result.empty() && result.back() == '\n') {
            result.pop_back();
          }
          if (!result.empty()) {
            sdk_path = result;
          }
        }
        pclose(pipe);
      }
      
      // Set the SDK root - this is the key to making clang find the right headers
      // Clang will automatically add include paths in the correct order:
      // 1. C++ headers (from SDK/usr/include/c++/v1)
      // 2. Clang builtin includes
      // 3. C headers (from SDK/usr/include)
      // This order is exactly what libc++ needs
      system_include_strings.push_back("-isysroot");
      system_include_strings.push_back(sdk_path);
      
      // Build the args vector from strings (strings persist in static storage)
      for (const auto& str : system_include_strings) {
        system_include_args.push_back(str.c_str());
      }
    #elif __linux__
      system_include_strings.push_back("-isystem");
      system_include_strings.push_back("/usr/include/c++/11");
      system_include_strings.push_back("-isystem");
      system_include_strings.push_back("/usr/include");
      for (const auto& str : system_include_strings) {
        system_include_args.push_back(str.c_str());
      }
    #elif _WIN32
      // Windows paths would go here
    #endif
  }
  
  // Add system includes to args (pointers are valid because strings are static)
  args.insert(args.end(), system_include_args.begin(), system_include_args.end());
  
  // Add include directories (as separate -I and path arguments)
  for (const auto& dir : include_dirs_) {
    args.push_back("-I");
    args.push_back(dir.c_str());
  }
  
  // Add defines
  for (const auto& def : defines_) {
    args.push_back(def.c_str());
  }
  
  return args;
}

int ClangContext::parse_file(const std::string& filepath,
                              const std::vector<std::string>& include_dirs,
                              const std::vector<std::string>& defines) {
  // Add the directory containing the file being parsed as an include directory
  // This allows relative includes (like "rect.h") to be found
  size_t last_slash = filepath.find_last_of("/\\");
  if (last_slash != std::string::npos) {
    std::string file_dir = filepath.substr(0, last_slash);
    add_include_dir(file_dir);
    
    // Also add parent directories up to ENIGMAsystem level
    // This allows includes from sibling directories
    std::string current = file_dir;
    while (current.length() > 0) {
      size_t slash = current.find_last_of("/\\");
      if (slash == std::string::npos) break;
      std::string parent = current.substr(0, slash);
      add_include_dir(parent);
      // Stop when we reach ENIGMAsystem directory
      if (parent.find("ENIGMAsystem") != std::string::npos && 
          parent.find_last_of("/\\") < parent.find("ENIGMAsystem") + 12) {
        break;
      }
      current = parent;
    }
  }
  
  // Add provided include dirs and defines
  for (const auto& dir : include_dirs) {
    add_include_dir(dir);
  }
  for (const auto& def : defines) {
    size_t eq = def.find('=');
    if (eq != std::string::npos) {
      add_define(def.substr(0, eq), def.substr(eq + 1));
    } else {
      add_define(def);
    }
  }
  
  auto args = build_args();
  
  // Parse the translation unit
  tu_ = clang_parseTranslationUnit(
    index_,
    filepath.c_str(),
    args.data(),
    args.size(),
    nullptr, 0,
    CXTranslationUnit_None | CXTranslationUnit_DetailedPreprocessingRecord
  );
  
  if (!tu_) {
    std::cerr << "Failed to parse translation unit: " << filepath << std::endl;
    return 1;
  }
  
  // Check for errors
  unsigned num_diagnostics = clang_getNumDiagnostics(tu_);
  if (num_diagnostics > 0) {
    for (unsigned i = 0; i < num_diagnostics; ++i) {
      CXDiagnostic diag = clang_getDiagnostic(tu_, i);
      CXString diag_str = clang_formatDiagnostic(diag, clang_defaultDiagnosticDisplayOptions());
      std::cerr << "Diagnostic: " << clang_getCString(diag_str) << std::endl;
      clang_disposeString(diag_str);
      clang_disposeDiagnostic(diag);
    }
  }
  
  // Build definition tree from AST
  build_definitions();
  
  return 0;
}

// Helper structure to track scope during traversal
// Use weak_ptr to avoid use-after-free issues
struct TraversalState {
  ClangContext* ctx;
  std::vector<std::weak_ptr<ClangDefinitionScope>> scope_stack;  // Stack of weak pointers to scopes
  
  TraversalState(ClangContext* c) : ctx(c) {
    // Start with global scope
    if (auto global = ctx->get_global_shared()) {
      scope_stack.push_back(global);
    }
  }
  
  std::shared_ptr<ClangDefinitionScope> current_scope() {
    if (scope_stack.empty()) {
      return ctx->get_global_shared();
    }
    
    // Try to lock the weak_ptr - if it's expired, return global
    auto locked = scope_stack.back().lock();
    if (!locked) {
      return ctx->get_global_shared();
    }
    return locked;
  }
  
  void push_scope(std::shared_ptr<ClangDefinitionScope> scope) {
    if (scope) {
      scope_stack.push_back(scope);
    }
  }
  
  void pop_scope() {
    if (!scope_stack.empty()) {
      scope_stack.pop_back();
    }
  }
};

void ClangContext::build_definitions() {
  if (!tu_) return;
  
  CXCursor root = clang_getTranslationUnitCursor(tu_);
  
  // Create traversal state
  TraversalState state(this);
  
  // Visit all children of the translation unit
  clang_visitChildren(root, visit_cursor, &state);
}

enum CXChildVisitResult ClangContext::visit_cursor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
  TraversalState* state = static_cast<TraversalState*>(client_data);
  ClangContext* ctx = state->ctx;
  
  auto target_scope = state->current_scope();
  
  // Safety check: ensure scope is valid
  if (!target_scope) {
    return CXChildVisit_Continue;
  }
  
  // Process the cursor - pass a lambda that re-fetches the scope to avoid use-after-free
  ctx->process_cursor(cursor, [state]() { return state->current_scope(); });
  
  // If this cursor creates a new scope, push it
  CXCursorKind kind = clang_getCursorKind(cursor);
  unsigned int flags = cursor_kind_to_flags(kind);
  
  if (flags & jdi::DEF_SCOPE) {
    std::string name = get_cursor_name(cursor);
    if (!name.empty()) {
      // Re-fetch scope to ensure it's valid
      target_scope = state->current_scope();
      if (target_scope) {
        auto it = target_scope->members.find(name);
        if (it != target_scope->members.end() && it->second) {
          auto new_scope = std::dynamic_pointer_cast<ClangDefinitionScope>(it->second);
          if (new_scope) {
            state->push_scope(new_scope);
            
            // Visit children in this scope
            clang_visitChildren(cursor, visit_cursor, client_data);
            
            state->pop_scope();
            return CXChildVisit_Continue;  // Don't recurse again
          }
        }
      }
    }
  }
  
  // Continue visiting children
  return CXChildVisit_Recurse;
}

void ClangContext::process_cursor(CXCursor cursor, std::function<std::shared_ptr<ClangDefinitionScope>()> get_scope) {
  // Re-fetch scope right before use to avoid use-after-free
  auto scope = get_scope();
  
  // Safety check: scope must be valid
  if (!scope) {
    return;
  }
  
  CXCursorKind kind = clang_getCursorKind(cursor);
  
  // Skip certain cursor kinds
  if (kind == CXCursor_TranslationUnit || 
      kind == CXCursor_UnexposedDecl ||
      kind == CXCursor_FirstInvalid) {
    return;
  }
  
  std::string name = get_cursor_name(cursor);
  if (name.empty()) {
    return;
  }
  
  unsigned int flags = cursor_kind_to_flags(kind);
  if (flags == 0) {
    return;  // Not a definition we care about
  }
  
  // Re-fetch scope again right before accessing members to ensure it's still valid
  scope = get_scope();
  if (!scope) {
    return;
  }
  
  // Create appropriate definition type
  std::shared_ptr<ClangDefinition> def;
  
  if (flags & jdi::DEF_SCOPE) {
    // For scopes, check if already exists (don't create duplicates)
    if (scope->members.find(name) != scope->members.end()) {
      return;  // Already added
    }
    
    if (flags & jdi::DEF_CLASS) {
      def = std::make_shared<ClangDefinitionClass>(name, scope.get(), flags, cursor);
    } else {
      def = std::make_shared<ClangDefinitionScope>(name, scope.get(), flags, cursor);
    }
    
    // Re-fetch scope before insertion in case it changed
    scope = get_scope();
    if (!scope) {
      return;
    }
    
    // Add to scope
    scope->members[name] = def;
    
    // Visit children to populate this scope (will be handled by visitor)
    
  } else if (flags & jdi::DEF_FUNCTION) {
    // For functions, check if function already exists (for overload detection)
    // If it exists, we'll add this as a new overload instead of creating a new function
    // Re-fetch scope to ensure it's valid
    scope = get_scope();
    if (!scope) {
      return;
    }
    
    // Check if a function with this EXACT name already exists in this scope
    bool function_exists = (scope->members.find(name) != scope->members.end());
    
    // Debug: if function_exists is true, verify it's actually the same name
    if (function_exists) {
      auto check_it = scope->members.find(name);
      if (check_it != scope->members.end() && check_it->first != name) {
        // This shouldn't happen - the key should match the name
        std::cerr << "ERROR: Scope member key '" << check_it->first 
                  << "' doesn't match function name '" << name << "'" << std::endl;
        function_exists = false;  // Treat as new function
      }
    }
    
    std::shared_ptr<ClangDefinitionFunction> func_def;
    
    if (function_exists) {
      // Function with this name already exists - get the existing one to add overload
      auto existing_it = scope->members.find(name);
      if (existing_it != scope->members.end()) {
        func_def = std::dynamic_pointer_cast<ClangDefinitionFunction>(existing_it->second);
        if (!func_def) {
          // Name collision with non-function - skip
          return;
        }
        def = existing_it->second;  // Reuse existing definition
      } else {
        // Shouldn't happen, but handle it
        function_exists = false;
      }
    }
    
    if (!func_def) {
      // Create new function definition - this is a NEW function name
      // Ensure overloads map is empty for a new function
      def = std::make_shared<ClangDefinitionFunction>(name, scope.get(), flags, cursor);
      func_def = std::static_pointer_cast<ClangDefinitionFunction>(def);
      // Verify overloads is empty for a new function
      if (!func_def->overloads.empty()) {
        std::cerr << "WARNING: New function '" << name << "' has non-empty overloads map!" << std::endl;
        func_def->overloads.clear();  // Clear it to be safe
      }
    }
    
    // Extract function parameters
    int num_args = clang_Cursor_getNumArguments(cursor);
    
    // Handle -1 return value (function templates, invalid cursors, etc.)
    if (num_args < 0) {
      num_args = 0;
    }
    
    // Get function type for variadic check (needed both for printing and overload creation)
    CXType func_type = clang_getCursorType(cursor);
    
    // Check if this is a template function
    bool is_template = (kind == CXCursor_FunctionTemplate || kind == CXCursor_ClassTemplate);
    std::vector<std::string> template_params;
    if (is_template) {
      // Get template parameters by visiting template parameter children
      // We'll collect them during traversal, but for now just mark as template
      // The actual template parameters are handled as separate cursors during AST traversal
    }
    
    // Create overload
    auto overload = std::make_shared<ClangDefinitionOverload>(name, scope.get(), flags | jdi::DEF_OVERLOAD, cursor);
    
    // Collect parameter types for printing
    std::vector<std::string> param_types;
    std::vector<std::string> param_names;
    
    for (int i = 0; i < num_args; ++i) {
      CXCursor arg_cursor = clang_Cursor_getArgument(cursor, i);
      std::string arg_name = get_cursor_name(arg_cursor);
      if (arg_name.empty()) {
        arg_name = "arg" + std::to_string(i);
      }
      
      // Get parameter type
      CXType arg_type = clang_getArgType(func_type, i);
      std::string arg_type_str = get_type_spelling(arg_type);
      param_types.push_back(arg_type_str);
      param_names.push_back(arg_name);
      
      // Re-fetch scope before creating parameter (to ensure it's valid)
      scope = get_scope();
      if (!scope) {
        return;
      }
      
      // Create typed definition for parameter
      unsigned arg_flags = jdi::DEF_TYPED;
      auto param_def = std::make_shared<ClangDefinitionTyped>(
        arg_name, scope.get(), arg_flags, arg_cursor, nullptr);
      
      // Store the shared_ptr to keep it alive, and add raw pointer to params
      overload->params.push_back(param_def.get());
      overload->owned_params_storage.push_back(param_def);
      // Don't add params to scope members - they're part of the function
    }
    
    if (clang_isFunctionTypeVariadic(func_type)) {
      overload->is_variadic = true;
    }
    
    // Generate a unique key for this overload based on parameter types
    // This allows us to store multiple overloads with the same name
    std::string overload_key = name;
    for (const auto& param_type : param_types) {
      overload_key += "_" + param_type;
    }
    
    // Check if this exact overload already exists
    bool overload_exists = (func_def->overloads.find(overload_key) != func_def->overloads.end());
    if (overload_exists) {
      // This exact overload already exists, skip it
      return;
    }
    
    // Count existing overloads BEFORE adding this one
    // For a new function (!function_exists), this should be 0, so overload_count will be 1
    // For an existing function (function_exists), this will be the number of existing overloads
    size_t overload_count;
    if (!function_exists) {
      // This is a new function, so this is the first overload
      overload_count = 1;
      // Verify overloads is empty
      if (!func_def->overloads.empty()) {
        std::cerr << "WARNING: New function '" << name << "' has " << func_def->overloads.size() 
                  << " existing overloads!" << std::endl;
      }
    } else {
      // This is an existing function, count existing overloads
      overload_count = func_def->overloads.size() + 1;
    }
    
    // Add the overload
    func_def->overloads[overload_key] = overload;
    
    // Only add to scope members if this is a new function (not just a new overload)
    if (!function_exists) {
      // Re-fetch scope before insertion
      scope = get_scope();
      if (!scope) {
        return;
      }
      scope->members[name] = def;
    }
    
    // Print function information (only if namespace filter matches)
    if (namespace_filter_.empty() || is_in_namespace(scope.get(), namespace_filter_)) {
      std::string scope_name = scope ? (scope->name.empty() ? "global" : scope->name) : "global";
      
      // Check if there are multiple overloads (after adding this one)
      // If function_exists is true, we know there was at least one overload before this one
      // If function_exists is false but overloads.size() > 1, we've added multiple in one go (shouldn't happen)
      bool has_multiple_overloads = function_exists || (func_def->overloads.size() > 1);
      
      // Print function header
      if (!function_exists) {
        std::cout << "Found function: " << scope_name << "::" << name;
        std::cout << std::endl;
      }
      
      // Only print "Overload X:" if there are multiple overloads
      // For single overloads, just print the signature directly
      if (has_multiple_overloads) {
        std::cout << "  Overload " << overload_count << ": ";
      } else {
        std::cout << "  ";
      }
      
      // Print return type
      CXType return_type = clang_getResultType(func_type);
      std::string return_type_str = get_type_spelling(return_type);
      std::cout << return_type_str << " ";
      std::cout << name << "(";
      
      // Print parameters with types
      if (param_types.empty()) {
        std::cout << "void";
      } else {
        for (size_t i = 0; i < param_types.size(); ++i) {
          if (i > 0) std::cout << ", ";
          std::cout << param_types[i] << " " << param_names[i];
        }
      }
      
      if (clang_isFunctionTypeVariadic(func_type)) {
        if (num_args > 0) std::cout << ", ";
        std::cout << "...";
      }
      
      std::cout << ")";
      
      if (clang_isFunctionTypeVariadic(func_type)) {
        std::cout << " [variadic]";
      }
      if (is_template) {
        std::cout << " [template]";
      }
      std::cout << std::endl;
    }
    
  } else if (flags & jdi::DEF_TYPED) {
    // Re-fetch scope before insertion
    scope = get_scope();
    if (!scope) {
      return;
    }
    
    // Create typed definition
    def = std::make_shared<ClangDefinitionTyped>(name, scope.get(), flags, cursor, nullptr);
    scope->members[name] = def;
    
  } else {
    // Re-fetch scope before insertion
    scope = get_scope();
    if (!scope) {
      return;
    }
    
    // Generic definition
    def = std::make_shared<ClangDefinition>(name, scope.get(), flags, cursor);
    scope->members[name] = def;
  }
}

ClangDefinition* ClangContext::look_up(const std::string& name) {
  return global_scope_->look_up(name);
}

ClangDefinition* ClangDefinitionScope::look_up(const std::string& name) {
  auto it = members.find(name);
  if (it != members.end()) {
    return it->second.get();
  }
  
  // Search parent scopes
  if (parent) {
    return parent->look_up(name);
  }
  
  return nullptr;
}

ClangDefinition* ClangDefinitionScope::find_local(const std::string& name) {
  auto it = members.find(name);
  if (it != members.end()) {
    return it->second.get();
  }
  return nullptr;
}

// Helper to convert clang tokens to enigma tokens
static enigma::parsing::TokenVector tokens_from_clang(CXTranslationUnit tu, CXSourceRange range) {
  enigma::parsing::TokenVector result;
  
  CXToken* tokens = nullptr;
  unsigned num_tokens = 0;
  clang_tokenize(tu, range, &tokens, &num_tokens);
  
  for (unsigned i = 0; i < num_tokens; ++i) {
    CXString spelling = clang_getTokenSpelling(tu, tokens[i]);
    std::string token_str = clang_getCString(spelling);
    clang_disposeString(spelling);
    
    // Create token - simplified for now
    // TODO: Map clang token kinds to enigma token types properly
    enigma::parsing::CodeSnippet snippet;
    snippet.content = token_str;
    snippet.line = 0;
    snippet.position = 0;
    enigma::parsing::Token token(enigma::parsing::TT_IDENTIFIER, snippet);
    result.push_back(token);
  }
  
  clang_disposeTokens(tu, tokens, num_tokens);
  return result;
}

std::map<std::string, std::unique_ptr<enigma::parsing::Macro>> ClangContext::get_macros() {
  if (!tu_) {
    return std::map<std::string, std::unique_ptr<enigma::parsing::Macro>>();
  }
  
  // Extract macros from preprocessor
  extract_macros();
  
  // Move the macros out (can't copy unique_ptr)
  // Note: macro_token_strings_storage_ is NOT cleared here - it needs to persist
  // for the lifetime of the returned macros since they contain string_views into it
  std::map<std::string, std::unique_ptr<enigma::parsing::Macro>> result;
  for (auto& pair : macros_) {
    result[pair.first] = std::move(pair.second);
  }
  macros_.clear();
  return result;
}

void ClangContext::extract_macros() {
  macros_.clear();
  // Don't clear macro_token_strings_storage_ here - it needs to persist for
  // macros that were previously returned by get_macros(). Only clear it when
  // the context is destroyed or explicitly reset.
  
  if (!tu_) {
    return;
  }
  
  // Get all preprocessor entities
  CXCursor root = clang_getTranslationUnitCursor(tu_);
  
  // Visit all preprocessor macros
  struct MacroVisitor {
    ClangContext* ctx;
    CXTranslationUnit tu;
    
    static enum CXChildVisitResult visit(CXCursor cursor, CXCursor parent, CXClientData client_data) {
      MacroVisitor* visitor = static_cast<MacroVisitor*>(client_data);
      
      CXCursorKind kind = clang_getCursorKind(cursor);
      
      // Look for macro definitions
      if (kind == CXCursor_MacroDefinition) {
        std::string name = get_cursor_name(cursor);
        if (name.empty()) {
          return CXChildVisit_Continue;
        }
        
        // Get the macro expansion
        CXSourceRange range = clang_getCursorExtent(cursor);
        
        // Tokenize the macro definition
        CXToken* tokens = nullptr;
        unsigned num_tokens = 0;
        clang_tokenize(visitor->tu, range, &tokens, &num_tokens);
        
        if (num_tokens > 0) {
          // Check if it's a function-like macro
          bool is_function = false;
          bool is_variadic = false;
          std::vector<std::string> params;
          
          // Look for '(' after macro name
          if (num_tokens > 1) {
            CXString second_token = clang_getTokenSpelling(visitor->tu, tokens[1]);
            std::string second_str = clang_getCString(second_token);
            clang_disposeString(second_token);
            
            if (second_str == "(") {
              is_function = true;
              // Extract parameters
              for (unsigned i = 2; i < num_tokens; ++i) {
                CXString token_str = clang_getTokenSpelling(visitor->tu, tokens[i]);
                std::string token = clang_getCString(token_str);
                clang_disposeString(token_str);
                
                if (token == ")") {
                  break;
                }
                if (token == "," || token == "(") {
                  continue;
                }
                if (token == "...") {
                  is_variadic = true;
                  break;
                }
                params.push_back(token);
              }
            }
          }
          
          // Extract macro value (everything after name and params)
          unsigned start_idx = is_function ? 2 + params.size() + (is_variadic ? 1 : 0) + 1 : 1;
          
          // Build a single string containing all token content, separated by spaces
          // This string will be owned by the Macro via owned_raw_string
          std::string macro_content;
          std::vector<std::pair<size_t, size_t>> token_positions;  // start, length for each token
          
          for (unsigned i = start_idx; i < num_tokens; ++i) {
            CXString token_str = clang_getTokenSpelling(visitor->tu, tokens[i]);
            std::string token = clang_getCString(token_str);
            clang_disposeString(token_str);
            
            // Skip whitespace/newline tokens
            if (token.empty() || token == "\n" || token == " ") {
              continue;
            }
            
            // Record position and add to content string
            size_t start = macro_content.length();
            if (!macro_content.empty()) {
              macro_content += " ";
              start++;
            }
            macro_content += token;
            token_positions.push_back({start, token.length()});
          }
          
          clang_disposeTokens(visitor->tu, tokens, num_tokens);
          
          // Create macro using the string-based constructor so it owns the string
          enigma::parsing::StdErrorHandler err_handler;
          std::unique_ptr<enigma::parsing::Macro> macro;
          
          if (is_function) {
            // For function-like macros, create a shared string to own the content
            // and create tokens with string_views into it
            auto owned_string = std::make_shared<std::string>(std::move(macro_content));
            enigma::parsing::TokenVector value_tokens;
            
            for (const auto& pos : token_positions) {
              enigma::parsing::CodeSnippet snippet;
              snippet.content = std::string_view(owned_string->data() + pos.first, pos.second);
              snippet.line = 0;
              snippet.position = 0;
              enigma::parsing::Token enigma_token(enigma::parsing::TT_IDENTIFIER, snippet);
              value_tokens.push_back(enigma_token);
            }
            
            // Store the shared string in context to keep it alive
            visitor->ctx->macro_token_strings_storage_.push_back(owned_string);
            
            macro = std::make_unique<enigma::parsing::Macro>(
              name, std::move(params), is_variadic, std::move(value_tokens), &err_handler);
          } else {
            // For object-like macros, use the string constructor which handles ownership
            macro = std::make_unique<enigma::parsing::Macro>(
              name, std::move(macro_content), &err_handler);
          }
          
          visitor->ctx->macros_[name] = std::move(macro);
        }
      }
      
      return CXChildVisit_Recurse;
    }
  };
  
  MacroVisitor visitor;
  visitor.ctx = this;
  visitor.tu = tu_;
  
  clang_visitChildren(root, MacroVisitor::visit, &visitor);
}

std::string ClangDefinition::qualified_id() const {
  std::string result = name;
  ClangDefinitionScope* p = parent;
  while (p && !p->name.empty()) {
    result = p->name + "::" + result;
    p = p->parent;
  }
  return result;
}

} // namespace clang_adapter
