/********************************************************************************\
**                                                                              **
**  Copyright (C) 2013 forthevin                                                **
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

#include "Object_Tiers/planar_object.h"
#include "instance_system.h"
#include "instance.h"
#include <cfloat>

namespace enigma_user
{

enigma::instance_t instance_nearest(int x,int y,int obj,bool notme)
{
  double dist_lowest = DBL_MAX;
  int retid = noone;
  double xl, yl;

  for (enigma::iterator it = enigma::fetch_inst_iter_by_int(obj); it; ++it)
  {
    if (notme && (*it)->id == enigma::instance_event_iterator->inst->id) continue;
    xl = ((enigma::object_planar*)*it)->x - x;
    yl = ((enigma::object_planar*)*it)->y - y;
    const double dstclc = xl * xl + yl * yl;
    if (dstclc < dist_lowest) {
      dist_lowest = dstclc;
      retid = it->id;
    }
  }

  return retid;
}

enigma::instance_t instance_furthest(int x,int y,int obj,bool notme)
{
  double dist_highest = -1;
  int retid = noone;
  double xl,yl;
  double dstclc;

  for (enigma::iterator it = enigma::fetch_inst_iter_by_int(obj); it; ++it)
  {
    if (notme && (*it)->id == enigma::instance_event_iterator->inst->id) continue;
    xl=((enigma::object_planar*)*it)->x - x;
    yl=((enigma::object_planar*)*it)->y - y;
    dstclc = xl * xl + yl * yl;
    if (dstclc > dist_highest)
    {
      dist_highest = dstclc;
      retid = it->id;
    }
  }

  return retid;
}

}
