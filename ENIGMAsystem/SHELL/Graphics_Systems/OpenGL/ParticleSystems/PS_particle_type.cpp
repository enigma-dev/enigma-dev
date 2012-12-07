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

namespace enigma
{
  particle_type_manager pt_manager;
}

int part_type_create()
{ 
  enigma::particle_type* pt = new enigma::particle_type();
  pt->particle_count = 0;
  pt->alive = true;
  pt->life_min = 100;
  pt->life_max = 100;
  pt->speed_min = 0.0, pt->speed_max = 0.0;
  pt->speed_incr = 0.0, pt->speed_wiggle = 0.0;
  pt->dir_min = 0.0, pt->dir_max = 0.0;
  pt->dir_incr = 0.0, pt->dir_wiggle = 0.0;

  enigma::pt_manager.max_id++;
  enigma::pt_manager.id_to_particletype.insert(std::pair<int,enigma::particle_type*>(enigma::pt_manager.max_id, pt));

  return enigma::pt_manager.max_id;
}
void part_type_destroy(int id);
void part_type_exists(int id);
void part_type_clear(int id);
// Motion.
void part_type_speed(int id, double speed_min, double speed_max, double speed_incr, double speed_wiggle)
{
  std::map<int,enigma::particle_type*>::iterator it = enigma::pt_manager.id_to_particletype.find(id);
  if (it != enigma::pt_manager.id_to_particletype.end()) {
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
    (*it).second->dir_min = dir_min;
    (*it).second->dir_max = dir_max;
    (*it).second->dir_incr = dir_incr;
    (*it).second->dir_wiggle = dir_wiggle;
  }
}

