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

#ifndef ENIGMA_SERIALIZE_PAIR_BYTES_H
#define ENIGMA_SERIALIZE_PAIR_BYTES_H

#include "../../serialization_fwd_decl.h"

namespace enigma {
namespace bytes_serialization {

template <typename T>
matches_t<T, std::size_t, is_std_pair> inline byte_size(const T &value) {
  return enigma_internal_sizeof(value.first) + enigma_internal_sizeof(value.second);
}

template <typename T>
matches_t<T, void, is_std_pair> inline internal_serialize_into_fn(std::byte *iter, T &&value) {
  internal_serialize_into(iter, value.first);
  iter += enigma_internal_sizeof(value.first);
  internal_serialize_into(iter, value.second);
  iter += enigma_internal_sizeof(value.second);
}

template <typename T>
matches_t<T, T, is_std_pair> inline internal_deserialize_fn(std::byte *iter) {
  std::size_t offset = 0;
  using firsttype = typename T::first_type;
  using secondtype = typename T::second_type;

  firsttype first = internal_deserialize<firsttype>(iter + offset);
  offset += enigma_internal_sizeof(first);
  secondtype second = internal_deserialize<secondtype>(iter + offset);
  offset += enigma_internal_sizeof(second);
  std::pair<firsttype, secondtype> result(first, second);
  return result;
}

template <typename T>
matches_t<T, void, is_std_pair> inline enigma_internal_deserialize_fn(T &value, std::byte *iter, std::size_t &len) {
  using firsttype = typename T::first_type;
  using secondtype = typename T::second_type;

  firsttype first = internal_deserialize<firsttype>(iter + len);
  len += enigma_internal_sizeof(first);
  secondtype second = internal_deserialize<secondtype>(iter + len);
  len += enigma_internal_sizeof(second);
  value = std::pair<firsttype, secondtype>(first, second);
}

}  // namespace bytes_serialization
}  // namespace enigma

#endif
