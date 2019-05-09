/** Copyright (C) 2008-2013 IsmAvatar <ismavatar@gmail.com>
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

////////////////////////////////////
// GM front-end functions - Implementations of standard GM collision functions.
////////////////////////////////////

#include "Universal_System/Object_Tiers/collisions_object.h"
#include "Universal_System/instance_system.h" //iter
#include "Universal_System/roomsystem.h"
#include "Collision_Systems/collision_mandatory.h" //iter
#include "BBOXimpl.h"
#include "../General/CSfuncs.h"
#include <limits>
#include <cmath>
#include "Universal_System/instance.h"

#include <floatcomp.h>

static inline void get_border(int *leftv, int *rightv, int *topv, int *bottomv, int left, int top, int right, int bottom, cs_scalar x, cs_scalar y, cs_scalar xscale, cs_scalar yscale, double angle)
{
    if (fzero(angle))
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
static inline double angle_difference(double dir1, double dir2) {return fmod((fmod((dir1 - dir2),360) + 540), 360) - 180;}
static inline double point_direction(double x1, double y1, double x2, double y2) {return fmod((atan2(y1-y2,x2-x1)*(180/M_PI))+360,360);}

namespace enigma_user
{

bool place_free(cs_scalar x, cs_scalar y)
{
  return collide_inst_inst(all,true,true,x,y) == NULL;
}

bool place_empty(cs_scalar x, cs_scalar y)
{
  return collide_inst_inst(all,false,true,x,y) == NULL;
}

bool place_meeting(cs_scalar x, cs_scalar y, int object)
{
  return collide_inst_inst(object,false,true,x,y);
}

enigma::instance_t instance_place(cs_scalar x, cs_scalar y, int object)
{
  enigma::object_collisions* const r = collide_inst_inst(object,false,true,x,y);
  return r == NULL ? noone : static_cast<int>(r->id);
}

}

namespace enigma {
  object_basic *place_meeting_inst(cs_scalar x, cs_scalar y, int object)
  {
    return collide_inst_inst(object,false,true,x,y);
  }
}

namespace enigma_user {

bool position_free(cs_scalar x, cs_scalar y)
{
  return collide_inst_point(all,true,false,x+.5,y+.5) == NULL;
}

bool position_empty(cs_scalar x, cs_scalar y)
{
  return collide_inst_point(all,false,false,x+.5,y+.5) == NULL;
}

bool position_meeting(cs_scalar x, cs_scalar y, int object)
{
  return collide_inst_point(object,false,false,x+.5,y+.5);
}

void position_destroy_object(cs_scalar x, cs_scalar y, int object, bool solid_only)
{
    destroy_inst_point(object,solid_only,x+.5,y+.5);
}

void position_destroy_solid(cs_scalar x, cs_scalar y)
{
    destroy_inst_point(all,true,x+.5,y+.5);
}

void position_destroy(cs_scalar x, cs_scalar y)
{
    destroy_inst_point(all,false,x+.5,y+.5);
}

enigma::instance_t instance_position(cs_scalar x, cs_scalar y, int object)
{
  const enigma::object_collisions* r = collide_inst_point(object,false,false,x+.5,y+.5);
  return r == NULL ? noone : static_cast<int>(r->id);
}

enigma::instance_t collision_rectangle(cs_scalar x1, cs_scalar y1, cs_scalar x2, cs_scalar y2, int obj, bool prec /*ignored*/, bool notme)
{
  const enigma::object_collisions* r = collide_inst_rect(obj,false,notme,x1+.5,y1+.5,x2+.5,y2+.5); //false is for solid_only, not prec
  return r == NULL ? noone : static_cast<int>(r->id);
}

enigma::instance_t collision_line(cs_scalar x1, cs_scalar y1, cs_scalar x2, cs_scalar y2, int obj, bool prec /*ignored*/, bool notme)
{
  const enigma::object_collisions* r = collide_inst_line(obj,false,notme,x1+.5,y1+.5,x2+.5,y2+.5); //false is for solid_only, not prec
  return r == NULL ? noone : static_cast<int>(r->id);
}

enigma::instance_t collision_point(cs_scalar x, cs_scalar y, int obj, bool prec /*ignored*/, bool notme)
{
  const enigma::object_collisions* r = collide_inst_point(obj,false,notme,x+.5,y+.5); //false is for solid_only, not prec
  return r == NULL ? noone : static_cast<int>(r->id);
}

enigma::instance_t collision_circle(cs_scalar x, cs_scalar y, double radius, int obj, bool prec /*ignored*/, bool notme)
{
  const enigma::object_collisions* r = collide_inst_circle(obj,false,notme,x+.5,y+.5,radius); //false is for solid_only, not prec
  return r == NULL ? noone : static_cast<int>(r->id);
}

enigma::instance_t collision_ellipse(cs_scalar x1, cs_scalar y1, cs_scalar x2, cs_scalar y2, int obj, bool prec /*ignored*/, bool notme)
{
  const enigma::object_collisions* r = collide_inst_ellipse(obj,false,notme,((x1+x2)/2)+.5,((y1+y2)/2)+.5,fabs(x2-x1)/2,fabs(y2-y1)/2); //false is for solid_only, not prec
  return r == NULL ? noone : static_cast<int>(r->id);
}

double distance_to_object(int object)
{
    const enigma::object_collisions* inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    if (inst1->sprite_index == -1 && (inst1->mask_index == -1))
        return -1;
    double distance = std::numeric_limits<double>::infinity();
    double tempdist;
    const enigma::bbox_rect_t &box = inst1->$bbox_relative();
    const double x1 = inst1->x, y1 = inst1->y,
                 xscale1 = inst1->image_xscale, yscale1 = inst1->image_yscale,
                 ia1 = inst1->image_angle;
    int left1, top1, right1, bottom1;

    get_border(&left1, &right1, &top1, &bottom1, box.left, box.top, box.right, box.bottom, x1, y1, xscale1, yscale1, ia1);

    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        const enigma::object_collisions* inst2 = (enigma::object_collisions*)*it;
        if (inst1 == inst2) continue;
        if (inst2->sprite_index == -1 && (inst2->mask_index == -1))
            continue;

        const enigma::bbox_rect_t &box2 = inst2->$bbox_relative();
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
    return (std::isinf(distance) ? -1 : distance);
}

double distance_to_point(cs_scalar x, cs_scalar y)
{
    enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    if (inst1->sprite_index == -1 && (inst1->mask_index == -1))
        return -1;
    const enigma::bbox_rect_t &box = inst1->$bbox_relative();
    const double x1 = inst1->x, y1 = inst1->y,
                 xscale1 = inst1->image_xscale, yscale1 = inst1->image_yscale,
                 ia1 = inst1->image_angle;
    int left1, top1, right1, bottom1;

    get_border(&left1, &right1, &top1, &bottom1, box.left, box.top, box.right, box.bottom, x1, y1, xscale1, yscale1, ia1);

    return fabs(hypot(min(left1 - x, right1 - x),
                    min(top1 - y, bottom1 - y)));
}

double move_contact_object(int object, double angle, double max_dist, bool solid_only)
{
    enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    if (inst1->sprite_index == -1 && (inst1->mask_index == -1))
        return -4;
    const double DMIN = 1, DMAX = 1000000;
    const double contact_distance = DMIN;
    double sin_angle, cos_angle;
    if (max_dist <= 0)
    {
        max_dist = DMAX;
    }
    angle = fmod(fmod(angle, 360) + 360, 360);
    if (fequal(angle, 90))
    {
        sin_angle = 1; cos_angle = 0;
    }
    else if (fequal(angle, 180))
    {
        sin_angle = 0; cos_angle = -1;
    }
    else if (fequal(angle, 270))
    {
        sin_angle = -1; cos_angle = 0;
    }
    else
    {
        const double radang = angle*(M_PI/180.0);
        sin_angle = sin(radang), cos_angle = cos(radang);
    }
    const int quad = int(angle/90.0);

    const enigma::bbox_rect_t &box = inst1->$bbox_relative();
    const double x1 = inst1->x, y1 = inst1->y,
                 xscale1 = inst1->image_xscale, yscale1 = inst1->image_yscale,
                 ia1 = inst1->image_angle;
    int left1, top1, right1, bottom1;

    get_border(&left1, &right1, &top1, &bottom1, box.left, box.top, box.right, box.bottom, x1, y1, xscale1, yscale1, ia1);

    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        const enigma::object_collisions* inst2 = (enigma::object_collisions*)*it;
        if (inst2->sprite_index == -1 && (inst2->mask_index == -1))
            continue;
        if (inst2->id == inst1->id || (solid_only && !inst2->solid))
            continue;
        const enigma::bbox_rect_t &box2 = inst2->$bbox_relative();
        const double x2 = inst2->x, y2 = inst2->y,
                     xscale2 = inst2->image_xscale, yscale2 = inst2->image_yscale,
                     ia2 = inst2->image_angle;
        int left2, top2, right2, bottom2;

        get_border(&left2, &right2, &top2, &bottom2, box2.left, box2.top, box2.right, box2.bottom, x2, y2, xscale2, yscale2, ia2);

        if (right2 >= left1 && bottom2 >= top1 && left2 <= right1 && top2 <= bottom1)
        {
            return 0;
        }

        switch (quad & 3)
        {
            case 0: default: // Default case prevents warnings; value can never be outside this range
                if ((left2 > right1 || top1 > bottom2) &&
                angle_difference(point_direction(right1, bottom1, left2, top2),angle) >= 0  &&
                angle_difference(point_direction(left1, top1, right2, bottom2),angle) <= 0)
                {
                    if (angle_difference(point_direction(right1, top1, left2, bottom2),angle) > 0)
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
                angle_difference(point_direction(left1, bottom1, right2, top2),angle) <= 0  &&
                angle_difference(point_direction(right1, top1, left2, bottom2),angle) >= 0)
                {
                    if (angle_difference(point_direction(left1, top1, right2, bottom2),angle) > 0)
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
                angle_difference(point_direction(right1, bottom1, left2, top2),angle) <= 0  &&
                angle_difference(point_direction(left1, top1, right2, bottom2),angle) >= 0)
                {
                    if (angle_difference(point_direction(left1, bottom1, right2, top2),angle) > 0)
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
                angle_difference(point_direction(right1, top1, left2, bottom2),angle) <= 0  &&
                angle_difference(point_direction(left1, bottom1, right2, top2),angle) >= 0)
                {
                    if (angle_difference(point_direction(right1, bottom1, left2, top2),angle) > 0)
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
    enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    if (inst1->sprite_index == -1 && (inst1->mask_index == -1))
        return -4;
    const double DMAX = 1000000;
    if (max_dist <= 0)
    {
        max_dist = DMAX;
    }
    double dist = 0;
    angle = fmod(angle, 360.0);
    double radang = angle*(M_PI/180.0);
    const double sin_angle = sin(radang), cos_angle = cos(radang);
    const int quad = int(angle/90.0);
    const double    xscale1 = inst1->image_xscale, yscale1 = inst1->image_yscale,
                     ia1 = inst1->image_angle;

    const double x_start = inst1->x, y_start = inst1->y;

    bool had_collision = true;

    while (had_collision) // If there was no collision in the last iteration, we have moved outside the object.
    {
        had_collision = false;
        for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
        {
            const enigma::bbox_rect_t &box = inst1->$bbox_relative();
            const double x1 = inst1->x, y1 = inst1->y;
            int left1, top1, right1, bottom1;

            get_border(&left1, &right1, &top1, &bottom1, box.left, box.top, box.right, box.bottom, x1, y1, xscale1, yscale1, ia1);

            const enigma::object_collisions* inst2 = (enigma::object_collisions*)*it;
            if (inst2->id == inst1->id || (solid_only && !inst2->solid))
                continue;
            if (inst2->sprite_index == -1 && (inst2->mask_index == -1))
                continue;
            const enigma::bbox_rect_t &box2 = inst2->$bbox_relative();
            const double x2 = inst2->x, y2 = inst2->y,
                         xscale2 = inst2->image_xscale, yscale2 = inst2->image_yscale,
                         ia2 = inst2->image_angle;
            int left2, top2, right2, bottom2;

            get_border(&left2, &right2, &top2, &bottom2, box2.left, box2.top, box2.right, box2.bottom, x2, y2, xscale2, yscale2, ia2);

            if (!(right2 >= left1 && bottom2 >= top1 && left2 <= right1 && top2 <= bottom1))
                continue;

            had_collision = true;

            // Move at least one step every time there is a collision.
            const double min_dist = dist + 1;

            switch (quad & 3)
            {
                case 0: default: // Default case prevents warnings; value can never be outside this range
                    if (angle_difference(point_direction(left1, bottom1, right2, top2),angle) < 0)
                    {
                        dist += ((bottom1) - (top2))/sin_angle;
                    }
                    else
                    {
                        dist += ((right2) - (left1))/cos_angle;
                    }
                break;
                case 1:
                    if (angle_difference(point_direction(right1, bottom1, left2, top2),angle) < 0)
                    {
                        dist += ((left2) - (right1))/cos_angle;
                    }
                    else
                    {
                        dist += ((bottom1) - (top2))/sin_angle;
                    }
                break;
                case 2:
                if (angle_difference(point_direction(right1, top1, left2, bottom2),angle) < 0)
                {
                    dist += ((top1) - (bottom2))/sin_angle;
                }
                else
                {
                    dist += ((left2) - (right1))/cos_angle;
                }
                break;
                case 3:
                    if (angle_difference(point_direction(left1, top1, right2, bottom2),angle) < 0)
                    {
                        dist += ((right2) - (left1))/cos_angle;
                    }
                    else
                    {
                        dist += ((top1) - (bottom2))/sin_angle;
                    }
                break;
            }
            dist = max(min(dist, max_dist), min_dist);
            inst1->x = x_start + cos_angle*dist;
            inst1->y = y_start - sin_angle*dist;

            for (int i = 0; i < 1; i++) // Move a single step on if the moving was not precise.
            {
                if (collide_inst_inst(inst2->id, solid_only, true, inst1->x, inst1->y) == NULL) {
                    break;
                }
                dist += 1;
                inst1->x = x_start + cos_angle*dist;
                inst1->y = y_start - sin_angle*dist;
            }
        }
    }

    return dist;
}

bool move_bounce_object(int object, bool adv, bool solid_only)
{
    enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    if (inst1->sprite_index == -1 && (inst1->mask_index == -1))
        return false;

    if (collide_inst_inst(object, solid_only, true, inst1->x, inst1->y) == NULL &&
        collide_inst_inst(object, solid_only, true, inst1->x + inst1->hspeed, inst1->y + inst1->vspeed) == NULL) {
        return false;
    }

    if (collide_inst_inst(object, solid_only, true, inst1->x, inst1->y) != NULL) {
        // Return the instance to its previous position.
        inst1->x = inst1->xprevious;
        inst1->y = inst1->yprevious;
    }

    const double angle = inst1->direction, radang = angle*(M_PI/180.0), BBOX_EPSILON = 0.00001;
    double sin_angle = sin(radang), cos_angle = cos(radang), pc_corner, pc_dist, max_dist = 1000000;
    int side_type = 0;
    const int quad = int(2*radang/M_PI);
    const enigma::bbox_rect_t &box = inst1->$bbox_relative();
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
        if (inst2->sprite_index == -1 && (inst2->mask_index == -1))
            continue;
        const enigma::bbox_rect_t &box2 = inst2->$bbox_relative();
        const double x2 = inst2->x, y2 = inst2->y,
                     xscale2 = inst2->image_xscale, yscale2 = inst2->image_yscale,
                     ia2 = inst2->image_angle;
        int left2, top2, right2, bottom2;

        get_border(&left2, &right2, &top2, &bottom2, box2.left, box2.top, box2.right, box2.bottom, x2, y2, xscale2, yscale2, ia2);

        if (right2 >= left1 && bottom2 >= top1 && left2 <= right1 && top2 <= bottom1)
            return false;

        switch (quad & 3)
        {
            case 0: default: // Default case prevents warnings; value can never be outside this range
                if ((left2 > right1 || top1 > bottom2) &&
                angle_difference(point_direction(right1, bottom1, left2, top2),angle) >= 0  &&
                angle_difference(point_direction(left1, top1, right2, bottom2),angle) <= 0)
                {
                    pc_corner = angle_difference(point_direction(right1, top1, left2, bottom2),angle);
                    if (fabs(pc_corner) < BBOX_EPSILON)
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
                        if (fabs(pc_dist - max_dist) < BBOX_EPSILON)
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
                        if (fabs(pc_dist - max_dist) < BBOX_EPSILON)
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
                angle_difference(point_direction(left1, bottom1, right2, top2),angle) <= 0  &&
                angle_difference(point_direction(right1, top1, left2, bottom2),angle) >= 0)
                {
                    pc_corner = angle_difference(point_direction(left1, top1, right2, bottom2),angle);

                    if (fabs(pc_corner) < BBOX_EPSILON)
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
                        if (fabs(pc_dist - max_dist) < BBOX_EPSILON)
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
                        if (fabs(pc_dist - max_dist) < BBOX_EPSILON)
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
                angle_difference(point_direction(right1, bottom1, left2, top2),angle) <= 0  &&
                angle_difference(point_direction(left1, top1, right2, bottom2),angle) >= 0)
                {
                    pc_corner = angle_difference(point_direction(left1, bottom1, right2, top2),angle);
                    if (fabs(pc_corner) < BBOX_EPSILON)
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
                        if (fabs(pc_dist - max_dist) < BBOX_EPSILON)
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
                        if (fabs(pc_dist - max_dist) < BBOX_EPSILON)
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
                angle_difference(point_direction(right1, top1, left2, bottom2),angle) <= 0  &&
                angle_difference(point_direction(left1, bottom1, right2, top2),angle) >= 0)
                {
                    pc_corner = angle_difference(point_direction(right1, bottom1, left2, top2),angle);
                    if (fabs(pc_corner) < BBOX_EPSILON)
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
                        if (fabs(pc_dist - max_dist) < BBOX_EPSILON)
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
                        if (fabs(pc_dist - max_dist) < BBOX_EPSILON)
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
        case 3: case 4: default: //corner or both horizontal and vertical side hit
            inst1->hspeed *= -1;
            inst1->vspeed *= -1;
        break;
    }
    return true;
}

}

typedef std::pair<int,enigma::object_basic*> inode_pair;

namespace enigma_user
{

void instance_deactivate_region(int rleft, int rtop, int rwidth, int rheight, bool inside, bool notme) {
    for (enigma::iterator it = enigma::instance_list_first(); it; ++it) {
        if (notme && (*it)->id == enigma::instance_event_iterator->inst->id) continue;
        enigma::object_collisions* const inst = (enigma::object_collisions*) *it;

        if (inst->sprite_index == -1 && (inst->mask_index == -1)) //no sprite/mask then no collision
            continue;

        const enigma::bbox_rect_t &box = inst->$bbox_relative();
        const double x = inst->x, y = inst->y,
        xscale = inst->image_xscale, yscale = inst->image_yscale,
        ia = inst->image_angle;

        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x, y, xscale, yscale, ia);

        if (left <= (rleft+rwidth) && rleft <= right && top <= (rtop+rheight) && rtop <= bottom) {
            if (inside) {
            inst->deactivate();
            enigma::instance_deactivated_list.insert(inode_pair(inst->id,inst));
            }
        } else {
            if (!inside) {
                inst->deactivate();
                enigma::instance_deactivated_list.insert(inode_pair(inst->id,inst));
            }
        }
    }
}

void instance_activate_region(int rleft, int rtop, int rwidth, int rheight, bool inside) {
    std::map<int,enigma::object_basic*>::iterator iter = enigma::instance_deactivated_list.begin();
    while (iter != enigma::instance_deactivated_list.end()) {

        enigma::object_collisions* const inst = (enigma::object_collisions*) iter->second;

        if (inst->sprite_index == -1 && (inst->mask_index == -1)) { //no sprite/mask then no collision
            ++iter;
            continue;
        }

        const enigma::bbox_rect_t &box = inst->$bbox_relative();
        const double x = inst->x, y = inst->y,
        xscale = inst->image_xscale, yscale = inst->image_yscale,
        ia = inst->image_angle;

        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x, y, xscale, yscale, ia);

        bool removed = false;
        if (left <= (rleft+rwidth) && rleft <= right && top <= (rtop+rheight) && rtop <= bottom) {
            if (inside) {
                inst->activate();
                enigma::instance_deactivated_list.erase(iter++);
                removed = true;
            }
        } else {
            if (!inside) {
                inst->activate();
                enigma::instance_deactivated_list.erase(iter++);
                removed = true;
            }
        }
        if (!removed) {
            ++iter;
        }
    }
}

}

static bool line_ellipse_intersects(cs_scalar rx, cs_scalar ry, cs_scalar x, cs_scalar ly1, cs_scalar ly2)
{
    // Formula: x^2/a^2 + y^2/b^2 = 1   <=>   y = +/- sqrt(b^2*(1 - x^2/a^2))

    const cs_scalar inner = ry*ry*(1 - x*x/(rx*rx));
    if (inner < 0) {
        return false;
    }
    else {
        const cs_scalar y1 = -sqrt(inner), y2 = sqrt(inner);
        return y1 <= ly2 && ly1 <= y2;
    }
}

namespace enigma_user
{

void instance_deactivate_circle(int x, int y, int r, bool inside, bool notme)
{
    for (enigma::iterator it = enigma::instance_list_first(); it; ++it)
    {
        if (notme && (*it)->id == enigma::instance_event_iterator->inst->id)
            continue;
        enigma::object_collisions* const inst = (enigma::object_collisions*) *it;

        if (inst->sprite_index == -1 && (inst->mask_index == -1)) //no sprite/mask then no collision
            continue;

        const enigma::bbox_rect_t &box = inst->$bbox_relative();
        const double x1 = inst->x, y1 = inst->y,
        xscale = inst->image_xscale, yscale = inst->image_yscale,
        ia = inst->image_angle;

        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x1, y1, xscale, yscale, ia);

        const bool intersects = line_ellipse_intersects(r, r, left-x, top-y, bottom-y) ||
                                 line_ellipse_intersects(r, r, right-x, top-y, bottom-y) ||
                                 line_ellipse_intersects(r, r, top-y, left-x, right-x) ||
                                 line_ellipse_intersects(r, r, bottom-y, left-x, right-x) ||
                                 (x >= left && x <= right && y >= top && y <= bottom); // Circle inside bbox.

        if (intersects)
        {
            if (inside)
            {
                inst->deactivate();
                enigma::instance_deactivated_list.insert(inode_pair(inst->id, inst));
            }
        }
        else
        {
            if (!inside)
            {
                inst->deactivate();
                enigma::instance_deactivated_list.insert(inode_pair(inst->id, inst));
            }
        }
    }
}


void instance_activate_circle(int x, int y, int r, bool inside)
{
    std::map<int,enigma::object_basic*>::iterator iter = enigma::instance_deactivated_list.begin();
    while (iter != enigma::instance_deactivated_list.end()) {
        enigma::object_collisions* const inst = (enigma::object_collisions*) iter->second;

        if (inst->sprite_index == -1 && (inst->mask_index == -1)) { //no sprite/mask then no collision
            ++iter;
            continue;
        }

        const enigma::bbox_rect_t &box = inst->$bbox_relative();
        const double x1 = inst->x, y1 = inst->y,
        xscale = inst->image_xscale, yscale = inst->image_yscale,
        ia = inst->image_angle;

        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x1, y1, xscale, yscale, ia);

        const bool intersects = line_ellipse_intersects(r, r, left-x, top-y, bottom-y) ||
                                 line_ellipse_intersects(r, r, right-x, top-y, bottom-y) ||
                                 line_ellipse_intersects(r, r, top-y, left-x, right-x) ||
                                 line_ellipse_intersects(r, r, bottom-y, left-x, right-x) ||
                                 (x >= left && x <= right && y >= top && y <= bottom); // Circle inside bbox.

        bool removed = false;
        if (intersects)
        {
            if (inside)
            {
                inst->activate();
                enigma::instance_deactivated_list.erase(iter++);
                removed = true;
            }
        }
        else
        {
            if (!inside)
            {
                inst->activate();
                enigma::instance_deactivated_list.erase(iter++);
                removed = true;
            }
        }
        if (!removed) {
            ++iter;
        }
    }
}

void position_change(cs_scalar x1, cs_scalar y1, int obj, bool perf)
{
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(enigma_user::all); it; ++it)
    {
        enigma::object_collisions* const inst = (enigma::object_collisions*)*it;

        if (inst->sprite_index == -1 && inst->mask_index == -1) //no sprite/mask then no collision
            continue;

        const enigma::bbox_rect_t &box = inst->$bbox_relative();
        const double x = inst->x, y = inst->y,
                     xscale = inst->image_xscale, yscale = inst->image_yscale,
                     ia = inst->image_angle;
        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x, y, xscale, yscale, ia);

        if (x1 >= left && x1 <= right && y1 >= top && y1 <= bottom)
            enigma::instance_change_inst(obj, perf, inst);
    }
}

}
