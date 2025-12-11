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

// ClangContext implementation
ClangContext::ClangContext() : index_(nullptr), tu_(nullptr) {
  index_ = clang_createIndex(0, 0);
  global_scope_ = std::make_unique<ClangDefinitionScope>("", nullptr, jdi::DEF_SCOPE, clang_getNullCursor());
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
  include_dirs_.push_back("-I" + dir);
}

void ClangContext::add_define(const std::string& name, const std::string& value) {
  if (value.empty()) {
    defines_.push_back("-D" + name);
  } else {
    defines_.push_back("-D" + name + "=" + value);
  }
}

std::vector<const char*> ClangContext::build_args() {
  std::vector<const char*> args;
  
  // Add C++ standard
  args.push_back("-std=c++17");
  
  // Add include directories
  for (const auto& dir : include_dirs_) {
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
struct TraversalState {
  ClangContext* ctx;
  std::vector<ClangDefinitionScope*> scope_stack;
  
  TraversalState(ClangContext* c) : ctx(c) {
    scope_stack.push_back(c->get_global());
  }
  
  ClangDefinitionScope* current_scope() {
    return scope_stack.empty() ? ctx->get_global() : scope_stack.back();
  }
  
  void push_scope(ClangDefinitionScope* scope) {
    scope_stack.push_back(scope);
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
  
  ClangDefinitionScope* target_scope = state->current_scope();
  
  // Process the cursor
  ctx->process_cursor(cursor, target_scope);
  
  // If this cursor creates a new scope, push it
  CXCursorKind kind = clang_getCursorKind(cursor);
  unsigned int flags = cursor_kind_to_flags(kind);
  
  if (flags & jdi::DEF_SCOPE) {
    std::string name = get_cursor_name(cursor);
    if (!name.empty()) {
      auto it = target_scope->members.find(name);
      if (it != target_scope->members.end()) {
        ClangDefinitionScope* new_scope = static_cast<ClangDefinitionScope*>(it->second.get());
        state->push_scope(new_scope);
        
        // Visit children in this scope
        clang_visitChildren(cursor, visit_cursor, client_data);
        
        state->pop_scope();
        return CXChildVisit_Continue;  // Don't recurse again
      }
    }
  }
  
  // Continue visiting children
  return CXChildVisit_Recurse;
}

void ClangContext::process_cursor(CXCursor cursor, ClangDefinitionScope* scope) {
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
  
  // Check if already exists
  if (scope->members.find(name) != scope->members.end()) {
    return;  // Already added
  }
  
  // Create appropriate definition type
  std::unique_ptr<ClangDefinition> def;
  
  if (flags & jdi::DEF_SCOPE) {
    if (flags & jdi::DEF_CLASS) {
      def = std::make_unique<ClangDefinitionClass>(name, scope, flags, cursor);
    } else {
      def = std::make_unique<ClangDefinitionScope>(name, scope, flags, cursor);
    }
    
    // Add to scope
    ClangDefinitionScope* def_scope = static_cast<ClangDefinitionScope*>(def.get());
    scope->members[name] = std::move(def);
    
    // Visit children to populate this scope (will be handled by visitor)
    
  } else if (flags & jdi::DEF_FUNCTION) {
    def = std::make_unique<ClangDefinitionFunction>(name, scope, flags, cursor);
    
    // Extract function parameters
    ClangDefinitionFunction* func_def = static_cast<ClangDefinitionFunction*>(def.get());
    int num_args = clang_Cursor_getNumArguments(cursor);
    
    // Create overload
    auto overload = std::make_unique<ClangDefinitionOverload>(name, scope, flags | jdi::DEF_OVERLOAD, cursor);
    
    for (int i = 0; i < num_args; ++i) {
      CXCursor arg_cursor = clang_Cursor_getArgument(cursor, i);
      std::string arg_name = get_cursor_name(arg_cursor);
      if (arg_name.empty()) {
        arg_name = "arg" + std::to_string(i);
      }
      
      // Create typed definition for parameter
      unsigned arg_flags = jdi::DEF_TYPED;
      auto param_def = std::make_unique<ClangDefinitionTyped>(
        arg_name, scope, arg_flags, arg_cursor, nullptr);
      
      overload->params.push_back(param_def.get());
      // Don't add params to scope members - they're part of the function
    }
    
    // Check if variadic
    CXType func_type = clang_getCursorType(cursor);
    if (clang_isFunctionTypeVariadic(func_type)) {
      overload->is_variadic = true;
    }
    
    func_def->overloads[name] = std::move(overload);
    scope->members[name] = std::move(def);
    
  } else if (flags & jdi::DEF_TYPED) {
    // Create typed definition
    def = std::make_unique<ClangDefinitionTyped>(name, scope, flags, cursor, nullptr);
    scope->members[name] = std::move(def);
    
  } else {
    // Generic definition
    def = std::make_unique<ClangDefinition>(name, scope, flags, cursor);
    scope->members[name] = std::move(def);
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
