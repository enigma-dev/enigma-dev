/********************************************************************************\
**                                                                              **
**  Copyright (C) 2013 forthevin                                                **
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

#ifndef ENIGMA_PS_ACTIONS
#define ENIGMA_PS_ACTIONS

extern bool argument_relative;

namespace enigma_user {
  void action_effect(int effect_type, double x, double y, int size, int color, int where);

  void action_partsyst_create(double depth);
  void action_partsyst_destroy();
  void action_partsyst_clear();

  // The particle "type" is in [0..15].

  void action_parttype_create(int type, int shape, int sprite, double min_size, double max_size, double size_increment);
  // color_shape: 0=mixed, 1=changing.
  void action_parttype_color(int type, int color_shape, int color1, int color2, double alpha1, double alpha2);
  void action_parttype_life(int type, int min_life, int max_life);
  void action_parttype_speed(int type, double min_speed, double max_speed, double min_dir, double max_dir, double friction);
  void action_parttype_gravity(int type, double amount, double direction);
  void action_parttype_secondary(int primary_type, int step_type, int step_count, int death_type, int death_count);

  // "emitter" is in [0..7].
  void action_partemit_create(int emitter, int shape, double x_left, double x_right, double y_top, double y_bottom);
  void action_partemit_destroy(int emitter);
  void action_partemit_burst(int emitter, int particle_type, int amount);
  void action_partemit_stream(int emitter, int particle_type, int amount);
}

#endif // ENIGMA_PS_ACTIONS

