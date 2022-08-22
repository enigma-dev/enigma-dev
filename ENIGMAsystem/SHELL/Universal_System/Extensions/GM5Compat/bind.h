//
// Copyright (C) 2014 Seth N. Hetu
//
// This file is a part of the ENIGMA Development Environment.
//
// ENIGMA is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, version 3 of the license or any later version.
//
// This application and its source code is distributed AS-IS, WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along
// with this code. If not, see <http://www.gnu.org/licenses/>
//


#ifndef ENIGMA_GM5COMPAT_BIND_H
#define ENIGMA_GM5COMPAT_BIND_H

#include "Universal_System/scalar.h"


//This file contains code used to bind one variable to another, existing one.
//This allows you to use the equality operator to update a "compat" variable, and
//have the changes mirrored in a mainstream variable.


namespace enigma
{

//A read-only binding for "variable" arrays (such as view variables).
class BindArrayRO {
public:
  explicit BindArrayRO(var& orig) : orig(orig) {}
  virtual ~BindArrayRO() {} //Give it a vtable

public:
  //Retrieve based on index.
  const evariant& operator[](int id) const {
    return orig[id];
  }
  const evariant& operator()(int id) const {
    return orig(id);
  }

  //Retrieve index zero.
  operator cs_scalar() const {
    return (*this)[0];
  }

  //Common operations.
  template <typename T>
  var operator+(const T& rhs) const {
    return orig + rhs;
  }
  template <typename T>
  var operator-(const T& rhs) const {
    return orig - rhs;
  }
  template <typename T>
  var operator*(const T& rhs) const {
    return orig * rhs;
  }
  template <typename T>
  var operator/(const T& rhs) const {
    return orig / rhs;
  }

protected:
  var& orig; //The value we are binding to.
};


//A read-write binding for "variable" arrays (such as view variables).
class BindArrayRW : public BindArrayRO {
public:
  explicit BindArrayRW(var& orig) : BindArrayRO(orig) {}
  virtual ~BindArrayRW() {} //Good practice

  //Retrieve based on index.
  evariant& operator[](int id) {
    return orig[id];
  }
  evariant& operator()(int id) {
    return orig(id);
  }

  //Set it (index 0)
  template <typename T>
  BindArrayRW& operator=(const T& val) {
    (*this)[0] = val;
    return *this;
  }
};


//A read-write binding for something with a getter/setter (such as draw_get/set_color())
class BindPropRW {
public:
  explicit BindPropRW(int (*get)(), void (*set)(int)) : get(get), set(set) {}

  //Get
  operator int() const {
    return get();
  }

  //set() overrides
  BindPropRW& operator=(gs_scalar val) {
    set(static_cast<int>(val)); //Truncate
    return *this;
  }
  BindPropRW& operator=(const BindPropRW& val) {
    set(static_cast<int>(val));
    return *this;
  }

  //Set
  template <typename T>
  BindPropRW& operator=(const T& val) {
    set(val);
    return *this;
  }

protected:
  //Getter/setter function.
  int (*get)();
  void (*set)(int);
};


}


#endif // ENIGMA_GM5COMPAT_BIND_H
