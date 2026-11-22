/**
 * @file clang_adapter.cpp
 * @brief Implementation of clang adapter
 */

#include "clang_adapter.h"
#include "parsing/macros.h"
#include "parsing/lexer.h"
#include <System/builtins.h>
#include <clang-c/Index.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <unordered_set>
#include <cctype>

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
      flags |= jdi::DEF_CLASS | jdi::DEF_SCOPE | jdi::DEF_TYPENAME;
      break;
    case CXCursor_ClassTemplate:
      // No DEF_TYPENAME on the template wrapper: the lexer retypes any
      // DEF_TYPENAME definition to TT_TYPE_NAME, which bypasses the
      // identifier path that parses `<args>`. Template names must lex as
      // plain identifiers (the JDI contract); the pattern class inside the
      // definition_template carries DEF_TYPENAME instead.
      flags |= jdi::DEF_CLASS | jdi::DEF_SCOPE | jdi::DEF_TEMPLATE;
      break;
    case CXCursor_UnionDecl:
      flags |= jdi::DEF_UNION | jdi::DEF_SCOPE | jdi::DEF_TYPENAME;
      break;
    case CXCursor_EnumDecl:
      flags |= jdi::DEF_ENUM | jdi::DEF_TYPENAME;
      break;
    case CXCursor_FunctionDecl:
    case CXCursor_CXXMethod:
      flags |= jdi::DEF_FUNCTION;
      break;
    case CXCursor_FunctionTemplate:
      // T1: DEF_TEMPLATE only, same rationale as CXCursor_ClassTemplate above.
      flags |= jdi::DEF_FUNCTION | jdi::DEF_TEMPLATE;
      break;
    case CXCursor_TypedefDecl:
    case CXCursor_TypeAliasDecl:
      flags |= jdi::DEF_TYPENAME | jdi::DEF_TYPED;
      break;
    case CXCursor_TypeAliasTemplateDecl:
      // Template type aliases (like std::string might be in some implementations)
      flags |= jdi::DEF_TYPENAME | jdi::DEF_TYPED | jdi::DEF_TEMPLATE;
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
      // T1: template parameter cursors are not populated as definitions at
      // all (flags == 0 routes them through process_cursor's early return).
      // Template-param introspection is T3 (see JDI2-DESIGN.md).
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
  // Try to get the USR (Unified Symbol Resolution) which gives us the fully qualified name
  CXString usr = clang_getCursorUSR(cursor);
  const char* usr_str = clang_getCString(usr);
  std::string result;

  if (usr_str && strlen(usr_str) > 0) {
    // USR format is like "c:@N@std@N@__1@T@string" for std::__1::string
    // Parse it to extract the qualified name
    // For now, fall back to display name, but we can improve this later
    result = clang_getCString(clang_getCursorDisplayName(cursor));
  } else {
    // Fall back to display name
    CXString name = clang_getCursorDisplayName(cursor);
    result = clang_getCString(name);
    clang_disposeString(name);
  }

  clang_disposeString(usr);

  // If we still don't have a qualified name, try getting the type spelling
  if (result.empty() || result.find("::") == std::string::npos) {
    CXType type = clang_getCursorType(cursor);
    CXString type_str = clang_getTypeSpelling(type);
    const char* type_cstr = clang_getCString(type_str);
    if (type_cstr && strlen(type_cstr) > 0) {
      result = type_cstr;
    }
    clang_disposeString(type_str);
  }

  return result;
}

// Get type spelling (string representation of a type)
std::string get_type_spelling(CXType type) {
  CXString type_str = clang_getTypeSpelling(type);
  std::string result = clang_getCString(type_str);
  clang_disposeString(type_str);
  return result;
}

// Reduce a builtin type's canonical spelling ("unsigned int", "const int", ...)
// down to its bare keyword ("int"), matching the names init_builtin_types()
// registers in the global scope.
static std::string extract_builtin_name(CXType canonical) {
  std::string type_name = get_type_spelling(canonical);
  size_t last_space = type_name.find_last_of(" \t");
  if (last_space != std::string::npos && last_space + 1 < type_name.length()) {
    type_name = type_name.substr(last_space + 1);
  }
  return type_name;
}

// ErrorContext for JDI Storage APIs (definition_function::overload, etc.)
// that require one. The adapter has no EDL-side ErrorHandler of its own;
// jdi::default_error_handler (a static DefaultErrorHandler instance, always
// valid) is the same fallback JDI1 itself uses when no caller-supplied
// handler is available (see API/context.h's Context constructor default).
static jdi::ErrorContext default_error_context() {
  return jdi::default_error_handler->at({"clang_adapter", 0, 0});
}

// Population diagnostics, gated by ENIGMA_POPULATE_DEBUG so the counters
// cost nothing when unset. Investigates the second-parse flake: an identical
// TU (byte-identical clang diagnostics) sometimes populates a near-empty
// definition tree. Reports, per build_definitions() pass: cursors visited,
// definitions created, cursors skipped (and why), and a canary that catches
// the specific corruption under suspicion -- a definition* flagged
// DEF_TYPED whose dynamic type is not actually definition_typed, which
// would make arg_key::put_type's `(definition_typed*)` cast read past the
// end of a smaller allocation.
namespace {
struct PopulateDebugStats {
  long cursors_visited = 0;
  long defs_created = 0;
  long skipped_tu_or_unexposed = 0;
  long skipped_using_decl = 0;
  long skipped_zero_flags = 0;
  long skipped_empty_name = 0;
  long skipped_function_local = 0;
  long type_mismatches = 0;
} g_pop_stats;

bool populate_debug_enabled() {
  static const bool enabled = std::getenv("ENIGMA_POPULATE_DEBUG") != nullptr;
  return enabled;
}

// Canary: flag (but do not alter) any resolved type definition* that claims
// DEF_TYPED without actually being a definition_typed. Passing such a def to
// arg_key::put_type is undefined behavior (it reads ->type/->referencers
// past the real object) -- exactly the shape of the second-parse population
// flake (see the dedup guard in process_cursor's DEF_TYPED branch). `context`
// names the enclosing declaration being processed, for the rare case this
// ever fires again. Returns `def` unchanged either way.
jdi::definition* check_resolved_type(jdi::definition* def, const char* where,
                                      const std::string& context) {
  if (!populate_debug_enabled() || !def) return def;
  if ((def->flags & jdi::DEF_TYPED) && !dynamic_cast<jdi::definition_typed*>(def)) {
    ++g_pop_stats.type_mismatches;
    std::cerr << "POPDBG: TYPE MISMATCH at " << where << " (" << context
              << "): def=" << def << " name=\"" << def->name << "\" flags=0x"
              << std::hex << def->flags << std::dec
              << " -- DEF_TYPED set but dynamic_cast<definition_typed*> failed\n";
  }
  return def;
}
}  // namespace

// ClangContext implementation
// One CXIndex for the process: repeated clang_createIndex/clang_disposeIndex
// cycles trip once-only LLVM globals, and the SECOND parse in a process
// nondeterministically populated an empty tree (caught by the test harness,
// which rebuilds the context per settings change). The index leaks at
// process exit by design.
static CXIndex shared_index() {
  static CXIndex index = clang_createIndex(0, 0);
  return index;
}

ClangContext::ClangContext() : index_(nullptr), tu_(nullptr), namespace_filter_("") {
  index_ = shared_index();
  // JDI2 T1 seam: mirror how jdi::Context builds its own global scope --
  // definition_scope's default constructor gives name "", null parent,
  // DEF_SCOPE (see JDI/src/Storage/definition.cpp and API/context.cpp).
  global_scope_ = std::make_unique<jdi::definition_scope>();

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
    // No originating cursor for these -- `cursor` stays null-cursor default.
    auto type_def = std::make_unique<jdi::definition>(
      type_name, global_scope_.get(), jdi::DEF_TYPENAME);
    global_scope_->declare(type_name, std::move(type_def));

    // TODO(T2): adapter must not mutate JDI1 globals while JDI1 is primary.
    // (jdi::builtin_type__int is JDI1's own definition* global; wiring it to
    // this clang-populated definition only makes sense once T2 cuts lang_CPP
    // over to the clang-populated context.)
    // if (std::string(type_name) == "int") {
    //   jdi::builtin_type__int = global_scope_->members[type_name].get();
    // }
  }
}

ClangContext::~ClangContext() {
  if (tu_) {
    clang_disposeTranslationUnit(tu_);
  }
  // index_ is the process-wide shared index; never disposed (see
  // shared_index above).
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
  // Report every diagnostic: the default cap both hides errors and can
  // truncate population of everything past the limit.
  args.push_back("-ferror-limit=0");

  // Get system include paths (only compute once)
  if (system_include_strings.empty()) {
    // clang's builtin headers (stddef.h, feature-detection intrinsics) come
    // from its resource directory; without it, libstdc++'s config macros
    // misdetect and its own headers self-contradict (the constexpr
    // redeclaration errors in <array>/<algorithmfwd>).
    if (system_include_strings.empty()) {
      FILE* rd = popen("clang -print-resource-dir 2>/dev/null", "r");
      if (rd) {
        char buffer[512];
        if (fgets(buffer, sizeof(buffer), rd)) {
          std::string dir = buffer;
          while (!dir.empty() && (dir.back() == '\n' || dir.back() == ' ')) dir.pop_back();
          if (!dir.empty()) {
            system_include_strings.push_back("-resource-dir");
            system_include_strings.push_back(dir);
          }
        }
        pclose(rd);
      }
    }
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

      // Add Homebrew include path for MacOSX
      system_include_strings.push_back("-isystem");
      system_include_strings.push_back("/opt/homebrew/include/");

      // Build the args vector from strings (strings persist in static storage)
      for (const auto& str : system_include_strings) {
        system_include_args.push_back(str.c_str());
      }
    #elif __linux__
      // System include directories come from the ACTIVE compiler descriptor
      // (Compilers/Linux/<compiler>.ey's Parser-Vars: `searchdirs`/`defines`),
      // not from an independent popen scrape here. gcc_backend.cpp's
      // establish_bearings already runs that descriptor's `searchdirs`
      // command and registers the resulting directories into
      // jdi::builtin_context() (System/builtins.h) -- and it does so from
      // parse_ide_settings, the first thing lang_CPP::definitionsModified
      // does, always before main_context->parse_file() reaches build_args().
      // Reading the established list here (instead of re-querying clang++/g++
      // on our own) means one descriptor governs both the codegen pipeline
      // and this parse: point the IDE at a different .ey and both follow.
      jdi::Context &builtin = jdi::builtin_context();
      for (size_t i = 0; i < builtin.search_dir_count(); ++i) {
        system_include_strings.push_back("-isystem");
        system_include_strings.push_back(builtin.search_dir(i));
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

  // Find enigma root by looking for ENIGMAsystem in the filepath
  // Always use mock-headers directory for parse
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

// Helper structure to track scope during traversal.
// Scopes populated by this adapter live in their parent's `members` defmap
// (owned via unique_ptr) for the lifetime of the owning ClangContext, so a
// plain non-owning pointer is sufficient here -- no use-after-free hazard
// the way the old shared_ptr/weak_ptr scheme guarded against.
struct TraversalState {
  ClangContext* ctx;
  std::vector<jdi::definition_scope*> scope_stack;

  explicit TraversalState(ClangContext* c) : ctx(c) {
    scope_stack.push_back(ctx->get_global());
  }

  jdi::definition_scope* current_scope() const {
    return scope_stack.empty() ? ctx->get_global() : scope_stack.back();
  }

  void push_scope(jdi::definition_scope* scope) {
    if (scope) scope_stack.push_back(scope);
  }

  void pop_scope() {
    if (!scope_stack.empty()) scope_stack.pop_back();
  }
};

// Helper function to recursively find a class definition by name
static jdi::definition_class* find_class_in_scope(
    const std::string& class_name, jdi::definition_scope* scope) {
  if (!scope) return nullptr;

  auto it = scope->members.find(class_name);
  if (it != scope->members.end()) {
    if (auto* class_def = dynamic_cast<jdi::definition_class*>(it->second.get())) {
      return class_def;
    }
  }

  for (const auto& member : scope->members) {
    if (auto* child_scope = dynamic_cast<jdi::definition_scope*>(member.second.get())) {
      if (auto* found = find_class_in_scope(class_name, child_scope)) return found;
    }
  }

  return nullptr;
}

// Helper struct to track current class during traversal
struct AncestorTraversalState {
  std::string current_class;
  jdi::definition_scope* global_scope;
};

// Helper struct for processing using declarations in a second pass
struct UsingDeclState {
  ClangContext* ctx;
  TraversalState* traversal_state;
};

static enum CXChildVisitResult process_using_declarations_visitor(CXCursor cursor, CXCursor /* parent */, CXClientData client_data) {
  UsingDeclState* using_state = static_cast<UsingDeclState*>(client_data);
  CXCursorKind kind = clang_getCursorKind(cursor);

  if (kind != CXCursor_UsingDeclaration) {
    return CXChildVisit_Recurse;
  }

  std::string using_name = get_cursor_name(cursor);
  CXCursor referenced = clang_getCursorReferenced(cursor);
  if (using_name.empty() || clang_Cursor_isNull(referenced)) {
    return CXChildVisit_Recurse;
  }

  // Get the qualified name of what's being referenced to help determine the target namespace
  std::string ref_qualified = get_qualified_name(referenced);

  if (ref_qualified.empty() || ref_qualified == using_name) {
    CXType ref_type = clang_getCursorType(referenced);
    CXString type_spelling = clang_getTypeSpelling(ref_type);
    if (const char* type_str = clang_getCString(type_spelling)) {
      ref_qualified = type_str;
    }
    clang_disposeString(type_spelling);
  }

  if (ref_qualified.empty() || ref_qualified == using_name) {
    CXString display_name = clang_getCursorDisplayName(referenced);
    if (const char* display_str = clang_getCString(display_name)) {
      ref_qualified = display_str;
    }
    clang_disposeString(display_name);
  }

  // Check if the referenced name contains "std::" to infer we're in std namespace
  bool likely_in_std = (ref_qualified.find("std::") == 0) ||
                       (ref_qualified.find("::std::") != std::string::npos);

  CXSourceLocation loc = clang_getCursorLocation(cursor);
  bool is_system = clang_Location_isInSystemHeader(loc);

  // Find the scope where this using declaration is by traversing up the parent chain
  // Check both semantic and lexical parents to find the namespace
  std::string target_namespace;
  CXCursor walk = cursor;
  for (int i = 0; i < 10; ++i) {  // Limit depth
    CXCursor semantic_parent = clang_getCursorSemanticParent(walk);
    CXCursor lexical_parent = clang_getCursorLexicalParent(walk);

    if (!clang_Cursor_isNull(semantic_parent)) {
      CXCursorKind parent_kind = clang_getCursorKind(semantic_parent);
      if (parent_kind == CXCursor_Namespace || parent_kind == CXCursor_NamespaceAlias) {
        std::string parent_name = get_cursor_name(semantic_parent);
        if (!parent_name.empty()) {
          target_namespace = parent_name;
          break;
        }
      }
    }

    if (!clang_Cursor_isNull(lexical_parent)) {
      CXCursorKind parent_kind = clang_getCursorKind(lexical_parent);
      if (parent_kind == CXCursor_Namespace || parent_kind == CXCursor_NamespaceAlias) {
        std::string parent_name = get_cursor_name(lexical_parent);
        if (!parent_name.empty()) {
          target_namespace = parent_name;
          break;
        }
      }
    }

    if (!clang_Cursor_isNull(semantic_parent)) {
      walk = semantic_parent;
    } else if (!clang_Cursor_isNull(lexical_parent)) {
      walk = lexical_parent;
    } else {
      break;
    }
  }

  jdi::definition_scope* global_scope = using_state->ctx->get_global();
  if (!global_scope) {
    return CXChildVisit_Recurse;
  }

  // Try to find the target scope in our definition tree
  jdi::definition_scope* target_scope = nullptr;
  if (!target_namespace.empty()) {
    jdi::definition* ns_def = global_scope->look_up(target_namespace);
    if (ns_def && (ns_def->flags & jdi::DEF_NAMESPACE)) {
      target_scope = dynamic_cast<jdi::definition_scope*>(ns_def);
    }
  }
  // If namespace detection failed but we're in a system header and the referenced
  // name suggests it's from std, try "std" as a fallback.
  if (!target_scope && is_system && likely_in_std) {
    jdi::definition* std_def = global_scope->look_up("std");
    if (std_def && (std_def->flags & jdi::DEF_NAMESPACE)) {
      target_scope = dynamic_cast<jdi::definition_scope*>(std_def);
    }
  }

  if (!target_scope || target_scope->members.find(using_name) != target_scope->members.end()) {
    return CXChildVisit_Recurse;
  }

  // Try to find the referenced definition
  jdi::definition* ref_def = nullptr;
  if (!ref_qualified.empty()) {
    ref_def = global_scope->look_up(ref_qualified);
  }

  if (!ref_def && !ref_qualified.empty()) {
    // Split the qualified name by "::" and traverse namespaces
    std::vector<std::string> parts;
    size_t start = 0;
    while (start < ref_qualified.length()) {
      size_t colon = ref_qualified.find("::", start);
      if (colon == std::string::npos) {
        parts.push_back(ref_qualified.substr(start));
        break;
      }
      parts.push_back(ref_qualified.substr(start, colon - start));
      start = colon + 2;
    }

    if (parts.size() >= 2) {
      jdi::definition* current = global_scope;
      bool found_path = true;
      for (size_t i = 0; i + 1 < parts.size(); ++i) {
        if (!current || !(current->flags & jdi::DEF_NAMESPACE)) {
          found_path = false;
          break;
        }
        auto* scope = dynamic_cast<jdi::definition_scope*>(current);
        if (!scope) {
          found_path = false;
          break;
        }
        jdi::definition* next = scope->look_up(parts[i]);
        if (!next) {
          found_path = false;
          break;
        }
        current = next;
      }
      if (found_path && current && (current->flags & jdi::DEF_NAMESPACE)) {
        if (auto* final_scope = dynamic_cast<jdi::definition_scope*>(current)) {
          ref_def = final_scope->look_up(parts.back());
        }
      }
    } else if (parts.size() == 1) {
      ref_def = target_scope->look_up(parts[0]);
    }
  }

  if (!ref_def) {
    return CXChildVisit_Recurse;
  }

  // Add the definition to the target scope
  std::unique_ptr<jdi::definition> using_def;
  if (ref_def->flags & jdi::DEF_TYPENAME) {
    using_def = std::make_unique<jdi::definition_typed>(
        using_name, target_scope, /*tp=*/nullptr, /*typeflags=*/0, ref_def->flags);
  } else {
    using_def = std::make_unique<jdi::definition>(using_name, target_scope, ref_def->flags);
  }
  target_scope->declare(using_name, std::move(using_def));

  return CXChildVisit_Recurse;
}

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
    }
  }

  // Process base class specifiers
  if (kind == CXCursor_CXXBaseSpecifier) {
    CXType base_type = clang_getCursorType(cursor);
    std::string base_name = get_type_spelling(base_type);
    std::string class_name = state->current_class;

    // Remove namespace prefix if present (e.g., "enigma::object_transform" -> "object_transform")
    std::string base_name_short = base_name;
    size_t ns_pos = base_name_short.find_last_of("::");
    if (ns_pos != std::string::npos && ns_pos + 1 < base_name_short.length()) {
      base_name_short = base_name_short.substr(ns_pos + 1);
    }

    if (!class_name.empty() && !base_name_short.empty()) {
      auto* class_def = find_class_in_scope(class_name, state->global_scope);
      auto* base_class = find_class_in_scope(base_name_short, state->global_scope);

      if (class_def && base_class) {
        unsigned protection = 0;
        switch (clang_getCXXAccessSpecifier(cursor)) {
          case CX_CXXProtected: protection = jdi::DEF_PROTECTED; break;
          case CX_CXXPrivate: protection = jdi::DEF_PRIVATE; break;
          default: protection = 0; break;  // public, or unspecified
        }
        class_def->ancestors.emplace_back(protection, base_class);
      }
    }
  }

  return CXChildVisit_Recurse;
}

void ClangContext::build_definitions() {
  if (!tu_) return;

  if (populate_debug_enabled()) g_pop_stats = PopulateDebugStats{};

  CXCursor root = clang_getTranslationUnitCursor(tu_);

  // Create traversal state
  TraversalState state(this);

  // First pass: Visit all children of the translation unit to create definitions
  clang_visitChildren(root, visit_cursor, &state);

  // Second pass: Populate ancestors by visiting base class specifiers
  AncestorTraversalState pass2_state;
  pass2_state.global_scope = global_scope_.get();
  clang_visitChildren(root, populate_ancestors_visitor, &pass2_state);

  // Third pass: Process using declarations now that all definitions are built
  // This ensures that referenced definitions exist when we process using declarations
  UsingDeclState using_state = {this, &state};
  clang_visitChildren(root, process_using_declarations_visitor, &using_state);

  if (populate_debug_enabled()) {
    std::cerr << "POPDBG build_definitions: cursors_visited=" << g_pop_stats.cursors_visited
              << " defs_created=" << g_pop_stats.defs_created
              << " skip[tu/unexposed]=" << g_pop_stats.skipped_tu_or_unexposed
              << " skip[using_decl]=" << g_pop_stats.skipped_using_decl
              << " skip[zero_flags]=" << g_pop_stats.skipped_zero_flags
              << " skip[empty_name]=" << g_pop_stats.skipped_empty_name
              << " skip[function_local]=" << g_pop_stats.skipped_function_local
              << " type_mismatches=" << g_pop_stats.type_mismatches
              << " global_members=" << (global_scope_ ? global_scope_->members.size() : 0)
              << std::endl;
  }
}

enum CXChildVisitResult ClangContext::visit_cursor(CXCursor cursor, CXCursor /* parent */, CXClientData client_data) {
  TraversalState* state = static_cast<TraversalState*>(client_data);
  ClangContext* ctx = state->ctx;

  if (populate_debug_enabled()) ++g_pop_stats.cursors_visited;

  jdi::definition_scope* target_scope = state->current_scope();

  // Safety check: ensure scope is valid
  if (!target_scope) {
    return CXChildVisit_Continue;
  }

  // Process the cursor - pass a lambda that re-fetches the current scope
  ctx->process_cursor(cursor, [state]() { return state->current_scope(); });

  // If this cursor creates a new scope, push it
  CXCursorKind kind = clang_getCursorKind(cursor);
  unsigned int flags = cursor_kind_to_flags(kind);

  if (flags & jdi::DEF_SCOPE) {
    std::string name = get_cursor_name(cursor);
    if (!name.empty()) {
      target_scope = state->current_scope();
      if (target_scope) {
        auto it = target_scope->members.find(name);
        if (it != target_scope->members.end() && it->second) {
          auto* new_scope = dynamic_cast<jdi::definition_scope*>(it->second.get());
          // Class-template members belong in the pattern class (the `def`
          // instantiate() duplicates), not the template's own scope.
          if (kind == CXCursor_ClassTemplate) {
            if (auto* t = dynamic_cast<jdi::definition_template*>(it->second.get()))
              new_scope = dynamic_cast<jdi::definition_scope*>(t->def.get());
          }
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

void ClangContext::register_usr(CXCursor cursor, jdi::definition* def) {
  if (clang_Cursor_isNull(cursor) || !def) return;
  CXString usr = clang_getCursorUSR(cursor);
  if (const char* usr_str = clang_getCString(usr); usr_str && *usr_str) {
    defs_by_usr_[usr_str] = def;
  }
  clang_disposeString(usr);
}

// libclang exposes no declaration for a dependent type (a field of type T
// inside a class template resolves to CXType_Unexposed and
// CXCursor_NoDeclFound), so USR lookup cannot reach the tempparam. The
// canonical spelling encodes the coordinates instead:
// `type-parameter-<depth>-<index>`, depth counting template-parameter lists
// from the outermost. Map those onto the definition_templates enclosing
// `scope`; instantiate()'s remap keys its substitution map on the returned
// tempparam pointers.
static jdi::definition* resolve_enclosing_tempparam(CXType resolved,
                                                    jdi::definition_scope *scope) {
  if (resolved.kind != CXType_Unexposed || !scope) return nullptr;
  CXString ts = clang_getTypeSpelling(resolved);
  const char *ts_c = clang_getCString(ts);
  std::string spelling = ts_c ? ts_c : "";
  clang_disposeString(ts);
  for (std::string_view prefix : {"const ", "volatile "}) {
    if (spelling.rfind(prefix, 0) == 0) spelling.erase(0, prefix.size());
  }
  unsigned depth, index;
  if (sscanf(spelling.c_str(), "type-parameter-%u-%u", &depth, &index) != 2)
    return nullptr;

  std::vector<jdi::definition_template*> enclosing;  // innermost first
  for (jdi::definition *s = scope; s; s = s->parent)
    if (auto *t = dynamic_cast<jdi::definition_template*>(s))
      enclosing.push_back(t);
  if (depth >= enclosing.size()) return nullptr;
  jdi::definition_template *t = enclosing[enclosing.size() - 1 - depth];
  if (index >= t->params.size()) return nullptr;
  return t->params[index].get();
}

jdi::definition* ClangContext::resolve_type_def(CXType type,
                                                jdi::definition_scope *scope) {
  CXType canonical = clang_getCanonicalType(type);

  // Fundamental types have no declaring cursor/USR; resolve by the name
  // init_builtin_types() registered in the global scope.
  if (canonical.kind >= CXType_FirstBuiltin && canonical.kind <= CXType_LastBuiltin) {
    std::string name = extract_builtin_name(canonical);
    if (!global_scope_) return nullptr;
    auto it = global_scope_->members.find(name);
    return it == global_scope_->members.end() ? nullptr : it->second.get();
  }

  // References and pointers have no declaring cursor; the pointee does.
  // The varargs convention passes `const enigma::varargs&`, and the
  // positional scan compares the parameter's def against the varargs
  // class definition, so resolve through to the underlying record.
  CXType resolved = canonical;
  while (resolved.kind == CXType_LValueReference ||
         resolved.kind == CXType_RValueReference ||
         resolved.kind == CXType_Pointer) {
    resolved = clang_getPointeeType(resolved);
  }

  CXCursor decl = clang_getTypeDeclaration(resolved);
  if (clang_Cursor_isNull(decl)) return resolve_enclosing_tempparam(resolved, scope);

  CXString usr = clang_getCursorUSR(decl);
  const char* usr_c = clang_getCString(usr);
  std::string usr_str = usr_c ? usr_c : "";
  clang_disposeString(usr);
  if (usr_str.empty()) return resolve_enclosing_tempparam(resolved, scope);

  auto it = defs_by_usr_.find(usr_str);
  if (it != defs_by_usr_.end()) return it->second;
  return resolve_enclosing_tempparam(resolved, scope);
}

void ClangContext::process_cursor(CXCursor cursor, std::function<jdi::definition_scope*()> get_scope) {
  // Re-fetch scope right before use to avoid use-after-free
  jdi::definition_scope* scope = get_scope();

  // Safety check: scope must be valid
  if (!scope) {
    return;
  }

  CXCursorKind kind = clang_getCursorKind(cursor);

  // Skip certain cursor kinds
  if (kind == CXCursor_TranslationUnit ||
      kind == CXCursor_UnexposedDecl ||
      kind == CXCursor_FirstInvalid) {
    if (populate_debug_enabled()) ++g_pop_stats.skipped_tu_or_unexposed;
    return;
  }

  // Handle using declarations - these are now processed in a third pass after all definitions are built
  // Skip them here to avoid processing them before referenced definitions exist
  if (kind == CXCursor_UsingDeclaration) {
    if (populate_debug_enabled()) ++g_pop_stats.skipped_using_decl;
    return;  // Don't process using declarations as regular definitions - handled in third pass
  }

  std::string name = get_cursor_name(cursor);

  unsigned int flags = cursor_kind_to_flags(kind);
  if (flags == 0) {
    // Not a definition we care about. Also covers TemplateTypeParameter/
    // NonTypeTemplateParameter cursors, which T1 skips outright (see
    // cursor_kind_to_flags).
    if (populate_debug_enabled()) ++g_pop_stats.skipped_zero_flags;
    return;
  }

  // For variable declarations, we need to extract builtin types even if name is empty
  // (though typically variable declarations have names)
  // Type aliases and typedefs with DEF_TYPENAME must have a name to be useful
  if (name.empty()) {
    // Allow through only if it's a typed definition that might have special handling
    // But type aliases and typedefs (DEF_TYPENAME) must have names
    if (!(flags & jdi::DEF_TYPED) || (flags & jdi::DEF_TYPENAME)) {
      if (populate_debug_enabled()) ++g_pop_stats.skipped_empty_name;
      return;
    }
  }

  // Re-fetch scope again right before accessing members to ensure it's still valid
  scope = get_scope();
  if (!scope) {
    return;
  }

  if (flags & jdi::DEF_SCOPE) {
    // For scopes, check if already exists (don't create duplicates)
    if (scope->members.find(name) != scope->members.end()) {
      return;  // Already added
    }

    std::unique_ptr<jdi::definition> def;
    if (kind == CXCursor_ClassTemplate) {
      // Class templates get the real definition_template: the parser's
      // TryParseTemplateArgs casts to it, reads params, and calls
      // instantiate() -- a plain definition_class there is an out-of-bounds
      // read. `def` holds the pattern class the members populate into and
      // instantiate() duplicates; params come from the parameter cursors
      // (type params carry DEF_TYPENAME, the kind test the parser uses).
      auto tmpl = std::make_unique<jdi::definition_template>(name, scope, flags);
      tmpl->def = std::make_unique<jdi::definition_class>(
          name, tmpl.get(), jdi::DEF_CLASS | jdi::DEF_TYPENAME);
      tmpl->def->cursor = cursor;
      struct ParamVisit { ClangContext *ctx; jdi::definition_template *tmpl; };
      ParamVisit pv{this, tmpl.get()};
      clang_visitChildren(
          cursor,
          [](CXCursor child, CXCursor, CXClientData data) {
            auto *v = static_cast<ParamVisit*>(data);
            CXCursorKind ck = clang_getCursorKind(child);
            if (ck == CXCursor_TemplateTypeParameter ||
                ck == CXCursor_NonTypeTemplateParameter) {
              unsigned pflags = jdi::DEF_TEMPPARAM | jdi::DEF_DEPENDENT;
              if (ck == CXCursor_TemplateTypeParameter)
                pflags |= jdi::DEF_TYPENAME;
              auto param = std::make_unique<jdi::definition_tempparam>(
                  get_cursor_name(child), v->tmpl, pflags);
              param->cursor = child;
              // Register the parameter so dependent member types (a field of
              // type T, a T& return) resolve to the tempparam; instantiate()'s
              // remap keys the substitution map on those pointers.
              v->ctx->register_usr(child, param.get());
              v->tmpl->params.push_back(std::move(param));
            }
            return CXChildVisit_Continue;
          },
          &pv);
      def = std::move(tmpl);
    } else if (flags & jdi::DEF_CLASS) {
      def = std::make_unique<jdi::definition_class>(name, scope, flags);
    } else {
      def = std::make_unique<jdi::definition_scope>(name, scope, flags);
    }
    def->cursor = cursor;
    jdi::definition* raw = def.get();

    // Re-fetch scope before insertion in case it changed
    scope = get_scope();
    if (!scope) {
      return;
    }

    scope->declare(name, std::move(def));
    register_usr(cursor, raw);
    if (populate_debug_enabled()) ++g_pop_stats.defs_created;

    // Children are visited via the caller (visit_cursor), which pushes this
    // scope once it sees DEF_SCOPE and looks it back up from scope->members.

  } else if (flags & jdi::DEF_FUNCTION) {
    // Find-or-create the definition_function container for this name,
    // mirroring definition_scope::overload_function's own idiom (see
    // Storage/definition.cpp).
    scope = get_scope();
    if (!scope) {
      return;
    }

    jdi::decpair dp = scope->declare(name, nullptr);
    jdi::definition_function* func_def = nullptr;
    if (dp.inserted) {
      auto uf = std::make_unique<jdi::definition_function>(name, scope, flags);
      uf->cursor = cursor;
      func_def = uf.get();
      dp.def = std::move(uf);
      register_usr(cursor, func_def);
      if (populate_debug_enabled()) ++g_pop_stats.defs_created;
    } else {
      func_def = dynamic_cast<jdi::definition_function*>(dp.def.get());
      if (!func_def) {
        return;  // Name collision with a non-function; skip.
      }
    }

    CXType func_type = clang_getCursorType(cursor);
    int num_args = clang_Cursor_getNumArguments(cursor);
    // Function templates answer -1 and expose no argument cursors; their
    // ParmDecls are plain children.
    std::vector<CXCursor> template_parms;
    if (num_args < 0) {
      clang_visitChildren(
          cursor,
          [](CXCursor child, CXCursor, CXClientData out) {
            if (clang_getCursorKind(child) == CXCursor_ParmDecl)
              static_cast<std::vector<CXCursor>*>(out)->push_back(child);
            return CXChildVisit_Continue;
          },
          &template_parms);
      num_args = clang_getNumArgTypes(func_type);
      if (num_args < 0) {
        num_args = 0;
      }
    }

    // Build the RT_FUNCTION ref_stack for this overload's parameter list,
    // exactly as walk_declarator_expr (parsing/ast.cpp) builds one from an
    // AST function-call-shaped declarator: a parameter_ct of minimal
    // full_types, pushed as a single RT_FUNCTION node.
    jdi::ref_stack::parameter_ct params;
    for (int i = 0; i < num_args; ++i) {
      CXType arg_type = clang_getArgType(func_type, i);
      jdi::full_type arg_ft(
          check_resolved_type(resolve_type_def(arg_type, scope), "function parameter",
                               name + "#arg" + std::to_string(i)),
          jdi::ref_stack(), 0);
      // Defaulted parameters carry JDI's presence sentinel (an empty AST,
      // matching JDI1's own convention) so parameter-bound minima count
      // them optional. A ParmDecl with a default has the initializer
      // expression among its children; type references are not expressions.
      // The default's VALUE stays unevaluated -- that is thin-node
      // territory, and bounds only need presence.
      jdi::AST *default_sentinel = nullptr;
      CXCursor arg_cursor = size_t(i) < template_parms.size()
                                ? template_parms[i]
                                : clang_Cursor_getArgument(cursor, i);
      if (!clang_Cursor_isNull(arg_cursor)) {
        bool has_default = false;
        clang_visitChildren(
            arg_cursor,
            [](CXCursor child, CXCursor, CXClientData found) {
              if (clang_isExpression(clang_getCursorKind(child))) {
                *static_cast<bool*>(found) = true;
                return CXChildVisit_Break;
              }
              return CXChildVisit_Continue;
            },
            &has_default);
        if (has_default) default_sentinel = new jdi::AST();
      }
      jdi::ref_stack::parameter p(arg_ft, default_sentinel);
      params.throw_on(p);
    }
    jdi::ref_stack rf;
    rf.push_func(params);

    // C-style varargs only: clang_isFunctionTypeVariadic never reports
    // C++11 parameter packs (those are a per-parameter PackExpansionType,
    // handled with the template work).
    unsigned int addflags = 0;
    if (clang_isFunctionTypeVariadic(func_type)) {
      addflags |= jdi::DEF_VARIADIC;
    }

    CXType return_type = clang_getResultType(func_type);
    // CXType_Void is only the naked type; `void*` arrives as CXType_Pointer.
    if (return_type.kind == CXType_Void) {
      addflags |= jdi::DEF_RETURNS_VOID;
    }
    jdi::definition* return_def =
        check_resolved_type(resolve_type_def(return_type, scope), "function return type", name);

    jdi::definition_overload* ovl = func_def->overload(
        return_def, rf, /*typeflags=*/0, addflags,
        /*implementation=*/nullptr, default_error_context());
    if (ovl) {
      ovl->cursor = cursor;
    }

  } else if (flags & jdi::DEF_TYPED || kind == CXCursor_EnumConstantDecl) {
    // Handle typed definitions (variables, fields, enum constants)
    if (kind == CXCursor_EnumConstantDecl && !(flags & jdi::DEF_TYPED)) {
      flags |= jdi::DEF_TYPED;
    }

    // Filter out function-local variable declarations
    // These should not be added to namespace or class scopes as they are only visible within the function
    // Note: We only filter CXCursor_VarDecl, not CXCursor_FieldDecl (struct/class members)
    if (kind == CXCursor_VarDecl) {
      CXCursor parent_cursor = clang_getCursorSemanticParent(cursor);
      if (!clang_Cursor_isNull(parent_cursor)) {
        CXCursorKind parent_kind = clang_getCursorKind(parent_cursor);

        if (parent_kind == CXCursor_FunctionDecl ||
            parent_kind == CXCursor_CXXMethod ||
            parent_kind == CXCursor_FunctionTemplate) {
          // This is a function-local variable (including loop variables), skip it
          if (populate_debug_enabled()) ++g_pop_stats.skipped_function_local;
          return;
        }

        if (parent_kind == CXCursor_CompoundStmt) {
          // Walk up to find if this compound statement is inside a function
          CXCursor walk_cursor = parent_cursor;
          bool is_in_function = false;
          for (int i = 0; i < 10; ++i) {
            CXCursor walk_parent = clang_getCursorSemanticParent(walk_cursor);
            if (clang_Cursor_isNull(walk_parent)) break;

            CXCursorKind walk_parent_kind = clang_getCursorKind(walk_parent);
            if (walk_parent_kind == CXCursor_FunctionDecl ||
                walk_parent_kind == CXCursor_CXXMethod ||
                walk_parent_kind == CXCursor_FunctionTemplate) {
              is_in_function = true;
              break;
            }
            if (walk_parent_kind == CXCursor_TranslationUnit ||
                walk_parent_kind == CXCursor_Namespace ||
                walk_parent_kind == CXCursor_ClassDecl ||
                walk_parent_kind == CXCursor_StructDecl) {
              // Reached a scope boundary, not in a function
              break;
            }
            walk_cursor = walk_parent;
          }
          if (is_in_function) {
            // This variable is in a function body, skip it
            if (populate_debug_enabled()) ++g_pop_stats.skipped_function_local;
            return;
          }
        }
      }
    }

    // Re-fetch scope before insertion
    scope = get_scope();
    if (!scope) {
      return;
    }

    // A redeclaration of the same name in the same scope (a legitimate
    // extern-then-definition pair, or a same-named member merged in from a
    // second template instantiation via visit_cursor's scope-reuse -- see
    // the DEF_SCOPE branch above) must not overwrite the existing entry.
    // Unlike this branch, DEF_SCOPE and DEF_FUNCTION already dedup this way;
    // an unconditional `scope->members[name] = ...` here destroys the prior
    // definition_typed while defs_by_usr_ (or an already-resolved full_type
    // elsewhere) may still hold its address, a use-after-free that only
    // crashes when the freed memory is later reused with bits that happen
    // to look like a definition's vtable/flags. First declaration wins.
    if (!name.empty() && scope->members.find(name) != scope->members.end()) {
      return;
    }

    // Extract builtin type from type information and add it to global scope if it's a builtin
    // This ensures builtin types like int, float, etc. are always available
    CXType var_type = clang_getCursorType(cursor);
    CXType canonical_type = clang_getCanonicalType(var_type);

    if (canonical_type.kind >= CXType_FirstBuiltin && canonical_type.kind <= CXType_LastBuiltin) {
      std::string type_name = extract_builtin_name(canonical_type);
      if (!type_name.empty() && global_scope_ &&
          global_scope_->members.find(type_name) == global_scope_->members.end() &&
          (type_name == "int" || type_name == "float" || type_name == "double" ||
           type_name == "char" || type_name == "bool" || type_name == "void" ||
           type_name == "short" || type_name == "long")) {
        auto type_def = std::make_unique<jdi::definition>(
            type_name, global_scope_.get(), jdi::DEF_TYPENAME);
        global_scope_->declare(type_name, std::move(type_def));

        // TODO(T2): adapter must not mutate JDI1 globals while JDI1 is
        // primary (see init_builtin_types(); jdi::builtin_type__int is
        // JDI1's own global).
      }
    }

    // Create typed definition
    auto def = std::make_unique<jdi::definition_typed>(
        name, scope, check_resolved_type(resolve_type_def(var_type, scope), "variable/field type", name),
        /*typeflags=*/0, flags);
    def->cursor = cursor;
    jdi::definition_typed* raw = def.get();

    if (!name.empty()) {
      scope->declare(name, std::move(def));
      register_usr(cursor, raw);
      if (populate_debug_enabled()) ++g_pop_stats.defs_created;

      // Handle inline namespaces: if the current scope is an inline namespace,
      // also add the definition to the parent namespace
      if (scope->parent && (scope->flags & jdi::DEF_NAMESPACE) &&
          clang_Cursor_isInlineNamespace(scope->cursor)) {
        if (scope->parent->members.find(name) == scope->parent->members.end()) {
          std::unique_ptr<jdi::definition> parent_def;
          if (flags & jdi::DEF_TYPENAME) {
            parent_def = std::make_unique<jdi::definition_typed>(
                name, scope->parent, /*tp=*/nullptr, /*typeflags=*/0, flags);
          } else {
            parent_def = std::make_unique<jdi::definition>(name, scope->parent, flags);
          }
          scope->parent->declare(name, std::move(parent_def));
        }
      }
    }

  } else {
    // Re-fetch scope before insertion
    scope = get_scope();
    if (!scope) {
      return;
    }

    // Same redeclaration hazard as the DEF_TYPED branch above (e.g. a
    // forward-declared `enum class Foo;` later defined `enum class Foo {...};`
    // -- two cursors, same name, same scope): first declaration wins rather
    // than destroying-and-replacing, which would leave any already-captured
    // pointer to the first object dangling.
    if (!name.empty() && scope->members.find(name) != scope->members.end()) {
      return;
    }

    // Generic definition
    auto def = std::make_unique<jdi::definition>(name, scope, flags);
    def->cursor = cursor;
    jdi::definition* raw = def.get();
    scope->declare(name, std::move(def));
    register_usr(cursor, raw);
  }
}

jdi::definition* ClangContext::look_up(const std::string& name) {
  return global_scope_->look_up(name);
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

      // Note: CXCursor_PreprocessingDirective does NOT include #undef in libclang
      // We handle undefs separately after visiting all cursors
      if (kind == CXCursor_PreprocessingDirective) {
        return CXChildVisit_Continue;
      }

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
          unsigned param_end_idx = 1;  // Default: start after name for object-like macros

          // Look for '(' after macro name
          if (num_tokens > 1) {
            CXString second_token = clang_getTokenSpelling(visitor->tu, tokens[1]);
            std::string second_str = clang_getCString(second_token);
            clang_disposeString(second_token);

            // `#define f(x)` vs `#define v (x)`: only clang knows whether
            // whitespace separated the name from the paren.
            if (second_str == "(" && clang_Cursor_isMacroFunctionLike(cursor)) {
              is_function = true;
              // Extract parameters and track where ')' is found
              param_end_idx = 2;  // Start after '('
              for (unsigned i = 2; i < num_tokens; ++i) {
                CXString token_str = clang_getTokenSpelling(visitor->tu, tokens[i]);
                std::string token = clang_getCString(token_str);
                clang_disposeString(token_str);

                if (token == ")") {
                  param_end_idx = i;  // Track where ')' was found
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
                      param_end_idx = i + 2;  // Track past the "..."
                      // Continue to find the ')'
                      continue;
                    }
                  } else if (token == "...") {
                    is_variadic = true;
                    // Continue to find the ')'
                    continue;
                  }
                }
                params.push_back(token);
              }
            }
          }

          // Extract macro value (everything after name and params)
          // For function-like macros, start after the ')' that closes the parameter list
          // For object-like macros, start after the name
          unsigned start_idx = is_function ? param_end_idx + 1 : 1;

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

  // libclang doesn't expose #undef as cursors, so we need to scan included files
  // for #undef directives and remove those macros from our map
  struct UndefCollector {
    CXTranslationUnit tu;
    std::unordered_set<std::string> undefined_macros;
    std::unordered_set<std::string> scanned_files;

    static enum CXChildVisitResult visit(CXCursor cursor, CXCursor /* parent */, CXClientData client_data) {
      UndefCollector* collector = static_cast<UndefCollector*>(client_data);
      CXCursorKind kind = clang_getCursorKind(cursor);

      // Visit #include directives to scan their files for #undef
      if (kind == CXCursor_InclusionDirective) {
        CXFile included_file = clang_getIncludedFile(cursor);
        if (included_file) {
          CXString filename = clang_getFileName(included_file);
          std::string filename_str = clang_getCString(filename);
          clang_disposeString(filename);

          // Only scan each file once
          if (collector->scanned_files.find(filename_str) == collector->scanned_files.end()) {
            collector->scanned_files.insert(filename_str);

            size_t size = 0;
            const char* contents = clang_getFileContents(collector->tu, included_file, &size);
            if (contents && size > 0) {
              std::string file_contents(contents, size);
              collector->scan_for_undefs(file_contents);
            }
          }
        }
      }

      return CXChildVisit_Recurse;
    }

    void scan_for_undefs(const std::string& contents) {
      size_t pos = 0;
      while ((pos = contents.find("#undef", pos)) != std::string::npos) {
        // Skip "#undef"
        pos += 6;

        // Skip whitespace
        while (pos < contents.size() && (contents[pos] == ' ' || contents[pos] == '\t')) {
          pos++;
        }

        // Extract macro name
        size_t name_start = pos;
        while (pos < contents.size() && (std::isalnum(contents[pos]) || contents[pos] == '_')) {
          pos++;
        }

        if (pos > name_start) {
          std::string macro_name = contents.substr(name_start, pos - name_start);
          undefined_macros.insert(macro_name);
        }
      }
    }
  };

  UndefCollector undef_collector;
  undef_collector.tu = tu_;

  // Visit all includes to scan their files for undefs
  clang_visitChildren(root, UndefCollector::visit, &undef_collector);

  // Remove undefined macros from our map
  for (const auto& undef_name : undef_collector.undefined_macros) {
    macros_.erase(undef_name);
  }
}

} // namespace clang_adapter
