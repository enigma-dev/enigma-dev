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

#ifdef DEBUG_MODE

/*
#ifndef stringinclude
#define stringinclude
#include <string>
using std::string;
#endif

  #include "libEGMstd.h"
*/
#define toString(s) s

#include "Widget_Systems/widgets_mandatory.h"
#define get_shaper(s, id, r)                                                                  \
  if (unsigned(id) >= bulletShapes.size() || id < 0) {                                        \
    show_error("Cannot access Bullet Dynamics physics shape with id " + toString(id), false); \
    return r;                                                                                 \
  }                                                                                           \
  BulletShape* s = bulletShapes[id];
#define get_shape(s, id)                                                                      \
  if (unsigned(id) >= bulletShapes.size() || id < 0) {                                        \
    show_error("Cannot access Bullet Dynamics physics shape with id " + toString(id), false); \
    return;                                                                                   \
  }                                                                                           \
  BulletShape* s = bulletShapes[id];
#else
#define get_shaper(s, id, r) BulletShape* s = bulletShapes[id];
#define get_shape(s, id) BulletShape* s = bulletShapes[id];
#endif

#ifndef _BULLETSHAPE__H
#define _BULLETSHAPE__H

#include <vector>
using std::vector;

#include <btBulletDynamicsCommon.h>

struct BulletShape {
  int type;
  int worldid;
  bool shapebuilt;
  btCollisionShape* colShape;
  vector<btVector3> vertices;

  BulletShape() { shapebuilt = false; }

  ~BulletShape() { delete colShape; }

  void beginShape() {
    if (shapebuilt) {
      delete colShape;
    } else {
      shapebuilt = true;
    }
  }
};
extern vector<BulletShape*> bulletShapes;

#endif
