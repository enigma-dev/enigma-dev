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

#include "coll_util.h"

bool place_free(double x,double y);
bool place_empty(double x,double y);
bool place_meeting(double x, double y, int object);
bool position_free(double x,double y);
bool position_empty(double x, double y);
bool position_meeting(double x, double y, int object);
void position_destroy_object(double x, double y, int object, bool solid_only = false);
void position_destroy_solid(double x, double y);
void position_destroy(double x, double y);
int instance_place(double x, double y, int object);
int instance_position(double x, double y, int object);
int collision_rectangle(double x1, double y1, double x2, double y2, int obj, bool prec = true, bool notme = true);

double distance_to_object(int object);
double distance_to_point(double x, double y);

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

void instance_deactivate_region(int rleft, int rtop, int rwidth, int rheight, int inside = true, bool notme = true);
void instance_activate_region(int left, int top, int width, int height, int inside = true);
void instance_deactivate_circle(int x, int y, int r, int inside = true, bool notme = true);
void instance_activate_circle(int x, int y, int r, int inside = true);

void position_change(double x1, double y1, int obj, bool perf);
