/** Copyright (C) 2022 Dhruv Chawla
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

#ifndef ENIGMA_SERIALIZE_POINTER_BYTES_H
#define ENIGMA_SERIALIZE_POINTER_BYTES_H

#include "../../serialization_fwd_decl.h"

namespace enigma {
namespace bytes_serialization {

template <typename T>
inline auto internal_serialize_into_fn(std::byte *iter, T *value) {
  internal_serialize_into<std::size_t>(iter, 0);
}

template <typename T>
inline auto internal_serialize_fn(T *&&value) {
  return internal_serialize<std::size_t>(0);
}

template <typename T>
matches_t<T, T, std::is_pointer> inline internal_deserialize_fn(std::byte *iter) {
  return nullptr;
}

}  // namespace bytes_serialization
}  // namespace enigma

#endif
