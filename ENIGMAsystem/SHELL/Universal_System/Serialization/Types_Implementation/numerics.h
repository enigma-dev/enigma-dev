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

#include "../serialization_fwd_decl.h"

template <typename T>
typename std::enable_if<(std::is_integral_v<std::decay_t<T>> ||
                         std::is_floating_point_v<std::decay_t<T>>)&&!std::is_same_v<std::decay_t<T>, bool>>::
    type inline enigma::internal_serialize_into_fn(std::byte *iter, T &&value) {
  internal_serialize_numeric_into(iter, value);
}

template <typename T>
typename std::enable_if<std::is_integral_v<std::decay_t<T>> || std::is_floating_point_v<std::decay_t<T>>,
                        std::array<std::byte, sizeof(T)>>::type inline enigma::internal_serialize_fn(T &&value) {
  return internal_serialize_numeric(value);
}

template <typename T>
typename std::enable_if<(std::is_integral_v<std::decay_t<T>> ||
                         std::is_floating_point_v<std::decay_t<T>>)&&!std::is_same_v<std::decay_t<T>, bool>,
                        T>::type inline enigma::internal_deserialize_fn(std::byte *iter) {
  return internal_deserialize_numeric<T>(iter);
}

template <typename T>
inline void enigma::internal_serialize_numeric_into(std::byte *iter, T value) {
  if constexpr (std::is_integral_v<T>) {
    internal_serialize_any_into<std::make_unsigned_t<T>>(iter, value);
  } else if constexpr (std::is_floating_point_v<T>) {
    if constexpr (std::is_same_v<T, float>) {
      internal_serialize_any_into<std::uint32_t>(iter, value);
    } else if constexpr (std::is_same_v<T, double>) {
      internal_serialize_any_into<std::uint64_t>(iter, value);
    } else {
      static_assert(always_false<T>, "'internal_serialize_floating_into' only accepts 'float' or 'double' types");
    }
  } else {
    static_assert(always_false<T>, "'internal_serialize_numeric_into' takes either integral or floating types");
  }
}

template <typename T>
inline std::array<std::byte, sizeof(T)> enigma::internal_serialize_numeric(T value) {
  if constexpr (std::is_integral_v<T>) {
    return serialize_any<std::make_unsigned_t<T>>(value);
  } else if constexpr (std::is_floating_point_v<T>) {
    if constexpr (std::is_same_v<T, float>) {
      return serialize_any<std::uint32_t>(value);
    } else if constexpr (std::is_same_v<T, double>) {
      return serialize_any<std::uint64_t>(value);
    } else {
      static_assert(always_false<T>, "'internal_serialize_floating' only accepts 'float' or 'double' types");
    }
  } else {
    static_assert(always_false<T>, "'internal_serialize_numeric' takes either integral or floating types");
  }
}

template <typename T>
inline T enigma::internal_deserialize_numeric(std::byte *iter) {
  if constexpr (std::is_integral_v<T>) {
    return internal_deserialize_any<std::make_unsigned_t<T>, T>(iter);
  } else if constexpr (std::is_floating_point_v<T>) {
    if constexpr (std::is_same_v<T, float>) {
      return internal_deserialize_any<std::uint32_t, T>(iter);
    } else if constexpr (std::is_same_v<T, double>) {
      return internal_deserialize_any<std::size_t, T>(iter);
    } else {
      static_assert(always_false<T>, "'internal_deserialize_floating' only accepts 'float' or 'double' types");
    }
  } else {
    static_assert(always_false<T>, "'internal_deserialize_numeric' takes either integral or floating types");
  }
}
