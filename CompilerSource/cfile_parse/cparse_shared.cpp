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

#include <string>
using namespace std;

#include "../externs/externs.h"
#include "cfile_parse_constants.h"
#include "cparse_shared.h"

char skipto, skipto2, skip_inc_on;
bool skippast;

string cferr;
//string tostring(int val);

my_string cfile;
pt pos = 0;
pt len = 0;

int skip_depth;
int specialize_start;
string specialize_string;
bool specializing;
bool flag_extern;

externs *last_type = NULL;
externs *argument_type = NULL;
string last_identifier = "";
int last_named = LN_NOTHING;
int last_named_phase = 0;
long long last_value = 0;

unsigned id_would_err_at = 0;

bool cfile_debug = 0;

#include <string.h>
#include <stdio.h>

my_string::operator const char* ()
{
  return value;
}
my_string::my_string(int* e)
{
  value = (char*)e;
}
my_string::my_string()
{
  int* a = new int[2]; a[0] = a[1] = 0; value = (char*)(a + 1);
}

my_string &my_string::operator= (const my_string &x)
{
  value = x.value;
  value = x.value;
  return *this;
}

bool my_string::operator==(int a) {
  return value == NULL;
}
bool my_string::operator!=(int a) {
  return value != NULL;
}

my_string &my_string::operator= (string x)
{
  const size_t LEN = x.length();
  int *val = (int*)new char[LEN + sizeof(int) + sizeof(int)];
  
  *(val++) = LEN;
  memcpy(val,x.c_str(),LEN);
  *(int*)((char*)val + LEN) = 0;
  
  value = (char*)val;
  return *this;
}

size_t my_string::length() {
  return (-1)[(int*)value];
}
string my_string::substr(size_t x, size_t y) {
  return string(value+x,y);
}

