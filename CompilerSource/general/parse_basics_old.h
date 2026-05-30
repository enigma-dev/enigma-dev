/*********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\*********************************************************************************/
#ifndef PARSE_BASICS_OLD_H
#define PARSE_BASICS_OLD_H

inline bool is_letter(char x) {
  return (x>='a' && x<='z') || (x>='A' && x<='Z') || (x=='_');
}

inline bool is_digit(char x) {
  return x>='0' && x<='9';
}

inline bool is_hexdigit(char x) {
  return (x>='0' && x<='9') || (x>='A' && x<='F') || (x>='a' && x<='f');
}

inline bool is_nybble(char x) {
  return is_hexdigit(x);
}

inline bool is_octal(char x) {
  return (x>='0' && x<='7');
}

inline bool is_bit(char x) {
  return x == '0' || x == '1';
}

inline bool is_letterd(char x) {
  return is_letter(x) || is_digit(x);
}
inline bool is_letterdd(char x)
{
  return  (x>='a' and x<='z') or (x>='A' and x<='Z') or (x=='_' or x=='$') or (x>='0' and x<='9');
}

inline bool is_useless(char x) 
{
  return x==' ' || x=='\r' || x=='\n' || x=='\t';
}
inline bool is_useless_macros(char x) 
{
  return x==' ' || x=='\t';
}

inline bool is_unary(char x) 
{
  return x=='!' || x=='~' || x=='+' || x=='-' || x=='&' || x=='*';
}

inline bool is_linker(char x)
{
  return x=='+' || x=='-' || x=='*' || x=='/' || x=='=' || x=='!' || x=='~' || x=='&' || x=='|' || x=='^' || x=='.';
}

#endif
