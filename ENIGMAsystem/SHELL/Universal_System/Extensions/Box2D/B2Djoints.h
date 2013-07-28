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

#ifndef _B2D_JOINTS__H
#define _B2D_JOINTS__H

#include <Box2D/Box2D.h>

struct B2DJoint {
  int worldid;
  b2Joint* joint;
  b2MouseJoint* mousejoint;

  B2DJoint()
  {
  }

  ~B2DJoint()
  {
    //worlds[worldid].world->DestroyJoint(joint);
  }
}; 
extern vector<B2DJoint*> b2djoints;

// constants for getting and setting joint data using b2d_joint_get_value and b2d_joint_set_value, use switch case
// see http://enigma-dev.org/docs/Wiki/, for information on what they are
namespace enigma_user
{

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

int b2d_joint_create(int world);
void b2d_joint_distance_create(int id, int fixture1, int fixture2,  bool collide_connected = true);
void b2d_joint_revolute_create(int id);
void b2d_joint_prismatic_create(int id);
void b2d_joint_pulley_create(int id);
void b2d_joint_gear_create(int id);
void b2d_joint_mouse_create(int id, int fixture);
void b2d_joint_delete(int id);

void b2d_joint_set_target(int id, double x, double y);
void b2d_joint_enable_motor(int id);
void b2d_joint_get_value(int id);
void b2d_joint_set_value(int id);

}

#endif // _ENIGMA_B2D_JOINTS__H

