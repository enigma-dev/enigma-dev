/** Copyright (C) 2008-2011 Josh Ventura
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

#ifndef ENIGMA_ESTRING_H
#define ENIGMA_ESTRING_H

#include "var4.h"

#include <string>

#include "../../../CompilerSource/OS_Switchboard.h"

#if CURRENT_PLATFORM_ID == OS_WINDOWS

#include <cwchar>

typedef std::basic_string<wchar_t> tstring;
tstring widen(const std::string &str);
std::string shorten(tstring str);

#endif

namespace enigma_user {

std::string base64_encode(std::string const& str);
std::string base64_decode(std::string const& str);

bool is_base64(unsigned char byte);

std::string ansi_char(char byte);
std::string chr(char val);
int ord(std::string str);

double real(variant str);

size_t string_length(std::string str);
size_t string_length(const char* str);
#define string_byte_length(x) string_length(x)
size_t string_length_utf8(std::string str);
size_t string_length_utf8(const char* str);
size_t string_pos(std::string substr, std::string str);

std::string string_format(double val, unsigned tot, unsigned dec);
std::string string_copy(std::string str, int index, int count);
std::string string_set_byte_at(std::string str, int pos, char byte);
char string_byte_at(std::string str, int index);
std::string string_char_at(std::string str, int index);
std::string string_delete(std::string str, int index, int count);
std::string string_insert(std::string substr, std::string str, int index);
std::string string_replace(std::string str, std::string substr, std::string newstr);
std::string string_replace_all(std::string str, std::string substr, std::string newstr);
size_t string_count(std::string substr, std::string str);

std::string string_lower(std::string str);
std::string string_upper(std::string str);

std::string string_repeat(std::string str, int count);

std::string string_letters(std::string str);
std::string string_digits(std::string str);
std::string string_lettersdigits(std::string str);

bool string_isletters(std::string str);
bool string_isdigits(std::string str);
bool string_islettersdigits(std::string str);

std::string filename_name(std::string fname);
std::string filename_path(std::string fname);
std::string filename_dir(std::string fname);
std::string filename_drive(std::string fname);
std::string filename_ext(std::string fname);
std::string filename_change_ext(std::string fname, std::string newext);

var string_split(const std::string &str, const std::string &delim,
                 bool skip_empty = false);

}  //namespace enigma_user

#endif  //ENIGMA_ESTRING_H
