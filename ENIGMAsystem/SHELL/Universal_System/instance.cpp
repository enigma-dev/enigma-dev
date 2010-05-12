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
#include "EGMstd.h"
#include "object.h"

#include "key_game_globals.h"

namespace enigma
{
  int destroycalls = 0, createcalls = 0;
}

int instance_destroy(double id)
{
    enigma::cleanups[enigma::cleancount++]=(int)id;
    enigma::instancecount--;
    instance_count--;
    return 0;
}

int instance_destroy(){
    #if SHOWERRORS
    show_error("Nothing to destroy",0);
    #endif
    return -1;
}

bool instance_exists(double obj)
{
     int instance=(int) obj;
     if (instance>100000){
        return (enigma::instance_list.find(instance) != enigma::instance_list.end());
     }else if (instance>=0){
       for (enigma::instance_iterator=enigma::instance_list.begin(); enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++)
           if ((*enigma::instance_iterator).second->object_index==instance)
              return 1;
     }else{
         switch (instance){
            case -1:return (enigma::instance_list.find((int)((*enigma::instance_iterator).second->id)) != enigma::instance_list.end());
			case -3:return (enigma::instance_list.size()>0);
            default:return 0;
         }
     }
     return 0;
}

int instance_find(double obj,double n)
{
   int objind=(int)obj;
   int num=(int)n;
   int nth=0;
   for (enigma::instance_iterator=enigma::instance_list.begin(); enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++){
       if ((*enigma::instance_iterator).second->object_index==objind){
          nth++;
          if (nth>num)
          return (int) (*enigma::instance_iterator).second->id;
       }
   }
   return -4;
}
int instance_number(double obj)
{
   int objind=(int)obj;
   int n=0;
   for (enigma::instance_iterator=enigma::instance_list.begin(); enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++)
       if ((*enigma::instance_iterator).second->object_index==objind)
          n++;
   return n;
}

//TODO: Move these to an instance_planar
#include "planar_object.h"

int instance_position(double x,double y,double obj)
{
    int objind=(int) obj;
    int xl,yl;
    int xc=(int)x,yc=(int)y;
    
    for (enigma::instance_iterator=enigma::instance_list.begin(); enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++)
    {
      if (((*enigma::instance_iterator).second->object_index==objind) || objind==-3)
      {
        xl=(int)((enigma::object_planar*)enigma::instance_iterator->second)->x;
        yl=(int)((enigma::object_planar*)enigma::instance_iterator->second)->y;
        
        if (xl==xc && yl==yc)
        return (int)(*enigma::instance_iterator).second->id;
      }
    }
    return noone;
}

//TODO: replace all these fucking enigma::instance_iterators with enigma::institer_t i or something

int instance_nearest(int x,int y,int obj)
{
    double dist_lowest=-1,retid=-4;
    int objind=(int) obj;
    double xl,yl;

    for (enigma::instance_iterator=enigma::instance_list.begin(); enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++)
    {
      if ((*enigma::instance_iterator).second->object_index==objind)
      {
         xl = ((enigma::object_planar*)enigma::instance_iterator->second)->x;
         yl = ((enigma::object_planar*)enigma::instance_iterator->second)->y;
         const double dstclc = hypot(xl,yl);
         if (dstclc < dist_lowest or dist_lowest == -1)
         {
            dist_lowest = dstclc;
            retid = (*enigma::instance_iterator).second->id;
         }
      }
    }

    return retid;
}
int instance_furthest(double x,double y,double obj)
{
    double dist_highest=0,retid=0;
    bool found=0;
    int objind=(int) obj;
    int xl,yl;
    double dstclc;

    for (enigma::instance_iterator=enigma::instance_list.begin(); enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++)
    {
    if (((*enigma::instance_iterator).second->object_index==objind) || objind==-3)
    {
       xl=(int)((enigma::object_planar*)enigma::instance_iterator->second)->x;
       yl=(int)((enigma::object_planar*)enigma::instance_iterator->second)->y;
       dstclc=sqrt(xl*xl+yl*yl);
       if (dstclc>dist_highest || found==0)
       {
          dist_highest=dstclc;
          retid=(*enigma::instance_iterator).second->id;
          found=1;
       }
    }
    }

    return (int)retid;
}

