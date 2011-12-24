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
static double point_direction(double x1,double y1,double x2,double y2) {return fmod((atan2(y1-y2,x2-x1)*(180/M_PI))+360,360);}

void motion_set(int dir, double newspeed)
{
    enigma::object_graphics* const inst = ((enigma::object_graphics*)enigma::instance_event_iterator->inst);
    inst->direction=dir;
    inst->speed=newspeed;
}

void motion_add(double newdirection, double newspeed)
{
    enigma::object_graphics* const inst = ((enigma::object_graphics*)enigma::instance_event_iterator->inst);
    newdirection *= (M_PI/180.0);
    inst->hspeed += (newspeed) * cos(newdirection);
    inst->vspeed -= (newspeed) * sin(newdirection);
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
