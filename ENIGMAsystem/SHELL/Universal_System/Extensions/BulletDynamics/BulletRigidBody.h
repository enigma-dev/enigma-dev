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
  #include "Widget_Systems/widgets_mandatory.h"
  #include "libEGMstd.h"
  #define get_bodyr(w,id,r) \
    if (unsigned(id) >= bulletBodies.size() || id < 0) { \
      enigma_user::show_error("Cannot access Bullet Dynamics physics body with id " + toString(id), false); \
      return r; \
    } BulletBody* w = bulletBodies[id];
  #define get_body(w,id) \
    if (unsigned(id) >= bulletBodies.size() || id < 0) { \
      enigma_user::show_error("Cannot access Bullet Dynamics physics body with id " + toString(id), false); \
      return; \
    } BulletBody* w = bulletBodies[id];
#else
  #define get_bodyr(w,id,r) \
    BulletBody* w = bulletBodies[id];
  #define get_body(w,id) \
    BulletBody* w = bulletBodies[id];
#endif

#ifndef ENIGMA_BULLETRIGIDBODY_H
#define ENIGMA_BULLETRIGIDBODY_H

#include <vector>
using std::vector;

#include <btBulletDynamicsCommon.h>

#include "BulletShape.h"
#include "BulletWorld.h"

struct BulletBody {
  int worldid;
  int shapeid;
  btRigidBody* rigidBody;

  BulletBody(int sid, double mass, double ix, double iy, double iz, double friction, double restitution)
  {
    btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
    shapeid = sid;
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass,motionstate,bulletShapes[shapeid]->colShape,btVector3(ix,iy,iz));
    rigidBodyCI.m_restitution = restitution;
    rigidBodyCI.m_friction = friction;
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
    btTransform trans = rigidBody->getWorldTransform();
    return trans.getOrigin().getZ();
  }

  void setPosition(double x, double y, double z)
  {
    btTransform trans = rigidBody->getWorldTransform();
    trans.setOrigin(btVector3(x, y, z));
    rigidBody->setWorldTransform(trans);
  }

  void setQuaternion(double qx, double qy, double qz, double qw)
  {
    btTransform trans = rigidBody->getWorldTransform();
    //trans.setIdentity();
    trans.setRotation(btQuaternion(qx, qy, qz, qw));
    rigidBody->setWorldTransform(trans);

//rigidBody->setCenterOfMassTransform(trans);
  }

  void setRotation(double yaw, double pitch, double roll)
  {
    btTransform trans = rigidBody->getWorldTransform();
    //trans.setIdentity();
    btQuaternion quat;
    quat.setEuler(yaw, pitch, roll);
    trans.setRotation(quat);
    rigidBody->setWorldTransform(trans);

//rigidBody->setCenterOfMassTransform(trans);
  }

  double getRoll()
  {
    return rigidBody->getOrientation().getAxis().getX();
  }

  double getPitch()
  {
    return rigidBody->getOrientation().getAxis().getY();
  }

  double getYaw()
  {
    return rigidBody->getOrientation().getAxis().getZ();
  }

  double getAngle()
  {
    return rigidBody->getOrientation().getAngle();
  }

  double getQuatX()
  {
    return rigidBody->getOrientation().getX();
  }

  double getQuatY()
  {
    return rigidBody->getOrientation().getY();
  }

  double getQuatZ()
  {
    return rigidBody->getOrientation().getZ();
  }

  double getQuatW()
  {
    return rigidBody->getOrientation().getW();
  }

};
extern vector<BulletBody*> bulletBodies;

#endif
