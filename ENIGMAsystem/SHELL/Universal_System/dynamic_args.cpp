/** Copyright (C) 2011 Josh Ventura
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
#include "dynamic_args.h"
#include <algorithm>
#include <vector>
using namespace std;
using namespace enigma;

#define v(argv) (*(vector<evariant>*)argv)

varargs& varargs::operator,(evariant x) { v(argv).push_back(x); argc++; return *this; }
void varargs::reverse() { std::reverse(v(argv).begin(),v(argv).end()); }
void varargs::sort() { std::sort(v(argv).begin(),v(argv).end()); }
evariant varargs::get(int i) const { return v(argv)[i]; }
varargs::varargs(): argc(0), argv(new vector<evariant>) {}
varargs::varargs(evariant x): argc(1), argv(new vector<evariant>) { v(argv).push_back(x); }
varargs::varargs(varargs& other): argc(other.argc), argv(new vector<evariant>) { v(argv).swap(v(other.argv)); other.argc = 0; }
varargs::~varargs() { delete (vector<evariant>*)argv; }

