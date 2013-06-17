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

#include "PS_particle.h"
#include "PS_particle_system.h"
#include "PS_particle_type.h"
#include "PS_particle_system_manager.h"
#include <cstddef>

using enigma::particle_system;
using enigma::particle_type;
using enigma::pt_manager;
using enigma::particle_type_manager;

namespace enigma_user {
  // Particles.

  void part_particles_create(int id, double x, double y, int particle_type_id, int number)
  {
    particle_system* p_s = enigma::get_particlesystem(id);
    if (p_s != NULL) {
      particle_type* p_t = enigma::get_particletype(particle_type_id);
      if (p_t != NULL) {
        p_s->create_particles(x, y, p_t, number);
      }
    }
  }
  void part_particles_create_color(int id, double x, double y, int particle_type_id, int color, int number)
  {
    particle_system* p_s = enigma::get_particlesystem(id);
    if (p_s != NULL) {
      particle_type* p_t = enigma::get_particletype(particle_type_id);
      if (p_t != NULL) {
        p_s->create_particles(x, y, p_t, number, true, color);
      }
    }
  }
  void part_particles_clear(int id)
  {
    particle_system* p_s = enigma::get_particlesystem(id);
    if (p_s != NULL) {
      for (std::vector<enigma::particle_instance>::iterator it = p_s->pi_list.begin(); it != p_s->pi_list.end(); it++)
      {
        particle_type* pt = it->pt;

        // Death handling.
        pt->particle_count--;
        if (pt->particle_count <= 0 && !pt->alive) {
          // Particle type is no longer used, delete it.
          int pid = pt->id;
          delete pt;
          enigma::pt_manager.id_to_particletype.erase(pid);
        }
      }
      p_s->pi_list.clear();
    }
  }
  int part_particles_count(int id)
  {
    particle_system* p_s = enigma::get_particlesystem(id);
    if (p_s != NULL) {
      return p_s->pi_list.size();
    }
    return 0;
  }
}

