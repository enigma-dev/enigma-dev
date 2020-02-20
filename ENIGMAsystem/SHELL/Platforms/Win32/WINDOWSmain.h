/** Copyright (C) 2011 Josh Ventura
*** Copyright (C) 2013-2014 Robert B. Colton
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

#ifndef ENIGMA_WINDOWS_MAIN
#define ENIGMA_WINDOWS_MAIN

#include "Platforms/General/PFmain.h"

void enigma_catchmouse_backend(bool x);
#define enigmacatchmouse() enigma_catchmouse_backend(enigma::mousestatus[0]==1 && enigma::last_mousestatus[0]==1)

#endif //ENIGMA_WINDOWS_MAIN
