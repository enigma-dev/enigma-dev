/**
 * @file clang_adapter.h
 * @brief Clang adapter that replaces JDI Context
 *
 * This provides a clang-based replacement for JDI's Context class,
 * using libclang to parse C++ headers and extract type information.
 *
 * JDI2 T1: the adapter populates real jdi::Storage structures (definition,
 * definition_scope, definition_class, definition_typed, definition_function)
 * directly -- see CompilerSource/JDI/JDI2-DESIGN.md, "T1 seam". There is no
 * longer a parallel ClangDefinition* mirror.
 */

#ifndef ENIGMA_CLANG_ADAPTER_H
#define ENIGMA_CLANG_ADAPTER_H

#include <clang-c/Index.h>
#include <string>
#include <map>
#include <unordered_map>
#include <memory>
#include <vector>
#include <functional>

#include <Storage/definition.h>

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
  jdi::definition_scope* get_global() { return global_scope_.get(); }

  // Look up a definition by name in global scope
  jdi::definition* look_up(const std::string& name);

  // Get macros (replaces get_macros())
  std::map<std::string, std::unique_ptr<enigma::parsing::Macro>> get_macros();

  // Add include directory
  void add_include_dir(const std::string& dir);

  // Add quote include directory (for -iquote, searched before -I for "" includes)
  void add_quote_include_dir(const std::string& dir);

  // Add preprocessor define
  void add_define(const std::string& name, const std::string& value = "");

  // Set namespace filter for function printing (empty string = print all)
  void set_namespace_filter(const std::string& namespace_name) { namespace_filter_ = namespace_name; }

private:
  CXIndex index_;
  CXTranslationUnit tu_;
  std::unique_ptr<jdi::definition_scope> global_scope_;
  std::vector<std::string> include_dirs_;
  std::vector<std::string> quote_include_dirs_;  // For -iquote (quote includes)
  std::vector<std::string> defines_;
  // Store shared strings to keep them alive for string_view references in macro tokens
  std::vector<std::shared_ptr<std::string>> macro_token_strings_storage_;
  // Namespace filter for function printing (empty = print all)
  std::string namespace_filter_;
  // JDI2 T1: definitions we've populated, keyed by their originating cursor's
  // USR. Lets later cursors (e.g. a function parameter's type) resolve back
  // to the jdi::definition already built for that type, without a full
  // name-resolution pass. Best-effort: absent entries just mean "not
  // resolved yet" (def = nullptr in the caller).
  std::unordered_map<std::string, jdi::definition*> defs_by_usr_;

  // Build command line arguments for clang
  std::vector<const char*> build_args();
  // Backing storage for the system arguments of the latest build_args().
  std::vector<std::string> system_args_;

  // Initialize builtin primitive types (int, float, etc.)
  void init_builtin_types();

  // Traverse AST and build definition tree
  void build_definitions();

  // Visitor callback for AST traversal
  static enum CXChildVisitResult visit_cursor(CXCursor cursor, CXCursor parent, CXClientData client_data);

  // Process a cursor and add to scope
  // Takes a function to re-fetch the current scope (a plain pointer -- scopes
  // live in their parent's defmap for the lifetime of the ClangContext, so
  // there's no use-after-free hazard to guard against here).
  void process_cursor(CXCursor cursor, std::function<jdi::definition_scope*()> get_scope);

  // Record that `def` was populated from `cursor`, for later resolution by
  // resolve_type_def(). No-op if the cursor is null or has no USR.
  void register_usr(CXCursor cursor, jdi::definition* def);

  // Best-effort lookup of the jdi::definition already populated for a clang
  // type: fundamental types resolve by name in the global scope; other types
  // resolve via the declaring cursor's USR (see defs_by_usr_). Dependent
  // types have no declaring cursor; when `scope` is given they resolve by
  // spelling against enclosing template parameters. Returns nullptr if the
  // traversal hasn't (yet) populated a definition for it.
  jdi::definition* resolve_type_def(CXType type,
                                    jdi::definition_scope *scope = nullptr);

  // Extract macros from translation unit
  void extract_macros();

  // Storage for extracted macros
  std::map<std::string, std::unique_ptr<enigma::parsing::Macro>> macros_;
};

// Helper to get qualified name from cursor (free function)
std::string get_qualified_name(CXCursor cursor);

} // namespace clang_adapter

// T0: no jdi::Context alias here -- the real jdi::Context (JDI/src/API/context.h)
// still exists on this branch and would collide. Nothing references
// ClangContext as jdi::Context yet; T1 does the real retargeting.

#endif // ENIGMA_CLANG_ADAPTER_H
