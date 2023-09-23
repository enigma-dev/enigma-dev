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
  @file lua_table.h
  @brief This file contains the implementation of JSON (de)serialization functions for lua_table.
*/

#ifndef ENIGMA_SERIALIZE_LUA_TABLE_JSON_H
#define ENIGMA_SERIALIZE_LUA_TABLE_JSON_H

#include "../../type_traits.h"
#include "../JSON_parsing_utilities.h"
#include "variant.h"

namespace enigma {
namespace JSON_serialization {

template <typename T>
inline matches_t<T, std::string, is_lua_table> internal_serialize_into_fn(const T &table) {
  std::string json = "[";
  using inner_type = typename lua_inner_type<std::decay_t<T>>::type;

  for (std::size_t i = 0; i < table.dense_part().size(); i++) {
    json += "{\"" + std::to_string(i) + "\":";
    if constexpr (is_lua_table_v<inner_type>) {
      lua_table<variant> table1 = table.dense_part()[i];
      json += enigma::JSON_serialization::enigma_serialize(table1);
    } else
      json += enigma::JSON_serialization::enigma_serialize(table.dense_part()[i]);
    json += "}";
    if (i != table.dense_part().size() - 1) json += ",";
  }

  if (table.sparse_part().size()) {
    if (table.dense_part().size()) json += ",";
  } else {
    json += "]";
    return json;
  }

  for (auto &[key, value] : table.sparse_part()) {
    json += "{\"" + std::to_string(key) + "\":";
    if constexpr (is_lua_table_v<inner_type>) {
      lua_table<variant> table1 = value;
      json += enigma::JSON_serialization::enigma_serialize(table1);
    } else
      json += enigma::JSON_serialization::enigma_serialize(value);
    json += "}";
    if (&value != &table.sparse_part().rbegin()->second) json += ",";
  }

  json += "]";

  return json;
}

template <typename T>
inline matches_t<T, T, is_lua_table> internal_deserialize_fn(const std::string &json) {
  using inner_type = typename lua_inner_type<std::decay_t<T>>::type;
  lua_table<inner_type> table;

  std::string jsonCopy = json.substr(1, json.length() - 2);
  std::vector<std::string> parts = json_split(jsonCopy, ',');

  for (auto it = parts.begin(); it != parts.end(); ++it) {
    std::string keyStr = (*it).substr(1, (*it).find(':'));
    std::string valueStr = (*it).substr((*it).find(':') + 1, (*it).length() - keyStr.length() - 2);

    keyStr = keyStr.substr(1, keyStr.length() - 2);

    size_t key = enigma::JSON_serialization::enigma_deserialize<size_t>(keyStr);
    inner_type value = enigma::JSON_serialization::enigma_deserialize<inner_type>(valueStr);

    table[key] = value;
  }

  return table;
}

}  // namespace JSON_serialization
}  // namespace enigma

#endif
