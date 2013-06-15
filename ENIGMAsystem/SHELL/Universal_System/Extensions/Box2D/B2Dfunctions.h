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

#ifndef _FUNCTIONS__H
#define _FUNCTIONS__H

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

  }

}; 
extern vector<B2DBody*> b2dbodies;

namespace enigma_user
{

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

/************** Worlds **************/

/** Creates a physics world
@param pixeltometerscale the number of pixels for each meter
**/
void b2d_world_create(int pixeltometerscale);
/** Creates a physics world
@return This function returns the integer of the newly created world.
**/
int b2d_world_create();
/** Deletes a physics world
@param index the index of the world to delete
**/
void b2d_world_delete(int index);
/** Sets the paused state of the physics world
@param index
@param paused
**/
void b2d_world_pause_enable(int index, bool paused);
/** Scales the physics world
@param index
@param pixelstometers
**/
void b2d_world_scale(int index, int pixelstometers);
/** description...
@param for each parameter
**/
void b2d_world_gravity(int index, double gx, double gy);
/** description...
@param index
**/
void b2d_world_update(int index); 
/** description...
@param index
**/
void b2d_world_dump(int index);
/** description...
@param index
@param timeStep
@param velocityIterations
@param positionIterations
**/
void b2d_world_update_settings(int index, double timeStep, int velocityIterations, int positionIterations);
/** description...
@param index
@param iterationsperstep
**/
void b2d_world_update_iterations(int index, int iterationsperstep);
/** description...
@param index
@param updatesperstep
**/
void b2d_world_update_speed(int index, int updatesperstep);
/** description...
**/
void b2d_world_draw_debug();

void b2d_world_clear_forces(int index);
void b2d_world_set_sleeping(int index, bool sleeping);
void b2d_world_set_warmstarting(int index, bool warmstarting);
void b2d_world_set_continuous(int index, bool continuous);
void b2d_world_set_substepping(int index, bool substepping);
void b2d_world_set_clearforces(int index, bool autoclear);
bool b2d_world_get_sleeping(int index);
bool b2d_world_get_warmstarting(int index);
bool b2d_world_get_continuous(int index);
bool b2d_world_get_substepping(int index);
int b2d_world_get_proxy_count(int index);
int b2d_world_get_body_count(int index);
int b2d_world_get_joint_count(int index);
int b2d_world_get_contect_count(int index);
bool b2d_world_get_locked(int index);
bool b2d_world_get_clearforces(int index);

/************** Bodies **************/

/** description...
@param world
@return This function returns the index of the newly created fixture.
**/
int  b2d_body_create(int world = 0);
/** description...
@param id
**/
void b2d_body_bind(int id); 
/** description...
**/
void b2d_body_bind(); 
/** description...
@param id
**/
void b2d_body_delete(int id);
/** description...
@param id
**/
void b2d_body_dump(int id);

/** description...
@param id
@param x
@param y
@param angle
**/
void b2d_body_set_transform(int id, double x, double y, double angle);
/** description...
@param id
@param x
@param y
**/
void b2d_body_set_position(int id, double x, double y);
/** description...
@param id
@param angle
**/
void b2d_body_set_angle(int id, double angle);
/** description...
@param id
@param fixed
**/
void b2d_body_set_angle_fixed(int id, bool fixed);
/** description...
@param id
@param density
**/
void b2d_body_set_density(int id, double density);
/** description...
@param id
@param friction
**/
void b2d_body_set_friction(int id, double friction);
/** description...
@param id
@param restitution
**/
void b2d_body_set_restitution(int id, double restitution);
/** description...
@param id
@param state
**/
void b2d_body_set_sensor(int id, bool state);
/** description...
@param id
@param damping
**/
void b2d_body_set_linear_damping(int id, double damping);
/** description...
@param id
@param damping
**/
void b2d_body_set_angular_damping(int id, double damping);
/** description...
@param id
**/
void b2d_body_set_static(int id);
/** description...
@param id
**/
void b2d_body_set_kinematic(int id);
/** description...
@param id
**/
void b2d_body_set_dynamic(int id);
/** description...
@param id
@param state
**/
void b2d_body_set_awake(int id, bool state);
/** description...
@param id
@param bullet
**/
void b2d_body_set_bullet(int id, bool bullet);
/** description...
@param id
@param allowsleep
**/
void b2d_body_set_sleep(int id, bool allowsleep);
/** description...
@param id
@param group
**/
void b2d_body_set_collision_group(int id, int group);
/** description...
@param id
@param mass
@param local_center_x
@param local_center_y
@param inertia
**/
void b2d_body_set_massdata(int id, double mass, double local_center_x, double local_center_y, double inertia);
/** description...
@param id
**/
void b2d_body_reset_massdata(int id);

/** description...
@param id
@return the return....
**/
double b2d_body_get_angle(int id);
/** description...
@param id
@return the return....
**/
double b2d_body_get_x(int id);
/** description...
@param id
@return the return....
**/
double b2d_body_get_y(int id);
/** description...
@param id
@return the return....
**/
double b2d_body_get_mass(int id);
/** description...
@param id
@return the return....
**/
double b2d_body_get_center_x(int id);
/** description...
@param id
@return the return....
**/
double b2d_body_get_center_y(int id);
/** description...
@param id
@return the return....
**/
double b2d_body_get_inertia(int id);
/** description...
@param id
@return the return...
**/
double b2d_body_get_angular_velocity(int id);
/** description...
@param id
@return the return...
**/
bool b2d_body_get_awake(int id);
/** description...
@param id
@return the return...
**/
bool b2d_body_get_sleeping(int id);
/** description...
@param id
@return the return...
**/
bool b2d_body_get_angle_fixed(int id);
/** description...
@param id
@return the return...
**/
bool b2d_body_get_bullet(int id);
/** description...
@param id
@param fid
@return the return...
**/
int b2d_body_get_fixture(int id, int fid);

/************** Forces **************/
void b2d_body_apply_force(int id, double xpos, double ypos, double xforce, double yforce);
void b2d_body_apply_force_center(int id, double xforce, double yforce);
void b2d_body_apply_torque(int id, double torque);
void b2d_body_apply_impulse_linear(int id, double xpos, double ypos, double ximpulse, double yimpulse);
void b2d_body_apply_impulse_angular(int id, double impulse);

/** description...
@param world
@param xpos
@param ypos
@param xforce
@param yforce
**/
void b2d_world_apply_force(int world, double xpos, double ypos, double xforce, double yforce);
/** description...
@param world
@param xpos
@param ypos
**/
void b2d_world_apply_force_radial(int world, double xpos, double ypos);
/** description...
@param world
@param xpos
@param ypos
@param ximpulse
@param yimpulse
**/
void b2d_world_apply_impulse(int world, double xpos, double ypos, double ximpulse, double yimpulse);
/** description...
@param world
@param xpos
@param ypos
**/
void b2d_world_apply_impulse_radial(int world, double xpos, double ypos);

/************** Miscellaneous **************/
/** description...
**/ 
void b2d_draw_debug(); 
/** description...
@param pause
**/
void b2d_pause_enable(bool pause);

}

#endif
