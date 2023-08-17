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

#ifndef ENIGMA_DETECT_SERIALIZATION_H
#define ENIGMA_DETECT_SERIALIZATION_H

#include <cstddef>

#include "serialization_fwd_decl.h"
// #include"detect_size.h"
#include "Types_Implementation/bool.h"
#include "Types_Implementation/containers.h"
#include "Types_Implementation/pointer.h"
#include "Types_Implementation/std_complex.h"
#include "Types_Implementation/std_map.h"
#include "Types_Implementation/std_pair.h"
#include "Types_Implementation/std_queue_stack.h"
#include "Types_Implementation/std_string.h"
#include "Types_Implementation/std_tuple.h"
// #include "Types_Implementation/numerics.h"
// #include "Types_Implementation/var.h"
// #include "Types_Implementation/variant.h"
// #include "Types_Implementation/lua_table.h"
// #include "types_serialization_includes.h"

#define INTERNAL_HAS_MEMBER_FUNCTION(NAME, FUNC)      \
  template <typename T>                               \
  class has_##NAME##_method {                         \
    FUNC;                                             \
    template <typename U>                             \
    static std::true_type func(Check<U, &U::NAME> *); \
    template <typename U>                             \
    static std::false_type func(...);                 \
                                                      \
   public:                                            \
    typedef has_##NAME##_method type;                 \
    enum { value = decltype(func<T>(0))::value };     \
  };                                                  \
                                                      \
  template <typename T>                               \
  constexpr static inline bool has_##NAME##_method_v = has_##NAME##_method<T>::value

#define INTERNAL_FUNCTION_PREFIX template <typename V,
#define INTERNAL_FUNCTION_SUFFIX > struct Check

#define HAS_MEMBER_FUNCTION(NAME, FUNCTION) \
  INTERNAL_HAS_MEMBER_FUNCTION(NAME, INTERNAL_FUNCTION_PREFIX FUNCTION INTERNAL_FUNCTION_SUFFIX)

HAS_MEMBER_FUNCTION(size, std::size_t (V::*)() const noexcept);
HAS_MEMBER_FUNCTION(byte_size, std::size_t (V::*)() const noexcept);
HAS_MEMBER_FUNCTION(serialize, std::vector<std::byte> (V::*)() const);
HAS_MEMBER_FUNCTION(deserialize_self, std::size_t (V::*)(std::byte *iter));

/**
 * Now we have 4 classes with the following names:
 * has_size_method
 * has_byte_size_method
 * has_serialize_method
 * has_deserialize_self_method
 * 
 * Each one has 2 data members:
 * type: This member is a typedef that refers to the class itself (has_size_method,
 * has_byte_size_method, has_serialize_method, has_deserialize_self_method)
 * 
 * value: This member is a boolean value that indicates whether the corresponding class
 * has a method with the specified NAME (size, byte_size, serialize and deserialize_self),
 * it is true if the class has the method and false otherwise.
 */

#undef HAS_MEMBER_FUNCTION
#undef INTERNAL_FUNCTION_SUFFIX
#undef INTERNAL_FUNCTION_PREFIX
#undef INTERNAL_HAS_MEMBER_FUNCTION

#define HAS_STATIC_FUNCTION(NAME)                                    \
  template <typename, typename>                                      \
  struct has_##NAME##_function;                                      \
                                                                     \
  template <typename T, typename Ret, typename... Args>              \
  struct has_##NAME##_function<T, Ret(Args...)> {                    \
    template <typename U, U>                                         \
    struct Check;                                                    \
                                                                     \
    template <typename U>                                            \
    static std::true_type Test(Check<Ret (*)(Args...), &U::NAME> *); \
                                                                     \
    template <typename U>                                            \
    static std::false_type Test(...);                                \
                                                                     \
    static const bool value = decltype(Test<T>(0))::value;           \
  };

#define HAS_STATIC_FUNCTION_V(NAME, ...) \
  HAS_STATIC_FUNCTION(NAME)              \
  template <typename T>                  \
  constexpr static inline bool has_##NAME##_function_v = has_##NAME##_function<T, __VA_ARGS__>::value

HAS_STATIC_FUNCTION_V(deserialize, std::pair<std::size_t, T>(std::byte *iter));

/**
 * Now we have 1 struct with the following name:
 * has_deserialize_function
 * 
 * The struct has 1 data member:
 * value: This member is a boolean value that indicates whether the corresponding class
 * has a static function with the specified NAME (deserialize),
 * it is true if the class has the function and false otherwise.
 */

#undef HAS_STATIC_FUNCTION_V
#undef HAS_STATIC_FUNCTION

#define HAS_FREE_FUNCTION(NAME, Parameters...)                                                             \
  template <typename T, typename = void>                                                                   \
  struct is_##NAME##_available : std::false_type {};                                                       \
                                                                                                           \
  template <typename T>                                                                                    \
  struct is_##NAME##_available<T, std::void_t<decltype(enigma::NAME<T>(Parameters))>> : std::true_type {}; \
                                                                                                           \
  template <typename T>                                                                                    \
  constexpr static inline bool has_##NAME##_free_function = is_##NAME##_available<T>::value

HAS_FREE_FUNCTION(byte_size, std::declval<const T &>());
HAS_FREE_FUNCTION(internal_serialize_into_fn, std::declval<std::byte *>(), std::declval<T &&>());
HAS_FREE_FUNCTION(internal_serialize_fn, std::declval<T &&>());
HAS_FREE_FUNCTION(internal_deserialize_fn, std::declval<std::byte *>());
HAS_FREE_FUNCTION(internal_resize_buffer_for_fn, std::declval<std::vector<std::byte> &>(), std::declval<T &&>());
HAS_FREE_FUNCTION(enigma_internal_deserialize_fn, std::declval<T &>(), std::declval<std::byte *>(),
                  std::declval<std::size_t &>());

/**
 * Now we have 6 structs with the following name:
 * is_byte_size_available
 * is_internal_serialize_into_fn_available
 * is_internal_serialize_fn_available
 * is_internal_deserialize_fn_available
 * is_internal_resize_buffer_for_fn_available
 * is_enigma_internal_deserialize_fn_available
 * 
 * Each struct has 1 data member:
 * value: This member is a boolean value that indicates whether the corresponding class
 * has a specialization which is callable with this type with the specified NAME (byte_size,
 * internal_serialize_into_fn, internal_serialize_fn, internal_deserialize_fn,
 * internal_resize_buffer_for_fn and enigma_internal_deserialize_fn),
 * it is true if the class has the function and false otherwise.
 */

#undef HAS_FREE_FUNCTION

#define HAS_INTERNAL_SERIALIZE_INTO_FUNCTION() \
  std::is_invocable_r_v<void, decltype(enigma::internal_serialize_into_fn<T>), std::byte *, T>
#define HAS_INTERNAL_SERIALIZE_FUNCTION() std::is_invocable_v<decltype(enigma::internal_serialize_fn<T>), T>
#define HAS_DESERIALIZE_FUNCTION() std::is_invocable_r_v<T, decltype(enigma::internal_deserialize<T>), std::byte *>

#endif
