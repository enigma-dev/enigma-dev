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

#ifndef ENIGMA_SERIALIZE_FWD_DECL_H
#define ENIGMA_SERIALIZE_FWD_DECL_H

#include "../var4.h"

template <typename T>
inline std::size_t byte_size(const T &value) = delete;

// inline std::size_t byte_size(const variant &value);

// inline std::size_t byte_size(const var &value);

// template <typename T>
// inline std::size_t byte_size(const lua_table<T> &value);

namespace enigma {
template <typename T>
inline void internal_serialize_into(std::byte *iter, T &&value);

template <typename T>
inline auto internal_serialize(T &&value);

template <typename T>
inline T internal_deserialize(std::byte *iter);

template <typename T>
inline void enigma_serialize(const T &value, std::size_t &len, std::vector<std::byte> &bytes);

template <typename T>
inline void enigma_deserialize(T &value, std::byte *iter, std::size_t &len);

inline void enigma_internal_deserialize_variant(variant &value, std::byte *iter, std::size_t &len);

// template <typename T>
// inline void enigma_internal_deserialize_fn(T &value, std::byte *iter, std::size_t &len) = delete;

// template<typename T>
// inline void enigma_internal_deserialize_fn(T &value, std::byte *iter, std::size_t &len) ;

// inline void enigma_internal_deserialize_fn(var &value, std::byte *iter, std::size_t &len);

// template <typename T>
// inline void enigma_internal_deserialize_fn(lua_table<T> &value, std::byte *iter, std::size_t &len);

// inline void enigma_internal_deserialize_fn(variant &value, std::byte *iter, std::size_t &len);

// inline void enigma_internal_deserialize_fn(std::string &value, std::byte *iter, std::size_t &len);

}  // namespace enigma

#endif