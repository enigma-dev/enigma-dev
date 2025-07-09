/** Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>
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
using std::string;
 
#include "../General/PFmain.h"

//TODO: Move all this shit to a general header, along with the sleep shit in Win32 and XLIB as well so platforms are consistent.

void gmw_init();

namespace enigma_user {
void sleep(int ms);

}

namespace enigma {
	extern char** parameters;
	//void writename(char* x);
    long int current_time();
}
