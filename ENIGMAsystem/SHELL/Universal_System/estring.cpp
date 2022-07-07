/** Copyright (C) 2008-2013 Josh Ventura
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

#include "strings_util.h"

#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <vector>
#include "var4.h"
#include "estring.h"

#ifdef DEBUG_MODE
#include "libEGMstd.h"
#include "Widget_Systems/widgets_mandatory.h"
#endif

using std::string;

#include "../../../CompilerSource/OS_Switchboard.h"
#include "../../../shared/libbase64_util/libbase64_util.h"

#if CURRENT_PLATFORM_ID == OS_WINDOWS

#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

using std::vector;

tstring widen(const string &str) {
  // Number of shorts will be <= number of bytes; add one for null terminator
  const size_t wchar_count = str.size() + 1;
  vector<WCHAR> buf(wchar_count);
  return tstring{buf.data(), (size_t)MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buf.data(), (int)wchar_count)};
}

string shorten(tstring str) {
  int nbytes = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), (int)str.length(), NULL, 0, NULL, NULL);
  vector<char> buf((size_t)nbytes);
  return string{buf.data(), (size_t)WideCharToMultiByte(CP_UTF8, 0, str.c_str(), (int)str.length(), buf.data(), nbytes, NULL, NULL)};
}

#endif

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

namespace enigma_user {

bool is_base64(unsigned char c) {
  return libbase64_is_base64(c);
}

string base64_encode(string const& str) {
  return libbase64_encode(str);
}

string base64_decode(string const& str) {
  return libbase64_decode(str);
}

double real(variant str) { return str.type ? atof(((string)str).c_str()) : (double) str; }

string ansi_char(char byte) { return string(1,byte); }
string chr(char val) { return string(1,val); }
int ord(string str)  { return str[0]; }

size_t string_length(string str) { return str.length(); }
size_t string_length(const char* str) { return strlen(str); }

size_t string_length_utf8(string str) { 
  size_t res = 0; 
  for (size_t i = 0; i < str.length(); ++i) 
    if ((str[i] & 0xC0) != 0x80) 
      ++res; 
  return res; 
}

size_t string_length_utf8(const char* str) { 
  size_t res = 0; 
  for (size_t i = 0; str[i]; ++i) 
    if ((str[i] & 0xC0) != 0x80) 
      ++res; 
  return res; 
}

size_t string_pos(string substr,string str) {
  const size_t res = str.find(substr,0)+1;
  return res == string::npos ? 0 : (int)res;
}

string string_format(double val, unsigned tot, unsigned dec) {
  std::vector<char> sbuf(19 + tot + dec);
  sbuf[0] = 0;
  sprintf(sbuf.data(), "%0*.*f", tot, dec, val);
  return sbuf.data();
}

string string_copy(string str, int index, int count) {
  index = index < 0 ? 0 : index;
  return (size_t)index > str.length()? "": str.substr(index < 2? 0: index-1, count < 1? 0: count);
}

string string_set_byte_at(string str, int index, char byte) {
  if (index<=1) return str + byte;
  const size_t x = index-1;
  return x>str.length()? str + byte: str.replace(x, 1, 1, byte);
}

char string_byte_at(string str, int index) {
  unsigned int n = index <= 1 ? 0 : (unsigned int)(index - 1);
  #ifdef DEBUG_MODE
    if (n > str.length())
      DEBUG_MESSAGE("Index " + toString(index) + " is outside range " + toString(str.length()) + " in the following string:\n\"" + str + "\".", MESSAGE_TYPE::M_ERROR);
  #endif
  return str[n];
}

string string_char_at(string str,int index) {
  unsigned int n = index <= 1 ? 0 : (unsigned int)(index - 1);
  #ifdef DEBUG_MODE
    if (n > str.length())
      DEBUG_MESSAGE("Index " + toString(index) + " is outside range " + toString(str.length()) + " in the following string:\n\"" + str + "\".", MESSAGE_TYPE::M_ERROR);
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
  return ::string_replace_all(str, substr, newstr);
}

size_t string_count(string substr,string str) {
  size_t pos = 0, occ = 0;
  const size_t sublen = substr.length();
  while((pos=str.find(substr,pos)) != string::npos)
    occ++, pos += sublen;
  return occ;
}

string string_lower(string str) {
  const size_t len = str.length();
  for (size_t i = 0; i < len; ++i)
    if (ldgrs[(int)(unsigned char)str[i]] & 2)
      str[i] += 32;
  return str;
}

string string_upper(string str) {
  const size_t len = str.length();
  for (size_t i = 0; i < len; ++i)
    if (ldgrs[(unsigned char)str[i]] & 1)
      str[i] -= 32;
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

//filename fucntions place here as they are just string based

string filename_name(string fname)
{
  size_t fp = fname.find_last_of("/\\");
  return fname.substr(fp+1);
}

string filename_path(string fname)
{
  size_t fp = fname.find_last_of("/\\");
  return fname.substr(0,fp+1);
}

string filename_dir(string fname)
{
  size_t fp = fname.find_last_of("/\\");
  if (fp == string::npos)
    return "";
  return fname.substr(0, fp);
}

string filename_drive(string fname)
{
  size_t fp = fname.find_first_of("/\\");
  if (!fp || fp == string::npos || fname[fp-1] != ':')
    return "";
  return fname.substr(0, fp);
}

string filename_ext(string fname)
{
  fname = filename_name(fname);
  size_t fp = fname.find_last_of(".");
  if (fp == string::npos)
    return "";
  return fname.substr(fp);
}

string filename_change_ext(string fname, string newext)
{
  size_t fp = fname.find_last_of(".");
  if (fp == string::npos)
    return fname + newext;
  return fname.replace(fp,fname.length(),newext);
}

var string_split(const std::string &str, const std::string &delim,
                 bool skip_empty) {
  var res;
  if (delim.empty()) {
    res[0] = str;
    return res;
  }
  size_t last = 0, next, found = 0;
  while ((next = str.find(delim, last)) != std::string::npos) {
    if (!skip_empty || next > last)
      res[found++] = str.substr(last, next - last);
    last = next + delim.length();
  }
  if (!skip_empty || last < str.length())
    res[found++] = str.substr(last);
  return res;
}

}
