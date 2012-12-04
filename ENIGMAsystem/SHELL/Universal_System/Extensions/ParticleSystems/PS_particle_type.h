/********************************************************************************\
**                                                                              **
**  Copyright (C) 2012 forthevin                                                **
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

namespace enigma
{
  struct particle_type
  {
    // If the particle count becomes 0, and the type is not alive,
    // the type should be removed, since it is no longer used.
    int particle_count; // The number of particles of this particle type.
    bool alive; // Whether the type is still alive.

    int life_min, life_max; // life_min <= life_max.
    double speed_min, speed_max;
    double speed_incr, speed_wiggle;
    double dir_min, dir_max;
    double dir_incr, dir_wiggle;
  };

  struct particle_type_manager
  {
    int max_id;
    std::map<int,particle_type*> id_to_particletype;
  };

  particle_type_manager* get_particle_type_manager();
}

#endif // ENIGMA_PS_PARTICLETYPE

