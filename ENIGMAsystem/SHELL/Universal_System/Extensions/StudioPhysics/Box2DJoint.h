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

#ifndef ENIGMA_BOX2D_JOINT__H
#define ENIGMA_BOX2D_JOINT__H

#include <Box2D/Box2D.h>
#include "SB2Dfunctions.h"

struct jointInstance {
  int worldid;
  b2Joint* joint;
  jointInstance()
  {
  }

  ~jointInstance()
  {
    //worlds[worldid].world->DestroyJoint(joint);
  }
};
extern vector<jointInstance> joints;

#endif // ENIGMA_BOX2D_JOINT__H

