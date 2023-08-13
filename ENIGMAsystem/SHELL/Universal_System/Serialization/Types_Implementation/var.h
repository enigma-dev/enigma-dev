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

inline void enigma::internal_serialize_var_into(std::byte *iter, const var &value) {
  std::size_t pos = 0;
  internal_serialize_into<const variant &>(iter, value);
  pos += variant_size(value);
  enigma_internal_serialize_lua_table(iter + pos, value.array1d);
  pos += enigma_internal_sizeof(value.array1d);
  enigma_internal_serialize_lua_table(iter + pos, value.array2d);
}

inline std::vector<std::byte> enigma::internal_serialize_var(const var &var) {
  std::vector<std::byte> result;
  result.resize(enigma_internal_sizeof(var));
  internal_serialize_var_into(result.data(), var);
  return result;
}

inline var enigma::internal_deserialize_var(std::byte *iter) {
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
typename std::enable_if<std::is_same_v<var, std::decay_t<T>>>::type inline enigma::internal_serialize_into_fn(
    std::byte *iter, T &&value) {
  internal_serialize_var_into(iter, value);
}

inline auto enigma::internal_serialize_fn(var &&value) { return internal_serialize_var(value); }

template <typename T>
typename std::enable_if<std::is_same_v<var, std::decay_t<T>>, T>::type inline enigma::internal_deserialize_fn(
    std::byte *iter) {
  return internal_deserialize_var(iter);
}

template <typename T>
typename std::enable_if<std::is_same_v<var, std::decay_t<T>>>::type inline enigma::internal_resize_buffer_for_fn(
    std::vector<std::byte> &buffer, T &&value) {
  buffer.resize(buffer.size() + var_size(value));
}

template <>
inline void enigma::enigma_internal_deserialize_fn(var &value, std::byte *iter, std::size_t &len) {
  value = internal_deserialize_var(iter + len);
  len += var_size(value);
}
