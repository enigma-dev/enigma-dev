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

#include "Universal_System/collisions_object.h"
#include "Universal_System/instance_system.h" //iter
#include "Universal_System/roomsystem.h"
#include "Collision_Systems/collision_mandatory.h" //iter
#include "coll_funcs.h"
#include "coll_impl.h"
#include <limits>
#include <cmath>
#include "Universal_System/instance.h"

static inline void get_border(int *leftv, int *rightv, int *topv, int *bottomv, int left, int top, int right, int bottom, double x, double y, double xscale, double yscale, double angle)
{
    if (angle == 0)
    {
        const bool xsp = (xscale >= 0), ysp = (yscale >= 0);
        const double lsc = left*xscale, rsc = (right+1)*xscale-1, tsc = top*yscale, bsc = (bottom+1)*yscale-1;

        *leftv   = (xsp ? lsc : rsc) + x + .5;
        *rightv  = (xsp ? rsc : lsc) + x + .5;
        *topv    = (ysp ? tsc : bsc) + y + .5;
        *bottomv = (ysp ? bsc : tsc) + y + .5;
    }
    else
    {
        const double arad = angle*(M_PI/180.0);
        const double sina = sin(arad), cosa = cos(arad);
        const double lsc = left*xscale, rsc = (right+1)*xscale-1, tsc = top*yscale, bsc = (bottom+1)*yscale-1;
        const int quad = int(fmod(fmod(angle, 360) + 360, 360)/90.0);
        const bool xsp = (xscale >= 0), ysp = (yscale >= 0),
                   q12 = (quad == 1 || quad == 2), q23 = (quad == 2 || quad == 3),
                   xs12 = xsp^q12, sx23 = xsp^q23, ys12 = ysp^q12, ys23 = ysp^q23;

        *leftv   = cosa*(xs12 ? lsc : rsc) + sina*(ys23 ? tsc : bsc) + x + .5;
        *rightv  = cosa*(xs12 ? rsc : lsc) + sina*(ys23 ? bsc : tsc) + x + .5;
        *topv    = cosa*(ys12 ? tsc : bsc) - sina*(sx23 ? rsc : lsc) + y + .5;
        *bottomv = cosa*(ys12 ? bsc : tsc) - sina*(sx23 ? lsc : rsc) + y + .5;
    }
}

static inline int min(int x, int y) { return x<y? x : y; }
static inline double min(double x, double y) { return x<y? x : y; }
static inline int max(int x, int y) { return x>y? x : y; }
static inline double max(double x, double y) { return x>y? x : y; }
static inline double direction_difference(double dir1, double dir2) {return fmod((fmod((dir1 - dir2),360) + 540), 360) - 180;}
static inline double point_direction(double x1,double y1,double x2,double y2) {return fmod((atan2(y1-y2,x2-x1)*(180/M_PI))+360,360);}

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

int instance_place(double x, double y, int object)
{
  enigma::object_collisions* const r = collide_inst_inst(object,false,true,x,y);
  return r == NULL ? noone : r->id;
}

namespace enigma {
  object_basic *place_meeting_inst(double x, double y, int object)
  {
    return collide_inst_inst(object,false,true,x,y);
  }
}

bool position_free(double x,double y)
{
  return collide_inst_point(all,true,true,true,x+.5,y+.5) == NULL;
}

bool position_empty(double x, double y)
{
  return collide_inst_point(all,false,true,true,x+.5,y+.5) == NULL;
}

bool position_meeting(double x, double y, int object)
{
  return collide_inst_point(object,false,true,true,x+.5,y+.5);
}

int instance_position(double x, double y, int object)
{
  const enigma::object_collisions* r = collide_inst_point(object,false,true,true,x+.5,y+.5);
  return r == NULL ? noone : r->id;
}

int collision_rectangle(double x1, double y1, double x2, double y2, int obj, bool prec, bool notme)
{
  const enigma::object_collisions* r = collide_inst_rect(obj,false,prec,notme,x1+.5,y1+.5,x2+.5,y2+.5); //false is for solid_only
  return r == NULL ? noone : r->id;
}

int collision_line(double x1, double y1, double x2, double y2, int obj, bool prec, bool notme)
{
  const enigma::object_collisions* r = collide_inst_line(obj,false,prec,notme,x1+.5,y1+.5,x2+.5,y2+.5);
  return r == NULL ? noone : r->id;
}

int collision_point(double x, double y, int obj, bool prec, bool notme)
{
  const enigma::object_collisions* r = collide_inst_point(obj,false, prec,notme,x+.5,y+.5);
  return r == NULL ? noone : r->id;
}

