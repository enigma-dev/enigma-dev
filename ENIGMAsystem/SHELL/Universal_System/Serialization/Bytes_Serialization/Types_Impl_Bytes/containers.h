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

/**
  @file containers.h
  @brief This file contains the implementation of bytes serialization function for std::vector
  and std::set, and bytes deserialization function for std::vector, std::set and std::queue. 
*/

#ifndef ENIGMA_SERIALIZE_CONTAINERS_BYTES_H
#define ENIGMA_SERIALIZE_CONTAINERS_BYTES_H

#include "../../serialization_fwd_decl.h"

namespace enigma {
namespace bytes_serialization {

template <typename T>
inline matches_t<T, std::size_t, is_std_vector, is_std_set> byte_size(const T &value) {
  std::size_t totalSize = sizeof(std::size_t);

  for (const auto &element : value) {
    totalSize += enigma_internal_sizeof(element);
  }
  return totalSize;
}

template <typename T>
inline matches_t<T, void, is_std_vector, is_std_set> internal_serialize_into_fn(std::byte *iter, T &&value) {
  internal_serialize_into<std::size_t>(iter, value.size());
  iter += sizeof(std::size_t);

  for (const auto &element : value) {
    internal_serialize_into(iter, element);
    iter += enigma_internal_sizeof(element);
  }
}

template <typename T>
inline matches_t<T, std::vector<std::byte>, is_std_vector, is_std_set> internal_serialize_fn(T &&value) {
  std::vector<std::byte> result;
  result.resize(enigma_internal_sizeof(value));
  internal_serialize_into<std::size_t>(result.data(), value.size());

  auto dataPtr = result.data() + sizeof(std::size_t);
  for (const auto &element : value) {
    internal_serialize_into(dataPtr, element);
    dataPtr += enigma_internal_sizeof(element);
  }
  return result;
}

template <typename T>
inline matches_t<T, T, is_std_vector, is_std_set, is_std_queue, is_std_stack> internal_deserialize_fn(std::byte *iter) {
  std::size_t size = internal_deserialize<std::size_t>(iter);
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
inline matches_t<T, void, is_std_vector, is_std_set, is_std_queue, is_std_stack> internal_resize_buffer_for_fn(
    std::vector<std::byte> &buffer, T &&value) {
  buffer.resize(buffer.size() + enigma_internal_sizeof(value));
}

template <typename T>
inline matches_t<T, void, is_std_vector, is_std_set, is_std_queue> enigma_internal_deserialize_fn(T &value,
                                                                                                  std::byte *iter,
                                                                                                  std::size_t &len) {
  std::size_t size = internal_deserialize<std::size_t>(iter + len);
  len += sizeof(std::size_t);
  using InnerType = typename T::value_type;

  for (std::size_t i = 0; i < size; ++i) {
    InnerType element = internal_deserialize<InnerType>(iter + len);
    insert_back(value, std::move(element));
    len += enigma_internal_sizeof(element);
  }
}

}  // namespace bytes_serialization
}  // namespace enigma

#endif
