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
  @file pointer.h
  @brief This file contains the implementation of JSON (de)serialization functions for pointer.
*/

#ifndef ENIGMA_SERIALIZE_POINTER_JSON_H
#define ENIGMA_SERIALIZE_POINTER_JSON_H

namespace enigma {
namespace JSON_serialization {

template <typename T>
inline matches_t<T, std::string, std::is_pointer> internal_serialize_into_fn(const T& value) {
  return "0";
}

template <typename T>
inline matches_t<T, T, std::is_pointer> internal_deserialize_fn(const std::string& json) {
  return nullptr;
}

}  // namespace JSON_serialization
}  // namespace enigma

#endif
