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

#include "detect_serialization.h"
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

namespace {
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

}  // namespace

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline void internal_serialize_integral_into(std::byte *iter, T value) {
  internal_serialize_any_into<std::make_unsigned_t<T>>(iter, value);
}

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline std::array<std::byte, sizeof(T)> internal_serialize_integral(T value) {
  return serialize_any<std::make_unsigned_t<T>>(value);
}

template <typename T>
inline void internal_serialize_floating_into(std::byte *iter, T value) {
  if constexpr (std::is_same_v<T, float>) {
    internal_serialize_any_into<std::uint32_t>(iter, value);
  } else if constexpr (std::is_same_v<T, double>) {
    internal_serialize_any_into<std::uint64_t>(iter, value);
  } else {
    static_assert(always_false<T>, "'internal_serialize_floating_into' only accepts 'float' or 'double' types");
  }
}

template <typename T>
inline std::array<std::byte, sizeof(T)> internal_serialize_floating(T value) {
  if constexpr (std::is_same_v<T, float>) {
    return serialize_any<std::uint32_t>(value);
  } else if constexpr (std::is_same_v<T, double>) {
    return serialize_any<std::uint64_t>(value);
  } else {
    static_assert(always_false<T>, "'internal_serialize_floating' only accepts 'float' or 'double' types");
  }
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

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline T internal_deserialize_integral(std::byte *iter) {
  return internal_deserialize_any<std::make_unsigned_t<T>, T>(iter);
}

template <typename T>
inline T internal_deserialize_floating(std::byte *iter) {
  if constexpr (std::is_same_v<T, float>) {
    return internal_deserialize_any<std::uint32_t, T>(iter);
  } else if constexpr (std::is_same_v<T, double>) {
    return internal_deserialize_any<std::size_t, T>(iter);
  } else {
    static_assert(always_false<T>, "'internal_deserialize_floating' only accepts 'float' or 'double' types");
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
typename std::enable_if<std::is_same_v<std::string, std::decay_t<T>>>::type inline internal_serialize_into_fn(
    std::byte *iter, T &&value) {
  internal_serialize_into<std::size_t>(iter, value.size());
  std::transform(value.begin(), value.end(), iter + sizeof(std::size_t),
                 [](char c) { return static_cast<std::byte>(c); });
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
typename std::enable_if<(std::is_integral_v<std::decay_t<T>> ||
                         std::is_floating_point_v<std::decay_t<T>>)&&!std::is_same_v<std::decay_t<T>, bool>>::
    type inline internal_serialize_into_fn(std::byte *iter, T &&value) {
  internal_serialize_numeric_into(iter, value);
}

template <typename T>
typename std::enable_if<is_std_vector_v<std::decay_t<T>> ||
                        is_std_set_v<std::decay_t<T>>>::type inline internal_serialize_into_fn(std::byte *iter,
                                                                                               T &&value) {
  internal_serialize_into<std::size_t>(iter, value.size());
  iter += sizeof(std::size_t);
  for (const auto &element : value) {
    internal_serialize_into(iter, element);
    iter += enigma_internal_sizeof(element);
  }
}

template <typename T>
typename std::enable_if<is_std_map_v<std::decay_t<T>>>::type inline internal_serialize_into_fn(std::byte *iter,
                                                                                               T &&value) {
  internal_serialize_into<std::size_t>(iter, value.size());
  iter += sizeof(std::size_t);
  for (const auto &element : value) {
    internal_serialize_into(iter, element.first);
    iter += enigma_internal_sizeof(element.first);
    internal_serialize_into(iter, element.second);
    iter += enigma_internal_sizeof(element.second);
  }
}

template <typename T>
typename std::enable_if<is_std_complex_v<std::decay_t<T>>>::type inline internal_serialize_into_fn(std::byte *iter,
                                                                                                   T &&value) {
  internal_serialize_into(iter, value.real());
  iter += enigma_internal_sizeof(value.real());
  internal_serialize_into(iter, value.imag());
  iter += enigma_internal_sizeof(value.imag());
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

inline auto internal_serialize_fn(std::string &&value) {
  std::vector<std::byte> result;
  result.resize(sizeof(std::size_t) + value.size());
  internal_serialize_into<std::size_t>(result.data(), value.size());
  std::transform(value.begin(), value.end(), result.data() + sizeof(std::size_t),
                 [](char c) { return static_cast<std::byte>(c); });
  return result;
}

inline auto internal_serialize_fn(bool &&value) { return std::vector<std::byte>{static_cast<std::byte>(value)}; }

inline auto internal_serialize_fn(var &&value) { return internal_serialize_var(value); }

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>>,
                        std::vector<std::byte>>::type inline internal_serialize_fn(T &&value) {
  return internal_serialize_variant(value);
}

template <typename T>
typename std::enable_if<std::is_integral_v<std::decay_t<T>> || std::is_floating_point_v<std::decay_t<T>>,
                        std::array<std::byte, sizeof(T)>>::type inline internal_serialize_fn(T &&value) {
  return internal_serialize_numeric(value);
}

template <typename T>
typename std::enable_if<is_std_vector_v<std::decay_t<T>> || is_std_set_v<std::decay_t<T>>,
                        std::vector<std::byte>>::type inline internal_serialize_fn(T &&value) {
  std::vector<std::byte> result;
  result.resize(sizeof(std::size_t) + value.size() * ((value.size()) ? enigma_internal_sizeof(*value.begin()) : 0));
  internal_serialize_into<std::size_t>(result.data(), value.size());

  auto dataPtr = result.data() + sizeof(std::size_t);
  for (const auto &element : value) {
    internal_serialize_into(dataPtr, element);
    dataPtr += enigma_internal_sizeof(element);
  }
  return result;
}

template <typename T>
typename std::enable_if<is_std_map_v<std::decay_t<T>>, std::vector<std::byte>>::type inline internal_serialize_fn(
    T &&value) {
  std::vector<std::byte> result;
  result.resize(sizeof(std::size_t) + value.size() * ((value.size()) ? (enigma_internal_sizeof(value.begin()->first) +
                                                                        enigma_internal_sizeof(value.begin()->second))
                                                                     : 0));
  internal_serialize_into<std::size_t>(result.data(), value.size());

  auto dataPtr = result.data() + sizeof(std::size_t);
  for (const auto &element : value) {
    internal_serialize_into(dataPtr, element.first);
    internal_serialize_into(dataPtr, element.second);
    dataPtr += enigma_internal_sizeof(element.first) + enigma_internal_sizeof(element.second);
  }
  return result;
}

template <typename T>
typename std::enable_if<is_std_complex_v<std::decay_t<T>>, std::vector<std::byte>>::type inline internal_serialize_fn(
    T &&value) {
  std::vector<std::byte> result;
  result.resize(2 * enigma_internal_sizeof(value.real()));

  auto dataPtr = result.data();
  internal_serialize_into(dataPtr, value.real());
  internal_serialize_into(dataPtr, value.imag());
  return result;
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
typename std::enable_if<std::is_same_v<std::string, std::decay_t<T>>, T>::type inline internal_deserialize_fn(
    std::byte *iter) {
  std::size_t len = internal_deserialize_numeric<std::size_t>(iter);
  std::size_t offset = sizeof(std::size_t);
  std::string result{reinterpret_cast<char *>(iter + offset), reinterpret_cast<char *>(iter + offset + len)};
  return result;
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
typename std::enable_if<(std::is_integral_v<std::decay_t<T>> ||
                         std::is_floating_point_v<std::decay_t<T>>)&&!std::is_same_v<std::decay_t<T>, bool>,
                        T>::type inline internal_deserialize_fn(std::byte *iter) {
  return internal_deserialize_numeric<T>(iter);
}

// needs a better place
template <typename Container, typename T>
typename std::enable_if<is_std_vector_v<std::decay_t<Container>>>::type inline insert_back(Container &container,
                                                                                           const T &val) {
  container.push_back(std::move(val));
}

template <typename Container, typename T>
typename std::enable_if<is_std_set_v<std::decay_t<Container>>>::type inline insert_back(Container &container,
                                                                                        const T &val) {
  container.insert(std::move(val));
}
//

template <typename T>
typename std::enable_if<is_std_vector_v<std::decay_t<T>> || is_std_set_v<std::decay_t<T>>,
                        T>::type inline internal_deserialize_fn(std::byte *iter) {
  std::size_t size = internal_deserialize_numeric<std::size_t>(iter);
  std::size_t offset = sizeof(std::size_t);

  using InnerType = typename T::value_type;
  std::vector<InnerType> result;
  result.reserve(size);

  for (std::size_t i = 0; i < size; ++i) {
    InnerType element = internal_deserialize<InnerType>(iter + offset);
    insert_back(result, std::move(element));
    offset += enigma_internal_sizeof(element);
  }
  return result;
}

template <typename T>
typename std::enable_if<is_std_map_v<std::decay_t<T>>, T>::type inline internal_deserialize_fn(std::byte *iter) {
  std::size_t size = internal_deserialize_numeric<std::size_t>(iter);
  std::size_t offset = sizeof(std::size_t);

  using KeyType = typename std::decay_t<T>::key_type;
  using ValueType = typename std::decay_t<T>::mapped_type;
  std::map<KeyType, ValueType> result;
  result.reserve(size);

  for (std::size_t i = 0; i < size; ++i) {
    KeyType key = internal_deserialize<KeyType>(iter + offset);
    offset += enigma_internal_sizeof(key);
    ValueType val = internal_deserialize<ValueType>(iter + offset);
    offset += enigma_internal_sizeof(val);
    result.insert(std::move(std::pair<KeyType, ValueType>{key, val}));
  }
  return result;
}

template <typename T>
typename std::enable_if<is_std_complex_v<std::decay_t<T>>, T>::type inline internal_deserialize_fn(std::byte *iter) {
  std::size_t offset = 0;
  using InnerType = typename complex_inner_type<std::decay_t<T>>::type;

  InnerType Real = internal_deserialize<InnerType>(iter + offset);
  offset += enigma_internal_sizeof(Real);
  InnerType Imag = internal_deserialize<InnerType>(iter + offset);
  offset += enigma_internal_sizeof(Imag);
  std::complex<InnerType> result(Real, Imag);
  return result;
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

inline void internal_resize_buffer_for_var(std::vector<std::byte> &buffer, const var &value) {
  buffer.resize(buffer.size() + var_size(value));
}

inline void internal_resize_buffer_for_variant(std::vector<std::byte> &buffer, const variant &value) {
  buffer.resize(buffer.size() + variant_size(value));
}

inline void internal_resize_buffer_for_string(std::vector<std::byte> &buffer, const std::string &value) {
  buffer.resize(buffer.size() + value.size() + sizeof(std::size_t));
}

template <typename T>
typename std::enable_if<is_std_vector_v<std::decay_t<T>> || is_std_set_v<std::decay_t<T>>>::
    type inline internal_resize_buffer_for_vector_set(std::vector<std::byte> &buffer, const T &value) {
  buffer.resize(buffer.size() + value.size() * ((value.size()) ? enigma_internal_sizeof(*value.begin()) : 0) +
                sizeof(std::size_t));
}

template <typename T, typename U>
void internal_resize_buffer_for_map(std::vector<std::byte> &buffer, const std::map<T, U> &value) {
  buffer.resize(buffer.size() +
                value.size() * ((value.size()) ? (enigma_internal_sizeof(value.begin()->first) +
                                                  enigma_internal_sizeof(value.begin()->second))
                                               : 0) +
                sizeof(std::size_t));
}

template <typename T>
void internal_resize_buffer_for_complex(std::vector<std::byte> &buffer, const std::complex<T> &value) {
  buffer.resize(
      buffer.size() +
      2 * enigma_internal_sizeof(value.real()));  // we don't need to store the size of the complex, it is always 1
}

template <typename T, typename = std::enable_if_t<has_byte_size_method_v<T>>>
inline void internal_resize_buffer_using_byte_size(std::vector<std::byte> &buffer, const T &value) {
  buffer.resize(buffer.size() + value.byte_size());
}

template <typename T>
typename std::enable_if<std::is_same_v<var, std::decay_t<T>>>::type inline internal_resize_buffer_for_fn(
    std::vector<std::byte> &buffer, T &&value) {
  internal_resize_buffer_for_var(buffer, value);
}

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>> && !std::is_same_v<var, std::decay_t<T>>>::
    type inline internal_resize_buffer_for_fn(std::vector<std::byte> &buffer, T &&value) {
  internal_resize_buffer_for_variant(buffer, value);
}

template <typename T>
typename std::enable_if<std::is_same_v<std::string, std::decay_t<T>>>::type inline internal_resize_buffer_for_fn(
    std::vector<std::byte> &buffer, T &&value) {
  internal_resize_buffer_for_string(buffer, value);
}

template <typename T>
typename std::enable_if<is_std_vector_v<std::decay_t<T>> || is_std_set_v<std::decay_t<T>>>::
    type inline internal_resize_buffer_for_fn(std::vector<std::byte> &buffer, T &&value) {
  internal_resize_buffer_for_vector_set(buffer, value);
}

template <typename T>
typename std::enable_if<is_std_map_v<std::decay_t<T>>>::type inline internal_resize_buffer_for_fn(
    std::vector<std::byte> &buffer, T &&value) {
  internal_resize_buffer_for_map(buffer, value);
}

template <typename T>
typename std::enable_if<is_std_complex_v<std::decay_t<T>>>::type inline internal_resize_buffer_for_fn(
    std::vector<std::byte> &buffer, T &&value) {
  internal_resize_buffer_for_complex(buffer, value);
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
typename std::enable_if<std::is_same_v<std::string, std::decay_t<T>>>::type inline enigma_internal_deserialize_fn(
    T &value, std::byte *iter, std::size_t &len) {
  value = enigma::internal_deserialize<std::string>(iter + len);
  len += value.length() + sizeof(std::size_t);
}

template <typename T>
typename std::enable_if<is_std_vector_v<std::decay_t<T>> ||
                        is_std_set_v<std::decay_t<T>>>::type inline enigma_internal_deserialize_fn(T &value,
                                                                                                   std::byte *iter,
                                                                                                   std::size_t &len) {
  std::size_t size = enigma::internal_deserialize_numeric<std::size_t>(iter + len);
  len += sizeof(std::size_t);
  value.clear();
  // value.reserve(size);
  using InnerType = typename T::value_type;

  for (std::size_t i = 0; i < size; ++i) {
    InnerType element = enigma::internal_deserialize<InnerType>(iter + len);
    insert_back(value, std::move(element));
    len += enigma_internal_sizeof(element);
  }
}

template <typename T>
typename std::enable_if<is_std_map_v<std::decay_t<T>>>::type inline enigma_internal_deserialize_fn(T &value,
                                                                                                   std::byte *iter,
                                                                                                   std::size_t &len) {
  std::size_t size = enigma::internal_deserialize_numeric<std::size_t>(iter + len);
  len += sizeof(std::size_t);
  value.clear();
  using KeyType = typename std::decay_t<T>::key_type;
  using ValueType = typename std::decay_t<T>::mapped_type;

  for (std::size_t i = 0; i < size; ++i) {
    KeyType key = enigma::internal_deserialize<KeyType>(iter + len);
    len += enigma_internal_sizeof(key);
    ValueType val = enigma::internal_deserialize<ValueType>(iter + len);
    len += enigma_internal_sizeof(val);
    value.insert(std::move(std::pair<KeyType, ValueType>{key, val}));
  }
}

template <typename T>
typename std::enable_if<is_std_complex_v<std::decay_t<T>>>::type inline enigma_internal_deserialize_fn(
    T &value, std::byte *iter, std::size_t &len) {
  using InnerType = typename complex_inner_type<std::decay_t<T>>::type;

  InnerType Real = enigma::internal_deserialize<InnerType>(iter + len);
  len += enigma_internal_sizeof(Real);
  InnerType Imag = enigma::internal_deserialize<InnerType>(iter + len);
  len += enigma_internal_sizeof(Imag);
  value = std::complex<InnerType>(Real, Imag);
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
