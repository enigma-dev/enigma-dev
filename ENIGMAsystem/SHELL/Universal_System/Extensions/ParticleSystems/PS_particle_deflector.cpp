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

#include "PS_particle_deflector.h"
#include "PS_particle_system.h"
#include "PS_particle_system_manager.h"
#include "PS_particle.h"
#include <cstdlib>
#include <algorithm>
#include <cmath>

namespace enigma
{
  void particle_deflector::initialize()
  {
    xmin = 0.0, xmax = 0.0;
    ymin = 0.0, ymax = 0.0;
    deflection_kind = ps_de_horizontal;
    friction = 0.0;
  }
  particle_deflector* create_particle_deflector()
  {
    particle_deflector* pdf = new particle_deflector();
    pdf->initialize();
    return pdf;
  }
  void particle_deflector::clear_particle_deflector()
  {
    initialize();
  }
  bool particle_deflector::is_inside(double x, double y)
  {
    // Regions with coordinates along the same axis which are the same are empty.
    // This has been tested and verified to be consistent with GM.
    return xmin < xmax && ymin < ymax && x >= xmin && x <= xmax && y >= ymin && y <= ymax;
  }
  void particle_deflector::set_region(double p_xmin, double p_xmax, double p_ymin, double p_ymax)
  {
    // NOTE: The assignment to the region's coordinates means that if p_xmin >= p_xmax, the region is empty.
    // This has been tested and verified to be consistent with GM.
    xmin = std::min(p_xmin, p_xmax);
    xmax = p_xmax;
    ymin = std::min(p_ymin, p_ymax);
    ymax = p_ymax;
  }
  void particle_deflector::set_kind(ps_deflect p_deflection_kind)
  {
    deflection_kind = p_deflection_kind;
  }
  void particle_deflector::set_friction(double p_friction)
  {
    friction = p_friction;
  }
}

using enigma::particle_system;
using enigma::particle_type;
using enigma::particle_deflector;

namespace enigma_user {
  int part_deflector_create(int id)
  {
    particle_system* p_s = enigma::get_particlesystem(id);
    if (p_s != NULL) {
      return p_s->create_deflector();
    }
    return -1;
  }
  void part_deflector_destroy(int ps_id, int df_id)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_deflector*>::iterator df_it = p_s->id_to_deflector.find(df_id);
      if (df_it != p_s->id_to_deflector.end()) {
        delete (*df_it).second;
        p_s->id_to_deflector.erase(df_it);
      }
    }
  }
  void part_deflector_destroy_all(int ps_id)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      for (std::map<int,particle_deflector*>::iterator it = p_s->id_to_deflector.begin(); it != p_s->id_to_deflector.end(); it++)
      {
        delete (*it).second;
      }
      p_s->id_to_deflector.clear();
    }
  }
  bool part_deflector_exists(int ps_id, int df_id)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_deflector*>::iterator df_it = p_s->id_to_deflector.find(df_id);
      if (df_it != p_s->id_to_deflector.end()) {
        return true;
      }
    }
    return false;
  }
  void part_deflector_clear(int ps_id, int df_id)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_deflector*>::iterator df_it = p_s->id_to_deflector.find(df_id);
      if (df_it != p_s->id_to_deflector.end()) {
        (*df_it).second->initialize();
      }
    }
  }
  void part_deflector_region(int ps_id, int df_id, double xmin, double xmax, double ymin, double ymax)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_deflector*>::iterator df_it = p_s->id_to_deflector.find(df_id);
      if (df_it != p_s->id_to_deflector.end()) {
        (*df_it).second->set_region(xmin, xmax, ymin, ymax);
      }
    }
  }
  void part_deflector_kind(int ps_id, int df_id, int kind)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_deflector*>::iterator df_it = p_s->id_to_deflector.find(df_id);
      if (df_it != p_s->id_to_deflector.end()) {
        (*df_it).second->set_kind(enigma::get_ps_deflect(kind));
      }
    }
  }
  void part_deflector_friction(int ps_id, int df_id, double friction)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_deflector*>::iterator df_it = p_s->id_to_deflector.find(df_id);
      if (df_it != p_s->id_to_deflector.end()) {
        (*df_it).second->set_friction(friction);
      }
    }
  }
}

