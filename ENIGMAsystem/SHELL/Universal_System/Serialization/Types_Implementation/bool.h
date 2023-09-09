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

#include "../serialization_fwd_decl.h"

template <typename T>
is_t<T, bool> inline enigma::internal_serialize_into_fn(std::byte *iter, T &&value) {
  *iter = static_cast<std::byte>(value);
}

template <typename T>
is_t<T, bool, std::vector<std::byte>> inline enigma::internal_serialize_fn(const bool &value) {
  return std::vector<std::byte>{static_cast<std::byte>(value)};
}

template <typename T>
is_t<T, bool, T> inline enigma::internal_deserialize_fn(std::byte *iter) {
  return static_cast<bool>(*iter);
}
