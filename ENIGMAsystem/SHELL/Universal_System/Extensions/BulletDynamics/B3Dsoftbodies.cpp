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

#include "B3Dsoftbodies.h"
#include "BulletSoftBody.h"
#include "BulletShape.h"
#include <iostream>

vector<BulletSoftBody*> bulletSoftBodies;

namespace enigma_user {

int b3d_bodys_create(int sid, double mass, double ix, double iy, double iz)
{
  int i = bulletSoftBodies.size();
  get_shaper(bulletshape, sid, -1);
  bulletSoftBodies.push_back(new BulletSoftBody(sid, 5, 5, 5, 5, 5 ));
  return i;
}

void b3d_bodys_delete(int id)
{
  get_softbody(bulletsoftbody, id);
  bulletSoftBodies.erase(bulletSoftBodies.begin() + id);
}

double b3d_bodys_get_x(int id)
{
  get_softbodyr(bulletsoftbody, id, -1);
  return bulletsoftbody->getX();
}

double b3d_bodys_get_y(int id)
{
  get_softbodyr(bulletsoftbody, id, -1);
  return bulletsoftbody->getY();
}

double b3d_bodys_get_z(int id)
{
  get_softbodyr(bulletsoftbody, id, -1);
  return bulletsoftbody->getZ();
}

double b3d_bodys_get_rot_x(int id)
{
  get_softbodyr(bulletsoftbody, id, -1);
  return bulletsoftbody->getYaw();
}

double b3d_bodys_get_rot_y(int id)
{
  get_softbodyr(bulletsoftbody, id, -1);
  return bulletsoftbody->getPitch();
}

double b3d_bodys_get_rot_z(int id)
{
  get_softbodyr(bulletsoftbody, id, -1);
  return bulletsoftbody->getRoll();
}

double b3d_bodys_get_quat_x(int id)
{
  get_softbodyr(bulletsoftbody, id, -1);
  return bulletsoftbody->getQuatX();
}

double b3d_bodys_get_quat_y(int id)
{
  get_softbodyr(bulletsoftbody, id, -1);
  return bulletsoftbody->getQuatY();
}

double b3d_bodys_get_quat_z(int id)
{
  get_softbodyr(bulletsoftbody, id, -1);
  return bulletsoftbody->getQuatZ();
}

double b3d_bodys_get_quat_w(int id)
{
  get_softbodyr(bulletsoftbody, id, -1);
  return bulletsoftbody->getQuatW();
}

void b3d_bodys_set_position(int id, double x, double y, double z)
{
  get_softbody(bulletsoftbody, id);
  bulletsoftbody->setPosition(x, y, z);
}

void b3d_bodys_set_rotation(int id, double yaw, double pitch, double roll)
{
  get_softbody(bulletsoftbody, id);
  bulletsoftbody->setRotation(yaw, pitch, roll);
}

void b3d_bodys_set_quaternion(int id, double qx, double qy, double qz, double qw)
{
  get_softbody(bulletsoftbody, id);
  bulletsoftbody->setQuaternion(qx, qy, qz, qw);
}

}

