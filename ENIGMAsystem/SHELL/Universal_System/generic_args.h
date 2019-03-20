/** Copyright (C) 2018 Josh Ventura
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

#ifndef ENIGMA_GENERIC_ARGS_H
#define ENIGMA_GENERIC_ARGS_H

#ifndef JUST_DEFINE_IT_RUN

#include <cstdint>
#include <limits>

#include "var4.h"

namespace enigma {

template<typename E> struct EnabledType {
  typedef E T;
  template<typename R> struct returns {
    typedef R T;
  };
};

template<typename T> struct SIntTypeEnabler {};
template<> struct SIntTypeEnabler<int8_t>  : EnabledType<int8_t>  {};
template<> struct SIntTypeEnabler<int16_t> : EnabledType<int16_t> {};
template<> struct SIntTypeEnabler<int32_t> : EnabledType<int32_t> {};
template<> struct SIntTypeEnabler<int64_t> : EnabledType<int64_t> {};
template<typename T> using SIntType = typename SIntTypeEnabler<T>::T;

template<typename T> struct UIntTypeEnabler {};
template<> struct UIntTypeEnabler<uint8_t>  : EnabledType<uint8_t> {};
template<> struct UIntTypeEnabler<uint16_t> : EnabledType<uint16_t> {};
template<> struct UIntTypeEnabler<uint32_t> : EnabledType<uint32_t> {};
template<> struct UIntTypeEnabler<uint64_t> : EnabledType<uint64_t> {};
template<typename T> using UIntType = typename UIntTypeEnabler<T>::T;

template<typename T> struct IntTypeEnabler {};
template<> struct IntTypeEnabler<int8_t>   : EnabledType<int8_t> {};
template<> struct IntTypeEnabler<int16_t>  : EnabledType<int16_t> {};
template<> struct IntTypeEnabler<int32_t>  : EnabledType<int32_t> {};
template<> struct IntTypeEnabler<int64_t>  : EnabledType<int64_t> {};
template<> struct IntTypeEnabler<uint8_t>  : EnabledType<uint8_t> {};
template<> struct IntTypeEnabler<uint16_t> : EnabledType<uint16_t> {};
template<> struct IntTypeEnabler<uint32_t> : EnabledType<uint32_t> {};
template<> struct IntTypeEnabler<uint64_t> : EnabledType<uint64_t> {};
template<typename T> using IntType = typename IntTypeEnabler<T>::T;

template<typename T> struct FloatTypeEnabler {};
template<> struct FloatTypeEnabler<float>       : EnabledType<float> {};
template<> struct FloatTypeEnabler<double>      : EnabledType<double> {};
template<> struct FloatTypeEnabler<long double> : EnabledType<long double> {};
template<typename T> using FloatType = typename FloatTypeEnabler<T>::T;

template<typename T> struct NumericTypeEnabler {};
template<> struct NumericTypeEnabler<int8_t>      : EnabledType<int8_t> {};
template<> struct NumericTypeEnabler<int16_t>     : EnabledType<int16_t> {};
template<> struct NumericTypeEnabler<int32_t>     : EnabledType<int32_t> {};
template<> struct NumericTypeEnabler<int64_t>     : EnabledType<int64_t> {};
template<> struct NumericTypeEnabler<uint8_t>     : EnabledType<uint8_t> {};
template<> struct NumericTypeEnabler<uint16_t>    : EnabledType<uint16_t> {};
template<> struct NumericTypeEnabler<uint32_t>    : EnabledType<uint32_t> {};
template<> struct NumericTypeEnabler<uint64_t>    : EnabledType<uint64_t> {};
template<> struct NumericTypeEnabler<float>       : EnabledType<float> {};
template<> struct NumericTypeEnabler<double>      : EnabledType<double> {};
template<> struct NumericTypeEnabler<long double> : EnabledType<long double> {};
template<typename T> using NumericType = typename NumericTypeEnabler<T>::T;

template<typename T> struct ArithmeticTypeEnabler {};
template<> struct ArithmeticTypeEnabler<int8_t>   : EnabledType<int8_t> {};
template<> struct ArithmeticTypeEnabler<int16_t>  : EnabledType<int16_t> {};
template<> struct ArithmeticTypeEnabler<int32_t>  : EnabledType<int32_t> {};
template<> struct ArithmeticTypeEnabler<int64_t>  : EnabledType<int64_t> {};
template<> struct ArithmeticTypeEnabler<uint8_t>  : EnabledType<uint8_t> {};
template<> struct ArithmeticTypeEnabler<uint16_t> : EnabledType<uint16_t> {};
template<> struct ArithmeticTypeEnabler<uint32_t> : EnabledType<uint32_t> {};
template<> struct ArithmeticTypeEnabler<uint64_t> : EnabledType<uint64_t> {};
template<> struct ArithmeticTypeEnabler<float>    : EnabledType<float> {};
template<> struct ArithmeticTypeEnabler<double>   : EnabledType<double> {};
template<> struct ArithmeticTypeEnabler<long double>    : EnabledType<long double>    {};
template<> struct ArithmeticTypeEnabler<const var&>     : EnabledType<const var&>     {};
template<> struct ArithmeticTypeEnabler<const variant&> : EnabledType<const variant&> {};
template<typename T> using ArithmeticType = typename ArithmeticTypeEnabler<T>::T;

template<typename X, typename Y = X, typename Z = Y,
         typename W = Z, typename P = W, typename R = P,
         typename EN = decltype(X() - Y() - Z() - W() - P() - R())>
struct ArithmeticTypes: EnabledType<EN> {};

}  // namespace enigma

namespace std {

template<> class numeric_limits<var>: numeric_limits<double> {};
template<> class numeric_limits<variant>: numeric_limits<double> {};
template<> class numeric_limits<const var&>: numeric_limits<double> {};
template<> class numeric_limits<const variant&>: numeric_limits<double> {};

}

#define TEMPLATE_FN(...) template<__VA_ARGS__>

#else  // JUST_DEFINE_IT_RUN

namespace enigma {

template<typename E> struct EnabledType {
  typedef E T;
  template<typename R> struct returns {
    typedef R T;
  };
};

template<typename X> class SIntType: EnabledType<int> {};
template<typename X> class UIntType: EnabledType<unsigned> {};
template<typename X> class IntType:  EnabledType<int> {};
template<typename X> class FloatType      : EnabledType<double> {};
template<typename X> class NumericType    : EnabledType<double> {};
template<typename X> class ArithmeticType : EnabledType<double> {};

template<typename X, typename Y=int, typename Z = int,
         typename W = int, typename P = int, typename R = int>
class ArithmeticTypes { typedef double T; };

}  // namespace enigma

#define TEMPLATE_FN(...)

#endif  // JUST_DEFINE_IT_RUN

#endif  // ENIGMA_GENERIC_ARGS_H
