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

#include "B3Dworlds.h"
#include "BulletRigidBody.h"
#include "BulletSoftBody.h"
#include <iostream>

vector<BulletWorld*> bulletWorlds;

namespace enigma_user {

int b3d_world_create_softrigid()
{
  int i = bulletWorlds.size();
  BulletWorld* bulletworld = new BulletWorld();
  bulletworld->createSoftRigid();
  bulletWorlds.push_back(bulletworld);
  return i;
}

int b3d_world_create_discrete()
{
  int i = bulletWorlds.size();
  BulletWorld* bulletworld = new BulletWorld();
  bulletworld->createDiscrete();
  bulletWorlds.push_back(bulletworld);
  return i;
}

int b3d_world_create_simple()
{
  int i = bulletWorlds.size();
  BulletWorld* bulletworld = new BulletWorld();
  bulletworld->createSimple();
  bulletWorlds.push_back(bulletworld);
  return i;
}

void b3d_world_delete(int id)
{
  get_world(bulletworld, id);
  bulletWorlds.erase(bulletWorlds.begin() + id);
}

void b3d_world_set_gravity(int id, double gx, double gy, double gz)
{
  get_world(bulletworld, id);
  bulletworld->dynamicsWorld->setGravity(btVector3(gx,gy,gz));
}

void b3d_world_add_body(int id, int body) 
{
  get_world(bulletworld, id);
  get_body(bulletbody, body);
  bulletworld->dynamicsWorld->addRigidBody(bulletbody->rigidBody);
}

void b3d_world_update(int id, double timestep, double iterations) 
{
  get_world(bulletworld, id);
  bulletworld->update(timestep, iterations);
}

void b3d_world_remove_body(int id, int body)
{
  get_world(bulletworld, id);
  bulletworld->dynamicsWorld->removeRigidBody(bulletBodies[body]->rigidBody);
}

}

