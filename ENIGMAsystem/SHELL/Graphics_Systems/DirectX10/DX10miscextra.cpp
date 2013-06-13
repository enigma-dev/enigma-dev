/** Copyright (C) 2013 Robert B. Colton
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

#include "../General/DirectXHeaders.h"
#include <string>
#include <stdio.h>
using namespace std;

#include "DX10miscextra.h"

//Fuck whoever did this to the spec
#ifndef DX_BGR
  #define DX_BGR 0x80E0
#endif

namespace enigma_user {
extern int window_get_width();
extern int window_get_height();
}

#include "Universal_System/estring.h"

namespace enigma_user
{

int screen_save(string filename) //Assumes native integers are little endian
{

}

int screen_save_part(string filename,unsigned x,unsigned y,unsigned w,unsigned h) //Assumes native integers are little endian
{

}

}

