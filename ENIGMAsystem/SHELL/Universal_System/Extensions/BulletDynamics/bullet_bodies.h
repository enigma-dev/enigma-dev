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

#ifndef _BULLETBODIES__H
#define _BULLETBODIES__H

#include <vector>
using std::vector;

#include <btBulletDynamicsCommon.h>

#include "bullet_shapes.h"
#include "bullet_worlds.h"

struct BulletBody {
  int worldid;
  int shapeid;
  btRigidBody* rigidBody;

  BulletBody(int sid, double mass, double ix, double iy, double iz)
  {
    btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
    shapeid = sid;
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass,motionstate,bulletShapes[shapeid]->colShape,btVector3(ix,iy,iz));
    rigidBody = new btRigidBody(rigidBodyCI);
  }

  ~BulletBody() 
  {
    bulletWorlds[worldid]->dynamicsWorld->removeRigidBody(rigidBody);
    delete rigidBody->getMotionState();
    delete rigidBody;
  }

  double getX() 
  {
    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);
    return trans.getOrigin().getX();
  }

  double getY() 
  {
    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);
    return trans.getOrigin().getY();
  }
  
  double getZ()
  {
    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);
    return trans.getOrigin().getZ();
  }

  void setPosition(double x, double y, double z)
  {
    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);
    trans.setOrigin(btVector3(x, y, z));
    rigidBody->setWorldTransform(trans);
  }

  void setRotation(double rx, double ry, double rz, double rw)
  {
    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);
    trans.setRotation(btQuaternion(rx, ry, rz, rw));
    rigidBody->setWorldTransform(trans);
  }

  double getRotX()
  {
    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);
    return rigidBody->getOrientation().getX();
  }

  double getRotY()
  {
    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);
    return rigidBody->getOrientation().getY();
  }

  double getRotZ()
  {
    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);
    return rigidBody->getOrientation().getZ();
  }

  double getRotW()
  {
    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);
    return rigidBody->getOrientation().getW();
  }

}; 
extern vector<BulletBody*> bulletBodies;

namespace enigma_user {

int bullet_body_create(int sid, double mass = 0, double ix = 0, double iy = 0, double iz = 0);
void bullet_body_delete(int id);
double bullet_body_get_x(int id);
double bullet_body_get_y(int id);
double bullet_body_get_z(int id);
double bullet_body_get_rot_x(int id);
double bullet_body_get_rot_y(int id);
double bullet_body_get_rot_z(int id);
double bullet_body_get_rot_w(int id);
void bullet_body_set_position(int id, double x, double y, double z);
void bullet_body_set_rotation(int id, double rx, double ry, double rz, double rw);
void bullet_body_set_massdata(int id, double mass, double ix, double iy, double iz);
void bullet_body_set_linear_velocity(int id, double vx, double vy, double vz);
void bullet_body_set_angular_velocity(int id, double vx, double vy, double vz);
void bullet_body_apply_torque(int id, double tx, double ty, double tz);
void bullet_body_apply_force(int id, double fx, double fy, double fz, double lx, double ly, double lz);
void bullet_body_apply_impulse(int id, double ix, double iy, double iz, double lx, double ly, double lz);
void bullet_body_apply_impulse_central(int id, double ix, double iy, double iz);
void bullet_body_apply_impulse_torque(int id, double tx, double ty, double tz);

}
#endif
