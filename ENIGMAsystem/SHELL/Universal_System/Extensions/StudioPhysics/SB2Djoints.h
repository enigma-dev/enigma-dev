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

#ifndef ENIGMA_STUDIO_PHYSICS_JOINTS_H
#define ENIGMA_STUDIO_PHYSICS_JOINTS_H

#include "Universal_System/var4.h"
#include <Box2D/Box2D.h>
#include "SB2Dfunctions.h"

// constants for getting and setting joint data using physics_joint_get_value and physics_joint_set_value, use switch case
// see http://enigma-dev.org/docs/Wiki/, for information on what they are
namespace enigma_user {
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
}

namespace enigma_user
{

void physics_joint_distance_create(int inst1, int inst2, double w_anchor1_x, double w_anchor1_y, double w_anchor2_x, double w_anchor2_y, bool col);
void physics_joint_revolute_create(int inst1, int inst2, double w_anchor_x, double w_anchor_y, double ang_min_limt, double ang_max_limit, double ang_limit, double max_motor_torque, double motor_speed, bool motor, bool col);
void physics_joint_prismatic_create(int inst1, int inst2, double w_anchor_x, double w_anchor_y, double w_axis_x, double w_axis_y, double lower_trans_limit, double upper_trans_limit, double limit, double max_motor_force, double motor_speed, bool motor, bool col);
void physics_joint_pulley_create(int inst1, int inst2, double w_anchor1_x, double w_anchor1_y, double w_anchor2_x, double w_anchor2_y, double l_anchor1_x, double l_anchor1_y, double l_anchor2_x, double l_anchor2_y, double ratio, double max_len1, double max_len2, bool col);
void physics_joint_gear_create(int inst1, int inst2, int joint_1, int joint_2, double ratio);
void physics_joint_delete(int id);

void physics_joint_enable_motor(int id);
evariant physics_joint_get_value(int id, int field);
void physics_joint_set_value(int id, int field, evariant value);

}

#endif // ENIGMA_STUDIO_PHYSICS_JOINTS_H
