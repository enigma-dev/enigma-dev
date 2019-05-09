/** Copyright (C) 2011 Polygone
*** Copyright (C) 2011 Josh Ventura
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

#include <cmath>
#include <floatcomp.h>

#include "Object_Tiers/collisions_object.h"
#include "instance_system.h"
#include "roomsystem.h"
#include "move_functions.h"
#include "math_consts.h"

namespace enigma_user
{

void motion_set(int dir, cs_scalar newspeed)
{
    enigma::object_graphics* const inst = ((enigma::object_graphics*)enigma::instance_event_iterator->inst);
    inst->direction=dir;
    inst->speed=newspeed;
}

void motion_add(cs_scalar newdirection, cs_scalar newspeed)
{
    enigma::object_graphics* const inst = ((enigma::object_graphics*)enigma::instance_event_iterator->inst);
    newdirection *= (M_PI/180.0);
    inst->hspeed += (newspeed) * cos(newdirection);
    inst->vspeed -= (newspeed) * sin(newdirection);
}

void move_snap(const cs_scalar hsnap, const cs_scalar vsnap)
{
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
    if (fnzero(hsnap))
        inst->x = round(inst->x/hsnap)*hsnap;
    if (fnzero(vsnap))
        inst->y = round(inst->y/vsnap)*vsnap;
}

void move_wrap(const bool hor, const bool vert, const cs_scalar margin)
{
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
    if (hor)
    {
        const cs_scalar wdis = room_width + margin*2;
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
        const cs_scalar hdis = room_height + margin*2;
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

void move_towards_point (const cs_scalar point_x, const cs_scalar point_y, const cs_scalar newspeed) {
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
    inst->direction = fmod((atan2(inst->y-point_y,point_x-inst->x)*(180/M_PI))+360,360);
    inst->speed = (newspeed);
}

extern ma_scalar random(ma_scalar x);

void move_random(const cs_scalar snapHor, const cs_scalar snapVer)
{
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
    const int mask_ind = ((enigma::object_collisions*)enigma::instance_event_iterator->inst)->mask_index;
    const int spr_ind = ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->sprite_index;
    cs_scalar x1, y1, x2, y2;
    if (spr_ind == -1 && (mask_ind == -1))
    {
        x1 = 0; y1 = 0; x2 = room_width; y2 = room_height;
    }
    else
    {
        const int mask = (mask_ind >= 0) ? mask_ind : spr_ind;
        x1 = sprite_get_xoffset(mask); y1 = sprite_get_yoffset(mask); x2 = room_width - sprite_get_width(mask) + sprite_get_xoffset(mask); y2 = room_height - sprite_get_height(mask) + sprite_get_yoffset(mask);
    }
    using enigma_user::random;
    inst->x = x1 + (fnzero(snapHor) ? floor(random(x2 - x1)/snapHor)*snapHor : random(x2 - x1));
    inst->y = y1 + (fnzero(snapVer) ? floor(random(y2 - y1)/snapVer)*snapVer : random(y2 - y1));
}

}
