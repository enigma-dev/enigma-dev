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

#ifndef ENIGMA_SERIALIZE_COMPLEX_BYTES_H
#define ENIGMA_SERIALIZE_COMPLEX_BYTES_H

#include "../../serialization_fwd_decl.h"

namespace enigma {
namespace bytes_serialization {

template <typename T>
matches_t<T, std::size_t, is_std_complex> inline byte_size(const T &value) {
  return sizeof(value.real()) * 2;  // we don't need enigma_internal_sizeof
}

template <typename T>
matches_t<T, void, is_std_complex> inline internal_serialize_into_fn(std::byte *iter, T &&value) {
  internal_serialize_into(iter, value.real());
  iter += enigma_internal_sizeof(value.real());
  internal_serialize_into(iter, value.imag());
  iter += enigma_internal_sizeof(value.imag());
}

template <typename T>
matches_t<T, std::vector<std::byte>, is_std_complex, is_std_pair> inline internal_serialize_fn(T &&value) {
  std::vector<std::byte> result;
  result.resize(enigma_internal_sizeof(value));

  auto dataPtr = result.data();
  internal_serialize_into(dataPtr, value.real());
  dataPtr += enigma_internal_sizeof(value.real());
  internal_serialize_into(dataPtr, value.imag());
  dataPtr += enigma_internal_sizeof(value.imag());
  return result;
}

template <typename T>
matches_t<T, T, is_std_complex> inline internal_deserialize_fn(std::byte *iter) {
  std::size_t offset = 0;
  using InnerType = typename complex_inner_type<std::decay_t<T>>::type;

  InnerType Real = internal_deserialize<InnerType>(iter + offset);
  offset += enigma_internal_sizeof(Real);
  InnerType Imag = internal_deserialize<InnerType>(iter + offset);
  offset += enigma_internal_sizeof(Imag);
  std::complex<InnerType> result(Real, Imag);
  return result;
}

template <typename T>
matches_t<T, void, is_std_complex, is_std_pair> inline internal_resize_buffer_for_fn(std::vector<std::byte> &buffer,
                                                                                     T &&value) {
  buffer.resize(buffer.size() + enigma_internal_sizeof(value));
}

template <typename T>
matches_t<T, void, is_std_complex> inline enigma_internal_deserialize_fn(T &value, std::byte *iter, std::size_t &len) {
  using InnerType = typename complex_inner_type<std::decay_t<T>>::type;

  InnerType Real = internal_deserialize<InnerType>(iter + len);
  len += enigma_internal_sizeof(Real);
  InnerType Imag = internal_deserialize<InnerType>(iter + len);
  len += enigma_internal_sizeof(Imag);
  value = std::complex<InnerType>(Real, Imag);
}

}  // namespace bytes_serialization
}  // namespace enigma

#endif
