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

#ifndef ENIGMA_SERIALIZATION_H
#define ENIGMA_SERIALIZATION_H

#include <array>
#include <cstddef>

#include "complex_serialization_functions.h"
#include "detect_serialization.h"
#include "detect_size.h"
#include "map_serialization_functions.h"
#include "numeric_serialization_functions.h"
#include "string_serialization_functions.h"
#include "vector_set_serialization_functions.h"

namespace enigma {
namespace utility {
template <typename T, typename U>
inline T bit_cast(const U &value) {
  T result;
  std::memcpy(reinterpret_cast<void *>(&result), reinterpret_cast<const void *>(&value), sizeof(T));
  return result;
}
}  // namespace utility

template <typename T>
inline std::size_t enigma_internal_sizeof(T &&value) {
  if constexpr (has_byte_size_free_function<T>) {
    return byte_size(value);
  } else if constexpr (has_size_method_v<std::decay_t<T>>) {
    return value.size() * enigma_internal_sizeof(has_nested_form<T, 1>::inner_type);
  } else if constexpr (has_byte_size_method_v<std::decay_t<T>>) {
    return value.byte_size();
  } else {
    return sizeof(T);
  }
}

template <typename Base, typename T>
inline void internal_serialize_any_into(std::byte *iter, T value) {
  std::size_t i = sizeof(Base) - 1;
  std::size_t as_unsigned = utility::bit_cast<Base>(value);

  for (std::size_t j = 0; j < sizeof(Base); j++) {
    iter[i--] = static_cast<std::byte>(as_unsigned & 0xff);
    as_unsigned >>= 8;
  }
}

template <typename Base, typename T>
inline std::array<std::byte, sizeof(T)> serialize_any(T value) {
  std::array<std::byte, sizeof(T)> result{};
  internal_serialize_any_into<Base, T>(result.data(), value);
  return result;
}

template <typename Base, typename T>
inline T internal_deserialize_any(std::byte *iter) {
  Base result{};
  for (std::size_t i = 0; i < sizeof(T); i++) {
    result = (result << 8) | static_cast<Base>(*(iter + i));
  }
  return utility::bit_cast<T>(result);
}

template <typename T>
inline void internal_serialize_numeric_into(std::byte *iter, T value) {
  if constexpr (std::is_integral_v<T>) {
    internal_serialize_integral_into(iter, value);
  } else if constexpr (std::is_floating_point_v<T>) {
    internal_serialize_floating_into(iter, value);
  } else {
    static_assert(always_false<T>, "'internal_serialize_numeric_into' takes either integral or floating types");
  }
}

template <typename T>
inline std::array<std::byte, sizeof(T)> internal_serialize_numeric(T value) {
  if constexpr (std::is_integral_v<T>) {
    return internal_serialize_integral(value);
  } else if constexpr (std::is_floating_point_v<T>) {
    return internal_serialize_floating(value);
  } else {
    static_assert(always_false<T>, "'internal_serialize_numeric' takes either integral or floating types");
  }
}

template <typename T>
inline T internal_deserialize_numeric(std::byte *iter) {
  if constexpr (std::is_integral_v<T>) {
    return internal_deserialize_integral<T>(iter);
  } else if constexpr (std::is_floating_point_v<T>) {
    return internal_deserialize_floating<T>(iter);
  } else {
    static_assert(always_false<T>, "'internal_deserialize_numeric' takes either integral or floating types");
  }
}

inline void internal_serialize_variant_into(std::byte *iter, const variant &value) {
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

inline std::vector<std::byte> internal_serialize_variant(const variant &value) {
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

inline variant internal_deserialize_variant(std::byte *iter) {
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
inline void enigma_internal_serialize_lua_table(std::byte *iter, const lua_table<T> &table) {
  std::size_t pos = 0;
  internal_serialize_into(iter, table.mx_size_part());
  pos += sizeof(std::size_t);
  internal_serialize_into(iter + pos, table.dense_part().size());
  pos += sizeof(std::size_t);
  for (auto &elem : table.dense_part()) {
    if constexpr (is_lua_table_v<std::decay_t<T>>) {
      enigma_internal_serialize_lua_table(iter + pos, elem);
      pos += enigma_internal_sizeof(elem);
    } else {
      internal_serialize_into(iter + pos, elem);
      pos += enigma_internal_sizeof(elem);
    }
  }
  internal_serialize_into(iter + pos, table.sparse_part().size());
  pos += sizeof(std::size_t);
  for (auto &[key, value] : table.sparse_part()) {
    internal_serialize_into(iter + pos, key);
    pos += enigma_internal_sizeof(key);
    if constexpr (is_lua_table_v<std::decay_t<T>>) {
      enigma_internal_serialize_lua_table(iter + pos, value);
      pos += enigma_internal_sizeof(value);
    } else {
      internal_serialize_into(iter + pos, value);
      pos += enigma_internal_sizeof(value);
    }
  }
}

template <typename T>
inline lua_table<T> enigma_internal_deserialize_lua_table(std::byte *iter) {
  lua_table<T> table;
  auto &mx_size = const_cast<std::size_t &>(table.mx_size_part());
  auto &dense = const_cast<typename lua_table<T>::dense_type &>(table.dense_part());
  auto &sparse = const_cast<typename lua_table<T>::sparse_type &>(table.sparse_part());

  std::size_t pos = 0;
  enigma_deserialize(mx_size, iter, pos);
  std::size_t dense_size = 0;
  enigma_deserialize(dense_size, iter, pos);
  dense.resize(dense_size);
  for (std::size_t i = 0; i < dense_size; i++) {
    enigma_deserialize(dense[i], iter, pos);
  }
  std::size_t sparse_size = 0;
  enigma_deserialize(sparse_size, iter, pos);
  for (std::size_t i = 0; i < sparse_size; i++) {
    std::size_t key = 0;
    enigma_deserialize(key, iter, pos);
    enigma_deserialize(sparse[key], iter, pos);
  }
  return table;
}

inline void internal_serialize_var_into(std::byte *iter, const var &value) {
  std::size_t pos = 0;
  internal_serialize_into<const variant &>(iter, value);
  pos += variant_size(value);
  enigma_internal_serialize_lua_table(iter + pos, value.array1d);
  pos += enigma_internal_sizeof(value.array1d);
  enigma_internal_serialize_lua_table(iter + pos, value.array2d);
}

inline std::vector<std::byte> internal_serialize_var(const var &var) {
  std::vector<std::byte> result;
  result.resize(enigma_internal_sizeof(var));
  internal_serialize_var_into(result.data(), var);
  return result;
}

inline var internal_deserialize_var(std::byte *iter) {
  std::size_t pos = 0;
  variant inner = internal_deserialize<variant>(iter);
  pos += variant_size(inner);
  var result{std::move(inner)};
  result.array1d = enigma_internal_deserialize_lua_table<variant>(iter + pos);
  pos += enigma_internal_sizeof(result.array1d);
  result.array2d = enigma_internal_deserialize_lua_table<lua_table<variant>>(iter + pos);
  return result;
}

template <typename T>
inline auto internal_serialize_into_fn(std::byte *iter, T *value) {
  internal_serialize_into<std::size_t>(iter, 0);
}

template <typename T>
typename std::enable_if<std::is_same_v<bool, std::decay_t<T>>>::type inline internal_serialize_into_fn(std::byte *iter,
                                                                                                       T &&value) {
  *iter = static_cast<std::byte>(value);
}

template <typename T>
typename std::enable_if<std::is_same_v<var, std::decay_t<T>>>::type inline internal_serialize_into_fn(std::byte *iter,
                                                                                                      T &&value) {
  internal_serialize_var_into(iter, value);
}

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>> &&
                        !std::is_same_v<var, std::decay_t<T>>>::type inline internal_serialize_into_fn(std::byte *iter,
                                                                                                       T &&value) {
  internal_serialize_variant_into(iter, value);
}

template <typename T>
inline void internal_serialize_into(std::byte *iter, T &&value) {
  if constexpr (has_internal_serialize_into_fn_free_function<std::decay_t<T>>) {
    enigma::internal_serialize_into_fn(iter, value);
  } else {
    static_assert(always_false<T>,
                  "'internal_serialize_into' takes 'variant', 'var', 'std::string', bool, integral, floating types, "
                  "std::vector, std::map, std::complex or std::set");
  }
}

template <typename T>
inline auto internal_serialize_fn(T *&&value) {
  return internal_serialize_numeric<std::size_t>(0);
}

inline auto internal_serialize_fn(bool &&value) { return std::vector<std::byte>{static_cast<std::byte>(value)}; }

inline auto internal_serialize_fn(var &&value) { return internal_serialize_var(value); }

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>>,
                        std::vector<std::byte>>::type inline internal_serialize_fn(T &&value) {
  return internal_serialize_variant(value);
}

template <typename T>
inline auto internal_serialize(T &&value) {
  if constexpr (has_internal_serialize_fn_free_function<std::decay_t<T>>) {
    return internal_serialize_fn(value);
  } else if constexpr (has_serialize_method_v<std::decay_t<T>>) {
    return value.serialize();
  } else {
    static_assert(always_false<T>,
                  "'serialize' takes 'variant', 'var', 'std::string', bool, integral, floating types, std::vector, "
                  "std::map, std::complex or std::set");
  }
}

template <typename T>
typename std::enable_if<std::is_pointer_v<std::decay_t<T>>, T>::type inline internal_deserialize_fn(std::byte *iter) {
  return nullptr;
}

template <typename T>
typename std::enable_if<std::is_same_v<bool, std::decay_t<T>>, T>::type inline internal_deserialize_fn(
    std::byte *iter) {
  return static_cast<bool>(*iter);
}

template <typename T>
typename std::enable_if<std::is_same_v<var, std::decay_t<T>>, T>::type inline internal_deserialize_fn(std::byte *iter) {
  return internal_deserialize_var(iter);
}

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>>, T>::type inline internal_deserialize_fn(
    std::byte *iter) {
  return (internal_deserialize_variant(iter));
}

template <typename T>
inline T internal_deserialize(std::byte *iter) {
  if constexpr (has_internal_deserialize_fn_free_function<std::decay_t<T>>) {
    return internal_deserialize_fn<T>(iter);
  } else if constexpr (has_deserialize_self_method_v<std::decay_t<T>>) {
    T result;
    result.deserialize_self(iter);
    return result;
  } else if (has_deserialize_function_v<std::decay_t<T>>) {
    return internal_deserialize<T>(iter).second;
  } else {
    static_assert(always_false<T>,
                  "'deserialize' takes 'variant', 'var', 'std::string', bool, integral, floating types, std::vector, "
                  "std::map, std::complex or std::set");
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
typename std::enable_if<std::is_same_v<var, std::decay_t<T>>>::type inline internal_resize_buffer_for_fn(
    std::vector<std::byte> &buffer, T &&value) {
  buffer.resize(buffer.size() + var_size(value));
}

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>> && !std::is_same_v<var, std::decay_t<T>>>::
    type inline internal_resize_buffer_for_fn(std::vector<std::byte> &buffer, T &&value) {
  buffer.resize(buffer.size() + variant_size(value));
}

template <typename T>
inline void internal_resize_buffer_for(std::vector<std::byte> &buffer, T &&value) {
  if constexpr (has_internal_resize_buffer_for_fn_free_function<std::decay_t<T>>) {
    internal_resize_buffer_for_fn(buffer, value);
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

template <>
inline void enigma_internal_deserialize_fn(var &value, std::byte *iter, std::size_t &len) {
  value = internal_deserialize_var(iter + len);
  len += var_size(value);
}

template <typename T>
inline void enigma_internal_deserialize_fn(lua_table<T> &value, std::byte *iter, std::size_t &len) {
  value = enigma_internal_deserialize_lua_table<T>(iter);
  len += enigma_internal_sizeof(value);
}

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>> && !std::is_same_v<var, std::decay_t<T>>>::
    type inline enigma_internal_deserialize_fn(T &value, std::byte *iter, std::size_t &len) {
  enigma_internal_deserialize_variant(value, iter, len);
}

template <typename T>
inline void enigma_deserialize(T &value, std::byte *iter, std::size_t &len) {
  if constexpr (has_enigma_internal_deserialize_fn_free_function<std::decay_t<T>>) {
    enigma_internal_deserialize_fn(value, iter, len);
  } else if constexpr (has_byte_size_method_v<std::decay_t<T>>) {
    value = enigma::internal_deserialize<T>(iter + len);
    len += value.byte_size();
  } else {
    value = enigma::internal_deserialize<T>(iter + len);
    len += enigma_internal_sizeof(value);
  }
}

inline void enigma_internal_deserialize_variant(variant &value, std::byte *iter, std::size_t &len) {
  value = enigma::internal_deserialize<variant>(iter + len);
  len += variant_size(value);
}

template <typename... Ts>
inline void enigma_serialize_many(std::size_t &len, std::vector<std::byte> &bytes, Ts &&...values) {
  (enigma_serialize(std::forward<Ts>(values), len, bytes), ...);
}

template <typename... Ts>
inline void enigma_deserialize_many(std::byte *iter, std::size_t &len, Ts &&...values) {
  (enigma_deserialize(std::forward<Ts>(values), iter, len), ...);
}

}  // namespace enigma

#endif
