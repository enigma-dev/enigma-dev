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

#include <algorithm>
#include "detect_size.h"
#include "serialization_fwd_decl.h"

inline void enigma::internal_serialize_variant_into(std::byte *iter, const variant &value) {
  if (value.type == variant::ty_real) {
    *(iter++) = static_cast<std::byte>(variant::ty_real);
    internal_serialize_floating_into(iter, value.rval.d);
  } else if (value.type == variant::ty_string) {
    *(iter++) = static_cast<std::byte>(variant::ty_string);
    internal_serialize_integral_into(iter, value.sval().length());
    iter += sizeof(std::size_t);
    std::transform(value.sval().begin(), value.sval().end(), iter, [](char c) { return static_cast<std::byte>(c); });
  }
}

inline std::vector<std::byte> enigma::internal_serialize_variant(const variant &value) {
  std::vector<std::byte> result{};
  if (value.type == variant::ty_real) {
    result.resize(9);
    internal_serialize_variant_into(result.data(), value);
  } else if (value.type == variant::ty_string) {
    result.resize(1 + sizeof(std::size_t) + value.sval().length());
    internal_serialize_variant_into(result.data(), value);
  }

  return result;
}

inline variant enigma::internal_deserialize_variant(std::byte *iter) {
  variant result;
  if (static_cast<int>(*iter) == variant::ty_real) {
    result.rval.d = internal_deserialize_floating<double>(iter + 1);
  } else if (static_cast<int>(*iter) == variant::ty_string) {
    auto len = internal_deserialize_integral<std::size_t>(iter + 1);
    result.sval().resize(len + 1);
    for (std::size_t i = 0; i < len; i++) {
      result.sval()[i] = static_cast<char>(*(iter + len + i));
    }
  }

  return result;
}

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>> && !std::is_same_v<var, std::decay_t<T>>>::
    type inline enigma::internal_serialize_into_fn(std::byte *iter, T &&value) {
  internal_serialize_variant_into(iter, value);
}

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>>,
                        std::vector<std::byte>>::type inline enigma::internal_serialize_fn(T &&value) {
  return internal_serialize_variant(value);
}

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>>, T>::type inline enigma::internal_deserialize_fn(
    std::byte *iter) {
  return (internal_deserialize_variant(iter));
}

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>> && !std::is_same_v<var, std::decay_t<T>>>::
    type inline enigma::internal_resize_buffer_for_fn(std::vector<std::byte> &buffer, T &&value) {
  buffer.resize(buffer.size() + variant_size(value));
}

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>> && !std::is_same_v<var, std::decay_t<T>>>::
    type inline enigma::enigma_internal_deserialize_fn(T &value, std::byte *iter, std::size_t &len) {
  enigma_internal_deserialize_variant(value, iter, len);
}

inline void enigma::enigma_internal_deserialize_variant(variant &value, std::byte *iter, std::size_t &len) {
  value = enigma::internal_deserialize<variant>(iter + len);
  len += variant_size(value);
}
