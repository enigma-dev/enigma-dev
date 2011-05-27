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

struct INTEGER_DIVISION
{
    int v;
    INTEGER_DIVISION(int a): v(a) {}
};
template<typename real>  int operator/ (real x, INTEGER_DIVISION y) { return int(x)/y.v; }
#define div /(INTEGER_DIVISION)(int)

#define until(x) while(!(x))
