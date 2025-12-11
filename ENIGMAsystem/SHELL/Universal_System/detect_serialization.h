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
#ifndef ENIGMA_DETECT_SERIALIZATION_H
#define ENIGMA_DETECT_SERIALIZATION_H

#include <cstddef>
#include <vector>

#define INTERNAL_HAS_MEMBER_FUNCTION(NAME, FUNC)         \
  template <typename T>                                  \
  class has_##NAME##_method {                            \
    FUNC;                                                \
    template <typename U>                                \
    static char func(Check<U, &U::NAME> *);              \
    template <typename U>                                \
    static int func(...);                                \
                                                         \
   public:                                               \
    typedef has_##NAME##_method type;                    \
    enum { value = sizeof(func<T>(0)) == sizeof(char) }; \
  };                                                     \
                                                         \
  template <typename T>                                  \
  constexpr static inline bool has_##NAME##_method_v = has_##NAME##_method<T>::value

#define INTERNAL_FUNCTION_PREFIX template <typename U,
#define INTERNAL_FUNCTION_SUFFIX > struct Check

#define HAS_MEMBER_FUNCTION(NAME, FUNCTION) \
  INTERNAL_HAS_MEMBER_FUNCTION(NAME, INTERNAL_FUNCTION_PREFIX FUNCTION INTERNAL_FUNCTION_SUFFIX)

HAS_MEMBER_FUNCTION(size, std::size_t (U::*)() const noexcept);
HAS_MEMBER_FUNCTION(byte_size, std::size_t (U::*)() const noexcept);
HAS_MEMBER_FUNCTION(serialize, std::vector<std::byte> (U::*)() const);
HAS_MEMBER_FUNCTION(deserialize_self, std::size_t (U::*)(std::byte *iter));

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

#undef HAS_STATIC_FUNCTION_V
#undef HAS_STATIC_FUNCTION

#define HAS_SERIALIZE_INTO_FUNCTION() std::is_invocable_r_v<void, decltype(enigma::serialize_into<T>), std::byte *, T>
#define HAS_SERIALIZE_FUNCTION() std::is_invocable_r_v<std::vector<std::byte>, decltype(enigma::serialize<T>), T>
#define HAS_DESERIALIZE_FUNCTION() std::is_invocable_r_v<T, decltype(enigma::deserialize<T>), std::byte *>

#endif