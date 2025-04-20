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
#include "B2Dshapes.h"

struct B2DWorld {
  b2World* world;
  // Prepare for simulation. Typically we use a time step of 1/60 of a
  // second (60Hz) and 10 iterations. This provides a high quality simulation
  // in most game scenarios.
  float32 timeStep;
  int32 velocityIterations;
  int32 positionIterations;
  int32 pixelstometers;
  bool paused;

  B2DWorld()
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
extern vector<B2DWorld*> b2dworlds;

struct B2DBody {
  int world;
  vector<int> fixtures;
  b2Body* body;

  B2DBody()
  {

  }

  ~B2DBody()
  {
    this->body->GetWorld()->DestroyBody(this->body);
  }

};
extern vector<B2DBody*> b2dbodies;

#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_worldr(w,id,r) \
    if (unsigned(id) >= b2dworlds.size() || id < 0) { \
      DEBUG_MESSAGE("Cannot access Box2D physics world with id " + toString(id), MESSAGE_TYPE::M_USER_ERROR); \
      return r; \
    } B2DWorld* w = b2dworlds[id];
  #define get_world(w,id) \
    if (unsigned(id) >= b2dworlds.size() || id < 0) { \
      DEBUG_MESSAGE("Cannot access Box2D physics world with id " + toString(id), MESSAGE_TYPE::M_USER_ERROR); \
      return; \
    } B2DWorld* w = b2dworlds[id];
  #define get_bodyr(b,id,r) \
    if (unsigned(id) >= b2dbodies.size() || id < 0) { \
      DEBUG_MESSAGE("Cannot access Box2D physics body with id " + toString(id), MESSAGE_TYPE::M_USER_ERROR); \
      return r; \
    } B2DBody* b = b2dbodies[id];
  #define get_body(b,id) \
    if (unsigned(id) >= b2dbodies.size() || id < 0) { \
      DEBUG_MESSAGE("Cannot access Box2D physics body with id " + toString(id), MESSAGE_TYPE::M_USER_ERROR); \
      return; \
    } B2DBody* b = b2dbodies[id];
#else
  #define get_worldr(w,id,r) \
    B2DWorld* w = b2dworlds[id];
  #define get_world(w,id) \
    B2DWorld* w = b2dworlds[id];
  #define get_bodyr(b,id,r) \
    B2DBody* b = b2dbodies[id];
  #define get_body(b,id) \
    B2DBody* b = b2dbodies[id];
#endif

#endif // ENIGMA_BOX2D_WORLD_H
