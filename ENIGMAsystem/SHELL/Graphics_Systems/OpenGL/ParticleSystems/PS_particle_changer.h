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

#ifndef ENIGMA_PS_PARTICLECHANGER
#define ENIGMA_PS_PARTICLECHANGER

#include "PS_particle_enums.h"

namespace enigma
{
  struct particle_changer
  {
    double xmin, xmax, ymin, ymax;
    ps_shape shape;
    int parttypeid1;
    int parttypeid2;
    ps_change change_kind;

    void initialize();
    void clear_particle_changer();
    void set_region(double xmin, double xmax, double ymin, double ymax, ps_shape shape);
    void set_types(int parttypeid1, int parttypeid2);
    void set_change_kind(ps_change change_kind);
    bool is_inside(double x, double y);
  };

  particle_changer* create_particle_changer();
}

#endif // ENIGMA_PS_PARTICLECHANGER

