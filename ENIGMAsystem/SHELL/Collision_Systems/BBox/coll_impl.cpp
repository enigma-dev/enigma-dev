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
// Collision implementation functions - API dependant implementations of instances colliding with other things.
// In this case, we treat instances as their bounding box (BBox).
////////////////////////////////////

#include "Universal_System/collisions_object.h"
#include "Universal_System/instance_system.h" //iter
#include "Universal_System/instance.h"

#include "coll_util.h"
#include "coll_impl.h"
#include <cmath>

static inline void get_border(int *leftv, int *rightv, int *topv, int *bottomv, int left, int top, int right, int bottom, double x, double y, double xscale, double yscale, double angle)
{
    if (angle == 0)
    {
        const bool xsp = (xscale >= 0), ysp = (yscale >= 0);

        *leftv   = (xsp ? left : right) + x + .5;
        *rightv  = (xsp ? right : left) + x + .5;
        *topv    = (ysp ? top : bottom) + y + .5;
        *bottomv = (ysp ? bottom : top) + y + .5;
    }
    else
    {
        const double arad = angle*(M_PI/180.0);
        const double sina = sin(arad), cosa = cos(arad);
        const int quad = int(fmod(fmod(angle, 360) + 360, 360)/90.0);
        const bool xsp = (xscale >= 0), ysp = (yscale >= 0),
                   q12 = (quad == 1 || quad == 2), q23 = (quad == 2 || quad == 3),
                   xs12 = xsp^q12, sx23 = xsp^q23, ys12 = ysp^q12, ys23 = ysp^q23;

        *leftv   = sina*(xs12 ? left : right) + cosa*(ys23 ? top : bottom) + x + .5;
        *rightv  = sina*(xs12 ? right : left) + cosa*(ys23 ? bottom : top) + x + .5;
        *topv    = cosa*(ys12 ? top : bottom) - sina*(sx23 ? right : left) + y + .5;
        *bottomv = cosa*(ys12 ? bottom : top) - sina*(sx23 ? left : right) + y + .5;
    }
}

static inline int min(int x, int y) { return x<y? x : y; }
static inline double min(double x, double y) { return x<y? x : y; }
static inline int max(int x, int y) { return x>y? x : y; }
static inline double max(double x, double y) { return x>y? x : y; }

enigma::object_collisions* const collide_inst_inst(int object, bool solid_only, bool notme, double x, double y)
{
    enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    const bbox_rect_t &box = inst1->$bbox_relative();
    const double xscale1 = inst1->image_xscale, yscale1 = inst1->image_yscale,
                 ia1 = inst1->image_angle;
    int left1, top1, right1, bottom1;

    get_border(&left1, &right1, &top1, &bottom1, box.left, box.top, box.right, box.bottom, x, y, xscale1, yscale1, ia1);

    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        enigma::object_collisions* const inst2 = (enigma::object_collisions*)*it;
        if (notme && inst2->id == inst1->id)
            continue;
        if (solid_only && !inst2->solid)
            continue;

        const bbox_rect_t &box2 = inst2->$bbox_relative();
        const double x2 = inst2->x, y2 = inst2->y,
                     xscale2 = inst2->image_xscale, yscale2 = inst2->image_yscale,
                     ia1 = inst2->image_angle;
        int left2, top2, right2, bottom2;
        get_border(&left2, &right2, &top2, &bottom2, box2.left, box2.top, box2.right, box2.bottom, x2, y2, xscale2, yscale2, ia1);

        if (left1 <= right2 && left2 <= right1 && top1 <= bottom2 && top2 <= bottom1)
            return inst2;
    }
    return NULL;
}

enigma::object_collisions* const collide_inst_rect(int object, bool solid_only, bool notme, int x1, int y1, int x2, int y2)
{
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        enigma::object_collisions* const inst = (enigma::object_collisions*)*it;
        if (notme && inst->id == enigma::instance_event_iterator->inst->id)
            continue;
        if (solid_only && !inst->solid)
            continue;

        const bbox_rect_t &box = inst->$bbox_relative();
        const double x = inst->x, y = inst->y,
                     xscale = inst->image_xscale, yscale = inst->image_yscale,
                     ia = inst->image_angle;
        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x, y, xscale, yscale, ia);

        if (left <= x2 && x1 <= right && top <= y2 && y1 <= bottom)
            return inst;
    }
    return NULL;
}

enigma::object_collisions* const collide_inst_line(int object, bool solid_only, bool notme, int x1, int y1, int x2, int y2)
{
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        enigma::object_collisions* const inst = (enigma::object_collisions*)*it;
        if (notme && inst->id == enigma::instance_event_iterator->inst->id)
            continue;
        if (solid_only && !inst->solid)
            continue;

        const bbox_rect_t &box = inst->$bbox_relative();
        const double x = inst->x, y = inst->y,
                     xscale = inst->image_xscale, yscale = inst->image_yscale,
                     ia = inst->image_angle;
        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x, y, xscale, yscale, ia);

        double minX = max(min(x1,x2),left);
        double maxX = min(max(x1,x2),right);
        if (minX > maxX)
            continue;

        // Find corresponding min and max Y for min and max X we found before
        double minY = y1;
        double maxY = y2;
        double dx = x2 - x1;

        //do slope check of non vertical lines (dx != 0)
        if ((float)dx)
        {
            double a = (y2 - y1) / dx;
            double b = y1 - a * x1;
            minY = a * minX + b;
            maxY = a * maxX + b;
        }

        if (minY > maxY) //swap
        {
            double tmp = maxY;
            maxY = minY;
            minY = tmp;
        }

        // Find the intersection of the segment's and rectangle's y-projections
        if (maxY > bottom)
            maxY = bottom;
        if (minY < top)
            minY = top;


        if (minY <= maxY) // If Y-projections do not intersect return false
            return inst;
    }
    return NULL;
}

enigma::object_collisions* const collide_inst_point(int object, bool solid_only, bool notme, int x1, int y1)
{
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        enigma::object_collisions* const inst = (enigma::object_collisions*)*it;
        if (notme && inst->id == enigma::instance_event_iterator->inst->id)
            continue;
        if (solid_only && !inst->solid)
            continue;

        const bbox_rect_t &box = inst->$bbox_relative();
        const double x = inst->x, y = inst->y,
                     xscale = inst->image_xscale, yscale = inst->image_yscale,
                     ia = inst->image_angle;
        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x, y, xscale, yscale, ia);

        if (x1 >= left && x1 <= right && y1 >= top && y1 <= bottom)
            return inst;
    }
    return NULL;
}

enigma::object_collisions* const collide_inst_circle(int object, bool solid_only, bool notme, int x1, int y1, double r)
{
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        enigma::object_collisions* const inst = (enigma::object_collisions*)*it;
        if (notme && inst->id == enigma::instance_event_iterator->inst->id)
            continue;
        if (solid_only && !inst->solid)
            continue;

        const bbox_rect_t &box = inst->$bbox_relative();
        const double x = inst->x, y = inst->y,
                     xscale = inst->image_xscale, yscale = inst->image_yscale,
                     ia = inst->image_angle;
        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x, y, xscale, yscale, ia);

        const double distx = x1 - min(max(int(x+.5), left), right),
                     disty = y1 - min(max(int(y+.5), top), bottom);

        if ((distx*distx) + (disty*disty) <= (r*r))
            return inst;
    }
    return NULL;
}

enigma::object_collisions* const collide_inst_ellipse(int object, bool solid_only, bool notme, int x1, int y1, double rx, double ry)
{
    if (rx == 0 || ry == 0)
        return 0;

    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        enigma::object_collisions* const inst = (enigma::object_collisions*)*it;
        if (notme && inst->id == enigma::instance_event_iterator->inst->id)
            continue;
        if (solid_only && !inst->solid)
            continue;

        const bbox_rect_t &box = inst->$bbox_relative();
        const double x = inst->x, y = inst->y,
                     xscale = inst->image_xscale, yscale = inst->image_yscale,
                     ia = inst->image_angle;
        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x, y, xscale, yscale, ia);

        const double distx = (x1 - min(max(int(x+.5), left), right))/rx,
                     disty = (y1 - min(max(int(y+.5), top), bottom))/ry;

        if ((distx*distx) + (disty*disty) <= 1)
            return inst;
    }
    return NULL;
}

void destroy_inst_point(int object, bool solid_only, int x1, int y1)
{
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        enigma::object_collisions* const inst = (enigma::object_collisions*)*it;
        if (solid_only && !inst->solid)
            continue;

        const bbox_rect_t &box = inst->$bbox_relative();
        const double x = inst->x, y = inst->y,
                     xscale = inst->image_xscale, yscale = inst->image_yscale,
                     ia = inst->image_angle;
        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x, y, xscale, yscale, ia);

        if (x1 >= left && x1 <= right && y1 >= top && y1 <= bottom)
            instance_destroy(inst->id);
    }
}
