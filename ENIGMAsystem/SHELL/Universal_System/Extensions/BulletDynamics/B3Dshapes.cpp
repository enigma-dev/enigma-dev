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

int b3d_shape_create()
{
  int i = bulletShapes.size();
  BulletShape* bulletshape = new BulletShape();
  bulletShapes.push_back(bulletshape);
  return i;
}

void b3d_shape_delete(int id)
{
  get_shape(bulletshape, id);
  bulletShapes.erase(bulletShapes.begin() + id);
}

int b3d_shape_create_box(double halfwidth, double halflength, double halfheight)
{
  int i = b3d_shape_create();
  b3d_shape_box(i, halfwidth, halflength, halfheight);
  return i;
}

int b3d_shape_create_cylinder(double halfwidth, double halflength, double halfheight)
{
  int i = b3d_shape_create();
  b3d_shape_cylinder(i, halfwidth, halflength, halfheight);
  return i;
}

int b3d_shape_create_capsule(double radius, double height)
{
  int i = b3d_shape_create();
  b3d_shape_capsule(i, radius, height);
  return i;
}

int b3d_shape_create_cone(double radius, double height)
{
  int i = b3d_shape_create();
  b3d_shape_cone(i, radius, height);
  return i;
}

int b3d_shape_create_sphere(double radius)
{
  int i = b3d_shape_create();
  b3d_shape_sphere(i, radius);
  return i;
}

int b3d_shape_create_plane_static(double nx, double ny, double nz, double constant)
{
  int i = b3d_shape_create();
  b3d_shape_plane_static(i, nx, ny, nz, constant);
  return i;
}

int b3d_shape_create_empty()
{
  int i = b3d_shape_create();
  b3d_shape_empty(i);
  return i;
}

void b3d_shape_box(int id, double halfwidth, double halflength, double halfheight)
{
  get_shape(bulletshape, id);
  btCollisionShape* btshape = new btBoxShape(btVector3(halfwidth, halflength, halfheight));
  bulletshape->beginShape();
  bulletshape->colShape = btshape;
}

void b3d_shape_cylinder(int id, double halfwidth, double halflength, double halfheight)
{
  get_shape(bulletshape, id);
  btCollisionShape* btshape = new btCylinderShape(btVector3(halfwidth, halflength, halfheight));
  bulletshape->beginShape();
  bulletshape->colShape = btshape;
}

void b3d_shape_capsule(int id, double radius, double height)
{
  get_shape(bulletshape, id);
  btCollisionShape* btshape = new btCapsuleShape(radius, height);
  bulletshape->beginShape();
  bulletshape->colShape = btshape;
}

void b3d_shape_cone(int id, double radius, double height)
{
  get_shape(bulletshape, id);
  btCollisionShape* btshape = new btConeShape(radius, height);
  bulletshape->beginShape();
  bulletshape->colShape = btshape;
}

void b3d_shape_sphere(int id, double radius)
{
  get_shape(bulletshape, id);
  btCollisionShape* btshape = new btSphereShape(radius);
  bulletshape->beginShape();
  bulletshape->colShape = btshape;
}

void b3d_shape_plane_static(int id, double nx, double ny, double nz, double constant)
{
  get_shape(bulletshape, id);
  btCollisionShape* btshape = new btStaticPlaneShape(btVector3(nx,ny,nz),constant);
  bulletshape->beginShape();
  bulletshape->colShape = btshape;
}

void b3d_shape_empty(int id)
{
  get_shape(bulletshape, id);
  btCollisionShape* btshape = new btEmptyShape();
  bulletshape->beginShape();
  bulletshape->colShape = btshape;
}

void b3d_shape_calculate_local_inertia(int id, double mass, double ix, double iy, double iz)
{
  get_shape(bulletshape, id);
  btVector3 inertia(ix,iy,iz);
  bulletshape->colShape->calculateLocalInertia(mass, inertia);
}

void b3d_shape_set_scale(int id, double sx, double sy, double sz)
{
  get_shape(bulletshape, id);
  bulletshape->colShape->setLocalScaling(btVector3(sx, sy, sz));
}

double b3d_shape_get_scale_x(int id)
{
  get_shaper(bulletshape, id, -1);
  return bulletshape->colShape->getLocalScaling().getX();
}

double b3d_shape_get_scale_y(int id)
{
  get_shaper(bulletshape, id, -1);
  return bulletshape->colShape->getLocalScaling().getY();
}

double b3d_shape_get_scale_z(int id)
{
  get_shaper(bulletshape, id, -1);
  return bulletshape->colShape->getLocalScaling().getZ();
}

void b3d_shape_set_margin(int id, double margin)
{
  get_shape(bulletshape, id);
  return bulletshape->colShape->setMargin(margin);
}

double b3d_shape_get_margin(int id)
{
  get_shaper(bulletshape, id, -1);
  return bulletshape->colShape->getMargin();
}


}

