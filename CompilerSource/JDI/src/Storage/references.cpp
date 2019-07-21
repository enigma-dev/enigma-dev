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

#include "references.h"
#include <API/AST.h>

#ifndef dbg_assert
  #ifdef DEBUG_MODE
    #include <assert.h>
    #define dbg_assert assert
  #else
    #define dbg_assert(x)
  #endif
#endif
#include <iostream>
#include <cstdio>
using namespace std;

namespace jdi {
  ref_stack::ref_stack(): name(), ndef(NULL), ntop(NULL), nbottom(NULL), sz(0) {}
  ref_stack::ref_stack(ref_stack& rf): name(), ndef(NULL), ntop(NULL), nbottom(NULL), sz(0) { swap(rf); }
  ref_stack::ref_stack(const ref_stack& rf) { /* cerr << "IMPLICITLY DUPLICATED REF STACK (CTOR)" << endl; */ copy(rf); }
  ref_stack::~ref_stack() { clear(); }
  
  ref_stack &ref_stack::operator= (const ref_stack& rf) { clear(); cout << "IMPLICITLY DUPLICATED REF STACK (ASSN)" << endl; copy(rf); return *this; }
  
  ref_stack &ref_stack::operator= (ref_stack& rf) { swap(rf); return *this; }
  
  ref_stack::node::node(node* p, ref_type rt): previous(p), type(rt) {}
  ref_stack::node_array::node_array(node* p, size_t b): node(p,RT_ARRAYBOUND), bound(b) {}
  ref_stack::node_func::node_func(node* p, parameter_ct &ps): node(p,RT_FUNCTION), params() { params.swap(ps); }
  ref_stack::node_memptr::node_memptr(node* p, definition_class *memof): node(p,RT_MEMBER_POINTER), member_of(memof) {}
  
  ref_stack::node::~node() { }
  ref_stack::node_func::~node_func() {}
  ref_stack::node_memptr::~node_memptr() {}
  
  ref_stack::node* ref_stack::node::duplicate() {
    if (type == RT_ARRAYBOUND) return new node_array(NULL,((node_array*)this)->bound);
    if (type == RT_FUNCTION) return new node_func(NULL,((node_func*)this)->params);
    return new node(NULL,type);
  }
  
  size_t ref_stack::node::arraysize() const {
    dbg_assert(this->type == RT_ARRAYBOUND);
    return ((node_array*)this)->bound;
  }
  size_t ref_stack::node::paramcount() const {
    dbg_assert(this->type == RT_FUNCTION);
    return ((node_func*)this)->params.size();
  }
  
  ref_stack::node* ref_stack::iterator::operator*() { return n; }
  ref_stack::node* ref_stack::iterator::operator->() { return n; }
  ref_stack::iterator ref_stack::iterator::operator++(int) { iterator res = *this; n = n->previous; return res; }
  ref_stack::iterator &ref_stack::iterator::operator++() { n = n->previous; return *this; }
  ref_stack::iterator::operator bool() { return n; }
  ref_stack::iterator::iterator(ref_stack::node *nconstruct): n(nconstruct) { }
  
  ref_stack::iterator ref_stack::begin() const { return ref_stack::iterator(ntop); }
  ref_stack::iterator ref_stack::end() const { return ref_stack::iterator(NULL); }
  
  bool ref_stack::ends_with(const ref_stack &rf) const {
    for (const node *n1 = ntop, *n2 = rf.ntop; n2; n1 = n1->previous, n2 = n2->previous) {
      if (!n1) return false;
      if (*n1 != *n2) return false;
    }
    return true;
  }
  
  void ref_stack::push(ref_stack::ref_type reference_type) {
    ntop = new node(ntop, reference_type);
    if (!nbottom) nbottom = ntop;
    ++sz;
  }
  void ref_stack::push_array(size_t array_size) {
    node* bo = nbottom;
    nbottom = new node_array(NULL, array_size);
    if (bo) bo->previous = nbottom;
    else ntop = nbottom;
    ++sz;
  }
  void ref_stack::push_func(parameter_ct &parameters) {
    node* bo = nbottom;
    nbottom = new node_func(NULL, parameters);
    if (bo) bo->previous = nbottom;
    else ntop = nbottom;
    ++sz;
  }
  void ref_stack::push_memptr(definition_class *memof) {
    node* bo = nbottom;
    nbottom = new node_memptr(NULL, memof);
    if (bo) bo->previous = nbottom;
    else ntop = nbottom;
    ++sz;
  }
  
  void ref_stack::pop() {
    node *dme = ntop;
    if (dme) {
      ntop = dme->previous;
      if (!ntop)
        nbottom = NULL;
      free(dme);
    }
  }
  
  void ref_stack::free(node* n) {
    switch (n->type) {
      case RT_FUNCTION: delete (node_func*)n; break;
      case RT_ARRAYBOUND: delete (node_array*)n; break;
      case RT_MEMBER_POINTER: delete (node_memptr*)n; break;
      case RT_POINTERTO: case RT_REFERENCE: default: delete n; break;
    }
  }

  void ref_stack::copy(const ref_stack& rf) {
    name = rf.name;
    ndef = rf.ndef;
    sz = rf.sz;
    if (!rf.nbottom) {
      ntop = nbottom = NULL;
      return;
    }
    nbottom = ntop = rf.ntop->duplicate();
    for (node *c = rf.ntop->previous; c; c = c->previous) {
      nbottom->previous = c->duplicate();
      nbottom = nbottom->previous;
    }
  }
  void ref_stack::swap(ref_stack& rf) {
    name.swap(rf.name);
    definition* sdef = ndef;
    ndef = rf.ndef; rf.ndef = sdef;
    node *ts = ntop, *bs= nbottom;
    ntop = rf.ntop, nbottom = rf.nbottom;
    rf.ntop = ts, rf.nbottom = bs;
    const size_t ss = rf.sz;
    rf.sz = sz; sz = ss;
  }
  
  void ref_stack::append_c(ref_stack &rf) {
    if (!rf.nbottom) return; // Appending an empty stack is meaningless
    if (!nbottom) nbottom = rf.nbottom; // If we didn't have anything on our stack, our nbottom is now its nbottom.
    rf.nbottom->previous = ntop; // If we had anything on our stack, then our ntop item comes before its nbottom item.
    ntop = rf.ntop; // Since we threw that stack on ntop of ours, its ntop is now our ntop.
    rf.ntop = rf.nbottom = NULL; // Make sure it doesn't free what we just stole
    sz += rf.sz; rf.sz = 0; // Steal its size, too.
  }
  
  void ref_stack::append_nest_c(ref_stack &rf) {
    if (!rf.nbottom) {
      if (!rf.name.empty()) name = rf.name; // Grab the name, if it's meaningful
      if (rf.ndef) ndef = rf.ndef; // This way, we don't overwrite with NULL/""
      return; // Appending an empty stack is meaningless
    }
    if (!nbottom) nbottom = rf.nbottom; // If we didn't have anything on our stack, our nbottom is now its nbottom.
    rf.nbottom->previous = ntop; // If we had anything on our stack, then our ntop item comes before its nbottom item.
    ntop = rf.ntop; // Since we threw that stack on ntop of ours, its ntop is now our ntop.
    rf.ntop = rf.nbottom = NULL; // Make sure it doesn't free what we just stole
    sz += rf.sz; rf.sz = 0; // Steal its size, too.
    name = rf.name; // Steal the name from the nested expression.
    ndef = rf.ndef;
  }
  
  void ref_stack::prepend_c(ref_stack& rf) {
    if (!rf.nbottom) return; //Prepending an empty stack is meaningless
    if (!ntop) ntop = rf.ntop; // If we didn't have anything on our stack, our ntop is now its ntop.
    else nbottom->previous = rf.ntop;
    nbottom = rf.nbottom; // Since we threw that stack on ntop of ours, its ntop is now our ntop.
    rf.ntop = rf.nbottom = NULL; // Make sure it doesn't free what we just stole
    sz += rf.sz; rf.sz = 0; // Steal its size, too.
  }
  
  void ref_stack::prepend(const ref_stack& rf) {
    ref_stack n(rf);
    prepend_c(n);
  }
  
  void ref_stack::clear() {
    for (node* n = ntop, *p; n; n = p) {
      p = n->previous;
      free(n);
    }
    sz = 0;
  }
  
  bool ref_stack::empty() const { return !nbottom; }
  size_t ref_stack::size() const { return sz; }
  
  ref_stack::node& ref_stack::top() { return *ntop; }
  ref_stack::node& ref_stack::bottom() { return *nbottom; }
  const ref_stack::node& ref_stack::top() const { return *ntop; }
  const ref_stack::node& ref_stack::bottom() const { return *nbottom; }
  
  void ref_stack::parameter_ct::throw_on(parameter &ft) {
    enswap(ft);
  }
  
  ref_stack::parameter::parameter(): variadic(false), default_value(NULL) {}
  ref_stack::parameter::parameter(const full_type& ft, AST* dv): full_type(ft), variadic(false), default_value(dv) {}
  ref_stack::parameter::~parameter() { delete default_value; }
    
  void ref_stack::parameter::swap(ref_stack::parameter &param) {
    full_type::swap(param);
    
    register bool swb;
    swb = param.variadic;
    param.variadic = variadic;
    variadic = swb;
    
    AST* swast = param.default_value;
    param.default_value = default_value;
    default_value = swast;
  }
  
  void ref_stack::parameter::swap_in(full_type &param) {
    full_type::swap(param);
  }
  
  // ============================================================================================
  // =====: String depiction :===================================================================
  // ============================================================================================
  
  string ref_stack::toStringLHS() const {
    string res;
    iterator it = begin();
    while (it)
    {
      while (it and (it->type == RT_ARRAYBOUND || it->type == RT_FUNCTION)) ++it;
      while (it) {
        if (it->type == RT_POINTERTO)
          res = '*' + res;
        else if (it->type == RT_REFERENCE)
          res = '&' + res;
        else if (it->type == RT_MEMBER_POINTER)
          res = ((node_memptr*)(*it))->member_of->toString(0,0) + "::*" + res;
        else break;
        ++it;
      }
      if (it) res = '(' + res;
    }
    return res;
  }

  static inline string arraybound_string(size_t b) {
    if (b == ref_stack::node_array::nbound)
      return "[]";
    char buf[32]; sprintf(buf,"[%lu]",(long unsigned)b);
    return buf;
  }

  string ref_stack::toStringRHS() const {
    string res;
    iterator it = begin();
    while (it)
    {
      while (it and (it->type == RT_ARRAYBOUND || it->type == RT_FUNCTION)) {
        if (it->type == RT_ARRAYBOUND) res += arraybound_string(it->arraysize());
        else {
          res += '(';
          node_func* nf = (node_func*)*it;
          for (size_t i = 0; i < nf->params.size(); i++) {
            res += nf->params[i].variadic? "..." : nf->params[i].toString();
            if (nf->params[i].default_value) res += " = " + nf->params[i].default_value->toString();
            if (i + 1 < nf->params.size()) res += ", ";
          }
          res += ')';
        }
        ++it;
      }
      while (it and (it->type == RT_POINTERTO || it->type == RT_REFERENCE || it->type == RT_MEMBER_POINTER)) ++it;
      if (it) res += ')';
    }
    return res;
  }

  string ref_stack::toString() const {
    return toStringLHS() + name + toStringRHS();
  }
  
  string ref_stack::toEnglish() const {
    string res = name.empty()? name : name + ": ";
    bool pl = false;
    
    for (iterator it = begin(); it; ++it)
    switch (it->type) {
      case RT_ARRAYBOUND:     res += pl?           "arrays of ":           "array of "; pl = true;  break;
      case RT_FUNCTION:       res += pl? "functions returning ": "function returning "; pl = false; break;
      case RT_MEMBER_POINTER: res += pl?  "member pointers to ":  "member pointer to "; break;
      case RT_POINTERTO:      res += pl?         "pointers to ":         "pointer to "; break;
      case RT_REFERENCE:      res += pl?       "references to ":       "reference to "; break;
      default: res += "{error}"; break;
    }
    
    return res;
  }
  
  //================================================================================
  //====: Comparison operators :====================================================
  //================================================================================
  
  bool ref_stack::node::operator==(const ref_stack::node &j) const {
      if (type != j.type) return false;
      if (type == RT_ARRAYBOUND and arraysize() != j.arraysize()) return false;
      if (type == RT_FUNCTION and ((node_func*)this)->params != ((node_func*)&j)->params) return false;
      return true;
  }
  bool ref_stack::node::operator!=(const ref_stack::node &j) const {
      if (type != j.type) return true;
      if (type == RT_ARRAYBOUND and arraysize() != j.arraysize()) return true;
      if (type == RT_FUNCTION and ((node_func*)this)->params != ((node_func*)&j)->params) return true;
      return false;
  }
  
  #ifdef DEBUG_MODE
    #define doublecheck_size(i, j, res) \
    if (not(j and i)) {        \
      cerr << "ref_stack::size() lied" << endl; \
      return res;\
    }
  #else
    #define doublecheck_size(i, j, res) void()
  #endif
  
  bool ref_stack::operator==(const ref_stack& other) const {
    if (size() != other.size()) return false;
    for (node *i = ntop, *j = other.ntop; i or j; i = i->previous, j = j->previous) {
      doublecheck_size(i, j, false);
      if (*i != *j) return false;
    }
    return true;
  }
  bool ref_stack::operator!=(const ref_stack& other) const {
    if (size() != other.size()) return true;
    for (node *i = ntop, *j = other.ntop; i or j; i = i->previous, j = j->previous) {
      doublecheck_size(i, j, true);
      if (*i != *j) return true;
    }
    return false;
  }
  bool ref_stack::operator< (const ref_stack& other) const {
    if (size() < other.size()) return true;
    if (size() > other.size()) return false;
    for (node *i = ntop, *j = other.ntop; i or j; i = i->previous, j = j->previous) {
      doublecheck_size(i, j, true);
      if (i->type != j->type) return i->type < j->type;
      if (i->type == RT_ARRAYBOUND and i->arraysize() != j->arraysize()) return i->arraysize() < j->arraysize();
      if (i->type == RT_FUNCTION and ((node_func*)i)->params != ((node_func*)j)->params) return ((node_func*)i)->params < ((node_func*)j)->params;
    }
    return false;
  }
  bool ref_stack::operator> (const ref_stack& other) const {
    if (size() > other.size()) return true;
    if (size() < other.size()) return false;
    for (node *i = ntop, *j = other.ntop; i or j; i = i->previous, j = j->previous) {
      doublecheck_size(i, j, false);
      if (i->type != j->type) return i->type < j->type;
      if (i->type == RT_ARRAYBOUND and i->arraysize() != j->arraysize()) return i->arraysize() > j->arraysize();
      if (i->type == RT_FUNCTION and ((node_func*)i)->params != ((node_func*)j)->params) return ((node_func*)i)->params > ((node_func*)j)->params;
    }
    return false;
  }
  bool ref_stack::operator<= (const ref_stack& other) const {
    if (size() < other.size()) return true;
    if (size() > other.size()) return false;
    for (node *i = ntop, *j = other.ntop; i or j; i = i->previous, j = j->previous) {
      doublecheck_size(i, j, true);
      if (i->type != j->type) return i->type < j->type;
      if (i->type == RT_ARRAYBOUND and i->arraysize() != j->arraysize()) return i->arraysize() < j->arraysize();
      if (i->type == RT_FUNCTION and ((node_func*)i)->params != ((node_func*)j)->params) return ((node_func*)i)->params < ((node_func*)j)->params;
    }
    return false;
  }
  bool ref_stack::operator>= (const ref_stack& other) const {
    if (size() > other.size()) return true;
    if (size() < other.size()) return false;
    for (node *i = ntop, *j = other.ntop; i or j; i = i->previous, j = j->previous) {
      doublecheck_size(i, j, false);
      if (i->type != j->type) return i->type >= j->type;
      if (i->type == RT_ARRAYBOUND and i->arraysize() != j->arraysize()) return i->arraysize() > j->arraysize();
      if (i->type == RT_FUNCTION and ((node_func*)i)->params != ((node_func*)j)->params) return ((node_func*)i)->params > ((node_func*)j)->params;
    }
    return true;
  }
  
  bool ref_stack::parameter_ct::operator==(const ref_stack::parameter_ct& other) const {
    if (size() != other.size()) return false;
    for (size_t i = 0; i < size(); ++i)
      if ((*this)[i] != other[i]) return false;
    return true;
  }
  bool ref_stack::parameter_ct::operator!=(const ref_stack::parameter_ct& other) const {
    if (size() != other.size()) return true;
    for (size_t i = 0; i < size(); ++i)
      if ((*this)[i] != other[i]) return true;
    return false;
  }
  bool ref_stack::parameter_ct::operator>(const ref_stack::parameter_ct& other) const {
    if (size() > other.size()) return true;
    if (size() < other.size()) return false;
    for (size_t i = 0; i < size(); ++i)
      if ((*this)[i] != other[i]) return (*this)[i] > other[i];
    return false;
  }
  bool ref_stack::parameter_ct::operator<(const ref_stack::parameter_ct& other) const {
    if (size() < other.size()) return true;
    if (size() > other.size()) return false;
    for (size_t i = 0; i < size(); ++i)
      if ((*this)[i] != other[i]) return (*this)[i] < other[i];
    return false;
  }
  bool ref_stack::parameter_ct::operator>=(const ref_stack::parameter_ct& other) const {
    if (size() > other.size()) return true;
    if (size() < other.size()) return false;
    for (size_t i = 0; i < size(); ++i)
      if ((*this)[i] < other[i]) return (*this)[i] > other[i];
    return true;
  }
  bool ref_stack::parameter_ct::operator<=(const ref_stack::parameter_ct& other) const {
    if (size() < other.size()) return true;
    if (size() > other.size()) return false;
    for (size_t i = 0; i < size(); ++i)
      if ((*this)[i] != other[i]) return (*this)[i] < other[i];
    return true;
  }
  
  bool ref_stack::parameter::operator==(const ref_stack::parameter& other) const {
    return !default_value == !other.default_value and variadic == other.variadic;
  }
  bool ref_stack::parameter::operator!=(const ref_stack::parameter& other) const {
    return !default_value != !other.default_value or variadic != other.variadic;
  }
  bool ref_stack::parameter::operator<(const ref_stack::parameter& other) const {
    //if (default_value != other.default_value) return default_value < other.default_value;
    if (!default_value != !other.default_value) return !default_value > !other.default_value;
    if (variadic != other.variadic) return variadic < other.variadic;
    return false;
  }
  bool ref_stack::parameter::operator>(const ref_stack::parameter& other) const {
    //if (default_value != other.default_value) return default_value > other.default_value;
    if (!default_value != !other.default_value) return !default_value < !other.default_value;
    if (variadic != other.variadic) return variadic > other.variadic;
    return false;
  }
  bool ref_stack::parameter::operator<=(const ref_stack::parameter& other) const {
    //if (default_value != other.default_value) return default_value < other.default_value;
    if (!default_value != !other.default_value) return !default_value > !other.default_value;
    if (variadic != other.variadic) return variadic < other.variadic;
    return true;
  }
  bool ref_stack::parameter::operator>=(const ref_stack::parameter& other) const {
    //if (default_value != other.default_value) return default_value > other.default_value;
    if (!default_value != !other.default_value) return !default_value < !other.default_value;
    if (variadic != other.variadic) return variadic > other.variadic;
    return true;
  }
  
}
