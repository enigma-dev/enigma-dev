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
//#include "reflexive_types.h"
//#include "EGMstd.h"
#include "object.h"

#include "instance_system.h"

namespace enigma
{
  int destroycalls = 0, createcalls = 0;
}

void instance_destroy(int id)
{
  enigma::inst_iter* who = enigma::fetch_inst_iter_by_id(id);
  if (who)
    who->inst->unlink();
}

int instance_destroy()
{
  enigma::instance_event_iterator->inst->unlink();
  return 0;
}

bool instance_exists(int obj)
{
  return enigma::fetch_instance_by_int(obj) != NULL;
}

int instance_find(double obj,double n)
{
  int objind=(int)obj;
  int num=(int)n;
  int nth=0;
  for (enigma::inst_iter *it = enigma::instance_list_first(); it != NULL; it = it->next)
  {
    if (it->inst->object_index==objind)
    {
      nth++;
      if (nth>num)
      return (int) it->inst->id;
    }
  }
  return -4;
}

int instance_number(int obj)
{
  return enigma::objects[obj].count;
}

//TODO: Move these to an instance_planar
#include "planar_object.h"

int instance_position(int x,int y,int obj)
{
    int objind=(int) obj;
    int xl,yl;
    int xc=(int)x,yc=(int)y;
    
    for (enigma::inst_iter *it = enigma::instance_list_first(); it != NULL; it = it->next)
    {
      if ((it->inst->object_index==objind) || objind==-3)
      {
        xl=(int)((enigma::object_planar*)it->inst)->x;
        yl=(int)((enigma::object_planar*)it->inst)->y;
        
        if (xl==xc && yl==yc)
        return (int)it->inst->id;
      }
    }
    return noone;
}

//TODO: replace all these fucking enigma::instance_iterators with enigma::institer_t i or something

int instance_nearest(int x,int y,int obj)
{
    double dist_lowest=-1;
    int retid=-4;
    int objind=(int) obj;
    double xl,yl;

    for (enigma::inst_iter *it = enigma::instance_list_first(); it != NULL; it = it->next)
    {
      if (it->inst->object_index==objind)
      {
         xl = ((enigma::object_planar*)it->inst)->x - x;
         yl = ((enigma::object_planar*)it->inst)->y - y;
         const double dstclc = hypot(xl,yl);
         if (dstclc < dist_lowest or dist_lowest == -1)
         {
            dist_lowest = dstclc;
            retid = it->inst->id;
         }
      }
    }

    return retid;
}

int instance_furthest(int x,int y,int obj)
{
  double dist_highest=0;
  bool found=0;
  int retid = noone;
  int objind=(int) obj;
  double xl,yl;
  double dstclc;

  for (enigma::inst_iter *it = enigma::instance_list_first(); it != NULL; it = it->next)
  {
    if ((it->inst->object_index==objind) || objind==-3)
    {
      xl=((enigma::object_planar*)it->inst)->x - x;
      yl=((enigma::object_planar*)it->inst)->y - y;
      dstclc = hypot(xl,yl);
      if (dstclc > dist_highest or !found)
      {
        dist_highest = dstclc;
        retid = it->inst->id;
        found = 1;
      }
    }
  }

  return retid;
}

