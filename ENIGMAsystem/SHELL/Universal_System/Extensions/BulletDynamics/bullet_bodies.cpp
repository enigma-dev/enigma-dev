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

#include "bullet_bodies.h"
#include <iostream>

vector<BulletBody*> bulletBodies;

#ifdef DEBUG_MODE
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_bodyr(w,id,r) \
    if (unsigned(id) >= bulletBodies.size() || id < 0) { \
      show_error("Cannot access Bullet Dynamics physics body with id " + toString(id), false); \
      return r; \
    } BulletBody* w = bulletBodies[id];
  #define get_body(w,id) \
    if (unsigned(id) >= bulletBodies.size() || id < 0) { \
      show_error("Cannot access Bullet Dynamics physics body with id " + toString(id), false); \
      return; \
    } BulletBody* w = bulletBodies[id];
#else
  #define get_bodyr(w,id,r) \
    BulletBody* w = bulletBodies[id];
  #define get_body(w,id) \
    BulletBody* w = bulletBodies[id];
#endif

namespace enigma_user {

int bullet_body_create(int sid, double mass, double ix, double iy, double iz)
{
  int i = bulletBodies.size();
  bulletBodies.push_back(new BulletBody(sid, mass, ix, iy, iz));
  return i;
}

void bullet_body_delete(int id)
{
  bulletBodies.erase(bulletBodies.begin() + id);
}

double bullet_body_get_x(int id)
{
  get_bodyr(bulletbody, id, -1);
  return bulletbody->getX();
}

double bullet_body_get_y(int id)
{
  get_bodyr(bulletbody, id, -1);
  return bulletbody->getY();
}

double bullet_body_get_z(int id)
{
  get_bodyr(bulletbody, id, -1);
  return bulletbody->getZ();
}

double bullet_body_get_rot_x(int id)
{
  get_bodyr(bulletbody, id, -1);
  return bulletbody->getRotX();
}

double bullet_body_get_rot_y(int id)
{
  get_bodyr(bulletbody, id, -1);
  return bulletbody->getRotY();
}

double bullet_body_get_rot_z(int id)
{
  get_bodyr(bulletbody, id, -1);
  return bulletbody->getRotZ();
}

double bullet_body_get_rot_w(int id)
{
  get_bodyr(bulletbody, id, -1);
  return bulletbody->getRotW();
}

void bullet_body_set_position(int id, double x, double y, double z)
{
  get_body(bulletbody, id);
  bulletbody->setPosition(x, y, z);
}

void bullet_body_set_rotation(int id, double rx, double ry, double rz, double rw)
{
  get_body(bulletbody, id);
  bulletbody->setRotation(rx, ry, rz, rw);
}

void bullet_body_set_linear_velocity(int id, double vx, double vy, double vz)
{
  get_body(bulletbody, id);
  bulletbody->rigidBody->setLinearVelocity(btVector3(vx, vy, vz));
}

void bullet_body_set_angular_velocity(int id, double vx, double vy, double vz)
{
  get_body(bulletbody, id);
  bulletbody->rigidBody->setAngularVelocity(btVector3(vx, vy, vz));
}

void bullet_body_apply_torque(int id, double tx, double ty, double tz)
{
  get_body(bulletbody, id);
  bulletbody->rigidBody->applyTorque(btVector3(tx, ty, tz));
}

void bullet_body_apply_force(int id, double fx, double fy, double fz, double lx, double ly, double lz)
{
  get_body(bulletbody, id);
  bulletbody->rigidBody->applyForce(btVector3(fx, fy, fz), btVector3(lx, ly, lz));
}

void bullet_body_apply_impulse(int id, double ix, double iy, double iz, double lx, double ly, double lz)
{
  get_body(bulletbody, id);
  bulletbody->rigidBody->applyImpulse(btVector3(ix, iy, iz), btVector3(lx, ly, lz));
}

void bullet_body_apply_impulse_central(int id, double ix, double iy, double iz)
{
  get_body(bulletbody, id);
  bulletbody->rigidBody->applyCentralImpulse(btVector3(ix, iy, iz));
}

void bullet_body_apply_impulse_torque(int id, double tx, double ty, double tz)
{
  get_body(bulletbody, id);
  bulletbody->rigidBody->applyTorqueImpulse(btVector3(tx, ty, tz));
}

}

