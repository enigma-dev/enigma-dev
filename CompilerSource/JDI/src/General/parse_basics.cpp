/**
 * @file parse_basics.cpp
 * @brief Source implementing the backside of basic methods required during a parse.
 * 
 * If Java has one nice feature, it's the static {} block in classes.
 * 
 * @section License
 * 
 * Copyright (C) 2011-2012 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include "parse_basics.h"
#include <cstdio>

namespace parse_bacics {
  is_::is_() {
    for (int i = 0; i < 256; i++)
      whatever[i] = 0;
    for (int i = 'A'; i <= 'Z'; i++)
      whatever[i] |= is_::LETTER;
    for (int i = 'a'; i <= 'z'; i++)
      whatever[i] |= is_::LETTER;
    for (int i = 'A'; i <= 'F'; i++)
      whatever[i] |= is_::HEXDIGIT;
    for (int i = 'a'; i <= 'f'; i++)
      whatever[i] |= is_::HEXDIGIT;
    for (int i = '0'; i <= '7'; i++)
      whatever[i] |= is_::DIGIT | is_::OCTDIGIT | is_::HEXDIGIT;
    for (int i = '8'; i <= '9'; i++)
      whatever[i] |= is_::DIGIT | is_::HEXDIGIT;
    
    whatever['_'] |= is_::LETTER;
    whatever['$'] |= is_::LETTER;
    
    whatever[' ']  |= is_::SPACER;
    whatever['\t'] |= is_::SPACER;
    whatever['\r'] |= is_::NEWLINE;
    whatever['\n'] |= is_::NEWLINE;
    whatever[0x0C] |= is_::NEWLINE;
  }

  is_ is;
  namespace visible {
    string toString(int n) { char buf[12]; return string(buf,sprintf(buf,"%d", n)); }
    string toString(long n) { char buf[12]; return string(buf,sprintf(buf,"%ld", n)); }
    string toString(short n) { char buf[12]; return string(buf,sprintf(buf,"%d", n)); }
    string toString(unsigned n) { char buf[12]; return string(buf,sprintf(buf,"%u", n)); }
    string toString(unsigned long n) { char buf[12]; return string(buf,sprintf(buf,"%lu", n)); }
    string toString(unsigned short n) { char buf[12]; return string(buf,sprintf(buf,"%u", n)); }
    string toString(char n) { char buf[8]; return string(buf,sprintf(buf,"%d", n)); }
    string toString(char* n) { return string(n); }
    string toString(float n) { char buf[32]; return string(buf,sprintf(buf,"%g", n)); }
    string toString(double n) { char buf[32]; return string(buf,sprintf(buf,"%g", n)); }
  }
}
