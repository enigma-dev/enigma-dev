/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
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

/**
  @file    graphics_object.h
  @summary Adds a planar tier following the main tier.
*/

#include <math.h>

#include "object.h"
#include "var4.h"
#include "reflexive_types.h"

#include "planar_object.h"

namespace enigma
{
  object_planar::object_planar()
  {
    hspeed.reflex1   = &vspeed.rval.d;
      hspeed.reflex2 = &direction.rval.d;
      hspeed.reflex3 = &speed.rval.d;
    vspeed.reflex1    = &hspeed.rval.d;
      vspeed.reflex2  = &direction.rval.d;
      vspeed.reflex3  = &speed.rval.d;
    direction.reflex1   = &speed.rval.d;
      direction.reflex2 = &hspeed.rval.d;
      direction.reflex3 = &vspeed.rval.d;
    speed.reflex1   = &direction.rval.d;
      speed.reflex2 = &hspeed.rval.d;
      speed.reflex3 = &vspeed.rval.d;
  }
  object_planar::object_planar(unsigned id, int objid): object_basic(id,objid)
  {
    hspeed.reflex1   = &vspeed.rval.d;
      hspeed.reflex2 = &direction.rval.d;
      hspeed.reflex3 = &speed.rval.d;
    vspeed.reflex1    = &hspeed.rval.d;
      vspeed.reflex2  = &direction.rval.d;
      vspeed.reflex3  = &speed.rval.d;
    direction.reflex1   = &speed.rval.d;
      direction.reflex2 = &hspeed.rval.d;
      direction.reflex3 = &vspeed.rval.d;
    speed.reflex1   = &direction.rval.d;
      speed.reflex2 = &hspeed.rval.d;
      speed.reflex3 = &vspeed.rval.d;
  }

  //This just needs implemented virtually so instance_destroy works.
  object_planar::~object_planar() {}

  void propagate_locals(object_planar* instance)
  {
    if(instance->gravity || instance->friction)
    {
      double
        hb4 = instance->hspeed.rval.d,
        vb4 = instance->vspeed.rval.d;
      int sign = (instance->speed > 0) - (instance->speed < 0);
      if (instance->hspeed!=0)
        instance->hspeed.rval.d -= (sign * instance->friction) * cos(instance->direction.rval.d * M_PI/180);
      if ((hb4>0 && instance->hspeed.rval.d<0) || (hb4<0 && instance->hspeed.rval.d>0))
        instance->hspeed.rval.d=0;
        if (instance->vspeed!=0)
        instance->vspeed.rval.d -= (sign * instance->friction) * -sin(instance->direction.rval.d * M_PI/180);
      if ((vb4>0 && instance->vspeed.rval.d<0) || (vb4<0 && instance->vspeed.rval.d>0))
        instance->vspeed.rval.d=0;

      instance->hspeed.rval.d += (instance->gravity) * cos(instance->gravity_direction * M_PI/180);
      instance->vspeed.rval.d += (instance->gravity) *-sin(instance->gravity_direction * M_PI/180);

      if(instance->speed.rval.d<0)
        instance->direction.rval.d = fmod(instance->direction.rval.d + 180, 360),
        instance->speed.    rval.d = -hypotf(instance->hspeed.rval.d, instance->vspeed.rval.d);
      else
        instance->direction.rval.d = fmod(instance->direction.rval.d, 360),
        instance->speed.    rval.d =  hypotf(instance->hspeed.rval.d, instance->vspeed.rval.d);
      if(instance->direction.rval.d < 0)
        instance->direction.rval.d += 360;
    }

    instance->x += instance->hspeed.rval.d;
    instance->y += instance->vspeed.rval.d;
  }
}
