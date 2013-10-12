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
#include "Box2DWorld.h"
#include "Box2DJoint.h"
#include "B2Djoints.h"
#include "B2Dfunctions.h"
#include "Universal_System/scalar.h"

vector<B2DJoint*> b2djoints(0);

namespace enigma_user
{

int b2d_joint_create_distance(int world, int bodya, int bodyb, bool collide)
{
	get_worldr(b2dworld, world, -1);
    b2DistanceJointDef jointDef;
    jointDef.bodyA = b2dbodies[bodya]->body;
    jointDef.bodyB = b2dbodies[bodyb]->body;
    jointDef.collideConnected = collide;
    jointDef.frequencyHz = 4.0f;
    jointDef.dampingRatio = 0.5f;
	B2DJoint* b2djoint = new B2DJoint();
    b2djoint->joint = b2dworld->world->CreateJoint(&jointDef);
    b2djoint->worldid = world;
    b2djoints.push_back(b2djoint);
    return b2djoints.size() - 1;
}

int b2d_joint_create_mouse(int world, int bodya, int bodyb, bool collide, double x, double y)
{
	get_worldr(b2dworld, world, -1);
    b2MouseJointDef jointDef;
    jointDef.bodyA = b2dbodies[bodya]->body;
	jointDef.bodyB = b2dbodies[bodyb]->body;
	jointDef.target.Set(x, y);
	jointDef.maxForce = 30000;
	jointDef.collideConnected = collide;
	B2DJoint* b2djoint = new B2DJoint();
    b2djoint->joint = b2dworld->world->CreateJoint(&jointDef);
	b2djoint->worldid = world;
    b2djoints.push_back(b2djoint);
    return b2djoints.size() - 1;
}

int b2d_joint_create_revolute(int world, int bodya, int bodyb, bool limit, double lower, double upper)
{
	get_worldr(b2dworld, world, -1);
	get_bodyr(b2dbodya, bodya, -1);
	get_bodyr(b2dbodyb, bodyb, -1);
    b2RevoluteJointDef jointDef;
    jointDef.bodyA = b2dbodya->body;
	jointDef.bodyB = b2dbodyb->body;
	jointDef.lowerAngle = cs_angular_degrees(lower);
	jointDef.upperAngle = cs_angular_degrees(upper);
	jointDef.localAnchorA = b2Vec2(0,0);
	jointDef.localAnchorB =b2Vec2(1,1);
	jointDef.enableLimit = limit;
	B2DJoint* b2djoint = new B2DJoint();
    b2djoint->joint = b2dworld->world->CreateJoint(&jointDef);
	b2djoint->worldid = world;
    b2djoints.push_back(b2djoint);
    return b2djoints.size() - 1;
}

void b2d_joint_delete(int id) {
	get_joint(b2djoint, id);
	delete b2djoint;
}

void b2d_joint_set_target(int id, double x, double y)
{
	get_joint(b2djoint, id);
    if (b2djoint->joint->GetType() != e_mouseJoint) {
     // return;
    }
    b2MouseJoint* mouseJoint = (b2MouseJoint*)b2djoint->joint;
    mouseJoint->SetTarget(b2Vec2(x, y));
}

}

