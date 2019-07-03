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

#ifndef ENIGMA_BOX2D_JOINT_H
#define ENIGMA_BOX2D_JOINT_H

#include <Box2D/Box2D.h>
#include "Box2DWorld.h"

struct B2DJoint {
  int worldid;
  b2Joint* joint;

  B2DJoint()
  {
  }

  ~B2DJoint()
  {
    b2dworlds[worldid]->world->DestroyJoint(joint);
	joint = NULL;
  }
};
extern vector<B2DJoint*> b2djoints;

#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_jointr(j,id,r) \
    if (unsigned(id) >= b2djoints.size() || id < 0) { \
      DEBUG_MESSAGE("Cannot access Box2D physics joint with id " + toString(id), MESSAGE_TYPE::M_USER_ERROR); \
      return r; \
    } B2DJoint* j = b2djoints[id];
  #define get_joint(j,id) \
    if (unsigned(id) >= b2djoints.size() || id < 0) { \
      DEBUG_MESSAGE("Cannot access Box2D physics joint with id " + toString(id), MESSAGE_TYPE::M_USER_ERROR); \
      return; \
    } B2DJoint* j = b2djoints[id];
#else
  #define get_jointr(j,id,r) \
    B2DJoint* j = b2djoints[id];
  #define get_joint(j,id) \
    B2DJoint* j = b2djoints[id];
#endif

#endif // ENIGMA_BOX2D_JOINT_H
