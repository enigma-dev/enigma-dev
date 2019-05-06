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
  #define get_softbodyr(w,id,r) \
    if (unsigned(id) >= bulletSoftBodies.size() || id < 0) { \
      enigma_user::show_error("Cannot access Bullet Dynamics physics body with id " + toString(id), false); \
      return r; \
    } BulletSoftBody* w = bulletSoftBodies[id];
  #define get_softbody(w,id) \
    if (unsigned(id) >= bulletSoftBodies.size() || id < 0) { \
      enigma_user::show_error("Cannot access Bullet Dynamics physics body with id " + toString(id), false); \
      return; \
    } BulletSoftBody* w = bulletSoftBodies[id];
#else
  #define get_softbodyr(w,id,r) \
    BulletSoftBody* w = bulletSoftBodies[id];
  #define get_softbody(w,id) \
    BulletSoftBody* w = bulletSoftBodies[id];
#endif

#ifndef ENIGMA_BULLETSOFTBODY_H
#define ENIGMA_BULLETSOFTBODY_H

#include <vector>
using std::vector;

#include <BulletSoftBody/btSoftBody.h>
#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>

#include "BulletWorld.h"
//#include "BulletShape.h"

struct BulletSoftBody {
  int worldid;
  int shapeid;
  btSoftBody* softBody;

  BulletSoftBody(int sid, double airdensity, double waterdensity, double wateroffset, int nodecount, int m)
  {
    shapeid = sid;
    btSoftBodyWorldInfo* softBodyWI = new btSoftBodyWorldInfo();
    softBodyWI->air_density = airdensity;
    softBodyWI->water_density = waterdensity;
    softBodyWI->water_offset = wateroffset;
    const btVector3* test = new btVector3(0, 0, 0);
    softBody = new btSoftBody(softBodyWI, nodecount, test, new btScalar(5));
  }

  ~BulletSoftBody()
  {
    btSoftRigidDynamicsWorld* dynamicsWorld = (btSoftRigidDynamicsWorld*) bulletWorlds[worldid]->dynamicsWorld;
    dynamicsWorld->removeSoftBody(softBody);
    delete softBody;
  }

  double getX()
  {
    btTransform trans = softBody->getWorldTransform();
    return trans.getOrigin().getX();
  }

  double getY()
  {
    btTransform trans = softBody->getWorldTransform();
    return trans.getOrigin().getY();
  }

  double getZ()
  {
    btTransform trans = softBody->getWorldTransform();
    return trans.getOrigin().getZ();
  }

  void setPosition(double x, double y, double z)
  {
    btTransform trans = softBody->getWorldTransform();
    trans.setOrigin(btVector3(x, y, z));
    softBody->setWorldTransform(trans);
  }

  void setQuaternion(double qx, double qy, double qz, double qw)
  {
    btTransform trans = softBody->getWorldTransform();
    //trans.setIdentity();
    trans.setRotation(btQuaternion(qx, qy, qz, qw));
    softBody->setWorldTransform(trans);

//softBody->setCenterOfMassTransform(trans);
  }

  void setRotation(double yaw, double pitch, double roll)
  {
    btTransform trans = softBody->getWorldTransform();
    //trans.setIdentity();
    btQuaternion quat;
    quat.setEuler(yaw, pitch, roll);
    trans.setRotation(quat);
    softBody->setWorldTransform(trans);

//softBody->setCenterOfMassTransform(trans);
  }

  double getYaw()
  {
    return softBody->getWorldTransform().getRotation().getAxis().getX();
  }

  double getPitch()
  {
    return softBody->getWorldTransform().getRotation().getAxis().getY();
  }

  double getRoll()
  {
    return softBody->getWorldTransform().getRotation().getAxis().getZ();
  }

  double getQuatX()
  {
    return softBody->getWorldTransform().getRotation().getX();
  }

  double getQuatY()
  {
    return softBody->getWorldTransform().getRotation().getY();
  }

  double getQuatZ()
  {
    return softBody->getWorldTransform().getRotation().getZ();
  }

  double getQuatW()
  {
    return softBody->getWorldTransform().getRotation().getW();
  }

};
extern vector<BulletSoftBody*> bulletSoftBodies;

#endif
