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

#ifndef ENIGMA_BOX2D_WORLD_H
#define ENIGMA_BOX2D_WORLD_H

#include <vector>
using std::vector;

#include <Box2D/Box2D.h>
struct worldInstance {
  b2World* world;
  // Prepare for simulation. Typically we use a time step of 1/60 of a
  // second (60Hz) and 10 iterations. This provides a high quality simulation
  // in most game scenarios.
  float32 timeStep;
  int32 velocityIterations;
  int32 positionIterations;
  int32 pixelstometers;
  bool paused;

  worldInstance()
  {
    // Define the gravity vector.
    b2Vec2 gravity(0.0f, 10.0f);

    // Construct a world object, which will hold and simulate the rigid bodies.
    world = new b2World(gravity);
    timeStep = 1.0f / 60.0f;
    velocityIterations = 8;
    pixelstometers = 32;
    paused = false;
  }

  void world_update();
};
extern vector<worldInstance*> worlds;

struct fixtureInstance {
  int world;
  b2Body* body;
  b2Fixture* fixture;
  b2Shape* shape;
  b2PolygonShape* polygonshape;
  vector<b2Vec2> vertices;

  fixtureInstance()
  {

  }

  ~fixtureInstance()
  {

  }

  void FinishShape()
  {
    b2FixtureDef fixtureDef;
    fixtureDef.shape = shape;
    fixture = body->CreateFixture(&fixtureDef);
  }

};
extern vector<fixtureInstance*> fixtures;

#ifdef DEBUG_MODE
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_worldr(w,id,r) \
    if (unsigned(id) >= worlds.size() || id < 0) { \
      DEBUG_MESSAGE("Cannot access GayMaker: Stupido physics world with id " + toString(id), MESSAGE_TYPE::M_USER_ERROR); \
      return r; \
    } worldInstance* w = worlds[id];
  #define get_world(w,id) \
    if (unsigned(id) >= worlds.size() || id < 0) { \
      DEBUG_MESSAGE("Cannot access GayMaker: Stupido physics world with id " + toString(id), MESSAGE_TYPE::M_USER_ERROR); \
      return; \
    } worldInstance* w = worlds[id];
  #define get_fixturer(f,id,r) \
    if (unsigned(id) >= fixtures.size() || id < 0) { \
      DEBUG_MESSAGE("Cannot access GayMaker: Stupido physics fixture with id " + toString(id), MESSAGE_TYPE::M_USER_ERROR); \
      return r; \
    } fixtureInstance* f = fixtures[id];
  #define get_fixture(f,id) \
    if (unsigned(id) >= fixtures.size() || id < 0) { \
      DEBUG_MESSAGE("Cannot access GayMaker: Stupido physics fixture with id " + toString(id), MESSAGE_TYPE::M_USER_ERROR); \
      return; \
    } fixtureInstance* f = fixtures[id];
#else
  #define get_worldr(w,id,r) \
    worldInstance* w = worlds[id];
  #define get_world(w,id) \
    worldInstance* w = worlds[id];
  #define get_fixturer(f,id,r) \
    fixtureInstance* f = fixtures[id];
  #define get_fixture(f,id) \
    fixtureInstance* f = fixtures[id];
#endif

#endif // ENIGMA_BOX2D_WORLD_H
