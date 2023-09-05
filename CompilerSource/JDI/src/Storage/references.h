/**
 * @file  references.h
 * @brief System header declaring a structure for working with C referencers.
 * 
 * The asterisk pointer symbol (*), ampersand reference symbol (&), bracket
 * array bound indicators ([]), and function parameter parentheses ((*)())
 * are each unique types of references dealt with in this file.
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

#ifndef _REFERENCES__H
#define _REFERENCES__H

#include <string>
#include "definition_forward.h"

namespace jdi {
  using std::string;
  /**
    @struct jdi::ref_stack
    @brief  A stack-like structure representing C referencers.
    
    A reference stack is an info stack stored with any typed definition.
    The stack can depict any of C's standard referencers; the asterisk
    pointer symbol (*), the ampersand reference symbol (&), bracket array
    bound indicators ([]), and function parameter parentheses ((*)()).
    
    The stack does not depict parentheses used in grouping. Instead, the
    grouped referencers are simply added to the stack in the correct order.
    
    The stack is ordered such that each dereference symbol (listed above)
    can be evaluated simply by popping one item from a copy of the stack,
    performing each action as it is removed.
    
    As an example, this is the stack for int* (*(*a)[10][12])[15]:
    * RT_POINTERTO
    * RT_ARRAYBOUND(10)
    * RT_ARRAYBOUND(12)
    * RT_POINTERTO
    * RT_ARRAYBOUND(15)
    * RT_POINTERTO
  **/
  struct ref_stack {
    /// Types of referencers you'll find on this stack
    enum ref_type {
      RT_POINTERTO, ///< This referencer is a pointer-to asterisk, (*).
      RT_REFERENCE, ///< This referencer is a reference ampersand (&).
      RT_ARRAYBOUND, ///< This referencer is an array boundary subscript, [].
      RT_FUNCTION,   ///< This referencer is a set of function parameters.
      RT_MEMBER_POINTER ///< This referencer is a pointer to a member function
    };
    
    struct parameter;
    struct parameter_ct;
    
    class iterator;
    
    /// Node type.
    class node {
      node* previous; ///< The node beneath this node in the stack.
      friend struct ref_stack;
      friend struct ref_stack::iterator;
      node* duplicate(); ///< Actually duplicate this node
      ~node(); ///< Non-virtual destructor; node freeing is handled by ref_stack.
      public:
        ref_type type; ///< The type of this node.
        size_t arraysize() const; ///< Return the size of this array if and only if type == RT_ARRAYBOUND. Undefined behavior otherwise.
        size_t paramcount() const; ///< Return the number of parameters if and only if type == RT_FUNCTION. Undefined behavior otherwise.
        node(node* p, ref_type rt); ///< Allow constructing a new node easily.
        bool operator==(const node &other) const; ///< Test for equality.
        bool operator!=(const node &other) const; ///< Test for inequality.push_memptr
    };
    /// Node containing an array boundary.
    struct node_array;
    /// Node containing function parameters.
    struct node_func;
    /// Node containing a pointer to a class member function.
    struct node_memptr;
    
    /// Push a node onto this stack by a given type.
    /// @param reference_type The type of this reference; should be either \c RT_REFERENCE or \c RT_POINTERTO.
    void push(ref_type reference_type);
    /// Push an array node onto the bottom of this stack with the given boundary size.
    /// @param array_size  The number of elements in this array, or node_array::nbound for unspecified.
    void push_array(size_t array_size);
    /// Push a function node onto the bottom of this stack with the given parameter descriptors, consuming them.
    /// @param parameters  A \c parameter_ct to consume containing details about the parameters of this function.
    void push_func(parameter_ct &parameters);
    /// Push a class member pointer onto the bottom of this stack, from the given class definition.
    /// @param member_of  The \c definition_class to which this member pointer belongs.
    void push_memptr(definition_class *member_of);
    /// Append a stack to the top of this stack, consuming it.
    void append_c(ref_stack &rf);
    /// Similar to append_c, but for reference stacks composed in a nest. This method copies the name member as well.
    void append_nest_c(ref_stack &rf);
    /// Prepend a stack to the bottom of this stack, copying each element.
    void prepend(const ref_stack &rf);
    /// Prepend a stack to the bottom of this stack, consuming the stack.
    void prepend_c(ref_stack &rf);
    /// Pop a reference from this stack, or dereference it once.
    void pop();
    /// Clear the stack, undoing all referencers.
    void clear();
    
    bool operator==(const ref_stack& other) const; ///< Compare for equality across all three attributes.
    bool operator!=(const ref_stack& other) const; ///< Compare against equality across all three attributes.
    bool operator< (const ref_stack& other) const; ///< Inequality comparison, just in case someone needs to shove these in a map.
    bool operator> (const ref_stack& other) const; ///< Inequality comparison, just in case someone needs to shove these in a map.
    bool operator<= (const ref_stack& other) const; ///< Inequality comparison, just in case someone needs to shove these in a map.
    bool operator>= (const ref_stack& other) const; ///< Inequality comparison, just in case someone needs to shove these in a map.
    
    /// Return whether this stack is empty.
    bool empty() const;
    /// Returns whether this stack ends with the elements in another stack.
    bool ends_with(const ref_stack& rf) const;
    
    /// Constructor wrapper to the copy() method so copying doesn't bite someone in the ass.
    ref_stack(const ref_stack&);
    /// Wrapper to the copy() method so operator= doesn't leak and bite someone in the ass.
    ref_stack& operator= (const ref_stack& rf);
    /// Swap-action data transfer constructor: DOES NOT COPY. Implemented to combat ABYSMAL RVO in g++.
    ref_stack(ref_stack&);
    /// Swap-action data transfer operator: DOES NOT COPY. Implemented to combat ABYSMAL RVO in g++.
    ref_stack& operator= (ref_stack& rf);
    
    /// Make a copy of the given ref_stack, DISCARDING any stored nodes! Call clear() first.
    /// @warning If the stack is not empty upon invoking this method, memory will be leaked.
    void copy(const ref_stack &rf);
    /// Swap contents with another ref_stack. This method is completely safe.
    void swap(ref_stack &rf);
    
    /// Represent this set of referencers as a string.
    string toString() const;
    /// Represent the left-hand side of this set of referencers as a string (the part before the name).
    string toStringLHS() const;
    /// Represent the right-hand side of this set of referencers as a string (the part after the name).
    string toStringRHS() const;
    /// Represent this set of referencers as a string in plain English.
    string toEnglish() const;
    
    /// Get the top node.
    node &top();
    /// Get the bottom node.
    node &bottom();
    /// Get the top node without allowing modification.
    const node &top() const;
    /// Get the bottom node without allowing modification.
    const node &bottom() const;
    
    /// Free a node* pointer.
    static void free(node *n);
    
    /// Return the number of nodes contained.
    size_t size() const;
    
    /// Return iterator from topmost item.
    iterator begin() const;
    /// Return invalid iterator for comparison.
    iterator end() const;
    
    ref_stack(); ///< Default contructor. Zeroes pointers.
    ~ref_stack(); ///< Default destructor. Frees the stack.
    
    /// Iterator type, complying with C++11; iterates elements in the stack from top to bottom.
    /// Implements a boolean cast for short, simple iteration.
    struct iterator {
      private:
        node* n; ///< The node to which we are pointing.
        iterator(node*); ///< Utility constructor for use in begin().
        friend iterator ref_stack::begin() const; ///< Let the begin() function use this constructor.
        friend iterator ref_stack::end() const; ///< Let the end() function use this constructor.
      public:
        node* operator*(); ///< Get the current node pointer.
        node* operator->(); ///< Treat iterator as current node pointer.
        iterator operator++(int); ///< Increment iterator; move to next element on the stack. @return Return copy of iterator before call.
        iterator &operator++(); ///< Increment iterator; move to next element on the stack. @return Returns self.
        operator bool(); ///< Allow quick checking for invalidity; this iterator class does not use sentinel nodes.
    };
    
    string name; ///< The name of the object with the contained referencers.
    definition *ndef; ///< Any definition from which the name was derived.
    
    private:
      node *ntop;     ///< The topmost node on the list, for everything else.
      node *nbottom; ///< The bottommost node on the list; used in the prepend method.
      size_t sz;    ///< The number of nodes on the list
  };
}

//=========================================================================================================
//===: Specializations with extended dependencies :========================================================
//=========================================================================================================

#include <Storage/full_type.h>
#include <Storage/definition_forward.h>
#include <General/quickvector.h>
#include <API/AST_forward.h>

namespace jdi {
  /// Parameter storage type; contains type info and other important parameter info.
  struct ref_stack::parameter: full_type {
    bool variadic; ///< True if this parameter can be passed several values; in C/C++, this is mandated to be the last parameter.
    AST *default_value; ///< An AST if a default value was given. NULL otherwise.
    
    parameter(); ///< Default constructor.
    parameter(const full_type& ft, AST *default_value); ///< Construct by copying a full_type and accepting an optional default value AST.
    ~parameter(); ///< Destructor; frees the default value AST.
    void swap_in(full_type& param); ///< Swap contents with another parameter class.
    void swap(parameter& param); ///< Swap contents with another parameter class.
    
    bool operator==(const parameter& other) const; ///< Compare for equality across all three attributes.
    bool operator!=(const parameter& other) const; ///< Compare against equality across all three attributes.
    bool operator< (const parameter& other) const; ///< Inequality comparison, just in case someone needs to shove these in a map.
    bool operator> (const parameter& other) const; ///< Inequality comparison, just in case someone needs to shove these in a map.
    bool operator<= (const parameter& other) const; ///< Inequality comparison, just in case someone needs to shove these in a map.
    bool operator>= (const parameter& other) const; ///< Inequality comparison, just in case someone needs to shove these in a map.
  };
  /// Parameter storage container type. Guaranteed to have a push_back(full_type) method.
  struct ref_stack::parameter_ct: public quick::vector<parameter> {
    /// Throw a full_type onto this list, consuming it.
    /// @param ft  The \c full_type that will be consumed and added to the stack.
    void throw_on(parameter& ft);
    
    bool operator==(const parameter_ct& other) const; ///< Compare for equality across all three attributes.
    bool operator!=(const parameter_ct& other) const; ///< Compare against equality across all three attributes.
    bool operator< (const parameter_ct& other) const; ///< Inequality comparison, just in case someone needs to shove these in a map.
    bool operator> (const parameter_ct& other) const; ///< Inequality comparison, just in case someone needs to shove these in a map.
    bool operator<= (const parameter_ct& other) const; ///< Inequality comparison, just in case someone needs to shove these in a map.
    bool operator>= (const parameter_ct& other) const; ///< Inequality comparison, just in case someone needs to shove these in a map.
  };
  /// A special ref_stack node with an array bound size member.
  struct ref_stack::node_array: ref_stack::node {
    size_t bound; ///< The size of the bound, or \c nbound for an unspecified or non-const boundary size.
    static const size_t nbound = size_t(-1); ///< Value denoting an unspecified or non-const boundary size.
    /// Construct a new array bound node with previous node and a boundary size.
    /// @param p  The previous node.
    /// @param b  The boundary size, or \c nbound if no definite size is available.
    node_array(node* p, size_t b);
  };
  /// A special ref_stack node with a list of function parameters.
  struct ref_stack::node_func: ref_stack::node {
    parameter_ct params;
    /// Construct new function node with previous node and a parameter container, consuming the parameter container.
    /// @param p   The previous node.
    /// @param ps  The parameter container to consume.
    node_func(node* p, parameter_ct &ps);
    ~node_func();
  };
  /// A special ref_stack node with a list of function parameters.
  struct ref_stack::node_memptr: ref_stack::node {
    definition_class* member_of;
    /// Construct new function node with previous node and a parameter container, consuming the parameter container.
    /// @param p      The previous node.
    /// @param memof  The parameter container to consume.
    node_memptr(node* p, definition_class *memof);
    ~node_memptr();
  };
}

#endif
