/********************************************************************************\
**                                                                              **
**  Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>                          **
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

////////////////////////////////////
// GM front-end functions - Implementations of standard GM collision functions.
////////////////////////////////////

#include "../../Universal_System/collisions_object.h"
#include "coll_impl.h"

bool place_free(double x,double y)
{
  return collide_inst_inst(all,true,true,x,y) == NULL;
}

bool place_empty(double x,double y)
{
  return collide_inst_inst(all,false,true,x,y) == NULL;
}

bool place_meeting(double x, double y, int object)
{
  return collide_inst_inst(object,false,true,x,y);
}

namespace enigma {
  object_basic *place_meeting_inst(double x, double y, int object)
  {
    return collide_inst_inst(object,false,true,x,y);
  }
}


bool position_free(double x,double y)
{
  return collide_inst_point(all,true,true,x,y) == NULL;
}

bool position_empty(double x, double y)
{
  return collide_inst_point(all,false,true,x,y) == NULL;
}

bool position_meeting(double x, double y, int object)
{
  return collide_inst_point(object,false,true,x,y);
}

int instance_place(double x, double y, int object)
{
  enigma::object_collisions* const r = collide_inst_inst(object,false,true,x,y);
  return r == NULL ? noone : r->id;
}

int instance_position(double x, double y, int object)
{
  const enigma::object_collisions* r = collide_inst_point(object,false,true,x,y);
  return r == NULL ? noone : r->id;
}

int collision_rectangle(double x1, double y1, double x2, double y2, int obj, bool prec /*ignored*/, bool notme)
{
  const enigma::object_collisions* r = collide_inst_rect(obj,false,notme,x1,y1,x2,y2); //false is for solid_only, not prec
  return r == NULL ? noone : r->id;
}

int collision_line(double x1, double y1, double x2, double y2, int obj, bool prec /*ignored*/, bool notme)
{
  const enigma::object_collisions* r = collide_inst_line(obj,false,notme,x1,y1,x2,y2); //false is for solid_only, not prec
  return r == NULL ? noone : r->id;
}

int collision_point(double x, double y, int obj, bool prec /*ignored*/, bool notme)
{
  const enigma::object_collisions* r = collide_inst_point(obj,false,notme,x,y); //false is for solid_only, not prec
  return r == NULL ? noone : r->id;
}

