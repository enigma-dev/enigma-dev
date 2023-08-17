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

inline std::size_t variant_size(const variant &value) {
  if (value.type == variant::ty_real) {
    return 9;
  } else {
    return 1 + sizeof(std::size_t) + value.sval().length();
  }
}

template <typename T>
is_t<T, var, std::size_t> inline enigma::byte_size(const T &value) {
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

template <typename T>
is_t<T, var> inline enigma::internal_serialize_into_fn(std::byte *iter, T &&value) {
  std::size_t pos = 0;
  internal_serialize_into<const variant &>(iter, value);
  pos += variant_size(value);
  enigma_internal_serialize_lua_table(iter + pos, value.array1d);
  pos += enigma_internal_sizeof(value.array1d);
  enigma_internal_serialize_lua_table(iter + pos, value.array2d);
}

template <typename T>
is_t<T, var, std::vector<std::byte>> inline enigma::internal_serialize_fn(const T &value) {
  std::vector<std::byte> result;
  result.resize(enigma_internal_sizeof(value));
  internal_serialize_into(result.data(), value);
  return result;
}

template <typename T>
is_t<T, var, T> inline enigma::internal_deserialize_fn(std::byte *iter) {
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
is_t<T, var> inline enigma::internal_resize_buffer_for_fn(std::vector<std::byte> &buffer, T &&value) {
  buffer.resize(buffer.size() + enigma::enigma_internal_sizeof(value));
}

template <typename T>
is_t<T, var> inline enigma::enigma_internal_deserialize_fn(T &value, std::byte *iter, std::size_t &len) {
  value = internal_deserialize<var>(iter + len);
  len += enigma::enigma_internal_sizeof(value);
}
