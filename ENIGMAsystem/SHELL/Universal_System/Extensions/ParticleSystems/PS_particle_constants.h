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

#ifndef ENIGMA_PS_PARTICLECONSTANTS
#define ENIGMA_PS_PARTICLECONSTANTS

namespace enigma_user {
  enum
  {
    pt_shape_pixel = 0,
    pt_shape_disk,
    pt_shape_square,
    pt_shape_line,
    pt_shape_star,
    pt_shape_circle,
    pt_shape_ring,
    pt_shape_sphere,
    pt_shape_flare,
    pt_shape_spark,
    pt_shape_explosion,
    pt_shape_cloud,
    pt_shape_smoke,
    pt_shape_snow
  };

  enum
  {
    ps_shape_rectangle = 0,
    ps_shape_ellipse,
    ps_shape_diamond,
    ps_shape_line
  };

  enum
  {
    ps_distr_linear = 0,
    ps_distr_gaussian,
    ps_distr_invgaussian
  };

  enum
  {
    ps_force_constant = 0,
    ps_force_linear,
    ps_force_quadratic
  };

  enum
  {
    ps_deflect_horizontal = 0,
    ps_deflect_vertical
  };

  enum
  {
    ps_change_all = 0
    // NOTE: ps_change_motion and ps_change_shape are intentionally not
    // implemented. The reasoning for this decision is that their
    // interface is loosely defined, and an existing implementation
    // seems contrary to the loosely defined interface.
  };
}

#endif // ENIGMA_PS_PARTICLECONSTANTS

