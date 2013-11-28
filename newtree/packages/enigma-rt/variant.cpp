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
    if (this->type != vt_real) {
      // TODO: show error
      throw;
    }
    // varargs_t n = (varargs_t(3), 4, 5, 6);
    // choose((varargs_t(3),4,5,6));
    return this->real;
  }

  variant::operator string_t() const {
    if (this->type != vt_string) {
      // TODO: show error
      throw;
    }
    return this->string;
  }
}
