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

<<<<<<< HEAD
#include <iostream>
#include <string>

#include "Bridges/Win32-Direct3D9/DX9Context.h"
=======
>>>>>>> master
#include "Direct3D9Headers.h"

#include "DIRECTX9Std.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h" // Room dimensions.
#include "Graphics_Systems/graphics_mandatory.h" // Room dimensions.

#include <iostream>
#include <string>

using namespace std;
using namespace enigma::dx9;

namespace enigma {

bool Direct3D9Managed = true;

void graphicssystem_initialize() {}

} // namespace enigma

namespace enigma_user {

string draw_get_graphics_error()
{
<<<<<<< HEAD
  return "";
}
}
=======
  return ""; //TODO: implement
}

} // namespace enigma_user
>>>>>>> master
