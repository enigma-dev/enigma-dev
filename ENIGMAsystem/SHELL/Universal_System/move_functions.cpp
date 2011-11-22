/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2011 Polygone                                                 **
 **  Copyright (C) 2011 Josh Ventura                                             **
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

#include <cmath>
#include "collisions_object.h"
#include "instance_system.h"
#include "roomsystem.h"
#include "move_functions.h"

extern double random(double x);
extern double min(double x, double y);
extern double max(double x, double y);
extern double degtorad(double x);
extern double point_direction(double x1,double y1,double x2,double y2);
extern double direction_difference(double dir1,double dir2);

double move_contact_object(double angle, double max_dist, const int object, const bool solid_only)
{
    const double DMIN = 1, DMAX = 1000000;
    const double contact_distance = DMIN;
    if (max_dist <= 0)
    {
        max_dist = DMAX;
    }
    angle = fmod(fmod(angle, 360) + 360, 360);
    const double sin_angle = sin(degtorad(angle)), cos_angle = cos(degtorad(angle));
    enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    const int quad = int(angle/90.0);
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        const enigma::object_collisions* inst2 = (enigma::object_collisions*)*it;
        if (inst2->id == inst1->id || (solid_only && !inst2->solid)) {continue;}

        if (inst2->$bbox_right() >= inst1->$bbox_left() && inst2->$bbox_bottom() >= inst1->$bbox_top() && inst2->$bbox_left() <= inst1->$bbox_right() && inst2->$bbox_top() <= inst1->$bbox_bottom())
        {
            return 0;
        }

        switch (quad)
        {
            case 0:
                if ((inst2->$bbox_left() > inst1->$bbox_right() || inst1->$bbox_top() > inst2->$bbox_bottom()) &&
                direction_difference(point_direction(inst1->$bbox_right(), inst1->$bbox_bottom(), inst2->$bbox_left(), inst2->$bbox_top()),angle) >= 0  &&
                direction_difference(point_direction(inst1->$bbox_left(), inst1->$bbox_top(), inst2->$bbox_right(), inst2->$bbox_bottom()),angle) <= 0)
                {
                    if (direction_difference(point_direction(inst1->$bbox_right(), inst1->$bbox_top(), inst2->$bbox_left(), inst2->$bbox_bottom()),angle) > 0)
                    {
                        max_dist = min(max_dist, (inst1->$bbox_top() - inst2->$bbox_bottom() - contact_distance)/sin_angle);
                    }
                    else
                    {
                        max_dist = min(max_dist, (inst2->$bbox_left() - inst1->$bbox_right() - contact_distance)/cos_angle);
                    }
                }
            break;
            case 1:
                if ((inst1->$bbox_left() > inst2->$bbox_right() || inst1->$bbox_top() > inst2->$bbox_bottom()) &&
                direction_difference(point_direction(inst1->$bbox_left(), inst1->$bbox_bottom(), inst2->$bbox_right(), inst2->$bbox_top()),angle) <= 0  &&
                direction_difference(point_direction(inst1->$bbox_right(), inst1->$bbox_top(), inst2->$bbox_left(), inst2->$bbox_bottom()),angle) >= 0)
                {
                    if (direction_difference(point_direction(inst1->$bbox_left(), inst1->$bbox_top(), inst2->$bbox_right(), inst2->$bbox_bottom()),angle) > 0)
                    {
                        max_dist = min(max_dist, (inst2->$bbox_right() - inst1->$bbox_left() + contact_distance)/cos_angle);
                    }
                    else
                    {
                        max_dist = min(max_dist, (inst1->$bbox_top() - inst2->$bbox_bottom() - contact_distance)/sin_angle);
                    }
                }
            break;
            case 2:
                if ((inst1->$bbox_left() > inst2->$bbox_right() || inst2->$bbox_top() > inst1->$bbox_bottom()) &&
                direction_difference(point_direction(inst1->$bbox_right(), inst1->$bbox_bottom(), inst2->$bbox_left(), inst2->$bbox_top()),angle) <= 0  &&
                direction_difference(point_direction(inst1->$bbox_left(), inst1->$bbox_top(), inst2->$bbox_right(), inst2->$bbox_bottom()),angle) >= 0)
                {
                    if (direction_difference(point_direction(inst1->$bbox_left(), inst1->$bbox_bottom(), inst2->$bbox_right(), inst2->$bbox_top()),angle) > 0)
                    {
                        max_dist = min(max_dist, (inst1->$bbox_bottom() - inst2->$bbox_top() + contact_distance)/sin_angle);
                    }
                    else
                    {
                        max_dist = min(max_dist, (inst2->$bbox_right() - inst1->$bbox_left() + contact_distance)/cos_angle);
                    }
                }
            break;
            case 3:
                if ((inst2->$bbox_left() > inst1->$bbox_right() || inst2->$bbox_top() > inst1->$bbox_bottom()) &&
                direction_difference(point_direction(inst1->$bbox_right(), inst1->$bbox_top(), inst2->$bbox_left(), inst2->$bbox_bottom()),angle) <= 0  &&
                direction_difference(point_direction(inst1->$bbox_left(), inst1->$bbox_bottom(), inst2->$bbox_right(), inst2->$bbox_top()),angle) >= 0)
                {
                    if (direction_difference(point_direction(inst1->$bbox_right(), inst1->$bbox_bottom(), inst2->$bbox_left(), inst2->$bbox_top()),angle) > 0)
                    {
                        max_dist = min(max_dist, (inst2->$bbox_left() - inst1->$bbox_right() - contact_distance)/cos_angle);
                    }
                    else
                    {
                        max_dist = min(max_dist, (inst1->$bbox_bottom() - inst2->$bbox_top() + contact_distance)/sin_angle);
                    }
                }
            break;
        }
    }
    inst1->x += cos_angle*max_dist;
    inst1->y -= sin_angle*max_dist;
    return max_dist;
}

double move_outside_object(double angle, double max_dist, const int object, const bool solid_only)
{
    const double DMIN = 0.000001, DMAX = 1000000;
    const double contact_distance = DMIN;
    if (max_dist <= 0)
    {
        max_dist = DMAX;
    }
    int dist = 0;
    angle = ((angle %(variant) 360) + 360) %(variant) 360;
    const double sin_angle = sin(degtorad(angle)), cos_angle = cos(degtorad(angle));
    enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    const int quad = int(angle/90.0);
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        const enigma::object_collisions* inst2 = (enigma::object_collisions*)*it;
        if (inst2->id == inst1->id || (solid_only && !inst2->solid)) {continue;}

        if (!(inst2->$bbox_right() >= inst1->$bbox_left() && inst2->$bbox_bottom() >= inst1->$bbox_top() && inst2->$bbox_left() <= inst1->$bbox_right() && inst2->$bbox_top() <= inst1->$bbox_bottom()))
        {
            continue;
        }

        switch (quad)
        {
            case 0:
                if (direction_difference(point_direction(inst1->$bbox_right(), inst1->$bbox_top(), inst2->$bbox_right(), inst2->$bbox_top()),angle) < 0)
                {
                    dist = max(dist, ((inst1->$bbox_bottom()) - (inst2->$bbox_top()) + contact_distance)/sin_angle);
                }
                else
                {
                    dist = max(dist, ((inst2->$bbox_right()) - (inst1->$bbox_left()) + contact_distance)/cos_angle);
                }
            break;
            case 1:
                if (direction_difference(point_direction(inst1->$bbox_left(), inst1->$bbox_top(), inst2->$bbox_left(), inst2->$bbox_top()),angle) < 0)
                {
                    dist = max(dist, ((inst2->$bbox_left()) - (inst1->$bbox_right()) - contact_distance)/cos_angle);
                }
                else
                {
                    dist = max(dist, ((inst1->$bbox_bottom()) - (inst2->$bbox_top()) + contact_distance)/sin_angle);
                }
            break;
            case 2:
                if (direction_difference(point_direction(inst1->$bbox_left(), inst1->$bbox_bottom(), inst2->$bbox_left(), inst2->$bbox_bottom()),angle) < 0)
                {
                    dist = max(dist, ((inst1->$bbox_top()) - (inst2->$bbox_bottom()) - contact_distance)/sin_angle);
                }
                else
                {
                    dist = max(dist, ((inst2->$bbox_left()) - (inst1->$bbox_right()) - contact_distance)/cos_angle);
                }
            break;
            case 3:
                if (direction_difference(point_direction(inst1->$bbox_right(), inst1->$bbox_bottom(), inst2->$bbox_right(), inst2->$bbox_bottom()),angle) < 0)
                {
                    dist = max(dist, ((inst2->$bbox_right()) - (inst1->$bbox_left()) + contact_distance)/cos_angle);
                }
                else
                {
                    dist = max(dist, ((inst1->$bbox_top()) - (inst2->$bbox_bottom()) - contact_distance)/sin_angle);
                }
            break;
        }
    }
    dist = min(dist, max_dist);
    inst1->x += cos_angle*dist;
    inst1->y -= sin_angle*dist;
    return dist;
}

int move_bounce_object(const bool adv, const int object, const bool solid_only)
{
    enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        const enigma::object_collisions* inst2 = (enigma::object_collisions*)*it;
        if (inst2->id == inst1->id || (solid_only && !inst2->solid)) {continue;}

        if (inst1->$bbox_left() <= inst2->$bbox_right() && inst1->$bbox_top() <= inst2->$bbox_bottom() && inst1->$bbox_right() >= inst2->$bbox_left() && inst1->$bbox_bottom() >= inst2->$bbox_top())
        {
            if (inst1->$bbox_left() - inst1->hspeed > inst2->$bbox_right() || inst1->$bbox_right() - inst1->hspeed < inst2->$bbox_left())
            {
                inst1->hspeed *= -1;
            }
            if (inst1->$bbox_top() - inst1->vspeed > inst2->$bbox_bottom() || inst1->$bbox_bottom() - inst1->vspeed < inst2->$bbox_top())
            {
                inst1->vspeed *= -1;
            }
            inst1->x += inst1->hspeed;
            inst1->y += inst1->vspeed;
            return inst2->id;
        }
        if (inst1->hspeed + inst1->$bbox_left() <= inst2->$bbox_right() && inst1->vspeed + inst1->$bbox_top() <= inst2->$bbox_bottom() && inst1->hspeed + inst1->$bbox_right() >= inst2->$bbox_left() && inst1->vspeed + inst1->$bbox_bottom() >= inst2->$bbox_top())
        {
            if (inst1->$bbox_left() > inst2->$bbox_right() || inst1->$bbox_right() < inst2->$bbox_left())
            {
                inst1->hspeed *= -1;
            }
            if (inst1->$bbox_top() > inst2->$bbox_bottom() || inst1->$bbox_bottom() < inst2->$bbox_top())
            {
                inst1->vspeed *= -1;
            }
            return inst2->id;
        }
    }
    return -4;
}

void motion_set(int dir, double newspeed)
{
    enigma::object_graphics* const inst = ((enigma::object_graphics*)enigma::instance_event_iterator->inst);
    inst->direction=dir;
    inst->speed=newspeed;
}

void motion_add(double newdirection, double newspeed)
{
    enigma::object_graphics* const inst = ((enigma::object_graphics*)enigma::instance_event_iterator->inst);
    inst->hspeed+= (newspeed) * cos(degtorad((newdirection)));
    inst->vspeed-= (newspeed) * sin(degtorad((newdirection)));
}

void move_random(const double snapHor, const double snapVer)
{
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
    const int mask_ind = ((enigma::object_collisions*)enigma::instance_event_iterator->inst)->mask_index;
    const int spr_ind = ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->sprite_index;
    const int mask = mask_ind >= 0 ? mask_ind : spr_ind;
    const double x1 = sprite_get_xoffset(mask), y1 = sprite_get_yoffset(mask), x2 = room_width - sprite_get_width(mask) + sprite_get_xoffset(mask), y2 = room_height - sprite_get_height(mask) + sprite_get_yoffset(mask);

    inst->x = x1 + (snapHor? random(x2 - x1) : floor(random(x2 - x1)/snapHor)*snapHor);
    inst->y = y1 + (snapVer == 0 ? floor(random(y2 - y1)/snapVer)*snapVer : random(y2 - y1));
}

void move_snap(const double hsnap, const double vsnap)
{
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
    if (hsnap)
        inst->x = round(inst->x/hsnap)*hsnap;
    if (vsnap)
        inst->y = round(inst->y/vsnap)*vsnap;
}

void move_wrap(const bool hor, const bool vert, const double margin)
{
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
    if (hor)
    {
        const double wdis = room_width + margin*2;
        if (inst->x < -margin)
        {
            inst->x += wdis*ceil(((-margin) - inst->x)/wdis);
        }
        if (inst->x > room_width + margin)
        {
            inst->x -= wdis*ceil((inst->x - (room_width + margin))/wdis);
        }
    }
    if (vert)
    {
        const double hdis = room_height + margin*2;
        if (inst->y < -margin)
        {
            inst->y += hdis*ceil(((-margin) - inst->y)/hdis);
        }
        if (inst->y > room_height + margin)
        {
            inst->y -= hdis*ceil((inst->y - (room_height + margin))/hdis);
        }
    }
}

bool place_snapped(int hsnap, int vsnap) {
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
    return  ((((int) inst->x) % ((int) hsnap) == 0) &&  (((int) inst->y) % ((int) vsnap)==0));
}

void move_towards_point (const double point_x, const double point_y, const double newspeed) {
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
    inst->direction = point_direction ( inst->x,inst->y, (point_x), (point_y) );
    inst->speed = (newspeed);
}
