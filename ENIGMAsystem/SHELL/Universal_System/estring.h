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

#include "var4.h"

namespace enigma_user
{

string base64_encode(string const& str);
string base64_decode(string const& str);

bool is_base64(unsigned char byte);
bool is_string(variant val);
bool is_real(variant val);

string ansi_char(char byte);
string chr(char val);
int ord(string str);

double real(variant str);

size_t string_length(string str);
size_t string_length(const char* str);
#define string_byte_length(x) string_length(x)
size_t string_length_utf8(string str);
size_t string_length_utf8(const char* str);
size_t string_pos(string substr, string str);

string string_format(double val, unsigned tot, unsigned dec);
string string_copy(string str, int index, int count);
string string_set_byte_at(string str, int pos, char byte);
char string_byte_at(string str, int index);
string string_char_at(string str,int index);
string string_delete(string str, int index, int count);
string string_insert(string substr, string str, int index);
string string_replace(string str, string substr, string newstr);
string string_replace_all(string str, string substr, string newstr);
size_t string_count(string substr, string str);

string string_lower(string str);
string string_upper(string str);

string string_repeat(string str, int count);

string string_letters(string str);
string string_digits(string str);
string string_lettersdigits(string str);

bool string_isletters(string str);
bool string_isdigits(string str);
bool string_islettersdigits(string str);

string filename_name(string fname);
string filename_path(string fname);
string filename_dir(string fname);
string filename_drive(string fname);
string filename_ext(string fname);
string filename_change_ext(string fname, string newext);

}
