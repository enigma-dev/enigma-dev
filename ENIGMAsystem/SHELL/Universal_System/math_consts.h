#ifdef INCLUDED_FROM_SHELLMAIN
  #error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

#ifndef MATH_CONSTS_h
#define MATH_CONSTS_h

#include <cmath>
#ifndef M_PI // M_PI is non-standard
#  define M_PI   3.14159265358979323846
#  define M_PI_2 1.57079632679489661923
#endif

#ifndef M_SQRT1_2 // This is ours
#  define M_SQRT1_2 0.70710678118654752440
#endif

#endif
