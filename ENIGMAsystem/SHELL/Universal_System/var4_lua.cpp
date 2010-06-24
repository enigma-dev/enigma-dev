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
using std::string;

#include "var4.h"      // Var stuff
#include "lua_table.h" // The Lua part

#define as_lua(x) (*(vararray*)(&(x)))
#define vararray lua_table<lua_table<variant> >

void var::initialize() {
  new(&values) vararray;
}
void var::cleanup() {
  as_lua(values) . ~vararray();
}

variant& var::operator*  ()
{
  return **as_lua(values);
}
variant& var::operator[] (int ind)
{
  return (*as_lua(values))[size_t(ind)];
}
variant& var::operator() (int ind)
{
  return (*as_lua(values))[size_t(ind)];
}
variant& var::operator() (int ind1,int ind2)
{
  return as_lua(values)[size_t(ind2)][size_t(ind1)];
}

const variant& var::operator*  () const
{
  return **as_lua(values);
}
const variant& var::operator[] (int ind) const
{
  return (*as_lua(values))[size_t(ind)];
}
const variant& var::operator() (int ind) const
{
  return (*as_lua(values))[size_t(ind)];
}
const variant& var::operator() (int ind1,int ind2) const
{
  return as_lua(values)[size_t(ind2)][size_t(ind1)];
}

var::var(const var& x) {
  new(&values) vararray(*(vararray*)&(x.values));
}
var& var::operator= (const var& x) {
  *(vararray*)&(values) = *(vararray*)&(x.values);
  return *this;
}
