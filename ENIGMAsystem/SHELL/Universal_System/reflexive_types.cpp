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

#include <cmath>
#include <cstdlib>
#include <string>
#include "var4.h"
#include "reflexive_types.h"
#include "math_consts.h"

// Variable not zero.
inline bool varnz(double x) { return fabs(x) > variant::epsilon; }

namespace enigma {
  //Make direction work
  void directionv::function(const variant&) {
    rval.d = fmod(rval.d, 360.0);
    if(rval.d < 0.0){
      rval.d += 360.0;
    }
    *hspd = *spd * cos(rval.d*M_PI/180.0);
    *vspd = *spd *-sin(rval.d*M_PI/180.0);
  }

  //Make speed work -- same as above, but rval.d and reflex1 are switched.
  void speedv::function(const variant&) {
    *hspd = rval.d * cos(*dir*M_PI/180.0);
    *vspd = rval.d *-sin(*dir*M_PI/180.0);
  }

  //Make hspeed work
  void hspeedv::function(const variant &oldval) {
    if (rval.d == oldval.rval.d)
    { // If no changes, return, don't make potentially negative speed non-negative.
        return;
    }
    else if (varnz(rval.d) or varnz(*vspd))
    {
        *dir = int((180.0+180.0*(1.0-atan2(*vspd,rval.d)/M_PI))+0.5)%360; //The +0.5 rounds it
        *spd = hypot(rval.d,*vspd);
    }
    else
    { // If both set to zero, make speed zero. Direction not set.
        *spd = 0.0;
    }
  }

  //Make vspeed work -- Same as above, except the arguments to atan2 are reversed
  void vspeedv::function(const variant &oldval) {
    if (rval.d == oldval.rval.d)
    { // If no changes, return, don't make potentially negative speed non-negative.
        return;
    }
    else if (varnz(rval.d) or varnz(*hspd))
    {
        *dir = int((180.0+180.0*(1.0-atan2(rval.d,*hspd)/M_PI))+0.5)%360; //The +0.5 rounds it
        *spd = hypot(rval.d,*hspd);
    }
    else
    { // If both set to zero, make speed zero. Direction not set.
        *spd = 0.0;
    }
  }
}
