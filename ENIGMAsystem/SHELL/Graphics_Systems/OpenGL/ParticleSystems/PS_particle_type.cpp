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

#include "PS_particle_type.h"
#include "PS_particle_sprites.h"
#include "Graphics_Systems/OpenGL/GScolors.h"

namespace enigma
{
  particle_type_manager pt_manager;
}

int part_type_create()
{ 
  enigma::particle_type* pt = new enigma::particle_type();
  pt->particle_count = 0;
  pt->alive = true;
  pt->is_particle_sprite = true;
  enigma::particle_sprite* sprite = enigma::get_particle_sprite(pt_shape_pixel);
  if (sprite != 0) {
    pt->part_sprite = sprite;
  }
  pt->size_min = 1.0, pt->size_max = 1.0;
  pt->size_incr = 0.0, pt->size_wiggle = 0.0;
  pt->xscale = 1.0, pt->yscale = 1.0;
  pt->c_mode = enigma::one_color;
  pt->color1 = c_white;
  pt->color2 = c_white;
  pt->a_mode = enigma::one_alpha;
  pt->alpha1 = 255;
  pt->alpha2 = 255;
  pt->life_min = 100;
  pt->life_max = 100;
  pt->death_on = false, pt->death_particle_id = 0, pt->death_number = 0;
  pt->speed_min = 0.0, pt->speed_max = 0.0;
  pt->speed_incr = 0.0, pt->speed_wiggle = 0.0;
  pt->dir_min = 0.0, pt->dir_max = 0.0;
  pt->dir_incr = 0.0, pt->dir_wiggle = 0.0;
  pt->grav_amount = 0.0, pt->grav_dir = 0.0;

  enigma::pt_manager.max_id++;
  enigma::pt_manager.id_to_particletype.insert(std::pair<int,enigma::particle_type*>(enigma::pt_manager.max_id, pt));

  return enigma::pt_manager.max_id;
}
void part_type_destroy(int id);
void part_type_exists(int id);
void part_type_clear(int id);
// Shape.
void part_type_shape(int id, pt_shape particle_shape)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    enigma::particle_sprite* sprite = enigma::get_particle_sprite(particle_shape);
    if (sprite != 0) {
       (*it).second->part_sprite = sprite;
    }
  }
}
void part_type_size(int id, double size_min, double size_max, double size_incr, double size_wiggle)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->size_min = size_min;
    (*it).second->size_max = std::max(size_min, size_max);
    (*it).second->size_incr = size_incr;
    (*it).second->size_wiggle = size_wiggle;
  }
}
// Color and blending.
void part_type_color1(int id, int color1)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->c_mode = enigma::one_color;
    (*it).second->color1 = color1;
  }
}
void part_type_color2(int id, int color1, int color2)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->c_mode = enigma::two_color;
    (*it).second->color1 = color1;
    (*it).second->color2 = color2;
  }
}
void part_type_alpha1(int id, int alpha1)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->a_mode = enigma::one_alpha;
    (*it).second->alpha1 = alpha1;
  }
}
void part_type_alpha2(int id, int alpha1, int alpha2)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->a_mode = enigma::two_alpha;
    (*it).second->alpha1 = alpha1;
    (*it).second->alpha2 = alpha2;
  }
}
// Life and death.
void part_type_life(int id, int life_min, int life_max)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    life_min = std::max(life_min, 1);
    life_max = std::max(life_max, 1);
    (*it).second->life_min = std::min(life_min, life_max);
    (*it).second->life_max = std::max(life_min, life_max);
  }
}
void part_type_death(int id, int death_number, int death_type)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->death_on = true;
    (*it).second->death_number = death_number;
    (*it).second->death_particle_id = death_type;
  }
}
// Motion.
void part_type_speed(int id, double speed_min, double speed_max, double speed_incr, double speed_wiggle)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    speed_min = std::max(speed_min, 0.0);
    speed_max = std::max(speed_min, speed_max);
    (*it).second->speed_min = speed_min;
    (*it).second->speed_max = speed_max;
    (*it).second->speed_incr = speed_incr;
    (*it).second->speed_wiggle = speed_wiggle;
  }
}
void part_type_direction(int id, double dir_min, double dir_max, double dir_incr, double dir_wiggle)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    dir_min = std::max(dir_min, 0.0);
    dir_max = std::max(dir_min, dir_max);
    (*it).second->dir_min = dir_min;
    (*it).second->dir_max = dir_max;
    (*it).second->dir_incr = dir_incr;
    (*it).second->dir_wiggle = dir_wiggle;
  }
}
void part_type_gravity(int id, double grav_amount, double grav_dir)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
    (*it).second->grav_amount = grav_amount;
    (*it).second->grav_dir = grav_dir;
  }
}

