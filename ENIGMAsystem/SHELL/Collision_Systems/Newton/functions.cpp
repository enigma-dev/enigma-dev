/** Copyright (C) 2013 Robert B. Colton
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
#include "Universal_System/collisions_object.h"
#include "Universal_System/instance_system.h" //iter
//#include "Universal_System/roomsystem.h"
#include "Collision_Systems/collision_mandatory.h" //iter
#include "Universal_System/instance.h"

#include <string>
#include <stdio.h>
#include <vector>
using std::vector;
//using namespace std;

#include "functions.h"
bool systemPaused = false;

static inline double r2d(double r) { return r * 180 / M_PI; }

/* This is just place holder shit to get the fuckin linker shut the fuckin hell up */
enigma::instance_t collision_rectangle(double x1, double y1, double x2, double y2, int obj, bool prec, bool notme)
{

}

namespace enigma
{ 
  void *get_collision_mask(sprite* spr, unsigned char* input_data, collision_type ct) // It is called for every subimage of every sprite loaded.
  {
    return 0;
  }

  void free_collision_mask(void* mask)
  {
  }
};

bool place_free(double x,double y)
{
  return true;
}

bool place_meeting(double x, double y, int object)
{
  return false;
}
