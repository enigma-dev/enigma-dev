/** Copyright (C) 2010-2011 Josh Ventura
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <math.h>
#include <string>
#include "var4.h"
#include "reflexive_types.h"

namespace enigma {
  //Make direction work
  INTERCEPT_DEFAULT_COPY(directionv);
  void directionv::function() {
    *hspd = *spd * cos(rval.d*M_PI/180);
    *vspd = *spd *-sin(rval.d*M_PI/180);
  }

  //Make speed work -- same as above, but rval.d and reflex1 are switched.
  INTERCEPT_DEFAULT_COPY(speedv);
  void speedv::function() {
    *hspd = rval.d * cos(*dir*M_PI/180);
    *vspd = rval.d *-sin(*dir*M_PI/180);
  }

  //Make hspeed work
  INTERCEPT_DEFAULT_COPY(hspeedv);
  void hspeedv::function() {
    if (rval.d or *vspd)
    {
        *dir = (int(180+180*(1-atan2(*vspd,rval.d)/M_PI)))%360;
        *spd = hypot(rval.d,*vspd);
    }
  }

  //Make vspeed work -- Same as above, except the arguments to atan2 are reversed
  INTERCEPT_DEFAULT_COPY(vspeedv);
  void vspeedv::function() {
    if (rval.d or *hspd)
    {
        *dir = (int(180+180*(1-atan2(rval.d,*hspd)/M_PI)))%360;
        *spd = hypot(rval.d,*hspd);
    }
  }
}
