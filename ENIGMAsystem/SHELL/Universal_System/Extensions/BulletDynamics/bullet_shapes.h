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

int bullet_shape_create_plane();
int bullet_shape_create_sphere(double radius);
int bullet_shape_create_plane_static(double nx, double ny, double nz, btScalar constant);
int bullet_shape_create_sphere_static();
void bullet_shape_calculate_local_inertia(int id, double mass, double ix, double iy, double iz);
void bullet_shape_delete(int id);

}
#endif
