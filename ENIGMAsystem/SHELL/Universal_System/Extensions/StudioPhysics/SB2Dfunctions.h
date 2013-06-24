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

#ifndef _ENIGMA_STUDIO_PHYSICS_FUNCTIONS__H
#define _ENIGMA_STUDIO_PHYSICS_FUNCTIONS__H

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

/** Check if there are no solid objects at a given position.
@param x coordinate representing the location
@param y 
@return This function will return true if there was collision, or false otherwise.
**/
bool place_free(double x,double y);
/** description...
@param x
@param y
@param object
@return the return....
**/
bool place_meeting(double x, double y, int object);

namespace enigma_user
{

/************** Worlds **************/

/** Creates a physics world
@param pixeltometerscale the number of pixels for each meter
**/
void physics_world_create(int pixeltometerscale);
/** Creates a physics world
@return This function returns the integer of the newly created world.
**/
int physics_world_create();
/** Deletes a physics world
@param index the index of the world to delete
**/
void physics_world_delete(int index);
/** Sets the paused state of the physics world
@param index
@param paused
**/
void physics_world_pause_enable(int index, bool paused);
/** Scales the physics world
@param index
@param pixelstometers
**/
void physics_world_scale(int index, int pixelstometers);
/** description...
@param for each parameter
**/
void physics_world_gravity(int index, double gx, double gy);
/** description...
@param index
**/
void physics_world_update(int index); 
/** description...
@param index
@param timeStep
@param velocityIterations
@param positionIterations
**/
void physics_world_update_settings(int index, double timeStep, int velocityIterations, int positionIterations);
/** description...
@param index
@param iterationsperstep
**/
void physics_world_update_iterations(int index, int iterationsperstep);
/** description...
@param index
@param updatesperstep
**/
void physics_world_update_speed(int index, int updatesperstep);
/** description...
**/
void physics_world_draw_debug();

/************** Fixtures **************/

/** description...
@param world
@return This function returns the index of the newly created fixture.
**/
int  physics_fixture_create(int world);
/** description...
@return the return....
**/
int  physics_fixture_create(); 
/** description...
@param id
**/
void physics_fixture_bind(int id); 
/** description...
**/
void physics_fixture_bind(); 
/** description...
@param id
@param group
**/
void physics_fixture_set_collision_group(int id, int group);
/** description...
@param id
**/
void physics_fixture_delete(int id);

/** description...
@param id
@param halfwidth
@param halfheight
**/
void physics_fixture_set_box_shape(int id, double halfwidth, double halfheight);
/** description...
@param id
@param radius
**/
void physics_fixture_set_circle_shape(int id, double radius);
/** description...
@param id
**/
void physics_fixture_set_polygon_shape(int id);
/** description...
@param id
@param x
@param y
**/
void physics_fixture_add_point(int id, double x, double y);

/** description...
@param id
@param x
@param y
@param angle
**/
void physics_fixture_set_transform(int id, double x, double y, double angle);
/** description...
@param id
@param x
@param y
**/
void physics_fixture_set_position(int id, double x, double y);
/** description...
@param id
@param angle
**/
void physics_fixture_set_angle(int id, double angle);
/** description...
@param id
@param density
**/
void physics_fixture_set_density(int id, double density);
/** description...
@param id
@param friction
**/
void physics_fixture_set_friction(int id, double friction);
/** description...
@param id
@param damping
**/
void physics_fixture_set_linear_damping(int id, double damping);
/** description...
@param id
@param damping
**/
void physics_fixture_set_angular_damping(int id, double damping);
/** description...
@param id
@param restitution
**/
void physics_fixture_set_restitution(int id, double restitution);
/** description...
@param id
@param state
**/
void physics_fixture_set_sensor(int id, bool state);
/** description...
@param id
**/
void physics_fixture_set_static(int id);
/** description...
@param id
**/
void physics_fixture_set_kinematic(int id);
/** description...
@param id
**/
void physics_fixture_set_dynamic(int id);
/** description...
@param id
@param state
**/
void physics_fixture_set_awake(int id, bool state);
/** description...
@param id
@param allowsleep
**/
void physics_fixture_set_sleep(int id, bool allowsleep);
/** description...
@param id
@param mass
@param local_center_x
@param local_center_y
@param inertia
**/
void physics_fixture_mass_properties(int id, double mass, double local_center_x, double local_center_y, double inertia);

/** description...
@param id
@return the return....
**/
double physics_fixture_get_angle(int id);
/** description...
@param id
@return the return....
**/
double physics_fixture_get_x(int id);
/** description...
@param id
@return the return....
**/
double physics_fixture_get_y(int id);
/** description...
@param id
@return the return....
**/
double physics_fixture_get_mass(int id);
/** description...
@param id
@return the return....
**/
double physics_fixture_get_center_x(int id);
/** description...
@param id
@return the return....
**/
double physics_fixture_get_center_y(int id);
/** description...
@param id
@return the return....
**/
double physics_fixture_get_inertia(int id);

/************** Forces **************/

/** description...
@param world
@param xpos
@param ypos
@param xforce
@param yforce
**/
void physics_apply_force(int world, double xpos, double ypos, double xforce, double yforce);
/** description...
@param world
@param xpos
@param ypos
**/
void physics_apply_force_radial(int world, double xpos, double ypos);
/** description...
@param world
@param xpos
@param ypos
@param ximpulse
@param yimpulse
**/
void physics_apply_impulse(int world, double xpos, double ypos, double ximpulse, double yimpulse);
/** description...
@param world
@param xpos
@param ypos
**/
void physics_apply_impulse_radial(int world, double xpos, double ypos);
/** description...
@param id
@param xlocal
@param ylocal
**/
void physics_apply_local_force(int id, double xlocal, double ylocal, double xforce, double yforce);
/** description...
@param world
@param xpos
@param ypos
**/
void physics_apply_local_force_radial(int world, double xpos, double ypos);
/** description...
@param id
@param xlocal
@param ylocal
@param ximpulse
@param yimpulse
**/
void physics_apply_local_impulse(int id, double xlocal, double ylocal, double ximpulse, double yimpulse);
/** description...
@param world
@param xpos
@param ypos
**/
void physics_apply_local_impulse_radial(int world, double xpos, double ypos);
/** description...
@param id
@param torque
**/
void physics_apply_local_torque(int id, double torque);

/************** Miscellaneous **************/
/** description...
**/
void physics_test_overlap();
/** description...
@param mass
@param local_center_x
@param local_center_y
@param interia
**/
void physics_mass_properties(double mass, double local_center_x, double local_center_y, double inertia);
/** description...
**/ 
void physics_draw_debug(); 
/** description...
@param pause
**/
void physics_pause_enable(bool pause);

}

#endif // _ENIGMA_STUDIO_PHYSICS_FUNCTIONS__H

