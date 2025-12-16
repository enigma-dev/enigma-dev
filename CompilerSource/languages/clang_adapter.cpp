/**
 * @file clang_adapter.cpp
 * @brief Implementation of clang adapter
 */

#include "clang_adapter.h"
#include "parsing/macros.h"
#include "parsing/lexer.h"
#include <clang-c/Index.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <filesystem>

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
    case CXCursor_EnumConstantDecl:
      // Enum constants are typed values (they have integer values)
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
  
  // Initialize builtin primitive types - these are fundamental to C++ and must always be available
  // They're added to the global scope so they can be looked up by the parser
  init_builtin_types();
}

void ClangContext::init_builtin_types() {
  // Create definitions for all fundamental C++ types
  // These are always available in C++ and should never be missing
  static const char* builtin_type_names[] = {
    "void", "bool", "char", "short", "int", "long", "float", "double",
    "signed", "unsigned", "wchar_t", "char16_t", "char32_t", "nullptr_t"
  };
  
  for (const char* type_name : builtin_type_names) {
    auto type_def = std::make_shared<ClangDefinition>(
      type_name, global_scope_.get(), jdi::DEF_TYPENAME, clang_getNullCursor());
    global_scope_->members[type_name] = type_def;
    
    // Set the global builtin_type__int pointer so the parser can use it
    if (std::string(type_name) == "int") {
      jdi::builtin_type__int = type_def.get();
    }
  }
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

void ClangContext::add_quote_include_dir(const std::string& dir) {
  quote_include_dirs_.push_back(dir);
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
  
  // Add C++ standard
  std::vector<const char*> args;
  args.push_back("-std=c++17");
  // Note: -stdlib=libc++ will be added conditionally based on detected compiler
  
  // Get system include paths (only compute once)
  if (system_include_strings.empty()) {
    #ifdef __APPLE__
      // macOS always uses clang, so add -stdlib=libc++
      system_include_strings.push_back("-stdlib=libc++");
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
      system_include_strings.push_back("-isysroot");
      system_include_strings.push_back(sdk_path);
      
      // Find the clang resource directory which contains builtin headers (stdarg.h, etc.)
      // This needs to be set via -resource-dir so clang looks there automatically
      std::vector<std::string> clang_resource_dirs = {
        "/Library/Developer/CommandLineTools/usr/lib/clang/17.0.0",
        "/Library/Developer/CommandLineTools/usr/lib/clang/17",
        "/Library/Developer/CommandLineTools/usr/lib/clang/16.0.0",
        "/Library/Developer/CommandLineTools/usr/lib/clang/16",
        "/Library/Developer/CommandLineTools/usr/lib/clang/15.0.0",
        "/Library/Developer/CommandLineTools/usr/lib/clang/15",
        "/opt/homebrew/opt/llvm/lib/clang/19",
        "/opt/homebrew/opt/llvm/lib/clang/18",
        "/opt/homebrew/opt/llvm/lib/clang/17"
      };
      
      for (const auto& dir : clang_resource_dirs) {
        FILE* check = fopen((dir + "/include/stdarg.h").c_str(), "r");
        if (check) {
          fclose(check);
          system_include_strings.push_back("-resource-dir");
          system_include_strings.push_back(dir);
          break;
        }
      }
      
      // Build the args vector from strings (strings persist in static storage)
      for (const auto& str : system_include_strings) {
        system_include_args.push_back(str.c_str());
      }
    #elif __linux__
      // Auto-detect C++ include paths by querying the compiler
      // Try clang++ first, then fall back to g++
      bool using_clang = false;
      FILE* pipe = popen("clang++ -E -x c++ -v /dev/null 2>&1", "r");
      if (pipe) {
        // Check if clang++ actually exists and works by reading a line
        char test_buffer[256];
        if (fgets(test_buffer, sizeof(test_buffer), pipe)) {
          using_clang = true;
          // Rewind - we need to read from the beginning
          // Since we can't rewind a pipe, close and reopen
          pclose(pipe);
          pipe = popen("clang++ -E -x c++ -v /dev/null 2>&1", "r");
        } else {
          pclose(pipe);
          pipe = nullptr;
        }
      }
      if (!pipe) {
        pipe = popen("g++ -E -x c++ -v /dev/null 2>&1", "r");
      }
      
      // Add -stdlib=libc++ only if using clang
      if (using_clang) {
        system_include_strings.push_back("-stdlib=libc++");
      }
      
      if (pipe) {
        char buffer[512];
        bool in_include_section = false;
        while (fgets(buffer, sizeof(buffer), pipe)) {
          std::string line(buffer);
          // Look for the "#include <...> search starts here:" marker
          if (line.find("#include <...> search starts here:") != std::string::npos) {
            in_include_section = true;
            continue;
          }
          // Look for the "End of search list." marker
          if (line.find("End of search list.") != std::string::npos) {
            break;
          }
          // Extract include paths
          if (in_include_section) {
            // Remove leading whitespace and newline
            size_t start = line.find_first_not_of(" \t\n");
            if (start != std::string::npos) {
              size_t end = line.find_last_not_of(" \t\n");
              if (end != std::string::npos) {
                std::string include_path = line.substr(start, end - start + 1);
                if (!include_path.empty()) {
                  system_include_strings.push_back("-isystem");
                  system_include_strings.push_back(include_path);
                }
              }
            }
          }
        }
        pclose(pipe);
      }
      
      // Fallback to common paths if auto-detection failed
      if (system_include_strings.empty()) {
        system_include_strings.push_back("-isystem");
        system_include_strings.push_back("/usr/include/c++/11");
        system_include_strings.push_back("-isystem");
        system_include_strings.push_back("/usr/include");
      }
      
      // Build the args vector from strings (strings persist in static storage)
      for (const auto& str : system_include_strings) {
        system_include_args.push_back(str.c_str());
      }
    #elif _WIN32
      // Windows paths would go here
    #endif
  }
  
  // Add system includes to args (pointers are valid because strings are static)
  args.insert(args.end(), system_include_args.begin(), system_include_args.end());
  
  // Add quote include directories first (for -iquote, searched before -I for "" includes)
  // These are searched after the directory of the including file, but before -I directories
  if (!quote_include_dirs_.empty()) {
    for (const auto& dir : quote_include_dirs_) {
      args.push_back("-iquote");
      args.push_back(dir.c_str());
    }
  }
  
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
  // Clear quote_include_dirs_ at the start of each parse (test-specific, so safe to clear)
  // Don't clear include_dirs_ as it may be used by build_args() which stores pointers to its strings
  quote_include_dirs_.clear();
  
  // In test mode, add mock directories to quote_include_dirs_ (for -iquote)
  // This ensures mock headers are found for "" includes
  const char* test_env = std::getenv("ENIGMA_TEST");
  const char* tests_env = std::getenv("TESTS");
  bool is_test_mode = (test_env && std::string(test_env) == "TRUE") || 
                      (tests_env && std::string(tests_env) == "TRUE");
  std::cerr << "DEBUG parse_file: filepath=" << filepath 
            << " is_test_mode=" << (is_test_mode ? "true" : "false")
            << " ENIGMA_TEST=" << (test_env ? test_env : "null")
            << " TESTS=" << (tests_env ? tests_env : "null") << std::endl;
  if (is_test_mode) {
    // Find enigma root by looking for ENIGMAsystem in the filepath
    std::string enigma_root;
    size_t enigma_pos = filepath.find("ENIGMAsystem");
    if (enigma_pos != std::string::npos) {
      enigma_root = filepath.substr(0, enigma_pos);
      std::filesystem::path mock_headers_dir = std::filesystem::path(enigma_root) / "CommandLine" / "emake-tests" / "mock-headers";
      if (std::filesystem::exists(mock_headers_dir)) {
        // Add mock directories to quote_include_dirs_ (for -iquote) so they're searched for "" includes
        // These are searched after the directory of the including file, but before -I directories
        std::filesystem::path mock_resources_dir = mock_headers_dir / "ENIGMAsystem" / "SHELL" / "Universal_System" / "Resources";
        if (std::filesystem::exists(mock_resources_dir)) {
          std::string abs_path = std::filesystem::absolute(mock_resources_dir).u8string();
          quote_include_dirs_.push_back(abs_path);
          std::cerr << "DEBUG: Added mock Resources dir: " << abs_path << std::endl;
        } else {
          std::cerr << "DEBUG: Mock Resources dir does not exist: " << mock_resources_dir << std::endl;
        }
        std::filesystem::path mock_pp_dir = mock_headers_dir / "ENIGMAsystem" / "SHELL" / "Preprocessor_Environment_Editable";
        if (std::filesystem::exists(mock_pp_dir)) {
          quote_include_dirs_.push_back(std::filesystem::absolute(mock_pp_dir).u8string());
        }
        std::filesystem::path mock_shell_dir = mock_headers_dir / "ENIGMAsystem" / "SHELL";
        if (std::filesystem::exists(mock_shell_dir)) {
          quote_include_dirs_.push_back(std::filesystem::absolute(mock_shell_dir).u8string());
        }
        quote_include_dirs_.push_back(std::filesystem::absolute(mock_headers_dir).u8string());
      }
    }
  }
  
  // Add the directory containing the file being parsed as an include directory
  // This allows relative includes (like "rect.h") to be found
  size_t last_slash = filepath.find_last_of("/\\");
  if (last_slash != std::string::npos) {
    std::string file_dir = filepath.substr(0, last_slash);
    add_include_dir(file_dir);
    
    // Also add parent directories up to ENIGMAsystem level
    // This allows includes from sibling directories
    std::string current = file_dir;
    std::string enigma_root;
    while (current.length() > 0) {
      size_t slash = current.find_last_of("/\\");
      if (slash == std::string::npos) break;
      std::string parent = current.substr(0, slash);
      add_include_dir(parent);
      // Stop when we reach ENIGMAsystem directory, but remember the root
      if (parent.find("ENIGMAsystem") != std::string::npos && 
          parent.find_last_of("/\\") < parent.find("ENIGMAsystem") + 12) {
        enigma_root = parent.substr(0, parent.find("ENIGMAsystem"));
        break;
      }
      current = parent;
    }
    
    // Also add the shared directory which contains common headers like rect.h
    if (!enigma_root.empty()) {
      add_include_dir(enigma_root + "shared");
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
  
  // Dispose of previous translation unit if it exists
  // This ensures we start fresh, but we need to preserve the global scope
  if (tu_) {
    clang_disposeTranslationUnit(tu_);
    tu_ = nullptr;
  }
  
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
  
  // Check for errors (but don't fail on warnings)
  unsigned num_diagnostics = clang_getNumDiagnostics(tu_);
  bool has_errors = false;
  if (num_diagnostics > 0) {
    std::cerr << "*** Parse diagnostics (" << num_diagnostics << " total): ***" << std::endl;
    for (unsigned i = 0; i < num_diagnostics; ++i) {
      CXDiagnostic diag = clang_getDiagnostic(tu_, i);
      CXDiagnosticSeverity severity = clang_getDiagnosticSeverity(diag);
      CXString diag_str = clang_formatDiagnostic(diag, clang_defaultDiagnosticDisplayOptions());
      const char* severity_str = (severity >= CXDiagnostic_Error ? "ERROR" : 
                                  severity >= CXDiagnostic_Warning ? "WARNING" : "NOTE");
      std::cerr << "  [" << severity_str << "] " << clang_getCString(diag_str) << std::endl;
      
      if (severity >= CXDiagnostic_Error) {
        has_errors = true;
      }
      clang_disposeString(diag_str);
      clang_disposeDiagnostic(diag);
    }
    std::cerr << "*** End parse diagnostics ***" << std::endl;
  }
  
  // Build definition tree from AST even if there are errors
  // This ensures namespaces and other declarations are still created
  // The global scope persists across multiple parses, so types extracted here will be available
  build_definitions();
  
  if (has_errors) {
    // Return error status, but definitions were still built
    return 1;
  }
  
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

// Helper function to recursively find a class definition by name
static std::shared_ptr<ClangDefinitionClass> find_class_in_scope(
    const std::string& class_name, 
    std::shared_ptr<ClangDefinitionScope> scope) {
  if (!scope) return nullptr;
  
  // Check if this scope has the class
  auto it = scope->members.find(class_name);
  if (it != scope->members.end()) {
    auto class_def = std::dynamic_pointer_cast<ClangDefinitionClass>(it->second);
    if (class_def) {
      return class_def;
    }
  }
  
  // Recursively search in child scopes
  for (const auto& member : scope->members) {
    auto child_scope = std::dynamic_pointer_cast<ClangDefinitionScope>(member.second);
    if (child_scope) {
      auto found = find_class_in_scope(class_name, child_scope);
      if (found) return found;
    }
  }
  
  return nullptr;
}

// Helper struct to track current class during traversal
struct AncestorTraversalState {
  std::string current_class;
  std::shared_ptr<ClangDefinitionScope> global_scope;
};

// Helper function to populate ancestors for all classes (second pass)
// Visits CXXBaseSpecifier cursors to find base classes
static enum CXChildVisitResult populate_ancestors_visitor(CXCursor cursor, CXCursor /* parent */, CXClientData client_data) {
  auto* state = static_cast<AncestorTraversalState*>(client_data);
  
  CXCursorKind kind = clang_getCursorKind(cursor);
  
  // Track current class being processed
  if (kind == CXCursor_StructDecl || kind == CXCursor_ClassDecl) {
    std::string name = get_cursor_name(cursor);
    if (!name.empty()) {
      state->current_class = name;
      if (name.find("object_") == 0) {  // Only log object_* classes
        std::cout << "DEBUG populate_ancestors: Processing class '" << name << "'" << std::endl;
      }
    }
  }
  
  // Process base class specifiers
  if (kind == CXCursor_CXXBaseSpecifier) {
    // Get the type of the base class
    CXType base_type = clang_getCursorType(cursor);
    std::string base_name = get_type_spelling(base_type);
    
    // Use the tracked current class name
    std::string class_name = state->current_class;
    
    if (!class_name.empty() && class_name.find("object_") == 0) {
      std::cout << "DEBUG populate_ancestors: Found base specifier for class '" << class_name 
                << "' with base '" << base_name << "'" << std::endl;
    }
    
    // Remove namespace prefix if present (e.g., "enigma::object_transform" -> "object_transform")
    std::string base_name_short = base_name;
    size_t ns_pos = base_name_short.find_last_of("::");
    if (ns_pos != std::string::npos && ns_pos + 1 < base_name_short.length()) {
      base_name_short = base_name_short.substr(ns_pos + 1);
    }
    
    if (!class_name.empty() && !base_name_short.empty()) {
      auto class_def = find_class_in_scope(class_name, state->global_scope);
      auto base_class = find_class_in_scope(base_name_short, state->global_scope);
      
      if (class_name.find("object_") == 0) {
        std::cout << "DEBUG populate_ancestors: Looking for class '" << class_name 
                  << "' and base '" << base_name_short << "'" << std::endl;
        std::cout << "DEBUG populate_ancestors: class_def=" << (class_def ? "found" : "NOT FOUND")
                  << ", base_class=" << (base_class ? "found" : "NOT FOUND") << std::endl;
      }
      
      if (class_def && base_class) {
        int access = clang_getCXXAccessSpecifier(cursor);
        class_def->ancestors.push_back(std::make_pair(base_class.get(), access));
        if (class_name.find("object_") == 0) {
          std::cout << "DEBUG populate_ancestors: Added ancestor '" << base_name_short 
                    << "' to class '" << class_name << "'" << std::endl;
        }
      }
    }
  }
  
  return CXChildVisit_Recurse;
}

void ClangContext::build_definitions() {
  if (!tu_) return;
  
  CXCursor root = clang_getTranslationUnitCursor(tu_);
  
  // Create traversal state
  TraversalState state(this);
  
  // First pass: Visit all children of the translation unit to create definitions
  clang_visitChildren(root, visit_cursor, &state);
  
  // Second pass: Populate ancestors by visiting base class specifiers
  std::cout << "DEBUG: Starting second pass to populate ancestors" << std::endl;
  AncestorTraversalState pass2_state;
  pass2_state.global_scope = global_scope_;
  clang_visitChildren(root, populate_ancestors_visitor, &pass2_state);
  std::cout << "DEBUG: Second pass complete" << std::endl;
}

enum CXChildVisitResult ClangContext::visit_cursor(CXCursor cursor, CXCursor /* parent */, CXClientData client_data) {
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
  
  unsigned int flags = cursor_kind_to_flags(kind);
  if (flags == 0) {
    return;  // Not a definition we care about
  }
  
  // For variable declarations, we need to extract builtin types even if name is empty
  // (though typically variable declarations have names)
  if (name.empty() && !(flags & jdi::DEF_TYPED)) {
    return;
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
    
    // Check for C-style variadic (e.g., printf)
    if (clang_isFunctionTypeVariadic(func_type)) {
      overload->is_variadic = true;
    }
    
    // Also check for ENIGMA-style variadic: parameters of type enigma::varargs
    // This handles functions like choose(const enigma::varargs& args)
    for (const auto& param_type_str : param_types) {
      if (param_type_str.find("varargs") != std::string::npos) {
        overload->is_variadic = true;
        break;  // Found varargs parameter, no need to check others
      }
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
    
  } else if (flags & jdi::DEF_TYPED || kind == CXCursor_EnumConstantDecl) {
    // Handle typed definitions (variables, fields, enum constants)
    // For enum constants, ensure they get DEF_TYPED flag
    if (kind == CXCursor_EnumConstantDecl && !(flags & jdi::DEF_TYPED)) {
      flags |= jdi::DEF_TYPED;
    }
    
    // Special handling for enum constants from anonymous enums
    // If the enum constant is from an anonymous enum in enigma_user namespace,
    // we need to ensure it's stored as a direct member of the namespace, not inside the enum type
    if (kind == CXCursor_EnumConstantDecl) {
      CXCursor parent_cursor = clang_getCursorSemanticParent(cursor);
      CXCursorKind parent_kind = clang_getCursorKind(parent_cursor);
      
      if (parent_kind == CXCursor_EnumDecl) {
        std::string enum_name = get_cursor_name(parent_cursor);
        // If enum is anonymous (empty name), we need to check the current scope
        // Re-fetch scope to get current context
        scope = get_scope();
        if (scope && enum_name.empty() && scope->name == "enigma_user") {
          // This is an enum constant from an anonymous enum in enigma_user namespace
          // Store it as a direct member of the namespace (current scope)
          // The scope is already correct (enigma_user namespace), so we can proceed
          // with storing it directly in the namespace members
        }
      }
    }
    
    // Re-fetch scope before insertion
    scope = get_scope();
    if (!scope) {
      return;
    }
    
    // Extract builtin type from type information and add it to global scope if it's a builtin
    // This ensures builtin types like int, float, etc. are always available
    CXType var_type = clang_getCursorType(cursor);
    CXType canonical_type = clang_getCanonicalType(var_type);
    enum CXTypeKind type_kind = canonical_type.kind;
    
    // Check if this is a builtin type
    // Builtin types have kind values between CXType_FirstBuiltin and CXType_LastBuiltin
    if (type_kind >= CXType_FirstBuiltin && type_kind <= CXType_LastBuiltin) {
      // For builtin types, get the spelling directly
      std::string type_name = get_type_spelling(canonical_type);
      // For builtin types, the spelling might include qualifiers like "const int" or "unsigned int"
      // Extract just the base type name (the last word, which is the actual type)
      // For "int", "unsigned int", "const int", etc., we want to extract "int"
      size_t last_space = type_name.find_last_of(" \t");
      if (last_space != std::string::npos && last_space + 1 < type_name.length()) {
        type_name = type_name.substr(last_space + 1);
      }
      
      // Add builtin type to global scope if it doesn't exist
      // We check for common builtin type names, but don't hardcode a full list
      // The type_name should be a valid C++ builtin type name
      if (!type_name.empty() && this->global_scope_) {
        auto global_scope = this->global_scope_;
        // Only add if it's not already there and looks like a builtin type name
        // (single word, all lowercase or common C++ type names)
        if (global_scope->members.find(type_name) == global_scope->members.end() &&
            (type_name == "int" || type_name == "float" || type_name == "double" || 
             type_name == "char" || type_name == "bool" || type_name == "void" ||
             type_name == "short" || type_name == "long")) {
          // Create a type definition for the builtin type
          // Use a null cursor since builtin types don't have a cursor
          auto type_def = std::make_shared<ClangDefinition>(
            type_name, global_scope.get(), jdi::DEF_TYPENAME, clang_getNullCursor());
          global_scope->members[type_name] = type_def;
          
          // If this is "int", also update the builtin_type__int global variable
          if (type_name == "int") {
            jdi::builtin_type__int = type_def.get();
          }
        }
      }
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
[[maybe_unused]] static enigma::parsing::TokenVector tokens_from_clang(CXTranslationUnit tu, CXSourceRange range) {
  enigma::parsing::TokenVector result;
  
  CXToken* tokens = nullptr;
  unsigned num_tokens = 0;
  clang_tokenize(tu, range, &tokens, &num_tokens);
  
  for (unsigned i = 0; i < num_tokens; ++i) {
    CXString spelling = clang_getTokenSpelling(tu, tokens[i]);
    std::string token_str = clang_getCString(spelling);
    CXTokenKind kind = clang_getTokenKind(tokens[i]);
    clang_disposeString(spelling);
    
    // Map clang token kinds to enigma token types
    enigma::parsing::TokenType token_type = enigma::parsing::TT_IDENTIFIER;
    switch (kind) {
      case CXToken_Identifier:
        token_type = enigma::parsing::TT_IDENTIFIER;
        break;
      case CXToken_Keyword:
        // Keywords need to be mapped individually based on spelling
        // For now, treat as identifier - full keyword mapping would require
        // checking token_str against keyword list
        token_type = enigma::parsing::TT_IDENTIFIER;
        break;
      case CXToken_Literal:
        // Literals need to be classified further (numeric, string, char)
        // For now, use a generic literal type
        if (!token_str.empty() && (token_str.front() == '"' || token_str.front() == '\'')) {
          token_type = (token_str.front() == '"') ? 
              enigma::parsing::TT_STRINGLIT : enigma::parsing::TT_CHARLIT;
        } else {
          // Numeric literal - could be further classified
          token_type = enigma::parsing::TT_DECLITERAL;
        }
        break;
      case CXToken_Punctuation:
        // Punctuation needs to be mapped based on spelling
        // This is a simplified mapping - full implementation would check token_str
        token_type = enigma::parsing::TT_IDENTIFIER;  // Placeholder
        break;
      case CXToken_Comment:
        // Comments are typically filtered out, but if present, treat as whitespace
        token_type = enigma::parsing::TT_IDENTIFIER;  // Placeholder
        break;
    }
    
    enigma::parsing::CodeSnippet snippet;
    snippet.content = token_str;
    snippet.line = 0;
    snippet.position = 0;
    enigma::parsing::Token token(token_type, snippet);
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
    
    static enum CXChildVisitResult visit(CXCursor cursor, CXCursor /* parent */, CXClientData client_data) {
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
                // Check for variadic parameter: "..." or three consecutive "." tokens
                if (token == "..." || token == ".") {
                  // Check if this is actually "..." (three dots)
                  if (token == "." && i + 2 < num_tokens) {
                    CXString next1 = clang_getTokenSpelling(visitor->tu, tokens[i+1]);
                    CXString next2 = clang_getTokenSpelling(visitor->tu, tokens[i+2]);
                    std::string next1_str = clang_getCString(next1);
                    std::string next2_str = clang_getCString(next2);
                    clang_disposeString(next1);
                    clang_disposeString(next2);
                    if (next1_str == "." && next2_str == ".") {
                      is_variadic = true;
                      i += 2; // Skip the next two tokens
                      break;
                    }
                  } else if (token == "...") {
                    is_variadic = true;
                    break;
                  }
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
            const char* token_cstr = clang_getCString(token_str);
            if (!token_cstr) {
              clang_disposeString(token_str);
              continue;
            }
            std::string token(token_cstr);
            clang_disposeString(token_str);
            
            // Only skip truly empty tokens - keep all punctuation, identifiers, etc.
            // Clang tokenizes operators like % as separate tokens, so we need to keep them
            if (!token.empty() && token != "\n" && token != "\r") {
              // Record position and add to content string
              size_t start = macro_content.length();
              if (!macro_content.empty()) {
                macro_content += " ";
                start++;
              }
              macro_content += token;
              token_positions.push_back({start, token.length()});
            }
          }
          
          clang_disposeTokens(visitor->tu, tokens, num_tokens);
          
          // Create macro using the string-based constructor so it owns the string
          enigma::parsing::StdErrorHandler err_handler;
          std::unique_ptr<enigma::parsing::Macro> macro;
          
          if (is_function) {
            // For function-like macros, tokenize the content string properly
            // This ensures parentheses and operators get correct token types
            auto owned_string = std::make_shared<std::string>(std::move(macro_content));
            // Tokenize using the lexer to get correct token types (like ParseTokens does)
            enigma::parsing::TokenVector value_tokens;
            enigma::parsing::Lexer lex(owned_string, &enigma::parsing::ParseContext::ForPreprocessorEvaluation(), &err_handler);
            lex.UseCppOptions();
            for (auto t = lex.ReadToken(); t.type != enigma::parsing::TT_ENDOFCODE; t = lex.ReadToken()) {
              value_tokens.push_back(t);
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

std::string ClangDefinition::toString() const {
  // For now, return qualified name
  // Can be enhanced later for function signatures, type representations, etc.
  return qualified_id();
}

} // namespace clang_adapter

// Initialize builtin flag constants
namespace jdi {
  // Define static instances with bit positions for each modifier
  // Using unique bit positions: 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, etc.
  flag_placeholder flag_const(1, 1);
  flag_placeholder flag_static(2, 2);
  flag_placeholder flag_volatile(4, 4);
  flag_placeholder flag_mutable(8, 8);
  flag_placeholder flag_register(16, 16);
  flag_placeholder flag_inline(32, 32);
  flag_placeholder flag_Complex(64, 64);
  flag_placeholder flag_unsigned(128, 128);
  flag_placeholder flag_signed(256, 256);
  flag_placeholder flag_short(512, 512);
  flag_placeholder flag_long(1024, 1024);
  flag_placeholder flag_long_long(2048, 2048);
  flag_placeholder flag_restrict(4096, 4096);
  flag_placeholder typeflag_override(8192, 8192);
  flag_placeholder typeflag_final(16384, 16384);
  flag_placeholder flag_virtual(32768, 32768);
  flag_placeholder flag_explicit(65536, 65536);
  
  // Pointers to static instances for backward compatibility
  flag_placeholder* builtin_flag__const = &flag_const;
  flag_placeholder* builtin_flag__static = &flag_static;
  flag_placeholder* builtin_flag__volatile = &flag_volatile;
  flag_placeholder* builtin_flag__mutable = &flag_mutable;
  flag_placeholder* builtin_flag__register = &flag_register;
  flag_placeholder* builtin_flag__inline = &flag_inline;
  flag_placeholder* builtin_flag__Complex = &flag_Complex;
  flag_placeholder* builtin_flag__unsigned = &flag_unsigned;
  flag_placeholder* builtin_flag__signed = &flag_signed;
  flag_placeholder* builtin_flag__short = &flag_short;
  flag_placeholder* builtin_flag__long = &flag_long;
  flag_placeholder* builtin_flag__long_long = &flag_long_long;
  flag_placeholder* builtin_flag__restrict = &flag_restrict;
  flag_placeholder* builtin_typeflag__override = &typeflag_override;
  flag_placeholder* builtin_typeflag__final = &typeflag_final;
  flag_placeholder* builtin_flag__virtual = &flag_virtual;
  flag_placeholder* builtin_flag__explicit = &flag_explicit;
  
  // Type definitions - initialized later in lang_CPP constructor
  definition* builtin_type__int = nullptr;
}
