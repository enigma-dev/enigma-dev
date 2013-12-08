/******************************************************************************

 ENIGMA
 Copyright (C) 2008-2013 Enigma Strike Force

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#ifndef ERT_PROPERTY_HPP_
#define ERT_PROPERTY_HPP_

#include <functional>

namespace ert {
  template <typename C, typename T, T (C::*getter)()>
  struct property_ro {
    property_ro(C *ptr)
      : owner(ptr) {
    }
    property_ro(property_ro&&) = default;
    property_ro(const property_ro&) = delete;
    property_ro& operator=(const property_ro&) = delete;
    property_ro& operator=(property_ro&&) = delete;
    
    operator T() {
      return (this->owner->*getter)();
    }
    
  private:
    C* const owner;
  };
  
  template <typename C, typename T, T (C::*getter)(), void (C::*setter)(T)>
  struct property {
    property(C *ptr)
      : owner(ptr) {
    }
    property(property&&) = default;
    property(const property&) = delete;
    property& operator=(const property&) = delete;
    property& operator=(property&&) = delete;
    
    const T& operator=(const T& val) {
      (this->owner->*setter)(val);
      return val;
    }
    
    T&& operator=(T&& val) {
      (this->owner->*setter)(std::move(val));
      return (this->owner->*getter)();
    }
    
    operator T() {
      return (this->owner->*getter)();
    }
    
  private:
    C* const owner;
  };
}

#endif // ERT_PROPERTY_HPP_
