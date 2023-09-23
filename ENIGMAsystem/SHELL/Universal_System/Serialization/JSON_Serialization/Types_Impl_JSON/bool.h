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
  @file bool.h
  @brief This file contains the implementation of JSON (de)serialization functions for bool.
*/

#ifndef ENIGMA_SERIALIZE_BOOL_JSON_H
#define ENIGMA_SERIALIZE_BOOL_JSON_H

#include "../../type_traits.h"

namespace enigma {
namespace JSON_serialization {

template <typename T>
inline is_t<T, bool, std::string> internal_serialize_into_fn(const T& value) {
  return value ? "\"true\"" : "\"false\"";
}

template <typename T>
inline is_t<T, bool, T> internal_deserialize_fn(const std::string& json) {
  return (json == "\"true\"") ? true : false;
}

}  // namespace JSON_serialization
}  // namespace enigma

#endif
