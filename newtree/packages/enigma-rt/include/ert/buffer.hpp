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

namespace ert {
  // Buffer Constants
  enum {
    buffer_fixed  = 0,
    buffer_grow   = 1,
    buffer_wrap   = 2,
    buffer_fast   = 3
  };
  enum {
    buffer_u8     = 0,
    buffer_s8     = 1,
    buffer_u16    = 2,
    buffer_s16    = 3,
    buffer_u32    = 4,
    buffer_s32    = 5,
    buffer_f16    = 6,
    buffer_f32    = 7,
    buffer_f64    = 8,
    buffer_bool   = 9,
    buffer_string = 10
  };
  enum {
    buffer_seek_start     = 0,
    buffer_seek_relative  = 1,
    buffer_seek_end       = 2
  }
  // Vector Functions
  double buffer_create(double, double, double);
  double buffer_delete(double);
  variant buffer_read(double, double);
  double buffer_write(double, double, const variant&);
  double buffer_fill(double, double, double, const variant&, double);
  double buffer_seek(double, double, double);
  double buffer_tell(double);
  variant buffer_peek(double, double, double);
  double buffer_poke(double, double, double, const variant&);
  double buffer_save(double, const string&);
  double buffer_save_ext(double, const string&, double, double);
  double buffer_load(const string&);
  double buffer_load_ext(double, const string&, double);
  double buffer_copy(double, double, double, double, double);
  double buffer_get_size(double);
  double buffer_resize(double, double);
  double buffer_sizeof(double);
  string buffer_md5(double, double, double);
  string buffer_sha1(double, double, double);
  string buffer_base64_encode(double, double, double);
  double buffer_base64_decode(const string&);
}

#endif // ERT_BUFFER_HPP_
