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

/**
  @file variant.h
  @brief This file contains the implementation of bytes (de)serialization functions for variant based
  types (variant, enigma::depthv, enigma::image_singlev, enigma::speedv, enigma::directionv, etc.).  
*/

#ifndef ENIGMA_SERIALIZE_VARIANT_BYTES_H
#define ENIGMA_SERIALIZE_VARIANT_BYTES_H

#include <algorithm>
#include "../../serialization_fwd_decl.h"

namespace enigma {
namespace bytes_serialization {

template <typename T>
enables_if_base_of_variant_t<T, std::size_t> inline byte_size(const T &value) {
  if (value.type == variant::ty_real) {
    return 9;
  } else {
    return 1 + sizeof(std::size_t) + value.sval().length();
  }
}

template <typename T>
enables_if_base_of_variant_t<T> inline internal_serialize_into_fn(std::byte *iter, T &&value) {
  if (value.type == variant::ty_real) {
    *(iter++) = static_cast<std::byte>(variant::ty_real);
    internal_serialize_into(iter, value.rval.d);
  } else if (value.type == variant::ty_string) {
    *(iter++) = static_cast<std::byte>(variant::ty_string);
    internal_serialize_into(iter, value.sval().length());
    iter += sizeof(std::size_t);
    std::transform(value.sval().begin(), value.sval().end(), iter, [](char c) { return static_cast<std::byte>(c); });
  }
}

template <typename T>
enables_if_base_of_variant_t<T, std::vector<std::byte>> inline internal_serialize_fn(T &&value) {
  std::vector<std::byte> result{};
  result.resize(enigma_internal_sizeof(value));
  internal_serialize_into(result.data(), value);
  return result;
}

template <typename T>
enables_if_base_of_variant_t<T, T> inline internal_deserialize_fn(std::byte *iter) {
  variant result;
  if (static_cast<int>(*iter) == variant::ty_real) {
    result.rval.d = internal_deserialize<double>(iter + 1);
  } else if (static_cast<int>(*iter) == variant::ty_string) {
    auto len = internal_deserialize<std::size_t>(iter + 1);
    result.sval().resize(len + 1);
    for (std::size_t i = 0; i < len; i++) {
      result.sval()[i] = static_cast<char>(*(iter + len + i));
    }
  }
  return result;
}

template <typename T>
enables_if_base_of_variant_t<T> inline internal_resize_buffer_for_fn(std::vector<std::byte> &buffer, T &&value) {
  buffer.resize(buffer.size() + enigma_internal_sizeof(value));
}

template <typename T>
enables_if_base_of_variant_t<T> inline enigma_internal_deserialize_fn(T &value, std::byte *iter, std::size_t &len) {
  value = internal_deserialize<variant>(iter + len);
  len += enigma_internal_sizeof(value);
}

}  // namespace bytes_serialization
}  // namespace enigma

#endif
