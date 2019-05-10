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

#include "PS_particle_system_manager.h"
#include "PS_particle_system.h"
#include "PS_particle_updatedraw.h"
#include "PS_particle_depths.h"
#include "PS_particle.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Universal_System/Instances/callbacks_events.h"

namespace enigma
{
  static void internal_update_particlesystems()
  {
    std::map<int,particle_system*>::iterator end = ps_manager.id_to_particlesystem.end();
    for (std::map<int,particle_system*>::iterator it = ps_manager.id_to_particlesystem.begin(); it != end; it++)
    {
      if ((*it).second->auto_update) {
        (*it).second->update_particlesystem();
      }
    }
  }

  static void internal_draw_particlesystems(double high, double low)
  {
    high = std::max(high, low); // Ensure consistency of arguments.
    const std::map<double,particle_depth_layer>::iterator ne_end = negated_particle_depths.upper_bound(-low);
    const std::map<double,particle_depth_layer>::iterator ne_upper_it = negated_particle_depths.find(-low);
    for (std::map<double,particle_depth_layer>::iterator ne_it = negated_particle_depths.lower_bound(-high); ne_it != ne_end && ne_it != ne_upper_it; ne_it++)
    {
      std::set<int>::iterator ps_ids_end = (*ne_it).second.particlesystem_ids.end();
      for (std::set<int>::iterator  ps_ids_it = (*ne_it).second.particlesystem_ids.begin();  ps_ids_it != ps_ids_end;  ps_ids_it++)
      {
        std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(*ps_ids_it);
        if (ps_it != ps_manager.id_to_particlesystem.end()) {
          if ((*ps_it).second->auto_draw) {
            (*ps_it).second->draw_particlesystem();
          }
        }
      }
    }
  }

  static inline void internal_clear_effects() {
    effect_clear();
  }

  particles_implementation part_impl;
  bool initialized;
  void initialize_particle_systems_drawing()
  {
    if (!initialized) {
      initialized = true;
      part_impl.draw_particlesystems = &internal_draw_particlesystems;
      set_particles_implementation(&part_impl);
      register_callback_particle_updating(internal_update_particlesystems);
      register_callback_clean_up_roomend(internal_clear_effects);
      particle_bridge::initialize_particle_bridge();
    }
  }
}
