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

#include <vector>
using std::vector;

#include <Box2D/Box2D.h>
#include "Box2DJoint.h"
#include "SB2Dfunctions.h"
#include "SB2Djoints.h"

vector<jointInstance> joints(0);

namespace enigma_user
{

void physics_joint_distance_create(int inst1, int inst2, double w_anchor1_x, double w_anchor1_y, double w_anchor2_x, double w_anchor2_y, bool col){

}

void physics_joint_revolute_create(int inst1, int inst2, double w_anchor_x, double w_anchor_y, double ang_min_limt, double ang_max_limit, double ang_limit, double max_motor_torque, double motor_speed, bool motor, bool col){

}

void physics_joint_prismatic_create(int inst1, int inst2, double w_anchor_x, double w_anchor_y, double w_axis_x, double w_axis_y, double lower_trans_limit, double upper_trans_limit, double limit, double max_motor_force, double motor_speed, bool motor, bool col){

}

void physics_joint_pulley_create(int inst1, int inst2, double w_anchor1_x, double w_anchor1_y, double w_anchor2_x, double w_anchor2_y, double l_anchor1_x, double l_anchor1_y, double l_anchor2_x, double l_anchor2_y, double ratio, double max_len1, double max_len2, bool col){

}

void physics_joint_gear_create(int inst1, int inst2, int joint_1, int joint_2, double ratio){

}

void physics_joint_delete(int id){

}

void physics_joint_enable_motor(int id){

}

evariant physics_joint_get_value(int id, int field){

}

void physics_joint_set_value(int id, int field, evariant value){

}

}
