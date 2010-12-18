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
// Collision implementation functions - API dependant implementations of instances colliding with other things.
// In this case, we treat instances as their bounding box (BBox).
////////////////////////////////////

#include "../../Universal_System/collisions_object.h"
#include "../../Universal_System/instance_system.h" //iter
#include "coll_util.h"

const enigma::object_collisions* collide_inst_line(int object, bool solid_only, bool notme, double x1, double y1, double x2, double y2)
{
  for (enigma::inst_iter *it = enigma::fetch_inst_iter_by_int(object); it != NULL; it = it->next)
  {
    const enigma::object_collisions* inst = (enigma::object_collisions*)it->inst;
    if (notme && inst->id == enigma::instance_event_iterator->inst->id) continue;
    if (solid_only && !inst->solid) continue;
    if (collide_bbox_line(inst,inst->x,inst->y, x1,y1,x2,y2))
      return inst;
  }
  return NULL;
}

const enigma::object_collisions* collide_inst_inst(int object, bool solid_only, bool notme, double x, double y)
{
  enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
  for (enigma::inst_iter *it = enigma::fetch_inst_iter_by_int(object); it != NULL; it = it->next)
  {
    const enigma::object_collisions* inst2 = (enigma::object_collisions*)it->inst;
    if (notme && inst2->id == inst1->id) continue;
    if (solid_only && !inst2->solid) continue;
    if (collide_bbox_bbox(inst1,x,y,inst2,inst2->x,inst2->y))
      return inst2;
  }
  return NULL;
}

const enigma::object_collisions* collide_inst_point(int object, bool solid_only, bool notme, double x, double y)
{
  for (enigma::inst_iter *it = enigma::fetch_inst_iter_by_int(object); it != NULL; it = it->next)
  {
    const enigma::object_collisions* inst = (enigma::object_collisions*)it->inst;
    if (notme && inst->id == enigma::instance_event_iterator->inst->id) continue;
    if (solid_only && !inst->solid) continue;
    if (collide_bbox_point(inst,inst->x,inst->y,x,y))
      return inst;
  }
  return NULL;

}

