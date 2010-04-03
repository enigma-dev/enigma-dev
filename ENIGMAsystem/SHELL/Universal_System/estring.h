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

/**String functions*****************************************************\

bool is_string(enigma::variant val)
bool is_real(enigma::variant val)
std::string chr(char val)
double ord(char* str)
double real(std::string str)
std::string string(double val)
int string_length(std::string str)
int string_pos(std::string substr,std::string str)
std::string string_copy(std::string str,double index,double count)
std::string string_char_at(std::string str,double index)
std::string string_delete(std::string str,double index,double count)
std::string string_insert(std::string substr,std::string str,double index)
std::string string_replace(std::string str,std::string substr,std::string newstr)
std::string string_replace_all(std::string str,std::string substr,std::string newstr)
int string_count(std::string substr,std::string str)
std::string string_lower(std::string str)
std::string string_upper(std::string str)
std::string string_repeat(std::string str,double count)
std::string string_letters(std::string str)
std::string string_digits(std::string str)
std::string string_lettersdigits(std::string str)

\******************************************************************************/




extern bool is_string(enigma::variant val);
bool is_string(var& val);

bool is_real(enigma::variant val);
bool is_real(var& val);

std::string chr(char val);

int ord(char str);
int ord(char* str);
int ord(std::string str);

double real(std::string str);
double real(char* str);
double real(double str);
double real(var& str);

size_t string_length(std::string str);
size_t string_length(char* str);
int string_pos(std::string substr,std::string str);
int string_pos(char* substr,std::string str);

std::string string_copy(std::string str,double index,double count);
std::string string_char_at(std::string str,double index);
std::string string_delete(std::string str,double index,double count);
std::string string_insert(std::string substr,std::string str,double index);
std::string string_replace(std::string str,std::string substr,std::string newstr);
std::string string_replace_all(std::string str,std::string substr,std::string newstr);
int string_count(std::string substr,std::string str);

std::string string_lower(std::string str);
std::string string_upper(std::string str);

std::string string_repeat(std::string str,double count);

std::string string_letters(std::string str);
std::string string_digits(std::string str);
std::string string_lettersdigits(std::string str);

