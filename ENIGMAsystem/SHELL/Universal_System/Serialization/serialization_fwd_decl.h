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
#include "type_traits.h"

namespace enigma {

template <typename T>
typename std::enable_if_t<always_false<T>> inline byte_size(const T &value) = delete;

template <typename Base, typename T>
inline void internal_serialize_primitive_into(std::byte *iter, const T &value);

template <typename Base, typename T>
inline std::array<std::byte, sizeof(T)> serialize_primitive(T value);

template <typename Base, typename T>
inline T internal_deserialize_primitive(std::byte *iter);

template <typename T>
inline void enigma_internal_serialize_lua_table(std::byte *iter, const lua_table<T> &table);

template <typename T>
inline lua_table<T> enigma_internal_deserialize_lua_table(std::byte *iter);

template <typename T>
inline std::size_t enigma_internal_sizeof(T &&value);

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

template <typename T>
typename std::enable_if_t<always_false<T>> inline internal_serialize_into_fn(std::byte *iter, T &&value) = delete;

template <typename T>
typename std::enable_if_t<always_false<T>, std::vector<std::byte>> inline internal_serialize_fn(const T &value) =
    delete;

template <typename T>
typename std::enable_if_t<always_false<T>, T> inline internal_deserialize_fn(std::byte *iter) = delete;

template <typename T>
typename std::enable_if_t<always_false<T>> inline internal_resize_buffer_for_fn(std::vector<std::byte> &buffer,
                                                                                T &&value) = delete;

template <typename T>
typename std::enable_if_t<always_false<T>> inline enigma_internal_deserialize_fn(T &value, std::byte *iter,
                                                                                 std::size_t &len) = delete;

}  // namespace enigma
#endif
