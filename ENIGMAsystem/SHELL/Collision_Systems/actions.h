/** Copyright (C) 2008-2013 Alasdair Morrison, Polygone
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

#include "Universal_System/scalar.h"
#include "Universal_System/Instances/instance_system_base.h"

namespace enigma_user
{
extern bool argument_relative;

inline bool action_if_object(const int object, const cs_scalar xx, const cs_scalar yy) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        return place_meeting(inst->x+xx,inst->y+yy,object);
    }
    else {
        return place_meeting(xx,yy,object);
    }
    return false;
}

inline bool action_if_empty(const cs_scalar xx, const cs_scalar yy, const int objects) {
    if (argument_relative) {
        if (objects == 0)
            return place_free(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->x+xx,((enigma::object_graphics*)enigma::instance_event_iterator->inst)->y+yy);
        else
            return place_empty(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->x+xx,((enigma::object_graphics*)enigma::instance_event_iterator->inst)->y+yy);
    }
    else {
        if (objects == 0)
            return place_free(xx,yy);
        else
            return place_empty(xx,yy);
    }
    return false;
}

inline void action_move_contact(const double direction, const double max_dist, const bool against) {
    move_contact_object(all, direction, max_dist, !against);
}

inline void action_bounce(int precisely, bool against) {
    move_bounce_object(all, precisely, !against);
}

inline void action_kill_position(cs_scalar x, cs_scalar y)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        position_destroy(inst->x+x, inst->y+y);
    }
    else
    {
        position_destroy(x, y);
    }
}

inline bool action_if_collision(const cs_scalar x, const cs_scalar y, const int object) {
  return !action_if_empty(x,y,object); //Already takes argument_relative into account
}

}
