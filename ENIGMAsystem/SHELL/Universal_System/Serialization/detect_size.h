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

#include <type_traits>
#include "serialization_fwd_decl.h"

namespace enigma {

inline std::size_t variant_size(const variant &value) {
  if (value.type == variant::ty_real) {
    return 9;
  } else {
    return 1 + sizeof(std::size_t) + value.sval().length();
  }
}

inline std::size_t var_size(const var &value) {
  std::size_t len = variant_size(value) + enigma_internal_sizeof(value.array1d);
  len += (3 * sizeof(std::size_t));
  for (auto &[key, elem] : value.array2d.sparse_part()) {
    len += enigma_internal_sizeof(elem);
  }
  for (auto &elem : value.array2d.dense_part()) {
    len += enigma_internal_sizeof(elem);
  }
  return len;
}

inline std::size_t byte_size(const variant &value) { return variant_size(value); }

inline std::size_t byte_size(const var &value) { return var_size(value); }

template <typename T>
typename std::enable_if<is_std_vector_v<std::decay_t<T>> || is_std_set_v<std::decay_t<T>>,
                        std::size_t>::type inline byte_size(const T &value) {
  std::size_t totalSize = sizeof(std::size_t);

  for (const auto &element : value) {
    totalSize += enigma_internal_sizeof(element);
  }
  return totalSize;
}

template <typename T>
typename std::enable_if<is_std_queue_v<std::decay_t<T>> || is_std_stack_v<std::decay_t<T>>,
                        std::size_t>::type inline byte_size(const T &value) {
  std::size_t totalSize = sizeof(std::size_t);
  std::decay_t<T> tempContainer = value;

  while (!tempContainer.empty()) {
    totalSize += enigma_internal_sizeof(get_top(tempContainer));
    tempContainer.pop();
  }

  return totalSize;
}

template <typename T, typename U>
inline std::size_t byte_size(const std::map<T, U> &value) {
  std::size_t totalSize = sizeof(std::size_t);

  for (const auto &element : value) {
    totalSize += enigma_internal_sizeof(element.first) + enigma_internal_sizeof(element.second);
  }

  return totalSize;
}

template <typename T>
inline std::size_t byte_size(const std::complex<T> &value) {
  return sizeof(T) * 2;  // we don't need enigma_internal_sizeof
}

template <typename T, typename U>
inline std::size_t byte_size(const std::pair<T, U> &value) {
  return enigma_internal_sizeof(value.first) + enigma_internal_sizeof(value.second);
}

template <typename T, typename U, typename V>
inline std::size_t byte_size(const std::tuple<T, U, V> &value) {
  return enigma_internal_sizeof(std::get<0>(value)) + enigma_internal_sizeof(std::get<1>(value)) +
         enigma_internal_sizeof(std::get<2>(value));
}

template <typename T>
inline std::size_t byte_size(const lua_table<T> &value) {
  return (3 * sizeof(std::size_t)) +  // The three different lengths (`mx_size`, `sparse.size()`, `dense.size()`)
         value.sparse_part().size() * (sizeof(T)) +                      // The elements of `dense`
         value.dense_part().size() * (sizeof(std::size_t) + sizeof(T));  // The elements of `sparse`
}

}  // namespace enigma
#endif
