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

#ifndef ENIGMA_PS_PARTICLEDEFLECTOR
#define ENIGMA_PS_PARTICLEDEFLECTOR

#include "PS_particle_enums.h"

namespace enigma {
struct particle_deflector {
  double xmin, xmax, ymin, ymax;
  ps_deflect deflection_kind;
  double friction;  // NOTE: Can be negative.

  void initialize();
  void clear_particle_deflector();
  bool is_inside(double x, double y);
  void set_region(double xmin, double xmax, double ymin, double ymax);
  void set_kind(ps_deflect deflection_kind);
  void set_friction(double friction);
};

particle_deflector* create_particle_deflector();
}  // namespace enigma

#endif  // ENIGMA_PS_PARTICLEDEFLECTOR
