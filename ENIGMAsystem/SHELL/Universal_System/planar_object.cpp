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
  @file  planar_object.cpp
  @brief Adds a planar tier following the main tier.
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
    hspeed.vspd  = &vspeed.rval.d;
      hspeed.dir = &direction.rval.d;
      hspeed.spd = &speed.rval.d;
    vspeed.hspd  = &hspeed.rval.d;
      vspeed.dir = &direction.rval.d;
      vspeed.spd = &speed.rval.d;
    direction.spd    = &speed.rval.d;
      direction.hspd = &hspeed.rval.d;
      direction.vspd = &vspeed.rval.d;
    speed.dir    = &direction.rval.d;
      speed.hspd = &hspeed.rval.d;
      speed.vspd = &vspeed.rval.d;
  }
  object_planar::object_planar(unsigned _id, int objid): object_basic(_id,objid)
  {
    hspeed.vspd  = &vspeed.rval.d;
      hspeed.dir = &direction.rval.d;
      hspeed.spd = &speed.rval.d;
    vspeed.hspd  = &hspeed.rval.d;
      vspeed.dir = &direction.rval.d;
      vspeed.spd = &speed.rval.d;
    direction.spd    = &speed.rval.d;
      direction.hspd = &hspeed.rval.d;
      direction.vspd = &vspeed.rval.d;
    speed.dir    = &direction.rval.d;
      speed.hspd = &hspeed.rval.d;
      speed.vspd = &vspeed.rval.d;
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
