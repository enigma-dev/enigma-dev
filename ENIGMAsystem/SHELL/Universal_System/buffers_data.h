/** Copyright (C) 2013 Robert B. Colton
*** Copyright (C) 2022 Dhruv Chawla
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#ifndef ENIGMA_BUFFERS_ENUMS_H
#define ENIGMA_BUFFERS_ENUMS_H

#include <cstdint>

namespace enigma_user {
// Other buffer constants
enum {
  buffer_generalerror,
  buffer_invalidtype,
  buffer_outofbounds,
  buffer_outofspace,
  buffer_vbuffer,
};

// Buffer surface constants
enum { buffer_surface_copy, buffer_surface_grayscale, buffer_surface_mask };

// Buffer seek base
enum buffer_seek_t { buffer_seek_start = 0, buffer_seek_relative = 1, buffer_seek_end = 2 };

// Buffer types
enum buffer_type_t { buffer_fixed = 0, buffer_grow = 1, buffer_wrap = 2, buffer_fast = 3 };

// Buffer data types
enum buffer_data_t {
  buffer_u8 = 1,
  buffer_s8 = 2,
  buffer_u16 = 3,
  buffer_s16 = 4,
  buffer_u32 = 5,
  buffer_s32 = 6,
  buffer_f16 = 7,
  buffer_f32 = 8,
  buffer_f64 = 9,
  buffer_bool = 10,
  buffer_string = 11,
  buffer_u64 = 12,
  buffer_text = 13,
};

/// @brief Handle to a buffer
///
/// This value is a signed 64-bit integer to allow -1 being the error case, and 9,223,372,036,854,775,807 buffers is
/// probably more than anyone will ever need to deal with.
using buffer_t = std::int64_t;
} // namespace enigma_user

#endif