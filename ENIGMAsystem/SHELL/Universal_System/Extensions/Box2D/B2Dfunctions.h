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

#ifndef ENIGMA_B2D_FUNCTIONS__H
#define ENIGMA_B2D_FUNCTIONS__H

#include <vector>
using std::vector;

#include <Box2D/Box2D.h>
#include "B2Dshapes.h"

namespace enigma_user
{

/************** Worlds **************/

void b2d_world_create(int pixeltometerscale);
int b2d_world_create();
void b2d_world_delete(int index);
void b2d_world_pause_enable(int index, bool paused);
void b2d_world_update(int index); 
void b2d_world_dump(int index);
void b2d_world_update_settings(int index, double timeStep, int velocityIterations, int positionIterations);
void b2d_world_update_iterations(int index, int iterationsperstep);
void b2d_world_update_speed(int index, int updatesperstep);
void b2d_world_clear_forces(int index);
void b2d_world_set_gravity(int index, double gx, double gy);
void b2d_world_set_scale(int index, int pixelstometers);
void b2d_world_set_sleep(int index, bool sleeping);
void b2d_world_set_warmstarting(int index, bool warmstarting);
void b2d_world_set_continuous(int index, bool continuous);
void b2d_world_set_substepping(int index, bool substepping);
void b2d_world_set_clearforces(int index, bool autoclear);
int b2d_world_get_scale(int index);
bool b2d_world_get_sleep(int index);
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

int  b2d_body_create(int world = 0);
int  b2d_body_create_shape(int world, int shape);
int  b2d_body_create_box(int world, double halfwidth, double halfheight);
int  b2d_body_create_circle(int world, double radius);
void b2d_body_bind(int id, int obj); 
void b2d_body_delete(int id);
void b2d_body_dump(int id);

void b2d_body_set_transform(int id, double x, double y, double angle);
void b2d_body_set_position(int id, double x, double y);
void b2d_body_set_angle(int id, double angle);
void b2d_body_set_angle_fixed(int id, bool fixed);
void b2d_body_set_density(int id, double density);
void b2d_body_set_friction(int id, double friction);
void b2d_body_set_restitution(int id, double restitution);
void b2d_body_set_sensor(int id, bool state);
void b2d_body_set_linear_damping(int id, double damping);
void b2d_body_set_angular_damping(int id, double damping);
void b2d_body_set_static(int id);
void b2d_body_set_kinematic(int id);
void b2d_body_set_dynamic(int id);
void b2d_body_set_awake(int id, bool state);
void b2d_body_set_bullet(int id, bool bullet);
void b2d_body_set_sleep(int id, bool allowsleep);
void b2d_body_set_collision_group(int id, int group);
void b2d_body_set_massdata(int id, double mass, double local_center_x, double local_center_y, double inertia);
void b2d_body_reset_massdata(int id);

double b2d_body_get_angle(int id);
double b2d_body_get_x(int id);
double b2d_body_get_y(int id);
double b2d_body_get_mass(int id);
double b2d_body_get_center_x(int id);
double b2d_body_get_center_y(int id);
double b2d_body_get_inertia(int id);

double b2d_body_get_angular_velocity(int id);
bool b2d_body_get_awake(int id);
bool b2d_body_get_sleep(int id);
bool b2d_body_get_angle_fixed(int id);
bool b2d_body_get_bullet(int id);
int b2d_body_get_fixture(int id, int fid);

/************** Forces **************/
void b2d_body_apply_force(int id, double xpos, double ypos, double xforce, double yforce);
void b2d_body_apply_force_center(int id, double xforce, double yforce);
void b2d_body_apply_torque(int id, double torque);
void b2d_body_apply_impulse_linear(int id, double xpos, double ypos, double ximpulse, double yimpulse);
void b2d_body_apply_impulse_angular(int id, double impulse);

void b2d_world_apply_force(int world, double xpos, double ypos, double xforce, double yforce);
void b2d_world_apply_force_radial(int world, double xpos, double ypos);
void b2d_world_apply_impulse(int world, double xpos, double ypos, double ximpulse, double yimpulse);
void b2d_world_apply_impulse_radial(int world, double xpos, double ypos);

/************** Miscellaneous **************/
void b2d_draw_debug(); 
void b2d_pause_system(bool pause);

}

#endif // ENIGMA_B2D_FUNCTIONS__H

