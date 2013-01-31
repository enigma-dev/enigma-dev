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

#include "PS_particle_changer.h"
#include "PS_particle_enums.h"
#include "PS_particle_system.h"
#include "PS_particle_system_manager.h"
#include <algorithm>
#include <cmath>

namespace enigma
{
  void particle_changer::initialize()
  {
    xmin = 0.0, xmax = 0.0;
    ymin = 0.0, ymax = 0.0;
    shape = ps_sh_rectangle;
    parttypeid1 = -1;
    parttypeid2 = -1;
    change_kind = ps_ch_all;
  }
  particle_changer* create_particle_changer()
  {
    particle_changer* pc = new particle_changer();
    pc->initialize();
    return pc;
  }
  void particle_changer::clear_particle_changer()
  {
    initialize();
  }
  void particle_changer::set_region(double xmin, double xmax, double ymin, double ymax, ps_shape shape)
  {
    xmin = std::min(xmin, xmax);
    ymin = std::min(ymin, ymax);
    this->xmin = xmin;
    this->xmax = std::max(xmin, xmax);
    this->ymin = ymin;
    this->ymax = std::max(ymin, ymax);
    this->shape = shape;
  }
  void particle_changer::set_types(int parttypeid1, int parttypeid2)
  {
    this->parttypeid1 = parttypeid1;
    this->parttypeid2 = parttypeid2;
  }
  void particle_changer::set_change_kind(ps_change change_kind)
  {
    this->change_kind = change_kind;
  }
  inline double sqr(double x) {return x*x;}
  bool particle_changer::is_inside(double x, double y)
  {
    if (xmin >= xmax || ymin >= ymax || x < xmin || x > xmax || y < ymin || y > ymax) {
      return false;
    }
    switch (shape) {
    case ps_sh_rectangle : {
      return x >= xmin && x <= xmax && y >= ymin && y <= ymax;
    }
    case ps_sh_ellipse : {
      const double a = (xmax - xmin)/2.0;
      const double b = (ymax - ymin)/2.0;
      return sqr(x - xmin - a)/sqr(a) + sqr(y - ymin - b)/sqr(b) <= 1;
    }
    case ps_sh_diamond : {
      const double a = (xmax - xmin)/2.0;
      const double b = (ymax - ymin)/2.0;
      const double center_x = xmin + a;
      const double center_y = ymin + b;
      const double dx = (x - center_x)/a;
      const double dy = (y - center_y)/b;
      return fabs(dx) + fabs(dy) <= 1.0;
    }
    case ps_sh_line: {
      return false;
    }
    default: {
      return false;
    }
    }
  }
}

using enigma::particle_system;
using enigma::particle_type;
using enigma::ps_manager;
using enigma::particle_changer;
using enigma::pt_manager;

int part_changer_create(int id)
{
  std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(id);
  if (ps_it != ps_manager.id_to_particlesystem.end()) {
    return (*ps_it).second->create_changer();
  }
  return -1;
}
void part_changer_destroy(int ps_id, int ch_id)
{
  std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(ps_id);
  if (ps_it != ps_manager.id_to_particlesystem.end()) {
    particle_system* p_s = (*ps_it).second;
    std::map<int,particle_changer*>::iterator ch_it = p_s->id_to_changer.find(ch_id);
    if (ch_it != p_s->id_to_changer.end()) {
      delete (*ch_it).second;
      p_s->id_to_changer.erase(ch_it);
    }
  }
}
void part_changer_destroy_all(int ps_id)
{
  std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(ps_id);
  if (ps_it != ps_manager.id_to_particlesystem.end()) {
    particle_system* p_s = (*ps_it).second;
    for (std::map<int,particle_changer*>::iterator it = p_s->id_to_changer.begin(); it != p_s->id_to_changer.end(); it++)
    {
      delete (*it).second;
    }
    p_s->id_to_changer.clear();
  }
}
bool part_changer_exists(int ps_id, int ch_id)
{
  std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(ps_id);
  if (ps_it != ps_manager.id_to_particlesystem.end()) {
    particle_system* p_s = (*ps_it).second;
    std::map<int,particle_changer*>::iterator ch_it = p_s->id_to_changer.find(ch_id);
    if (ch_it != p_s->id_to_changer.end()) {
      return true;
    }
  }
  return false;
}
void part_changer_clear(int ps_id, int ch_id)
{
  std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(ps_id);
  if (ps_it != ps_manager.id_to_particlesystem.end()) {
    particle_system* p_s = (*ps_it).second;
    std::map<int,particle_changer*>::iterator ch_it = p_s->id_to_changer.find(ch_id);
    if (ch_it != p_s->id_to_changer.end()) {
      (*ch_it).second->initialize();
    }
  }
}
void part_changer_region(int ps_id, int ch_id, double xmin, double xmax, double ymin, double ymax, int shape)
{
  std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(ps_id);
  if (ps_it != ps_manager.id_to_particlesystem.end()) {
    particle_system* p_s = (*ps_it).second;
    std::map<int,particle_changer*>::iterator ch_it = p_s->id_to_changer.find(ch_id);
    if (ch_it != p_s->id_to_changer.end()) {
      (*ch_it).second->set_region(xmin, xmax, ymin, ymax, enigma::get_ps_shape(shape));
    }
  }
}
void part_changer_types(int ps_id, int ch_id, int parttype1, int parttype2)
{
  std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(ps_id);
  if (ps_it != ps_manager.id_to_particlesystem.end()) {
    particle_system* p_s = (*ps_it).second;
    std::map<int,particle_changer*>::iterator ch_it = p_s->id_to_changer.find(ch_id);
    if (ch_it != p_s->id_to_changer.end()) {
      (*ch_it).second->set_types(parttype1, parttype2);
    }
  }
}
void part_changer_kind(int ps_id, int ch_id, int kind)
{
  std::map<int,particle_system*>::iterator ps_it = ps_manager.id_to_particlesystem.find(ps_id);
  if (ps_it != ps_manager.id_to_particlesystem.end()) {
    particle_system* p_s = (*ps_it).second;
    std::map<int,particle_changer*>::iterator ch_it = p_s->id_to_changer.find(ch_id);
    if (ch_it != p_s->id_to_changer.end()) {
      (*ch_it).second->set_change_kind(enigma::get_ps_change(kind));
    }
  }
}

