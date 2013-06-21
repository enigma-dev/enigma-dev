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

#ifndef _BULLETWORLDS__H
#define _BULLETWORLDS__H

#include <vector>
using std::vector;

#include <btBulletDynamicsCommon.h>

struct BulletWorld {
  btDynamicsWorld* dynamicsWorld;
  btBroadphaseInterface* broadphase;
  btDefaultCollisionConfiguration* collisionConfiguration;
  btCollisionDispatcher* dispatcher;
  btSequentialImpulseConstraintSolver* solver;

  bool paused;

  BulletWorld()
  {
    // Construct a world object, which will hold and simulate the rigid bodies.
    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

    paused = false;
  }

  ~BulletWorld() 
  {
    delete dynamicsWorld;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;
  }

  void Update();
}; 
extern vector<BulletWorld*> bulletWorlds;

#include "bullet_bodies.h"

namespace enigma_user {

int bullet_world_discrete_create();
int bullet_world_simple_create();
int bullet_world_softbody_create();
int bullet_world_collision_create();
void bullet_world_delete(int id);
void bullet_world_set_gravity(int id, double gx, double gy, double gz);
void bullet_world_add_body(int id, int body);
void bullet_world_remove_body(int id, int body);
void bullet_world_step_simulation(int id, double timestep, double iterations); 

}
#endif
