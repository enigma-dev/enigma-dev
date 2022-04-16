/**
 * @file  definition.h
 * @brief System header declaring structures for representing C definitions.
 *
 * This file is likely used by absolutely everything in the parse system, as
 * it is the medium through which output definitions are created and manipulated.
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

#ifndef _DEFINITION__H
#define _DEFINITION__H

#include <General/quickreference.h>
#include "definition_forward.h"

#include <map>
#include <set>
#include <string>
#include <cstddef>
#include <memory>
#include <vector>
#include <deque>

#include <Storage/arg_key.h>
#include <API/error_reporting.h>

namespace jdi {

using std::deque;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::unique_ptr;
using std::vector;

/// The class responsible for storing all parsed definitions.
/// The class is geared to, at the cost of a couple pointers of size,
/// be extensible. It is subclassed for every distinct kind of definition.
struct definition {
  /// DEF_FLAGS to use in identifying this definition's properties
  unsigned int flags;
  string name; ///< The name of this definition, as it appears in code.
  /// The definition of the scope in which this definition is declared.
  /// Except for the global scope of the context, this must be non-null.
  definition_scope* parent;

  /// Returns the kind of this definition; an identifier for the class.
  virtual string kind() const;

  /// Returns whether all of the given flags (OR'd together) are present in
  /// this definition. Equivalent to ((this->flags & flags) == flags).
  bool has_all_flags(unsigned flgs) const { return (flags & flgs) == flgs; }

  /** Duplicates this definition, whatever it may contain.
  @param n  A remap_set containing definitions to replace during duplication.
            This map will grow as more definitions are spawned recursively.
  @return A pointer to a newly-allocated copy of this definition. **/
  virtual unique_ptr<definition> duplicate(remap_set &n) const;

  /** Re-map all uses of each definition used as a key in the remap_set to the
  corresponding definition used as the value. For example, if the given map
  contains { <builtin_type__float, builtin_type__double> }, then any float
  contained in this definition or its descendents will be replaced with a
  double. This can be used to eliminate references to a particular definition
  before free, or to instantiate templates without hassle. */
  virtual void remap(remap_set &n, const ErrorContext &errc);

  /// Returns the size of this definition, as returned by the sizeof operator.
  virtual value size_of(const ErrorContext &errc);

  /** Compare two definitions, returning a comparison sign.
  @param d1  The first definition to compare.
  @param d2  The second definition to compare.
  @return  An integer sharing the sign of the comparison; positive if d1 > d2,
           negative if d1 < d2, 0 if d1 == d2. */
  static ptrdiff_t defcmp(definition *d1, definition *d2);

  /** Print the contents of this scope to a string, returning it.
  @param levels  How many levels of children to print beneath this
                 scope, assuming this is a scope.
  @param indent  The indent, in spaces, to place before each line printed.
  @return Returns a string representation of everything in this definition. */
  virtual string toString(unsigned levels = unsigned(-1),
                          unsigned indent = 0) const;

  /// Return the qualified ID of this definition, eg, ::std::string.
  string qualified_id() const;

  #ifdef CUSTOM_MEMORY_MANAGEMENT
  void *operator new(size_t sz);
  void operator delete(void *ptr);
  #endif

  /// Construct a definition with a name, parent scope, and flags.
  /// Makes necessary allocations based on the given flags.
  definition(string n,definition* p,unsigned int f);
  /// Default constructor. Only valid for the global scope.
  definition();
  /// Default destructor.
  virtual ~definition() = default;
};

}  // namespace jdi

//==============================================================================
//===: Specializations with extended dependencies:==============================
//==============================================================================

#include <Storage/value.h>
#include <Storage/full_type.h>
#include <Storage/references.h>
#include <API/error_reporting.h>
#include <API/AST.h>

namespace jdi {

/// A piece of a definition for anything simply declared with a type.
/// Use of this class includes regular variables, as well as typedefs. This is
/// only part of the class for functions; see \c jdi::definition_function.
struct definition_typed: definition {
  /// The definition of the type of this definition.
  /// This is not guaranteed to be non-null.
  definition* type;
  /// Any referencers modifying the type, such as *, &, [], or (*)().
  ref_stack referencers;
  /// Flags such as long, const, unsigned, etc, as a bitmask.
  /// These can be looked up in \c builtin_decls_byflag.
  unsigned int modifiers;

  virtual string kind() const;
  unique_ptr<definition> duplicate(remap_set &n) const override;
  virtual void remap(remap_set &n, const ErrorContext &errc);
  virtual value size_of(const ErrorContext &errc);
  string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const override;

  definition_typed(string name, definition* p, definition* tp, unsigned int typeflags, int flags = DEF_TYPED);
  /// Construct with all information. Consumes the given \c ref_stack.
  definition_typed(string name, definition* p, definition* tp, ref_stack *rf, unsigned int typeflags, int flags = DEF_TYPED);
  /// Construct without consuming a ref_stack.
  definition_typed(string name, definition* p, definition* tp, const ref_stack &rf, unsigned int typeflags, int flags = DEF_TYPED);
  /// Convenience ctor
  definition_typed(string name, definition* p, const full_type &tp, int flags = DEF_TYPED);

  ~definition_typed() override = default;
};

/// Structure to augment an arg_key when choosing the correct specialization
struct spec_key {
  /** An array of counts of usages of arguments in the arg_key of a template
  specialization. Each count is followed in memory by the indices of the
  parameters the argument was used in. For instance, this code:

  ```cpp
  template<int a, int b, int c> struct x<a, a, b, c, b, b> { .. }
  ```

  Produces this arg_inds:
  ```
  { {2, 0,1}, {1, 3}, {3, 2,4,5} }
  ```

  The argument `a' occurs two times: parameters 0 and 1.
  The argument `b' occurs one time: parameter 3.
  The argument `c' occurs three times: parameters 2, 4, and 5. */
  vector<vector<unsigned>> arg_inds;
  /// The length of the arg_inds array.
  unsigned ind_count;
  /// The maximum number of parameters passed the same argument
  unsigned max_param;

  /** Calculates the merit of an arg_key to this spec_key; that is, returns how
  well it matches, which is based on the greatest number of parameter matches
  for one argument.
  @return Returns the calculated merit; 0 marks incompatibility. */
  int merit(const arg_key &k);
  /** Construct from a parameter count of a particular specialization, and the
  parameter count of the original.
  @param big_count    The parameter count of the specialization.
  @param small_count  The parameter count of the specialization. */
  spec_key(size_t big_count, size_t small_count);
  /// Constructs a new arg key from which to instantiate this specialization.
  arg_key get_key(const arg_key &source_key);
  /// Compare to another key to check if they mean the same thing.
  bool same_as(const spec_key &other);

  spec_key(const spec_key&, bool);
  spec_key(const spec_key&) = delete;
  ~spec_key() = default;
};

/// Separate definition representing a single overload of a function.
struct definition_overload: definition_typed {
  /// The implementation of this function as harvested by an external system.
  void *implementation;

  virtual string kind() const;
  unique_ptr<definition> duplicate(remap_set &n) const override;
  virtual void remap(remap_set &n, const ErrorContext &errc);
  string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const override;

  definition_overload(string name, definition* p, definition* tp, const ref_stack &rf, unsigned int typeflags, int flags = DEF_FUNCTION);
  ~definition_overload() override = default;
};

/// A piece of a definition specifically for functions.
/// The class is based on implements a method of storing overload information.
struct definition_function: definition {
  /// The map type used for storing overloads.
  typedef map<arg_key, unique_ptr<definition_overload>> overload_map;
  /// An iterator type for \c overload_map.
  typedef overload_map::iterator overload_iter;
  /// An iterator type for \c overload_map.
  typedef overload_map::const_iterator overload_citer;

  /// Standard overloads, checked before template overloads.
  overload_map overloads;
  /// Array of reference stacks for each overload of this function.
  vector<unique_ptr<definition_template>> template_overloads;

  virtual string kind() const;
  unique_ptr<definition> duplicate(remap_set &n) const override;
  virtual void remap(remap_set &n, const ErrorContext &errc);
  virtual value size_of(const ErrorContext &errc);
  string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const override;

  /** Function to add the given definition as an overload if no such overload
      exists, or to merge it in (handling any errors) otherwise.
      @param tp        The full_type giving the return type and parameters.
      @param addflags  Any additional flags to be assigned to the overload.
      @param herr      Error handler to report problems to.
      @return Returns the final definition for the requested overload.
  */
  definition_overload *overload(const full_type &tp, unsigned addflags,
                                ErrorContext errc);
  definition_overload *overload(definition* tp, const ref_stack &rf,
                                unsigned int typeflags, unsigned addflags,
                                void *implementation, ErrorContext errc);

  /** Function to add the given definition as a template overload
      exists, or to merge it in (handling any errors) otherwise.
      @param ovrl  The template definition representing the new overload; this definition
                   will belong to the system after you pass it.
      @param herr  An error handler to which errors will be reported.
  */
  void overload(unique_ptr<definition_template> ovrl, ErrorContext errc);

  /// Create a function with one overload, created from the given ref_stack.
  definition_function(string name, definition* p, definition* tp,
                      const ref_stack &rf, unsigned int typeflags,
                      int flags, SourceLocation sloc, ErrorHandler *herr);
  /// Create a function which has no prototypes/overloads; it is impossible
  /// to call this function.
  definition_function(string name, definition* p, int flags = DEF_FUNCTION);
  ~definition_function() override = default;
};

/// Any definition declared with a type, and given a value.
struct definition_valued: definition_typed {
  value value_of; ///< The constant value of this definition.

  virtual string kind() const;
  string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const override;
  unique_ptr<definition> duplicate(remap_set &n) const override;

  definition_valued(); ///< Default constructor; invalidates value.
  definition_valued(string vname, definition *parnt, definition* type, unsigned int modifiers, unsigned int flags, const value &val); ///< Construct with a value and type.
  ~definition_valued() override = default;
};

/// A piece of a definition for anything containing its own scope.
/// This class is meant for namespaces, mostly. It is a base
/// class for structs and classes; see \c jdi::definition_polyscope.
struct definition_scope: definition {
  /// Storage container for all definitions declared in this scope.
  typedef map<string, unique_ptr<definition>> defmap;
  /// Storage container for all definitions declared in this scope.
  typedef map<string, definition*> defrefmap;
  /// Shortcut to an iterator type for \c defmap.
  /// This iterator is **NOT INVALIDATED** by map resizes!
  typedef defmap::iterator defiter;
  /// Shortcut to a constant iterator type for \c defmap.
  /// This iterator is **NOT INVALIDATED** by map resizes!
  typedef defmap::const_iterator defiter_c;
  /// The result from an insert operation on our map.
  typedef pair<defiter, bool> inspair;
  /// The type of key-value entry pair stored in our map.
  typedef defmap::value_type entry;
  /// Vector of `definition_scope`s.
  typedef vector<definition_scope*> scopevec;
  /// In-order view of the definitions in this scope. This includes definitions
  /// owned by both the `members` defmap and `c_structs` defmap.
  // They are stored as iterators because it's a common pattern in JDI sources
  // to insert nullptr, check for errors, *then* actually allocate the class.
  typedef deque<defiter> ordeque;
  /// Iterator for the in-order definition view.
  typedef ordeque::iterator orditer;
  /// Const iterator for the in-order definition view.
  typedef ordeque::const_iterator orditer_c;

  defmap members; ///< Members of this enum or namespace
  scopevec using_scopes; ///< Set of namespaces or other scopes to use.
  defrefmap using_general; ///< A separate map of definitions to use

  /// This is a map of structures declared in this scope, which is needed to
  /// emulate C's ability to have an object and structure by the same name.
  /// C++ extends this ability by allowing the behavior in any scope.
  defmap c_structs;

  /// A deque listing all declaraions (and dependent object references) in this scope, in order.
  /// May contain duplicates.
  ordeque dec_order;
  /** Function to insert into c_structs by the rules of definition_scope::declare.
      @param name  The name of the definition to declare.
      @param def   Pointer to the definition being declared, if one is presently available.
  */
  decpair declare_c_struct(string name, unique_ptr<definition> def = nullptr);

  /// Add a namespace to the using list.
  /// This can technically be called with any scope, in spite of the name.
  void use_namespace(definition_scope* scope);
  /// Remove a recently-added namespace from the using list.
  /// The more recent, the more efficient.
  void unuse_namespace(definition_scope* scope);

  /// Add a specific definition to our using list.
  void use_general(string name, definition* def);

  /** Free all contents of this scope. No copy is made. **/
  void clear();
  /** Relinquish all content to another definition. **/
  void dump(definition_scope* to);
  /** Copy content from another definition. **/
  void copy(const definition_scope* from, remap_set &n);
  /** Swap content with another definition. **/
  void swap(definition_scope* with);

  /** Look up a \c definition* given its identifier.
      @param name  The identifier by which the definition can be referenced. This is NOT qualified! If you have a qualified ID, break it into tokens and ask read_qualified_id, or parse it yourself.
      @return  If found, a pointer to the definition with the given name is returned. Otherwise, nullptr is returned.
  **/
  virtual definition* look_up(string name);
  /// Declare a definition by the given name in this scope. If no definition by
  /// that name exists in this scope, the given definition is inserted.
  /// Otherwise, the given definition is discarded, and the memory is freed.
  /// @param name  The name of the definition to declare.
  /// @param def   A pointer to the definition in memory, or nullptr if it will
  ///              be allocated later.
  /// @return Returns a pair containing whether the item was inserted fresh and
  ///         a mutable pointer to the data inserted.
  ///         The pointer may be changed later to point to newly-allocated data.
  virtual decpair declare(string name, unique_ptr<definition> def = nullptr);
  /** Look up a \c definition* in the current scope or its using scopes given
  its identifier.
  @param name  The identifier by which the definition can be referenced.
               This is NOT qualified!
  @return If found, a pointer to the definition with the given name is returned.
          Otherwise, nullptr is returned. **/
  virtual definition* find_local(string name);
  /** Same as find_local, except called when failure to retrieve a local will
  result in an error. This call may still fail; the system will just try harder,
  generating more recoverable errors to make the flow succeed.
  Used so definition_hypothetical can return another definition_hypothetical.
  @param name  The identifier by which the definition can be referenced.
               This is NOT qualified!
  @return If found, a pointer to the definition with the given name is returned.
          Otherwise, nullptr is returned. **/
  virtual definition* get_local(string name);

  /** Wraps `declare()`; overloads a function, creating it if it does not exist.
  If the function already exists, an overload will be created for it.
  If the overload already exists, its pointer is returned.
  @param name   The function name.
  @param tp     The return type and parameters of the overload to add.
  @param flags  Flags to assign to this overload, such as DEF_PRIVATE
                or DEF_VIRTUAL.
  @param errtok A token referenced for error reporting purposes.
  @param herr   An error handler to receieve errors.
  @return Returns the definition_overload corresponding to the overload with the
          given parameters, or nullptr if an error occurred. */
  definition_overload *overload_function(string name, full_type &tp,
                                         unsigned flags, ErrorContext errc);
  /** Function to add the given definition as a template overload of a function.
  @param name   The function name.
  @param ovrl   The template definition representing the new overload;
                this definition will belong to the system after you pass it.
  @param flags  Flags to assign to this overload, such as DEF_PRIVATE
                or DEF_VIRTUAL.
  @param errtok A token referenced for error reporting purposes.
  @param herr   An error handler to which errors will be reported.
  @return Returns the definition_function to which the overload was added,
          or nullptr if an error occurred. */
  definition_function *overload_function(string name,
                                         unique_ptr<definition_template> ovrl,
                                         unsigned flags, ErrorContext errc);

  virtual string kind() const;
  unique_ptr<definition> duplicate(remap_set &n) const override;
  virtual void remap(remap_set &n, const ErrorContext &errc);
  virtual value size_of(const ErrorContext &errc);
  string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const override;

  /// Default constructor. Only to be used for global!
  definition_scope();
  definition_scope(const definition_scope&);
  /// Construct with a name and type.
  /// @param  name   The name of this scope.
  /// @param  parent The parent scope of this scope. Non-null (only global scope can have null parent).
  /// @param  flags  The type of this scope, such as DEF_NAMESPACE.
  definition_scope(string name, definition *parent, unsigned int flags);
  ~definition_scope() override = default;
};

/// An extension of \c jdi::definition_scope for classes and structures,
/// which can have ancestors.
struct definition_class: definition_scope {
  /// Simple structure for storing an inheritance type and the \c definition* of an ancestor.
  struct ancestor {
    unsigned protection; ///< The protection level of this inheritance, as one of the DEF_ constants, or 0 for public.
    definition_class* def; ///< The \c definition of the structure or class from which this one inherits members.
    ancestor(unsigned protection_level, definition_class* inherit_from); ///< Convenience constructor with both members.
    ancestor(); ///< Default constructor for vector.
  };

  /// Ancestors of this structure or class
  vector<ancestor> ancestors;
  /// This tells us if we're an instance or specialization of some template.
  definition_template *instance_of;

  /// A collection of all our friends.
  set<definition*> friends;

  virtual string kind() const;
  unique_ptr<definition> duplicate(remap_set &n) const override;
  virtual void remap(remap_set &n, const ErrorContext &errc);
  virtual value size_of(const ErrorContext &errc);
  string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const override;

  virtual definition* look_up(string name); ///< Look up a definition in this class (including its ancestors).
  virtual definition* find_local(string name);
  /** Same as find_local, except called when failure to retrieve a local will result in an error.
      This call may still fail; the system will just "try harder." In this case, that means returning
      the constructor definition, named @c constructor_name, when asked for an identifier by the same name.
      Used so definition_hypothetical can return another definition_hypothetical.
      @param name  The identifier by which the definition can be referenced. This is NOT qualified!
      @return  If found, a pointer to the definition with the given name is returned. Otherwise, nullptr is returned.
  **/
  virtual definition* get_local(string name);
  virtual decpair declare(string name, unique_ptr<definition> def = nullptr) override;

  definition_class(string classname, definition_scope* parent, unsigned flags = DEF_CLASS | DEF_TYPENAME);
  ~definition_class() override = default;
};

/**
  @struct jdi::definition_union
  An extension of \c jdi::definition_scope for unions, which have a unified tyoe and unique sizeof() operator.
**/
struct definition_union: definition_scope {
  virtual string kind() const;
  unique_ptr<definition> duplicate(remap_set &n) const override;
  virtual void remap(remap_set &n, const ErrorContext &errc);
  virtual value size_of(const ErrorContext &errc);
  string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const override;

  definition_union(string classname, definition_scope* parent, unsigned flags = DEF_CLASS | DEF_UNION | DEF_TYPENAME);
  ~definition_union() override = default;
};

/// @struct jdi::definition_enum
/// An extension of \c jdi::definition for enums, which contain mirrors of members in the parent scope.
struct definition_enum: definition_class {
  /// A small structure for storing definition_value definitions for each constant, plus an AST if the constant
  /// could not be evaluated at its occurrence (sometimes the case with templates).
  struct const_pair {
    /// The definition generated for the constant. Owned by definition_scope.
    definition_valued* def;
    /// If needed, an AST to allow re-evaluating the value expression later.
    /// (Used for dependent enum values.)
    unique_ptr<AST> ast;
    const_pair(definition_valued *d, unique_ptr<AST> a):
        def(d), ast(std::move(a)) {}
  };

  /// The constants contained in this enum, in order.
  vector<const_pair> constants;

  string kind() const override;
  unique_ptr<definition> duplicate(remap_set &n) const override;
  void remap(remap_set &n, const ErrorContext &errc) override;
  value size_of(const ErrorContext &errc) override;
  string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const override;

  /// The (primitive) type of the constants in this enum; a cast to this type is
  /// valid but not favored (in overload resolution).
  definition *type;
  /// Modifiers to our type, namely, unsigned.
  unsigned modifiers;

  definition_enum(string classname, definition_scope* parent, unsigned flags = DEF_ENUM | DEF_TYPENAME);
  ~definition_enum() override = default;
};

/**
  @struct jdi::definition_template
  A piece of a definition for anything with template parameters.
  This class can be used alongside structs, classes, and functions.
**/
struct definition_template: definition_scope {
  /// The definition to which template parameters here contained are applied.
  unique_ptr<definition> def;

  typedef vector<unique_ptr<definition_tempparam>> pvector;
  typedef pvector::iterator piterator;
  typedef pvector::const_iterator pciterator;
  pvector params;

  /// Class representing a single template specialization. Stored in a list of
  /// specializations which is mapped by arg_key fitting.
  struct specialization {
    spec_key key;
    arg_key filter;
    unique_ptr<definition_template> spec_temp;
    specialization(definition_template *base_template,
                   unique_ptr<definition_template> spec_template):
      key(base_template->params.size(), spec_template->params.size()), filter(),
      spec_temp(std::move(spec_template)) {}
    specialization(const specialization &other,
                   unique_ptr<definition_template> spec_template):
      key(other.key, true), filter(other.filter),
      spec_temp(std::move(spec_template)) {}
    specialization(const specialization&) = delete;
    specialization(specialization&&) = default;
    ~specialization() = default;
  };

  /// Look up a specialization fitting an arg_key.
  /// @param key  The key to find a specialization for.
  /// @return  The best-fit specialization, or nullptr if none was found.
  specialization *find_specialization(const arg_key &key) const;

  /// List of specializations fitting a certain abstracted arg_key
  typedef vector<unique_ptr<specialization>> speclist;
  /// Map iterator type for specializations
  typedef speclist::iterator speciter;
  /// Constant map iterator type for specializations
  typedef speclist::const_iterator speciter_c;

  struct instantiation {
    unique_ptr<definition> def;
    vector<unique_ptr<definition>> parameter_defs;
    instantiation(): def(nullptr), parameter_defs() {}
    ~instantiation() = default;
    instantiation(const instantiation&) = delete;
  };

  typedef map<arg_key, unique_ptr<instantiation>> instmap; ///< Map type for instantiations
  typedef instmap::iterator institer; ///< Map iterator type for instantiations

  /// Dependent member list. See instantiation below.
  typedef vector<unique_ptr<definition_hypothetical>> deplist;
  typedef deplist::iterator depiter; ///< Dependent member iterator

  /// A map of all specializations.
  speclist specializations;
  /// A map of all existing instantiations.
  instmap instantiations;
  /// A listing of all dependent members of this template. These are definitions
  /// that must be reinterpreted once all parameters are known.
  deplist dependents;

  /// Instantiate this template with the values given in the passed key. If this
  /// template has been instantiated previously, that instantiation is returned.
  /// @param key   The \c arg_key structure containing the template parameter
  ///              values to use.
  /// @param errc  The \c ErrorContext to which any problems will be reported.
  definition *instantiate(const arg_key& key, const ErrorContext &errc);

  virtual string kind() const;
  unique_ptr<definition> duplicate(remap_set &n) const override;
  virtual void remap(remap_set &n, const ErrorContext &errc);
  virtual value size_of(const ErrorContext &errc);
  string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const override;

  /** Construct with name, parent, and flags **/
  definition_template(string name, definition *parent, unsigned flags);
  /** Destructor to free template parameters, instantiations, etc. **/
  ~definition_template() override = default;
};

/// A definition inheriting from definition_class, which is meant to represent
/// a template parameter. Definitions in this class are considered hypothetical;
/// they must exist in the type speicified to the template (in standard speak,
/// they depend on that type).
struct definition_tempparam: definition_class {
  unique_ptr<AST> default_assignment;
  full_type integer_type;
  /// Denotes that this template parameter was used in a way that only a class
  /// can be used (such as inheritance or member access).
  bool must_be_class;

  /** Construct with default information.
    @param name   Some unique key name for this scope.
    @param parent The scope above this one.
    @param flags  The additional flag data about this scope. */
  definition_tempparam(string name, definition_scope* parent,
                       unsigned flags = DEF_TEMPPARAM | DEF_DEPENDENT);
  /** Construct with default information.
    @param name   Some unique key name for this scope.
    @param parent The scope above this one.
    @param defval The default value given to this parameter, read in as an AST
                  to enable it to depend on other parameters.
    @param flags  The additional flag data about this scope. */
  definition_tempparam(string name, definition_scope* parent,
                       unique_ptr<AST> defval,
                       unsigned flags = DEF_TEMPPARAM | DEF_DEPENDENT);
  /** Construct with default information.
    @param name    Some unique key name for this scope.
    @param parent  The scope above this one.
    @param inttype The integer type used if this template parameter is instead
                   a numeric constant.
    @param defval  The default value given to this parameter, read in as an AST
                   to enable it to depend on other parameters.
    @param flags   The additional flag data about this scope. */
  definition_tempparam(string name, definition_scope* parent,
                       full_type &inttype, unique_ptr<AST> defval = nullptr,
                       unsigned flags = DEF_TEMPPARAM | DEF_DEPENDENT);

  string kind() const override;
  unique_ptr<definition> duplicate(remap_set &n) const override;
  void remap(remap_set &n, const ErrorContext &errc) override;

  /// Look up a definition in the parent of this scope (skip this scope).
  /// This function will never be used by the system.
  definition* look_up(string name) override;
  /// Declare a definition by the given name in this scope. The definition will
  /// be marked HYPOTHETICAL, and the \c must_be_class flag will be set.
  decpair declare(string name, unique_ptr<definition> = nullptr) override;
  /// Behaves identically to declare if the given name does not exist, or else
  /// returns it. In either case, the returned definition will be HYPOTHETICAL.
  definition* get_local(string name) override;

  ~definition_tempparam() override = default;
};

/// A definition for atomic types.
struct definition_atomic: definition_scope {
  size_t sz;
  string kind() const;
  void remap(remap_set &n, const ErrorContext &errc);
  unique_ptr<definition> duplicate(remap_set &n) const override;
  value size_of(const ErrorContext &errc);
  string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const;
  definition_atomic(string n,definition* p,unsigned int f, size_t size);
  ~definition_atomic() override = default;
};

/// A class representing a dependent (here called "hypothetical") type--a type which
/// depends on an abstract parent or scope.
struct definition_hypothetical: definition_class {
  /// The expression this definition represents.
  unique_ptr<AST> def;
  /// Set of flags required of any type provided to this template parameter.
  unsigned int required_flags;

  virtual string kind() const;
  unique_ptr<definition> duplicate(remap_set &n) const override;
  virtual void remap(remap_set &n, const ErrorContext &errc);
  virtual value size_of(const ErrorContext &errc);
  string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const override;
  virtual definition* get_local(string name) override;

  /// Construct with basic definition info.
  definition_hypothetical(string name, definition_scope *parent,
                          unsigned flags, unique_ptr<AST> def);
  definition_hypothetical(string name, definition_scope *parent,
                          unique_ptr<AST> def);
  ~definition_hypothetical() override = default;
};


//==================================================================================================
//===: User Definitions :===========================================================================
//==================================================================================================

// These definitions are not used by JDI, but rather are provided as utility classes for the
// end-user. Use them if you want. Or don't.

/// A smart scope designed to be used (as in a using directive) by another scope.
/// This class automatically adds itself to the using collection of the given scope
/// on construct, and automatically removes itself on destruct.
class using_scope: public definition_scope {
  /// The scope using us.
  definition_scope* user_;
  /// Index of this scope in the using list.
  size_t position_hint_;

 public:
  /// Construct with name and user scope.
  /// @param name  The name of this scope, in case a trace is ever printed.
  /// @param user  The scope which will use this scope, as in a using directive.
  using_scope(string name, definition_scope* user);
  /// RAII: Unuse ourself.
  ~using_scope() override;
};

}  // namespace jdi

#endif
