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
typename std::enable_if<is_std_complex_v<std::decay_t<T>>>::type inline enigma::internal_serialize_into_fn(
    std::byte *iter, T &&value) {
  internal_serialize_into(iter, value.real());
  iter += enigma_internal_sizeof(value.real());
  internal_serialize_into(iter, value.imag());
  iter += enigma_internal_sizeof(value.imag());
}

template <typename T>
typename std::enable_if<is_std_complex_v<std::decay_t<T>>,
                        std::vector<std::byte>>::type inline enigma::internal_serialize_fn(T &&value) {
  std::vector<std::byte> result;
  result.resize(2 * enigma_internal_sizeof(value.real()));

  auto dataPtr = result.data();
  internal_serialize_into(dataPtr, value.real());
  internal_serialize_into(dataPtr, value.imag());
  return result;
}

template <typename T>
typename std::enable_if<is_std_complex_v<std::decay_t<T>>, T>::type inline enigma::internal_deserialize_fn(
    std::byte *iter) {
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
typename std::enable_if<is_std_complex_v<std::decay_t<T>>>::type inline enigma::internal_resize_buffer_for_fn(
    std::vector<std::byte> &buffer, T &&value) {
  buffer.resize(
      buffer.size() +
      2 * enigma_internal_sizeof(value.real()));  // we don't need to store the size of the complex, it is always 1
}

template <typename T>
typename std::enable_if<is_std_complex_v<std::decay_t<T>>>::type inline enigma::enigma_internal_deserialize_fn(
    T &value, std::byte *iter, std::size_t &len) {
  using InnerType = typename complex_inner_type<std::decay_t<T>>::type;

  InnerType Real = enigma::internal_deserialize<InnerType>(iter + len);
  len += enigma_internal_sizeof(Real);
  InnerType Imag = enigma::internal_deserialize<InnerType>(iter + len);
  len += enigma_internal_sizeof(Imag);
  value = std::complex<InnerType>(Real, Imag);
}
