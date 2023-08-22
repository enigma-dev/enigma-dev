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

#ifndef ENIGMA_SERIALIZE_STRING_JSON_H
#define ENIGMA_SERIALIZE_STRING_JSON_H

#include "../../type_traits.h"

namespace enigma {
namespace JSONserialization {

template <typename T>
is_t<T, std::string, std::string> inline JSON_serialize_into(const T& value) {
  return "\"" + value + "\"";
}
}  // namespace JSONserialization
}  // namespace enigma

#endif
