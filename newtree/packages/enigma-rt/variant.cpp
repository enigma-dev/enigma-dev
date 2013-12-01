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

namespace ert {
  static void assert_init(const variant_t& var) {
    if (var.type != variant::vt_uninit) {
      // TODO: error
    }
  }

  static void assert_real(const variant_t& var) {
    if (var.type != variant::vt_real) {
      // TODO: error
    }
  }

  static void assert_string(const variant_t& var) {
    if (var.type != variant::vt_string) {
      // TODO: error
    }
  }

  variant::variant()
    : type(vt_uninit) {
  }

  variant::variant(real_t val)
    : type(vt_real), real(val) {
  }

  variant::variant(string_t val)
    : type(vt_string), string(val) {
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

  variant& variant::operator =(const variant& rhs) {
    assert_init(rhs);
    this->type = rhs.type;
    this->real = rhs.real;
    this->string = rhs.string;
    return *this;
  }

  // TODO
  bool operator <(const variant&, const variant&) {
    return true;
  }
}
