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
typename std::enable_if<is_std_queue_v<std::decay_t<T>> || is_std_stack_v<std::decay_t<T>>>::type inline enigma::
    internal_serialize_into_fn(std::byte *iter, T &&value) {
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
typename std::enable_if<is_std_queue_v<std::decay_t<T>> || is_std_stack_v<std::decay_t<T>>,
                        std::vector<std::byte>>::type inline enigma::internal_serialize_fn(T &&value) {
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
typename std::enable_if<is_std_stack_v<std::decay_t<T>>>::type inline enigma::enigma_internal_deserialize_fn(
    T &value, std::byte *iter, std::size_t &len) {
  std::size_t size = enigma::internal_deserialize_numeric<std::size_t>(iter + len);
  len += sizeof(std::size_t);

  using InnerType = typename T::value_type;
  std::stack<InnerType> tempStack;

  for (std::size_t i = 0; i < size; ++i) {
    InnerType element = enigma::internal_deserialize<InnerType>(iter + len);
    insert_back(tempStack, std::move(element));
    len += enigma_internal_sizeof(element);
  }

  while (!tempStack.empty()) {
    insert_back(value, std::move(get_top(tempStack)));
    tempStack.pop();
  }
}
