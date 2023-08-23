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

#ifndef ENIGMA_SERIALIZE_TUPLE_JSON_H
#define ENIGMA_SERIALIZE_TUPLE_JSON_H

#include "../../type_traits.h"

namespace enigma {
namespace JSON_serialization {

template <typename T>
matches_t<T, std::string, is_std_tuple> inline internal_serialize_into_fn(const T &value) {
  std::string json = "[";

  std::size_t tupleSize = std::tuple_size<std::decay_t<T>>::value;
  std::size_t i = 0;
  auto serialize_process_element = [&json, &i, tupleSize](const auto &element) {
    json += internal_serialize_into_fn(element);
    if (i++ < tupleSize - 1) json += ",";
  };

  auto LoopTuple = [&serialize_process_element](const auto &tuple) {
    std::apply([&serialize_process_element](const auto &...elements) { (serialize_process_element(elements), ...); },
               tuple);
  };

  LoopTuple(value);
  json += "]";

  return json;
}

template <typename Tuple, std::size_t Index>
void tuple_deserialize_helper(std::string tupleStr, Tuple &tuple) {
  if constexpr (Index < std::tuple_size<Tuple>::value) {
    std::string elementStr = tupleStr.substr(0, tupleStr.find(','));
    tupleStr = tupleStr.substr(tupleStr.find(',') + 1);

    using TypeAtIndex = typename std::tuple_element<Index, Tuple>::type;
    std::get<Index>(tuple) = internal_deserialize_fn<TypeAtIndex>(elementStr);

    tuple_deserialize_helper<Tuple, Index + 1>(tupleStr, tuple);
  }
}

template <typename T>
matches_t<T, T, is_std_tuple> inline internal_deserialize_fn(const std::string &json) {
  std::string tupleStr = json.substr(1, json.length() - 2);

  typename std::decay_t<T> resultTuple;
  tuple_deserialize_helper<T, 0>(tupleStr, resultTuple);

  return resultTuple;
}

}  // namespace JSON_serialization
}  // namespace enigma

#endif
