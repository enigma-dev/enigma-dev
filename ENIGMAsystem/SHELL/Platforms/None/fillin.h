/** Copyright (C) 2017 Faissal I. Bensefia
*** Copyright (C) 2008-2011 IsmAvatar <cmagicj@nni.com>, Josh Ventura
*** Copyright (C) 2013 Robert B. Colton
*** Copyright (C) 2014 Seth N. Hetu
*** Copyright (C) 2015 Harijs Grinbergs
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

namespace enigma {
  void input_push();
}

#include "../General/PFwindow.h"
#include "../General/PFmain.h"

#include <string>
using std::string;

void Sleep(int ms);

namespace enigma_user {
  static inline void sleep(int ms) { Sleep(ms); }
}

namespace enigma {
  extern string* parameters;
  extern int parameterc;
  extern int current_room_speed;
  //void writename(char* x);
}
