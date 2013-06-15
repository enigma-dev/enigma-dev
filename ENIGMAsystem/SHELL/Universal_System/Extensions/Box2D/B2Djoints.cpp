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

#include <vector>
using std::vector;

#include <Box2D/Box2D.h>
#include "B2Djoints.h"
#include "B2Dfunctions.h"

vector<B2DJoint*> b2djoints(0);

namespace enigma_user
{

int b2d_joint_create(int world)
{
    int i = b2djoints.size();
    B2DJoint* b2djoint = new B2DJoint();
    b2djoint->worldid = world;
    b2djoints.push_back(b2djoint);
    return i;
}

void b2d_joint_distance_create(int id, int body1, int body2, bool collide_connected)
{
  if (unsigned(id) >= b2djoints.size() || id < 0)
  {
    return;
  }
  else
  {
    b2DistanceJointDef jointDef;
    jointDef.bodyA = b2dbodies[body1]->body;
    jointDef.bodyB = b2dbodies[body2]->body;
    jointDef.collideConnected = collide_connected;
    jointDef.frequencyHz = 4.0f;
    jointDef.dampingRatio = 0.5f;
    b2djoints[id]->joint = b2dworlds[b2djoints[id]->worldid]->world->CreateJoint(&jointDef);
  }
}

void b2d_joint_mouse_create(int id, int bodyid)
{
  if (unsigned(id) >= b2djoints.size() || id < 0)
  {
    return;
  }
  else
  {
    b2MouseJointDef jointDef;
    jointDef.bodyA = b2dbodies[bodyid]->body;
    b2djoints[id]->joint = b2dworlds[b2djoints[id]->worldid]->world->CreateJoint(&jointDef);
  }
}

void b2d_joint_set_target(int id, double x, double y)
{
  if (unsigned(id) >= b2djoints.size() || id < 0)
  {
    return;
  }
  else
  {
    if (b2djoints[id]->joint->GetType() != e_mouseJoint) {
      return;
    }
    b2MouseJoint* mouseJoint = (b2MouseJoint*)b2djoints[id]->joint;
    mouseJoint->SetTarget(b2Vec2(x, y));
  }
}

}

