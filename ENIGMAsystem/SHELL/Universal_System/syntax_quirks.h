/**  Copyright (C) 2008 Josh Ventura
 This file is a part of the ENIGMA Development Environment.
 ENIGMA is free software: you can redistribute it and/or modify it under the
 terms of the GNU General Public License as published by the Free Software
 Foundation, version 3 of the license or any later version. This application
 and its source code are distributed AS-IS, WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU General Public License for more details. 
**/

#define repeat(x) for (int ENIGMA_REPEAT_VAR = (x); ENIGMA_REPEAT_VAR > 0; ENIGMA_REPEAT_VAR--)
#define mod %(variant)

#include <cmath>

// x div y is x/y truncated toward zero, computed in reals: int operands would
// make 5 div 0.5 divide by zero.
struct INTEGER_DIVISION
{
    double v;
    explicit INTEGER_DIVISION(double a): v(a) {}
};
template<typename real> double operator/ (real x, INTEGER_DIVISION y) { return std::trunc(double(x)/y.v); }
#define div /(INTEGER_DIVISION)(double)

#define until(x) while(!(x))
