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
  #define get_worldr(w,id,r) \
    if (unsigned(id) >= bulletWorlds.size() || id < 0) { \
      DEBUG_MESSAGE("Cannot access Bullet Dynamics physics world with id " + toString(id), MESSAGE_TYPE::M_USER_ERROR); \
      return r; \
    } BulletWorld* w = bulletWorlds[id];
  #define get_world(w,id) \
    if (unsigned(id) >= bulletWorlds.size() || id < 0) { \
      DEBUG_MESSAGE("Cannot access Bullet Dynamics physics world with id " + toString(id), MESSAGE_TYPE::M_USER_ERROR); \
      return; \
    } BulletWorld* w = bulletWorlds[id];
  #define get_worldc(w,id, c) \
    if (unsigned(id) >= bulletWorlds.size() || id < 0) { \
      DEBUG_MESSAGE("Cannot cast Bullet Dynamics physics world with id " + toString(id), MESSAGE_TYPE::M_USER_ERROR); \
      return; \
    } c w = (c) bulletWorlds[id]->dynamicsWorld;
#else
  #define get_worldr(w,id,r) \
    BulletWorld* w = bulletWorlds[id];
  #define get_world(w,id) \
    BulletWorld* w = bulletWorlds[id];
  #define get_worldc(w,id,c) \
    c w = (c) bulletWorlds[id]->dynamicsWorld;
#endif

#ifndef ENIGMA_BULLETWORLD_H
#define ENIGMA_BULLETWORLD_H

#include <vector>
using std::vector;

#undef None
#include <BulletSoftBody/btSoftBody.h>
#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>

namespace enigma_user {

enum {
  b3d_world_unknown,
  b3d_world_softrigid,
  b3d_world_discrete,
  b3d_world_simple
};

}

struct BulletWorld {
  int type;
  btDynamicsWorld* dynamicsWorld;
  btBroadphaseInterface* broadphase;
  btDefaultCollisionConfiguration* collisionConfiguration;
  btCollisionDispatcher* dispatcher;
  btSequentialImpulseConstraintSolver* solver;

  bool paused;

  BulletWorld()
  {
    // Construct a world object, which will hold and simulate the physics bodies.
    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver;

    paused = false;
  }

  void createDiscrete()
  {
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    type = enigma_user::b3d_world_discrete;
  }

  void createSoftRigid()
  {
    dynamicsWorld = new btSoftRigidDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    type = enigma_user::b3d_world_softrigid;
  }

  void createSimple()
  {
    dynamicsWorld = new btSimpleDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    type = enigma_user::b3d_world_simple;
  }

  ~BulletWorld()
  {
    delete dynamicsWorld;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;
  }

  void update(double timestep, double iterations)
  {
    dynamicsWorld->stepSimulation(timestep, iterations);
  }
};
extern vector<BulletWorld*> bulletWorlds;

#endif
