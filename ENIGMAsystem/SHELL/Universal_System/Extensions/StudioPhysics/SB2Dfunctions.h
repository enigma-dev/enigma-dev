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

namespace enigma_user
{

/************** Worlds **************/

void physics_world_create(int pixeltometerscale);
int physics_world_create();
void physics_world_delete(int index);
void physics_world_pause_enable(int index, bool paused);
void physics_world_scale(int index, int pixelstometers);
void physics_world_gravity(int index, double gx, double gy);
void physics_world_update(int index); 
void physics_world_update_settings(int index, double timeStep, int velocityIterations, int positionIterations);
void physics_world_update_iterations(int index, int iterationsperstep);
void physics_world_update_speed(int index, int updatesperstep);
void physics_world_draw_debug();

/************** Fixtures **************/

int  physics_fixture_create(int world);
int  physics_fixture_create(); 
void physics_fixture_bind(int id); 
void physics_fixture_bind(); 
void physics_fixture_set_collision_group(int id, int group);
void physics_fixture_delete(int id);

void physics_fixture_set_box_shape(int id, double halfwidth, double halfheight);
void physics_fixture_set_circle_shape(int id, double radius);
void physics_fixture_set_polygon_shape(int id);
void physics_fixture_set_edge_shape(int id, bool adjstart = true, bool adjend = true); 
void physics_fixture_add_point(int id, double x, double y);

void physics_fixture_set_transform(int id, double x, double y, double angle);
void physics_fixture_set_position(int id, double x, double y);
void physics_fixture_set_angle(int id, double angle);
void physics_fixture_set_density(int id, double density);
void physics_fixture_set_friction(int id, double friction);
void physics_fixture_set_linear_damping(int id, double damping);
void physics_fixture_set_angular_damping(int id, double damping);
void physics_fixture_set_restitution(int id, double restitution);
void physics_fixture_set_sensor(int id, bool state);
void physics_fixture_set_static(int id);
void physics_fixture_set_kinematic(int id);
void physics_fixture_set_dynamic(int id);
void physics_fixture_set_awake(int id, bool state);
void physics_fixture_set_sleep(int id, bool allowsleep);
void physics_fixture_mass_properties(int id, double mass, double local_center_x, double local_center_y, double inertia);

double physics_fixture_get_angle(int id);
double physics_fixture_get_x(int id);
double physics_fixture_get_y(int id);
double physics_fixture_get_mass(int id);
double physics_fixture_get_center_x(int id);
double physics_fixture_get_center_y(int id);
double physics_fixture_get_inertia(int id);

/************** Forces **************/

void physics_apply_force(int world, double xpos, double ypos, double xforce, double yforce);
void physics_apply_force_radial(int world, double xpos, double ypos);
void physics_apply_impulse(int world, double xpos, double ypos, double ximpulse, double yimpulse);
void physics_apply_impulse_radial(int world, double xpos, double ypos);
void physics_apply_local_force(int id, double xlocal, double ylocal, double xforce, double yforce);
void physics_apply_local_force_radial(int world, double xpos, double ypos);
void physics_apply_local_impulse(int id, double xlocal, double ylocal, double ximpulse, double yimpulse);
void physics_apply_local_impulse_radial(int world, double xpos, double ypos);
void physics_apply_local_torque(int id, double torque);

/************** Miscellaneous **************/
void physics_test_overlap();
void physics_mass_properties(double mass, double local_center_x, double local_center_y, double inertia);
void physics_draw_debug(); 
void physics_pause_enable(bool pause);

}

#endif // _ENIGMA_STUDIO_PHYSICS_FUNCTIONS__H

