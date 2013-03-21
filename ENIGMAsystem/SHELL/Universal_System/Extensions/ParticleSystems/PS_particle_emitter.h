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

#ifndef ENIGMA_PS_PARTICLEEMITTER
#define ENIGMA_PS_PARTICLEEMITTER

#include "PS_particle_enums.h"

namespace enigma
{
  struct particle_emitter
  {
    double xmin, ymin, xmax, ymax;
    ps_shape shape;
    ps_distr distribution;
    int particle_type_id; // -1 for no id.
    int number;

    void initialize();
    void clear_particle_emitter();
    void set_region(double xmin, double ymin, double xmax, double ymax, ps_shape shape, ps_distr distribution);
    void set_burst(int particle_type_id, int number);
    void set_stream(int particle_type_id, int number);
    void get_point(int& x, int& y);
    int get_step_number();
  };

  particle_emitter* create_particle_emitter();
}

#endif // ENIGMA_PS_PARTICLEEMITTER

