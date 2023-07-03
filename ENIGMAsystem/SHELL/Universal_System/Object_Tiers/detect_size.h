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

#ifndef ENIGMA_DETECT_SIZE_H
#define ENIGMA_DETECT_SIZE_H

#include<type_traits>
#include "../var4.h"

inline std::size_t variant_size(const variant &value) {
  if (value.type == variant::ty_real) {
    return 9;
  } else {
    return 1 + sizeof(std::size_t) + value.sval().length();
  }
}

template <typename T>
inline std::size_t enigma_internal_sizeof_lua_table(const lua_table<T> &table) {
  return (3 * sizeof(std::size_t)) + // The three different lengths (`mx_size`, `sparse.size()`, `dense.size()`)
         table.sparse_part().size() * (sizeof(T)) + // The elements of `dense`
         table.dense_part().size() * (sizeof(std::size_t) + sizeof(T)); // The elements of `sparse`
}

inline std::size_t var_size(const var &value) {
  std::size_t len = variant_size(value) + enigma_internal_sizeof_lua_table(value.array1d);
  len += (3 * sizeof(std::size_t));
  for (auto &[key, elem] : value.array2d.sparse_part()) {
    len += enigma_internal_sizeof_lua_table(elem);
  }
  for (auto &elem : value.array2d.dense_part()) {
    len += enigma_internal_sizeof_lua_table(elem);
  }
  return len;
}

template <typename T>
struct is_lua_table : std::false_type {};

template <typename U>
struct is_lua_table<lua_table<U>> : std::true_type {
  using inner_type = U;
};

template <typename T>
constexpr static inline bool is_lua_table_v = is_lua_table<T>::value;

inline std::size_t Size(const variant& value) {
  return variant_size(value);
}

inline std::size_t Size(const var& value) {
  return var_size(value);
}

template <typename T>
inline std::size_t Size(const lua_table<T>& value) {
  return enigma_internal_sizeof_lua_table(value);
}

// This is a helper struct to check if there is a specialization for the `Size` function which is callable with this type
template <typename U>
struct CheckSizeFunction {
  template <typename T>
  static constexpr auto Check(T*) -> decltype(Size(std::declval<T>()), std::true_type{});

  template <typename>
  static constexpr std::false_type Check(...);

  using type = decltype(Check<U>(nullptr));
  static constexpr bool value = type::value;
};

template <typename T>
constexpr static inline bool has_free_size_function_v = CheckSizeFunction<T>::value;

#endif