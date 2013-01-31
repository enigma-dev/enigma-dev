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
#include "Universal_System/depth_draw.h"
#include <cmath>

using enigma::particle_system;
using enigma::particle_type;
using enigma::ps_manager;
using enigma::pt_manager;
using enigma::particle_type_manager;

// General functions.

int part_system_create()
{
  particle_system* p_s = new particle_system();
  p_s->initialize();

  ps_manager.max_id++;
  ps_manager.id_to_particlesystem.insert(std::pair<int,particle_system*>(ps_manager.max_id, p_s));
  p_s->id = ps_manager.max_id;

  // Drawing is automatic, so register in depth.
  enigma::drawing_depths[p_s->depth].particlesystem_ids.insert(ps_manager.max_id);

  return ps_manager.max_id;
}
void part_system_destroy(int id)
{
  part_system_clear(id);
  // Remember to destroy the system.
  std::map<int,particle_system*>::iterator it = ps_manager.id_to_particlesystem.find(id);
  if (it != ps_manager.id_to_particlesystem.end()) {
    particle_system* p_s = (*it).second;
    delete p_s;
    ps_manager.id_to_particlesystem.erase(it);
  }
}
bool part_system_exists(int id)
{
  std::map<int,particle_system*>::iterator it = ps_manager.id_to_particlesystem.find(id);
  if (it != ps_manager.id_to_particlesystem.end()) {
    return true;
  }
  return false;
}
void part_system_clear(int id)
{
  part_emitter_destroy_all(id);
  part_attractor_destroy_all(id);
  part_destroyer_destroy_all(id);
  part_deflector_destroy_all(id);
  part_changer_destroy_all(id);
  part_particles_clear(id);
}
void part_system_draw_order(int id, bool oldtonew)
{
  std::map<int,particle_system*>::iterator it = ps_manager.id_to_particlesystem.find(id);
  if (it != ps_manager.id_to_particlesystem.end()) {
    (*it).second->oldtonew = oldtonew;
  }
}
void part_system_depth(int id, double depth)
{
  std::map<int,particle_system*>::iterator it = ps_manager.id_to_particlesystem.find(id);
  if (it != ps_manager.id_to_particlesystem.end()) {
    particle_system* p_s = (*it).second;
    const double new_depth = round(depth);
    if (p_s->auto_draw) {
      // If the particle system has automatic drawing enabled, it is in the depth system,
      // and it should be moved.
      const double current_depth = p_s->depth;
      enigma::drawing_depths[current_depth].particlesystem_ids.erase(p_s->id);
      enigma::drawing_depths[new_depth].particlesystem_ids.insert(p_s->id);
    }
    else {
      // If the particle system does not have automatic drawing enabled, it is not in the depth system,
      // and the value should just be updated.
      p_s->depth = new_depth;
    }
  }
}
void part_system_position(int id, double x, double y)
{
  std::map<int,particle_system*>::iterator it = ps_manager.id_to_particlesystem.find(id);
  if (it != ps_manager.id_to_particlesystem.end()) {
    (*it).second->x_offset = x;
    (*it).second->y_offset = y;
  }
}

// Update and draw.

void part_system_automatic_update(int id, bool automatic)
{
  std::map<int,particle_system*>::iterator it = ps_manager.id_to_particlesystem.find(id);
  if (it != ps_manager.id_to_particlesystem.end()) {
    (*it).second->auto_update = automatic;
  }
}
void part_system_automatic_draw(int id, bool automatic)
{
  std::map<int,particle_system*>::iterator it = ps_manager.id_to_particlesystem.find(id);
  if (it != ps_manager.id_to_particlesystem.end()) {
    particle_system* p_s = (*it).second;
    bool auto_draw_before = p_s->auto_draw;
    p_s->auto_draw = automatic;
    if (automatic && !auto_draw_before) { // Add to drawing depths.
      enigma::drawing_depths[p_s->depth].particlesystem_ids.insert(p_s->id);
    }
    else if (!automatic && auto_draw_before) { // Remove from drawing depths.
      enigma::drawing_depths[p_s->depth].particlesystem_ids.erase(p_s->id);
    }
  }
}
void part_system_update(int id)
{
  std::map<int,particle_system*>::iterator it = ps_manager.id_to_particlesystem.find(id);
  if (it != ps_manager.id_to_particlesystem.end()) {
    (*it).second->update_particlesystem();
  }
}
void part_system_drawit(int id)
{
  std::map<int,particle_system*>::iterator it = ps_manager.id_to_particlesystem.find(id);
  if (it != ps_manager.id_to_particlesystem.end()) {
    (*it).second->draw_particlesystem();
  }
}

