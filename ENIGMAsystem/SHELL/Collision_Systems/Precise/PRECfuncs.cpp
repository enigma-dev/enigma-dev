/** Copyright (C) 2008-2013 IsmAvatar <ismavatar@gmail.com>
*** Copyright (C) 2014 Seth N. Hetu
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

#include <cmath>
#include <limits>

#include "Universal_System/Object_Tiers/collisions_object.h"
#include "Universal_System/Instances/instance_system.h" //iter
#include "Universal_System/roomsystem.h"
#include "Collision_Systems/collision_mandatory.h" //iter
#include "Universal_System/Instances/instance.h"
#include "Universal_System/math_consts.h"

#include "../General/CSfuncs.h"
#include "PRECimpl.h"

static inline void get_border(int *leftv, int *rightv, int *topv, int *bottomv, int left, int top, int right, int bottom, cs_scalar x, cs_scalar y, double xscale, double yscale, double angle)
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

template<typename T> static inline T min(T x, T y) { return x<y? x : y; }
template<typename T> static inline T max(T x, T y) { return x>y? x : y; }
static inline double angle_difference(double dir1, double dir2) {return fmod((fmod((dir1 - dir2),360) + 540), 360) - 180;}
static inline double point_direction(cs_scalar x1, cs_scalar y1,cs_scalar x2, cs_scalar y2) {return fmod((atan2(y1-y2,x2-x1)*(180/M_PI))+360,360);}

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

namespace enigma_user
{

bool position_free(cs_scalar x, cs_scalar y)
{
  return collide_inst_point(all,true,true,false,x+.5,y+.5) == NULL;
}

bool position_empty(cs_scalar x, cs_scalar y)
{
  return collide_inst_point(all,false,true,false,x+.5,y+.5) == NULL;
}

bool position_meeting(cs_scalar x, cs_scalar y, int object)
{
  return collide_inst_point(object,false,true,false,x+.5,y+.5);
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

void position_change(cs_scalar x, cs_scalar y, int obj, bool perf)
{
    change_inst_point(obj, perf, x+.5, y+.5);
}

enigma::instance_t instance_position(cs_scalar x, cs_scalar y, int object)
{
  const enigma::object_collisions* r = collide_inst_point(object,false,true,false,x+.5,y+.5);
  return r == NULL ? noone : static_cast<int>(r->id);
}

enigma::instance_t collision_rectangle(cs_scalar x1, cs_scalar y1, cs_scalar x2, cs_scalar y2, int obj, bool prec, bool notme)
{
  const enigma::object_collisions* r = collide_inst_rect(obj,false,prec,notme,x1+.5,y1+.5,x2+.5,y2+.5); //false is for solid_only
  return r == NULL ? noone : static_cast<int>(r->id);
}

enigma::instance_t collision_line(cs_scalar x1, cs_scalar y1, cs_scalar x2, cs_scalar y2, int obj, bool prec, bool notme)
{
  const enigma::object_collisions* r = collide_inst_line(obj,false,prec,notme,x1+.5,y1+.5,x2+.5,y2+.5);
  return r == NULL ? noone : static_cast<int>(r->id);
}

enigma::instance_t collision_point(cs_scalar x, cs_scalar y, int obj, bool prec, bool notme)
{
  const enigma::object_collisions* r = collide_inst_point(obj,false, prec,notme,x+.5,y+.5);
  return r == NULL ? noone : static_cast<int>(r->id);
}

enigma::instance_t collision_circle(cs_scalar x, cs_scalar y, double radius, int obj, bool prec, bool notme)
{
  const enigma::object_collisions* r = collide_inst_circle(obj,false,prec,notme,x+.5,y+.5,radius);
  return r == NULL ? noone : static_cast<int>(r->id);
}

enigma::instance_t collision_ellipse(cs_scalar x1, cs_scalar y1, cs_scalar x2, cs_scalar y2, int obj, bool prec, bool notme)
{
  const enigma::object_collisions* r = collide_inst_ellipse(obj,false,prec,notme,((x1+x2)/2)+.5,((y1+y2)/2)+.5,fabs(x2-x1)/2,fabs(y2-y1)/2);
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
    return (distance == std::numeric_limits<double>::infinity() ? -1 : distance);
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
    if (inst1->sprite_index == -1 && (inst1->mask_index == -1)) {
        return -4;
    }

    const double x = inst1->x, y = inst1->y;

    if (collide_inst_inst(object, solid_only, true, x, y) != NULL) {
        return 0;
    }

    const double DMIN = 1, DMAX = 1000; // Arbitrary max for non-positive values, 1000 fits with other implementations.
    const double contact_distance = DMIN;
    double mid_dist = max_dist; // mid_dist is used for the subtraction part.
    if (max_dist <= 0) { // Use the arbitrary max for non-positive values.
        max_dist = DMAX;
    }
    mid_dist = max_dist;
    const double radang = (fmod(fmod(angle, 360) + 360, 360))*(M_PI/180.0);
    const double sin_angle = sin(radang), cos_angle = cos(radang);

    // Subtraction.

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
            mid_dist = DMIN;
            break;
        }

        switch (quad)
        {
            case 0:
                if ((left2 > right1 || top1 > bottom2) &&
                angle_difference(point_direction(right1, bottom1, left2, top2),angle) >= 0  &&
                angle_difference(point_direction(left1, top1, right2, bottom2),angle) <= 0)
                {
                    if (angle_difference(point_direction(right1, top1, left2, bottom2),angle) > 0)
                    {
                        mid_dist = min(mid_dist, (top1 - bottom2 - contact_distance)/sin_angle);
                    }
                    else
                    {
                        mid_dist = min(mid_dist, (left2 - right1 - contact_distance)/cos_angle);
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
                        mid_dist = min(mid_dist, (right2 - left1 + contact_distance)/cos_angle);
                    }
                    else
                    {
                        mid_dist = min(mid_dist, (top1 - bottom2 - contact_distance)/sin_angle);
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
                        mid_dist = min(mid_dist, (bottom1 - top2 + contact_distance)/sin_angle);
                    }
                    else
                    {
                        mid_dist = min(mid_dist, (right2 - left1 + contact_distance)/cos_angle);
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
                        mid_dist = min(mid_dist, (left2 - right1 - contact_distance)/cos_angle);
                    }
                    else
                    {
                        mid_dist = min(mid_dist, (bottom1 - top2 + contact_distance)/sin_angle);
                    }
                }
            break;
        }
    }

    double current_dist = DMIN;
    {
        // Avoid moving to position which isn't free. mid_dist is not guaranteed to indicate a free position.
        double next_x = x + mid_dist*cos_angle;
        double next_y = y - mid_dist*sin_angle;
        if (collide_inst_inst(object, solid_only, true, next_x, next_y) == NULL) {
            inst1->x = next_x;
            inst1->y = next_y;
            current_dist = mid_dist;
        }
    }

    // Subtraction end.

    for (; current_dist <= max_dist; current_dist++)
    {
        const double next_x = x + current_dist*cos_angle;
        const double next_y = y - current_dist*sin_angle;
        if (collide_inst_inst(object, solid_only, true, next_x, next_y) != NULL) {
            current_dist--;
            break;
        }
        else {
            inst1->x = next_x;
            inst1->y = next_y;
        }
    }

    return current_dist;
}

double move_outside_object(int object, double angle, double max_dist, bool solid_only)
{
    enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    if (inst1->sprite_index == -1 && (inst1->mask_index == -1)) {
        return -4;
    }

    double x = inst1->x, y = inst1->y;

    if (collide_inst_inst(object, solid_only, true, x, y) == NULL) {
        return 0;
    }

    const double DMIN = 1, DMAX = 1000; // Arbitrary max for non-positive values, 1000 fits with other implementations.
    if (max_dist <= 0) { // Use the arbitrary max for non-positive values.
        max_dist = DMAX;
    }
    const double radang = (fmod(fmod(angle, 360) + 360, 360))*(M_PI/180.0);

    double current_dist;
    for (current_dist = DMIN; current_dist <= max_dist; current_dist++)
    {
        const double next_x = x + current_dist*cos(radang);
        const double next_y = y - current_dist*sin(radang);
        inst1->x = next_x;
        inst1->y = next_y;
        if (collide_inst_inst(object, solid_only, true, next_x, next_y) == NULL) {
            break;
        }
    }

    return current_dist;
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

    const double x_start = inst1->x, y_start = inst1->y;

    const double x = inst1->x + inst1->hspeed, y = inst1->y + inst1->vspeed;

    if (adv) {
        const double angle_radial = 10.0; // The angle increment for the radials.
        const int radial_max = max(int(180/angle_radial), 1); // The maximum number of radials in one direction.

        const double effective_direction = inst1->speed >= 0 ? inst1->direction : fmod(inst1->direction+180.0, 360.0);
        const double flipped_direction = fmod(effective_direction + 180.0, 360.0);
        const double speed = abs((double) inst1->speed + 1); //max(1, abs(inst1->speed));

        // Find the normal direction of the collision by doing radial collisions based on the speed and flipped direction.

        int d1, d2;
        for (d1 = 0; d1 < radial_max; d1++) // Positive direction.
        {
            const double rad = (flipped_direction + d1*angle_radial)*M_PI/180.0;
            const double x1 = x + speed*cos(rad);
            const double y1 = y - speed*sin(rad);
            if (collide_inst_inst(object, solid_only, true, x1, y1) != NULL) {
                break;
            }
        }
        for (d2 = 0; d2 > -radial_max; d2--) // Negative direction.
        {
            const double rad = (flipped_direction + d2*angle_radial)*M_PI/180.0;
            const double x1 = x + speed*cos(rad);
            const double y1 = y - speed*sin(rad);
            if (collide_inst_inst(object, solid_only, true, x1, y1) != NULL) {
                break;
            }
        }

        const int d = int(round((d1 + d2)/2.0));
        const double normal_direction = fmod(flipped_direction + d*angle_radial + 360.0, 360.0);

        // Flip and then mirror the effective direction unit vector along the direction of the normal.

        const double normal_rad = normal_direction * M_PI / 180.0;
        const double normal_x = cos(normal_rad), normal_y = -sin(normal_rad);

        const double flipped_rad = flipped_direction * M_PI / 180.0;
        const double flipped_x = cos(flipped_rad), flipped_y = -sin(flipped_rad);

        const double dot_product = normal_x*flipped_x + normal_y*flipped_y;
        const double flipped_on_normal_x = dot_product*normal_x, flipped_on_normal_y = dot_product*normal_y;

        const double crossed_x = flipped_x - flipped_on_normal_x, crossed_y = flipped_y - flipped_on_normal_y;

        const double mirror_x = flipped_x - 2*crossed_x, mirror_y = flipped_y - 2*crossed_y;

        // Set final direction from flipped, mirrored direction unit vector.

        const double mirror_direction = fmod(atan2(-mirror_y, mirror_x) * 180.0 / M_PI + 360.0, 360.0);
        inst1->direction = inst1->speed >= 0 ? mirror_direction : fmod(mirror_direction + 180.0, 360.0);
        return true;
    }
    else {
        const double direction = inst1->speed >= 0 ? inst1->direction : fmod(inst1->direction+180.0, 360.0);
        const bool free_horizontal = collide_inst_inst(object, solid_only, true, x, y_start) == NULL;
        const bool free_vertical = collide_inst_inst(object, solid_only, true, x_start, y) == NULL;
        double new_direction;
        if (!free_horizontal && free_vertical) {
            new_direction = direction <= 180.0 ? fmod(180.0 - direction, 360.0) : fmod((360.0 - direction) + 180.0, 360.0);
        }
        else if (free_horizontal && !free_vertical) {
            new_direction = fmod(360.0 - direction, 360.0);
        }
        else {
            new_direction = fmod(direction + 180.0, 360.0);
        }
        inst1->direction = inst1->speed >= 0 ? new_direction : fmod(new_direction + 180.0, 360.0);

        return true;
    }
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

        if ((left <= (rleft+rwidth) && rleft <= right && top <= (rtop+rheight) && rtop <= bottom) == inside) {
            inst->deactivate();
            enigma::instance_deactivated_list.insert(inode_pair(inst->id,inst));
        }
    }
}

void instance_activate_region(int rleft, int rtop, int rwidth, int rheight, bool inside) {
    std::map<int,enigma::object_basic*>::iterator iter = enigma::instance_deactivated_list.begin();
    while (iter != enigma::instance_deactivated_list.end()) {
        enigma::object_collisions* const inst = (enigma::object_collisions*) iter->second;

        if (inst->sprite_index == -1 && (inst->mask_index == -1)) {//no sprite/mask then no collision
            ++iter;
            continue;
        }

        const enigma::bbox_rect_t &box = inst->$bbox_relative();
        const double x = inst->x, y = inst->y,
        xscale = inst->image_xscale, yscale = inst->image_yscale,
        ia = inst->image_angle;

        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x, y, xscale, yscale, ia);

        if ((left <= (rleft+rwidth) && rleft <= right && top <= (rtop+rheight) && rtop <= bottom) == inside) {
            inst->activate();
            enigma::instance_deactivated_list.erase(iter++);
        } else {
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
                enigma::instance_deactivated_list.insert(inode_pair(inst->id,inst));
            }
        }
        else
        {
            if (!inside)
            {
                inst->deactivate();
                enigma::instance_deactivated_list.insert(inode_pair(inst->id,inst));
            }
        }
    }
}

void instance_activate_circle(int x, int y, int r, bool inside)
{
    std::map<int,enigma::object_basic*>::iterator iter = enigma::instance_deactivated_list.begin();
    while (iter != enigma::instance_deactivated_list.end()) {
        enigma::object_collisions* const inst = (enigma::object_collisions*)iter->second;

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

}
