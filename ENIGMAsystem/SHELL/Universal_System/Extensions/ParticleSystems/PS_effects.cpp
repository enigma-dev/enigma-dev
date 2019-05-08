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

#include <map>
#include <cmath>
#include <cstddef>
#include <cstdlib>

#include "PS_effects_enums.h"
#include "PS_particle.h"
#include "PS_particle_system.h"
#include "PS_particle_system_manager.h"
#include "PS_particle_type.h"
#include "PS_particle_updatedraw.h"
#include "Universal_System/roomsystem.h"
#include "Widget_Systems/widgets_mandatory.h" // show_error
#include "Universal_System/math_consts.h"

namespace enigma
{
  // NOTE: The effects are initialized lazily.
  int ps_below_id = -1;
  int ps_above_id = -1;
  const double below_depth = 100000;
  const double above_depth = -100000;
  enum ef_pt {
    ef_pt_explosion_s1 = 0, ef_pt_explosion_s2, ef_pt_explosion_s3,
    ef_pt_ring_s1, ef_pt_ring_s2, ef_pt_ring_s3,
    ef_pt_ellipse_s1, ef_pt_ellipse_s2, ef_pt_ellipse_s3,
    ef_pt_firework_s1, ef_pt_firework_s2, ef_pt_firework_s3,
    ef_pt_smoke_s1, ef_pt_smoke_s2, ef_pt_smoke_s3,
    ef_pt_smokeup_s1, ef_pt_smokeup_s2, ef_pt_smokeup_s3,
    ef_pt_star_s1, ef_pt_star_s2, ef_pt_star_s3,
    ef_pt_spark_s1, ef_pt_spark_s2, ef_pt_spark_s3,
    ef_pt_flare_s1, ef_pt_flare_s2, ef_pt_flare_s3,
    ef_pt_cloud_s1, ef_pt_cloud_s2, ef_pt_cloud_s3,
    ef_pt_rain,
    ef_pt_snow
  };
  std::map<ef_pt,int> effectparticletype_to_particletypeid;

  static particle_system* get_effect_particlesystem(int& ps_id, double depth)
  {
    {
      std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(ps_id);
      if (ps_it == ps_manager.id_to_particlesystem.end()) {
        ps_id = part_system_create();
        part_system_depth(ps_id, depth);
      }
    }
    std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(ps_id);
    if (ps_it != ps_manager.id_to_particlesystem.end()) {
      return (*ps_it).second;
    }
    return NULL;
  }
  using enigma::ef_pt;
  static particle_type* get_particletype(ef_pt effectparticletype, ef_size effect_size)
  {
    {
      std::map<ef_pt,int>::iterator id_it = effectparticletype_to_particletypeid.find(effectparticletype);
      if (id_it != effectparticletype_to_particletypeid.end()) {
        int pt_id = (*id_it).second;
        std::map<int,particle_type*>::iterator pt_it = pt_manager.id_to_particletype.find(pt_id);
        if (pt_it != pt_manager.id_to_particletype.end()) { // Return the existing particle type.
          return (*pt_it).second;
        }
        else { // If a particle type has been created, but it cannot be found, return NULL.
          return NULL;
        }
      }
    }
    // The particle type has not been created yet, so create it.
    const int pt_id = part_type_create();
    particle_type* p_t = enigma::get_particletype(pt_id);
    if (p_t == NULL) {
      return NULL;
    }
    effectparticletype_to_particletypeid.insert(std::pair<ef_pt,int>(effectparticletype,pt_id));
    switch (effectparticletype) {
    case ef_pt_explosion_s1:
    case ef_pt_explosion_s2:
    case ef_pt_explosion_s3: {
      part_type_shape(pt_id, pt_shape_explosion);
      switch (effect_size) {
      case ef_si_small: {
        part_type_size(pt_id, 0.0, 0.0, 0.1, 0.0);
        part_type_speed(pt_id, 0, 1.3, 0, 0);
        part_type_life(pt_id, 12, 12);
        break;
      }
      case ef_si_medium: {
        part_type_size(pt_id, 0.0, 0.0, 0.15, 0.0);
        part_type_speed(pt_id, 0, 3, 0, 0);
        part_type_life(pt_id, 14, 14);
        break;
      }
      case ef_si_large: {
        part_type_size(pt_id, 0, 0, 0.25, 0.0);
        part_type_speed(pt_id, 0, 7, 0, 0);
        part_type_life(pt_id, 16, 16);
        break;
      }
      default: break;
      }
      part_type_alpha3(pt_id, 0.3, 0.3, 0.0);
      part_type_direction(pt_id, 0, 360, 0, 0);
      part_type_orientation(pt_id, 0, 360, 0, 0, 0);
      return p_t;
    }
    case ef_pt_ring_s1:
    case ef_pt_ring_s2:
    case ef_pt_ring_s3: {
      part_type_shape(pt_id, pt_shape_ring);
      switch (effect_size) {
      case ef_si_small: {
        part_type_size(pt_id, 0.0, 0.0, 0.15, 0.0);
        part_type_life(pt_id, 10, 10);
        break;
      }
      case ef_si_medium: {
        part_type_size(pt_id, 0.0, 0.0, 0.25, 0.0);
        part_type_life(pt_id, 14, 14);
        break;
      }
      case ef_si_large: {
        part_type_size(pt_id, 0.0, 0.0, 0.5, 0.0);
        part_type_life(pt_id, 18, 18);
        break;
      }
      default: break;
      }
      part_type_alpha2(pt_id, 1.0, 0.0);
      return p_t;
    }
    case ef_pt_ellipse_s1:
    case ef_pt_ellipse_s2:
    case ef_pt_ellipse_s3: {
      part_type_shape(pt_id, pt_shape_ring);
      switch (effect_size) {
      case ef_si_small: {
        part_type_size(pt_id, 0.0, 0.0, 0.15, 0.0);
        part_type_life(pt_id, 10, 10);
        break;
      }
      case ef_si_medium: {
        part_type_size(pt_id, 0.0, 0.0, 0.25, 0.0);
        part_type_life(pt_id, 14, 14);
        break;
      }
      case ef_si_large: {
        part_type_size(pt_id, 0.0, 0.0, 0.5, 0.0);
        part_type_life(pt_id, 18, 18);
        break;
      }
      default: break;
      }
      part_type_alpha2(pt_id, 1.0, 0.0);
      part_type_scale(pt_id, 1.2, 0.8);
      return p_t;
    }
    case ef_pt_firework_s1:
    case ef_pt_firework_s2:
    case ef_pt_firework_s3: {
      part_type_shape(pt_id, pt_shape_flare);
      switch (effect_size) {
      case ef_si_small: {
        part_type_life(pt_id, 18, 22);
        part_type_speed(pt_id, 0, 3, 0, 0);
        break;
      }
      case ef_si_medium: {
        part_type_life(pt_id, 22, 26);
        part_type_speed(pt_id, 0, 5, 0, 0);
        break;
      }
      case ef_si_large: {
        part_type_life(pt_id, 36, 40);
        part_type_speed(pt_id, 0, 7, 0, 0);
        break;
      }
      default: break;
      }
      part_type_size(pt_id, 0.16, 0.20, 0, 0.0);
      part_type_gravity(pt_id, 0.2, 270.0);
      part_type_direction(pt_id, 0, 360, 0, 0);
      part_type_alpha1(pt_id, 0.7);
      return p_t;
    }
    case ef_pt_smoke_s1:
    case ef_pt_smoke_s2:
    case ef_pt_smoke_s3: {
      part_type_shape(pt_id, pt_shape_explosion);
      switch (effect_size) {
      case ef_si_small: {
        part_type_life(pt_id, 18, 22);
        part_type_size(pt_id, 0.2, 0.4, -0.01, 0.0);
        break;
      }
      case ef_si_medium: {
        part_type_life(pt_id, 26, 30);
        part_type_size(pt_id, 0.4, 0.6, -0.01, 0.0);
        break;
      }
      case ef_si_large: {
        part_type_life(pt_id, 46, 54);
        part_type_size(pt_id, 0.5, 0.8, -0.01, 0.0);
        break;
      }
      default: break;
      }
      part_type_orientation(pt_id, 0.0, 360.0, 0, 0, 0);
      part_type_alpha2(pt_id, 0.3, 0.0);
      return p_t;
    }
    case ef_pt_smokeup_s1:
    case ef_pt_smokeup_s2:
    case ef_pt_smokeup_s3: {
      part_type_shape(pt_id, pt_shape_explosion);
      switch (effect_size) {
      case ef_si_small: {
        part_type_life(pt_id, 18, 22);
        part_type_size(pt_id, 0.2, 0.4, -0.01, 0.0);
        part_type_speed(pt_id, 3, 4, 0, 0);
        break;
      }
      case ef_si_medium: {
        part_type_life(pt_id, 26, 30);
        part_type_size(pt_id, 0.4, 0.6, -0.01, 0.0);
        part_type_speed(pt_id, 4, 5, 0, 0);
        break;
      }
      case ef_si_large: {
        part_type_life(pt_id, 46, 54);
        part_type_size(pt_id, 0.5, 0.8, -0.01, 0.0);
        part_type_speed(pt_id, 5, 7, 0, 0);
        break;
      }
      default: break;
      }
      part_type_direction(pt_id, 90.0, 90.0, 0, 0);
      part_type_orientation(pt_id, 0.0, 360.0, 0, 0, 0);
      part_type_alpha2(pt_id, 0.3, 0.0);
      return p_t;
    }
    case ef_pt_star_s1:
    case ef_pt_star_s2:
    case ef_pt_star_s3: {
      part_type_shape(pt_id, pt_shape_star);
      switch (effect_size) {
      case ef_si_small: {
        part_type_life(pt_id, 18, 18);
        part_type_size(pt_id, 0.4, 0.4, -0.02, 0);
        break;
      }
      case ef_si_medium: {
        part_type_life(pt_id, 26, 26);
        part_type_size(pt_id, 0.7, 0.7, -0.03, 0);
        break;
      }
      case ef_si_large: {
        part_type_life(pt_id, 34, 34);
        part_type_size(pt_id, 1.0, 1.0, -0.03, 0);
        break;
      }
      default: break;
      }
      part_type_orientation(pt_id, 0.0, 360.0, 0, 0, 0);
      return p_t;
    }
    case ef_pt_spark_s1:
    case ef_pt_spark_s2:
    case ef_pt_spark_s3: {
      part_type_shape(pt_id, pt_shape_spark);
      switch (effect_size) {
      case ef_si_small: {
        part_type_life(pt_id, 18, 18);
        part_type_size(pt_id, 0.4, 0.4, -0.02, 0);
        break;
      }
      case ef_si_medium: {
        part_type_life(pt_id, 26, 26);
        part_type_size(pt_id, 0.7, 0.7, -0.03, 0);
        break;
      }
      case ef_si_large: {
        part_type_life(pt_id, 34, 34);
        part_type_size(pt_id, 1.0, 1.0, -0.03, 0);
        break;
      }
      default: break;
      }
      part_type_orientation(pt_id, 0.0, 360.0, 0, 0, 0);
      return p_t;
    }
    case ef_pt_flare_s1:
    case ef_pt_flare_s2:
    case ef_pt_flare_s3: {
      part_type_shape(pt_id, pt_shape_flare);
      switch (effect_size) {
      case ef_si_small: {
        part_type_life(pt_id, 18, 18);
        part_type_size(pt_id, 0.4, 0.4, -0.02, 0);
        break;
      }
      case ef_si_medium: {
        part_type_life(pt_id, 26, 26);
        part_type_size(pt_id, 0.7, 0.7, -0.03, 0);
        break;
      }
      case ef_si_large: {
        part_type_life(pt_id, 34, 34);
        part_type_size(pt_id, 1.0, 1.0, -0.03, 0);
        break;
      }
      default: break;
      }
      part_type_orientation(pt_id, 0.0, 360.0, 0, 0, 0);
      return p_t;
    }
    case ef_pt_cloud_s1:
    case ef_pt_cloud_s2:
    case ef_pt_cloud_s3: {
      part_type_shape(pt_id, pt_shape_explosion);
      double size = 0.0;
      switch (effect_size) {
      case ef_si_small: {
        size = 1.0;
        break;
      }
      case ef_si_medium: {
        size = 2.0;
        break;
      }
      case ef_si_large: {
        size = 4.0;
        break;
      }
      default: break;
      }
      part_type_life(pt_id, 100, 100);
      part_type_size(pt_id, size, size, 0, 0);
      part_type_scale(pt_id, 2.0, 1.0);
      part_type_alpha3(pt_id, 0.0, 0.4, 0.0);
      return p_t;
    }
    case ef_pt_rain: {
      // NOTE: Be careful when changing this and be careful of the dependencies between this
      // and the rain creation part.
      part_type_shape(pt_id, pt_shape_line);
      part_type_size(pt_id, 0.14, 0.18, 0, 0);
      part_type_direction(pt_id, 260,260,0,0);
      part_type_orientation(pt_id, 0, 0, 0, 0, true);
      const double sp = 7.0;
      part_type_speed(pt_id, sp, sp, 0, 0);
      part_type_scale(pt_id, 2.0, 1.0);
      part_type_alpha1(pt_id,0.5);
      // Calculate life based on room dimensions.
      const double v = 10*M_PI/180.0;
      const double h = std::max(1.0, 1.0*room_height);
      const double life = h/(sp*cos(v)) + 100;
      part_type_life(pt_id, life, life);
      return p_t;
    }
    case ef_pt_snow: {
      // NOTE: Be careful when changing this and be careful of the dependencies between this
      // and the snow creation part.
      part_type_shape(pt_id, pt_shape_snow);
      part_type_size(pt_id, 0.1, 0.25, 0, 0);
      part_type_direction(pt_id, 250,290,0,30);
      part_type_orientation(pt_id, 0, 360, 0, 0, false);
      const double sp = 3.0;
      part_type_speed(pt_id, sp, sp, 0, 0);
      part_type_alpha1(pt_id, 0.5);
      // Calculate life based on room dimensions.
      const double v = 20*M_PI/180.0;
      const double h = std::max(1.0, 1.0*room_height);
      const double life = 2*h/(sp*cos(v)) + 100;
      part_type_life(pt_id, life, life);
      return p_t;
    }
    default: return NULL;
    }
  }
  static void create_effect(int ps_id, ef_effect effect_kind, ef_size effect_size, double x, double y, int color)
  {
    particle_type* p_t = NULL;
    switch (effect_kind) {
    case ef_effects_explosion: {
      switch (effect_size) {
      case ef_si_small: {
        p_t = get_particletype(ef_pt_explosion_s1, effect_size);
        break;
      }
      case ef_si_medium: {
        p_t = get_particletype(ef_pt_explosion_s2, effect_size);
        break;
      }
      case ef_si_large: {
        p_t = get_particletype(ef_pt_explosion_s3, effect_size);
        break;
      }
      default: {
        break;
      }
      }
      if (p_t != NULL) {
        p_t->hidden = false;
        part_particles_create_color(ps_id, x, y, p_t->id, color, 6);
        p_t->hidden = true;
      }
      break;
    }
    case ef_effects_ring: {
      switch (effect_size) {
      case ef_si_small: {
        p_t = get_particletype(ef_pt_ring_s1, effect_size);
        break;
      }
      case ef_si_medium: {
        p_t = get_particletype(ef_pt_ring_s2, effect_size);
        break;
      }
      case ef_si_large: {
        p_t = get_particletype(ef_pt_ring_s3, effect_size);
        break;
      }
      default: {
        break;
      }
      }
      if (p_t != NULL) {
        p_t->hidden = false;
        part_particles_create_color(ps_id, x, y, p_t->id, color, 1);
        p_t->hidden = true;
      }
      break;
    }
    case ef_effects_ellipse: {
      switch (effect_size) {
      case ef_si_small: {
        p_t = get_particletype(ef_pt_ellipse_s1, effect_size);
        break;
      }
      case ef_si_medium: {
        p_t = get_particletype(ef_pt_ellipse_s2, effect_size);
        break;
      }
      case ef_si_large: {
        p_t = get_particletype(ef_pt_ellipse_s3, effect_size);
        break;
      }
      default: {
        break;
      }
      }
      if (p_t != NULL) {
        p_t->hidden = false;
        part_particles_create_color(ps_id, x, y, p_t->id, color, 1);
        p_t->hidden = true;
      }
      break;
    }
    case ef_effects_firework: {
      int num = 0;
      switch (effect_size) {
      case ef_si_small: {
        p_t = get_particletype(ef_pt_firework_s1, effect_size);
        num = 100;
        break;
      }
      case ef_si_medium: {
        p_t = get_particletype(ef_pt_firework_s2, effect_size);
        num = 150;
        break;
      }
      case ef_si_large: {
        p_t = get_particletype(ef_pt_firework_s3, effect_size);
        num = 200;
        break;
      }
      default: {
        break;
      }
      }
      if (p_t != NULL) {
        p_t->hidden = false;
        part_particles_create_color(ps_id, x, y, p_t->id, color, num);
        p_t->hidden = true;
      }
      break;
    }
    case ef_effects_smoke: {
      int num = 0;
      double dist = 0.0;
      switch (effect_size) {
      case ef_si_small: {
        p_t = get_particletype(ef_pt_smoke_s1, effect_size);
        num = 10;
        dist = 12.0;
        break;
      }
      case ef_si_medium: {
        p_t = get_particletype(ef_pt_smoke_s2, effect_size);
        num = 15;
        dist = 32.0;
        break;
      }
      case ef_si_large: {
        p_t = get_particletype(ef_pt_smoke_s3, effect_size);
        num = 25;
        dist = 64.0;
        break;
      }
      default: {
        break;
      }
      }
      if (p_t != NULL) {
        p_t->hidden = false;
        for (int i = 0; i < num; i++)
        {
          part_particles_create_color(ps_id,
            x + dist*rand()/RAND_MAX - dist/2.0,
            y + dist*rand()/RAND_MAX - dist/2.0,
            p_t->id, color, 1
          );
        }
        p_t->hidden = true;
      }
      break;
    }
    case ef_effects_smokeup: {
      int num = 0;
      double dist = 0.0;
      switch (effect_size) {
      case ef_si_small: {
        p_t = get_particletype(ef_pt_smokeup_s1, effect_size);
        num = 10;
        dist = 12.0;
        break;
      }
      case ef_si_medium: {
        p_t = get_particletype(ef_pt_smokeup_s2, effect_size);
        num = 15;
        dist = 32.0;
        break;
      }
      case ef_si_large: {
        p_t = get_particletype(ef_pt_smokeup_s3, effect_size);
        num = 25;
        dist = 64.0;
        break;
      }
      default: {
        break;
      }
      }
      if (p_t != NULL) {
        p_t->hidden = false;
        for (int i = 0; i < num; i++)
        {
          part_particles_create_color(ps_id,
            x + dist*rand()/RAND_MAX - dist/2.0,
            y + dist*rand()/RAND_MAX - dist/2.0,
            p_t->id, color, 1
          );
        }
        p_t->hidden = true;
      }
      break;
    }
    case ef_effects_star: {
      double dist = 0.0;
      switch (effect_size) {
      case ef_si_small: {
        p_t = get_particletype(ef_pt_star_s1, effect_size);
        dist = 1.0;
        break;
      }
      case ef_si_medium: {
        p_t = get_particletype(ef_pt_star_s2, effect_size);
        dist = 2.0;
        break;
      }
      case ef_si_large: {
        p_t = get_particletype(ef_pt_star_s3, effect_size);
        dist = 4.0;
        break;
      }
      default: {
        break;
      }
      }
      if (p_t != NULL) {
        p_t->hidden = false;
        part_particles_create_color(ps_id,
          x + dist*rand()/RAND_MAX - dist/2.0,
          y + dist*rand()/RAND_MAX - dist/2.0,
          p_t->id, color, 1
        );
        p_t->hidden = true;
      }
      break;
    }
    case ef_effects_spark: {
      double dist = 0.0;
      switch (effect_size) {
      case ef_si_small: {
        p_t = get_particletype(ef_pt_spark_s1, effect_size);
        dist = 1.0;
        break;
      }
      case ef_si_medium: {
        p_t = get_particletype(ef_pt_spark_s2, effect_size);
        dist = 2.0;
        break;
      }
      case ef_si_large: {
        p_t = get_particletype(ef_pt_spark_s3, effect_size);
        dist = 4.0;
        break;
      }
      default: {
        break;
      }
      }
      if (p_t != NULL) {
        p_t->hidden = false;
        part_particles_create_color(ps_id,
          x + dist*rand()/RAND_MAX - dist/2.0,
          y + dist*rand()/RAND_MAX - dist/2.0,
          p_t->id, color, 1
        );
        p_t->hidden = true;
      }
      break;
    }
    case ef_effects_flare: {
      double dist = 0.0;
      switch (effect_size) {
      case ef_si_small: {
        p_t = get_particletype(ef_pt_flare_s1, effect_size);
        dist = 1.0;
        break;
      }
      case ef_si_medium: {
        p_t = get_particletype(ef_pt_flare_s2, effect_size);
        dist = 2.0;
        break;
      }
      case ef_si_large: {
        p_t = get_particletype(ef_pt_flare_s3, effect_size);
        dist = 4.0;
        break;
      }
      default: {
        break;
      }
      }
      if (p_t != NULL) {
        p_t->hidden = false;
        part_particles_create_color(ps_id,
          x + dist*rand()/RAND_MAX - dist/2.0,
          y + dist*rand()/RAND_MAX - dist/2.0,
          p_t->id, color, 1
        );
        p_t->hidden = true;
      }
      break;
    }
    case ef_effects_cloud: {
      switch (effect_size) {
      case ef_si_small: {
        p_t = get_particletype(ef_pt_cloud_s1, effect_size);
        break;
      }
      case ef_si_medium: {
        p_t = get_particletype(ef_pt_cloud_s2, effect_size);
        break;
      }
      case ef_si_large: {
        p_t = get_particletype(ef_pt_cloud_s3, effect_size);
        break;
      }
      default: {
        break;
      }
      }
      if (p_t != NULL) {
        p_t->hidden = false;
        part_particles_create_color(ps_id, x, y, p_t->id, color, 1);
        p_t->hidden = true;
      }
      break;
    }
    case ef_effects_rain: {
      int num = 0;
      switch (effect_size) {
      case ef_si_small: {
        num = 2;
        break;
      }
      case ef_si_medium: {
        num = 5;
        break;
      }
      case ef_si_large: {
        num = 9;
        break;
      }
      default: {
        break;
      }
      }
      p_t = get_particletype(ef_pt_rain, effect_size);
      // NOTE: Be careful when changing this and be careful of the dependencies between this
      // and the rain type part.
      if (p_t != NULL) {
        p_t->hidden = false;
        const double v = 10*M_PI/180.0;
        const double w = std::max(1.0, 1.0*room_width);
        const double h = std::max(1.0, 1.0*room_height);
        const double s = h*tan(v);
        const double x_dist = w + s;
        for (int i = 0; i < num; i++)
        {
          const double l = 20.0 + 10.0*rand()/(RAND_MAX-1);
          const double x_offset = l*sin(v);
          const double y_offset = -l*cos(v);
          part_particles_create_color(ps_id, x_offset + x_dist*rand()/(RAND_MAX-1), y_offset, p_t->id, color, 1);
        }
        p_t->hidden = true;
      }
      break;
    }
    case ef_effects_snow: {
      int num = 0;
      switch (effect_size) {
      case ef_si_small: {
        num = 1;
        break;
      }
      case ef_si_medium: {
        num = 3;
        break;
      }
      case ef_si_large: {
        num = 7;
        break;
      }
      default: {
        break;
      }
      }
      p_t = get_particletype(ef_pt_snow, effect_size);
      // NOTE: Be careful when changing this and be careful of the dependencies between this
      // and the snow type part.
      if (p_t != NULL) {
        p_t->hidden = false;
        const double v = 20*M_PI/180.0;
        const double w = std::max(1.0, 1.0*room_width);
        const double h = std::max(1.0, 1.0*room_height);
        const double s = h*tan(v);
        const double x_dist = w + 2*s;
        for (int i = 0; i < num; i++)
        {
          const double l = 20.0 + 10.0*rand()/(RAND_MAX-1);
          const double x_offset = -s - l*sin(v);
          const double y_offset = -l*cos(v);
          part_particles_create_color(ps_id, x_offset + x_dist*rand()/(RAND_MAX-1), y_offset, p_t->id, color, 1);
        }
        p_t->hidden = true;
      }
      break;
    }
      default:
        #if DEBUG_MODE
          enigma_user::show_error("Internal error: invalid particle effect type.", false)
        #endif
        ;
    }
  }

  static void create_effect_in_particlesystem(int& ps_id, double depth, int kind, double x, double y, int size, int color)
  {
    const enigma::ef_effect effect_kind = enigma::get_ef(kind);
    const enigma::ef_size effect_size = enigma::get_ef_size(size);
    enigma::particle_system* p_s = enigma::get_effect_particlesystem(ps_id, depth);
    if (p_s != NULL) {
      p_s->hidden = false;
      enigma::create_effect(ps_id, effect_kind, effect_size, x, y, color);
      p_s->hidden = true;
    }
  }
}

namespace enigma_user {
  void effect_create_below(int kind, double x, double y, int size, int color)
  {
    enigma::initialize_particle_systems_drawing();
    enigma::create_effect_in_particlesystem(enigma::ps_below_id, enigma::below_depth, kind, x, y, size, color);
  }
  void effect_create_above(int kind, double x, double y, int size, int color)
  {
    enigma::initialize_particle_systems_drawing();
    enigma::create_effect_in_particlesystem(enigma::ps_above_id, enigma::above_depth, kind, x, y, size, color);
  }
  void effect_clear()
  {
    using enigma::ps_manager;
    using enigma::particle_system;
    using enigma::pt_manager;
    using enigma::particle_type;
    using enigma::ef_pt;
    {
      std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(enigma::ps_below_id);
      if (ps_it != ps_manager.id_to_particlesystem.end()) {
        ps_it->second->hidden = false;
      }
    }
    {
      std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(enigma::ps_above_id);
      if (ps_it != ps_manager.id_to_particlesystem.end()) {
        ps_it->second->hidden = false;
      }
    }
    part_system_destroy(enigma::ps_below_id);
    part_system_destroy(enigma::ps_above_id);
    enigma::ps_below_id = -1;
    enigma::ps_above_id = -1;
    {
      const std::map<ef_pt,int>::iterator id_end = enigma::effectparticletype_to_particletypeid.end();
      for (std::map<ef_pt,int>::iterator id_it = enigma::effectparticletype_to_particletypeid.begin(); id_it != id_end; id_it++)
      {
        std::map<int,particle_type*>::iterator pt_it = pt_manager.id_to_particletype.find(id_it->second);
        if (pt_it != pt_manager.id_to_particletype.end()) {
          pt_it->second->hidden = false;
        }
        part_type_destroy(id_it->second);
      }
      enigma::effectparticletype_to_particletypeid.clear();
    }
  }
}
