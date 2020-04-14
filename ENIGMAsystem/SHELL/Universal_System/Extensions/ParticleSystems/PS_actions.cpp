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

#include "PS_actions.h"
#include "PS_particle.h"
#include "PS_effects_enums.h"
#include "PS_particle_system.h"
#include "PS_particle_system_manager.h"
#include "Universal_System/Object_Tiers/planar_object.h"
#include "Universal_System/Instances/instance_system_base.h"
#include <map>

namespace enigma_user {
  void action_effect(int effect_type, double x, double y, int size, int color, int where)
  {
    if (argument_relative) {
      enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
      x += inst->x;
      y += inst->y;
    }
    if (where == 0) {
      effect_create_below(effect_type, x, y, size, color);
    }
    else {
      effect_create_above(effect_type, x, y, size, color);
    }
  }
}

namespace enigma
{
  int action_ps_id;
  std::map<int,int> actiontypeid_to_particleid;
  std::map<int,int> actionemitterid_to_particleemitterid;
}

using enigma::action_ps_id;
using enigma::actiontypeid_to_particleid;
using enigma::actionemitterid_to_particleemitterid;
using enigma::particle_system;
using enigma::ps_manager;
using enigma::particle_type;
using enigma::pt_manager;

namespace enigma_user {
  void action_partsyst_create(double depth)
  {
    {
      std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(action_ps_id);
      if (ps_it == ps_manager.id_to_particlesystem.end()) {
        action_ps_id = part_system_create();
      }
    }
    std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(action_ps_id);
    if (ps_it != ps_manager.id_to_particlesystem.end()) {
      ps_it->second->hidden = false;
      part_system_depth(action_ps_id, depth);
      ps_it->second->hidden = true; // If the particle system exists, it will always be hidden here.
    }
  }
  void action_partsyst_destroy()
  {
    std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(action_ps_id);
    if (ps_it != ps_manager.id_to_particlesystem.end()) {
      ps_it->second->hidden = false;
      part_system_destroy(action_ps_id);
      action_ps_id = -1;
    }
  }
  void action_partsyst_clear()
  {
    std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(action_ps_id);
    if (ps_it != ps_manager.id_to_particlesystem.end()) {
      ps_it->second->hidden = false;
      part_particles_clear(action_ps_id);
      ps_it->second->hidden = true;
    }
  }
}

static particle_type* get_action_particle_type(int action_type_id)
{
  std::map<int,int>::iterator at_it = actiontypeid_to_particleid.find(action_type_id);
  if (at_it != actiontypeid_to_particleid.end()) {
    std::map<int,particle_type*>::iterator pt_it = pt_manager.id_to_particletype.find(at_it->second);
    if (pt_it != pt_manager.id_to_particletype.end()) {
      // Do nothing, already created.
      return pt_it->second;
    }
    else {
      // Clean up mapping.
      actiontypeid_to_particleid.erase(at_it);
    }
  }
  // If reaching this point, create the type.
  int particle_type_id = part_type_create();
  actiontypeid_to_particleid.insert(std::pair<int,int>(action_type_id, particle_type_id));
  return pt_manager.id_to_particletype.find(particle_type_id)->second;
}

namespace enigma_user {
  void action_parttype_create(int type, int shape, int sprite, double min_size, double max_size, double size_increment)
  {
    particle_type* p_t = get_action_particle_type(type);
    if (p_t != NULL) {
      p_t->hidden = false;
      int id = p_t->id;
      if (sprite >= 0) {
        part_type_sprite(id, sprite, true, false, false);
      }
      else {
        part_type_shape(id, shape);
      }
      part_type_size(id, min_size, max_size, size_increment, 0);
      p_t->hidden = true;
    }
  }
  // color_shape: 0=mixed, 1=changing.
  void action_parttype_color(int type, int color_shape, int color1, int color2, double alpha1, double alpha2)
  {
    particle_type* p_t = get_action_particle_type(type);
    if (p_t != NULL) {
      p_t->hidden = false;
      int id = p_t->id;
      if (color_shape == 0) {
        part_type_color_mix(id, color1, color2);
      }
      else {
        part_type_color2(id, color1, color2);
      }
      part_type_alpha2(id, alpha1, alpha2);
      p_t->hidden = true;
    }
  }
  void action_parttype_life(int type, int min_life, int max_life)
  {
    particle_type* p_t = get_action_particle_type(type);
    if (p_t != NULL) {
      p_t->hidden = false;
      int id = p_t->id;
      part_type_life(id, min_life, max_life);
      p_t->hidden = true;
    }
  }
  void action_parttype_speed(int type, double min_speed, double max_speed, double min_dir, double max_dir, double friction)
  {
    particle_type* p_t = get_action_particle_type(type);
    if (p_t != NULL) {
      p_t->hidden = false;
      int id = p_t->id;
      part_type_speed(id, min_speed, max_speed, -friction, 0);
      part_type_direction(id, min_dir, max_dir, 0, 0);
      p_t->hidden = true;
    }
  }
  void action_parttype_gravity(int type, double amount, double direction)
  {
    particle_type* p_t = get_action_particle_type(type);
    if (p_t != NULL) {
      p_t->hidden = false;
      int id = p_t->id;
      part_type_gravity(id, amount, direction);
      p_t->hidden = true;
    }
  }
  void action_parttype_secondary(int primary_type, int step_type, int step_count, int death_type, int death_count)
  {
    particle_type* p_t_primary = get_action_particle_type(primary_type);
    particle_type* p_t_step = get_action_particle_type(step_type);
    particle_type* p_t_death = get_action_particle_type(death_type);
    if (p_t_primary != NULL && p_t_step != NULL && p_t_death != NULL) {
      int primary_id = p_t_primary->id;
      int step_id = p_t_step->id;
      int death_id = p_t_death->id;
      p_t_primary->hidden = false;
      p_t_step->hidden = false;
      p_t_death->hidden = false;

      part_type_step(primary_id, step_count, step_id);
      part_type_death(primary_id, death_count, death_id);

      p_t_primary->hidden = true;
      p_t_step->hidden = true;
      p_t_death->hidden = true;
    }
  }

  static int get_action_particle_emitter_id(int ps_id, int emitter)
  {
    int emitter_id = -1;
    std::map<int,int>::iterator at_it = actionemitterid_to_particleemitterid.find(emitter);
    if (at_it != actionemitterid_to_particleemitterid.end()) {
      emitter_id = at_it->second;
      if (part_emitter_exists(ps_id, emitter_id)) {
        // Do nothing.
      }
      else {
        emitter_id = part_emitter_create(ps_id);
        actionemitterid_to_particleemitterid.erase(at_it);
        actionemitterid_to_particleemitterid.insert(std::pair<int,int>(emitter, emitter_id));
      }
    }
    else {
      emitter_id = part_emitter_create(ps_id);
      actionemitterid_to_particleemitterid.insert(std::pair<int,int>(emitter, emitter_id));
    }
    return emitter_id;
  }

  void action_partemit_create(int emitter, int shape, double x_left, double x_right, double y_top, double y_bottom)
  {
    if (ps_manager.id_to_particlesystem.find(action_ps_id) == ps_manager.id_to_particlesystem.end()) {
      // If particle system has not been created, or has been destroyed since last creation,
      // create a default particle system with depth 0.
      action_partsyst_create(0.0);
    }
    std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(action_ps_id);
    if (ps_it != ps_manager.id_to_particlesystem.end()) {
      ps_it->second->hidden = false;

      int ps_id = ps_it->second->id;
      int emitter_id = get_action_particle_emitter_id(ps_it->second->id, emitter);
      part_emitter_region(ps_id, emitter_id, x_left, x_right, y_top, y_bottom, shape, ps_distr_linear);

      ps_it->second->hidden = true;
    }
  }
  void action_partemit_destroy(int emitter)
  {
    if (ps_manager.id_to_particlesystem.find(action_ps_id) == ps_manager.id_to_particlesystem.end()) {
      // If particle system has not been created, or has been destroyed since last creation,
      // create a default particle system with depth 0.
      action_partsyst_create(0.0);
    }
    std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(action_ps_id);
    if (ps_it != ps_manager.id_to_particlesystem.end()) {
      ps_it->second->hidden = false;

      int ps_id = ps_it->second->id;
      int emitter_id = get_action_particle_emitter_id(ps_it->second->id, emitter);
      part_emitter_destroy(ps_id, emitter_id);
      actionemitterid_to_particleemitterid.erase(emitter);

      ps_it->second->hidden = true;
    }
  }
  void action_partemit_burst(int emitter, int part_type, int amount)
  {
    if (ps_manager.id_to_particlesystem.find(action_ps_id) == ps_manager.id_to_particlesystem.end()) {
      // If particle system has not been created, or has been destroyed since last creation,
      // create a default particle system with depth 0.
      action_partsyst_create(0.0);
    }
    std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(action_ps_id);
    if (ps_it != ps_manager.id_to_particlesystem.end()) {
      ps_it->second->hidden = false;

      int ps_id = ps_it->second->id;
      int emitter_id = get_action_particle_emitter_id(ps_it->second->id, emitter);
      particle_type* p_t = get_action_particle_type(part_type);
      if (p_t != NULL) {
        p_t->hidden = false;
        part_emitter_burst(ps_id, emitter_id, p_t->id, amount);
        p_t->hidden = true;
      }

      ps_it->second->hidden = true;
    }
  }
  void action_partemit_stream(int emitter, int part_type, int amount)
  {
    if (ps_manager.id_to_particlesystem.find(action_ps_id) == ps_manager.id_to_particlesystem.end()) {
      // If particle system has not been created, or has been destroyed since last creation,
      // create a default particle system with depth 0.
      action_partsyst_create(0.0);
    }
    std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(action_ps_id);
    if (ps_it != ps_manager.id_to_particlesystem.end()) {
      ps_it->second->hidden = false;

      int ps_id = ps_it->second->id;
      int emitter_id = get_action_particle_emitter_id(ps_it->second->id, emitter);
      particle_type* p_t = get_action_particle_type(part_type);
      if (p_t != NULL) {
        p_t->hidden = false;
        part_emitter_stream(ps_id, emitter_id, p_t->id, amount);
        p_t->hidden = true;
      }

      ps_it->second->hidden = true;
    }
  }
}
