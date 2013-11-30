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

#ifndef ERT_VARARGS_HPP_
#define ERT_VARARGS_HPP_

#include "ert/variant.hpp"

namespace ert {
  const unsigned MAX_VARGS = 15;

  template <typename T>
  struct varargs {
  public:
    varargs()
      : argc(0) {
    }

    varargs& operator ,(const T& arg) {
      this->argv[this->argc++] = arg;
      return *this;
    }

    unsigned argc;
    T argv[MAX_VARGS];
  };
}

#endif // ERT_VARARGS_HPP_
