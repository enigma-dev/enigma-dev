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
matches_t<T, std::size_t, is_std_tuple> inline enigma::byte_size(const T &value) {
  return enigma_internal_sizeof(std::get<0>(value)) + enigma_internal_sizeof(std::get<1>(value)) +
         enigma_internal_sizeof(std::get<2>(value));
}

template <typename T>
matches_t<T, void, is_std_tuple> inline enigma::internal_serialize_into_fn(std::byte *iter, T &&value) {
  internal_serialize_into(iter, std::get<0>(value));
  iter += enigma_internal_sizeof(std::get<0>(value));
  internal_serialize_into(iter, std::get<1>(value));
  iter += enigma_internal_sizeof(std::get<1>(value));
  internal_serialize_into(iter, std::get<2>(value));
  iter += enigma_internal_sizeof(std::get<2>(value));
}

template <typename T>
matches_t<T, std::vector<std::byte>, is_std_tuple> inline enigma::internal_serialize_fn(T &&value) {
  std::vector<std::byte> result;
  result.resize(enigma_internal_sizeof(value));

  auto dataPtr = result.data();
  internal_serialize_into(dataPtr, std::get<0>(value));
  internal_serialize_into(dataPtr, std::get<1>(value));
  internal_serialize_into(dataPtr, std::get<2>(value));
  return result;
}

template <typename T>
matches_t<T, T, is_std_tuple> inline enigma::internal_deserialize_fn(std::byte *iter) {
  std::size_t offset = 0;
  using firsttype = typename TupleTypeExtractor<T>::FirstType;
  using secondtype = typename TupleTypeExtractor<T>::SecondType;
  using thirdtype = typename TupleTypeExtractor<T>::ThirdType;

  firsttype val1 = internal_deserialize<firsttype>(iter + offset);
  offset += enigma_internal_sizeof(val1);
  secondtype val2 = internal_deserialize<secondtype>(iter + offset);
  offset += enigma_internal_sizeof(val2);
  thirdtype val3 = internal_deserialize<thirdtype>(iter + offset);
  offset += enigma_internal_sizeof(val3);
  std::tuple<firsttype, secondtype, thirdtype> result(val1, val2, val3);
  return result;
}

template <typename T>
matches_t<T, void, is_std_tuple> inline enigma::internal_resize_buffer_for_fn(std::vector<std::byte> &buffer,
                                                                              T &&value) {
  buffer.resize(buffer.size() + enigma_internal_sizeof(value));
}

template <typename T>
matches_t<T, void, is_std_tuple> inline enigma::enigma_internal_deserialize_fn(T &value, std::byte *iter,
                                                                               std::size_t &len) {
  using firsttype = typename TupleTypeExtractor<T>::FirstType;
  using secondtype = typename TupleTypeExtractor<T>::SecondType;
  using thirdtype = typename TupleTypeExtractor<T>::ThirdType;

  firsttype val1 = enigma::internal_deserialize<firsttype>(iter + len);
  len += enigma_internal_sizeof(val1);
  secondtype val2 = enigma::internal_deserialize<secondtype>(iter + len);
  len += enigma_internal_sizeof(val2);
  thirdtype val3 = enigma::internal_deserialize<thirdtype>(iter + len);
  len += enigma_internal_sizeof(val3);
  value = std::tuple<firsttype, secondtype, thirdtype>(val1, val2, val3);
}
