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

        *leftv   = (xsp ? left : -right - 2) + x + .5;
        *rightv  = (xsp ? right : -left) + x + .5;
        *topv    = (ysp ? top : -bottom - 2) + y + .5;
        *bottomv = (ysp ? bottom : -top) + y + .5;
    }
    else
    {
        const double arad = angle*(M_PI/180.0);
        const double sina = sin(arad), cosa = cos(arad);
        const int quad = int(fmod(fmod(angle, 360) + 360, 360)/90.0);
        const bool xsp = (xscale >= 0), ysp = (yscale >= 0),
                   q12 = (quad == 1 || quad == 2), q23 = (quad == 2 || quad == 3),
                   xs12 = xsp^q12, sx23 = xsp^q23, ys12 = ysp^q12, ys23 = ysp^q23;

        *leftv   = sina*(xs12 ? left : -right - 2) + cosa*(ys23 ? top : -bottom - 2) + x + .5;
        *rightv  = sina*(xs12 ? right : left) + cosa*(ys23 ? bottom : top) + x + .5;
        *topv    = cosa*(ys12 ? top : -bottom - 2) - sina*(sx23 ? right : left) + y + .5;
        *bottomv = cosa*(ys12 ? bottom : top) - sina*(sx23 ? left : -right - 2) + y + .5;
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
  return collide_inst_point(all,true,true,x+.5,y+.5) == NULL;
}

bool position_empty(double x, double y)
{
  return collide_inst_point(all,false,true,x+.5,y+.5) == NULL;
}

bool position_meeting(double x, double y, int object)
{
  return collide_inst_point(object,false,true,x+.5,y+.5);
}

void position_destroy_object(double x, double y, int object, bool solid_only)
{
    destroy_inst_point(object,solid_only,x+.5,y+.5);
}

void position_destroy_solid(double x, double y)
{
    destroy_inst_point(all,true,x+.5,y+.5);
}

void position_destroy(double x, double y)
{
    destroy_inst_point(all,false,x+.5,y+.5);
}

int instance_position(double x, double y, int object)
{
  const enigma::object_collisions* r = collide_inst_point(object,false,true,x+.5,y+.5);
  return r == NULL ? noone : r->id;
}

int collision_rectangle(double x1, double y1, double x2, double y2, int obj, bool prec /*ignored*/, bool notme)
{
  const enigma::object_collisions* r = collide_inst_rect(obj,false,notme,x1+.5,y1+.5,x2+.5,y2+.5); //false is for solid_only, not prec
  return r == NULL ? noone : r->id;
}

int collision_line(double x1, double y1, double x2, double y2, int obj, bool prec /*ignored*/, bool notme)
{
  const enigma::object_collisions* r = collide_inst_line(obj,false,notme,x1+.5,y1+.5,x2+.5,y2+.5); //false is for solid_only, not prec
  return r == NULL ? noone : r->id;
}

int collision_point(double x, double y, int obj, bool prec /*ignored*/, bool notme)
{
  const enigma::object_collisions* r = collide_inst_point(obj,false,notme,x+.5,y+.5); //false is for solid_only, not prec
  return r == NULL ? noone : r->id;
}

int collision_circle(double x, double y, double radius, int obj, bool prec /*ignored*/, bool notme)
{
  const enigma::object_collisions* r = collide_inst_circle(obj,false,notme,x+.5,y+.5,radius); //false is for solid_only, not prec
  return r == NULL ? noone : r->id;
}

int collision_ellipse(double x1, double y1, double x2, double y2, int obj, bool prec /*ignored*/, bool notme)
{
  const enigma::object_collisions* r = collide_inst_ellipse(obj,false,notme,((x1+x2)/2)+.5,((y1+y2)/2)+.5,fabs(x2-x1)/2,fabs(y2-y1)/2); //false is for solid_only, not prec
  return r == NULL ? noone : r->id;
}

double distance_to_object(int object)
{
    double distance = std::numeric_limits<double>::infinity();
    double tempdist;
    const enigma::object_collisions* inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    const bbox_rect_t &box = inst1->$bbox_relative();
    const double x1 = inst1->x, y1 = inst1->y,
                 xscale1 = inst1->image_xscale, yscale1 = inst1->image_yscale,
                 ia1 = inst1->image_angle;
    int left1, top1, right1, bottom1;

    get_border(&left1, &right1, &top1, &bottom1, box.left, box.top, box.right, box.bottom, x1, y1, xscale1, yscale1, ia1);

    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        const enigma::object_collisions* inst2 = (enigma::object_collisions*)*it;
        if (inst1 == inst2) continue;

        const bbox_rect_t &box2 = inst2->$bbox_relative();
        const double x2 = inst2->x, y2 = inst2->y,
                     xscale2 = inst2->image_xscale, yscale2 = inst2->image_yscale,
                     ia2 = inst2->image_angle;
        int left2, top2, right2, bottom2;

        get_border(&left2, &right2, &top2, &bottom2, box2.left, box2.top, box2.right, box2.bottom, x2, y2, xscale2, yscale2, ia2);

        const int right  = min(right1, right2),   left = max(left1, left2),
                  bottom = min(bottom1, bottom2), top  = max(top1, top2);

        tempdist = hypot((left > right ? left - right : 0),
                         (top > bottom ? top - bottom : 0));

        if (tempdist < distance)
        {
            distance = tempdist;
        }
    }
    return (distance == std::numeric_limits<double>::infinity() ? -1 : distance);
}

double distance_to_point(double x, double y)
{
    enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    const bbox_rect_t &box = inst1->$bbox_relative();
    const double x1 = inst1->x, y1 = inst1->y,
                 xscale1 = inst1->image_xscale, yscale1 = inst1->image_yscale,
                 ia1 = inst1->image_angle;
    int left1, top1, right1, bottom1;

    get_border(&left1, &right1, &top1, &bottom1, box.left, box.top, box.right, box.bottom, x1, y1, xscale1, yscale1, ia1);

    return fabs(hypot(min(x1 + left1 - x, x1 + right1 - x),
                    min(y1 + top1 - y, y1 + bottom1 - y)));
}

double move_contact_object(int object, double angle, double max_dist, bool solid_only)
{
    const double DMIN = 1, DMAX = 1000000;
    const double contact_distance = DMIN;
    double sin_angle, cos_angle;
    if (max_dist <= 0)
    {
        max_dist = DMAX;
    }
    angle = fmod(fmod(angle, 360) + 360, 360);
    if (angle == 90)
    {
        sin_angle = 1; cos_angle = 0;
    }
    else if (angle == 180)
    {
        sin_angle = 0; cos_angle = -1;
    }
    else if (angle == 270)
    {
        sin_angle = -1; cos_angle = 0;
    }
    else
    {
        const double radang = angle*(M_PI/180.0);
        sin_angle = sin(radang), cos_angle = cos(radang);
    }
    const int quad = int(angle/90.0);
    enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    const bbox_rect_t &box = inst1->$bbox_relative();
    const double x1 = inst1->x, y1 = inst1->y,
                 xscale1 = inst1->image_xscale, yscale1 = inst1->image_yscale,
                 ia1 = inst1->image_angle;
    int left1, top1, right1, bottom1;

    get_border(&left1, &right1, &top1, &bottom1, box.left, box.top, box.right, box.bottom, x1, y1, xscale1, yscale1, ia1);

    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        const enigma::object_collisions* inst2 = (enigma::object_collisions*)*it;
        if (inst2->id == inst1->id || (solid_only && !inst2->solid))
            continue;
        const bbox_rect_t &box2 = inst2->$bbox_relative();
        const double x2 = inst2->x, y2 = inst2->y,
                     xscale2 = inst2->image_xscale, yscale2 = inst2->image_yscale,
                     ia1 = inst2->image_angle;
        int left2, top2, right2, bottom2;

        get_border(&left2, &right2, &top2, &bottom2, box2.left, box2.top, box2.right, box2.bottom, x2, y2, xscale2, yscale2, ia1);

        if (right2 >= left1 && bottom2 >= top1 && left2 <= right1 && top2 <= bottom1)
        {
            return 0;
        }

        switch (quad)
        {
            case 0:
                if ((left2 > right1 || top1 > bottom2) &&
                direction_difference(point_direction(right1, bottom1, left2, top2),angle) >= 0  &&
                direction_difference(point_direction(left1, top1, right2, bottom2),angle) <= 0)
                {
                    if (direction_difference(point_direction(right1, top1, left2, bottom2),angle) > 0)
                    {
                        max_dist = min(max_dist, (top1 - bottom2 - contact_distance)/sin_angle);
                    }
                    else
                    {
                        max_dist = min(max_dist, (left2 - right1 - contact_distance)/cos_angle);
                    }
                }
            break;
            case 1:
                if ((left1 > right2 || top1 > bottom2) &&
                direction_difference(point_direction(left1, bottom1, right2, top2),angle) <= 0  &&
                direction_difference(point_direction(right1, top1, left2, bottom2),angle) >= 0)
                {
                    if (direction_difference(point_direction(left1, top1, right2, bottom2),angle) > 0)
                    {
                        max_dist = min(max_dist, (right2 - left1 + contact_distance)/cos_angle);
                    }
                    else
                    {
                        max_dist = min(max_dist, (top1 - bottom2 - contact_distance)/sin_angle);
                    }
                }
            break;
            case 2:
                if ((left1 > right2 || top2 > bottom1) &&
                direction_difference(point_direction(right1, bottom1, left2, top2),angle) <= 0  &&
                direction_difference(point_direction(left1, top1, right2, bottom2),angle) >= 0)
                {
                    if (direction_difference(point_direction(left1, bottom1, right2, top2),angle) > 0)
                    {
                        max_dist = min(max_dist, (bottom1 - top2 + contact_distance)/sin_angle);
                    }
                    else
                    {
                        max_dist = min(max_dist, (right2 - left1 + contact_distance)/cos_angle);
                    }
                }
            break;
            case 3:
                if ((left2 > right1 || top2 > bottom1) &&
                direction_difference(point_direction(right1, top1, left2, bottom2),angle) <= 0  &&
                direction_difference(point_direction(left1, bottom1, right2, top2),angle) >= 0)
                {
                    if (direction_difference(point_direction(right1, bottom1, left2, top2),angle) > 0)
                    {
                        max_dist = min(max_dist, (left2 - right1 - contact_distance)/cos_angle);
                    }
                    else
                    {
                        max_dist = min(max_dist, (bottom1 - top2 + contact_distance)/sin_angle);
                    }
                }
            break;
        }
    }
    inst1->x += cos_angle*max_dist;
    inst1->y -= sin_angle*max_dist;
    return max_dist;
}

double move_outside_object(int object, double angle, double max_dist, bool solid_only)
{
    const double DMIN = 0.000001, DMAX = 1000000;
    const double contact_distance = DMIN;
    if (max_dist <= 0)
    {
        max_dist = DMAX;
    }
    double dist = 0;
    angle = ((angle %(variant) 360) + 360) %(variant) 360;
    double radang = angle*(M_PI/180.0);
    const double sin_angle = sin(radang), cos_angle = cos(radang);
    const int quad = int(angle/90.0);
    enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    const bbox_rect_t &box = inst1->$bbox_relative();
    const double x1 = inst1->x, y1 = inst1->y,
                 xscale1 = inst1->image_xscale, yscale1 = inst1->image_yscale,
                 ia1 = inst1->image_angle;
    int left1, top1, right1, bottom1;

    get_border(&left1, &right1, &top1, &bottom1, box.left, box.top, box.right, box.bottom, x1, y1, xscale1, yscale1, ia1);

    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        const enigma::object_collisions* inst2 = (enigma::object_collisions*)*it;
        if (inst2->id == inst1->id || (solid_only && !inst2->solid))
            continue;
        const bbox_rect_t &box2 = inst2->$bbox_relative();
        const double x2 = inst2->x, y2 = inst2->y,
                     xscale2 = inst2->image_xscale, yscale2 = inst2->image_yscale,
                     ia1 = inst2->image_angle;
        int left2, top2, right2, bottom2;

        get_border(&left2, &right2, &top2, &bottom2, box2.left, box2.top, box2.right, box2.bottom, x2, y2, xscale2, yscale2, ia1);

        if (!(right2 >= left1 && bottom2 >= top1 && left2 <= right1 && top2 <= bottom1))
            continue;

        switch (quad)
        {
            case 0:
                if (direction_difference(point_direction(right1, top1, right2, top2),angle) < 0)
                {
                    dist = max(dist, ((bottom1) - (top2) + contact_distance)/sin_angle);
                }
                else
                {
                    dist = max(dist, ((right2) - (left1) + contact_distance)/cos_angle);
                }
            break;
            case 1:
                if (direction_difference(point_direction(left1, top1, left2, top2),angle) < 0)
                {
                    dist = max(dist, ((left2) - (right1) - contact_distance)/cos_angle);
                }
                else
                {
                    dist = max(dist, ((bottom1) - (top2) + contact_distance)/sin_angle);
                }
            break;
            case 2:
                if (direction_difference(point_direction(left1, bottom1, left2, bottom2),angle) < 0)
                {
                    dist = max(dist, ((top1) - (bottom2) - contact_distance)/sin_angle);
                }
                else
                {
                    dist = max(dist, ((left2) - (right1) - contact_distance)/cos_angle);
                }
            break;
            case 3:
                if (direction_difference(point_direction(right1, bottom1, right2, bottom2),angle) < 0)
                {
                    dist = max(dist, ((right2) - (left1) + contact_distance)/cos_angle);
                }
                else
                {
                    dist = max(dist, ((top1) - (bottom2) - contact_distance)/sin_angle);
                }
            break;
        }
    }
    dist = min(dist, max_dist);
    inst1->x += cos_angle*dist;
    inst1->y -= sin_angle*dist;
    return dist;
}

bool move_bounce_object_inside(int object, bool adv, bool solid_only)
{
    enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    const double angle = inst1->direction, radang = angle*(M_PI/180.0), DBL_EPSILON = 0.00001;
    double sin_angle = sin(radang), cos_angle = cos(radang), pc_corner, pc_dist, max_dist = 1000000;
    int side_type = 0;
    const int quad = int(2*radang/M_PI);
    const bbox_rect_t &box = inst1->$bbox_relative();
    const double x1 = inst1->x - inst1->hspeed, y1 = inst1->y - inst1->vspeed,
                 xscale1 = inst1->image_xscale, yscale1 = inst1->image_yscale,
                 ia1 = inst1->image_angle;
    int left1, top1, right1, bottom1;

    get_border(&left1, &right1, &top1, &bottom1, box.left, box.top, box.right, box.bottom, x1, y1, xscale1, yscale1, ia1);

    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        const enigma::object_collisions* inst2 = (enigma::object_collisions*)*it;
        if (inst2->id == inst1->id || (solid_only && !inst2->solid))
            continue;
        const bbox_rect_t &box2 = inst2->$bbox_relative();
        const double x2 = inst2->x, y2 = inst2->y,
                     xscale2 = inst2->image_xscale, yscale2 = inst2->image_yscale,
                     ia1 = inst2->image_angle;
        int left2, top2, right2, bottom2;

        get_border(&left2, &right2, &top2, &bottom2, box2.left, box2.top, box2.right, box2.bottom, x2, y2, xscale2, yscale2, ia1);

        if (right2 >= left1 && bottom2 >= top1 && left2 <= right1 && top2 <= bottom1)
        {
            return false;
        }

        switch (quad)
        {
            case 0:
                if ((left2 > right1 || top1 > bottom2) &&
                direction_difference(point_direction(right1, bottom1, left2, top2),angle) >= 0  &&
                direction_difference(point_direction(left1, top1, right2, bottom2),angle) <= 0)
                {
                    pc_corner = direction_difference(point_direction(right1, top1, left2, bottom2),angle);
                    if (fabs(pc_corner) < DBL_EPSILON)
                    {
                        pc_dist = (left2 - right1)/cos_angle;
                        if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 4;
                        }
                    }
                    else if (pc_corner > 0)
                    {
                        pc_dist = (top1 - bottom2)/sin_angle;
                        if (fabs(pc_dist - max_dist) < DBL_EPSILON)
                        {
                            if (side_type == 2)
                                side_type = 3;
                            else if (side_type != 3)
                                side_type = 1;
                        }
                        else if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 1;
                        }
                    }
                    else
                    {
                        pc_dist = (left2 - right1)/cos_angle;
                        if (fabs(pc_dist - max_dist) < DBL_EPSILON)
                        {
                            if (side_type == 1)
                                side_type = 3;
                            else if (side_type != 3)
                                side_type = 2;
                        }
                        else if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 2;
                        }
                    }
                }
            break;
            case 1:
                if ((left1 > right2 || top1 > bottom2) &&
                direction_difference(point_direction(left1, bottom1, right2, top2),angle) <= 0  &&
                direction_difference(point_direction(right1, top1, left2, bottom2),angle) >= 0)
                {
                    pc_corner = direction_difference(point_direction(left1, top1, right2, bottom2),angle);

                    if (fabs(pc_corner) < DBL_EPSILON)
                    {
                        pc_dist = (left2 - right1)/cos_angle;
                        if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 4;
                        }
                    }
                    else if (pc_corner > 0)
                    {
                        pc_dist = (right2 - left1)/cos_angle;
                        if (fabs(pc_dist - max_dist) < DBL_EPSILON)
                        {
                            if (side_type == 1)
                                side_type = 3;
                            else if (side_type != 3)
                                side_type = 2;
                        }
                        else if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 2;
                        }
                    }
                    else
                    {
                        pc_dist = (top1 - bottom2)/sin_angle;
                        if (fabs(pc_dist - max_dist) < DBL_EPSILON)
                        {
                            if (side_type == 2)
                                side_type = 3;
                            else if (side_type != 3)
                                side_type = 1;
                        }
                        else if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 1;
                        }
                    }
                }
            break;
            case 2:
                if ((left1 > right2 || top2 > bottom1) &&
                direction_difference(point_direction(right1, bottom1, left2, top2),angle) <= 0  &&
                direction_difference(point_direction(left1, top1, right2, bottom2),angle) >= 0)
                {
                    pc_corner = direction_difference(point_direction(left1, bottom1, right2, top2),angle);
                    if (fabs(pc_corner) < DBL_EPSILON)
                    {
                        pc_dist = (right2 - left1)/cos_angle;
                        if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 4;
                        }
                    }
                    else if (pc_corner > 0)
                    {
                        pc_dist = (bottom1 - top2)/sin_angle;
                        if (fabs(pc_dist - max_dist) < DBL_EPSILON)
                        {
                            if (side_type == 2)
                                side_type = 3;
                            else if (side_type != 3)
                                side_type = 1;
                        }
                        else if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 1;
                        }
                    }
                    else
                    {
                        pc_dist = (right2 - left1)/cos_angle;
                        if (fabs(pc_dist - max_dist) < DBL_EPSILON)
                        {
                            if (side_type == 1)
                                side_type = 3;
                            else if (side_type != 3)
                                side_type = 2;
                        }
                        else if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 2;
                        }
                    }
                }
            break;
            case 3:
                if ((left2 > right1 || top2 > bottom1) &&
                direction_difference(point_direction(right1, top1, left2, bottom2),angle) <= 0  &&
                direction_difference(point_direction(left1, bottom1, right2, top2),angle) >= 0)
                {
                    pc_corner = direction_difference(point_direction(right1, bottom1, left2, top2),angle);
                    if (fabs(pc_corner) < DBL_EPSILON)
                    {
                        pc_dist = (bottom1 - top2)/sin_angle;
                        if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 4;
                        }
                    }
                    else if (pc_corner > 0)
                    {
                        pc_dist = (left2 - right1)/cos_angle;
                        if (fabs(pc_dist - max_dist) < DBL_EPSILON)
                        {
                            if (side_type == 1)
                                side_type = 3;
                            else if (side_type != 3)
                                side_type = 2;
                        }
                        else if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 2;
                        }
                    }
                    else
                    {
                        pc_dist = (bottom1 - top2)/sin_angle;
                        if (fabs(pc_dist - max_dist) < DBL_EPSILON)
                        {
                            if (side_type == 2)
                                side_type = 3;
                            else if (side_type != 3)
                                side_type = 1;
                        }
                        else if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 1;
                        }
                    }
                }
            break;
        }
    }

    switch (side_type)
    {
        case 0:  //no side hit
            return false;
        case 1:  //horizontal side hit
            inst1->vspeed *= -1;
        break;
        case 2:  //vertical side hit
            inst1->hspeed *= -1;
        break;
        case 3: case 4:  //corner or both horizontal and vertical side hit
            inst1->hspeed *= -1;
            inst1->vspeed *= -1;
        break;
    }
    return true;
}

bool move_bounce_object_outside(int object, bool adv, bool solid_only)
{
    enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    const double angle = inst1->direction, radang = angle*(M_PI/180.0), DBL_EPSILON = 0.00001;
    double sin_angle = sin(radang), cos_angle = cos(radang), pc_corner, pc_dist, max_dist = 1000000;
    int side_type = 0;
    const int quad = int(2*radang/M_PI);
    const bbox_rect_t &box = inst1->$bbox_relative();
    const double x1 = inst1->x, y1 = inst1->y,
                 xscale1 = inst1->image_xscale, yscale1 = inst1->image_yscale,
                 ia1 = inst1->image_angle;
    int left1, top1, right1, bottom1;

    get_border(&left1, &right1, &top1, &bottom1, box.left, box.top, box.right, box.bottom, x1, y1, xscale1, yscale1, ia1);

    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        const enigma::object_collisions* inst2 = (enigma::object_collisions*)*it;
        if (inst2->id == inst1->id || (solid_only && !inst2->solid))
            continue;
        const bbox_rect_t &box2 = inst2->$bbox_relative();
        const double x2 = inst2->x, y2 = inst2->y,
                     xscale2 = inst2->image_xscale, yscale2 = inst2->image_yscale,
                     ia1 = inst2->image_angle;
        int left2, top2, right2, bottom2;

        get_border(&left2, &right2, &top2, &bottom2, box2.left, box2.top, box2.right, box2.bottom, x2, y2, xscale2, yscale2, ia1);

        if (right2 >= left1 && bottom2 >= top1 && left2 <= right1 && top2 <= bottom1)
        {
            return false;
        }

        switch (quad)
        {
            case 0:
                if ((left2 > right1 || top1 > bottom2) &&
                direction_difference(point_direction(right1, bottom1, left2, top2),angle) >= 0  &&
                direction_difference(point_direction(left1, top1, right2, bottom2),angle) <= 0)
                {
                    pc_corner = direction_difference(point_direction(right1, top1, left2, bottom2),angle);
                    if (fabs(pc_corner) < DBL_EPSILON)
                    {
                        pc_dist = (left2 - right1)/cos_angle;
                        if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 4;
                        }
                    }
                    else if (pc_corner > 0)
                    {
                        pc_dist = (top1 - bottom2)/sin_angle;
                        if (fabs(pc_dist - max_dist) < DBL_EPSILON)
                        {
                            if (side_type == 2)
                                side_type = 3;
                            else if (side_type != 3)
                                side_type = 1;
                        }
                        else if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 1;
                        }
                    }
                    else
                    {
                        pc_dist = (left2 - right1)/cos_angle;
                        if (fabs(pc_dist - max_dist) < DBL_EPSILON)
                        {
                            if (side_type == 1)
                                side_type = 3;
                            else if (side_type != 3)
                                side_type = 2;
                        }
                        else if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 2;
                        }
                    }
                }
            break;
            case 1:
                if ((left1 > right2 || top1 > bottom2) &&
                direction_difference(point_direction(left1, bottom1, right2, top2),angle) <= 0  &&
                direction_difference(point_direction(right1, top1, left2, bottom2),angle) >= 0)
                {
                    pc_corner = direction_difference(point_direction(left1, top1, right2, bottom2),angle);

                    if (fabs(pc_corner) < DBL_EPSILON)
                    {
                        pc_dist = (left2 - right1)/cos_angle;
                        if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 4;
                        }
                    }
                    else if (pc_corner > 0)
                    {
                        pc_dist = (right2 - left1)/cos_angle;
                        if (fabs(pc_dist - max_dist) < DBL_EPSILON)
                        {
                            if (side_type == 1)
                                side_type = 3;
                            else if (side_type != 3)
                                side_type = 2;
                        }
                        else if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 2;
                        }
                    }
                    else
                    {
                        pc_dist = (top1 - bottom2)/sin_angle;
                        if (fabs(pc_dist - max_dist) < DBL_EPSILON)
                        {
                            if (side_type == 2)
                                side_type = 3;
                            else if (side_type != 3)
                                side_type = 1;
                        }
                        else if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 1;
                        }
                    }
                }
            break;
            case 2:
                if ((left1 > right2 || top2 > bottom1) &&
                direction_difference(point_direction(right1, bottom1, left2, top2),angle) <= 0  &&
                direction_difference(point_direction(left1, top1, right2, bottom2),angle) >= 0)
                {
                    pc_corner = direction_difference(point_direction(left1, bottom1, right2, top2),angle);
                    if (fabs(pc_corner) < DBL_EPSILON)
                    {
                        pc_dist = (right2 - left1)/cos_angle;
                        if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 4;
                        }
                    }
                    else if (pc_corner > 0)
                    {
                        pc_dist = (bottom1 - top2)/sin_angle;
                        if (fabs(pc_dist - max_dist) < DBL_EPSILON)
                        {
                            if (side_type == 2)
                                side_type = 3;
                            else if (side_type != 3)
                                side_type = 1;
                        }
                        else if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 1;
                        }
                    }
                    else
                    {
                        pc_dist = (right2 - left1)/cos_angle;
                        if (fabs(pc_dist - max_dist) < DBL_EPSILON)
                        {
                            if (side_type == 1)
                                side_type = 3;
                            else if (side_type != 3)
                                side_type = 2;
                        }
                        else if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 2;
                        }
                    }
                }
            break;
            case 3:
                if ((left2 > right1 || top2 > bottom1) &&
                direction_difference(point_direction(right1, top1, left2, bottom2),angle) <= 0  &&
                direction_difference(point_direction(left1, bottom1, right2, top2),angle) >= 0)
                {
                    pc_corner = direction_difference(point_direction(right1, bottom1, left2, top2),angle);
                    if (fabs(pc_corner) < DBL_EPSILON)
                    {
                        pc_dist = (bottom1 - top2)/sin_angle;
                        if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 4;
                        }
                    }
                    else if (pc_corner > 0)
                    {
                        pc_dist = (left2 - right1)/cos_angle;
                        if (fabs(pc_dist - max_dist) < DBL_EPSILON)
                        {
                            if (side_type == 1)
                                side_type = 3;
                            else if (side_type != 3)
                                side_type = 2;
                        }
                        else if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 2;
                        }
                    }
                    else
                    {
                        pc_dist = (bottom1 - top2)/sin_angle;
                        if (fabs(pc_dist - max_dist) < DBL_EPSILON)
                        {
                            if (side_type == 2)
                                side_type = 3;
                            else if (side_type != 3)
                                side_type = 1;
                        }
                        else if (pc_dist < max_dist)
                        {
                            max_dist = pc_dist;
                            side_type = 1;
                        }
                    }
                }
            break;
        }
    }

    switch (side_type)
    {
        case 0:  //no side hit
            return false;
        case 1:  //horizontal side hit
            inst1->vspeed *= -1;
            inst1->y += inst1->vspeed;
        break;
        case 2:  //vertical side hit
            inst1->hspeed *= -1;
            inst1->x += inst1->hspeed;
        break;
        case 3: case 4:  //corner or both horizontal and vertical side hit
            inst1->hspeed *= -1;
            inst1->vspeed *= -1;
            inst1->x += inst1->hspeed;
            inst1->y += inst1->vspeed;
        break;
    }
    return true;
}

bool move_bounce_object(int object, bool adv, bool solid_only)
{
    if (move_bounce_object_inside(object, adv, solid_only))
        return true;
    return (move_bounce_object_outside(object, adv, solid_only));
}

typedef std::pair<int,enigma::inst_iter*> inode_pair;

void instance_deactivate_region(int rleft, int rtop, int rwidth, int rheight, int inside, bool notme) {
    for (enigma::iterator it = enigma::instance_list_first(); it; ++it) {
        if (notme && (*it)->id == enigma::instance_event_iterator->inst->id) continue;
        enigma::object_collisions* const inst = ((enigma::object_collisions*)*it);

        if (inst->sprite_index == -1 && (inst->mask_index == -1)) //no sprite/mask then no collision
            continue;

        const bbox_rect_t &box = inst->$bbox_relative();
        const double x = inst->x, y = inst->y,
        xscale = inst->image_xscale, yscale = inst->image_yscale,
        ia = inst->image_angle;

        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x, y, xscale, yscale, ia);

        if (left <= (rleft+rwidth) && rleft <= right && top <= (rtop+rheight) && rtop <= bottom) {
            if (inside) {
            inst->deactivate();
            enigma::instance_deactivated_list.insert(inode_pair((*it)->id,it.it));
            }
        } else {
            if (!inside) {
                inst->deactivate();
                enigma::instance_deactivated_list.insert(inode_pair((*it)->id,it.it));
            }
        }
    }
}

void instance_activate_region(int rleft, int rtop, int rwidth, int rheight, int inside) {
    std::map<int,enigma::inst_iter*>::iterator iter;
    for (iter = enigma::instance_deactivated_list.begin(); iter != enigma::instance_deactivated_list.end(); ++iter) {

        enigma::object_collisions* const inst = ((enigma::object_collisions*)(iter->second->inst));

        if (inst->sprite_index == -1 && (inst->mask_index == -1)) //no sprite/mask then no collision
            continue;

        const bbox_rect_t &box = inst->$bbox_relative();
        const double x = inst->x, y = inst->y,
        xscale = inst->image_xscale, yscale = inst->image_yscale,
        ia = inst->image_angle;

        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x, y, xscale, yscale, ia);

        if (left <= (rleft+rwidth) && rleft <= right && top <= (rtop+rheight) && rtop <= bottom) {
            if (inside) {
                inst->activate();
                enigma::instance_deactivated_list.erase(iter);
            }
        } else {
            if (!inside) {
                inst->activate();
                enigma::instance_deactivated_list.erase(iter);
            }
        }
    }
}

void instance_deactivate_circle(int x, int y, int r, int inside, bool notme)
{
    for (enigma::iterator it = enigma::instance_list_first(); it; ++it)
    {
        if (notme && (*it)->id == enigma::instance_event_iterator->inst->id)
            continue;
        enigma::object_collisions* const inst = ((enigma::object_collisions*)*it);

        if (inst->sprite_index == -1 && (inst->mask_index == -1)) //no sprite/mask then no collision
            continue;

        const bbox_rect_t &box = inst->$bbox_relative();
        const double x1 = inst->x, y1 = inst->y,
        xscale = inst->image_xscale, yscale = inst->image_yscale,
        ia = inst->image_angle;

        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x1, y1, xscale, yscale, ia);

        if (fabs(hypot(min(x1 + left - x, x1 + right - x), min(y1 + top - y, y1 + bottom - y))) <= r)
        {
            if (inside)
            {
                inst->deactivate();
                enigma::instance_deactivated_list.insert(inode_pair((*it)->id,it.it));
            }
        }
        else
        {
            if (!inside)
            {
                inst->deactivate();
                enigma::instance_deactivated_list.insert(inode_pair((*it)->id,it.it));
            }
        }
    }
}

void instance_activate_circle(int x, int y, int r, int inside)
{
    std::map<int,enigma::inst_iter*>::iterator iter;
    for (iter = enigma::instance_deactivated_list.begin(); iter != enigma::instance_deactivated_list.end(); ++iter)
    {
        enigma::object_collisions* const inst = ((enigma::object_collisions*)(iter->second->inst));

        if (inst->sprite_index == -1 && (inst->mask_index == -1)) //no sprite/mask then no collision
            continue;

        const bbox_rect_t &box = inst->$bbox_relative();
        const double x1 = inst->x, y1 = inst->y,
        xscale = inst->image_xscale, yscale = inst->image_yscale,
        ia = inst->image_angle;

        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x1, y1, xscale, yscale, ia);

        if (fabs(hypot(min(x1 + left - x, x1 + right - x), min(y1 + top - y, y1 + bottom - y))) <= r)
        {
            if (inside)
            {
                inst->activate();
                enigma::instance_deactivated_list.erase(iter);
            }
        }
        else
        {
            if (!inside)
            {
                inst->activate();
                enigma::instance_deactivated_list.erase(iter);
            }
        }
    }
}

void position_change(double x1, double y1, int obj, bool perf)
{
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(all); it; ++it)
    {
        enigma::object_collisions* const inst = (enigma::object_collisions*)*it;

        if (inst->sprite_index == -1 && inst->mask_index == -1) //no sprite/mask then no collision
            continue;

        const bbox_rect_t &box = inst->$bbox_relative();
        const double x = inst->x, y = inst->y,
                     xscale = inst->image_xscale, yscale = inst->image_yscale,
                     ia = inst->image_angle;
        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x, y, xscale, yscale, ia);

        if (x1 >= left && x1 <= right && y1 >= top && y1 <= bottom)
            instance_change(obj, perf);
    }
}
