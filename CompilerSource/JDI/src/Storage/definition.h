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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
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
#include <vector>
#include <deque>
using std::map;
using std::set;
using std::vector;
using std::deque;
using std::pair;

#include <Storage/arg_key.h>
#include <API/error_context.h>

namespace jdi {
  /**
    @struct jdi::definition
    @brief  The class responsible for storing all parsed definitions.
    
    The class is geared to, at the cost of a couple pointers of size, be extensible. 
  **/
  struct definition
  {
    unsigned int flags; ///< DEF_FLAGS to use in identifying this definition's properties
    string name; ///< The name of this definition, as it appears in code.
    definition_scope* parent; ///< The definition of the scope in which this definition is declared.
                        ///< Except for the global scope of the context, this must be non-NULL.
    
    virtual string kind() const; ///< Return the kind of this definition; an identifier for the class.
    
    /** Duplicate this definition, whatever it may contain.
        The duplicated version must be freed separately.
        @param n A remap_set containing any definitions to replace in this duplication.
                 This map will grow as more definitions are spawned recursively.
        @return A pointer to a newly-allocated copy of this definition. **/
    virtual definition* duplicate(remap_set &n) const;
    
    /** Re-map all uses of each definition used as a key in the remap_set to the
        corresponding definition used as the value. For example, if the given map
        contains { <builtin_type__float, builtin_type__double> }, then any float
        contained in this definition or its descendents will be replaced with a
        double. This can be used to eliminate references to a particular definition
        before free, or to instantiate templates without hassle. **/
    virtual void remap(remap_set &n, const error_context &errc);
    
    /** Return the size of this definition. **/
    virtual value size_of(const error_context &errc);
    
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
        @return Returns a string representation of everything in this definition.
    **/
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const;
    
    /// Return the qualified ID of this definition, eg, ::std::string.
    string qualified_id() const;
    
    #ifdef CUSTOM_MEMORY_MANAGEMENT
    void *operator new(size_t sz);
    void operator delete(void *ptr);
    #endif
    
    /** Construct a definition with a name, parent scope, and flags.
        Makes necessary allocations based on the given flags. **/
    definition(string n,definition* p,unsigned int f);
    /// Default constructor. Only valid for the global scope.
    definition();
    /// Default destructor.
    virtual ~definition();
  };
}

//=========================================================================================================
//===: Specializations with extended dependencies:=========================================================
//=========================================================================================================

#include <Storage/value.h>
#include <Storage/full_type.h>
#include <Storage/references.h>
#include <Parser/context_parser.h>
#include <API/error_reporting.h>
#include <API/error_context.h>
#include <API/AST.h>

namespace jdi {
  /**
    @struct jdi::definition_typed
    A piece of a definition for anything simply declared with a type.
    Use of this class includes regular variables, as well as typedefs. This is
    only part of the class for functions; see \c jdi::definition_function.
  **/
  struct definition_typed: definition {
    definition* type; ///< The definition of the type of this definition. This is not guaranteed to be non-NULL.
    ref_stack referencers; ///< Any referencers modifying the type, such as *, &, [], or (*)().
    unsigned int modifiers; ///< Flags such as long, const, unsigned, etc, as a bitmask. These can be looked up in \c builtin_decls_byflag.
    
    virtual string kind() const;
    virtual definition* duplicate(remap_set &n) const;
    virtual void remap(remap_set &n, const error_context &errc);
    virtual value size_of(const error_context &errc);
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const;
    
    definition_typed(string name, definition* p, definition* tp, unsigned int typeflags, int flags = DEF_TYPED);
    /// Construct with all information. Consumes the given \c ref_stack.
    definition_typed(string name, definition* p, definition* tp, ref_stack *rf, unsigned int typeflags, int flags = DEF_TYPED);
    /// Construct without consuming a ref_stack.
    definition_typed(string name, definition* p, definition* tp, const ref_stack &rf, unsigned int typeflags, int flags = DEF_TYPED);
    /// Convenience ctor
    definition_typed(string name, definition* p, const full_type &tp, int flags = DEF_TYPED);
    
    virtual ~definition_typed();
  };
  
  /// Structure to augment an arg_key when choosing the correct specialization
  struct spec_key {
    /** This is an array of counts of uses of arguments in an arg_key in a template specialization.
        Each count is followed in memory by the indices of the parameters the argument was used in.
        For instance, this code:
        
           template<int a, int b, int c> struct x<a, a, b, c, b, b> { .. } 
        
        Produces this arg_inds:
        
           { {2, 0,1}, {1, 3}, {3, 2,4,5} }
        
        The argument `a' occurs two times: parameters 0 and 1.
        The argument `b' occurs one time: parameter 3.
        The argument `c' occurs three times: parameters 2, 4, and 5.
    */
    unsigned **arg_inds;
    /// The length of the arg_inds array.
    unsigned ind_count;
    /// The maximum number of parameters passed the same argument
    unsigned max_param;
    
    /** Calculates the merit of an arg_key to this spec_key; that is, returns how well it matches,
        which is based on the greatest number of parameter matches for one argument.
      @return Returns the calculated merit; 0 marks incompatibility. */
    int merit(const arg_key &k);
    /** Construct from a parameter count of a particular specialization, and the parameter count of the original.
        @param big_count    The parameter count of the specialization.
        @param small_count  The parameter count of the specialization.
    */
    spec_key(size_t big_count, size_t small_count);
    /* *
      Construct from a template's parameter list and an arg_key containing the specialization.
      @param special_template    The template whose parameters are used in the given key.
      @param specialization_key  The arg_key read in from the specialization's parameters.
                                 template<not this one> class my_specialized_class<this one> {};
    * /
    spec_key(const definition_template *special_template, const arg_key& specialization_key);*/
    /** Construct a new argument key from which to instantiate this specialization. */
    arg_key get_key(const arg_key &source_key);
    /// Compare to another key to check if they mean the same thing.
    bool same_as(const spec_key &other);
    /// Destruct, freeing array.
    ~spec_key();
    
    spec_key(const spec_key&, bool);
    private: spec_key(const spec_key&);
  };
  
  struct definition_overload: definition_typed {
    void *implementation; ///< The implementation of this function as harvested by an external system.
    
    virtual string kind() const;
    virtual definition* duplicate(remap_set &n) const;
    virtual void remap(remap_set &n, const error_context &errc);
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const;
    
    definition_overload(string name, definition* p, definition* tp, const ref_stack &rf, unsigned int typeflags, int flags = DEF_FUNCTION);
  };
  
  /**
    @struct jdi::definition_function
    A piece of a definition specifically for functions.
    The class is based on implements a method of storing overload information.
  **/
  struct definition_function: definition {
    typedef map<arg_key, definition_overload*> overload_map; ///< The map type used for storing overloads.
    typedef overload_map::iterator overload_iter; ///< An iterator type for \c overload_map.
    typedef overload_map::const_iterator overload_citer; ///< An iterator type for \c overload_map.
    
    overload_map overloads; ///< Standard overloads, checked before template overloads.
    vector<definition_template*> template_overloads; ///< Array of reference stacks for each overload of this function.
    
    virtual string kind() const;
    virtual definition* duplicate(remap_set &n) const;
    virtual void remap(remap_set &n, const error_context &errc);
    virtual value size_of(const error_context &errc);
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const;
    
    /** Function to add the given definition as an overload if no such overload
        exists, or to merge it in (handling any errors) otherwise.
        @param tp        The full_type giving the return type and parameters.
        @param addflags  Any additional flags to be assigned to the overload.
        @param herr      Error handler to report problems to.
        @return Returns the final definition for the requested overload.
    */
    definition_overload *overload(const full_type &tp, unsigned addflags, error_handler *herr);
    definition_overload *overload(definition* tp, const ref_stack &rf, unsigned int typeflags, unsigned addflags, void *implementation, error_handler *herr);
    
    /** Function to add the given definition as a template overload
        exists, or to merge it in (handling any errors) otherwise.
        @param ovrl  The template definition representing the new overload; this definition
                     will belong to the system after you pass it.
        @param herr  An error handler to which errors will be reported.
    */
    void overload(definition_template* ovrl, error_handler *herr);
    
    definition_function(string name, definition* p, definition* tp, const ref_stack &rf, unsigned int typeflags, int flags = DEF_FUNCTION); ///< Create a function with one overload, created from the given ref_stack.
    definition_function(string name, definition* p, int flags = DEF_FUNCTION); ///< Create a function which has no prototypes/overloads; it is impossible to call this function.
    virtual ~definition_function();
  };
  
  
  /**
    @struct jdi::definition_valued
    A subclass of definition for anything declared with a type, and given a value.
  **/
  struct definition_valued: definition_typed {
    value value_of; ///< The constant value of this definition.
    definition_valued(); ///< Default constructor; invalidates value.
    
    virtual string kind() const;
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const;
    virtual definition* duplicate(remap_set &n) const;
    
    definition_valued(string vname, definition *parnt, definition* type, unsigned int modifiers, unsigned int flags, const value &val); ///< Construct with a value and type.
  };
  
  /**
    @struct jdi::definition_scope
    A piece of a definition for anything containing its own scope.
    This class is meant for namespaces, mostly. It is a base
    class for structs and classes; see \c jdi::definition_polyscope.
  **/
  struct definition_scope: definition {
    typedef map<string, definition*> defmap; ///< Shortcut defined to be the storage container by which definitions are looked up.
    typedef defmap::iterator defiter; ///< Shortcut to an iterator type for \c defmap.
    typedef defmap::const_iterator defiter_c; ///< Shortcut to a constant iterator type for \c defmap.
    typedef pair<defiter,bool> inspair; ///< The result from an insert operation on our map.
    typedef pair<string, definition*> entry; ///< The type of key-value entry pair stored in our map.
    
    defmap members; ///< Members of this enum or namespace
    defmap using_general; ///< A separate map of definitions to use
    
    /// Linked list node to contain using scopes
    struct using_node {
      definition_scope *use; ///< Scope to search
      using_node *next; ///< The next node on our list, or NULL.
      private: friend class definition_scope;
        using_node *prev; ///< The previous node on our list, or NULL, for removal purposes.
        using_node(definition_scope* scope); ///< Construct with a scope to use
        using_node(definition_scope* scope, using_node* prev); ///< Construct with previous node
    };
    /** Add a namespace to the using list. This can technically be used on any scope. **/
    using_node *use_namespace(definition_scope* scope);
    
    /// This is a map of structures declared in this scope, which is needed to emulate C's ability
    /// to have an object and structure by the same name. C++ extends this ability by allowing the
    /// behavior in any scope.
    map<string, definition*> c_structs;
    /** Function to insert into c_structs by the rules of definition_scope::declare.
        @param name  The name of the definition to declare.
        @param def   Pointer to the definition being declared, if one is presently available.
    */
    decpair declare_c_struct(string name, definition* def = NULL);
    
    /// A structure for keeping a pointer to a declaration.
    struct dec_order_g { virtual definition *def() = 0; virtual ~dec_order_g() {} };
    struct dec_order_defiter: dec_order_g {
      defiter it;
      dec_order_defiter(defiter i): it(i) {}
      virtual definition *def() { return it->second; }
      ~dec_order_defiter() {}
    };
    
    typedef deque<dec_order_g*> ordeque;
    typedef ordeque::iterator orditer;
    typedef ordeque::const_iterator orditer_c;
    
    /// A deque listing all declaraions (and dependent object references) in this scope, in order.
    /// May contain duplicates.
    ordeque dec_order;
    
    /** Remove a previously added namespace from our using list. **/
    void unuse_namespace(using_node *ns);
    /** Add a namespace to the using list. This can technically be used on any scope. **/
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
        @return  If found, a pointer to the definition with the given name is returned. Otherwise, NULL is returned.
    **/
    virtual definition* look_up(string name);
    /** Declare a definition by the given name in this scope. If no definition by that name exists in this scope,
        the given definition is inserted. Otherwise, the given definition is discarded, and the 
        @param name  The name of the definition to declare.
        @param def   A pointer to the definition in memory, or NULL if it will be allocated later.
        @return Returns a pair containing whether the item was inserted fresh and a mutable pointer to the data inserted.
                The pointer may be changed to indicate newly allocated data.
    **/
    virtual decpair declare(string name, definition* def = NULL);
    /** Look up a \c definition* in the current scope or its using scopes given its identifier.
        @param name  The identifier by which the definition can be referenced. This is NOT qualified!
        @return  If found, a pointer to the definition with the given name is returned. Otherwise, NULL is returned.
    **/
    virtual definition* find_local(string name);
    /** Same as find_local, except called when failure to retrieve a local will result in an error.
        This call may still fail; the system will just "try harder."
        Used so definition_hypothetical can return another definition_hypothetical.
        @param name  The identifier by which the definition can be referenced. This is NOT qualified!
        @return  If found, a pointer to the definition with the given name is returned. Otherwise, NULL is returned.
    **/
    virtual definition* get_local(string name);
    
    /** Wrapper around `declare()` which overloads a function, creating it if it does not exist.
        If the function already exists, an overload will be created for it.
        If the overload already exists, its pointer is returned.
        @param name   The function name.
        @param tp     The return type and parameters of the overload to add.
        @param flags  Flags to assign to this overload, such as DEF_PRIVATE or DEF_VIRTUAL.
        @param errtok A token referenced for error reporting purposes.
        @param herr   An error handler to receieve errors.
        @return Returns the definition_overload corresponding to the overload with the given parameters, or NULL if an error occurred.
    */
    definition_overload *overload_function(string name, full_type &tp, unsigned flags, const jdip::token_t& errtok, error_handler *herr);
    /** Function to add the given definition as a template overload
        exists, or to merge it in (handling any errors) otherwise.
        @param name   The function name.
        @param ovrl   The template definition representing the new overload; this definition
                      will belong to the system after you pass it.
        @param flags  Flags to assign to this overload, such as DEF_PRIVATE or DEF_VIRTUAL.
        @param errtok A token referenced for error reporting purposes.
        @param herr   An error handler to which errors will be reported.
        @return Returns the definition_function to which the overload was added, or NULL if an error occurred.
    */
    definition_function *overload_function(string name, definition_template* ovrl, unsigned flags, const jdip::token_t& errtok, error_handler *herr);
    
    virtual string kind() const;
    virtual definition* duplicate(remap_set &n) const;
    virtual void remap(remap_set &n, const error_context &errc);
    virtual value size_of(const error_context &errc);
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const;
    
    /** Default constructor. Only to be used for global! **/
    definition_scope();
    /** Overwrite copy constructor. **/
    definition_scope(const definition_scope&);
    /** Construct with a name and type.
        @param  name   The name of this scope.
        @param  parent The parent scope of this scope. Non-NULL (only global scope can have null parent).
        @param  flags  The type of this scope, such as DEF_NAMESPACE.
    **/
    definition_scope(string name, definition *parent, unsigned int flags);
    /** Default destructor, which simply calls clear().
        @warning If you reference objects that are local to a destroyed scope,
                 these references will be invalidated after that scope is destroyed.
    **/
    virtual ~definition_scope();
    
    protected:
      /// First linked list entry
      using_node *using_front;
      /// Final linked list entry
      using_node *using_back;
  };
  
  /**
    @struct jdi::definition_class
    An extension of \c jdi::definition_scope for classes and structures, which can have ancestors.
  **/
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
    virtual definition* duplicate(remap_set &n) const;
    virtual void remap(remap_set &n, const error_context &errc);
    virtual value size_of(const error_context &errc);
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const;
    
    virtual definition* look_up(string name); ///< Look up a definition in this class (including its ancestors).
    virtual definition* find_local(string name);
    /** Same as find_local, except called when failure to retrieve a local will result in an error.
        This call may still fail; the system will just "try harder." In this case, that means returning
        the constructor definition, named @c constructor_name, when asked for an identifier by the same name.
        Used so definition_hypothetical can return another definition_hypothetical.
        @param name  The identifier by which the definition can be referenced. This is NOT qualified!
        @return  If found, a pointer to the definition with the given name is returned. Otherwise, NULL is returned.
    **/
    virtual definition* get_local(string name);
    virtual decpair declare(string name, definition* def = NULL); ///< Declare a definition by the given name in this scope.
    
    definition_class(string classname, definition_scope* parent, unsigned flags = DEF_CLASS | DEF_TYPENAME);
  };
  
  /**
    @struct jdi::definition_union
    An extension of \c jdi::definition_scope for unions, which have a unified tyoe and unique sizeof() operator.
  **/
  struct definition_union: definition_scope {
    virtual string kind() const;
    virtual definition* duplicate(remap_set &n) const;
    virtual void remap(remap_set &n, const error_context &errc);
    virtual value size_of(const error_context &errc);
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const;
    
    definition_union(string classname, definition_scope* parent, unsigned flags = DEF_CLASS | DEF_UNION | DEF_TYPENAME);
  };
  
  /**
    @struct jdi::definition_enum
    An extension of \c jdi::definition for enums, which contain mirrors of members in the parent scope.
  **/
  struct definition_enum: definition_class {
    /// A small structure for storing definition_value definitions for each constant, plus an AST if the constant
    /// could not be evaluated at its occurrence (sometimes the case with templates).
    struct const_pair {
      definition_valued* def; ///< The definition generated for the constant.
      AST* ast; ///< If needed, an AST to allow re-evaluating the value expression later.
      const_pair(definition_valued *d, AST *a): def(d), ast(a) {} ///< Full constructor.
    };
    
    /// The constants contained in this enum, in order.
    vector<const_pair> constants;
    
    virtual string kind() const;
    virtual definition* duplicate(remap_set &n) const;
    virtual void remap(remap_set &n, const error_context &errc);
    virtual value size_of(const error_context &errc);
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const;
    
    definition *type; ///< The type of the constants in this enum; a cast to this type is valid but not favored (in overload resolution).
    unsigned modifiers; ///< Modifiers to our type, namely, unsigned.
    
    definition_enum(string classname, definition_scope* parent, unsigned flags = DEF_ENUM | DEF_TYPENAME);
    ~definition_enum();
  };
  
  /**
    @struct jdi::definition_template
    A piece of a definition for anything with template parameters.
    This class can be used alongside structs, classes, and functions.
  **/
  struct definition_template: definition_scope {
    /** The definition to which template parameters here contained are applied. **/
    definition* def;
    
    typedef vector<definition_tempparam*> pvector;
    typedef pvector::iterator piterator;
    typedef pvector::const_iterator pciterator;
    pvector params;
    
    /// Class representing a single template specialization. Stored in a list of
    /// specializations which is mapped by arg_key fitting.
    struct specialization {
      spec_key key;
      arg_key filter;
      definition_template *spec_temp;
      //specialization(definition_template *stemp, const arg_key &specialization_key):
      //  key(stemp, specialization_key), spec_temp(stemp) {}
      specialization(size_t big_count, size_t small_count, definition_template* spect):
        key(big_count, small_count), filter(), spec_temp(spect) {}
      specialization(const specialization&);
      ~specialization();
    };
    
    /// Look up a specialization fitting an arg_key.
    /// @param key  The key to find a specialization for.
    /// @return  The best-fit specialization, or NULL if none was found.
    specialization *find_specialization(const arg_key &key) const;
    
    typedef vector<specialization*> speclist; ///< List of specializations fitting a certain abstracted arg_key
    typedef speclist::iterator speciter; ///< Map iterator type for specializations
    typedef speclist::const_iterator speciter_c; ///< Constant map iterator type for specializations
    
    struct instantiation {
      definition* def;
      vector<definition*> parameter_defs;
      instantiation(): def(NULL), parameter_defs() {}
      ~instantiation();
      private: instantiation(const instantiation&);
    };
    
    typedef map<arg_key, instantiation*> instmap; ///< Map type for instantiations
    typedef instmap::iterator institer; ///< Map iterator type for instantiations
    
    typedef vector<definition_hypothetical*> deplist; ///< Dependent member liat
    typedef deplist::iterator depiter; ///< Dependent member iterator
    
    struct dec_order_hypothetical: dec_order_g {
      definition_hypothetical *hyp;
      dec_order_hypothetical(definition_hypothetical *h): hyp(h) {}
      virtual definition *def();
      ~dec_order_hypothetical() {}
    };
    
    /** A map of all specializations **/
    speclist specializations;
    /** A map of all existing instantiations **/
    instmap instantiations;
    /** A map of all dependent members of our template parameters */
    deplist dependents;
    
    /** Instantiate this template with the values given in the passed key.
        If this template has been instantiated previously, that instantiation is given.
        @param key   The \c arg_key structure containing the template parameter values to use.
        @param errc  The \c error_context to which any problems will be reported. **/
    definition *instantiate(const arg_key& key, const error_context &errc);
    
    virtual string kind() const;
    virtual definition* duplicate(remap_set &n) const;
    virtual void remap(remap_set &n, const error_context &errc);
    virtual value size_of(const error_context &errc);
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const;
    
    /** Construct with name, parent, and flags **/
    definition_template(string name, definition *parent, unsigned flags);
    /** Destructor to free template parameters, instantiations, etc. **/
    ~definition_template();
  };
  
  /**
    @struct jdi::definition_tempparam
    A definition inheriting from definition_class, which is meant to represent a template parameter. Definitions in this
    class are considered hypothetical; they must exist in the type 
  */
  struct definition_tempparam: definition_class {
    AST *default_assignment;
    full_type integer_type;
    bool must_be_class; ///< Denotes to the compiler that this template parameter was used in a way only a class can be used (such as inheritance or member access).
    
    /** Construct with default information.
      @param name   Some unique key name for this scope.
      @param parent The scope above this one.
      @param flags  The additional flag data about this scope. */
    definition_tempparam(string name, definition_scope* parent, unsigned flags = DEF_TEMPPARAM | DEF_DEPENDENT);
    /** Construct with default information.
      @param name   Some unique key name for this scope.
      @param parent The scope above this one.
      @param defval The default value given to this parameter, read in as an AST to enable it to depend on other parameters.
      @param flags  The additional flag data about this scope. */
    definition_tempparam(string name, definition_scope* parent, AST* defval, unsigned flags = DEF_TEMPPARAM | DEF_DEPENDENT);
    /** Construct with default information.
      @param name    Some unique key name for this scope.
      @param parent  The scope above this one.
      @param inttype The integer type used if this template parameter is instead a numeric constant.
      @param defval  The default value given to this parameter, read in as an AST to enable it to depend on other parameters.
      @param flags   The additional flag data about this scope. */
    definition_tempparam(string name, definition_scope* parent, full_type &inttype, AST* defval = NULL, unsigned flags = DEF_TEMPPARAM | DEF_DEPENDENT);
    
    virtual string kind() const;
    virtual definition* duplicate(remap_set &n) const;
    virtual void remap(remap_set &n, const error_context &errc);
    
    virtual definition* look_up(string name); ///< Look up a definition in the parent of this scope (skip this scope). This function will never be used by the system.
    virtual decpair declare(string name, definition* def = NULL); ///< Declare a definition by the given name in this scope. The definition will be marked HYPOTHETICAL, and the \c must_be_class flag will be set.
    virtual definition* get_local(string name); ///< Behaves identically to declare if the given name does not exist, or else returns it. In either case, the returned definition will be HYPOTHETICAL.
    
    ~definition_tempparam();
  };
  
  /**
    @struct jdi::definition_atomic
    A definition for atomic types.
  */
  struct definition_atomic: definition_scope {
    definition* duplicate(remap_set &n);
    
    string kind() const;
    void remap(remap_set &n, const error_context &errc);
    value size_of(const error_context &errc);
    string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const;
    size_t sz;
    definition_atomic(string n,definition* p,unsigned int f, size_t size);
  };
  
  /**
    @struct jdi::definition_hypothetical
    A class representing a dependent (here called "hypothetical") type--a type which
    depends on an abstract parent or scope.
  */
  struct definition_hypothetical: definition_class {
    AST *def; ///< The expression this definition represents.
    jdip::context_parser *context_p; ///< A context/parser in which this definition exists, and will exist.
    unsigned int required_flags; ///< Set of flags required of any type provided to this template parameter
    
    virtual string kind() const;
    virtual definition* duplicate(remap_set &n) const;
    virtual void remap(remap_set &n, const error_context &errc);
    virtual value size_of(const error_context &errc);
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0) const;
    virtual definition* get_local(string name);
    
    /// Construct with basic definition info.
    definition_hypothetical(string name, definition_scope *parent, unsigned flags, AST* def);
    definition_hypothetical(string name, definition_scope *parent, AST* def);
    ~definition_hypothetical();
  };
  
  
  //==========================================================================================
  //===: User Definitions :===================================================================
  //==========================================================================================
  
  // These definitions are not used by JDI, but rather are provided as utility classes for the
  // end-user. Use them if you want. Or don't.
  
  /**
    A smart scope designed to be used (as in a using directive) by another scope.
    This class automatically adds itself to the using collection of the given scope
    on construct, and automatically removes itself on destruct.
  */
  struct using_scope: definition_scope {
    jdi::definition_scope::using_node* using_me; ///< The scope that is using us.
    /// Construct with name and user scope.
    /// @param name  The name of this scope, in case a trace is ever printed.
    /// @param user  The scope which will use this scope, as in a using directive.
    using_scope(string name, definition_scope* user);
    ~using_scope();
  };
}

#endif
