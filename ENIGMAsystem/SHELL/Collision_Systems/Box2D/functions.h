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
// Linker shit right meow...
#include "Universal_System/instance_system_base.h"

bool place_free(double x,double y);
bool place_meeting(double x, double y, int object);
// End of linking shit to stfu 

/* Worlds */
int physics_world_create();
void physics_world_delete(int index);
void physics_world_gravity(int index, double gx, double gy);
void physics_world_update(int index); // extra function to control your world update, should be auto done inside our game loop
void physics_world_update_iterations();
void physics_world_update_speed();
void physics_world_draw_debug(); // draws all the fixtures and their rotations in the room for u, wants constants, fuck that

/* Fixtures */
int  physics_fixture_create(int world);
void physics_fixture_create(); // overloaded cause the default studio function uses the current rooms binded world
void physics_fixture_bind(); // binds a fixture to an object, im not writing this fuck YYG
void physics_fixture_set_collision_group(int id, int group);
void physics_fixture_delete(int id);

void physics_fixture_set_box_shape(int id, double halfwidth, double halfheight);
void physics_fixture_set_circle_shape(int id, double radius);
void physics_fixture_set_polygon_shape();
void physics_fixture_add_point();

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
void physics_fixture_set_mass(int id, double mass, double local_center_x, double local_center_y, double inertia);

double physics_fixture_get_angle(int id);
double physics_fixture_get_x(int id);
double physics_fixture_get_y(int id);

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

/* Miscellaneous */
void physics_test_overlap();
void physics_mass_properties(double mass, double local_center_x, double local_center_y, double inertia); 
void physics_draw_debug(); 
void physics_pause_enable(bool pause);
