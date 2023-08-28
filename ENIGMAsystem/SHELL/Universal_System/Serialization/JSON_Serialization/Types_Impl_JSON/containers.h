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

    auto split = [](const std::string& s, char delimiter) {
      std::vector<std::string> parts;
      size_t start = 0;
      size_t end = 0;
      bool within_quotes = false;
      bool within_array = false;
      size_t num_open_braces = 0;
      size_t len = s.length();

      while (end < len) {
        if (s[end] == '"' && (end == 0 || s[end - 1] != '\\')) {
          within_quotes = !within_quotes;
        } else if (s[end] == '[' && !within_quotes) {
          within_array = true;
        } else if (s[end] == ']' && !within_quotes) {
          within_array = false;
        } else if (s[end] == '{' && !within_quotes) {
          num_open_braces++;
        } else if (s[end] == '}' && !within_quotes) {
          num_open_braces--;
        } else if (s[end] == delimiter && !within_quotes && !within_array && num_open_braces == 0) {
          if (end != start) {
            parts.push_back(s.substr(start, end - start));
          }
          start = end + 1;
        }
        end++;
      }
      if (start != len) parts.push_back(s.substr(start));
      return parts;
    };

    std::vector<std::string> parts = split(jsonCopy, ',');
    for (auto it = parts.begin(); it != parts.end(); ++it)
      insert_back(result, internal_deserialize_fn<typename T::value_type>(*it));
  }

  return result;
}

}  // namespace JSON_serialization
}  // namespace enigma

#endif
