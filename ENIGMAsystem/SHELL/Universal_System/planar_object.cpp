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
#include "var_cr3.h"
#include "reflexive_types.h"

#include "planar_object.h"

namespace enigma
{
  object_planar::object_planar() {}
  object_planar::object_planar(unsigned x, int y): object_basic(x,y) {}
  
  void step_basic(object_basic* instance_b)
  {
    object_planar *instance = (object_planar*)instance_b;
    instance->xprevious = instance->x;
    instance->yprevious=instance->y;
    if(instance->gravity || instance->friction)
    {
      double hb4 = instance->hspeed.realval, vb4 = instance->vspeed.realval;
      int sign = (instance->speed > 0) - (instance->speed < 0);
      
      instance->hspeed.realval -= (sign * instance->friction) * cos(instance->direction.realval * M_PI/180);
      if ((hb4>0 && instance->hspeed.realval<0) || (hb4<0 && instance->hspeed.realval>0))
        instance->hspeed.realval=0;
      instance->vspeed.realval -= (sign * instance->friction) * -sin(instance->direction.realval * M_PI/180);
      if ((vb4>0 && instance->vspeed.realval<0) || (vb4<0 && instance->vspeed.realval>0))
        instance->vspeed.realval=0;
      
      instance->hspeed.realval += (instance->gravity) * cos(instance->gravity_direction * M_PI/180);
      instance->vspeed.realval += (instance->gravity) * -sin(instance->gravity_direction * M_PI/180);
      
      if(instance->speed.realval<0) {
        instance->direction.realval=fmod(180+instance->direction.realval,360);//180+(int(180+180*(1-atan2(instance->vspeed.realval,instance->hspeed.realval)/pi)))%360;
        instance->speed.realval=-hypotf((instance->hspeed.realval),(instance->vspeed.realval));
      }
      else {
        instance->direction.realval=fmod(instance->direction.realval,360);//(int(180+180*(1-atan2(instance->vspeed.realval,instance->hspeed.realval)/pi)))%360;
        instance->speed.realval=hypotf((instance->hspeed.realval),(instance->vspeed.realval));
      }
      if(instance->direction.realval<0) instance->direction.realval+=360;
    }
    
    instance->x+=instance->hspeed.realval;
    instance->y+=instance->vspeed.realval;
  }
}
