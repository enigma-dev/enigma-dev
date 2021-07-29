/** Copyright (C) 2021 Nabeel Danish  <nabeelben@gmail.com>
*** This file is part of the ENIGMA Development Environment
**/

// -------------------------------------------------------------------------------------------
// General Collisions Functions are found in this file, these functions are used to refactor 
// Code throughout many different collisions systems
// -------------------------------------------------------------------------------------------

#ifndef COLLISION_GENERAL_H
#define COLLISION_GENERAL_H

#include "Universal_System/Object_Tiers/collisions_object.h"
#include "Universal_System/Instances/instance_system.h" 
#include "Universal_System/Instances/instance.h"
#include "Universal_System/math_consts.h"
#include "Universal_System/Resources/polygon.h"
#include "Universal_System/Resources/polygon_internal.h"

#include <cmath>
#include <utility>

// -------------------------------------------------------------------------
// Function to get the bbox border. Not made by Nabeel Danish
// -------------------------------------------------------------------------
static inline void get_border(int *leftv, int *rightv, int *topv, int *bottomv, int left, int top, int right, int bottom, double x, double y, double xscale, double yscale, double angle)
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


// Simple Mathematical functions
template<typename T> static inline T min(T x, T y) { return x < y? x : y; }
template<typename T> static inline T max(T x, T y) { return x > y? x : y; }
static inline double angle_difference(double dir1, double dir2) {
    return fmod((fmod((dir1 - dir2), 360) + 540), 360) - 180;
}
static inline double point_direction(cs_scalar x1, cs_scalar y1,cs_scalar x2, cs_scalar y2) {
    return fmod((atan2(y1 - y2, x2 - x1) * (180 / M_PI)) + 360, 360);
}

namespace enigma
{
    void get_bbox_border(int &left, int &top, int &right, int &bottom, const enigma::object_collisions* inst);
}
#endif // ~COLLISION_GENERAL_H
