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

namespace enigma {

template <typename T>
inline std::size_t byte_size(const T &value) = delete;

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

template <typename T>
inline auto internal_serialize_into_fn(std::byte *iter, T *value);

template <typename T>
typename std::enable_if<std::is_same_v<std::string, std::decay_t<T>>>::type inline internal_serialize_into_fn(
    std::byte *iter, T &&value);

template <typename T>
typename std::enable_if<std::is_same_v<bool, std::decay_t<T>>>::type inline internal_serialize_into_fn(std::byte *iter,
                                                                                                       T &&value);

template <typename T>
typename std::enable_if<std::is_same_v<var, std::decay_t<T>>>::type inline internal_serialize_into_fn(std::byte *iter,
                                                                                                      T &&value);

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>> &&
                        !std::is_same_v<var, std::decay_t<T>>>::type inline internal_serialize_into_fn(std::byte *iter,
                                                                                                       T &&value);

template <typename T>
typename std::enable_if<(std::is_integral_v<std::decay_t<T>> ||
                         std::is_floating_point_v<std::decay_t<T>>)&&!std::is_same_v<std::decay_t<T>, bool>>::
    type inline internal_serialize_into_fn(std::byte *iter, T &&value);

// needs a better place
template <typename T>
struct is_std_vector : std::false_type {};

template <typename T, typename Alloc>
struct is_std_vector<std::vector<T, Alloc>> : std::true_type {};

template <typename T>
constexpr static inline bool is_std_vector_v = is_std_vector<T>::value;

template <typename T>
struct is_std_map : std::false_type {};

template <typename Key, typename T, typename Compare, typename Allocator>
struct is_std_map<std::map<Key, T, Compare, Allocator>> : std::true_type {};

template <typename T>
constexpr bool is_std_map_v = is_std_map<T>::value;
//

template <typename T>
typename std::enable_if<is_std_vector_v<std::decay_t<T>>>::type inline internal_serialize_into_fn(std::byte *iter,
                                                                                                  T &&value);

template <typename T>
typename std::enable_if<is_std_map_v<std::decay_t<T>>>::type inline internal_serialize_into_fn(std::byte *iter,
                                                                                               T &&value);

template <typename T>
inline auto internal_serialize_fn(T *&&value);

inline auto internal_serialize_fn(std::string &&value);

inline auto internal_serialize_fn(bool &&value);

inline auto internal_serialize_fn(var &&value);

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>>,
                        std::vector<std::byte>>::type inline internal_serialize_fn(T &&value);

template <typename T>
typename std::enable_if<std::is_integral_v<std::decay_t<T>> || std::is_floating_point_v<std::decay_t<T>>,
                        std::array<std::byte, sizeof(T)>>::type inline internal_serialize_fn(T &&value);

template <typename T>
typename std::enable_if<is_std_vector_v<std::decay_t<T>>, std::vector<std::byte>>::type inline internal_serialize_fn(
    std::vector<T> &&value);

template <typename T>
typename std::enable_if<is_std_map_v<std::decay_t<T>>, std::vector<std::byte>>::type inline internal_serialize_fn(
    T &&value);

template <typename T>
typename std::enable_if<std::is_pointer_v<std::decay_t<T>>, T>::type inline internal_deserialize_fn(std::byte *iter);

template <typename T>
typename std::enable_if<std::is_same_v<std::string, std::decay_t<T>>, T>::type inline internal_deserialize_fn(
    std::byte *iter);

template <typename T>
typename std::enable_if<std::is_same_v<bool, std::decay_t<T>>, T>::type inline internal_deserialize_fn(std::byte *iter);

template <typename T>
typename std::enable_if<std::is_same_v<var, std::decay_t<T>>, T>::type inline internal_deserialize_fn(std::byte *iter);

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>>, T>::type inline internal_deserialize_fn(
    std::byte *iter);

template <typename T>
typename std::enable_if<(std::is_integral_v<std::decay_t<T>> ||
                         std::is_floating_point_v<std::decay_t<T>>)&&!std::is_same_v<std::decay_t<T>, bool>,
                        T>::type inline internal_deserialize_fn(std::byte *iter);

template <typename T>
typename std::enable_if<is_std_vector_v<std::decay_t<T>>, T>::type inline internal_deserialize_fn(std::byte *iter);

template <typename T>
typename std::enable_if<is_std_map_v<std::decay_t<T>>, T>::type inline internal_deserialize_fn(std::byte *iter);

template <typename T>
typename std::enable_if<std::is_same_v<var, std::decay_t<T>>>::type inline internal_resize_buffer_for_fn(
    std::vector<std::byte> &buffer, T &&value);

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>> && !std::is_same_v<var, std::decay_t<T>>>::
    type inline internal_resize_buffer_for_fn(std::vector<std::byte> &buffer, T &&value);

template <typename T>
typename std::enable_if<std::is_same_v<std::string, std::decay_t<T>>>::type inline internal_resize_buffer_for_fn(
    std::vector<std::byte> &buffer, T &&value);

template <typename T>
typename std::enable_if<is_std_vector_v<std::decay_t<T>>>::type inline internal_resize_buffer_for_fn(
    std::vector<std::byte> &buffer, T &&value);

template <typename T>
typename std::enable_if<is_std_map_v<std::decay_t<T>>>::type inline internal_resize_buffer_for_fn(
    std::vector<std::byte> &buffer, T &&value);

template <typename T>
typename std::enable_if<std::is_same_v<var, std::decay_t<T>>>::type inline enigma_internal_deserialize_fn(
    T &value, std::byte *iter, std::size_t &len);

// needs a better place
template <typename T>
struct is_lua_table : std::false_type {};

template <typename U>
struct is_lua_table<lua_table<U>> : std::true_type {
  using inner_type = U;
};

template <typename T>
constexpr static inline bool is_lua_table_v = is_lua_table<T>::value;
//

template <typename T>
typename std::enable_if<is_lua_table_v<std::decay_t<T>>>::type inline enigma_internal_deserialize_fn(T &value,
                                                                                                     std::byte *iter,
                                                                                                     std::size_t &len);

template <typename T>
typename std::enable_if<std::is_base_of_v<variant, std::decay_t<T>> && !std::is_same_v<var, std::decay_t<T>>>::
    type inline enigma_internal_deserialize_fn(T &value, std::byte *iter, std::size_t &len);

template <typename T>
typename std::enable_if<std::is_same_v<std::string, std::decay_t<T>>>::type inline enigma_internal_deserialize_fn(
    T &value, std::byte *iter, std::size_t &len);

template <typename T>
typename std::enable_if<is_std_vector_v<std::decay_t<T>>>::type inline enigma_internal_deserialize_fn(T &value,
                                                                                                      std::byte *iter,
                                                                                                      std::size_t &len);

template <typename T>
typename std::enable_if<is_std_map_v<std::decay_t<T>>>::type inline enigma_internal_deserialize_fn(T &value,
                                                                                                   std::byte *iter,
                                                                                                   std::size_t &len);

}  // namespace enigma
#endif