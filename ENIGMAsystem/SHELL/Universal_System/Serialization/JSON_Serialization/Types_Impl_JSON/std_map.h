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

#ifndef ENIGMA_SERIALIZE_MAP_JSON_H
#define ENIGMA_SERIALIZE_MAP_JSON_H

#include "../../type_traits.h"

namespace enigma {
namespace JSON_serialization {

template <typename T>
matches_t<T, std::string, is_std_map> inline internal_serialize_into_fn(const T& value) {
  std::string json = "{";

  using KeyType = typename std::decay_t<T>::key_type;

  for (auto it = value.begin(); it != value.end(); ++it) {
    if (is_numeric_v<KeyType>)
      json += "\"" + internal_serialize_into_fn(it->first) + "\"";
    else
      json += internal_serialize_into_fn(it->first);
    json += ":" + internal_serialize_into_fn(it->second);
    if (std::next(it) != value.end()) {
      json += ",";
    }
  }

  json += "}";

  return json;
}

template <typename T>
matches_t<T, T, is_std_map> inline internal_deserialize_fn(const std::string& json) {
  using KeyType = typename std::decay_t<T>::key_type;
  using ValueType = typename std::decay_t<T>::mapped_type;

  T result;

  std::string jsonCopy = json.substr(1, json.length() - 2);

  while (!jsonCopy.empty()) {
    std::string keyStr = jsonCopy.substr(0, jsonCopy.find(':'));
    std::string valueStr = jsonCopy.substr(jsonCopy.find(':') + 1, jsonCopy.find(',') - jsonCopy.find(':') - 1);

    if constexpr (is_numeric_v<KeyType>) keyStr = keyStr.substr(1, keyStr.length() - 2);

    KeyType key = internal_deserialize_fn<KeyType>(keyStr);
    ValueType value = internal_deserialize_fn<ValueType>(valueStr);

    result[key] = value;

    if (jsonCopy.find(',') != std::string::npos)
      jsonCopy = jsonCopy.substr(jsonCopy.find(',') + 1);
    else
      jsonCopy = "";
  }

  return result;
}

}  // namespace JSON_serialization
}  // namespace enigma

#endif
