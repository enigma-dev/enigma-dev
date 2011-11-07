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
  void directionv::function() {
    *reflex2 = *reflex1 * cos(rval.d*M_PI/180);
    *reflex3 = *reflex1 * -sin(rval.d*M_PI/180);
  }

  //Make speed work -- same as above, but rval.d and reflex1 are switched.
  void speedv::function() {
    *reflex2 = rval.d * cos(*reflex1*M_PI/180);
    *reflex3 = rval.d * -sin(*reflex1*M_PI/180);
  }

  //Make hspeed work
  void hspeedv::function() {
    *reflex2 = (int(180+180*(1-atan2(*reflex1,rval.d)/M_PI)))%360;
    *reflex3 = hypot(rval.d,*reflex1);
  }

  //Make vspeed work -- Same as above, except the arguments to atan2 are reversed
  void vspeedv::function() {
    *reflex2 = (int(180+180*(1-atan2(rval.d,*reflex1)/M_PI)))%360;
    *reflex3 = hypot(rval.d,*reflex1);
  }
}
