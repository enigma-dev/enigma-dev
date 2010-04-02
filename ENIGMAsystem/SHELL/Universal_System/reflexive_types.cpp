/*********************************************************************************\
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
\*********************************************************************************/

#include <math.h>
#include <string>
#include "var_cr3.h"

//Make direction work
#define TYPEPURPOSE directionv
#define TYPEFUNCTION *reflex2=*reflex1*cos(realval/(180*M_PI)); *reflex3=*reflex1*-sin(realval/(180*M_PI));
#include "multifunction_variant_source.h"
#undef TYPEFUNCTION
#undef TYPEPURPOSE

//Make speed work -- same as above, but realval and reflex1 are switched.
#define TYPEPURPOSE speedv
#define TYPEFUNCTION *reflex2=realval*cos(*reflex1/(180*M_PI)); *reflex3=realval*-sin(*reflex1/(180*M_PI));
#include "multifunction_variant_source.h"
#undef TYPEFUNCTION
#undef TYPEPURPOSE

//Make hspeed work
#define TYPEPURPOSE hspeedv
#define TYPEFUNCTION *reflex2=(int(180+180*(1-atan2(*reflex1,realval)/M_PI)))%360; *reflex3=hypot(realval,*reflex1);
#include "multifunction_variant_source.h"
#undef TYPEFUNCTION
#undef TYPEPURPOSE

//Make vspeed work -- Same as above, except the arguments to atan2 are reversed
#define TYPEPURPOSE vspeedv
#define TYPEFUNCTION *reflex2=(int(180+180*(1-atan2(realval,*reflex1)/M_PI)))%360; *reflex3=hypot(realval,*reflex1);
#include "multifunction_variant_source.h"
#undef TYPEFUNCTION
#undef TYPEPURPOSE

