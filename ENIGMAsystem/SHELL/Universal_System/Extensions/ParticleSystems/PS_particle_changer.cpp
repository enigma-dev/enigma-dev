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
#include "PS_particle_system.h"
#include "PS_particle_system_manager.h"
#include "PS_particle.h"
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
  void particle_changer::set_region(double p_xmin, double p_xmax, double p_ymin, double p_ymax, ps_shape p_shape)
  {
    // NOTE: The assignment to the region's coordinates means that if p_xmin >= p_xmax, the region is empty.
    // This has been tested and verified to be consistent with GM.
    xmin = std::min(p_xmin, p_xmax);
    xmax = p_xmax;
    ymin = std::min(p_ymin, p_ymax);
    ymax = p_ymax;
    shape = p_shape;
  }
  void particle_changer::set_types(int pt_id1, int pt_id2)
  {
    parttypeid1 = pt_id1;
    parttypeid2 = pt_id2;
  }
  void particle_changer::set_change_kind(ps_change ps_change_kind)
  {
    change_kind = ps_change_kind;
  }
  inline double sqr(double x) {return x*x;}
  bool particle_changer::is_inside(double x, double y)
  {
    // Regions with coordinates along the same axis which are the same are empty.
    // This has been tested and verified to be consistent with GM.
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
using enigma::particle_changer;

namespace enigma_user {
  int part_changer_create(int id)
  {
    particle_system* p_s = enigma::get_particlesystem(id);
    if (p_s != NULL) {
      return p_s->create_changer();
    }
    return -1;
  }
  void part_changer_destroy(int ps_id, int ch_id)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_changer*>::iterator ch_it = p_s->id_to_changer.find(ch_id);
      if (ch_it != p_s->id_to_changer.end()) {
        delete (*ch_it).second;
        p_s->id_to_changer.erase(ch_it);
      }
    }
  }
  void part_changer_destroy_all(int ps_id)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      for (std::map<int,particle_changer*>::iterator it = p_s->id_to_changer.begin(); it != p_s->id_to_changer.end(); it++)
      {
        delete (*it).second;
      }
      p_s->id_to_changer.clear();
    }
  }
  bool part_changer_exists(int ps_id, int ch_id)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_changer*>::iterator ch_it = p_s->id_to_changer.find(ch_id);
      if (ch_it != p_s->id_to_changer.end()) {
        return true;
      }
    }
    return false;
  }
  void part_changer_clear(int ps_id, int ch_id)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_changer*>::iterator ch_it = p_s->id_to_changer.find(ch_id);
      if (ch_it != p_s->id_to_changer.end()) {
        (*ch_it).second->initialize();
      }
    }
  }
  void part_changer_region(int ps_id, int ch_id, double xmin, double xmax, double ymin, double ymax, int shape)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_changer*>::iterator ch_it = p_s->id_to_changer.find(ch_id);
      if (ch_it != p_s->id_to_changer.end()) {
        (*ch_it).second->set_region(xmin, xmax, ymin, ymax, enigma::get_ps_shape(shape));
      }
    }
  }
  void part_changer_types(int ps_id, int ch_id, int parttype1, int parttype2)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL && enigma::get_particletype(parttype1) != NULL && enigma::get_particletype(parttype2) != NULL) {
      std::map<int,particle_changer*>::iterator ch_it = p_s->id_to_changer.find(ch_id);
      if (ch_it != p_s->id_to_changer.end()) {
        (*ch_it).second->set_types(parttype1, parttype2);
      }
    }
  }
  void part_changer_kind(int ps_id, int ch_id, int kind)
  {
    particle_system* p_s = enigma::get_particlesystem(ps_id);
    if (p_s != NULL) {
      std::map<int,particle_changer*>::iterator ch_it = p_s->id_to_changer.find(ch_id);
      if (ch_it != p_s->id_to_changer.end()) {
        (*ch_it).second->set_change_kind(enigma::get_ps_change(kind));
      }
    }
  }
}

