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

#ifndef ENIGMA_SERIALIZE_TUPLE_BYTES_H
#define ENIGMA_SERIALIZE_TUPLE_BYTES_H

#include "../../serialization_fwd_decl.h"

namespace enigma {
namespace bytes_serialization {

template <typename T>
matches_t<T, std::size_t, is_std_tuple> inline byte_size(const T &value) {
  std::size_t totalSize = sizeof(std::size_t);

  LoopTuple(
      value, [&totalSize](const auto &element, auto &extraParam) { totalSize += enigma_internal_sizeof(element); },
      totalSize);

  return totalSize;
}

template <typename T>
matches_t<T, void, is_std_tuple> inline internal_serialize_into_fn(std::byte *iter, T &&value) {
  std::size_t tupleSize = std::tuple_size<std::decay_t<T>>::value;

  auto serialize_process_element = [&iter](std::byte *&elementIter, const auto &element) {
    internal_serialize_into(elementIter, element);
    elementIter += enigma_internal_sizeof(element);
  };

  internal_serialize_primitive_into<std::size_t>(iter, tupleSize);
  iter += sizeof(std::size_t);

  auto LoopTuple = [&iter, &serialize_process_element](const auto &tuple, auto &extraParam) {
    std::apply([&serialize_process_element,
                &extraParam](const auto &...elements) { (serialize_process_element(extraParam, elements), ...); },
               tuple);
  };

  LoopTuple(value, iter);
}

template <typename T>
matches_t<T, std::vector<std::byte>, is_std_tuple> inline internal_serialize_fn(T &&value) {
  std::vector<std::byte> result;
  result.resize(enigma_internal_sizeof(value));

  auto dataPtr = result.data();

  auto LoopTuple = [&dataPtr](const auto &tuple, auto &extraParam) {
    auto serialize_process_element = [&extraParam](const auto &element, auto &innerParam) {
      internal_serialize_into(innerParam, element);
      innerParam += enigma_internal_sizeof(element);
    };

    std::apply([&serialize_process_element,
                &extraParam](const auto &...elements) { (serialize_process_element(elements, extraParam), ...); },
               tuple);
  };

  LoopTuple(value, dataPtr);
  return result;
}

template <typename T>
matches_t<T, T, is_std_tuple> inline internal_deserialize_fn(std::byte *iter) {
  std::size_t tupleSize = internal_deserialize<std::size_t>(iter);
  std::size_t offset = sizeof(std::size_t);

  using ResultTuple = typename TupleTypeExtractor<T>::ResultType;
  ResultTuple result;

  auto internal_deserialize_loop = [](std::byte *iter, ResultTuple &result) {
    auto apply_tuple_element = [&iter](auto &element) {
      using ElementType = std::decay_t<decltype(element)>;
      element = internal_deserialize<ElementType>(iter);
    };

    std::apply([&](auto &...elements) { (apply_tuple_element(elements), ...); }, result);
  };

  internal_deserialize_loop(iter + offset, result);
  return result;
}

template <typename T>
matches_t<T, void, is_std_tuple> inline internal_resize_buffer_for_fn(std::vector<std::byte> &buffer, T &&value) {
  buffer.resize(buffer.size() + enigma_internal_sizeof(value));
}

template <typename T>
matches_t<T, void, is_std_tuple> inline enigma_internal_deserialize_fn(T &value, std::byte *iter, std::size_t &len) {
  std::size_t tupleSize = internal_deserialize<std::size_t>(iter + len);
  len += sizeof(std::size_t);
  using ResultTuple = typename TupleTypeExtractor<T>::ResultType;
  ResultTuple result;

  auto internal_deserialize_loop = [&len](std::byte *iter, ResultTuple &result) {
    auto apply_tuple_element = [&iter, &len](auto &element) {
      using ElementType = std::decay_t<decltype(element)>;
      element = internal_deserialize<ElementType>(iter + len);
      len += enigma_internal_sizeof(element);
    };

    std::apply([&](auto &...elements) { (apply_tuple_element(elements), ...); }, result);
  };

  internal_deserialize_loop(iter, result);
  value = result;
}

}  // namespace bytes_serialization
}  // namespace enigma

#endif
