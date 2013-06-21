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

#include "bullet_worlds.h"
#include <iostream>
 
#ifdef DEBUG_MODE
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_worldr(w,id,r) \
    if (unsigned(id) >= bulletWorlds.size() || id < 0) { \
      show_error("Cannot access Bullet Dynamics physics world with id " + toString(id), false); \
      return r; \
    } BulletWorld* w = bulletWorlds[id];
  #define get_world(w,id) \
    if (unsigned(id) >= bulletWorlds.size() || id < 0) { \
      show_error("Cannot access Bullet Dynamics physics world with id " + toString(id), false); \
      return; \
    } BulletWorld* w = bulletWorlds[id];
#else
  #define get_worldr(w,id,r) \
    BulletWorld* w = bulletWorlds[id];
  #define get_world(w,id) \
    BulletWorld* w = bulletWorlds[id];
#endif

vector<BulletWorld*> bulletWorlds;

namespace enigma_user {

int bullet_world_discrete_create()
{
  int i = bulletWorlds.size();
  bulletWorlds.push_back(new BulletWorld());
  return i;
}

void bullet_world_delete(int id)
{
  bulletWorlds.erase(bulletWorlds.begin() + id);
}

void bullet_world_set_gravity(int id, double gx, double gy, double gz)
{
  get_world(bulletworld, id);
  bulletworld->dynamicsWorld->setGravity(btVector3(gx,gy,gz));
}

void bullet_world_add_body(int id, int body) 
{
  get_world(bulletworld, id);
  bulletworld->dynamicsWorld->addRigidBody(bulletBodies[body]->rigidBody);
}

void bullet_world_step_simulation(int id, double timestep, double iterations) 
{
  get_world(bulletworld, id);
  bulletworld->dynamicsWorld->stepSimulation(timestep, iterations);
}

void bullet_world_remove_body(int id, int body)
{
  get_world(bulletworld, id);
  bulletworld->dynamicsWorld->removeRigidBody(bulletBodies[body]->rigidBody);
}

}

