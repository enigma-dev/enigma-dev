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

#ifndef ENIGMA_SERIALIZE_LUA_TABLE_JSON_H
#define ENIGMA_SERIALIZE_LUA_TABLE_JSON_H

#include "../../type_traits.h"
#include "variant.h"

namespace enigma {
namespace JSON_serialization {

template <typename T>
inline std::string internal_serialize_into_fn(const lua_table<T> &table) {
  std::string json = "{";

  json += "\"mx_size_part\":" + internal_serialize_into_fn(table.mx_size_part()) + ",\"dense_part\":[";

  for (int i = 0; i < table.dense_part().size(); i++) {
    if constexpr (is_lua_table_v<std::decay_t<T>>) {
      lua_table<variant> table1 = table.dense_part()[i];
      json += internal_serialize_into_fn(table1);
    } else
      json += internal_serialize_into_fn(table.dense_part()[i]);
    if (i != table.dense_part().size() - 1) json += ",";
  }

  json += "],\"sparse_part\":{";
  for (auto &[key, value] : table.sparse_part()) {
    using key_type = std::decay_t<decltype(key)>;
    if (is_numeric_v<key_type>)
      json += "\"" + internal_serialize_into_fn(key) + "\"";
    else
      json += internal_serialize_into_fn(key);
    json += ":";
    if constexpr (is_lua_table_v<std::decay_t<T>>) {
      lua_table<variant> table1 = value;
      json += internal_serialize_into_fn(table1);
    } else
      json += internal_serialize_into_fn(value);

    if (&value != &table.sparse_part().rbegin()->second) json += ",";
  }

  json += "}}";

  return json;
}

template <typename T>
matches_t<T, T, is_lua_table> inline internal_deserialize_fn(const std::string &json) {
  using Type = typename lua_inner_type<std::decay_t<T>>::type;
  lua_table<Type> table;
  auto &mx_size = const_cast<std::size_t &>(table.mx_size_part());
  auto &dense = const_cast<typename lua_table<Type>::dense_type &>(table.dense_part());
  auto &sparse = const_cast<typename lua_table<Type>::sparse_type &>(table.sparse_part());

  std::string mx_size_part = json.substr(16, json.find(',') - 16);
  std::string dense_part =
      json.substr(json.find("dense_part") + 12, json.find("sparse_part") - json.find("dense_part") - 14);
  std::string sparse_part = json.substr(json.find("sparse_part") + 13, json.length() - json.find("sparse_part") - 14);

  mx_size = internal_deserialize_fn<std::size_t>(mx_size_part);

  std::string dense_part1 = dense_part.substr(1, dense_part.length() - 2);
  std::string sparse_part1 = sparse_part.substr(1, sparse_part.length() - 2);

  auto split = [](const std::string &s, char delimiter) {
    std::vector<std::string> parts;
    size_t start = 0;
    size_t end = s.find(delimiter);
    while (end != std::string::npos) {
      if (end != start) {
        parts.push_back(s.substr(start, end - start));
      }
      start = end + 1;
      end = s.find(delimiter, start);
    }
    if (start != s.length()) {
      parts.push_back(s.substr(start));
    }
    return parts;
  };

  std::vector<std::string> dense_part2 = split(dense_part1, ',');
  dense.resize(dense_part2.size());
  for (std::size_t i = 0; i < dense_part2.size(); i++) dense[i] = internal_deserialize_fn<Type>(dense_part2[i]);

  std::vector<std::string> sparse_part2 = split(sparse_part1, ',');
  for (auto &elem : sparse_part2) {
    std::size_t key = internal_deserialize_fn<std::size_t>(elem.substr(1, elem.find(":") - 1));
    sparse[key] = internal_deserialize_fn<Type>(elem.substr(elem.find(":") + 1, elem.length() - elem.find(":") - 1));
  }

  return table;
}

}  // namespace JSON_serialization
}  // namespace enigma

#endif
