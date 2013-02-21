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
// Linker shit right meow...
#include "Universal_System/instance_system_base.h"

bool place_free(double x,double y);
bool place_meeting(double x, double y, int object);
// End of linking shit to stfu 

void physics_test();

/* Worlds */
int physics_world_create();
void physics_world_gravity();
void physics_world_update(int index);

void physics_world_update_iterations();
void physics_world_update_speed();
void physics_world_draw_debug();
void physics_draw_debug();
void physics_pause_enable();

/* Fixtures */
int physics_fixture_create();
void physics_fixture_bind();
void physics_fixture_set_collision_group();
void physics_fixture_delete();

void physics_fixture_set_box_shape(int id, float halfwidth, float halfheight);
void physics_fixture_set_circle_shape(int id, float radius);
void physics_fixture_set_polygon_shape();
void physics_fixture_add_point();

void physics_fixture_set_position(int id, float x, float y);
void physics_fixture_set_density(int id, float density);
void physics_fixture_set_friction(int id, float friction);
void physics_fixture_set_linear_damping();
void physics_fixture_set_angular_damping();
void physics_fixture_set_restitution();
void physics_fixture_set_sensor();

float physics_fixture_get_angle(int id);
float physics_fixture_get_x(int id);
float physics_fixture_get_y(int id);

/* Forces */
void physics_apply_force();
void physics_apply_impulse();
void physics_apply_local_force();
void physics_apply_local_impulse();
void physics_apply_torque();

/* Joints */
void physics_joint_distance_create();
void physics_joint_revolute_create();
void physics_joint_prismatic_create();
void physics_joint_pulley_create();
void physics_joint_gear_create();
void physics_joint_delete();

void physics_joint_enable_motor();
void physics_joint_get_value();
void physics_joint_set_value();
void physics_fixture_set_kinematic();
void physics_fixture_set_awake();

/* Miscellaneous */
void physics_test_overlap();
void physics_mass_properties();
