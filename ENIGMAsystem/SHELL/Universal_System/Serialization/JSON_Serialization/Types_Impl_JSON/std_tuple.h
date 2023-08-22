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

}  // namespace JSON_serialization
}  // namespace enigma

#endif
