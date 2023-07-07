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

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstring>

#include "Universal_System/detect_serialization.h"
#include "detect_size.h"

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
inline void serialize_into(std::byte *iter, T &&value);

template <typename T>
inline auto serialize(T &&value);

template <typename T>
inline T deserialize(std::byte *iter);

template <typename T>
inline void enigma_internal_serialize(const T &value, std::size_t &len, std::vector<std::byte> &bytes);

template <typename T>
inline void enigma_internal_deserialize(T &value, std::byte *iter, std::size_t &len);

inline void enigma_internal_deserialize_variant(variant &value, std::byte *iter, std::size_t &len);

template <typename T, std::size_t N>
struct has_nested_form : std::false_type {
  using inner_type = void;
};

template <typename T>
struct has_nested_form<T, 0> : std::true_type {
  using inner_type = T;
};

template <template <typename> typename T, typename U, std::size_t N>
struct has_nested_form<T<U>, N> : has_nested_form<U, N - 1> {
  using inner_type = U;
};

template <typename T, std::size_t N>
constexpr static inline bool has_nested_form_v = has_nested_form<T, N>::value;

template <typename T>
inline std::size_t enigma_internal_sizeof(T &&value) {
  if constexpr (has_byte_size_free_function_v<T>) {
    return byte_size(value);
  } else if constexpr (has_size_method_v<std::decay_t<T>>) {
    return value.size() * enigma_internal_sizeof(has_nested_form<T, 1>::inner_type);
  } else if constexpr (has_byte_size_method_v<std::decay_t<T>>) {
    return value.byte_size();
  } else {
    return sizeof(T);
  }
}

namespace {
template <typename Base, typename T>
inline void serialize_any_into(std::byte *iter, T value) {
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
  serialize_any_into<Base, T>(result.data(), value);
  return result;
}

template <typename Base, typename T>
inline T deserialize_any(std::byte *iter) {
  Base result{};
  for (std::size_t i = 0; i < sizeof(T); i++) {
    result = (result << 8) | static_cast<Base>(*(iter + i));
  }
  return utility::bit_cast<T>(result);
}

}  // namespace

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline void serialize_integral_into(std::byte *iter, T value) {
  serialize_any_into<std::make_unsigned_t<T>>(iter, value);
}

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline std::array<std::byte, sizeof(T)> serialize_integral(T value) {
  return serialize_any<std::make_unsigned_t<T>>(value);
}

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
    std::transform(value.sval().begin(), value.sval().end(), iter, [](char c) { return static_cast<std::byte>(c); });
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
inline void enigma_internal_serialize_lua_table(std::byte *iter, const lua_table<T> &table) {
  std::size_t pos = 0;
  serialize_into(iter, table.mx_size_part());
  pos += sizeof(std::size_t);
  serialize_into(iter + pos, table.dense_part().size());
  pos += sizeof(std::size_t);
  for (auto &elem : table.dense_part()) {
    if constexpr (is_lua_table_v<std::decay_t<T>>) {
      enigma_internal_serialize_lua_table(iter + pos, elem);
      pos += enigma_internal_sizeof_lua_table(elem);
    } else {
      serialize_into(iter + pos, elem);
      pos += enigma_internal_sizeof(elem);
    }
  }
  serialize_into(iter + pos, table.sparse_part().size());
  pos += sizeof(std::size_t);
  for (auto &[key, value] : table.sparse_part()) {
    serialize_into(iter + pos, key);
    pos += enigma_internal_sizeof(key);
    if constexpr (is_lua_table_v<std::decay_t<T>>) {
      enigma_internal_serialize_lua_table(iter + pos, value);
      pos += enigma_internal_sizeof_lua_table(value);
    } else {
      serialize_into(iter + pos, value);
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
  enigma_internal_deserialize(mx_size, iter, pos);
  std::size_t dense_size = 0;
  enigma_internal_deserialize(dense_size, iter, pos);
  dense.resize(dense_size);
  for (std::size_t i = 0; i < dense_size; i++) {
    enigma_internal_deserialize(dense[i], iter, pos);
  }
  std::size_t sparse_size = 0;
  enigma_internal_deserialize(sparse_size, iter, pos);
  for (std::size_t i = 0; i < sparse_size; i++) {
    std::size_t key = 0;
    enigma_internal_deserialize(key, iter, pos);
    enigma_internal_deserialize(sparse[key], iter, pos);
  }
  return table;
}

inline void serialize_var_into(std::byte *iter, const var &value) {
  std::size_t pos = 0;
  serialize_into<const variant &>(iter, value);
  pos += variant_size(value);
  enigma_internal_serialize_lua_table(iter + pos, value.array1d);
  pos += enigma_internal_sizeof_lua_table(value.array1d);
  enigma_internal_serialize_lua_table(iter + pos, value.array2d);
}

inline std::vector<std::byte> serialize_var(const var &var) {
  std::vector<std::byte> result;
  result.resize(enigma_internal_sizeof(var));
  serialize_var_into(result.data(), var);
  return result;
}

inline var deserialize_var(std::byte *iter) {
  std::size_t pos = 0;
  variant inner = deserialize<variant>(iter);
  pos += variant_size(inner);
  var result{std::move(inner)};
  result.array1d = enigma_internal_deserialize_lua_table<variant>(iter + pos);
  pos += enigma_internal_sizeof_lua_table(result.array1d);
  result.array2d = enigma_internal_deserialize_lua_table<lua_table<variant>>(iter + pos);
  return result;
}

template <typename T>
inline void serialize_into(std::byte *iter, T &&value) {
  if constexpr (std::is_pointer_v<std::decay_t<T>>) {
    serialize_into<std::size_t>(iter, 0);
  } else if constexpr (std::is_same_v<std::string, std::decay_t<T>>) {
    serialize_into<std::size_t>(iter, value.size());
    std::transform(value.begin(), value.end(), iter + sizeof(std::size_t),
                   [](char c) { return static_cast<std::byte>(c); });
  } else if constexpr (std::is_same_v<bool, std::decay_t<T>>) {
    *iter = static_cast<std::byte>(value);
  } else if constexpr (std::is_same_v<var, std::decay_t<T>>) {
    serialize_var_into(iter, value);
  } else if constexpr (std::is_base_of_v<variant, std::decay_t<T>>) {
    serialize_variant_into(iter, value);
  } else if constexpr (std::is_integral_v<std::decay_t<T>> || std::is_floating_point_v<std::decay_t<T>>) {
    serialize_numeric_into(iter, value);
  } else {
    static_assert(always_false<T>,
                  "'serialize_into' takes 'variant', 'var', 'std::string', bool, integral or floating types");
  }
}

template <typename T>
inline auto serialize(T &&value) {
  if constexpr (std::is_pointer_v<std::decay_t<T>>) {
    return serialize_numeric<std::size_t>(0);
  } else if constexpr (std::is_same_v<std::string, std::decay_t<T>>) {
    std::vector<std::byte> result;
    result.resize(sizeof(std::size_t) + value.size());
    serialize_into<std::size_t>(result.data(), 0);
    std::transform(value.begin(), value.end(), result.data() + sizeof(std::size_t),
                   [](char c) { return static_cast<std::byte>(c); });
  } else if constexpr (std::is_same_v<bool, std::decay_t<T>>) {
    return std::vector<std::byte>{static_cast<std::byte>(value)};
  } else if constexpr (std::is_same_v<var, std::decay_t<T>>) {
    return serialize_var(value);
  } else if constexpr (std::is_base_of_v<variant, std::decay_t<T>>) {
    return serialize_variant(value);
  } else if constexpr (std::is_integral_v<std::decay_t<T>> || std::is_floating_point_v<std::decay_t<T>>) {
    return serialize_numeric(value);
  } else if constexpr (has_serialize_method_v<std::decay_t<T>>) {
    return value.serialize();
  } else {
    static_assert(always_false<T>,
                  "'serialize' takes 'variant', 'var', 'std::string', bool, integral or floating types");
  }
}

template <typename T>
inline T deserialize(std::byte *iter) {
  if constexpr (std::is_pointer_v<std::decay_t<T>>) {
    return nullptr;
  } else if constexpr (std::is_same_v<std::string, std::decay_t<T>>) {
    std::size_t len = deserialize_numeric<std::size_t>(iter);
    std::size_t offset = sizeof(std::size_t);
    std::string result{reinterpret_cast<char *>(iter + offset), reinterpret_cast<char *>(iter + offset + len)};
    return result;
  } else if constexpr (std::is_same_v<bool, std::decay_t<T>>) {
    return static_cast<bool>(*iter);
  } else if constexpr (std::is_same_v<var, std::decay_t<T>>) {
    return deserialize_var(iter);
  } else if constexpr (std::is_base_of_v<variant, std::decay_t<T>>) {
    return deserialize_variant(iter);
  } else if constexpr (std::is_integral_v<std::decay_t<T>> || std::is_floating_point_v<std::decay_t<T>>) {
    return deserialize_numeric<T>(iter);
  } else if constexpr (has_deserialize_self_method_v<std::decay_t<T>>) {
    T result;
    result.deserialize_self(iter);
    return result;
  } else if (has_deserialize_function_v<std::decay_t<T>>) {
    return deserialize<T>(iter).second;
  } else {
    static_assert(always_false<T>,
                  "'deserialize' takes 'variant', 'var', 'std::string', bool, integral or floating types");
  }
}

template <typename T>
inline void resize_buffer_for_value(std::vector<std::byte> &buffer, T &&value) {
  buffer.resize(buffer.size() + enigma_internal_sizeof(value));
}

inline void resize_buffer_for_var(std::vector<std::byte> &buffer, const var &value) {
  buffer.resize(buffer.size() + var_size(value));
}

inline void resize_buffer_for_variant(std::vector<std::byte> &buffer, const variant &value) {
  buffer.resize(buffer.size() + variant_size(value));
}

inline void resize_buffer_for_string(std::vector<std::byte> &buffer, const std::string &value) {
  buffer.resize(buffer.size() + value.size() + sizeof(std::size_t));
}

template <typename T, typename = std::enable_if_t<has_byte_size_method_v<T>>>
inline void resize_buffer_using_byte_size(std::vector<std::byte> &buffer, const T &value) {
  buffer.resize(buffer.size() + value.byte_size());
}

template <typename T>
inline void resize_buffer_for(std::vector<std::byte> &buffer, T &&value) {
  if constexpr (std::is_same_v<var, std::decay_t<T>>) {
    resize_buffer_for_var(buffer, value);
  } else if constexpr (std::is_base_of_v<variant, std::decay_t<T>>) {
    resize_buffer_for_variant(buffer, value);
  } else if constexpr (std::is_same_v<std::string, std::decay_t<T>>) {
    resize_buffer_for_string(buffer, value);
  } else if constexpr (has_byte_size_method_v<std::decay_t<T>>) {
    resize_buffer_using_byte_size(buffer, value);
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

template <typename T>
inline void enigma_internal_serialize(const T &value, std::size_t &len, std::vector<std::byte> &bytes) {
  len = bytes.size();
  resize_buffer_for(bytes, value);
  if constexpr (has_serialize_method_v<std::decay_t<T>>) {
    std::vector<std::byte> serialized = value.serialize();
    std::copy(serialized.begin(), serialized.end(), bytes.begin() + len);
  } else {
    serialize_into(bytes.data() + len, value);
  }
  len = bytes.size();
}

inline void enigma_internal_deserialize_fn(var &value, std::byte *iter, std::size_t &len) {
  value = deserialize_var(iter + len);
  len += var_size(value);
}

template <typename T>
inline void enigma_internal_deserialize_fn(lua_table<T> &value, std::byte *iter, std::size_t &len) {
  value = enigma_internal_deserialize_lua_table<T>(iter);
  len += enigma_internal_sizeof_lua_table(value);
}

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>>>::type inline enigma_internal_deserialize_fn(
    T &value, std::byte *iter, std::size_t &len) {
  enigma_internal_deserialize_variant(value, iter, len);
}

inline void enigma_internal_deserialize_fn(std::string &value, std::byte *iter, std::size_t &len) {
  value = enigma::deserialize<std::string>(iter + len);
  len += value.length() + sizeof(std::size_t);
}

template <typename t>
constexpr static inline bool has_enigma_internal_deserialize_free_function_v2 =
    std::is_same_v<t, std::string> || std::is_same_v<t, var> || std::is_base_of_v<variant, std::decay_t<t>> ||
    is_lua_table_v<t>;

template <typename T>
inline void enigma_internal_deserialize(T &value, std::byte *iter, std::size_t &len) {
  if constexpr (has_enigma_internal_deserialize_free_function_v2<T>) {
    enigma_internal_deserialize_fn(value, iter, len);
  } else if constexpr (has_byte_size_method_v<std::decay_t<T>>) {
    value = enigma::deserialize<T>(iter + len);
    len += value.byte_size();
  } else {
    value = enigma::deserialize<T>(iter + len);
    len += enigma_internal_sizeof(value);
  }
}

inline void enigma_internal_deserialize_variant(variant &value, std::byte *iter, std::size_t &len) {
  value = enigma::deserialize<variant>(iter + len);
  len += variant_size(value);
}

template <typename... Ts>
inline void enigma_internal_serialize_many(std::size_t &len, std::vector<std::byte> &bytes, Ts &&...values) {
  (enigma_internal_serialize(std::forward<Ts>(values), len, bytes), ...);
}

template <typename... Ts>
inline void enigma_internal_deserialize_many(std::byte *iter, std::size_t &len, Ts &&...values) {
  (enigma_internal_deserialize(std::forward<Ts>(values), iter, len), ...);
}

}  // namespace enigma

#endif