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

#ifndef _MULTIFUNCTION_VARIANT__H
#define _MULTIFUNCTION_VARIANT__H

#include "var_te.h"

namespace enigma
{
  struct multifunction_variant: variant
  {
    #undef  types_extrapolate_alldec
    #define types_extrapolate_alldec(prefix)\
     types_extrapolate_real_p  (prefix,;)\
     types_extrapolate_string_p(prefix,;)\
     prefix (const variant &x);\
     prefix (const var &x);
    
    virtual void function();
    
    //These are assignment operators and require a reference to be passed
    types_extrapolate_alldec(multifunction_variant& operator=);
    types_extrapolate_alldec(multifunction_variant& operator+=);
    types_extrapolate_alldec(multifunction_variant& operator-=);
    types_extrapolate_alldec(multifunction_variant& operator*=);
    types_extrapolate_alldec(multifunction_variant& operator/=);
    types_extrapolate_alldec(multifunction_variant& operator<<=);
    types_extrapolate_alldec(multifunction_variant& operator>>=);
    types_extrapolate_alldec(multifunction_variant& operator&=);
    types_extrapolate_alldec(multifunction_variant& operator|=);
    types_extrapolate_alldec(multifunction_variant& operator^=);
    
    virtual ~multifunction_variant();
    #undef  types_extrapolate_alldec
  };
}

#define INHERIT_OPERATORS()\
  using multifunction_variant::operator=;

#endif
