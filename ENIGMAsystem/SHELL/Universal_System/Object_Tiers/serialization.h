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
#ifndef ENIGMA_SERIALIZATION_H
#define ENIGMA_SERIALIZATION_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstring>
#include <type_traits>

#include "../var4.h"

#define ENIGMA_INTERNAL_OBJECT_SERIALIZE(value)                                \
    len = bytes.size();                                                        \
    resize_buffer_for(bytes, value);                                           \
    serialize_into(bytes.data() + len, value)

#define ENIGMA_INTERNAL_OBJECT_SERIALIZE_BOOL(value)                           \
    len = bytes.size();                                                        \
    bytes.resize(bytes.size() + 1);                                            \
    bytes.back() = static_cast<std::byte>(value);

#define ENIGMA_INTERNAL_OBJECT_DESERIALIZE(value)                              \
    value = enigma::deserialize<decltype(value)>(iter + len);                  \
    len += sizeof(value)

#define ENIGMA_INTERNAL_OBJECT_DESERIALIZE_VARIANT(value)                      \
    value = enigma::deserialize<variant>(iter + len);                          \
    if (value.type == variant::ty_real) {                                      \
      len += 9;                                                                \
    } else {                                                                   \
      len += 1 + sizeof(std::size_t) + value.sval().length();                  \
    }

#define ENIGMA_INTERNAL_OBJECT_DESERIALIZE_BOOL(value)                         \
  value = static_cast<bool>(*(iter++))

namespace enigma {
namespace utility {
template <typename T, typename U>
inline T bit_cast(const U &value) {
  T result;
  std::memcpy(reinterpret_cast<void *>(&result), reinterpret_cast<const void *>(&value), sizeof(T));
  return result;
}
}

namespace {
template <typename Base, typename T>
inline void serialize_any_into(std::byte *iter, T value) {
  std::size_t i = sizeof(T) - 1;
  std::size_t as_unsigned = utility::bit_cast<Base>(value);

  while (as_unsigned > 0) {
    iter[i--] = static_cast<std::byte>(as_unsigned & 0xff);
    as_unsigned >>= 8;
  }
}

template <typename Base, typename T>
inline std::array<std::byte, sizeof(T)> serialize_any(T value) {
  std::array<std::byte, sizeof(T)> result{};
  serialize_any_into<Base, T>(result.data(), value);
  return result;
}

template <typename Base, typename T>
inline T deserialize_any(std::byte *iter) {
  Base result;
  for (std::size_t i = 0; i < sizeof(T); i++) {
    result = (result << 8) | static_cast<Base>(*(iter + i));
  }
  return utility::bit_cast<T>(result);
}

}

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline void serialize_integral_into(std::byte *iter, T value) {
  serialize_any_into<std::make_unsigned_t<T>>(iter, value);
}

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline std::array<std::byte, sizeof(T)> serialize_integral(T value) {
  return serialize_any<std::make_unsigned_t<T>>(value);
}

template <typename T>
inline constexpr bool always_false = false;

template <typename T>
inline void serialize_floating_into(std::byte *iter, T value) {
  if constexpr (std::is_same_v<T, float>) {
    serialize_any_into<std::uint32_t>(iter, value);
  } else if constexpr (std::is_same_v<T, double>) {
    serialize_any_into<std::uint64_t>(iter, value);
  } else {
    static_assert(always_false<T>, "'serialize_floating_into' only accepts 'float' or 'double' types");
  }
}

template <typename T>
inline std::array<std::byte, sizeof(T)> serialize_floating(T value) {
  if constexpr (std::is_same_v<T, float>) {
    return serialize_any<std::uint32_t>(value);
  } else if constexpr (std::is_same_v<T, double>) {
    return serialize_any<std::uint64_t>(value);
  } else {
    static_assert(always_false<T>, "'serialize_floating' only accepts 'float' or 'double' types");
  }
}

template <typename T>
inline void serialize_numeric_into(std::byte *iter, T value) {
  if constexpr (std::is_integral_v<T>) {
    serialize_integral_into(iter, value);
  } else if constexpr (std::is_floating_point_v<T>) {
    serialize_floating_into(iter, value);
  } else {
    static_assert(always_false<T>, "'serialize_numeric_into' takes either integral or floating types");
  }
}

template <typename T>
inline std::array<std::byte, sizeof(T)> serialize_numeric(T value) {
  if constexpr (std::is_integral_v<T>) {
    return serialize_integral(value);
  } else if constexpr (std::is_floating_point_v<T>) {
    return serialize_floating(value);
  } else {
    static_assert(always_false<T>, "'serialize_numeric' takes either integral or floating types");
  }
}

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline T deserialize_integral(std::byte *iter) {
  return deserialize_any<std::make_unsigned_t<T>, T>(iter);
}

template <typename T>
inline T deserialize_floating(std::byte *iter) {
  if constexpr (std::is_same_v<T, float>) {
    return deserialize_any<std::uint32_t, T>(iter);
  } else if constexpr (std::is_same_v<T, double>) {
    return deserialize_any<std::size_t, T>(iter);
  } else {
    static_assert(always_false<T>, "'deserialize_floating' only accepts 'float' or 'double' types");
  }
}

template <typename T>
inline T deserialize_numeric(std::byte *iter) {
  if constexpr (std::is_integral_v<T>) {
    return deserialize_integral<T>(iter);
  } else if constexpr (std::is_floating_point_v<T>) {
    return deserialize_floating<T>(iter);
  } else {
    static_assert(always_false<T>, "'deserialize_numeric' takes either integral or floating types");
  }
}

inline void serialize_variant_into(std::byte *iter, const variant &value) {
  if (value.type == variant::ty_real) {
    *(iter++) = static_cast<std::byte>(variant::ty_real);
    serialize_floating_into(iter, value.rval.d);
  } else if (value.type == variant::ty_string) {
    *(iter++) = static_cast<std::byte>(variant::ty_string);
    serialize_integral_into(iter, value.sval().length());
    iter += sizeof(std::size_t);
    std::transform(value.sval().begin(), value.sval().end(), iter,
      [](char c) { return static_cast<std::byte>(c); }
    );
  }
}

inline std::vector<std::byte> serialize_variant(const variant &value) {
  std::vector<std::byte> result{};
  if (value.type == variant::ty_real) {
    result.resize(9);
    serialize_variant_into(result.data(), value);
  } else if (value.type == variant::ty_string) {
    result.resize(1 + sizeof(std::size_t) + value.sval().length());
    serialize_variant_into(result.data(), value);
  }

  return result;
}

inline variant deserialize_variant(std::byte *iter) {
  variant result;
  if (static_cast<int>(*iter) == variant::ty_real) {
    result.rval.d = deserialize_floating<double>(iter + 1);
  } else if (static_cast<int>(*iter) == variant::ty_string) {
    auto len = deserialize_integral<std::size_t>(iter + 1);
    result.sval().resize(len + 1);
    for (std::size_t i = 0; i < len; i++) {
      result.sval()[i] = static_cast<char>(*(iter + len + i));
    }
  }

  return result;
}

template <typename T>
inline void serialize_into(std::byte *iter, T &&value) {
  if constexpr (std::is_base_of_v<variant, std::decay_t<T>>) {
    if constexpr (std::is_pointer_v<T>) {
      serialize_variant_into(iter, *value);
    } else {
      serialize_variant_into(iter, value);
    }
  } else if constexpr (std::is_integral_v<std::decay_t<T>> || std::is_floating_point_v<std::decay_t<T>>){
    if constexpr (std::is_pointer_v<T>) {
      serialize_numeric_into(iter, *value);
    } else {
      serialize_numeric_into(iter, value);
    }
  } else {
    static_assert(always_false<T>, "'serialize_into' takes 'variant', integral or floating types");
  }
}

template <typename T>
inline auto serialize(T &&value) {
  if constexpr (std::is_base_of_v<variant, std::decay_t<T>>) {
    if constexpr (std::is_pointer_v<T>) {
      return serialize_variant(*value);
    } else {
      return serialize_variant(value);
    }
  } else if constexpr (std::is_integral_v<std::decay_t<T>> || std::is_floating_point_v<std::decay_t<T>>){
    if constexpr (std::is_pointer_v<T>) {
      return serialize_numeric(*value);
    } else {
      return serialize_numeric(value);
    }
  } else {
    static_assert(always_false<T>, "'serialize' takes 'variant', integral or floating types");
  }
}

template <typename T>
inline T deserialize(std::byte *iter) {
  if constexpr (std::is_base_of_v<variant, std::decay_t<T>>) {
    return deserialize_variant(iter);
  } else if constexpr (std::is_integral_v<std::decay_t<T>> || std::is_floating_point_v<std::decay_t<T>>) {
    return deserialize_numeric<T>(iter);
  } else {
    static_assert(always_false<T>, "'deserialize' takes 'variant', integral or floating types");
  }
}

template <typename T>
inline void resize_buffer_for_value(std::vector<std::byte> &buffer, T) {
  buffer.resize(buffer.size() + sizeof(T));
}

inline void resize_buffer_for_variant(std::vector<std::byte> &buffer, const variant &value) {
  if (value.type == variant::ty_real) {
    buffer.resize(buffer.size() + 9);
  } else {
    buffer.resize(buffer.size() + 1 + sizeof(std::size_t) + value.sval().length());
  }
}

template <typename T>
inline void resize_buffer_for(std::vector<std::byte> &buffer, T &&value) {
  if constexpr (std::is_same_v<T, variant>) {
    resize_buffer_for_variant(buffer, value);
  } else {
    resize_buffer_for_value(buffer, value);
  }
}

// A varint encoding version of the integer functions
/*
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
std::vector<std::byte> serialize_integral(T value) {
  std::vector<std::byte> result{};
  result.reserve(sizeof(T));

std::size_t as_unsigned = utility::bit_cast<std::make_unsigned_t<T>>(value);
while (as_unsigned > 0) {
  result.push_back(static_cast<std::byte>(as_unsigned & 0x7f));
  as_unsigned >>= 7;
  if (as_unsigned > 0) {
    result.back() |= std::byte{1 << 7};
  } else {
    result.back() &= std::byte{0x7f};
  }
}

return result;
}

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
T deserialize_integral(std::byte *iter) {
  T result{};
  std::size_t i = 0;

  while ((*iter & std::byte{0x80}) != std::byte{0}) {
    result |= static_cast<T>(*iter++ & std::byte{0x7f}) << (i++ * 7);
  }
  result |= static_cast<T>(*iter & std::byte{0x7f}) << (i * 7);

  return result;
}
*/

}

#endif