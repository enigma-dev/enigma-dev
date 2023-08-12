/** Copyright (C) 2022 Dhruv Chawla
*** Copyright (C) 2023 Fares Atef
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

#include <complex>
#include <set>
#include "../var4.h"

template <typename T>
struct is_lua_table : std::false_type {};

template <typename U>
struct is_lua_table<lua_table<U>> : std::true_type {
  using inner_type = U;
};

template <typename T>
constexpr static inline bool is_lua_table_v = is_lua_table<T>::value;

template <typename T>
struct is_std_vector : std::false_type {};

template <typename T, typename Alloc>
struct is_std_vector<std::vector<T, Alloc>> : std::true_type {};

template <typename T>
constexpr static inline bool is_std_vector_v = is_std_vector<T>::value;

template <typename T>
struct is_std_set : std::false_type {};

template <typename Key, typename Compare, typename Allocator>
struct is_std_set<std::set<Key, Compare, Allocator>> : std::true_type {};

template <typename T>
constexpr bool is_std_set_v = is_std_set<T>::value;

template <typename T>
struct is_std_map : std::false_type {};

template <typename Key, typename T, typename Compare, typename Allocator>
struct is_std_map<std::map<Key, T, Compare, Allocator>> : std::true_type {};

template <typename T>
constexpr bool is_std_map_v = is_std_map<T>::value;

template <typename T>
struct is_std_complex : std::false_type {};

template <typename T>
struct is_std_complex<std::complex<T>> : std::true_type {};

template <typename T>
constexpr bool is_std_complex_v = is_std_complex<T>::value;

template <typename T>
struct is_std_tuple : std::false_type {};

template <typename... Ts>
struct is_std_tuple<std::tuple<Ts...>> : std::true_type {};

template <typename T>
constexpr bool is_std_tuple_v = is_std_tuple<T>::value;

template <typename T>
struct complex_inner_type;

template <typename T>
struct complex_inner_type<std::complex<T>> {
  using type = T;
};

template <typename T>
using complex_inner_type_t = typename complex_inner_type<T>::type;

template <typename T, typename U>
inline void insert_back(std::vector<T> &container, const U &val) {
  container.push_back(std::move(val));
}

template <typename T, typename U>
inline void insert_back(std::set<T> &container, const U &val) {
  container.insert(std::move(val));
}

template <typename... Types>
struct TupleTypeExtractor;

template <typename First, typename Second, typename Third>
struct TupleTypeExtractor<std::tuple<First, Second, Third>> {
  using FirstType = First;
  using SecondType = Second;
  using ThirdType = Third;
};

template <typename T, std::size_t N>
struct has_nested_form : std::false_type {
  using inner_type = void;
};

template <typename T>
struct has_nested_form<T, 0> : std::true_type {
  using inner_type = T;
};

template <template <typename> typename T, typename U, std::size_t N>
struct has_nested_form<T<U>, N> : has_nested_form<U, N - 1> {
  using inner_type = U;
};

template <typename T, std::size_t N>
constexpr static inline bool has_nested_form_v = has_nested_form<T, N>::value;
