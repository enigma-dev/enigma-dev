/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Josh Ventura                                             **
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

#define vararray lua_table<lua_table<variant> >
#define as_lua(x) (*((vararray*)x))

//#define as_lua(x) (*(vararray*)(&(x)))
//typedef lua_table<lua_table<variant> > vararray;

void var::initialize() {
  values = new vararray();
}
void var::cleanup() {
  if (values) { 
    delete ((vararray*)values);
    values = NULL; 
  }
}

variant& var::operator*  ()
{
  return **as_lua(values);
}
variant& var::operator() ()
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
  return as_lua(values)[size_t(ind1)][size_t(ind2)];
}

const variant& var::operator*  () const
{
  return **as_lua(values);
}
const variant& var::operator() () const
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
  return as_lua(values)[size_t(ind1)][size_t(ind2)];
}

int var::array_len() const
{
  return (*as_lua(values)).max_index();
}

int var::array_height() const
{
  return as_lua(values).max_index();
}

int var::array_len(int row) const
{
  return row>=array_height() ? 0 : as_lua(values)[size_t(row)].max_index();
}

var::var(const var& x) {
  values = new vararray();
  as_lua(values) = as_lua(x.values);
  //new(&values) vararray(*(vararray*)&(x.values));
}
var& var::operator= (const var& x) {
  if (values) {
    delete ((vararray*)values);
  }

  values = new vararray();
  as_lua(values) = as_lua(x.values);
  //*(vararray*)&(values) = *(vararray*)&(x.values);
  return *this;
}
