/** Copyright (C) 2022 Dhruv Chawla
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

#ifndef ENIGMA_SERIALIZE_VARIANT_JSON_H
#define ENIGMA_SERIALIZE_VARIANT_JSON_H

#include "../../type_traits.h"

namespace enigma {
namespace JSON_serialization {

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>> && !std::is_same_v<var, std::decay_t<T>>,
                        std::string>::type inline internal_serialize_into_fn(const T& value) {
  std::string json = "{";

  if (value.type == variant::ty_real)
    json += "\"type\":\"real\",\"value\":" + internal_serialize_into_fn(value.rval.d);
  else if (value.type == variant::ty_string)
    json += "\"type\":\"string\",\"value\":" + internal_serialize_into_fn(value.sval());

  json += "}";

  return json;
}

}  // namespace JSON_serialization
}  // namespace enigma

#endif
