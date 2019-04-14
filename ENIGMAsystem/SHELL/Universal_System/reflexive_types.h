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

#ifndef ENIGMA_REFLEXIVE_TYPES_H
#define ENIGMA_REFLEXIVE_TYPES_H

#include "multifunction_variant.h"

namespace enigma {
  struct directionv: multifunction_variant<directionv> {
    INHERIT_OPERATORS(directionv)
    double *spd, *hspd, *vspd;
    void function(const variant &oldval);
  };

  struct speedv: multifunction_variant<speedv> {
    INHERIT_OPERATORS(speedv)
    double *dir, *hspd, *vspd;
    void function(const variant &oldval);
  };

  struct hspeedv: multifunction_variant<hspeedv> {
    INHERIT_OPERATORS(hspeedv)
    double *vspd, *dir, *spd;
    void function(const variant &oldval);
  };

  struct vspeedv: multifunction_variant<vspeedv> {
    INHERIT_OPERATORS(vspeedv)
    double *hspd, *dir, *spd;
    void function(const variant &oldval);
  };
}

#endif // ENIGMA_REFLEXIVE_TYPES_H
