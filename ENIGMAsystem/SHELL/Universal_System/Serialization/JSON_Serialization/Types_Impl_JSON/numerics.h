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

#ifndef ENIGMA_SERIALIZE_NUMERICS_JSON_H
#define ENIGMA_SERIALIZE_NUMERICS_JSON_H

#include "../../type_traits.h"

namespace enigma {
namespace JSON_serialization {

template <typename T>
enables_if_numeric_t<T, std::string> inline internal_serialize_into_fn(const T& value) {
  std::string json = std::to_string(value);

  if constexpr (std::is_floating_point_v<std::decay_t<T>>) {
    size_t decimalPos = json.find('.');
    if (decimalPos != std::string::npos) {
      if (decimalPos == json.length() - 1) {
        json.erase(decimalPos);
      } else {
        size_t lastNonZero = json.find_last_not_of('0');
        if (lastNonZero != std::string::npos && lastNonZero > decimalPos) {
          json.resize(lastNonZero + 1);
        } else {
          json.resize(decimalPos);
        }
      }
    }
  }

  if constexpr (std::is_same_v<T, char>) return "\"" + std::string(1, value) + "\"";

  return json;
}

}  // namespace JSON_serialization
}  // namespace enigma

#endif
