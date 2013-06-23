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

#include "B3Dshapes.h"
#include "BulletShape.h"
#include <iostream>
 
vector<BulletShape*> bulletShapes;

namespace enigma_user {

int b3d_shape_create_sphere(double radius)
{
  int i = bulletShapes.size();
  BulletShape* bulletshape = new BulletShape();
  btCollisionShape* btshape = new btSphereShape(radius);
  bulletshape->colShape = btshape;
  bulletShapes.push_back(bulletshape);
  return i;
}

int b3d_shape_create_plane_static(double nx, double ny, double nz, double constant)
{
  int i = bulletShapes.size();
  BulletShape* bulletshape = new BulletShape();
  btCollisionShape* btshape = new btStaticPlaneShape(btVector3(nx,ny,nz),constant);
  bulletshape->colShape = btshape;
  bulletShapes.push_back(bulletshape);
  return i;
}

void b3d_shape_delete(int id)
{
  get_shape(bulletshape, id);
  bulletShapes.erase(bulletShapes.begin() + id);
}

void b3d_shape_calculate_local_inertia(int id, double mass, double ix, double iy, double iz)
{
  get_shape(bulletshape, id);
  btVector3 inertia(ix,iy,iz);
  bulletshape->colShape->calculateLocalInertia(mass, inertia);
}

}

