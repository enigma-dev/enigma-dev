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

#include "bullet_shapes.h"
#include <iostream>
 
vector<BulletShape*> bulletShapes;

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

namespace enigma_user {

int bullet_shape_create_sphere(double radius)
{
  int i = bulletShapes.size();
  BulletShape* bulletshape = new BulletShape();
  btCollisionShape* btshape = new btSphereShape(radius);
  bulletshape->colShape = btshape;
  bulletShapes.push_back(bulletshape);
  return i;
}

int bullet_shape_create_plane_static(double nx, double ny, double nz, btScalar constant)
{
  int i = bulletShapes.size();
  BulletShape* bulletshape = new BulletShape();
  btCollisionShape* btshape = new btStaticPlaneShape(btVector3(nx,ny,nz),constant);
  bulletshape->colShape = btshape;
  bulletShapes.push_back(bulletshape);
  return i;
}

void bullet_shape_delete(int id)
{
  bulletShapes.erase(bulletShapes.begin() + id);
}

void bullet_shape_calculate_local_inertia(int id, double mass, double ix, double iy, double iz)
{
  get_shape(bulletshape, id);
  btVector3 inertia(ix,iy,iz);
  bulletshape->colShape->calculateLocalInertia(mass, inertia);
}

}

