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
#include "B2Dfunctions.h"
#include "B2Djoints.h"

vector<jointInstance> joints(0);

int physics_joint_create(int world)
{
    int i = joints.size();
    jointInstance joint = jointInstance();
    joint.worldid = world;
    joints.push_back(joint);
    return i;
}

void physics_joint_distance_create(int id, int fixture1, int fixture2, bool collide_connected)
{
  if (unsigned(id) >= joints.size() || id < 0)
  {
    return;
  }
  else
  {
    b2DistanceJointDef jointDef;
    jointDef.bodyA = fixtures[fixture1].body;
    jointDef.bodyB = fixtures[fixture2].body;
    jointDef.collideConnected = collide_connected;
    jointDef.frequencyHz = 4.0f;
    jointDef.dampingRatio = 0.5f;
    joints[id].joint = worlds[joints[id].worldid].world->CreateJoint(&jointDef);
  }
}

void physics_joint_mouse_create(int id, int fixture)
{
  if (unsigned(id) >= joints.size() || id < 0)
  {
    return;
  }
  else
  {
    b2MouseJointDef jointDef;
    jointDef.bodyA = fixtures[fixture].body;
    joints[id].joint = worlds[joints[id].worldid].world->CreateJoint(&jointDef);
  }
}

void physics_joint_set_target(int id, double x, double y)
{
  if (unsigned(id) >= joints.size() || id < 0)
  {
    return;
  }
  else
  {
    if (joints[id].joint->GetType() != e_mouseJoint) {
      return;
    }
    b2MouseJoint* mouseJoint = (b2MouseJoint*)joints[id].joint;
    mouseJoint->SetTarget(b2Vec2(x, y));
  }
}
