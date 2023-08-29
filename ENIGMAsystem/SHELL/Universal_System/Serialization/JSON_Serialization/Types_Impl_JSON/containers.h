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

#ifndef ENIGMA_SERIALIZE_CONTAINERS_JSON_H
#define ENIGMA_SERIALIZE_CONTAINERS_JSON_H

#include "../../type_traits.h"
#include "../JSON_parsing_utilities.h"

namespace enigma {
namespace JSON_serialization {

template <typename T>
matches_t<T, std::string, is_std_vector, is_std_set> inline internal_serialize_into_fn(const T& value) {
  std::string json = "[";

  for (auto it = value.begin(); it != value.end(); ++it) {
    json += internal_serialize_into_fn(*it);

    if (std::next(it) != value.end()) {
      json += ",";
    }
  }

  json += "]";

  return json;
}

template <typename T>
matches_t<T, T, is_std_vector, is_std_set, is_std_queue> inline internal_deserialize_fn(const std::string& json) {
  T result;

  if (json.length() > 2) {  // Empty array
    std::string jsonCopy = json.substr(1, json.length() - 2);
    std::vector<std::string> parts = json_split(jsonCopy, ',');
    for (auto it = parts.begin(); it != parts.end(); ++it)
      insert_back(result, internal_deserialize_fn<typename T::value_type>(*it));
  }

  return result;
}

}  // namespace JSON_serialization
}  // namespace enigma

#endif
