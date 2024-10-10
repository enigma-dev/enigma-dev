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

#include "B3Dbodies.h"
#include "BulletRigidBody.h"
#include <iostream>

vector<BulletBody*> bulletBodies;

namespace enigma_user {

int b3d_bodyr_create(int sid, double mass, double ix, double iy, double iz, double friction, double restitution)
{
  int i = bulletBodies.size();
  get_shaper(bulletshape, sid, -1);
  bulletBodies.push_back(new BulletBody(sid, mass, ix, iy, iz, friction, restitution));
  return i;
}

void b3d_bodyr_delete(int id)
{
  get_body(bulletbody, id);
  bulletBodies.erase(bulletBodies.begin() + id);
}

double b3d_bodyr_get_x(int id)
{
  get_bodyr(bulletbody, id, -1);
  return bulletbody->getX();
}

double b3d_bodyr_get_y(int id)
{
  get_bodyr(bulletbody, id, -1);
  return bulletbody->getY();
}

double b3d_bodyr_get_z(int id)
{
  get_bodyr(bulletbody, id, -1);
  return bulletbody->getZ();
}

double b3d_bodyr_get_rot_x(int id)
{
  get_bodyr(bulletbody, id, -1);
  return bulletbody->getRoll();
}

double b3d_bodyr_get_rot_y(int id)
{
  get_bodyr(bulletbody, id, -1);
  return bulletbody->getPitch();
}

double b3d_bodyr_get_rot_z(int id)
{
  get_bodyr(bulletbody, id, -1);
  return bulletbody->getYaw();
}

double b3d_bodyr_get_rot_ang(int id)
{
  get_bodyr(bulletbody, id, -1);
  return bulletbody->getAngle();
}

double b3d_bodyr_get_quat_x(int id)
{
  get_bodyr(bulletbody, id, -1);
  return bulletbody->getQuatX();
}

double b3d_bodyr_get_quat_y(int id)
{
  get_bodyr(bulletbody, id, -1);
  return bulletbody->getQuatY();
}

double b3d_bodyr_get_quat_z(int id)
{
  get_bodyr(bulletbody, id, -1);
  return bulletbody->getQuatZ();
}

double b3d_bodyr_get_quat_w(int id)
{
  get_bodyr(bulletbody, id, -1);
  return bulletbody->getQuatW();
}

void b3d_bodyr_set_position(int id, double x, double y, double z)
{
  get_body(bulletbody, id);
  bulletbody->setPosition(x, y, z);
}

void b3d_bodyr_set_rotation(int id, double yaw, double pitch, double roll)
{
  get_body(bulletbody, id);
  bulletbody->setRotation(yaw, pitch, roll);
}

void b3d_bodyr_set_quaternion(int id, double qx, double qy, double qz, double qw)
{
  get_body(bulletbody, id);
  bulletbody->setQuaternion(qx, qy, qz, qw);
}

void b3d_bodyr_set_linear_velocity(int id, double vx, double vy, double vz)
{
  get_body(bulletbody, id);
  bulletbody->rigidBody->setLinearVelocity(btVector3(vx, vy, vz));
}

void b3d_bodyr_set_angular_velocity(int id, double vx, double vy, double vz)
{
  get_body(bulletbody, id);
  bulletbody->rigidBody->setAngularVelocity(btVector3(vx, vy, vz));
}

void b3d_bodyr_apply_torque(int id, double tx, double ty, double tz)
{
  get_body(bulletbody, id);
  bulletbody->rigidBody->applyTorque(btVector3(tx, ty, tz));
}

void b3d_bodyr_apply_force(int id, double fx, double fy, double fz, double lx, double ly, double lz)
{
  get_body(bulletbody, id);
  bulletbody->rigidBody->applyForce(btVector3(fx, fy, fz), btVector3(lx, ly, lz));
}

void b3d_bodyr_apply_impulse(int id, double ix, double iy, double iz, double lx, double ly, double lz)
{
  get_body(bulletbody, id);
  bulletbody->rigidBody->applyImpulse(btVector3(ix, iy, iz), btVector3(lx, ly, lz));
}

void b3d_bodyr_apply_impulse_central(int id, double ix, double iy, double iz)
{
  get_body(bulletbody, id);
  bulletbody->rigidBody->applyCentralImpulse(btVector3(ix, iy, iz));
}

void b3d_bodyr_apply_impulse_torque(int id, double tx, double ty, double tz)
{
  get_body(bulletbody, id);
  bulletbody->rigidBody->applyTorqueImpulse(btVector3(tx, ty, tz));
}

}

