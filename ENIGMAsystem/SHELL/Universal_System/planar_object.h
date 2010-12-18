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
  @file  graphics_object.h
  @brief Adds a planar tier following the main tier.
*/

#ifndef _planar_object_h
#define _planar_object_h

#include "object.h"
#include "var4.h"
#include "reflexive_types.h"

namespace enigma
{
  struct object_planar: object_basic
  {
    //Position
      double  x, y;
      double  xprevious, yprevious;
      double  xstart, ystart;
    
    //Persistence
      bool  persistent;
    
    //Motion
      directionv direction;
      speedv     speed;
      hspeedv    hspeed;
      vspeedv    vspeed;
      
    //Accelerators
      double  gravity;
      double  gravity_direction;
      double  friction;
    
    //Constructors
      object_planar();
      object_planar(unsigned, int);
      virtual ~object_planar();
  };
  
  void propagate_locals(object_planar*);
}

#endif //_planar_object_h
