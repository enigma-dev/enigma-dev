/**
 * @file  references.cpp
 * @brief Source implementing methods for dealing with C referencers as a structure.
 * 
 * The asterisk pointer symbol (*), ampersand reference symbol (&), bracket
 * array bound indicators ([]), and function parameter parentheses ((*)())
 * are each unique types of references dealt with in this file.
 * 
 * @section License
 * 
 * Copyright (C) 2011 Josh Ventura
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

#include "references.h"

#ifndef dbg_assert
  #ifdef DEBUG_MODE
    #include <assert.h>
    #define dbg_assert assert
  #else
    #define dbg_assert(x)
  #endif
#endif
#include <iostream>

namespace jdi {
  ref_stack::ref_stack(): bottom(NULL), top(NULL) {}
  ref_stack::~ref_stack() { clear(); }
  
  ref_stack::ref_stack(const ref_stack& rf) { copy(rf); }
  ref_stack &ref_stack::operator= (const ref_stack& rf) { clear(); copy(rf); return *this; }
  
  ref_stack::ref_stack(ref_stack& rf): bottom(NULL), top(NULL) { swap(rf); }
  ref_stack &ref_stack::operator= (ref_stack& rf) { swap(rf); return *this; }
  
  ref_stack::node::node(node* p, ref_type rt): previous(p), type(rt) {}
  ref_stack::node_array::node_array(node* p, size_t b): node(p,RT_ARRAYBOUND), bound(b) {}
  ref_stack::node_func::node_func(node* p, parameter_ct &ps): node(p,RT_FUNCTION), params() { params.swap(ps); }
  
  ref_stack::node::~node() { }
  ref_stack::node_func::~node_func() {}
  
  ref_stack::node* ref_stack::node::duplicate() {
    if (type == RT_ARRAYBOUND) return new node_array(NULL,((node_array*)this)->bound);
    if (type == RT_FUNCTION) return new node_func(NULL,((node_func*)this)->params);
    cout << "DUPLICATE CALLED" << endl;
    return new node(NULL,type);
  }
  
  size_t ref_stack::node::arraysize() {
    dbg_assert(this->type == RT_ARRAYBOUND);
    return ((node_array*)this)->bound;
  }
  
  ref_stack::node* ref_stack::iterator::operator*() { return n; }
  ref_stack::node* ref_stack::iterator::operator->() { return n; }
  ref_stack::iterator ref_stack::iterator::operator++(int) { iterator res = *this; n = n->previous; return res; }
  ref_stack::iterator &ref_stack::iterator::operator++() { n = n->previous; return *this; }
  ref_stack::iterator::operator bool() { return n; }
  ref_stack::iterator::iterator(ref_stack::node *nconstruct): n(nconstruct) { }
  
  ref_stack::iterator ref_stack::begin() { return ref_stack::iterator(top); }
  ref_stack::iterator ref_stack::end() { return ref_stack::iterator(NULL); }
  
  void ref_stack::push(ref_stack::ref_type reference_type) {
    top = new node(top, reference_type);
    if (!bottom) bottom = top;
  }
  void ref_stack::push_array(size_t array_size) {
    node* bo = bottom;
    bottom = new node_array(NULL, array_size);
    if (bo) bo->previous = bottom;
    else top = bottom;
  }
  void ref_stack::push_func(parameter_ct &parameters) {
    node* bo = bottom;
    bottom = new node_func(NULL, parameters);
    if (bo) bo->previous = bottom;
    else top = bottom;
  }
  
  void ref_stack::copy(const ref_stack& rf) {
    name = rf.name;
    cout << "DUPLICATED REF STACK" << endl;
    if (!rf.bottom) {
      top = bottom = NULL;
      return;
    }
    bottom = top = rf.top->duplicate();
    for (node *c = rf.top->previous; c; c = c->previous) {
      bottom->previous = c->duplicate();
      bottom = bottom->previous;
    }
  }
  void ref_stack::swap(ref_stack& rf) {
    name.swap(rf.name);
    node *ts = top, *bs= bottom;
    top = rf.top, bottom = rf.bottom;
    rf.top = ts, rf.bottom = bs;
  }
  
  void ref_stack::append(ref_stack &rf) {
    if (!rf.bottom) return; // Appending an empty stack is meaningless
    if (!bottom) bottom = rf.bottom; // If we didn't have anything on our stack, our bottom is now its bottom.
    rf.bottom->previous = top; // If we had anything on our stack, then our top item comes before its bottom item.
    top = rf.top; // Since we threw that stack on top of ours, its top is now our top.
    rf.top = rf.bottom = NULL; // Make sure it doesn't free what we just stole
  }
  void ref_stack::append_nest(ref_stack &rf) {
    if (!rf.bottom) {
      if (!rf.name.empty()) name = rf.name; // Grab the name, if it's meaningful
      return; // Appending an empty stack is meaningless
    }
    if (!bottom) bottom = rf.bottom; // If we didn't have anything on our stack, our bottom is now its bottom.
    rf.bottom->previous = top; // If we had anything on our stack, then our top item comes before its bottom item.
    top = rf.top; // Since we threw that stack on top of ours, its top is now our top.
    rf.top = rf.bottom = NULL; // Make sure it doesn't free what we just stole
    name = rf.name; // Steal the name from the nested expression.
  }
  
  void ref_stack::clear() {
    for (node* n = top, *p; n; n = p) {
      p = n->previous;
      switch (n->type) {
        case RT_FUNCTION: delete (node_func*)n; break;
        case RT_ARRAYBOUND: delete (node_array*)n; break;
        case RT_POINTERTO: case RT_REFERENCE: default: delete n; break;
      }
    }
  }
  
  bool ref_stack::empty() { return !bottom; }
  
  void ref_stack::parameter_ct::throw_on(full_type &ft) {
    enswap(ft);
  }
}
