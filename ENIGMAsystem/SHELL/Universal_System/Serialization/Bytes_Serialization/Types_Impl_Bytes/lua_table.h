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
  @file lua_table.h
  @brief This file contains the implementation of bytes (de)serialization functions for lua_table.
*/

#ifndef ENIGMA_SERIALIZE_LUA_TABLE_BYTES_H
#define ENIGMA_SERIALIZE_LUA_TABLE_BYTES_H

#include "../../serialization_fwd_decl.h"

namespace enigma {
namespace bytes_serialization {

template <typename T>
inline matches_t<T, std::size_t, is_lua_table> byte_size(const T &value) {
  std::size_t totalSize = sizeof(std::size_t);

  totalSize += enigma_internal_sizeof(value.dense_part());  // The elements of `dense`, we can use `byte_size` directly
  totalSize +=
      enigma_internal_sizeof(value.sparse_part());  // The elements of `sparse`, we can use `byte_size` directly

  return totalSize;
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

template <typename T>
inline matches_t<T, void, is_lua_table> enigma_internal_deserialize_fn(T &value, std::byte *iter, std::size_t &len) {
  using Type = typename lua_inner_type<T>::type;
  value = enigma_internal_deserialize_lua_table<Type>(iter);
  len += enigma_internal_sizeof(value);
}

}  // namespace bytes_serialization
}  // namespace enigma

#endif
