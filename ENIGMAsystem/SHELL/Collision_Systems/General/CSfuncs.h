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

#include "Universal_System/scalar.h"
#include "Universal_System/Instances/instance_system_base.h"

namespace enigma_user
{

bool place_free(cs_scalar x, cs_scalar y);
bool place_empty(cs_scalar x, cs_scalar y);
bool place_meeting(cs_scalar x, cs_scalar y, int object);
void position_change(cs_scalar x1, cs_scalar y1, int obj, bool perf);
bool position_free(cs_scalar x, cs_scalar y);
bool position_empty(cs_scalar x, cs_scalar y);
bool position_meeting(cs_scalar x, cs_scalar y, int object);
void position_destroy_object(cs_scalar x, cs_scalar y, int object, bool solid_only = false);
void position_destroy_solid(cs_scalar x, cs_scalar y);
void position_destroy(cs_scalar x, cs_scalar y);
enigma::instance_t instance_place(cs_scalar x, cs_scalar y, int object);
enigma::instance_t instance_position(cs_scalar x, cs_scalar y, int object);
enigma::instance_t collision_rectangle(cs_scalar x1, cs_scalar y1, cs_scalar x2, cs_scalar y2, int obj, bool prec = true, bool notme = true);
enigma::instance_t collision_line(cs_scalar x1, cs_scalar y1, cs_scalar x2, cs_scalar y2, int obj, bool prec = true, bool notme = true);
enigma::instance_t collision_point(cs_scalar x, cs_scalar y, int obj, bool prec = true, bool notme = true);
enigma::instance_t collision_circle(cs_scalar x, cs_scalar y, double radius, int obj, bool prec = true, bool notme = true);
enigma::instance_t collision_ellipse(cs_scalar x1, cs_scalar y1, cs_scalar x2, cs_scalar y2, int obj, bool prec = true, bool notme = true);

double distance_to_object(int object);
double distance_to_point(cs_scalar x, cs_scalar y);

double move_contact_object(int object, double angle, double max_dist = 1000000, bool solid_only = false);

inline double move_contact_all(const double direction, const double max_dist = 1000000) {
    return move_contact_object(all, direction, max_dist);
}

inline double move_contact(const double direction) {
    return move_contact_object(all, direction, -1);
}

inline double move_contact_solid(const double direction, const double max_dist = 1000000) {
    return move_contact_object(all, direction, max_dist, true);
}

double move_outside_object(int object, double angle, double max_dist = 1000000, bool solid_only = false);

inline double move_outside_all(const double direction, const double speed = 1000000) {
    return move_outside_object(all, direction, speed);
}

inline double move_outside_solid(const double direction, const double speed = 1000000) {
    return move_outside_object(all, direction, speed, true);
}

bool move_bounce_object(int object, bool adv = true, bool solid_only = false);

inline bool move_bounce_all(bool adv = true) {
    return move_bounce_object(all, adv);
}

inline bool move_bounce_solid(bool adv = true) {
    return move_bounce_object(all, adv, true);
}

void instance_deactivate_region(int rleft, int rtop, int rwidth, int rheight, bool inside = true, bool notme = true);
void instance_activate_region(int left, int top, int width, int height, bool inside = true);
void instance_deactivate_circle(int x, int y, int r, bool inside = true, bool notme = true);
void instance_activate_circle(int x, int y, int r, bool inside = true);
var instance_get_mtv(int object);

}
