/**
 * @file context.h
 * @brief Header containing methods for creating contexts of parsed code.
 *
 * One idea that surfaced which seemed like a keeper was a system for creating
 * transferrable contexts. What we mean by that is allowing users to decide how
 * many parse contexts to create and what to parse with them, and providing a set
 * of functions to copy definitions from one context to another. This can be done
 * by class or namespace, or by copying the entire context at once. This enables
 * users to optimize their use of the parser. For instance, by parsing large libs
 * such as windows.h or opengl.h as individual contexts, users are then able to
 * parse smaller headers which include or otherwise depend on them by creating a
 * new context, copying the contents of the large header contexts over, and then
 * invoking the parser on the remaining code.
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

#ifndef _JDI_CONTEXT__H
#define _JDI_CONTEXT__H

#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <memory>

namespace jdi {
class Context;
class context_parser;
}

#include <System/macros.h>
#include <System/type_usage_flags.h>
#include <Storage/definition.h>
#include <General/llreader.h>
#include <System/token.h>
#include <API/error_reporting.h>

namespace jdi {

using std::map;
using std::string;
using std::vector;
using std::ostream;
using std::cout;
using std::set;
using std::shared_ptr;
using std::unique_ptr;

/// Map of string to token type; a map-of-keywords type.
typedef std::map<string, TOKEN_TYPE> keyword_map;

/**
@class context
@brief A class representing a context for manipulation.

The context class allows users to create, search, manage, and splice collections
of parsed source code on separate bases.

@see context.h
**/
class Context {
  bool parse_open; ///< True if we're already parsing something
  friend class jdi::AST;
  friend class jdi::context_parser;

  protected: // Make sure our method-packing child can use these.
  macro_map macros; ///< A map of macros defined in this context.
  /// A list of #include directories in the order they will be searched.
  vector<string> search_directories;
  /// The global scope represented in this context.
  unique_ptr<definition_scope> global;

  ErrorHandler *herr;

 public:
  set<definition*> variadics; ///< Set of variadic types.

  size_t search_dir_count() const; ///< Return the number of search directories
  string search_dir(size_t index) const; ///< Return the search directory with the given index, in [0, search_dir_count).

  /** Add a type name to this context
      The type will be added as a primitive. To add a typedef, use \c jdi::context::add_typedef().
      @param name The name of the type, as it appears in code.
  **/
  void add_typename(string name);
  /** Add a typedef'd type to this context.
      This function creates a new type aliased to an existing type. Hence,
      \p definiens must not be null. Chances are, if you don't have anything to
      pass to definiens, you are wanting to create a primitive type.
      See \c jdi::context::add_typename().
      @param definiendum  The name of the aliased type, as it appears in code.
      @param definiens    The type to which the created type will be aliased.
                          This cannot be null.
  **/
  void add_typedef(string definiendum, definition *definiens);

  void read_macros(const char* filename); ///< Read a file containing exclusively macros, in C format.
  void add_macro(string definiendum, string definiens); ///< Add a macro to this context.

  /// Add a macro function with no parameters to this context.
  void add_macro_func(string definiendum, string definiens);
  /// Add a macro function with one parameter to this context.
  /// If variadic is true, the parameter can contain commas.
  void add_macro_func(string definiendum, string p1, string definiens, bool variadic);
  /// Add a macro function with two parameters to this context.
  /// If variadic is true, the second parameter can contain commas.
  void add_macro_func(string definiendum, string p1, string p2, string definiens, bool variadic);
  /// Add a macro function with three parameters to this context.
  /// If variadic is true, the third parameter can contain commas.
  void add_macro_func(string definiendum, string p1, string p2, string p3, string definiens, bool variadic);

  /// Read a file containing a list of #include search directories, one full path per line.
  void read_search_directories(const char* filename);
  /// Same as read_search_directories, but designed to parse GNU cpp output.
  /// Ignores all lines up to and including the first line containing begin_line,
  /// then reads in all additional lines as search directories until it reaches end_line.
  void read_search_directories_gnu(const char* filename, const char* begin_line, const char* end_line);
  /// Add an #include search directory to this context.
  void add_search_directory(string dir);

  void reset(); ///< Reset back to the built-ins; delete all parsed definitions
  void reset_all(); ///< Reset everything, dumping all built-ins as well as all parsed definitions
  void copy(const Context &ct); ///< Copy the contents of another context.
  void swap(Context &ct); ///< Swap contents with another context.

  /** Load standard built-in types, such as int.
      This function is really only for use with the built-in context.
  **/
  void load_standard_builtins();
  /** Load GNU-specific built-in types, such as nonsense_t.
      This function is really only for use with the built-in context.
  **/
  void load_gnu_builtins();

  /** Sets the error handler to use when parsing macros or code. */
  void set_error_handler(ErrorHandler *herr_) {
    herr = herr_;
  }

  void output_types(ostream &out = cout); ///< Print a list of scoped-in types.
  void output_macro(string macroname, ostream &out = cout); ///< Print a single macro to a given stream.
  void output_macros(ostream &out = cout); ///< Print a list of scoped-in macros.
  void output_definitions(ostream &out = cout); ///< Print a any scoped-in other shit.

  /// Drop all current macros, freeing them.
  void delete_macros();

  /// Get a reference to the macro map
  inline const macro_map& get_macros() const { return macros; }
  /// Get our global scope
  inline definition_scope* get_global() const { return global.get(); }

  /// Get a non-const reference to the global macro set.
  static macro_map &global_macros();

  /// List of C++ keywords, mapped to the type of their token.
  /// This list is assumed to contain tokens whose contents are unambiguous;
  /// one string maps to one token, and vice-versa.
  keyword_map keywords;

  /** Parse an input stream for definitions.
      @param cfile     The stream to be read in.
  **/
  int parse_stream(llreader& cfile);

  /** Default constructor; allocates a global context with built-in definitions.
      Definitions are copied into the new context from the \c builtin context.
      You may specify an error handler to use in this constructor, or you may
      assign one later using `set_error_handler(herr)`.
  **/
  Context(ErrorHandler *herr = default_error_handler);


  /** Construct with essentially nothing. This constructor circumvents the copy process. It has
      no purpose other than and is not to be used except in allocating the builtin scope.
      While it is not necessary under common circumstances to avoid copying from the
      global scope into itself on construct, should the scope ever be populated before
      the ctor body due to one change or another, it could potentially cause issues.

      As the issues are as unlikely as the scenario, this method may eventually be removed.
      For the time being, we may as well keep a distinction between the constructors of the
      normal scopes and of the builtin scope.

      @param disregarded  Disregarded. The parameter is there only to distinguish this constructor.
  **/
  Context(int disregarded);

  /** Copy constructor.
      Overrides the C++ default copy constructor with a version meant to simplify
      building off of existing contexts. Simply constructs and duplicates the passed
      context instead of the builtin context.
  **/
  Context(const Context&);

  /** A simple destructor to clean up after the definition loading. **/
  ~Context() = default;
};

}

#endif
