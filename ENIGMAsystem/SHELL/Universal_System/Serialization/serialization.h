/** Copyright (C) 2022 Dhruv Chawla
*** Copyright (C) 2023 Fares Atef
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
  @file serialization.h
  @brief This file contains the main serialization functions for bytes and JSON.
*/

#ifndef ENIGMA_SERIALIZATION_H
#define ENIGMA_SERIALIZATION_H

#include <array>
#include "detect_serialization.h"

namespace enigma {

namespace utility {
template <typename T, typename U>
inline T bit_cast(const U &value) {
  T result;
  std::memcpy(reinterpret_cast<void *>(&result), reinterpret_cast<const void *>(&value), sizeof(T));
  return result;
}
}  // namespace utility

namespace bytes_serialization {

template <typename T>
inline std::size_t enigma_internal_sizeof(T &&value) {
  if constexpr (bytes_serialization_has_byte_size<T>) {
    return enigma::bytes_serialization::byte_size(value);
  } else if constexpr (has_byte_size_method_v<std::decay_t<T>>) {
    return value.byte_size();
  } else {
    return sizeof(T);
  }
}

template <typename Base, typename T>
inline void internal_serialize_primitive_into(std::byte *iter, const T &value) {
  std::size_t i = sizeof(Base) - 1;
  std::size_t as_unsigned = utility::bit_cast<Base>(value);

  for (std::size_t j = 0; j < sizeof(Base); j++) {
    iter[i--] = static_cast<std::byte>(as_unsigned & 0xff);
    as_unsigned >>= 8;
  }
}

template <typename Base, typename T>
inline std::array<std::byte, sizeof(T)> serialize_primitive(T value) {
  std::array<std::byte, sizeof(T)> result{};
  internal_serialize_primitive_into<Base, T>(result.data(), value);
  return result;
}

template <typename Base, typename T>
inline T internal_deserialize_primitive(std::byte *iter) {
  Base result{};
  for (std::size_t i = 0; i < sizeof(T); i++) {
    result = (result << 8) | static_cast<Base>(*(iter + i));
  }
  return utility::bit_cast<T>(result);
}

template <typename T>
inline void internal_serialize_into(std::byte *iter, T &&value) {
  if constexpr (bytes_serialization_has_internal_serialize_into_fn<std::decay_t<T>>) {
    enigma::bytes_serialization::internal_serialize_into_fn(iter, value);
  } else {
    static_assert(always_false<T>, "Type to be serialized has to implement 'internal_serialize_into_fn'!");
  }
}

template <typename T>
inline auto internal_serialize(T &&value) {
  if constexpr (bytes_serialization_has_internal_serialize_fn<std::decay_t<T>>) {
    return enigma::bytes_serialization::internal_serialize_fn(value);
  } else if constexpr (has_serialize_method_v<std::decay_t<T>>) {
    return value.serialize();
  } else {
    static_assert(always_false<T>,
                  "Type to be serialized has to implement one of 'serialize' or 'internal_serialize_fn'!");
  }
}

template <typename T>
inline T internal_deserialize(std::byte *iter) {
  if constexpr (bytes_serialization_has_internal_deserialize_fn<std::decay_t<T>>) {
    return enigma::bytes_serialization::internal_deserialize_fn<T>(iter);
  } else if constexpr (has_deserialize_self_method_v<std::decay_t<T>>) {
    T result;
    result.deserialize_self(iter);
    return result;
  } else if (has_deserialize_function_v<std::decay_t<T>>) {
    return internal_deserialize<T>(iter).second;
  } else {
    static_assert(always_false<T>,
                  "Type to be deserialized has to implement one of 'internal_deserialize', 'deserialize_self' or "
                  "'internal_deserialize_fn'!");
  }
}

template <typename T>
inline void internal_resize_buffer_for_value(std::vector<std::byte> &buffer, T &&value) {
  buffer.resize(buffer.size() + enigma_internal_sizeof(value));
}

template <typename T, typename = std::enable_if_t<has_byte_size_method_v<T>>>
inline void internal_resize_buffer_using_byte_size(std::vector<std::byte> &buffer, const T &value) {
  buffer.resize(buffer.size() + value.byte_size());
}

template <typename T>
inline void internal_resize_buffer_for(std::vector<std::byte> &buffer, T &&value) {
  if constexpr (bytes_serialization_has_internal_resize_buffer_for_fn<std::decay_t<T>>) {
    enigma::bytes_serialization::internal_resize_buffer_for_fn(buffer, value);
  } else if constexpr (has_byte_size_method_v<std::decay_t<T>>) {
    internal_resize_buffer_using_byte_size(buffer, value);
  } else {
    internal_resize_buffer_for_value(buffer, value);
  }
}

template <typename T>
inline void enigma_serialize(const T &value, std::size_t &len, std::vector<std::byte> &bytes) {
  len = bytes.size();
  internal_resize_buffer_for(bytes, value);
  if constexpr (has_serialize_method_v<std::decay_t<T>>) {
    std::vector<std::byte> serialized = value.serialize();
    std::copy(serialized.begin(), serialized.end(), bytes.begin() + len);
  } else {
    internal_serialize_into(bytes.data() + len, value);
  }
  len = bytes.size();
}

template <typename T>
inline void enigma_deserialize(T &value, std::byte *iter, std::size_t &len) {
  if constexpr (bytes_serialization_has_enigma_internal_deserialize_fn<std::decay_t<T>>) {
    enigma::bytes_serialization::enigma_internal_deserialize_fn(value, iter, len);
  } else if constexpr (has_byte_size_method_v<std::decay_t<T>>) {
    value = enigma::bytes_serialization::internal_deserialize<T>(iter + len);
    len += value.byte_size();
  } else {
    value = enigma::bytes_serialization::internal_deserialize<T>(iter + len);
    len += enigma_internal_sizeof(value);
  }
}

template <typename... Ts>
inline void enigma_serialize_many(std::size_t &len, std::vector<std::byte> &bytes, Ts &&...values) {
  (enigma_serialize(std::forward<Ts>(values), len, bytes), ...);
}

template <typename... Ts>
inline void enigma_deserialize_many(std::byte *iter, std::size_t &len, Ts &&...values) {
  (enigma_deserialize(std::forward<Ts>(values), iter, len), ...);
}

}  // namespace bytes_serialization

namespace JSON_serialization {

template <typename T>
inline std::string enigma_serialize(const T &value) {
  if constexpr (JSON_serialization_has_internal_serialize_into_fn<std::decay_t<T>>) {
    return enigma::JSON_serialization::internal_serialize_into_fn(value);
  } else if constexpr (has_json_serialize_method_v<std::decay_t<T>>) {
    return value.json_serialize();
  } else {
    static_assert(always_false<T>,
                  "Type to be serialized has to implement one of 'json_serialize' or 'internal_serialize_into_fn'!");
  }
}

template <typename T>
inline T enigma_deserialize(std::string json) {
  json_remove_whitespace(json);
  if constexpr (JSON_serialization_has_internal_deserialize_fn<std::decay_t<T>>) {
    return enigma::JSON_serialization::internal_deserialize_fn<std::decay_t<T>>(json);
  } else if constexpr (has_json_deserialize_function_v<std::decay_t<T>>) {
    return T::json_deserialize(json);
  } else {
    static_assert(always_false<T>,
                  "Type to be deserialized has to implement one of 'json_deserialize' or 'internal_deserialize_fn'!");
  }
}

template <typename T>
inline void enigma_deserialize_val(T &val, std::string json) {
  val = enigma::JSON_serialization::enigma_deserialize<T>(json);
}

}  // namespace JSON_serialization
}  // namespace enigma

#endif
