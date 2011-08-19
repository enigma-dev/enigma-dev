/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Polygone                                                 **
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
#include "collisions_object.h"
//#include "instance_system.h"
#include <cmath>
#include "path_functions.h"

extern bool place_meeting(double x, double y, int object);
extern bool place_free(double x, double y);
extern double min(double x, double y);
extern double max(double x, double y);

class mp_potential
{
    public:
    double maxrot, rotstep, ahead;
    bool onspot;
    mp_potential() {maxrot = 30; rotstep = 10; ahead = 3; onspot = true;}
    ~mp_potential() {}
}
mp_potential_global;

void mp_potential_settings(double maxrot, double rotstep, double ahead, double onspot)
{
    mp_potential_global.maxrot = max(0, min(180, maxrot));
    mp_potential_global.rotstep = rotstep;
    mp_potential_global.ahead = ahead;
    mp_potential_global.onspot = onspot;
}

bool mp_potential_step_object(const double x, const double y, const double stepsize, const int object, const bool solid_only = false)
{
    enigma::object_collisions* const inst = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    if (inst->x == x && inst->y == y)
        return true;

    double dir, xstep, ystep, goaldir;
    goaldir = atan2(inst->y - y, x - inst->x)*(180/M_PI);
    dir = (goaldir + 360) %(variant) 360;

    //direct goal direction
    if (abs((dir - inst->direction + 540) %(variant) 360 - 180) <= mp_potential_global.maxrot)
    {
        if (hypot(x - inst->x, y - inst->y) <= stepsize)
        {
            if (solid_only ? (place_free(x, y)) : (!place_meeting(x, y, object)))
            {
                inst->direction = dir;
                inst->x = x;
                inst->y = y;
                return true;
            }
            if (mp_potential_global.onspot)
            {
                inst->direction += min(mp_potential_global.maxrot, max(-mp_potential_global.maxrot, (goaldir - inst->direction + 540) %(variant) 360 - 180));
            }
            return false;
        }
        xstep = cos(dir*M_PI/180)*stepsize;
        ystep = -sin(dir*M_PI/180)*stepsize;
        if (solid_only ? (place_free(inst->x + mp_potential_global.ahead*xstep, y + mp_potential_global.ahead*ystep) && place_free(inst->x + xstep,inst->y + ystep))
                       : (!place_meeting(inst->x + mp_potential_global.ahead*xstep, y + mp_potential_global.ahead*ystep, object) && !place_meeting(inst->x + xstep, inst->y + ystep, object)))
        {
            inst->direction = dir;
            inst->x += xstep;
            inst->y += ystep;
            return true;
        }
    }

    //alternate either side of the goal direction full circle
    for (int i = mp_potential_global.rotstep; i < 180; i += mp_potential_global.rotstep)
    {
        dir = (goaldir - i + 360) %(variant) 360;
        if (abs((dir - inst->direction + 540) %(variant) 360 - 180) <= mp_potential_global.maxrot)
        {
            xstep = cos(dir*M_PI/180)*stepsize;
            ystep = -sin(dir*M_PI/180)*stepsize;
            if (solid_only ? (place_free(inst->x + mp_potential_global.ahead*xstep, y + mp_potential_global.ahead*ystep) && place_free(inst->x + xstep,inst->y + ystep))
                           : (!place_meeting(inst->x + mp_potential_global.ahead*xstep, y + mp_potential_global.ahead*ystep, object) && !place_meeting(inst->x + xstep, inst->y + ystep, object)))
            {
                inst->direction = dir;
                inst->x += xstep;
                inst->y += ystep;
                return true;
            }
        }
        dir = (goaldir + i + 360) %(variant) 360;
        if (abs((dir - inst->direction + 540) %(variant) 360 - 180) <= mp_potential_global.maxrot)
        {
            xstep = cos(dir*M_PI/180)*stepsize;
            ystep = -sin(dir*M_PI/180)*stepsize;
            if (solid_only ? (place_free(inst->x + mp_potential_global.ahead*xstep, inst->y + mp_potential_global.ahead*ystep) && place_free(inst->x + xstep, inst->y + ystep))
                           : (!place_meeting(inst->x + mp_potential_global.ahead*xstep, inst->y + mp_potential_global.ahead*ystep, object) && !place_meeting(inst->x + xstep, inst->y + ystep, object)))
            {
                inst->direction = dir;
                inst->x += xstep;
                inst->y += ystep;
                return true;
            }
        }
    }
    if (mp_potential_global.onspot)
    {
        inst->direction += min(mp_potential_global.maxrot, max(-mp_potential_global.maxrot, (goaldir - inst->direction + 540) %(variant) 360 - 180));
    }
    return false;
}

inline bool mp_potential_step(const double x, const double y, const double stepsize, const bool checkall)
{
    return mp_potential_step_object(x, y, stepsize, all, checkall ? false : true);
}

bool mp_potential_path_object(int path, const double x, const double y, const double stepsize, double factor, const int object, const bool solid_only = false)
{
    enigma::object_collisions* const inst = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    double pathpx = inst->x, pathpy = inst->y, pathpdir = inst->direction;
    path_clear_points(path);
    path_add_point(path, pathpx, pathpy, 100);
    if (pathpx == x && pathpy == y)
        return true;

    if (factor < 1)
        return false;

    double dir, xstep, ystep, goaldir;
    bool pathfound;
    const double max_dist = factor*hypot(x - pathpx, y - pathpy);
    while (path_get_length(path) < max_dist)
    {
        goaldir = atan2(pathpy - y, x - pathpx)*(180/M_PI);
        pathfound = false;

        //direct goal direction
        dir = (goaldir + 360) %(variant) 360;
        if (abs((dir - pathpdir + 540) %(variant) 360 - 180) <= mp_potential_global.maxrot)
        {
            if (hypot(x - pathpx, y - pathpy) <= stepsize)
            {
                if (solid_only ? (place_free(x, y)) : (!place_meeting(x, y, object)))
                {
                   path_add_point(path, x, y, 100);
                   return true;
                }
                return false;
            }
            xstep = cos(dir*M_PI/180)*stepsize;
            ystep = -sin(dir*M_PI/180)*stepsize;
            if (solid_only ? (place_free(pathpx + mp_potential_global.ahead*xstep, pathpy + mp_potential_global.ahead*ystep) && place_free(pathpx + xstep, pathpy + ystep))
                           : (!place_meeting(pathpx + mp_potential_global.ahead*xstep, pathpy + mp_potential_global.ahead*ystep, object) && !place_meeting(pathpx + xstep, pathpy + ystep, object)))
            {
                pathpdir = dir;
                pathpx += xstep;
                pathpy += ystep;
                path_add_point(path, pathpx, pathpy, 100);
                pathfound = true;
            }
        }

        if (!pathfound)
        {
            //alternate either side of the goal direction full circle
            for (int i = mp_potential_global.rotstep; i < 180; i += mp_potential_global.rotstep)
            {
                dir = (goaldir - i + 360) %(variant) 360;
                if (abs((dir - pathpdir + 540) %(variant) 360 - 180) <= mp_potential_global.maxrot)
                {
                    xstep = cos(dir*M_PI/180)*stepsize;
                    ystep = -sin(dir*M_PI/180)*stepsize;
                    if (solid_only ? (place_free(pathpx + mp_potential_global.ahead*xstep, pathpy + mp_potential_global.ahead*ystep) && place_free(pathpx + xstep, pathpy + ystep))
                                   : (!place_meeting(pathpx + mp_potential_global.ahead*xstep, pathpy + mp_potential_global.ahead*ystep, object) && !place_meeting(pathpx + xstep, pathpy + ystep, object)))
                    {
                        pathpdir = dir;
                        pathpx += xstep;
                        pathpy += ystep;
                        path_add_point(path, pathpx, pathpy, 100);
                        pathfound = true;
                        break;
                    }
                }
                dir = (goaldir + i + 360) %(variant) 360;
                if (abs((dir - pathpdir + 540) %(variant) 360 - 180) <= mp_potential_global.maxrot)
                {
                    xstep = cos(dir*M_PI/180)*stepsize;
                    ystep = -sin(dir*M_PI/180)*stepsize;
                    if (solid_only ? (place_free(pathpx + mp_potential_global.ahead*xstep, pathpy + mp_potential_global.ahead*ystep) && place_free(pathpx + xstep, pathpy + ystep))
                                   : (!place_meeting(pathpx + mp_potential_global.ahead*xstep, pathpy + mp_potential_global.ahead*ystep, object) && !place_meeting(pathpx + xstep, pathpy + ystep, object)))
                    {
                        pathpdir = dir;
                        pathpx += xstep;
                        pathpy += ystep;
                        path_add_point(path, pathpx, pathpy, 100);
                        pathfound = true;
                        break;
                    }
                }
            }
            if (!pathfound)
                return false;
        }
    }
    return false;
}

inline bool mp_potential_path(int path, const double x, const double y, const double stepsize, double factor, const bool checkall, const bool solid_only = false)
{
    return mp_potential_path_object(path, x, y, stepsize, factor, all, checkall ? false : true);
}

bool mp_linear_step_object(const double x, const double y, const double stepsize, const int object, const bool solid_only = false)
{
    enigma::object_collisions* const inst = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    if (inst->x == x && inst->y == y)
        return true;

    inst->direction = atan2(inst->y - y, x - inst->x)*(180/M_PI);
    if (hypot(x - inst->x, y - inst->y) <= stepsize)
    {
        if (solid_only ? (place_free(x, y)) : (!place_meeting(x, y, object)))
        {
            inst->x = x;
            inst->y = y;
            return true;
        }
        return false;
    }

    double xstep, ystep;
    xstep = cos(inst->direction*M_PI/180)*stepsize;
    ystep = -sin(inst->direction*M_PI/180)*stepsize;
    if (solid_only ? (place_free(inst->x + xstep, inst->y + ystep)) : (!place_meeting(inst->x + xstep, inst->y + ystep, object)))
    {
        inst->x += xstep;
        inst->y += ystep;
        return true;
    }
    return false;
}

inline bool mp_linear_step(const double x, const double y, const double stepsize, const bool checkall)
{
    return mp_linear_step_object(x, y, stepsize, all, checkall ? false : true);
}

bool mp_linear_path_object(int path, const double x, const double y, const double stepsize, const int object, const bool solid_only = false)
{
    enigma::object_collisions* const inst = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    double pathpx = inst->x, pathpy = inst->y, pathpdir = atan2(pathpy - y, x - pathpx)*(180/M_PI);
    path_clear_points(path);
    path_add_point(path, pathpx, pathpy, 100);
    if (pathpx == x && pathpy == y)
        return true;

    double xstep, ystep;
    bool pathfound;
    const double max_dist = hypot(x - pathpx, y - pathpy);
    while (path_get_length(path) < max_dist)
    {
        pathfound = false;
        if (hypot(x - pathpx, y - pathpy) <= stepsize)
        {
            if (solid_only ? (place_free(x, y)) : (!place_meeting(x, y, object)))
            {
                path_add_point(path, x, y, 100);
                return true;
            }
            return false;
        }

        xstep = cos(pathpdir*M_PI/180)*stepsize;
        ystep = -sin(pathpdir*M_PI/180)*stepsize;
        if (solid_only ? (place_free(pathpx + xstep, pathpy + ystep)) : (!place_meeting(pathpx + xstep, pathpy + ystep, object)))
        {
            pathpx += xstep;
            pathpy += ystep;
            path_add_point(path, pathpx, pathpy, 100);
            pathfound = true;
        }
        if (!pathfound)
            return false;
    }
    return false;
}

inline bool mp_linear_path(int path, const double x, const double y, const double stepsize, const bool checkall)
{
    return mp_linear_path_object(path, x, y, stepsize, all, checkall ? false : true);
}
