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

/**
  @file type_traits.h
  @brief This file contains type traits which are used with the (de)serialization functions.
*/

#ifndef ENIGMA_TYPE_TRAITS_H
#define ENIGMA_TYPE_TRAITS_H

#include <complex>
#include <queue>
#include <set>
#include <stack>
#include "../var4.h"

#define DEFINE_STD_CONTAINER_TRAIT(CONTAINER_TYPE, TRAIT_NAME)    \
  template <typename T>                                           \
  struct TRAIT_NAME : std::false_type {};                         \
                                                                  \
  template <typename... Args>                                     \
  struct TRAIT_NAME<CONTAINER_TYPE<Args...>> : std::true_type {}; \
                                                                  \
  template <typename T>                                           \
  constexpr bool TRAIT_NAME##_v = TRAIT_NAME<T>::value;

DEFINE_STD_CONTAINER_TRAIT(std::vector, is_std_vector)
DEFINE_STD_CONTAINER_TRAIT(std::set, is_std_set)
DEFINE_STD_CONTAINER_TRAIT(std::queue, is_std_queue)
DEFINE_STD_CONTAINER_TRAIT(std::stack, is_std_stack)
DEFINE_STD_CONTAINER_TRAIT(std::map, is_std_map)
DEFINE_STD_CONTAINER_TRAIT(std::complex, is_std_complex)
DEFINE_STD_CONTAINER_TRAIT(std::pair, is_std_pair)
DEFINE_STD_CONTAINER_TRAIT(std::tuple, is_std_tuple)
DEFINE_STD_CONTAINER_TRAIT(lua_table, is_lua_table)

#undef DEFINE_STD_CONTAINER_TRAIT

template <typename T>
struct lua_inner_type;

template <typename T>
struct lua_inner_type<lua_table<T>> {
  using type = T;
};

template <typename T>
struct complex_inner_type;

template <typename T>
struct complex_inner_type<std::complex<T>> {
  using type = T;
};

template <typename T>
using complex_inner_type_t = typename complex_inner_type<T>::type;

template <typename T, typename = void>
struct TupleTypeExtractor {};

template <typename... Ts>
struct TupleTypeExtractor<std::tuple<Ts...>> {
  using TupleTypes = std::tuple<Ts...>;
  using ResultType = std::tuple<Ts...>;
};

template <typename T, std::size_t I = 0, typename = void>
struct TupleTypeExtractorHelper {};

template <typename T, std::size_t I>
struct TupleTypeExtractorHelper<T, I,
                                std::enable_if_t<I == std::tuple_size_v<typename TupleTypeExtractor<T>::TupleTypes>>> {
  using TupleTypes = std::tuple<>;
  using ResultType = std::tuple<>;
};

template <typename T, std::size_t I>
    struct TupleTypeExtractorHelper < T,
    I, std::enable_if_t<I<std::tuple_size_v<typename TupleTypeExtractor<T>::TupleTypes>>> {
  using ValueType = std::tuple_element_t<I, typename TupleTypeExtractor<T>::TupleTypes>;
  using RemainingTypes = TupleTypeExtractorHelper<T, I + 1>;
  using TupleTypes = std::tuple<ValueType, typename RemainingTypes::TupleTypes>;
  using ResultType = std::tuple<ValueType, typename RemainingTypes::ResultType>;
};

template <typename... Args>
struct TupleSize;

template <typename... Args>
struct TupleSize<std::tuple<Args...>> {
  static const std::size_t value;
};

template <typename... Args>
const std::size_t TupleSize<std::tuple<Args...>>::value = sizeof...(Args);

template <typename... Args>
struct TupleSize<const std::tuple<Args...>&> {
  static const std::size_t value;
};

template <typename... Args>
const std::size_t TupleSize<const std::tuple<Args...>&>::value = sizeof...(Args);

template <std::size_t Index, typename Tuple, typename Function, typename ExtraParam>
struct TupleLooper {
  static void loop(const Tuple& tuple, Function&& func, ExtraParam& extraParam) {
    TupleLooper<Index - 1, Tuple, Function, ExtraParam>::loop(tuple, std::forward<Function>(func), extraParam);
    func(std::get<Index - 1>(tuple), extraParam);
  }
};

template <typename Tuple, typename Function, typename ExtraParam>
struct TupleLooper<0, Tuple, Function, ExtraParam> {
  static void loop(const Tuple& tuple, Function&& func, const ExtraParam& extraParam) {}
};

template <typename... Args, typename Function, typename ExtraParam>
void LoopTuple(const std::tuple<Args...>& tuple, Function&& func, ExtraParam& extraParam) {
  TupleLooper<sizeof...(Args), std::tuple<Args...>, Function, ExtraParam>::loop(tuple, std::forward<Function>(func),
                                                                                extraParam);
}

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

template <typename T, typename ExpectedType, typename ReturnType = void>
using is_t = std::enable_if_t<std::is_same_v<ExpectedType, std::decay_t<T>>, ReturnType>;

template <typename T, typename ReturnType, template <typename> typename... Classes>
using matches_t = std::enable_if_t<std::disjunction<Classes<std::decay_t<T>>...>::value, ReturnType>;

template <typename T, typename ReturnType = void>
using enables_if_numeric_t =
    std::enable_if_t<(std::is_integral_v<std::decay_t<T>> ||
                      std::is_floating_point_v<std::decay_t<T>>)&&!std::is_same_v<std::decay_t<T>, bool>,
                     ReturnType>;

template <typename T>
constexpr bool is_numeric_v = (std::is_integral_v<std::decay_t<T>> ||
                               std::is_floating_point_v<std::decay_t<T>>)&&!std::is_same_v<std::decay_t<T>, bool> &&
                              !std::is_same_v<std::decay_t<T>, char>;

template <typename T, typename ReturnType = void>
using enable_if_inherits_variant_t =
    std::enable_if_t<std::is_base_of_v<variant, std::decay_t<T>> && !std::is_same_v<var, std::decay_t<T>>, ReturnType>;

template <typename T>
inline void insert_back(std::vector<T>& container, const T& val) {
  container.push_back(std::move(val));
}

template <typename T>
inline void insert_back(std::set<T>& container, const T& val) {
  container.insert(std::move(val));
}

template <typename Container, typename U>
inline matches_t<Container, void, is_std_queue, is_std_stack> insert_back(Container& container, const U& val) {
  container.push(std::move(val));
}

template <typename T>
inline T get_top(const std::queue<T>& container) {
  return container.front();
}

template <typename T>
inline T get_top(const std::stack<T>& container) {
  return container.top();
}

#endif
