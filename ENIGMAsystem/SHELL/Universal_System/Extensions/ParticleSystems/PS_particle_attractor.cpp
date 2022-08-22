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

#include "PS_particle_system.h"
#include "PS_particle_system_manager.h"
#include "PS_particle.h"
#include <cstdlib>
#include <algorithm>
#include <cmath>

namespace enigma
{
  void particle_attractor::initialize()
  {
    x = 0, y = 0;
    force_strength = 0;
    dist_effect = 0;
    force_kind = ps_fo_constant;
    additive = false;
  }
  particle_attractor* create_particle_attractor()
  {
    particle_attractor* pa = new particle_attractor();
    pa->initialize();
    return pa;
  }
  void particle_attractor::clear_particle_attractor()
  {
    initialize();
  }
  void particle_attractor::set_position(double p_x, double p_y)
  {
    x = p_x;
    y = p_y;
  }
  void particle_attractor::set_force(double p_force_strength, double p_dist_effect, ps_force p_force_kind, bool p_additive)
  {
    force_strength = p_force_strength;
    dist_effect = std::max(0., p_dist_effect);
    force_kind = p_force_kind;
    additive = p_additive;
  }
}

using enigma::particle_system;
using enigma::particle_type;
using enigma::particle_attractor;

namespace enigma_user {
  int part_attractor_create(int id)
  {
    particle_system* p_s = enigma::get_particlesystem(id);
    if (p_s != NULL) {
      return p_s->create_attractor();
    }
    return -1;
  }
  void part_attractor_destroy(int ps_id, int at_id)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_attractor*>::iterator at_it = p_s->id_to_attractor.find(at_id);
      if (at_it != p_s->id_to_attractor.end()) {
        delete (*at_it).second;
        p_s->id_to_attractor.erase(at_it);
      }
    }
  }
  void part_attractor_destroy_all(int ps_id)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      for (std::map<int,particle_attractor*>::iterator it = p_s->id_to_attractor.begin(); it != p_s->id_to_attractor.end(); it++)
      {
        delete (*it).second;
      }
      p_s->id_to_attractor.clear();
    }
  }
  bool part_attractor_exists(int ps_id, int at_id)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_attractor*>::iterator at_it = p_s->id_to_attractor.find(at_id);
      if (at_it != p_s->id_to_attractor.end()) {
        return true;
      }
    }
    return false;
  }
  void part_attractor_clear(int ps_id, int at_id)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_attractor*>::iterator at_it = p_s->id_to_attractor.find(at_id);
      if (at_it != p_s->id_to_attractor.end()) {
        (*at_it).second->initialize();
      }
    }
  }
  void part_attractor_position(int ps_id, int at_id, double x, double y)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_attractor*>::iterator at_it = p_s->id_to_attractor.find(at_id);
      if (at_it != p_s->id_to_attractor.end()) {
        (*at_it).second->set_position(x, y);
      }
    }
  }
  void part_attractor_force(int ps_id, int at_id, double force, double dist, int kind, bool additive)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_attractor*>::iterator at_it = p_s->id_to_attractor.find(at_id);
      if (at_it != p_s->id_to_attractor.end()) {
        (*at_it).second->set_force(force, dist, enigma::get_ps_force(kind), additive);
      }
    }
  }
}

