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
void physics_fixture_create();
void physics_fixture_bind();
void physics_fixture_set_collision_group();
void physics_fixture_delete();

void physics_fixture_set_box_shape();
void physics_fixture_set_circle_shape();
void physics_fixture_set_polygon_shape();
void physics_fixture_add_point();

void physics_fixture_set_density();
void physics_fixture_set_friction();
void physics_fixture_set_linear_damping();
void physics_fixture_set_angular_damping();
void physics_fixture_set_restitution();
void physics_fixture_set_sensor();
void physics_fixture_set_kinematic();
void physics_fixture_set_awake();
