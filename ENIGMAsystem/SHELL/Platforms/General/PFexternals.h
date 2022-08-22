/** Copyright (C) 2008 Josh Ventura
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

#include "Universal_System/var4.h"

namespace enigma_user
{

static int dll_cdecl   = 0;
static int dll_stdcall = 1;

int external_define(std::string lib,std::string func,int calltype,bool returntype,int argcount,
                    bool t01=0,bool t02=0,bool t03=0,bool t04=0,bool t05=0,bool t06=0,bool t07=0,bool t08=0,
                    bool t09=0,bool t10=0,bool t11=0,bool t12=0,bool t13=0,bool t14=0,bool t15=0,bool t16=0);

evariant external_call(int id,evariant a1=0,evariant a2=0,evariant a3=0,evariant a4=0,evariant a5=0,evariant a6=0,evariant a7=0,evariant a8=0,
                             evariant a9=0,evariant a10=0,evariant a11=0,evariant a12=0,evariant a13=0,evariant a14=0,evariant a15=0,evariant a16=0);

void external_free(std::string lib);

}

