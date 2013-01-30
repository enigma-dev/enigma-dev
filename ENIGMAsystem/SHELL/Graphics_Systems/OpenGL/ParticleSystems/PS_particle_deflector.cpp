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
#include "PS_particle_enums.h"
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
    return xmin != xmax && ymin != ymax && x >= xmin && x <= xmax && y >= ymin && y <= ymax;
  }
  void particle_deflector::set_region(double xmin, double xmax, double ymin, double ymax)
  {
    this->xmin = std::min(xmin, xmax);
    this->xmax = xmax;
    this->ymin = std::min(ymin, ymax);
    this->ymax = ymax;
  }
  void particle_deflector::set_kind(ps_deflect deflection_kind)
  {
    this->deflection_kind = deflection_kind;
  }
  void particle_deflector::set_friction(double friction)
  {
    this->friction = friction;
  }
}

