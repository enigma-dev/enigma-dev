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
#include "ert/variant.hpp"
#include "ert/varargs.hpp"

namespace ert {
  template <typename T>
  varargs<T>::varargs()
    : argc(0) {
  }

  template <typename T>
  varargs<T>& varargs<T>::operator ,(const T& val) {
    this->argv[this->argc++] = val;
    return *this;
  }

  template varargs<real_t>::varargs();
  template varargs<real_t>& varargs<real_t>::operator ,(const real_t&);

  template varargs<variant_t>::varargs();
  template varargs<variant_t>& varargs<variant_t>::operator ,(const variant_t&);
}
