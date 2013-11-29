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

#ifndef ERT_VARIANT_HPP_
#define ERT_VARIANT_HPP_

#include "ert/real.hpp"
#include "ert/string.hpp"

namespace ert {
  struct variant {
    variant();
    variant(real_t);
    variant(string_t);
    operator real_t() const;
    operator string_t() const;
    operator bool() const;
    bool operator !() const;
    variant& operator =(const variant&);

    // Binary Operations
    static variant& add(const variant&, const variant&);
    static variant& sub(const variant&, const variant&);
    static variant& mul(const variant&, const variant&);
    static variant& div(const variant&, const variant&);
    static variant& and(const variant&, const variant&);
    static variant& or(const variant&, const variant&);
    static variant& xor(const variant&, const variant&);
    static variant& lsh(const variant&, const variant&);
    static variant& rsh(const variant&, const variant&);
    static variant& eq(const variant&, const variant&);
    static variant& neq(const variant&, const variant&);
    static variant& gt(const variant&, const variant&);
    static variant& gte(const variant&, const variant&);
    static variant& lt(const variant&, const variant&);
    static variant& lte(const variant&, const variant&);

    enum type_t {
      vt_uninit = 0,
      vt_real,
      vt_string
    } type;

    real_t real;
    string_t string;
  };

  typedef variant variant_t;
}

#endif // ERT_VARIANT_HPP_
