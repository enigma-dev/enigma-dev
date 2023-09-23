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
  @file std_string.h
  @brief This file contains the implementation of JSON (de)serialization functions for std::string.
*/

#ifndef ENIGMA_SERIALIZE_STRING_JSON_H
#define ENIGMA_SERIALIZE_STRING_JSON_H

#include "../../type_traits.h"

namespace enigma {
namespace JSON_serialization {

template <typename T>
inline is_t<T, std::string, std::string> internal_serialize_into_fn(const T& value) {
  std::string json = "\"";

  for (char c : value) {
    switch (c) {
      case '"':
        json += "\\\"";
        break;
      case '\\':
        json += "\\\\";
        break;
      case '\b':
        json += "\\b";
        break;
      case '\f':
        json += "\\f";
        break;
      case '\n':
        json += "\\n";
        break;
      case '\r':
        json += "\\r";
        break;
      case '\t':
        json += "\\t";
        break;
      default:
        json += c;
        break;
    }
  }

  json += "\"";
  return json;
}

template <typename T>
inline is_t<T, std::string, T> internal_deserialize_fn(const std::string& json) {
  std::string result;
  bool escaped = false;

  for (size_t i = 1; i < json.size() - 1; ++i) {
    if (escaped) {
      switch (json[i]) {
        case '"':
          result += '"';
          break;
        case '\\':
          result += '\\';
          break;
        case 'b':
          result += '\b';
          break;
        case 'f':
          result += '\f';
          break;
        case 'n':
          result += '\n';
          break;
        case 'r':
          result += '\r';
          break;
        case 't':
          result += '\t';
          break;
        default:
          result += json[i];
          break;
      }
      escaped = false;
    } else if (json[i] == '\\') {
      escaped = true;
    } else {
      result += json[i];
    }
  }

  return result;
}

}  // namespace JSON_serialization
}  // namespace enigma

#endif
