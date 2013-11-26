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

#ifndef ERT_BUFFER_HPP_
#define ERT_BUFFER_HPP_

#include "string.hpp"

namespace ert {
  // Buffer Constants
  enum {
    buffer_fixed = 0,
    buffer_grow,
    buffer_wrap,
    buffer_fast
  };
  enum {
    buffer_u8 = 0,
    buffer_s8,
    buffer_u16,
    buffer_s16,
    buffer_u32,
    buffer_s32,
    buffer_f16,
    buffer_f32,
    buffer_f64,
    buffer_bool,
    buffer_string
  };
  enum {
    buffer_seek_start = 0,
    buffer_seek_relative,
    buffer_seek_end
  }

  // Buffer Functions
  double buffer_create(double, double, double);
  double buffer_delete(double);
  variant_t buffer_read(double, double);
  double buffer_write(double, double, const variant_t&);
  double buffer_fill(double, double, double, const variant_t&, double);
  double buffer_seek(double, double, double);
  double buffer_tell(double);
  variant_t buffer_peek(double, double, double);
  double buffer_poke(double, double, double, const variant_t&);
  double buffer_save(double, const string_t&);
  double buffer_save_ext(double, const string_t&, double, double);
  double buffer_load(const string_t&);
  double buffer_load_ext(double, const string_t&, double);
  double buffer_copy(double, double, double, double, double);
  double buffer_get_size(double);
  double buffer_resize(double, double);
  double buffer_sizeof(double);
  string_t buffer_md5(double, double, double);
  string_t buffer_sha1(double, double, double);
  string_t buffer_base64_encode(double, double, double);
  double buffer_base64_decode(const string_t&);
}

#endif // ERT_BUFFER_HPP_
