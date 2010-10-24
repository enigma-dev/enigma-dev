/********************************************************************************\
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
\********************************************************************************/

bool is_string(variant val);
bool is_real(variant val);

string chr(char val);

int ord(char str);
int ord(const char* str);
int ord(string str);

double real(string str);
double real(char* str);
double real(double str);
double real(var& str);

size_t string_length(string str);
size_t string_length(char* str);
int string_pos(string substr,string str);
int string_pos(char* substr,string str);

string string_copy(string str,double index,double count);
string string_char_at(string str,double index);
string string_delete(string str,double index,double count);
string string_insert(string substr,string str,double index);
string string_replace(string str,string substr,string newstr);
string string_replace_all(string str,string substr,string newstr);
int string_count(string substr,string str);

string string_lower(string str);
string string_upper(string str);

string string_repeat(string str,double count);

string string_letters(string str);
string string_digits(string str);
string string_lettersdigits(string str);

