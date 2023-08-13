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
#include <queue>
#include <set>
#include <stack>
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
struct is_std_queue : std::false_type {};

template <typename T, typename Container>
struct is_std_queue<std::queue<T, Container>> : std::true_type {};

template <typename T>
constexpr bool is_std_queue_v = is_std_queue<T>::value;

template <typename T>
struct is_std_stack : std::false_type {};

template <typename T, typename Container>
struct is_std_stack<std::stack<T, Container>> : std::true_type {};

template <typename T>
constexpr bool is_std_stack_v = is_std_stack<T>::value;

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
struct is_std_pair : std::false_type {};

template <typename T, typename U>
struct is_std_pair<std::pair<T, U>> : std::true_type {};

template <typename T>
constexpr bool is_std_pair_v = is_std_pair<T>::value;

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

template <typename... Types>
struct TupleTypeExtractor;

template <typename First, typename Second, typename Third>
struct TupleTypeExtractor<std::tuple<First, Second, Third>> {
  using FirstType = First;
  using SecondType = Second;
  using ThirdType = Third;
};

template <typename... Types>
struct PairTypeExtractor;

template <typename First, typename Second>
struct PairTypeExtractor<std::pair<First, Second>> {
  using FirstType = First;
  using SecondType = Second;
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

template <typename T>
constexpr static inline bool always_false = false;

template <typename T>
inline void insert_back(std::vector<T> &container, const T &val) {
  container.push_back(std::move(val));
}

template <typename T>
inline void insert_back(std::set<T> &container, const T &val) {
  container.insert(std::move(val));
}

template <typename Container, typename U>
typename std::enable_if<is_std_queue_v<std::decay_t<Container>> ||
                        is_std_stack_v<std::decay_t<Container>>>::type inline insert_back(Container &container,
                                                                                          const U &val) {
  container.push(std::move(val));
}

template <typename T>
inline T get_top(const std::queue<T> &container) {
  return container.front();
}

template <typename T>
inline T get_top(const std::stack<T> &container) {
  return container.top();
}
