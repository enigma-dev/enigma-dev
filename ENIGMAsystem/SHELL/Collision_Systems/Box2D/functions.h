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

/* Fixtures */
int  physics_fixture_create(int world);
int  physics_fixture_create(); 
void physics_fixture_bind(int id); 
void physics_fixture_bind(); 
void physics_fixture_set_collision_group(int id, int group);
void physics_fixture_delete(int id);

void physics_fixture_set_box_shape(int id, double halfwidth, double halfheight);
void physics_fixture_set_circle_shape(int id, double radius);
void physics_fixture_set_polygon_shape(int id);
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

/* Forces */
void physics_apply_force(int world, double xpos, double ypos, double xforce, double yforce);
void physics_apply_force_radial(int world, double xpos, double ypos);
void physics_apply_impulse(int world, double xpos, double ypos, double ximpulse, double yimpulse);
void physics_apply_impulse_radial(int world, double xpos, double ypos);
void physics_apply_local_force(int id, double xlocal, double ylocal, double xforce, double yforce);
void physics_apply_local_force_radial(int world, double xpos, double ypos);
void physics_apply_local_impulse(int id, double xlocal, double ylocal, double ximpulse, double yimpulse);
void physics_apply_local_impulse_radial(int world, double xpos, double ypos);
void physics_apply_local_torque(int id, double torque);

/* Joints */

// constants for getting and setting joint data using physics_joint_get_value and physics_joint_set_value, use switch case
// see http://enigma-dev.org/docs/Wiki/Physics_Constant, for information on what they are
enum {
  phy_joint_anchor_1_x,
  phy_joint_anchor_1_y,
  phy_joint_anchor_2_x,	
  phy_joint_anchor_2_y,	
  phy_joint_reaction_force_x,
  phy_joint_reaction_force_y,
  phy_joint_reaction_torque,
  phy_joint_max_motor_force,
  phy_joint_max_motor_torque,
  phy_joint_motor_force,
  phy_joint_motor_speed,
  phy_joint_motor_torque,
  phy_joint_angle,
  phy_joint_translation,
  phy_joint_speed,
  phy_joint_length_1,
  phy_joint_length_2,
  phy_joint_damping_ratio,
  phy_joint_frequency
};

int physics_joint_create(int world);
void physics_joint_distance_create(int id, int fixture1, int fixture2);
void physics_joint_revolute_create(int id);
void physics_joint_prismatic_create(int id);
void physics_joint_pulley_create(int id);
void physics_joint_gear_create(int id);
void physics_joint_mouse_create(int id, int fixture);
void physics_joint_delete(int id);

void physics_joint_set_target(int id, double x, double y);

void physics_joint_enable_motor(int id);
void physics_joint_get_value(int id);
void physics_joint_set_value(int id);

/* Miscellaneous */
void physics_test_overlap();
void physics_mass_properties(double mass, double local_center_x, double local_center_y, double inertia); 
void physics_draw_debug(); 
void physics_pause_enable(bool pause);
