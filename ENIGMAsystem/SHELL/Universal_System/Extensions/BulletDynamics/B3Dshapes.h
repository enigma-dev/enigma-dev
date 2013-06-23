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
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_shaper(w,id,r) \
    if (unsigned(id) >= bulletShapes.size() || id < 0) { \
      show_error("Cannot access Bullet Dynamics physics shape with id " + toString(id), false); \
      return r; \
    } BulletShape* w = bulletShapes[id];
  #define get_shape(w,id) \
    if (unsigned(id) >= bulletShapes.size() || id < 0) { \
      show_error("Cannot access Bullet Dynamics physics shape with id " + toString(id), false); \
      return; \
    } BulletShape* w = bulletShapes[id];
#else
  #define get_shaper(w,id,r) \
    BulletShape* w = bulletShapes[id];
  #define get_shape(w,id) \
    BulletShape* w = bulletShapes[id];
#endif

#ifndef _BULLETSHAPES__H
#define _BULLETSHAPES__H

#include <vector>
using std::vector;

#include <btBulletDynamicsCommon.h>

struct BulletShape {
  int worldid;
  btCollisionShape* colShape;

  BulletShape()
  {

  }

  ~BulletShape() 
  {
    delete colShape;
  }

}; 
extern vector<BulletShape*> bulletShapes;

namespace enigma_user {

int b3d_shape_create_plane();
int b3d_shape_create_sphere(double radius);
int b3d_shape_create_plane_static(double nx, double ny, double nz, btScalar constant);
int b3d_shape_create_sphere_static();
void b3d_shape_calculate_local_inertia(int id, double mass, double ix, double iy, double iz);
void b3d_shape_delete(int id);

}
#endif
