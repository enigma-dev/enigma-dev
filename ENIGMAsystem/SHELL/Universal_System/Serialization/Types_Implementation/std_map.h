/** Copyright (C) 2023 Fares Atef
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
matches_t<T, std::size_t, is_std_map> inline enigma::byte_size(const T &value) {
  std::size_t totalSize = sizeof(std::size_t);

  for (const auto &element : value) {
    totalSize += enigma_internal_sizeof(element.first) + enigma_internal_sizeof(element.second);
  }

  return totalSize;
}

template <typename T>
matches_t<T, void, is_std_map> inline enigma::internal_serialize_into_fn(std::byte *iter, T &&value) {
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
matches_t<T, std::vector<std::byte>, is_std_map> inline enigma::internal_serialize_fn(T &&value) {
  std::vector<std::byte> result;
  result.resize(enigma_internal_sizeof(value));
  internal_serialize_into<std::size_t>(result.data(), value.size());

  auto dataPtr = result.data() + sizeof(std::size_t);
  for (const auto &element : value) {
    internal_serialize_into(dataPtr, element.first);
    dataPtr+=enigma_internal_sizeof(element.first);
    internal_serialize_into(dataPtr, element.second);
    dataPtr += enigma_internal_sizeof(element.second);
  }
  return result;
}

template <typename T>
matches_t<T, T, is_std_map> inline enigma::internal_deserialize_fn(std::byte *iter) {
  std::size_t size = internal_deserialize<std::size_t>(iter);
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
matches_t<T, void, is_std_map> inline enigma::internal_resize_buffer_for_fn(std::vector<std::byte> &buffer, T &&value) {
  buffer.resize(buffer.size() + enigma_internal_sizeof(value));
}

template <typename T>
matches_t<T, void, is_std_map> inline enigma::enigma_internal_deserialize_fn(T &value, std::byte *iter,
                                                                             std::size_t &len) {
  std::size_t size = enigma::internal_deserialize<std::size_t>(iter + len);
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
