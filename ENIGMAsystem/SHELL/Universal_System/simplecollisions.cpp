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

#include "var_cr3.h"
#include "reflexive_types.h"

#include "ENIGMA_GLOBALS.h"
#include "EGMstd.h"

#include "collisions_object.h"

bool collision_bbox_rect(double object,double x1,double y1,double x2,double y2)
{
  int obj=(int)object;
  if (obj==-3)
  for (enigma::instance_iterator=enigma::instance_list.begin();
  enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++)
  {
    int ox=((enigma::object_collisions*)enigma::instance_iterator->second)->x;
    int oy=((enigma::object_collisions*)enigma::instance_iterator->second)->y;
    
    int bl=((enigma::object_collisions*)enigma::instance_iterator->second)->bbox_left;
    int br=((enigma::object_collisions*)enigma::instance_iterator->second)->bbox_right;
    int bt=((enigma::object_collisions*)enigma::instance_iterator->second)->bbox_top;
    int bb=((enigma::object_collisions*)enigma::instance_iterator->second)->bbox_bottom;
    if (x1<ox+br && x2>ox+bl && y1<oy+bb && y2>oy+bt)
    return 1;
  }
  else if (obj>=0 && obj<100000)
  for (enigma::instance_iterator=enigma::instance_list.begin();
  enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++)
  {
    if (((enigma::object_collisions*)enigma::instance_iterator->second)->object_index==obj)
    {
      int ox=((enigma::object_collisions*)enigma::instance_iterator->second)->x;
      int oy=((enigma::object_collisions*)enigma::instance_iterator->second)->y;
      
      int bl=((enigma::object_collisions*)enigma::instance_iterator->second)->bbox_left;
      int br=((enigma::object_collisions*)enigma::instance_iterator->second)->bbox_right;
      int bt=((enigma::object_collisions*)enigma::instance_iterator->second)->bbox_top;
      int bb=((enigma::object_collisions*)enigma::instance_iterator->second)->bbox_bottom;
      if (x1<ox+br && x2>ox+bl && y1<oy+bb && y2>oy+bt)
      return 1;
    }
  }
  else if (obj>=100000)
  {
    enigma::instance_iterator=enigma::instance_list.find(obj);
    if (enigma::instance_iterator != enigma::instance_list.end())
    {
      //if (((enigma::object_collisions*)enigma::instance_iterator->second)->id == obj)
      //{
        int ox=((enigma::object_collisions*)enigma::instance_iterator->second)->x;
        int oy=((enigma::object_collisions*)enigma::instance_iterator->second)->y;
        
        int bl=((enigma::object_collisions*)enigma::instance_iterator->second)->bbox_left;
        int br=((enigma::object_collisions*)enigma::instance_iterator->second)->bbox_right;
        int bt=((enigma::object_collisions*)enigma::instance_iterator->second)->bbox_top;
        int bb=((enigma::object_collisions*)enigma::instance_iterator->second)->bbox_bottom;
        if (x1<ox+br && x2>ox+bl && y1<oy+bb && y2>oy+bt)
          return 1;
      //}
    }
  }
  return 0;
}
