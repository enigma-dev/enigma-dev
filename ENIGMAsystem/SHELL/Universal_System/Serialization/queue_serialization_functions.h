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

#include "serialization_fwd_decl.h"

template <typename T>
typename std::enable_if<is_std_queue_v<std::decay_t<T>>>::type inline enigma::internal_serialize_into_fn(
    std::byte *iter, T &&value) {
  using InnerType = typename std::decay_t<T>::value_type;
  std::queue<InnerType> tempQueue = value;

  internal_serialize_into<std::size_t>(iter, tempQueue.size());
  iter += sizeof(std::size_t);

  while (!tempQueue.empty()) {
    internal_serialize_into(iter, tempQueue.front());
    iter += enigma_internal_sizeof(tempQueue.front());
    tempQueue.pop();
  }
}
