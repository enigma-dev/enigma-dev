/**
 * @file  definition.h
 * @brief System header declaring structures for representing C definitions.
 * 
 * This file is likely used by absolutely everything in the parse system, as
 * it is the medium through which output definitions are created and manipulated.
 * 
 * @section License
 * 
 * Copyright (C) 2011-2012 Josh Ventura
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

namespace jdi {
  /** Flags given to a definition to describe its type simply and quickly. **/
  enum DEF_FLAGS
  {
    DEF_TYPENAME =     1 <<  0, ///< This definition has template parameters attached.
    DEF_NAMESPACE =    1 <<  1, ///< This definition can be used as a typename. This does not imply that it has a valid type; see DEF_TYPED.
    DEF_CLASS =        1 <<  2, ///< This definition is a namespace.
    DEF_ENUM =         1 <<  3, ///< This definition is a class or structure. 
    DEF_UNION =        1 <<  4, ///< This definition is an enumeration of valued constants.
    DEF_SCOPE =        1 <<  5, ///< This definition is an enumeration of valued constants.
    DEF_TYPED =        1 <<  6, ///< This definition is a scope of some sort.
    DEF_FUNCTION =     1 <<  7, ///< This definition contains a type and referencer list. Used with DEF_TYPENAME to mean TYPEDEF.
    DEF_VALUED =       1 <<  8, ///< This definition is a function containing a list of zero or more overloads.
    DEF_DEFAULTED =    1 <<  9, ///< This definition has a default expression attached.
    DEF_EXTERN =       1 << 10, ///< This definition is a parameter of a template.
    DEF_TEMPLATE =     1 << 11, ///< This definition was declared with the "extern" flag.
    DEF_TEMPPARAM =    1 << 12, ///< This definition belongs to a list of template parameters, and is therefore abstract.
    DEF_HYPOTHETICAL = 1 << 13, ///< This definition is a purely hypothetical template type, eg, template_param::typename type;
    DEF_TEMPSCOPE =    1 << 14, ///< This definition is filling in for a scope; it contains a source definition which is not really a scope.
    DEF_PRIVATE =      1 << 15, ///< This definition was declared as a private member.
    DEF_PROTECTED =    1 << 16, ///< This definition was declared as a protected member.
    DEF_INCOMPLETE =   1 << 17, ///< This definition was declared but not implemented.
    DEF_ATOMIC =       1 << 18  ///< This is a global definition for objects of a fixed size, such as primitives.
  };
  
  struct definition;
  struct definition_typed;
  struct function_overload;
  struct definition_function;
  struct definition_valued;
  struct definition_scope;
  struct definition_class;
  struct definition_enum;
  struct definition_template;
  struct definition_atomic;
  struct definition_tempscope;
  struct definition_hypothetical;
  
  /// Structure for inserting declarations into a scope.
  struct decpair {
    quick::double_pointer<definition> def; ///< The definition that was there previously, or that was inserted if it did not exist.
    bool inserted; ///< True if the given definition was inserted, false otherwise.
    /** Construct with data.
      @param def        A pointer to the pointer to the definition that exists under the previously given key.
      @param inserted   True if a new entry was created under the given key, false if the key-value was left unchanged.
    */
    decpair(definition* *def, bool inserted);
  };
}


#include <map>
#include <string>
#include <vector>
#include <iostream>
using namespace std;
typedef size_t pt;

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
    
    /// Map type to contain definitions to remap along with the definition with which it will be replaced
    typedef map<definition*, definition*> remap_set;
    
    /** Duplicate this definition, whatever it may contain.
        The duplicated version must be freed separately.
        @return A pointer to a newly-allocated copy of this definition. **/
    virtual definition* duplicate(remap_set &n);
    
    /** Re-map all uses of each definition used as a key in the remap_set to the
        corresponding definition used as the value. For example, if the given map
        contains { <builtin_type__float, builtin_type__double> }, then any float
        contained in this definition or its descendents will be replaced with a
        double. This can be used to eliminate references to a particular definition
        before free, or to instantiate templates without hassle. **/
    virtual void remap(const remap_set& n);
    
    /** Return the size of this definition. **/
    virtual size_t size_of();
    
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
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0);
    
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
#include <API/error_reporting.h>
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
    
    virtual definition* duplicate(remap_set &n);
    virtual void remap(const remap_set &n);
    virtual size_t size_of();
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0);
    
    /// Construct with all information. Consumes the given \c ref_stack.
    definition_typed(string name, definition* p, definition* tp, unsigned int typeflags, int flags = DEF_TYPED);
    definition_typed(string name, definition* p, definition* tp, ref_stack &rf, unsigned int typeflags, int flags = DEF_TYPED);
    
    virtual ~definition_typed();
  };
  
  /** Structure containing template arguments; can be used as the key in an std::map. **/
  class arg_key {
  public:
    enum ak_type { AKT_NONE, AKT_FULLTYPE, AKT_VALUE };
    /** Improvised C++ Union of full_type and value. */
    struct node {
      struct antialias {
        char data[
          (((sizeof(full_type) > sizeof(value))? sizeof(full_type) : sizeof(value)) + sizeof(char) - 1)
          /sizeof(char)
        ];
      } data;
      ak_type type;
      
      inline const full_type& ft() const { return *(full_type*)&data; }
      inline const value& val() const { return *(value*)&data; }
      inline full_type& ft() { return *(full_type*)&data; }
      inline value& val() { return *(value*)&data; }
      node &operator= (const node& other);
      
      inline node(): type(AKT_NONE) {}
      ~node();
    };
    
    private:
      /// An array of all our values
      node *values;
      /// A pointer past our value array
      node *endv;
      
    public:
      static definition abstract; ///< A sentinel pointer marking that this parameter is still abstract.
      /// A comparator to allow storage in a map.
      bool operator<(const arg_key& other) const;
      /// A method to prepare this instance for storage of parameter values for the given template.
      void mirror(definition_template* temp);
      /// A fast function to assign to our list at a given index, consuming the given type.
      void swap_final_type(size_t argnum, full_type &type);
      /// A less fast function to assign to our list at a given index, copying the given type.
      void put_final_type(size_t argnum, const full_type &type);
      /// A slower function to put the most basic type representation down, consuming the given type
      void swap_type(size_t argnum, full_type &type);
      /// An even slower function to put the most basic type representation down, copying the given starting type
      void put_type(size_t argnum, const full_type &type);
      /// A quick function to put a value at a given index
      void put_value(size_t argnum, const value& val);
      /// A quick function to grab the type at a position
      node &operator[](int i) const { return values[i]; }
      /// A quick function to return an immutable pointer to the first parameter
      inline node* begin() { return values; }
      /// A quick function to return a pointer past the end of our list
      inline node* end() { return endv; }
      /// Default constructor; mark values NULL.
      arg_key();
      /// Construct with a size, reserving sufficient memory.
       arg_key(size_t n);
      /// Construct a copy.
      arg_key(const arg_key& other);
      /// Construct from a ref_stack.
      arg_key(const ref_stack& refs);
      /// Destruct, freeing items.
      ~arg_key();
  };
  
  /**
    @struct jdi::function_overload
    A structure detailing an alternate overload of a function without using too much memory.
  **/
  struct function_overload {
    full_type type; ///< The non-null return type of this overload.
    string declaration; ///< The full prototype for the function.
    function_overload *duplicate(); ///< Make a copy
  };
  
  /**
    @struct jdi::definition_function
    A piece of a definition specifically for functions.
    The class is based on implements a method of storing overload information.
  **/
  struct definition_function: definition_typed {
    virtual definition* duplicate(remap_set &n);
    virtual void remap(const remap_set &n);
    virtual size_t size_of();
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0);
    
    typedef map<arg_key, definition_function*> overload_map;
    typedef overload_map::iterator overload_iter;
    
    overload_map overloads; ///< Standard overloads, checked before template overloads.
    vector<definition_template*> template_overloads; ///< Array of reference stacks for each overload of this function.
    void *implementation; ///< The implementation of this function as harvested by an external system.
    
    /** Function to add the given definition as an overload if no such overload
        exists, or to merge it in (handling any errors) otherwise.
        @param key    The arg_key structure to consume, representing the parameter combination.
        @param ovrl   The definition representing the new overload; this definition will
                      belong to the system after you pass it.
        @param errtok Token to facilitate error reporting.
        @param herr   Error handler to report problems to.
        @return Returns the final definition for the requested overload.
    */
    definition *overload(arg_key &key, definition_function* ovrl, error_handler *herr);
    
    /** Function to add the given definition as a template overload
        exists, or to merge it in (handling any errors) otherwise.
        @param ovrl  The template definition representing the new overload; this definition
                     will belong to the system after you pass it.
    */
    void overload(definition_template* ovrl);
    
    definition_function(string name, definition* p, definition* tp, ref_stack &rf, unsigned int typeflags, int flags = DEF_FUNCTION);
    virtual ~definition_function();
  };
  
  
  /**
    @struct jdi::definition_valued
    A subclass of definition for anything declared with a type, and given a value.
  **/
  struct definition_valued: definition_typed {
    value value_of; ///< The constant value of this definition.
    definition_valued(); ///< Default constructor; invalidates value.
    
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0);
    
    //definition_valued(string vname, definition *parnt, definition* type, unsigned int flags, value &val); ///< Construct with a value and type.
    definition_valued(string vname, definition *parnt, definition* type, unsigned int modifiers, unsigned int flags, value &val); ///< Construct with a value and type.
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
    /** Remove a previously added namespace from our using list. **/
    void unuse_namespace(using_node *ns);
    /** Add a namespace to the using list. This can technically be used on any scope. **/
    void use_general(string name, definition* def);
    
    /** Free all contents of this scope. No copy is made. **/
    void clear();
    /** Relinquish all content to another definition. **/
    void dump(definition_scope* to);
    /** Copy content from another definition. **/
    void copy(const definition_scope* from);
    /** Swap content with another definition. **/
    void swap(definition_scope* with);
    
    /** Look up a \c definition* given its identifier.
        @param name  The identifier by which the definition can be referenced. This is NOT qualified!
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
    definition* find_local(string name);
    
    virtual definition* duplicate(remap_set &n);
    virtual void remap(const remap_set &n);
    virtual size_t size_of();
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0);
    
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
    virtual definition* duplicate(remap_set &n);
    virtual void remap(const remap_set &n);
    virtual size_t size_of();
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0);
    
    /// Simple structure for storing an inheritance type and the \c definition* of an ancestor.
    struct ancestor {
      unsigned protection; ///< The protection level of this inheritance, as one of the DEF_ constants, or 0 for public.
      definition_class* def; ///< The \c definition of the structure or class from which this one inherits members.
      ancestor(unsigned protection_level, definition_class* inherit_from); ///< Convenience constructor with both members.
      ancestor(); ///< Default constructor for vector.
    };
    
    virtual definition* look_up(string name); ///< Look up a definition in this class (including its ancestors).
    virtual decpair declare(string name, definition* def = NULL); ///< Declare a definition by the given name in this scope.
    vector<ancestor> ancestors; ///< Ancestors of this structure or class
    definition_class(string classname, definition_scope* parent, unsigned flags = DEF_CLASS | DEF_TYPENAME);
  };
  
  /**
    @struct jdi::definition_union
    An extension of \c jdi::definition_scope for unions, which have a unified tyoe and unique sizeof() operator.
  **/
  struct definition_union: definition_scope {
    virtual definition* duplicate(remap_set &n);
    virtual void remap(const remap_set &n);
    virtual size_t size_of();
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0);
    
    definition_union(string classname, definition_scope* parent, unsigned flags = DEF_CLASS | DEF_UNION | DEF_TYPENAME);
  };
  
  /**
    @struct jdi::definition_enum
    An extension of \c jdi::definition for enums, which contain mirrors of members in the parent scope.
  **/
  struct definition_enum: definition_typed {
    virtual definition* duplicate(remap_set &n);
    virtual void remap(const remap_set &n);
    virtual size_t size_of();
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0);
    
    definition_scope::defmap constants;
    definition_enum(string classname, definition_scope* parent, unsigned flags = DEF_ENUM | DEF_TYPENAME);
  };
  
  /**
    @struct jdi::definition_template
    A piece of a definition for anything with template parameters.
    This class can be used alongside structs, classes, and functions.
  **/
  struct definition_template: definition {
    /** The definition to which template parameters here contained are applied. **/
    definition* def;
    /** A list of template parameters or arguments, as typedefs.
        The definition pointed to by this vector must be a typedef.
        
        If the typedef is to NULL, the type is completely abstract. Otherwise,
        if the \c DEF_DEFAULTED flag is set, the type being pointed to is the
        default for this template parameter. If the type is non-null and this
        flag is not set, then this parameter has been specified in a (partial)
        instantiation of the parent function/class (the definition containing
        the vector).
    **/
    vector<definition*> params;
    
    virtual definition* duplicate(remap_set &n);
    virtual void remap(const remap_set &n);
    virtual size_t size_of();
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0);
    
    typedef map<arg_key,definition_template*> specmap; ///< Map type for specializations
    typedef specmap::iterator speciter; ///< Map iterator type for specializations
    
    typedef map<arg_key,definition*> instmap; ///< Map type for instantiations
    typedef instmap::iterator institer; ///< Map iterator type for instantiations
    
    typedef vector<definition_hypothetical*> deplist; ///< Dependent member liat
    typedef deplist::iterator depiter; ///< Dependent member iterator
    
    /** A map of all specializations **/
    specmap specializations;
    /** A map of all existing instantiations **/
    instmap instantiations;
    /** A map of all dependent members of our template parameters */
    deplist dependents;
    
    /** Instantiate this template with the values given in the passed key.
        If this template has been instantiated previously, that instantiation is given.
        @param key  The \c arg_key structure containing the template parameter values to use. **/
    definition *instantiate(arg_key& key);
    /** Specialize this template with the values given in the passed key.
        If the specialization exists, it is returned. Otherwise, a new specialization
        is created with the flag DEF_INCOMPLETE to signify that it contains no unique definition.
        @param key  The \c arg_key structure containing the template parameter values to use.
        @param ts   The temporary scope allocated to store the template. **/
    definition_template *specialize(arg_key& key, definition_tempscope* ts);
    
    /** Construct with name, parent, and flags **/
    definition_template(string name, definition *parent, unsigned flags);
    /** Destructor to free template parameters, instantiations, etc. **/
    ~definition_template();
  };
  
  /**
    @struct jdi::definition_atomic
    A definition for atomic types.
  */
  struct definition_atomic: definition_scope {
    definition* duplicate(remap_set &n);
    void remap(const remap_set &n);
    size_t size_of();
    string toString(unsigned levels = unsigned(-1), unsigned indent = 0);
    size_t sz;
    definition_atomic(string n,definition* p,unsigned int f, size_t size);
  };
  
  /**
    @struct jdi::definition_tempscope
    A class which can be used as a temporary scope with a definition attached.
  */
  struct definition_tempscope: definition_scope {
    definition *source; ///< The definition for which this subscope was created.
    bool referenced; ///< Whether this->source has been consumed by another object.
    /** Construct with default information.
      @param name   Some unique key name for this scope.
      @param parent The scope above this one.
      @param flags  The additional flag data about this scope.
      @param source The definition with which this scope is affiliated, however loosely. */
    definition_tempscope(string name, definition* parent, unsigned flags, definition *source);
    
    virtual definition* duplicate(remap_set &n);
    virtual void remap(const remap_set &n);
    
    virtual definition* look_up(string name); ///< Look up a definition in the parent of this scope (skip this temp scope).
    virtual decpair declare(string name, definition* def = NULL); ///< Declare a definition by the given name in this scope.
  };
  
  /**
    @struct jdi::definition_hypothetical
    A class representing a dependent (here called "hypothetical") type--a type which
    depends on an abstract parent or scope.
  */
  struct definition_hypothetical: definition_class {
    AST *def;
    virtual definition* duplicate(remap_set &n);
    virtual void remap(const remap_set &n);
    virtual size_t size_of();
    virtual string toString(unsigned levels = unsigned(-1), unsigned indent = 0);
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
