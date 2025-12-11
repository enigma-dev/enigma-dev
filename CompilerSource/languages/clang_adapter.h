/**
 * @file clang_adapter.h
 * @brief Clang adapter that replaces JDI Context
 * 
 * This provides a clang-based replacement for JDI's Context class,
 * using libclang to parse C++ headers and extract type information.
 */

#ifndef ENIGMA_CLANG_ADAPTER_H
#define ENIGMA_CLANG_ADAPTER_H

#include "clang_definitions.h"
#include <clang-c/Index.h>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <functional>

// Forward declare for macro translation
namespace enigma {
  namespace parsing {
    class Macro;
    class ErrorHandler;
  }
}

namespace clang_adapter {

/**
 * ClangContext - replacement for jdi::Context
 * Parses C++ files using libclang and provides lookup functionality
 */
class ClangContext {
public:
  ClangContext();
  ~ClangContext();
  
  // Parse a file (replaces parse_stream)
  int parse_file(const std::string& filepath, 
                 const std::vector<std::string>& include_dirs = {},
                 const std::vector<std::string>& defines = {});
  
  // Get global scope (replaces get_global())
  ClangDefinitionScope* get_global() { return global_scope_.get(); }
  
  // Get global scope as shared_ptr (for use with TraversalState)
  std::shared_ptr<ClangDefinitionScope> get_global_shared() { return global_scope_; }
  
  // Look up a definition by name in global scope
  ClangDefinition* look_up(const std::string& name);
  
  // Get macros (replaces get_macros())
  std::map<std::string, std::unique_ptr<enigma::parsing::Macro>> get_macros();
  
  // Add include directory
  void add_include_dir(const std::string& dir);
  
  // Add preprocessor define
  void add_define(const std::string& name, const std::string& value = "");
  
  // Set namespace filter for function printing (empty string = print all)
  void set_namespace_filter(const std::string& namespace_name) { namespace_filter_ = namespace_name; }

private:
  CXIndex index_;
  CXTranslationUnit tu_;
  std::shared_ptr<ClangDefinitionScope> global_scope_;
  std::vector<std::string> include_dirs_;
  std::vector<std::string> defines_;
  // Store shared strings to keep them alive for string_view references in macro tokens
  std::vector<std::shared_ptr<std::string>> macro_token_strings_storage_;
  // Namespace filter for function printing (empty = print all)
  std::string namespace_filter_;
  
  // Build command line arguments for clang
  std::vector<const char*> build_args();
  
  // Traverse AST and build definition tree
  void build_definitions();
  
  // Visitor callback for AST traversal
  static enum CXChildVisitResult visit_cursor(CXCursor cursor, CXCursor parent, CXClientData client_data);
  
  // Process a cursor and add to scope
  // Takes a function to re-fetch the scope as shared_ptr to avoid use-after-free
  void process_cursor(CXCursor cursor, std::function<std::shared_ptr<ClangDefinitionScope>()> get_scope);
  
  // Extract macros from translation unit
  void extract_macros();
  
  // Storage for extracted macros
  std::map<std::string, std::unique_ptr<enigma::parsing::Macro>> macros_;
};

// Helper to get qualified name from cursor (free function)
std::string get_qualified_name(CXCursor cursor);

} // namespace clang_adapter

// Compatibility: make ClangContext usable as jdi::Context replacement
namespace jdi {
  typedef clang_adapter::ClangContext Context;
}

#endif // ENIGMA_CLANG_ADAPTER_H
