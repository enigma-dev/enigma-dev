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

#ifndef _reflexive_types_h
#define _reflexive_types_h

#include "multifunction_variant.h"

namespace enigma {
  struct directionv: multifunction_variant {
    INHERIT_OPERATORS();
    double *reflex1, *reflex2, *reflex3;
    void function();
  };

  struct speedv: multifunction_variant {
    INHERIT_OPERATORS();
    double *reflex1, *reflex2, *reflex3;
    void function();
  };

  struct hspeedv: multifunction_variant {
    INHERIT_OPERATORS();
    double *reflex1, *reflex2, *reflex3;
    void function();
  };

  struct vspeedv: multifunction_variant {
    INHERIT_OPERATORS();
    double *reflex1, *reflex2, *reflex3;
    void function();
  };
}

#endif //_reflexive_types_h
