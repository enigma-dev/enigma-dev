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

#include "Box2DWorld.h"

#include <vector>
using std::vector;

namespace enigma {
  struct extension_box2dphysics
  {
    void box2dphysics_update() {
      for (std::vector<B2DWorld*>::iterator it = b2dworlds.begin(); it != b2dworlds.end(); it++) {
        (*it)->world_update();
      }
    }
  };
}

