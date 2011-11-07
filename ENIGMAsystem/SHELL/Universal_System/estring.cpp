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

#include <string>
#include <cstdlib>
#include "var4.h"
#include "estring.h"

#ifdef DEBUG_MODE
#include "../libEGMstd.h"
#include "../Widget_Systems/widgets_mandatory.h"
#endif

#undef string

static const char ldgrs[256] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,
  0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0
};

bool is_string(variant val) { return val.type;  }
bool is_real(variant val)   { return !val.type; }
string chr(char val) { return string(1,val); }
int ord(string str)  { return str[0]; }

double real(variant str) { return str.type ? atof(((string)str).c_str()) : (double) str; }

size_t string_length(string str) { return str.length(); }
size_t string_length(const char* str)
{
	const char* s = str;
	while (*s++);
	return str - s;
}

int string_pos(string substr,string str) {
	const size_t res = str.find(substr,0)+1;
	return res == string::npos ? -1 : (int)res;
}

string string_copy(string str, int index, int count) {
	if (index <= 1) return str;
	return (size_t)index > str.length()? "": str.substr(index-1, count);
}

string string_char_at(string str,int index) {
	unsigned int n = index <= 1? 0: (unsigned int)index-1;
	#ifdef DEBUG_MODE
	  if (n > str.length())
	    show_error("Index " + toString(index) + " is outside range " + toString(str.length()) + " in the following string:\n\"" + str + "\".", false);
  #endif
	return string(1, str[n]);
}

string string_delete(string str,int index,int count) {
	return str.erase(index < 2? 0: index-1, count < 1? 0: count);
}

string string_insert(string substr,string str,int index) {
	if (index<=1) return substr + str;
	const size_t x = index-1;
	return x>str.length()? str + substr: str.insert(x, substr);
}

string string_replace(string str,string substr,string newstr) {
	size_t pos=str.find(substr,0);
	return pos==(size_t)-1?str:str.replace(pos,substr.length(),newstr);
}

string string_replace_all(string str,string substr,string newstr) {
	size_t pos = 0;
	const size_t sublen = substr.length(), newlen = newstr.length();
    while((pos=str.find(substr,pos)) != string::npos) {
		str.replace(pos,sublen,newstr);
		pos += newlen;
	}
	return str;
}

int string_count(string substr,string str) {
	size_t pos = 0, occ = 0;
	const size_t sublen = substr.length();
  while((pos=str.find(substr,pos))!=-1) occ++, pos += sublen;
  return occ;
}

string string_lower(string str) {
	const size_t len = str.length()-1;
	for(size_t i = 0; i < len; i++)
		if(ldgrs[(int)(unsigned char)str[i]] & 2)
		  str[i] += 32;
	return str;
}

string string_upper(string str) {
	int len=str.length()-1;
	for(int i = 0; i < len; i++)
		if(ldgrs[(unsigned char)str[i]]&1) str[i]-=32;
	return str;
}

string string_repeat(string str,int count) {
	string ret; ret.reserve(str.length() * count);
	for(int i = count; i; i--) ret.append(str);
	return ret;
}

string string_letters(string str) {
	string ret;
	for(const char*c=str.c_str();*c;c++)
		if(ldgrs[(unsigned char)*c]&3) ret+=*c;
	return ret;
}

string string_digits(string str) {
	string ret;
	for(const char*c=str.c_str();*c;c++)
		if(ldgrs[(unsigned char)*c]&4) ret += *c;
	return ret;
}

string string_lettersdigits(string str) {
	string ret;
	for(const char*c=str.c_str();*c;c++)
		if(ldgrs[(unsigned char)*c]) ret += *c;
	return ret;
}

bool string_isletters(string str) {
	for(const char*c = str.c_str(); *c; c++)
		if(!(ldgrs[(unsigned char)*c] & 3))
      return false;
	return true;
}

bool string_isdigits(string str) {
	for(const char*c = str.c_str(); *c; c++)
		if(!(ldgrs[(unsigned char)*c] & 4))
		  return false;
	return true;
}

bool string_islettersdigits(string str) {
	for(const char*c=str.c_str(); *c; c++)
		if(!ldgrs[(unsigned char)*c])
		  return false;
	return true;
}
