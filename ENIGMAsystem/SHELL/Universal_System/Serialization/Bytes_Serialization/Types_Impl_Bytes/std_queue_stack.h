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

#ifndef ENIGMA_SERIALIZE_QUEUE_STACK_BYTES_H
#define ENIGMA_SERIALIZE_QUEUE_STACK_BYTES_H

#include "../../serialization_fwd_decl.h"

namespace enigma {
namespace bytes_serialization {

template <typename T>
matches_t<T, std::size_t, is_std_queue, is_std_stack> inline byte_size(const T &value) {
  std::size_t totalSize = sizeof(std::size_t);
  std::decay_t<T> tempContainer = value;

  while (!tempContainer.empty()) {
    totalSize += enigma_internal_sizeof(get_top(tempContainer));
    tempContainer.pop();
  }

  return totalSize;
}

template <typename T>
matches_t<T, void, is_std_queue, is_std_stack> inline internal_serialize_into_fn(std::byte *iter, T &&value) {
  std::decay_t<T> tempContainer = value;

  internal_serialize_into<std::size_t>(iter, tempContainer.size());
  iter += sizeof(std::size_t);

  while (!tempContainer.empty()) {
    internal_serialize_into(iter, get_top(tempContainer));
    iter += enigma_internal_sizeof(get_top(tempContainer));
    tempContainer.pop();
  }
}

template <typename T>
matches_t<T, std::vector<std::byte>, is_std_queue, is_std_stack> inline internal_serialize_fn(T &&value) {
  std::vector<std::byte> result;
  result.resize(enigma_internal_sizeof(value));
  internal_serialize_into<std::size_t>(result.data(), value.size());

  auto dataPtr = result.data() + sizeof(std::size_t);
  std::decay_t<T> tempContainer = value;

  while (!tempContainer.empty()) {
    internal_serialize_into(dataPtr, get_top(tempContainer));
    dataPtr += enigma_internal_sizeof(get_top(tempContainer));
    tempContainer.pop();
  }
  return result;
}

template <typename T>
matches_t<T, void, is_std_stack> inline enigma_internal_deserialize_fn(T &value, std::byte *iter, std::size_t &len) {
  std::size_t size = internal_deserialize<std::size_t>(iter + len);
  len += sizeof(std::size_t);

  using InnerType = typename T::value_type;
  std::stack<InnerType> tempStack;

  for (std::size_t i = 0; i < size; ++i) {
    InnerType element = internal_deserialize<InnerType>(iter + len);
    insert_back(tempStack, std::move(element));
    len += enigma_internal_sizeof(element);
  }

  while (!tempStack.empty()) {
    insert_back(value, std::move(get_top(tempStack)));
    tempStack.pop();
  }
}

}  // namespace bytes_serialization
}  // namespace enigma

#endif
