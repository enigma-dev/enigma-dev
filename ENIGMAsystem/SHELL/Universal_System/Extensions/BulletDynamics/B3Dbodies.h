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

#ifndef _BULLETBODIES__H
#define _BULLETBODIES__H

#include <vector>
using std::vector;

#include <btBulletDynamicsCommon.h>

#include "B3Dshapes.h"
#include "B3Dworlds.h"

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

  double getYaw()
  {
    return rigidBody->getOrientation().getAxis().getX();
  }

  double getPitch()
  {
    return rigidBody->getOrientation().getAxis().getY();
  }

  double getRoll()
  {
    return rigidBody->getOrientation().getAxis().getZ();
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

namespace enigma_user {

int b3d_bodyr_create(int sid, double mass = 0, double ix = 0, double iy = 0, double iz = 0);
void b3d_bodyr_delete(int id);
double b3d_bodyr_get_x(int id);
double b3d_bodyr_get_y(int id);
double b3d_bodyr_get_z(int id);
double b3d_bodyr_get_rot_x(int id);
double b3d_bodyr_get_rot_y(int id);
double b3d_bodyr_get_rot_z(int id);
double b3d_bodyr_get_quat_x(int id);
double b3d_bodyr_get_quat_y(int id);
double b3d_bodyr_get_quat_z(int id);
double b3d_bodyr_get_quat_w(int id);
void b3d_bodyr_set_position(int id, double x, double y, double z);
void b3d_bodyr_set_rotation(int id, double yaw, double pitch, double roll);
void b3d_bodyr_set_quaternion(int id, double qx, double qy, double qz, double qw);
void b3d_bodyr_set_massdata(int id, double mass, double ix, double iy, double iz);
void b3d_bodyr_set_linear_velocity(int id, double vx, double vy, double vz);
void b3d_bodyr_set_angular_velocity(int id, double vx, double vy, double vz);
void b3d_bodyr_apply_torque(int id, double tx, double ty, double tz);
void b3d_bodyr_apply_force(int id, double fx, double fy, double fz, double lx, double ly, double lz);
void b3d_bodyr_apply_impulse(int id, double ix, double iy, double iz, double lx, double ly, double lz);
void b3d_bodyr_apply_impulse_central(int id, double ix, double iy, double iz);
void b3d_bodyr_apply_impulse_torque(int id, double tx, double ty, double tz);

}
#endif
