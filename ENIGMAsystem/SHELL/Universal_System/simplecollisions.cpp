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

#include <map>
#include <math.h>
#include <string>

#include "var4.h"
#include "reflexive_types.h"

#include "ENIGMA_GLOBALS.h"

#include "collisions_object.h"
#include "instance_system.h"

bool collision_bbox_rect(int object,double x1,double y1,double x2,double y2)
{
  for (enigma::inst_iter *it = enigma::fetch_inst_iter_by_int(object); it != NULL; it = it->next)
  {
    const enigma::object_collisions* inst = (enigma::object_collisions*)it->inst;
    const int ox = (int)inst->x,         oy = (int)inst->y,
              bl = inst->bbox_left, br = inst->bbox_right,
              bt = inst->bbox_top,  bb = inst->bbox_bottom;
    if (x1<ox+br && x2>ox+bl && y1<oy+bb && y2>oy+bt)
      return 1;
  }
  return 0;
}

int collision_bbox_rect_first(int object,double x1,double y1,double x2,double y2)
{
  for (enigma::inst_iter *it = enigma::fetch_inst_iter_by_int(object); it != NULL; it = it->next)
  {
    const enigma::object_collisions* inst = (enigma::object_collisions*)it->inst;
    const int ox = (int)inst->x,         oy = (int)inst->y,
              bl = inst->bbox_left, br = inst->bbox_right,
              bt = inst->bbox_top,  bb = inst->bbox_bottom;
    if (x1<ox+br && x2>ox+bl && y1<oy+bb && y2>oy+bt)
      return inst->id;
  }
  return noone;
}

