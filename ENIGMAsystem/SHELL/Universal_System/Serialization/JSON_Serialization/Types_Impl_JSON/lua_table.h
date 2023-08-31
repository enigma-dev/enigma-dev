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
matches_t<T, std::string, is_lua_table> inline internal_serialize_into_fn(const T &table) {
  std::string json = "{";
  using inner_type = typename lua_inner_type<std::decay_t<T>>::type;

  json += "\"mx_size_part\":" + enigma::JSON_serialization::enigma_serialize(table.mx_size_part()) + ",\"dense_part\":[";

  for (int i = 0; i < table.dense_part().size(); i++) {
    if constexpr (is_lua_table_v<inner_type>) {
      lua_table<variant> table1 = table.dense_part()[i];
      json += enigma::JSON_serialization::enigma_serialize(table1);
    } else
      json += enigma::JSON_serialization::enigma_serialize(table.dense_part()[i]);
    if (i != table.dense_part().size() - 1) json += ",";
  }

  json += "],\"sparse_part\":{";
  for (auto &[key, value] : table.sparse_part()) {
    using key_type = std::decay_t<decltype(key)>;
    if (is_numeric_v<key_type>)
      json += "\"" + enigma::JSON_serialization::enigma_serialize(key) + "\"";
    else
      json += enigma::JSON_serialization::enigma_serialize(key);
    json += ":";
    if constexpr (is_lua_table_v<inner_type>) {
      lua_table<variant> table1 = value;
      json += enigma::JSON_serialization::enigma_serialize(table1);
    } else
      json += enigma::JSON_serialization::enigma_serialize(value);

    if (&value != &table.sparse_part().rbegin()->second) json += ",";
  }

  json += "}}";

  return json;
}

template <typename T>
matches_t<T, T, is_lua_table> inline internal_deserialize_fn(const std::string &json) {
  using inner_type = typename lua_inner_type<std::decay_t<T>>::type;
  lua_table<inner_type> table;
  auto &mx_size = const_cast<std::size_t &>(table.mx_size_part());
  auto &dense = const_cast<typename lua_table<inner_type>::dense_type &>(table.dense_part());
  auto &sparse = const_cast<typename lua_table<inner_type>::sparse_type &>(table.sparse_part());

  std::string mx_size_part = json.substr(16, json.find(',') - 16);
  std::string dense_part =
      json.substr(json.find("dense_part") + 12, json.find("sparse_part") - json.find("dense_part") - 14);
  std::string sparse_part = json.substr(json.find("sparse_part") + 13, json.length() - json.find("sparse_part") - 14);

  mx_size = enigma::JSON_serialization::enigma_deserialize<std::size_t>(mx_size_part);

  std::string dense_part1 = dense_part.substr(1, dense_part.length() - 2);
  std::string sparse_part1 = sparse_part.substr(1, sparse_part.length() - 2);

  std::vector<std::string> dense_part2 = json_split(dense_part1, ',');
  dense.resize(dense_part2.size());
  for (std::size_t i = 0; i < dense_part2.size(); i++) dense[i] = enigma::JSON_serialization::enigma_deserialize<inner_type>(dense_part2[i]);

  std::vector<std::string> sparse_part2 = json_split(sparse_part1, ',');
  for (auto &elem : sparse_part2) {
    std::size_t key = enigma::JSON_serialization::enigma_deserialize<std::size_t>(elem.substr(1, elem.find(":") - 1));
    sparse[key] = enigma::JSON_serialization::enigma_deserialize<inner_type>(elem.substr(elem.find(":") + 1, elem.length() - elem.find(":") - 1));
  }

  return table;
}

}  // namespace JSON_serialization
}  // namespace enigma

#endif
