/********************************************************************************\
**                                                                              **
**  Copyright (C) 2013 Robert B. Colton                                         **
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
#include "Universal_System/collisions_object.h"
#include "Universal_System/instance_system.h" //iter
//#include "Universal_System/roomsystem.h"
#include "Collision_Systems/collision_mandatory.h" //iter
#include "Universal_System/instance.h"

#include <Box2D/Box2D.h>

#include <string>
#include <stdio.h>

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

}

bool place_meeting(double x, double y, int object)
{

}

/* This is where the actual physics_* functions are */

void physics_world_create()
{
	// Define the gravity vector.
	b2Vec2 gravity(0.0f, -10.0f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	//b2World world(gravity);
}
