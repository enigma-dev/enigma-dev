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

#ifndef ENIGMA_SERIALIZE_QUEUE_STACK_JSON_H
#define ENIGMA_SERIALIZE_QUEUE_STACK_JSON_H

#include "../../type_traits.h"

namespace enigma {
namespace JSON_serialization {

template <typename T>
matches_t<T, std::string, is_std_queue, is_std_stack> inline internal_serialize_into_fn(const T& value) {
  std::string json = "[";

  std::decay_t<T> tempContainer = value;
  while (!tempContainer.empty()) {
    json += internal_serialize_into_fn(get_top(tempContainer));
    if (tempContainer.size() > 1) json += ",";
    tempContainer.pop();
  }

  json += "]";

  return json;
}

template <typename T>
matches_t<T, T, is_std_stack> inline internal_deserialize_fn(const std::string& json) {
  std::string tempJson = json.substr(1, json.length() - 2);

  T tempContainer;

  size_t commaPos = tempJson.find(',');
  while (commaPos != std::string::npos) {
    tempContainer.push(internal_deserialize_fn<typename T::value_type>(tempJson.substr(0, commaPos)));
    tempJson.erase(0, commaPos + 1);
    commaPos = tempJson.find(',');
  }

  tempContainer.push(internal_deserialize_fn<typename T::value_type>(tempJson));

  T result;
  while (!tempContainer.empty()) {
    result.push(tempContainer.top());
    tempContainer.pop();
  }

  return result;
}

}  // namespace JSON_serialization
}  // namespace enigma

#endif
