/********************************************************************************\
**                                                                              **
**  Copyright (C) 2013 Robert B. Colton                                         **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/
#include "Universal_System/collisions_object.h"
#include "Universal_System/instance_system.h" //iter
//#include "Universal_System/roomsystem.h"
#include "Collision_Systems/collision_mandatory.h" //iter
#include "Universal_System/instance.h"

#include <string>
#include <stdio.h>
#include <vector>
using std::vector;
//using namespace std;

#include <Box2D/Box2D.h>
#include "functions.h"

struct worldInstance { 
  b2World* world;
  // Prepare for simulation. Typically we use a time step of 1/60 of a
  // second (60Hz) and 10 iterations. This provides a high quality simulation
  // in most game scenarios.
  float32 timeStep = 1.0f / 30.0f;
  int32 velocityIterations = 6;
  int32 positionIterations = 2;
  worldInstance() 
  {
    // Define the gravity vector.
    b2Vec2 gravity(0.0f, 10.0f);

    // Construct a world object, which will hold and simulate the rigid bodies.
    world = new b2World(gravity);
  }
  void world_update();
}; 
void worldInstance::world_update() 
{
  world->Step(timeStep, velocityIterations, positionIterations);
}
vector<worldInstance> worlds(0);

struct fixtureInstance { 
  int world;
  b2Body* body;
  b2Fixture* fixture;
  b2Shape* shape;
  fixtureInstance() 
  {
  }
}; 
vector<fixtureInstance> fixtures(0);

/* This is just place holder shit to get the fuckin linker shut the fuckin hell up */
enigma::instance_t collision_rectangle(double x1, double y1, double x2, double y2, int obj, bool prec, bool notme)
{

}

namespace enigma
{ 
  void *get_collision_mask(sprite* spr, unsigned char* input_data, collision_type ct) // It is called for every subimage of every sprite loaded.
  {
    return 0;
  }

  void free_collision_mask(void* mask)
  {
  }
};

bool place_free(double x,double y)
{
  return true;
}

bool place_meeting(double x, double y, int object)
{
  return false;
}

/* This is where the actual physics_* functions start at */

void physics_test()
{

  int world = physics_world_create();

  int groundfixture = physics_fixture_create();
  physics_fixture_set_position(groundfixture, 0.0f, -10.0f);
  physics_fixture_set_box_shape(groundfixture, 50.0f, 10.0f);
  physics_fixture_set_density(groundfixture, 0);

  int dynamicfixture = physics_fixture_create();
  physics_fixture_set_position(dynamicfixture, 0.0f, 4.0f);
  //physics_fixture_set_box_shape(dynamicfixture, 1.0f, 1.0f);
  physics_fixture_set_circle_shape(dynamicfixture, 10.0f);
  physics_fixture_set_density(dynamicfixture, 1.0f);
  physics_fixture_set_friction(dynamicfixture, 0.3f);

  // This is our little game loop.
  for (int i = 0; i < 60; ++i)
  {
    physics_world_update(world);

    printf("%4.2f %4.2f %4.2f\n", 
      physics_fixture_get_x(dynamicfixture), 
      physics_fixture_get_y(dynamicfixture), 
      physics_fixture_get_angle(dynamicfixture));
  }

  // When the world destructor is called, all bodies and joints are freed. This can
  // create orphaned pointers, so be careful about your world management.

  return;
}

int physics_world_create() 
{
  int i = worlds.size();
  worlds.resize(i + 1);
  worlds[i] = worldInstance();
  return i;
}

void physics_world_update(int index) 
{
  worlds[index].world_update();
}

int physics_fixture_create() 
{
  int i = fixtures.size();
  fixtures.resize(i + 1);
  fixtureInstance fixture = fixtureInstance();
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  fixture.body = worlds[0].world->CreateBody(&bodyDef);
  fixtures[i] = fixture;
  return i;
}

void physics_fixture_set_box_shape(int id, float halfwidth, float halfheight)
{
  b2PolygonShape shape;
  shape.SetAsBox(halfwidth, halfheight);
  fixtures[id].shape = &shape;
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &shape;
  fixtures[id].fixture = fixtures[id].body->CreateFixture(&fixtureDef);
}

void physics_fixture_set_circle_shape(int id, float radius) 
{
  b2CircleShape shape;
  shape.m_radius = radius;
  fixtures[id].shape = &shape;
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &shape;
  fixtures[id].fixture = fixtures[id].body->CreateFixture(&fixtureDef);
}

void physics_fixture_set_position(int id, float x, float y)
{
  fixtures[id].body->SetTransform(b2Vec2(x, y), fixtures[id].body->GetAngle());
}

void physics_fixture_set_density(int id, float density) 
{
  if (density == 0) {
    fixtures[id].body->SetType(b2_staticBody);
  } else {
    fixtures[id].fixture->SetDensity(density);
  }
}

void physics_fixture_set_friction(int id, float friction) 
{
  fixtures[id].fixture->SetFriction(friction);
}

void physics_fixture_set_type(int id, b2BodyType type) 
{
  fixtures[id].body->SetType(type);
}

float physics_fixture_get_angle(int id) 
{
  return fixtures[id].body->GetAngle();
}

float physics_fixture_get_x(int id) 
{
  return fixtures[id].body->GetPosition().x;
}

float physics_fixture_get_y(int id) 
{
  return fixtures[id].body->GetPosition().y;
}
