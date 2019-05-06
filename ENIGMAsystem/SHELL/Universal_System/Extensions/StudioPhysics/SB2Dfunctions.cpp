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

#include <string>
#include <stdio.h>
//using namespace std;

/*
#include "Universal_System/Extensions/recast.h"
#include "implement.h"
declare_recast(enigma::extension_studiophysics);

namespace enigma {
  extension_studiophysics::extension_studiophysics() {
  }
}
*/

#include "Universal_System/callbacks_events.h"
#include "Universal_System/scalar.h"

#include <Box2D/Box2D.h>
#include "Box2DWorld.h"
#include "SB2Dfunctions.h"
bool systemPaused = false;

// NOTES:
// 1) box2d uses an inversed y-axis, thus why physics_fixture_get_angle() returns -radianstodegrees(angle)
// 2) static objects when hit appear to me to move about one pixel at times, this is caused by bodies/fixtures not being created in the world
// 3) leave the option to continue allowing you to manually update your world
// 4) box2d's manual also states you should blend previous timesteps for updating the world with the current timestep
//    in order to make the simulation run smoother
// 5) box2d manual is available here... http://www.box2d.org/manual.html
// 6) box2d's classes allow you to set a b2Shape for instance to a b2CircleShape or b2PolygonShape
//    that is why I wrote the classes to use an abstracted pointer reference such as b2Shape and b2Joint

vector<worldInstance*> worlds(0);
vector<fixtureInstance*> fixtures;

void worldInstance::world_update()
{
  if (!systemPaused && !paused) {
    world->Step(timeStep, velocityIterations, positionIterations);
    world->ClearForces();
  }
}

#ifdef DEBUG_MODE
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_worldr(w,id,r) \
    if (unsigned(id) >= worlds.size() || id < 0) { \
      enigma_user::show_error("Cannot access GayMaker: Stupido physics world with id " + toString(id), false); \
      return r; \
    } worldInstance* w = worlds[id];
  #define get_world(w,id) \
    if (unsigned(id) >= worlds.size() || id < 0) { \
      enigma_user::show_error("Cannot access GayMaker: Stupido physics world with id " + toString(id), false); \
      return; \
    } worldInstance* w = worlds[id];
  #define get_fixturer(f,id,r) \
    if (unsigned(id) >= fixtures.size() || id < 0) { \
      enigma_user::show_error("Cannot access GayMaker: Stupido physics fixture with id " + toString(id), false); \
      return r; \
    } fixtureInstance* f = fixtures[id];
  #define get_fixture(f,id) \
    if (unsigned(id) >= fixtures.size() || id < 0) { \
      enigma_user::show_error("Cannot access GayMaker: Stupido physics fixture with id " + toString(id), false); \
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

namespace enigma {
  bool has_been_initialized = false;

  void update_worlds_automatically() {
    vector<worldInstance*>::iterator it_end = worlds.end();
    for (vector<worldInstance*>::iterator it = worlds.begin(); it != it_end; it++) {
      (*it)->world_update();
    }
  }

  // Should be called whenever a world is created.
  void init_studio_physics() {
    if (!has_been_initialized) {
      has_been_initialized = true;

      // Register callback.
      register_callback_before_collision_event(update_worlds_automatically);
    }
  }
}

namespace enigma_user
{

void physics_world_create(int pixeltometerscale)
{
  enigma::init_studio_physics();
  /** stupido's messed up world creation just auto binds it to the current room
  **/
}

int physics_world_create()
{
  enigma::init_studio_physics();
  int i = worlds.size();
  worlds.push_back(new worldInstance());
  return i;
}

void physics_world_delete(int index)
{

}

void physics_world_pause_enable(int index, bool paused)
{
  get_world(sb2dworld, index);
  sb2dworld->paused = paused;
}

void physics_world_scale(int index, int pixelstometers)
{
  get_world(sb2dworld, index);
  sb2dworld->pixelstometers = pixelstometers;
}

void physics_world_gravity(int index, double gx, double gy)
{
  get_world(sb2dworld, index);
  worlds[index]->world->SetGravity(b2Vec2(gx, gy));
}

void physics_world_update(int index)
{
  // extra function to control your world update, should be auto done inside our game loop
  get_world(sb2dworld, index);
  worlds[index]->world_update();
}

void physics_world_update_settings(int index, double timeStep, int velocityIterations, int positionIterations)
{
  // extra function to control your world update settings
  get_world(sb2dworld, index);
  worlds[index]->timeStep = timeStep;
  worlds[index]->velocityIterations = velocityIterations;
  worlds[index]->positionIterations = positionIterations;
}

void physics_world_update_iterations(int iterationsperstep)
{
  // provide overloads so that you can still change indexed worlds
}

void physics_world_update_iterations(int index, int iterationsperstep)
{
  // this sets the number of iterations the physics system takes each step
}

void physics_world_update_speed(int updatesperstep)
{
  // provide overloads so that you can still change indexed worlds
}

void physics_world_update_speed(int index, int updatesperstep)
{
  // this sets the number of updates the physics system takes each step
}

void physics_world_draw_debug()
{
  // draws all the fixtures and their rotations in the room for u, wants constants
}

int physics_fixture_create(int world)
{
  get_worldr(sb2dworld, world, -1);
  int i = fixtures.size();
  fixtureInstance* fixture = new fixtureInstance();
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  fixture->body = sb2dworld->world->CreateBody(&bodyDef);
  fixtures.push_back(fixture);
  fixtures[i]->world = world;
  return i;
}

int physics_fixture_create()
{
  // this needs to create a fixture on the object, thats why i can't just overload the prior function
  physics_fixture_create(0);
}

void physics_fixture_bind(int id)
{
  // binds a fixture to nothing, just closes and fills the definition
  get_fixture(sb2dfixture, id);
}

void physics_fixture_bind()
{
  // binds a fixture to an object
}

void physics_fixture_set_collision_group(int id, int group)
{
  get_fixture(sb2dfixture, id);
  // sets the collision group used to make parts of things not collide, like a ragdoll for
  // instance should not collide with itself
  b2Filter newfilter;
  newfilter.groupIndex = group;
  fixtures[id]->fixture->SetFilterData(newfilter);
}

void physics_fixture_delete(int id)
{
  get_fixture(sb2dfixture, id);
  delete sb2dfixture;
}

void physics_fixture_set_box_shape(int id, double halfwidth, double halfheight)
{
  get_fixture(sb2dfixture, id);
  b2PolygonShape shape;
  shape.SetAsBox(halfwidth, halfheight);
  sb2dfixture->shape = &shape;
  sb2dfixture->FinishShape();
}

void physics_fixture_set_circle_shape(int id, double radius)
{
  get_fixture(sb2dfixture, id);
  b2CircleShape shape;
  shape.m_radius = radius;
  sb2dfixture->shape = &shape;
  sb2dfixture->FinishShape();
}

void physics_fixture_set_polygon_shape(int id)
{
  get_fixture(sb2dfixture, id);
  b2PolygonShape shape;

  shape.Set(&sb2dfixture->vertices[0], sb2dfixture->vertices.size());
  sb2dfixture->shape = &shape;
  sb2dfixture->FinishShape();
}

void physics_fixture_set_edge_shape(int id, bool adjstart, bool adjend)
{
  get_fixture(sb2dfixture, id);
  b2EdgeShape shape;

  int vid = 0;
  if (adjstart) {
    shape.m_hasVertex0 = true;
    shape.m_vertex0 = sb2dfixture->vertices[vid];
    vid += 1;
  }
  shape.Set(sb2dfixture->vertices[vid],  sb2dfixture->vertices[vid + 1]);
  vid += 2;
  if (adjend) {
    shape.m_hasVertex3 = true;
    shape.m_vertex3 = sb2dfixture->vertices[vid];
  }
  sb2dfixture->shape = &shape;
  sb2dfixture->FinishShape();
}

void physics_fixture_add_point(int id, double x, double y)
{
  get_fixture(sb2dfixture, id);
  sb2dfixture->vertices.push_back(b2Vec2(x, y));
}

void physics_fixture_set_transform(int id, double x, double y, double angle)
{
  get_fixture(sb2dfixture, id);
  sb2dfixture->body->SetTransform(b2Vec2(x, y), cs_angle_to_radians(angle));
}

void physics_fixture_set_position(int id, double x, double y)
{
  get_fixture(sb2dfixture, id);
  sb2dfixture->body->SetTransform(b2Vec2(x, y), sb2dfixture->body->GetAngle());
}

void physics_fixture_set_angle(int id, double angle)
{
  get_fixture(sb2dfixture, id);
  sb2dfixture->body->SetTransform(sb2dfixture->body->GetPosition(), cs_angle_to_radians(angle));
}

void physics_fixture_set_density(int id, double density)
{
  get_fixture(sb2dfixture, id);
  // stupido makes it so 0 density, means infinite density and just makes it
  // a static object, thats actually stupid though because box2d lets you use it as a flag
  // for floating object, oh well use the ENIGMA version instead :/
  if (density == 0) {
    sb2dfixture->body->SetType(b2_staticBody);
  } else {
    sb2dfixture->fixture->SetDensity(density);
    sb2dfixture->body->ResetMassData();
  }
}

void physics_fixture_set_friction(int id, double friction)
{
  get_fixture(sb2dfixture, id);
  sb2dfixture->fixture->SetFriction(friction);
  sb2dfixture->body->ResetMassData();
}

void physics_fixture_set_linear_damping(int id, double damping)
{
  get_fixture(sb2dfixture, id);
  sb2dfixture->body->SetLinearDamping(damping);
}

void physics_fixture_set_angular_damping(int id, double damping)
{
  get_fixture(sb2dfixture, id);
  sb2dfixture->body->SetAngularDamping(damping);
}

void physics_fixture_set_restitution(int id, double restitution)
{
  get_fixture(sb2dfixture, id);
  sb2dfixture->fixture->SetRestitution(restitution);
}

void physics_fixture_set_sensor(int id, bool state)
{
  get_fixture(sb2dfixture, id);
  sb2dfixture->fixture->SetSensor(state);
}

void physics_fixture_set_awake(int id, bool state)
{
  get_fixture(sb2dfixture, id);
  sb2dfixture->body->SetAwake(state);
}

void physics_fixture_set_sleep(int id, bool allowsleep)
{
  get_fixture(sb2dfixture, id);
  sb2dfixture->body->SetSleepingAllowed(allowsleep);
}

void physics_fixture_mass_properties(int id, double mass, double local_center_x, double local_center_y, double inertia)
{
  get_fixture(sb2dfixture, id);
  b2MassData lMassData;
  sb2dfixture->body->GetMassData(&lMassData);
  lMassData.mass = mass;
  lMassData.center.Set(local_center_x, local_center_y);
  lMassData.I = inertia;
  sb2dfixture->body->SetMassData(&lMassData);
}

void physics_fixture_set_static(int id)
{
  get_fixture(sb2dfixture, id);
  sb2dfixture->body->SetType(b2_staticBody);
  sb2dfixture->body->ResetMassData();
}

void physics_fixture_set_kinematic(int id)
{
  get_fixture(sb2dfixture, id);
  sb2dfixture->body->SetType(b2_kinematicBody);
}

void physics_fixture_set_dynamic(int id)
{
  get_fixture(sb2dfixture, id);
  sb2dfixture->body->SetType(b2_dynamicBody);
}

double physics_fixture_get_angle(int id)
{
  get_fixturer(sb2dfixture, id, -1);
  return -cs_angle_from_radians(sb2dfixture->body->GetAngle());
}

double physics_fixture_get_x(int id)
{
  get_fixturer(sb2dfixture, id, -1);
  return sb2dfixture->body->GetPosition().x;
}

double physics_fixture_get_y(int id)
{
  get_fixturer(sb2dfixture, id, -1);
  return sb2dfixture->body->GetPosition().y;
}

double physics_fixture_get_mass(int id)
{
  get_fixturer(sb2dfixture, id, -1);
  b2MassData lMassData;
  sb2dfixture->body->GetMassData(&lMassData);
  return lMassData.mass;
}

double physics_fixture_get_center_x(int id)
{
  get_fixturer(sb2dfixture, id, -1);
  b2MassData lMassData;
  sb2dfixture->body->GetMassData(&lMassData);
  return lMassData.center.x;
}

double physics_fixture_get_center_y(int id)
{
  get_fixturer(sb2dfixture, id, -1);
  b2MassData lMassData;
  sb2dfixture->body->GetMassData(&lMassData);
  return lMassData.center.y;
}

double physics_fixture_get_inertia(int id)
{
  get_fixturer(sb2dfixture, id, -1);
  b2MassData lMassData;
  sb2dfixture->body->GetMassData(&lMassData);
  return lMassData.I;
}

void physics_apply_force(int world, double xpos, double ypos, double xforce, double yforce, bool wake)
{
  get_fixture(sb2dworld, world);
  for (int i = 0; i < fixtures.size(); i++)
  {
    if (fixtures[i]->world == world)
    {
      fixtures[i]->body->ApplyForce(b2Vec2(xforce, yforce), b2Vec2(xpos, ypos), wake);
    }
  }
}

void physics_apply_impulse(int world, double xpos, double ypos, double ximpulse, double yimpulse, bool wake)
{
  get_fixture(sb2dworld, world);
  for (int i = 0; i < fixtures.size(); i++)
  {
    if (fixtures[i]->world == world)
    {
      fixtures[i]->body->ApplyLinearImpulse(b2Vec2(ximpulse, yimpulse), b2Vec2(xpos, ypos), wake);
    }
  }
}

void physics_apply_local_force(int id, double xlocal, double ylocal, double xforce, double yforce, bool wake)
{
  get_fixture(sb2dfixture, id);
  sb2dfixture->body->ApplyForce(b2Vec2(xforce, yforce), b2Vec2(xlocal, ylocal), wake);
}

void physics_apply_local_impulse(int id, double xlocal, double ylocal, double ximpulse, double yimpulse, bool wake)
{
  get_fixture(sb2dfixture, id);
  sb2dfixture->body->ApplyLinearImpulse(b2Vec2(ximpulse, yimpulse), b2Vec2(xlocal, ylocal), wake);
}

void physics_apply_local_torque(int id, double torque, bool wake)
{
  get_fixture(sb2dfixture, id);
  sb2dfixture->body->ApplyTorque(torque, wake);
}

void physics_pause_enable(bool pause)
{
  systemPaused = pause;
}

void physics_mass_properties(double mass, double local_center_x, double local_center_y, double inertia)
{
  // same as physics_fixture_mass_properties except it doesnt need an id, uses the currently bound fixture
  // of whatever is calling the function, im not writing it cause its stupid
}

void physics_draw_debug()
{
  // draws the currently bound fixture's shape with draw color, im not doing that let the end programmer have control
}

}
