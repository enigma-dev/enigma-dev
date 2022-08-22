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

#ifndef ENIGMA_PS_EFFECTSENUMS
#define ENIGMA_PS_EFFECTSENUMS

#include "PS_effects_constants.h"

namespace enigma
{
  enum ef_effect {
    ef_effects_explosion = 0,
    ef_effects_ring,
    ef_effects_ellipse,
    ef_effects_firework,
    ef_effects_smoke,
    ef_effects_smokeup,
    ef_effects_star,
    ef_effects_spark,
    ef_effects_flare,
    ef_effects_cloud,
    ef_effects_rain,
    ef_effects_snow
  };
  inline ef_effect get_ef(int effect)
  {
    using namespace enigma_user;
    switch (effect) {
    case ef_explosion : return ef_effects_explosion;
    case ef_ring : return ef_effects_ring;
    case ef_ellipse : return ef_effects_ellipse;
    case ef_firework : return ef_effects_firework;
    case ef_smoke : return ef_effects_smoke;
    case ef_smokeup : return ef_effects_smokeup;
    case ef_star : return ef_effects_star;
    case ef_spark : return ef_effects_spark;
    case ef_flare : return ef_effects_flare;
    case ef_cloud : return ef_effects_cloud;
    case ef_rain : return ef_effects_rain;
    case ef_snow : return ef_effects_snow;
    default : return ef_effects_explosion;
    }
  }

  enum ef_size {
    ef_si_small = 0,
    ef_si_medium,
    ef_si_large
  };
  inline ef_size get_ef_size(int size_kind)
  {
    using namespace enigma_user;
    switch (size_kind) {
    case ef_size_small : return ef_si_small;
    case ef_size_medium : return ef_si_medium;
    case ef_size_large : return ef_si_large;
    default : return ef_si_medium;
    }
  }
}

#endif // ENIGMA_PS_EFFECTSENUMS

