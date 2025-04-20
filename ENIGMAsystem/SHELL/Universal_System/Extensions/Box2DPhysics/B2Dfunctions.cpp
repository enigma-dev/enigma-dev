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

#include "Universal_System/Instances/callbacks_events.h"
#include "Universal_System/scalar.h"

#include <Box2D/Box2D.h>
#include "Box2DWorld.h"
#include "Box2DShape.h"
#include "B2Dfunctions.h"
bool systemPaused = false;

vector<B2DWorld*> b2dworlds(0);
vector<B2DBody*> b2dbodies;

void B2DWorld::world_update()
{
  if (!systemPaused && !paused) {
    world->Step(timeStep, velocityIterations, positionIterations);
    world->ClearForces();
  }
}

#include <cmath>
#include <cstdlib>
#include <string>
using std::string;

namespace enigma {
  bool has_been_initialized = false;

  void update_worlds_automatically() {
    vector<B2DWorld*>::iterator it_end = b2dworlds.end();
    for (vector<B2DWorld*>::iterator it = b2dworlds.begin(); it != it_end; it++) {
      (*it)->world_update();
    }
  }

  // Should be called whenever a world is created.
  void init_box2d_system() {
    if (!has_been_initialized) {
      has_been_initialized = true;

      // Register callback.
      register_callback_before_collision_event(update_worlds_automatically);
    }
  }
}

namespace enigma_user
{

void b2d_world_create(int pixeltometerscale)
{
  enigma::init_box2d_system();
  /** studio's messed up world creation just auto binds it to the current room
      thats dumb, thus why i overloaded the function and provided an extra
      function for setting the pixel to metre scale
  **/
}

int b2d_world_create()
{
  enigma::init_box2d_system();
  int i = b2dworlds.size();
  b2dworlds.push_back(new B2DWorld());
  return i;
}

void b2d_world_delete(int index)
{
  get_world(b2dworld, index);
  delete b2dworld;
}

void b2d_world_pause_enable(int index, bool paused)
{
  get_world(b2dworld, index);
  b2dworld->paused = paused;
}

void b2d_world_dump(int index)
{
  get_world(b2dworld, index);
  b2dworld->world->Dump();
}

void b2d_world_update(int index)
{
  get_world(b2dworld, index);
  b2dworld->world_update();
}

void b2d_world_update_settings(int index, double timeStep, int velocityIterations, int positionIterations)
{
  get_world(b2dworld, index);
  b2dworld->timeStep = timeStep;
  b2dworld->velocityIterations = velocityIterations;
  b2dworld->positionIterations = positionIterations;
}

void b2d_world_update_iterations(int iterationsperstep)
{
  // provide overloads if we do adopt this system so that you can still
  // change indexed worlds
}

void b2d_world_update_iterations(int index, int iterationsperstep)
{
  // this sets the number of iterations the physics system takes each step
  // not needed for the current implementation
}

void b2d_world_update_speed(int updatesperstep)
{
  // provide overloads if we do adopt this system so that you can still
  // change indexed worlds
}

void b2d_world_update_speed(int index, int updatesperstep)
{
  // this sets the number of updates the physics system takes each step
  // not needed for the current implementation
}

void b2d_world_draw_debug()
{
  // draws all the fixtures and their rotations in the room for u, wants constants, im not doing that
  // end programmer can do it themselves
}

void b2d_world_clear_forces(int index)
{
  get_world(b2dworld, index);
  b2dworld->world->ClearForces();
}

void b2d_world_set_gravity(int index, double gx, double gy)
{
  get_world(b2dworld, index);
  b2dworld->world->SetGravity(b2Vec2(gx, gy));
}

void b2d_world_set_scale(int index, int pixelstometers)
{
  get_world(b2dworld, index);
  b2dworld->pixelstometers = pixelstometers;
}

void b2d_world_set_sleep(int index, bool sleeping)
{
  get_world(b2dworld, index);
  b2dworld->world->SetAllowSleeping(sleeping);
}

void b2d_world_set_warmstarting(int index, bool warmstarting)
{
  get_world(b2dworld, index);
  b2dworld->world->SetWarmStarting(warmstarting);
}

void b2d_world_set_continuous(int index, bool continuous)
{
  get_world(b2dworld, index);
  b2dworld->world->SetContinuousPhysics(continuous);
}

void b2d_world_set_substepping(int index, bool substepping)
{
  get_world(b2dworld, index);
  b2dworld->world->SetSubStepping(substepping);
}

void b2d_world_set_clearforces(int index, bool autoclear)
{
  get_world(b2dworld, index);
  b2dworld->world->SetAutoClearForces(autoclear);
}

int b2d_world_get_scale(int index)
{
  get_worldr(b2dworld, index, -1);
  return b2dworld->pixelstometers;
}

bool b2d_world_get_sleep(int index)
{
  get_worldr(b2dworld, index, false);
  return b2dworld->world->GetAllowSleeping();
}

bool b2d_world_get_warmstarting(int index)
{
  get_worldr(b2dworld, index, false);
  return b2dworld->world->GetWarmStarting();
}

bool b2d_world_get_continuous(int index)
{
  get_worldr(b2dworld, index, false);
  return b2dworld->world->GetContinuousPhysics();
}

bool b2d_world_get_substepping(int index)
{
  get_worldr(b2dworld, index, false);
  return b2dworld->world->GetSubStepping();
}

int b2d_world_get_proxy_count(int index)
{
  get_worldr(b2dworld, index, -1);
  return b2dworld->world->GetProxyCount();
}

int b2d_world_get_body_count(int index)
{
  get_worldr(b2dworld, index, -1);
  return b2dworld->world->GetBodyCount();
}

int b2d_world_get_joint_count(int index)
{
  get_worldr(b2dworld, index, -1);
  return b2dworld->world->GetJointCount();
}

int b2d_world_get_contect_count(int index)
{
  get_worldr(b2dworld, index, -1);
  return b2dworld->world->GetContactCount();
}

bool b2d_world_get_locked(int index)
{
  get_worldr(b2dworld, index, false);
  return b2dworld->world->IsLocked();
}

bool b2d_world_get_clearforces(int index)
{
  get_worldr(b2dworld, index, false);
  return b2dworld->world->GetAutoClearForces();
}

int b2d_body_create(int world)
{
  get_worldr(b2dworld, world, -1);
  int i = b2dbodies.size();
  B2DBody* b2dbody = new B2DBody();
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  b2dbody->body = b2dworld->world->CreateBody(&bodyDef);
  b2dbodies.push_back(b2dbody);
  b2dbodies[i]->world = world;
  return i;
}

int b2d_body_create_shape(int world, int shape)
{
  int i = b2d_body_create(world);
  int f = b2d_fixture_create(i, shape);
  return i;
}

int b2d_body_create_box(int world, double halfwidth, double halfheight)
{
  int i = b2d_body_create(world);
  int s = b2d_shape_create();
  b2d_shape_box(s, halfwidth, halfheight);
  int f = b2d_fixture_create(i, s);
  return i;
}

int b2d_body_create_circle(int world, double radius)
{
  int i = b2d_body_create(world);
  int s = b2d_shape_create();
  b2d_shape_circle(s, radius);
  int f = b2d_fixture_create(i, s);
  return i;
}

void b2d_body_bind(int id, int obj)
{
  get_body(b2dbody, id);
}

void b2d_body_delete(int id)
{
  get_body(b2dbody, id);
  delete b2dbody;
}

void b2d_body_dump(int id)
{
  get_body(b2dbody, id);
  b2dbody->body->Dump();
}


void b2d_body_set_collision_group(int id, int group)
{
  get_body(b2dbody, id);
  // sets the collision group used to make parts of things not collide, like a ragdoll for
  // instance should not collide with itself
  b2Filter newfilter;
  newfilter.groupIndex = group;
  for (int i = 0; i < b2dbody->fixtures.size(); i++) {
    b2dfixtures[b2dbody->fixtures[i]]->fixture->SetFilterData(newfilter);
  }
}

void b2d_body_set_transform(int id, double x, double y, double angle)
{
  get_body(b2dbody, id);
  b2dbody->body->SetTransform(b2Vec2(x, y), cs_angle_to_radians(angle));
}

void b2d_body_set_position(int id, double x, double y)
{
  get_body(b2dbody, id);
  b2dbody->body->SetTransform(b2Vec2(x, y), b2dbodies[id]->body->GetAngle());
}

void b2d_body_set_angle(int id, double angle)
{
  get_body(b2dbody, id);
  b2dbody->body->SetTransform(b2dbodies[id]->body->GetPosition(), cs_angle_to_radians(angle));
}

void b2d_body_set_angle_fixed(int id, bool fixed)
{
  get_body(b2dbody, id);
  b2dbody->body->SetFixedRotation(fixed);
}

void b2d_body_set_density(int id, double density)
{
  get_body(b2dbody, id);
  for (int i = 0; i < b2dbody->fixtures.size(); i++) {
    b2dfixtures[b2dbody->fixtures[i]]->fixture->SetDensity(density);
  }
  b2dbody->body->ResetMassData();
}

void b2d_body_set_friction(int id, double friction)
{
  get_body(b2dbody, id);
  for (int i = 0; i < b2dbody->fixtures.size(); i++) {
    b2dfixtures[b2dbody->fixtures[i]]->fixture->SetFriction(friction);
  }
  b2dbody->body->ResetMassData();
}

void b2d_body_set_restitution(int id, double restitution)
{
  get_body(b2dbody, id);
  for (int i = 0; i < b2dbody->fixtures.size(); i++) {
    b2dfixtures[b2dbody->fixtures[i]]->fixture->SetRestitution(restitution);
  }
}

void b2d_body_set_sensor(int id, bool state)
{
  get_body(b2dbody, id);
  for (int i = 0; i < b2dbody->fixtures.size(); i++) {
    b2dfixtures[b2dbody->fixtures[i]]->fixture->SetSensor(state);
  }
}

void b2d_body_set_linear_damping(int id, double damping)
{
  get_body(b2dbody, id);
  b2dbody->body->SetLinearDamping(damping);
}

void b2d_body_set_angular_damping(int id, double damping)
{
  get_body(b2dbody, id);
  b2dbody->body->SetAngularDamping(damping);
}

void b2d_body_set_awake(int id, bool state)
{
  get_body(b2dbody, id);
  b2dbody->body->SetAwake(state);
}

void b2d_body_set_bullet(int id, bool bullet)
{
  get_body(b2dbody, id);
  b2dbody->body->SetBullet(bullet);
}

void b2d_body_set_sleep(int id, bool allowsleep)
{
  get_body(b2dbody, id);
  b2dbody->body->SetSleepingAllowed(allowsleep);
}

void b2d_body_set_massdata(int id, double mass, double local_center_x, double local_center_y, double inertia)
{
  get_body(b2dbody, id);
  b2MassData lMassData;
  b2dbody->body->GetMassData(&lMassData);
  lMassData.mass = mass;
  lMassData.center.Set(local_center_x, local_center_y);
  lMassData.I = inertia;
  b2dbody->body->SetMassData(&lMassData);
}

void b2d_body_reset_massdata(int id)
{
  get_body(b2dbody, id);
  b2dbody->body->ResetMassData();
}

void b2d_body_set_static(int id)
{
  get_body(b2dbody, id);
  b2dbody->body->SetType(b2_staticBody);
  b2dbody->body->ResetMassData();
}

void b2d_body_set_kinematic(int id)
{
  get_body(b2dbody, id);
  b2dbody->body->SetType(b2_kinematicBody);
}

void b2d_body_set_dynamic(int id)
{
  get_body(b2dbody, id);
  b2dbody->body->SetType(b2_dynamicBody);
}

double b2d_body_get_angle(int id)
{
  get_bodyr(b2dbody, id, -1);
  return -cs_angle_from_radians(b2dbody->body->GetAngle());
}

double b2d_body_get_x(int id)
{
  get_bodyr(b2dbody, id, -1);
  return b2dbody->body->GetPosition().x;
}

double b2d_body_get_y(int id)
{
  get_bodyr(b2dbody, id, -1);
  return b2dbody->body->GetPosition().y;
}

double b2d_body_get_mass(int id)
{
  get_bodyr(b2dbody, id, -1);
  b2MassData lMassData;
  b2dbody->body->GetMassData(&lMassData);
  return lMassData.mass;
}

double b2d_body_get_center_x(int id)
{
  get_bodyr(b2dbody, id, -1);
  b2MassData lMassData;
  b2dbody->body->GetMassData(&lMassData);
  return lMassData.center.x;
}

double b2d_body_get_center_y(int id)
{
  get_bodyr(b2dbody, id, -1);
  b2MassData lMassData;
  b2dbody->body->GetMassData(&lMassData);
  return lMassData.center.y;
}

double b2d_body_get_inertia(int id)
{
  get_bodyr(b2dbody, id, -1);
  b2MassData lMassData;
  b2dbody->body->GetMassData(&lMassData);
  return lMassData.I;
}

double b2d_body_get_angular_velocity(int id)
{
  get_bodyr(b2dbody, id, -1);
  return b2dbody->body->GetAngularVelocity();
}

bool b2d_body_get_awake(int id)
{
  get_bodyr(b2dbody, id, false);
  return b2dbody->body->IsActive();
}

bool b2d_body_get_sleep(int id)
{
  get_bodyr(b2dbody, id, false);
  return b2dbody->body->IsSleepingAllowed();
}

bool b2d_body_get_angle_fixed(int id)
{
  get_bodyr(b2dbody, id, false);
  return b2dbody->body->IsFixedRotation();
}

bool b2d_body_get_bullet(int id)
{
  get_bodyr(b2dbody, id, false);
  return b2dbody->body->IsBullet();
}

int b2d_body_get_fixture(int id, int fid)
{
  get_bodyr(b2dbody, id, -1);
  return b2dbody->fixtures[fid];
}

/************** Forces **************/
void b2d_body_apply_force(int id, double xpos, double ypos, double xforce, double yforce, bool wake)
{
  get_body(b2dbody, id);
  b2dbody->body->ApplyForce(b2Vec2(xforce, yforce), b2Vec2(xpos, ypos), wake);
}

void b2d_body_apply_force_center(int id, double xforce, double yforce, bool wake)
{
  get_body(b2dbody, id);
  b2dbody->body->ApplyForceToCenter(b2Vec2(xforce, yforce), wake);
}

void b2d_body_apply_torque(int id, double torque, bool wake)
{
  get_body(b2dbody, id);
  b2dbody->body->ApplyTorque(torque, wake);
}

void b2d_body_apply_impulse_linear(int id, double xpos, double ypos, double ximpulse, double yimpulse, bool wake)
{
  get_body(b2dbody, id);
  b2dbody->body->ApplyLinearImpulse(b2Vec2(ximpulse, yimpulse), b2Vec2(xpos, ypos), wake);
}

void b2d_body_apply_impulse_angular(int id, double impulse, bool wake)
{
  get_body(b2dbody, id);
  b2dbody->body->ApplyAngularImpulse(impulse, wake);
}

void b2d_world_apply_force(int world, double xpos, double ypos, double xforce, double yforce, bool wake)
{
  get_world(b2dworld, world);
  for (int i = 0; i < b2dbodies.size(); i++)
  {
    if (b2dbodies[i]->world == world)
    {
      b2dbodies[i]->body->ApplyForce(b2Vec2(xforce, yforce), b2Vec2(xpos, ypos), wake);
    }
  }
}

void b2d_world_apply_impulse(int world, double xpos, double ypos, double ximpulse, double yimpulse, bool wake)
{
  get_body(b2dworld, world);
  for (int i = 0; i < b2dbodies.size(); i++)
  {
    if (b2dbodies[i]->world == world)
    {
      b2dbodies[i]->body->ApplyLinearImpulse(b2Vec2(ximpulse, yimpulse), b2Vec2(xpos, ypos), wake);
    }
  }
}

void b2d_pause_system(bool pause)
{
  systemPaused = pause;
}

}
