/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#ifndef ENIGMA_MULTIFUNCTION_VARIANT_H
#define ENIGMA_MULTIFUNCTION_VARIANT_H

#include "var4.h"

namespace enigma
{
  template<class Kernel> struct multifunction_variant: evariant {
    //These are assignment operators and require a reference to be passed
    template<class T> multifunction_variant& operator=(const T &nv) {
      evariant old = std::move(*this);
      *(evariant*) this = nv;
      static_cast<Kernel*>(this)->function(old);
      return *this;
    }
    
    #define declare_relative_assign(op) template<class T>                      \
    multifunction_variant<Kernel> &operator op##=(const T &value) {            \
      evariant old = std::move(*(evariant*) this);                               \
      *(evariant*) this = old op value;                                         \
      static_cast<Kernel*>(this)->function(old);                               \
      return *this;                                                            \
    }

    declare_relative_assign(+)
    declare_relative_assign(-)
    declare_relative_assign(*)
    declare_relative_assign(/)
    declare_relative_assign(%)
    declare_relative_assign(<<)
    declare_relative_assign(>>)
    declare_relative_assign(&)
    declare_relative_assign(|)
    declare_relative_assign(^)

    #undef declare_relative_assign

    multifunction_variant() = default;
    template<typename T> explicit multifunction_variant(T t): evariant(t) {}
  };

} //namespace enigma

#define INHERIT_OPERATORS(T)\
  using multifunction_variant<T>::operator=; \
  T &operator=(const T &x) {                 \
    *this = (const evariant&) x;              \
    return *this;                            \
  }

#endif //ENIGMA_MULTIFUNCTION_VARIANT_H
