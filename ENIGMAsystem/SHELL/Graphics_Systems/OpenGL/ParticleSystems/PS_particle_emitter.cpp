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

#include "PS_particle_emitter.h"
#include "PS_particle_enums.h"
#include <cstdlib>
#include <algorithm>

namespace enigma
{
  void particle_emitter::initialize()
  {
    xmin = 0, xmax = 0, ymin = 0, ymax = 0;
    shape = ps_shape_rectangle;
    distribution = ps_distr_linear;
    particle_type_id = -1;
    number = 0;
  }
  particle_emitter* create_particle_emitter()
  {
    particle_emitter* pe = new particle_emitter();
    pe->initialize();
    return pe;
  }
  void particle_emitter::clear_particle_emitter()
  {
    initialize();
  }
  void particle_emitter::set_region(double xmin, double xmax, double ymin, double ymax, ps_shape shape, ps_distr distribution)
  {
    this->xmin = xmin;
    this->xmax = xmax;
    this->ymin = ymin;
    this->ymax = ymax;

    this->shape = shape;
    this->distribution = distribution;
  }
  void particle_emitter::set_stream(int particle_type_id, int number)
  {
    this->particle_type_id = particle_type_id;
    this->number = number;
  }
  void particle_emitter::get_point(int& x, int&y)
  {
    // TODO: Missing shapes and distributions.

    switch (shape) {
    case ps_shape_rectangle: {
      x = std::min(xmin, xmax) + rand() % abs(xmin-xmax) + 1;
      y = std::min(ymin, ymax) + rand() % abs(ymin-ymax) + 1;
      break;
    }
    }
  }
  int particle_emitter::get_step_number()
  {
    if (number >= 0) {
      return number;
    }
    else {
      return rand() % (-number) < 1 ? 1 : 0; // Create particle with probability -1/number.
    }
  }
}

