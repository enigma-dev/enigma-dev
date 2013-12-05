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

#include "ert/real.hpp"
#include "ert/string.hpp"
#include "ert/variant.hpp"

#include <iostream>
#include <algorithm>

namespace ert {
  namespace {
    void assert_init(const variant_t& var) {
      if (var.type == variant::vt_uninit) {
        std::cerr << "error: attempted to access uninitialized variable" << std::endl;
        std::abort();
      }
    }
    
    void assert_real(const variant_t& var) {
      if (var.type != variant::vt_real) {
        std::cerr << "error: variable is not of type real" << std::endl;
        std::abort();
      }
    }
    
    void assert_string(const variant_t& var) {
      if (var.type != variant::vt_string) {
        std::cerr << "error: variable is not of type string" << std::endl;
        std::abort();
      }
    }
  }
  
  variant::variant(real_t val)
    : type(vt_real), real(val) {
  }
  
  variant::variant(string_t val)
    : type(vt_string), string(std::move(val)) {
  }

  variant::variant(variant && rhs)
    : type(rhs.type), real(rhs.real), string(std::move(rhs.string)) {
  }
  
  variant::operator real_t() const {
    assert_init(*this);
    assert_real(*this);
    return this->real;
  }
  
  variant::operator string_t() const {
    assert_init(*this);
    assert_string(*this);
    return this->string;
  }

  variant& variant::operator=(variant && rhs) {
    assert_init(rhs);
    this->type = rhs.type;
    this->real = rhs.real;
    this->string = std::move(rhs.string);
    return *this;
  }
  
  bool variant::operator <(const variant& rhs) const {
    assert_init(*this);
    if (this->type == variant::vt_real) {
      return this->real < static_cast<real_t>(rhs);
    }
    return std::lexicographical_compare(this->string.begin(), this->string.end(),
                                        rhs.string.begin(), rhs.string.end());
  }
  
  bool variant::operator >(const variant& rhs) const {
    assert_init(*this);
    if (this->type == variant::vt_real) {
      return this->real > static_cast<real_t>(rhs);
    }
    return std::lexicographical_compare(rhs.string.begin(), rhs.string.end(),
                                        this->string.begin(), this->string.end());
  }
}
