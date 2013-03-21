/********************************************************************************\
**                                                                              **
**  Copyright (C) 2012-2013 forthevin                                           **
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

#ifndef ENIGMA_PS_PARTICLEINSTANCE
#define ENIGMA_PS_PARTICLEINSTANCE

#include "PS_particle_type.h"

namespace enigma
{
  struct particle_instance
  {
    particle_type* pt;

    int sprite_subimageindex_initial;
    double size;
    double size_wiggle_offset; // [-1;1].
    double angle;
    double ang_wiggle_offset; // [-1;1].
    int color;
    int alpha;
    int life_current, life_start;
    double x, y;
    double speed, direction;
    double speed_wiggle_offset; // [-1;1].
    double dir_wiggle_offset; // [-1;1].
  };
}

#endif // ENIGMA_PS_PARTICLEINSTANCE

