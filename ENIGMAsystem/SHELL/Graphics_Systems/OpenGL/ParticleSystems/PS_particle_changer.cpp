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
    }
  }
}

