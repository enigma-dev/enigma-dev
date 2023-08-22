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
  using ValueType = typename std::decay_t<T>::mapped_type;

  for (auto it = value.begin(); it != value.end(); ++it) {
    if (is_numeric_v<KeyType>)
      json += "\"" + internal_serialize_into_fn(it->first) + "\"";
    else
      json += internal_serialize_into_fn(it->first);
    json += ":";
    if (is_numeric_v<ValueType>)
      json += "\"" + internal_serialize_into_fn(it->second) + "\"";
    else
      json += internal_serialize_into_fn(it->second);
    if (std::next(it) != value.end()) {
      json += ",";
    }
  }

  json += "}";

  return json;
}

}  // namespace JSON_serialization
}  // namespace enigma

#endif
