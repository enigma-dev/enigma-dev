#ifndef ENIGMA_COMPILER_PARSING_LANGUAGE_FRONTEND_h
#define ENIGMA_COMPILER_PARSING_LANGUAGE_FRONTEND_h

#include <frontend.h>
#include <Storage/definition.h>
#include <settings.h>

#include <string>

typedef set<string, std::less<>> NameSet;

// The main idea here is that ENIGMA's EDL can be pretty language-agnostic,
// despite inheriting some of C++'s traits. A language frontend tells ENIGMA
// what functions are available in some implementation of the engine, be it the
// main C++ engine or EGMJS. The backend is how ENIGMA emits code back into that
// language. So the backend requires information from the frontend to carry out
// a code generation operation. The frontend just requires some config settings.
class LanguageFrontend {
 public:
  // Called any time definitions from the engine must be reindexed.
  virtual syntax_error* definitionsModified(const char *wscode,
                                            const char *targetYaml) = 0;

  // Globals and locals

  /// Assuming definitions have already been loaded, iterate the object tier
  /// classes for locals belonging to all instances.
  virtual int  load_shared_locals() = 0;
  /// Assuming definitions have already been loaded, iterate the extension
  /// classes of enabled extensions for locals belonging to all instances.
  virtual void load_extension_locals() = 0;
  /// Returns whether a global variable by the given name has been defined.
  virtual bool global_exists(std::string name) const = 0;

  /// Retrieves a map of built-in macros, preprocessed into ENIGMA format.
  virtual const enigma::parsing::MacroMap &builtin_macros() const = 0;

  /// Retrieves the set of shared local variables, inherited by all instances.
  virtual const NameSet &shared_object_locals() const = 0;

  bool is_shared_local(std::string_view varname) const {
    const auto &shared = shared_object_locals();  // Cache for vtable skip
    return shared.find(varname) != shared.end();
  }

  // TODO: This doesn't really belong here, except definitionsModified is passed
  // the IDE settings right now... and putting it here meant ParseContext needed
  // only one parameter to its constructor.
  virtual const setting::CompatibilityOptions &compatibility_opts() const = 0;

  // TODO: This stuff all depends directly on JDI storage structures, rather
  // than on some ENIGMA-centric representation of these definitions.

  /// Returns whether the given definition is a variadic function--in ENIGMA's
  /// case, this means a function accepting `enigma::varargs` as its final arg.
  virtual bool is_variadic_function(jdi::definition *d) const = 0;
  /// Returns the index at which a function parameters ref_stack is variadic;
  /// that is, at which argument position it accepts `enigma::varargs`.
  virtual int function_variadic_after(jdi::definition_function *refs) const = 0;

  /// Look up a type by its name.
  virtual jdi::definition* find_typename(std::string name) const = 0;
  /// Check whether the given definition is callable as a function.
  virtual bool definition_is_function(jdi::definition *d) const = 0;
  /// Assuming this definition is a function, retrieve the number of overloads it has.
  virtual size_t definition_overload_count(jdi::definition *d) const = 0;
  /// Read parameter bounds from the current definition into args min and max.
  /// For variadics, max = unsigned(-1).
  virtual void definition_parameter_bounds(jdi::definition *d,
                                           unsigned &min,
                                           unsigned &max) const = 0;

  /// Create a script with the given name (and an assumed 16 parameters,
  /// all defaulted) to the given scope.
  virtual void quickmember_script(jdi::definition_scope* scope,
                                  std::string name) = 0;
  /// Create a standard integer variable member in the given scope.
  virtual void quickmember_integer(jdi::definition_scope* scope,
                                   std::string name) = 0;
  /// Look up an enigma_user definition by its name.
  virtual jdi::definition* look_up(const std::string &name) const = 0;

  virtual ~LanguageFrontend() = default;
};

#endif  // ENIGMA_COMPILER_PARSING_LANGUAGE_FRONTEND_h
