/********************************************************************************\
**                                                                              **
**  Copyright (C) 2012-2013 forthevin                                           **
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

#ifndef ENIGMA_PS_PARTICLETYPE
#define ENIGMA_PS_PARTICLETYPE

#include <map>
#include "PS_particle_enums.h"
#include "PS_particle_sprites.h"

namespace enigma
{
  enum color_mode {
    one_color, two_color, three_color, mix_color, rgb_color, hsv_color
  };
  enum alpha_mode {
    one_alpha, two_alpha, three_alpha
  };
  struct particle_type
  {
    // If the particle count becomes 0, and the type is not alive,
    // the memory of the particle type should be freed, since it is no longer used.
    int particle_count; // The number of particles of this particle type.
    bool alive; // Whether the type is still alive.
    int id; // Id of the particle type.

    // Shape.
    bool is_particle_sprite; // Whether an internal particle sprite is used or not.
    particle_sprite* part_sprite;
    int sprite_id;
    bool sprite_animated, sprite_stretched, sprite_random;
    double size_min, size_max;
    double size_incr, size_wiggle;
    double xscale, yscale;
    double ang_min, ang_max;
    double ang_incr, ang_wiggle;
    bool ang_relative;
    // Color and blending.
    color_mode c_mode;
    int color1;
    int color2;
    int color3;
    unsigned char rmin, rmax, gmin, gmax, bmin, bmax;
    unsigned char hmin, hmax, smin, smax, vmin, vmax;
    alpha_mode a_mode;
    double alpha1;
    double alpha2;
    double alpha3;
    bool blend_additive;
    // Life and death.
    int life_min, life_max; // 1 <= life_min <= life_max.
    bool step_on;
    int step_particle_id;
    int step_number;
    bool death_on;
    int death_particle_id;
    int death_number;
    // Motion.
    double speed_min, speed_max;
    double speed_incr, speed_wiggle;
    double dir_min, dir_max;
    double dir_incr, dir_wiggle;
    double grav_amount, grav_dir;
    // Protection.
    bool hidden;
  };

  struct particle_type_manager
  {
    int max_id;
    std::map<int,particle_type*> id_to_particletype;
  };

  extern particle_type_manager pt_manager;
  particle_type* get_particletype(int id); // Null if not found.

  void initialize_particle_type(particle_type* pt);
}

#endif // ENIGMA_PS_PARTICLETYPE

