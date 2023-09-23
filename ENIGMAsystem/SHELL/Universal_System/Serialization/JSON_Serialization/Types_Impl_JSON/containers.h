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

/**
  @file containers.h
  @brief This file contains the implementation of JSON serialization function for std::vector
  and std::set, and JSON deserialization function for std::vector, std::set and std::queue. 
*/

#ifndef ENIGMA_SERIALIZE_CONTAINERS_JSON_H
#define ENIGMA_SERIALIZE_CONTAINERS_JSON_H

#include "../../type_traits.h"
#include "../JSON_parsing_utilities.h"

namespace enigma {
namespace JSON_serialization {

template <typename T>
inline matches_t<T, std::string, is_std_vector, is_std_set> internal_serialize_into_fn(const T& value) {
  std::string json = "[";

  for (auto it = value.begin(); it != value.end(); ++it) {
    json += enigma::JSON_serialization::enigma_serialize(*it);

    if (std::next(it) != value.end()) {
      json += ",";
    }
  }

  json += "]";

  return json;
}

template <typename T>
inline matches_t<T, T, is_std_vector, is_std_set, is_std_queue> internal_deserialize_fn(const std::string& json) {
  T result;

  if (json.length() > 2) {  // Not empty
    std::string jsonCopy = json.substr(1, json.length() - 2);
    std::vector<std::string> parts = json_split(jsonCopy, ',');
    for (auto it = parts.begin(); it != parts.end(); ++it)
      insert_back(result, enigma::JSON_serialization::enigma_deserialize<typename T::value_type>(*it));
  }

  return result;
}

}  // namespace JSON_serialization
}  // namespace enigma

#endif
