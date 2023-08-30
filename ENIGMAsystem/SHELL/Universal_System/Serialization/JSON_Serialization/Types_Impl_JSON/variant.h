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
  @file variant.h
  @brief This file contains the implementation of JSON (de)serialization functions for variant based
  types (variant, enigma::depthv, enigma::image_singlev, enigma::speedv, enigma::directionv, etc.).  
*/

#ifndef ENIGMA_SERIALIZE_VARIANT_JSON_H
#define ENIGMA_SERIALIZE_VARIANT_JSON_H

#include "../../type_traits.h"

namespace enigma {
namespace JSON_serialization {

template <typename T>
enables_if_base_of_variant_t<T, std::string> inline internal_serialize_into_fn(const T& value) {
  std::string json = "{";

  if (value.type == variant::ty_real)
    json += "\"type\":\"real\",\"value\":" + internal_serialize_into_fn(value.rval.d);
  else if (value.type == variant::ty_string)
    json += "\"type\":\"string\",\"value\":" + internal_serialize_into_fn(value.sval());

  json += "}";

  return json;
}

template <typename T>
enables_if_base_of_variant_t<T, T> inline internal_deserialize_fn(const std::string& json) {
  std::string type = json.substr(9, json.find(',') - 10);
  std::string value = json.substr(json.find("value") + 7, json.length() - json.find("value") - 8);

  if (type == "real") return T(internal_deserialize_fn<double>(value));
  return T(internal_deserialize_fn<std::string>(value));
}

}  // namespace JSON_serialization
}  // namespace enigma

#endif
