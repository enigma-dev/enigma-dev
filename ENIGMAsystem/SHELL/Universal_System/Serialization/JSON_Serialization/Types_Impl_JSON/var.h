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

#ifndef ENIGMA_SERIALIZE_VAR_JSON_H
#define ENIGMA_SERIALIZE_VAR_JSON_H

#include "../../../var4.h"
#include "../../type_traits.h"
#include "lua_table.h"
#include "variant.h"
namespace enigma {
namespace JSON_serialization {

template <typename T>
is_t<T, var, std::string> internal_serialize_into_fn(const T &value) {
  std::string json = "{\"variant\":";

  json += internal_serialize_into_fn<variant>(value);
  json += ",\"array1d\":";
  json += internal_serialize_into_fn(value.array1d);
  json += ",\"array2d\":";
  json += internal_serialize_into_fn(value.array2d);

  json += "}";
  return json;
}

template <typename T>
is_t<T, var, T> inline internal_deserialize_fn(const std::string &json) {}

}  // namespace JSON_serialization
}  // namespace enigma

#endif
