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
is_t<T, variant, std::size_t> inline byte_size(const T &value);

template <typename T>
is_t<T, var, std::size_t> inline byte_size(const T &value);

template <typename T>
matches_t<T, std::size_t, is_lua_table> inline byte_size(const T &value);

template <typename T>
inline void internal_serialize_numeric_into(std::byte *iter, T value);

template <typename T>
inline std::array<std::byte, sizeof(T)> internal_serialize_numeric(T value);

template <typename T>
inline T internal_deserialize_numeric(std::byte *iter);

template <typename Base, typename T>
inline void internal_serialize_any_into(std::byte *iter, T value);

template <typename Base, typename T>
inline std::array<std::byte, sizeof(T)> serialize_any(T value);

template <typename Base, typename T>
inline T internal_deserialize_any(std::byte *iter);

template <typename T>
inline T internal_deserialize_numeric(std::byte *iter);

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
is_t<T, var> inline internal_serialize_into_fn(std::byte *iter, T &&value);

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>> &&
                        !std::is_same_v<var, std::decay_t<T>>>::type inline internal_serialize_into_fn(std::byte *iter,
                                                                                                       T &&value);

template <typename T>
typename std::enable_if<(std::is_integral_v<std::decay_t<T>> ||
                         std::is_floating_point_v<std::decay_t<T>>)&&!std::is_same_v<std::decay_t<T>, bool>>::
    type inline internal_serialize_into_fn(std::byte *iter, T &&value);

template <typename T>
inline auto internal_serialize_fn(T *&&value);

inline auto internal_serialize_fn(const var &value);

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>>,
                        std::vector<std::byte>>::type inline internal_serialize_fn(T &&value);

template <typename T>
typename std::enable_if<std::is_integral_v<std::decay_t<T>> || std::is_floating_point_v<std::decay_t<T>>,
                        std::array<std::byte, sizeof(T)>>::type inline internal_serialize_fn(T &&value);

template <typename T>
is_t<T, var, T> inline internal_deserialize_fn(std::byte *iter);

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>> && !std::is_same_v<var, std::decay_t<T>>,
                        T>::type inline internal_deserialize_fn(std::byte *iter);

template <typename T>
typename std::enable_if<(std::is_integral_v<std::decay_t<T>> ||
                         std::is_floating_point_v<std::decay_t<T>>)&&!std::is_same_v<std::decay_t<T>, bool>,
                        T>::type inline internal_deserialize_fn(std::byte *iter);

template <typename T>
is_t<T, var> inline internal_resize_buffer_for_fn(std::vector<std::byte> &buffer, T &&value);

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>> && !std::is_same_v<var, std::decay_t<T>>>::
    type inline internal_resize_buffer_for_fn(std::vector<std::byte> &buffer, T &&value);

template <typename T>
is_t<T, var> inline enigma_internal_deserialize_fn(T &value, std::byte *iter, std::size_t &len);

template <typename T>
matches_t<T, void, is_lua_table> inline enigma_internal_deserialize_fn(T &value, std::byte *iter, std::size_t &len);

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>> && !std::is_same_v<var, std::decay_t<T>>>::
    type inline enigma_internal_deserialize_fn(T &value, std::byte *iter, std::size_t &len);

}  // namespace enigma
#endif
