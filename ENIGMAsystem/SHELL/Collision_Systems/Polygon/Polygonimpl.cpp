/** Copyright (C) 2021 Nabeel Danish
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
// Collision implementation functions - API dependant implementations of instances colliding with other things.
// In this case, we treat instances as their bounding box (BBox).
////////////////////////////////////

// Nabeel Danish

#include "Universal_System/Object_Tiers/collisions_object.h"
#include "Universal_System/Instances/instance_system.h" //iter
#include "Universal_System/Instances/instance.h"
#include "Universal_System/math_consts.h"
#include "Universal_System/Resources/polygon.h"
#include "Universal_System/Resources/polygon_internal.h"

#include "Polygonimpl.h"
#include <cmath>
#include <utility>

// DEFING GLOBAL CONSTANTS FOR CASES
enum collision_cases {POLYGON_VS_POLYGON, POLYGON_VS_BBOX, BBOX_VS_POLYGON, BBOX_VS_BBOX, POLYGON_VS_PREC, PREC_VS_POLYGON};

// Simple Mathematical Functions
template<typename T> static inline T min(T x, T y) { return x < y? x : y; }
template<typename T> static inline T max(T x, T y) { return x > y? x : y; }

// -------------------------------------------------------------------------
// Function that returns the Minimum and Maximum
// Projection along an axis of a given normal
// set of points
//
// Args: 
//      vecs_box -- a vector array of Points from a polygon normals
//      axis     -- a Point specifying the axis
// Returns:
//      min_max_proj -- MinMaxProjection object that stores minmax information
// -------------------------------------------------------------------------
enigma::MinMaxProjection getMinMaxProjection(std::vector<enigma::Vector2D>& vecs_box, enigma::Vector2D axis) {
    
    // Preparing
    double min_proj_box = vecs_box[0].dotProduct(axis);
    double max_proj_box = vecs_box[0].dotProduct(axis);
    int min_dot_box = 0;
    int max_dot_box = 0;

    // Iterating over the points
    for (int j = 1; j < vecs_box.size(); ++j) {
        double current_proj = vecs_box[j].dotProduct(axis);

        // Selecting Min
        if (min_proj_box > current_proj) {
            min_proj_box = current_proj;
            min_dot_box = j;
        }
        // Selecting Max
        if (current_proj > max_proj_box) {
            max_proj_box = current_proj;
            max_dot_box = j;
        }
    }

    // Filling return data
    enigma::MinMaxProjection minMaxProjection;
    minMaxProjection.min_projection = min_proj_box;
    minMaxProjection.max_projection = max_proj_box;
    minMaxProjection.max_index = max_dot_box;
    minMaxProjection.min_index = min_dot_box;

    return minMaxProjection;
}

// -------------------------------------------------------------------------
// Function that returns whether or not two polygons of the two instances are
// colliding or not
//
// Args: 
//      x1, y1      -- position of instance 1
//      x2, y2      -- position of instance 2
//      polygon1    -- Polygon object of the first instance
//      polygon2    -- Polygon object of the second instance
// Returns:
//      inst2       -- object_collision if the collision is happening otherwise NULL
// -------------------------------------------------------------------------
bool get_polygon_polygon_collision(double x1, double y1, double x2, double y2, enigma::Polygon &polygon1, enigma::Polygon &polygon2) {
    bool isSeparated = false;
    
    // Preparing Points
    std::vector<enigma::Vector2D> points_poly1 = polygon1.getPoints(x1, y1);
    std::vector<enigma::Vector2D> points_poly2 = polygon2.getPoints(x2, y2);

    // Preparing Normals
    std::vector<enigma::Vector2D> normals_poly1 = polygon1.getNorms(x1, y1);
    std::vector<enigma::Vector2D> normals_poly2 = polygon2.getNorms(x2, y2);

    // Using polygon1 normals
    for (int i = 0; i < normals_poly1.size(); ++i) {
        enigma::MinMaxProjection result1, result2;

        // Get Min Max Projection of all the points on 
        // this normal vector
        result1 = getMinMaxProjection(points_poly1, normals_poly1[i]);
        result2 = getMinMaxProjection(points_poly2, normals_poly1[i]);

        // Checking Projections for Collision
        isSeparated = result1.max_projection < result2.min_projection || result2.max_projection < result1.min_projection;

        // Break if Separated
        if (isSeparated) {
            break;
        }
    }

    // Using polygon2 normals
    if (!isSeparated) {
        for (int i = 0; i < normals_poly2.size(); ++i) {
            enigma::MinMaxProjection result1, result2;

            // Get Min Max Projection of all the points on 
            // this normal vector
            result1 = getMinMaxProjection(points_poly1, normals_poly2[i]);
            result2 = getMinMaxProjection(points_poly2, normals_poly2[i]);

            // Checking Projections for Collision
            isSeparated = result1.max_projection < result2.min_projection || result2.max_projection < result1.min_projection;

            // Break if Separated
            if (isSeparated) {
                break;
            }
        }
    }

    if (!isSeparated) {
        // printf("Collision Detected!\n");
        return true;
    } else {
        // printf("No Collision Detected!\n");
        return false;
    }

    // Debugging Part Starts
    // printf("normals_poly1:\n");
    // for (int j = 0; j < normals_poly1.size(); ++j) {
    //     printf("( %f, %f )\n", normals_poly1[j].getX(), normals_poly1[j].getY());
    // }
    // printf("normals_poly2:\n");
    // for (int j = 0; j < normals_poly2.size(); ++j) {
    //     printf("( %f, %f )\n", normals_poly2[j].getX(), normals_poly2[j].getY());
    // }
    // Debugging Part Ends
}

enigma::Polygon get_bbox_from_dimensions(double x1, double y1, int width, int height) {
    // Creating bbox vectors
    enigma::Vector2D top_left(0, 0);
    enigma::Vector2D top_right(width, 0);
    enigma::Vector2D bottom_left(0, height);
    enigma::Vector2D bottom_right(width, height);

    // Creating bbox polygons
    enigma::Polygon bbox_polygon;
    bbox_polygon.setWidth(width);
    bbox_polygon.setHeight(height);

    // Adding Polygon points
    bbox_polygon.addPoint(top_left);
    bbox_polygon.addPoint(top_right);
    bbox_polygon.addPoint(bottom_right);
    bbox_polygon.addPoint(bottom_left);

    return bbox_polygon;
}

enigma::object_collisions* const get_polygon_bbox_collision(enigma::object_collisions* const inst1, enigma::object_collisions* const inst2, enigma::Polygon &polygon1) {
    // polygon vs bbox
    const int collsprite_index2 = inst2->mask_index != -1 ? inst2->mask_index : inst2->sprite_index;
    enigma::Sprite& sprite2 = enigma::sprites.get(collsprite_index2);

    // Calculating points for bbox
    int w2 = sprite2.width;
    int h2 = sprite2.height;

    // Getting bbox polygon
    enigma::Polygon bbox_polygon = get_bbox_from_dimensions(0, 0, w2, h2);

    // Collision Detection
    return get_polygon_polygon_collision(inst1->x, inst1->y, inst2->x, inst2->y, polygon1, bbox_polygon)? inst2: NULL;
}

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

void get_bbox_border(int &left, int &top, int &right, int &bottom, enigma::object_collisions* const inst) {
    const enigma::BoundingBox &box = inst->$bbox_relative();
    const double x = inst->x, y = inst->y,
                    xscale = inst->image_xscale, yscale = inst->image_yscale,
                    ia = inst->image_angle;
    get_border(&left, &right, &top, &bottom, box.left(), box.top(), box.right(), box.bottom(), x, y, xscale, yscale, ia);
}

static bool precise_collision_single(int intersection_left, int intersection_right, int intersection_top, int intersection_bottom,
                                double x1, double y1,
                                double xscale1, double yscale1,
                                double ia1,
                                unsigned char* pixels1,
                                int w1, int h1,
                                int xoffset1, int yoffset1)
{

    if (xscale1 != 0.0 && yscale1 != 0.0) {

        const double pi_half = M_PI/2.0;

        const double arad1 = ia1*M_PI/180.0;

        const double cosa1 = cos(-arad1);
        const double sina1 = sin(-arad1);
        const double cosa90_1 = cos(-arad1 + pi_half);
        const double sina90_1 = sin(-arad1 + pi_half);

        for (int rowindex = intersection_top; rowindex <= intersection_bottom; rowindex++)
        {
            for(int colindex = intersection_left; colindex <= intersection_right; colindex++)
            {

                //Test for single image.
                const int bx1 = (colindex - x1);
                const int by1 = (rowindex - y1);
                const int px1 = (int)((bx1*cosa1 + by1*sina1)/xscale1 + xoffset1);
                const int py1 = (int)((bx1*cosa90_1 + by1*sina90_1)/yscale1 + yoffset1);
                const bool p1 = px1 >= 0 && py1 >= 0 && px1 < w1 && py1 < h1 && pixels1[py1*w1 + px1] != 0;

                if (p1) {
                    return true;
                }
            }
        }
    }
    return false;
}

static bool precise_collision_pair(int intersection_left, int intersection_right, int intersection_top, int intersection_bottom,
                                double x1, double y1, double x2, double y2,
                                double xscale1, double yscale1, double xscale2, double yscale2,
                                double ia1, double ia2,
                                unsigned char* pixels1, unsigned char* pixels2,
                                int w1, int h1, int w2, int h2,
                                int xoffset1, int yoffset1, int xoffset2, int yoffset2)
{

    if (xscale1 != 0.0 && yscale1 != 0.0 && xscale2 != 0.0 && yscale2 != 0.0) {

        const double pi_half = M_PI/2.0;

        const double arad1 = ia1*M_PI/180.0;
        const double arad2 = ia2*M_PI/180.0;

        const double cosa1 = cos(-arad1);
        const double sina1 = sin(-arad1);
        const double cosa90_1 = cos(-arad1 + pi_half);
        const double sina90_1 = sin(-arad1 + pi_half);

        const double cosa2 = cos(-arad2);
        const double sina2 = sin(-arad2);
        const double cosa90_2 = cos(-arad2 + pi_half);
        const double sina90_2 = sin(-arad2 + pi_half);

        for (int rowindex = intersection_top; rowindex <= intersection_bottom; rowindex++)
        {
            for(int colindex = intersection_left; colindex <= intersection_right; colindex++)
            {

                // Test for first image.
                const int bx1 = (colindex - x1);
                const int by1 = (rowindex - y1);
                const int px1 = (int)((bx1*cosa1 + by1*sina1)/xscale1 + xoffset1);
                const int py1 = (int)((bx1*cosa90_1 + by1*sina90_1)/yscale1 + yoffset1);
                const bool p1 = px1 >= 0 && py1 >= 0 && px1 < w1 && py1 < h1 && pixels1[py1*w1 + px1] != 0;

                // Test for second image.
                const int bx2 = (colindex - x2);
                const int by2 = (rowindex - y2);
                const int px2 = (int)((bx2*cosa2 + by2*sina2)/xscale2 + xoffset2);
                const int py2 = (int)((bx2*cosa90_2 + by2*sina90_2)/yscale2 + yoffset2);
                const bool p2 = px2 >= 0 && py2 >= 0 && px2 < w2 && py2 < h2 && pixels2[py2*w2 + px2] != 0;

                //Final test.
                if (p1 && p2) {
                    return true;
                }
            }
        }
    }
    return false;
}

// Assumes lx1 != lx2 || ly1 != ly2.
static bool precise_collision_line(int intersection_left, int intersection_right, int intersection_top, int intersection_bottom,
                                double x1, double y1,
                                double xscale1, double yscale1,
                                double ia1,
                                unsigned char* pixels1,
                                int w1, int h1,
                                int xoffset1, int yoffset1,
                                int lx1, int ly1, int lx2, int ly2)
{
    if (xscale1 != 0.0 && yscale1 != 0.0) {

        const double pi_half = M_PI/2.0;

        const double arad1 = ia1*M_PI/180.0;

        const double cosa1 = cos(-arad1);
        const double sina1 = sin(-arad1);
        const double cosa90_1 = cos(-arad1 + pi_half);
        const double sina90_1 = sin(-arad1 + pi_half);

        if (lx1 != lx2 && abs(lx1-lx2) >= abs(ly1-ly2)) { // The slope is defined and in [-1;1].
            const int minX = max(min(lx1, lx2), intersection_left),
                       maxX = min(max(lx1, lx2), intersection_right);

            const double denom = lx2 - lx1;
            for (int gx = minX; gx <= maxX; gx++)
            {
                int gy = (int)round((gx - lx1)*(ly2-ly1)/denom + ly1);
                if (gy < intersection_top || gy > intersection_bottom) {
                    continue;
                }
                // Test for single image.
                const int bx1 = (gx - x1);
                const int by1 = (gy - y1);
                const int px1 = (int)((bx1*cosa1 + by1*sina1)/xscale1 + xoffset1);
                const int py1 = (int)((bx1*cosa90_1 + by1*sina90_1)/yscale1 + yoffset1);
                const bool p1 = px1 >= 0 && py1 >= 0 && px1 < w1 && py1 < h1 && pixels1[py1*w1 + px1] != 0;

                if (p1) {
                    return true;
                }
            }
        }
        else { // ly1 != ly2.
            const int minY = max(min(ly1, ly2), intersection_top),
                       maxY = min(max(ly1, ly2), intersection_bottom);

            const double denom = ly2 - ly1;
            for (int gy = minY; gy <= maxY; gy++)
            {
                int gx = (int)round((gy - ly1)*(lx2-lx1)/denom + lx1);
                if (gx < intersection_left || gx > intersection_right) {
                    continue;
                }
                // Test for single image.
                const int bx1 = (gx - x1);
                const int by1 = (gy - y1);
                const int px1 = (int)((bx1*cosa1 + by1*sina1)/xscale1 + xoffset1);
                const int py1 = (int)((bx1*cosa90_1 + by1*sina90_1)/yscale1 + yoffset1);
                const bool p1 = px1 >= 0 && py1 >= 0 && px1 < w1 && py1 < h1 && pixels1[py1*w1 + px1] != 0;

                if (p1) {
                    return true;
                }
            }
        }
    }
    return false;
}

// rx > 0, ry > 0.
static bool precise_collision_ellipse(int intersection_left, int intersection_right, int intersection_top, int intersection_bottom,
                                double x1, double y1,
                                double xscale1, double yscale1,
                                double ia1,
                                unsigned char* pixels1,
                                int w1, int h1,
                                int xoffset1, int yoffset1,
                                int ex, int ey, int rx, int ry)
{

    if (xscale1 != 0.0 && yscale1 != 0.0) {

        const double pi_half = M_PI/2.0;

        const double arad1 = ia1*M_PI/180.0;

        const double cosa1 = cos(-arad1);
        const double sina1 = sin(-arad1);
        const double cosa90_1 = cos(-arad1 + pi_half);
        const double sina90_1 = sin(-arad1 + pi_half);

        const double rx_2 = rx*rx, ry_2 = ry*ry;

        for (int rowindex = intersection_top; rowindex <= intersection_bottom; rowindex++)
        {
            for(int colindex = intersection_left; colindex <= intersection_right; colindex++)
            {
                const double px = colindex - ex;
                const double py = rowindex - ey;
                if (px*px/rx_2 + py*py/ry_2 > 1.0) continue;

                // Test for single image.
                const int bx1 = (colindex - x1);
                const int by1 = (rowindex - y1);
                const int px1 = (int)((bx1*cosa1 + by1*sina1)/xscale1 + xoffset1);
                const int py1 = (int)((bx1*cosa90_1 + by1*sina90_1)/yscale1 + yoffset1);
                const bool p1 = px1 >= 0 && py1 >= 0 && px1 < w1 && py1 < h1 && pixels1[py1*w1 + px1] != 0;

                if (p1) {
                    return true;
                }
            }
        }
    }
    return false;
}

enigma::object_collisions* const collide_inst_inst(int object, bool solid_only, bool notme, double x, double y)
{
    // Obtain the first Object
    enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);

    // If instance cannot collide with anything, stop.
    if (inst1->sprite_index == -1 && inst1->mask_index == -1 && inst1->polygon_index == -1) {
        return NULL;
    }

    // Getting Bounding Box for first polygon for Sweep and Prune
    const enigma::BoundingBox &box = inst1->$bbox_relative();
    const double xscale1 = inst1->image_xscale, yscale1 = inst1->image_yscale,
                 ia1 = inst1->image_angle;
    int left1, top1, right1, bottom1;
    get_border(&left1, &right1, &top1, &bottom1, box.left(), box.top(), box.right(), box.bottom(), x, y, xscale1, yscale1, ia1);

    // Debugging Part Starts
    // printf("-----------------------------------------------------------------------------\n");
    // printf("collide_inst_inst: object = %d, solid_only = %d, notme = %d, x = %d, y = %d\n", object, solid_only, notme, x, y);
    // printf("inst1->polygon_index = %d\n", inst1->polygon_index);
    // printf("size = %d\n", polygon1.getNumPoints());
    // Debugging Part Ends

    int num_instances = 0;

    // Iterating over instances in the room to detect collision
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        ++num_instances;

        // Selecting the instance
        enigma::object_collisions* const inst2 = (enigma::object_collisions*)*it;

        // Initial Checks
        if (notme && inst2->id == inst1->id)
            continue;
        if (solid_only && !inst2->solid)
            continue;
        if (inst2->sprite_index == -1 && inst2->mask_index == -1 && inst2->polygon_index == -1) //no sprite/mask/polygon then no collision
            continue;

        // Finding the Collision Type
        collision_cases collision_case = POLYGON_VS_POLYGON;

        if (inst1->polygon_index != -1 && inst2->polygon_index != -1) {
            collision_case = POLYGON_VS_POLYGON;
        } else if (inst1->polygon_index != -1 && inst2->polygon_index == -1) {
            collision_case = POLYGON_VS_BBOX;
        } else if (inst1->polygon_index == -1 && inst2->polygon_index != -1) {
            collision_case = BBOX_VS_POLYGON;
        } else {
            collision_case = BBOX_VS_BBOX;
        }
        
        // Getting the Bounding box for the second polygon for sweep and prune check
        int left2, right2, top2, bottom2;
        get_bbox_border(left2, right2, top2, bottom2, inst2);

        // Main Sweep and Prune Check
        if (left1 <= right2 && left2 <= right1 && top1 <= bottom2 && top2 <= bottom1) {
            // printf("Bounding Box Collision is deteted!");
            if (collision_case == BBOX_VS_BBOX) {
                return inst2;
            }
        } else {
            // printf("Collision Check Pruned!\n");
            continue;
        }

        // Main Collision Detection check
        switch (collision_case)
        {
        case POLYGON_VS_POLYGON: {
            enigma::Polygon& polygon1(enigma::polygons.get(inst1->polygon_index));
            enigma::Polygon& polygon2 = enigma::polygons.get(inst2->polygon_index);
            return get_polygon_polygon_collision(inst1->x, inst1->y, inst2->x, inst2->y, polygon1, polygon2)? inst2: NULL;
        }
        case POLYGON_VS_BBOX: {
            enigma::Polygon& polygon1(enigma::polygons.get(inst1->polygon_index));
            return get_polygon_bbox_collision(inst1, inst2, polygon1);
        } case BBOX_VS_POLYGON: {
            enigma::Polygon& polygon2(enigma::polygons.get(inst2->polygon_index));
            return get_polygon_bbox_collision(inst2, inst1, polygon2);
        }
        default:
            break;
        } 
    }
    // printf("num instances = %d\n", num_instances);
    return NULL;
}

enigma::object_collisions* const collide_inst_rect(int object, bool solid_only, bool prec, bool notme, int x1, int y1, int x2, int y2)
{
    // Swapping incorrectly sent values
    if (x1 > x2)
        std::swap(x1, x2);  
    if (y1 > y2)
        std::swap(y1, y2);

    // Calculating width and height
    int width = x2 - x1;
    int height = y2 - y1;

    // Creating a polygon for the region
    enigma::Polygon bbox_main = get_bbox_from_dimensions(0, 0, width, height);

    // Iterating over instances to find any object that is colliding with
    // this rectangle
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        // Getting the instance
        enigma::object_collisions* const inst = (enigma::object_collisions*)*it;

        // Preliminary checks for collision
        if (notme && inst->id == enigma::instance_event_iterator->inst->id)
            continue;
        if (solid_only && !inst->solid)
            continue;
         if (inst->sprite_index == -1 && inst->mask_index == -1 && inst->polygon_index == -1) //no sprite/mask/polygon then no collision
            continue;

        // Getting the bounding box for the instance 
        int left, top, right, bottom;
        get_bbox_border(left, top, right, bottom, inst);

        // Doing a Sweep and Prune Check
        if (left <= x2 && x1 <= right && top <= y2 && y1 <= bottom) {

            // Only do polygon if prec is true AND we have a polygon.
            if (!prec) {
                return inst;
            }
            if (inst->polygon_index == -1) {
                return inst;
            }

            // Polygon collision check
            enigma::Polygon polygon2 = enigma::polygons.get(inst->polygon_index);
            return get_polygon_polygon_collision(x1, y1, inst->x, inst->y, bbox_main, polygon2)? inst : NULL;
        }
    }
    return NULL;
}

enigma::object_collisions* const collide_inst_line(int object, bool solid_only, bool prec, bool notme, int x1, int y1, int x2, int y2)
{
    // Ensure x1 != x2 || y1 != y2.
    if (x1 == x2 && y1 == y2)
        return collide_inst_point(object, solid_only, prec, notme, x1, y1);

    // Iterating over instances 
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        // Retrieving the instance
        enigma::object_collisions* const inst = (enigma::object_collisions*)*it;

        // Preliminary checks
        if (notme && inst->id == enigma::instance_event_iterator->inst->id)
            continue;
        if (solid_only && !inst->solid)
            continue;
        if (inst->sprite_index == -1 && inst->mask_index == -1 && inst->polygon_index == -1) // No sprite/mask/polygon then no collision.
            continue;

        // Getting the Bounding box for instance
        int left, top, right, bottom;
        get_bbox_border(left, top, right, bottom, inst);

        // Doing a Sweep and Prune Check using bounding box and the 
        // Line. Using line projections.
        double minX = max(min(x1,x2),left);
        double maxX = min(max(x1,x2),right);
        if (minX > maxX)
            continue;

        // Find corresponding min and max Y for min and max X we found before.
        double minY = y1;
        double maxY = y2;
        double dx = x2 - x1;

        // Do slope check of non vertical lines (dx != 0).
        if ((float)dx)
        {
            double a = (y2 - y1) / dx;
            double b = y1 - a * x1;
            minY = a * minX + b;
            maxY = a * maxX + b;
        }

        if (minY > maxY) // Swap.
        {
            double tmp = maxY;
            maxY = minY;
            minY = tmp;
        }

        // Find the intersection of the segment's and rectangle's y-projections.
        if (maxY > bottom)
            maxY = bottom;
        if (minY < top)
            minY = top;

        if (minY <= maxY) { 
            // If Y-projections do not intersect then no collision.
            // At this point, the line segment intersects the bounding box.

            // We do a further polygon check if both the prec is true AND 
            // we have a polygon for this sprite
            if (!prec) {
                return inst;
            } else if (inst->polygon_index == -1) {
                return inst;
            }
            else {
                // Converting the line to a polygon
                int width = x2 - x1;
                int height = y2 - y1;

                enigma::Vector2D point1(0, 0);
                enigma::Vector2D point2(width, height);

                enigma::Polygon polygon1;
                polygon1.setWidth(width);
                polygon1.setHeight(height);
                polygon1.addPoint(point1);
                polygon1.addPoint(point2);

                // doing a polygon polygon check
                enigma::Polygon polygon2 = enigma::polygons.get(inst->polygon_index);

                return get_polygon_polygon_collision(x1, y1, inst->x, inst->y, polygon1, polygon2)? inst : NULL;
            }
        }
    }
    return NULL;
}

enigma::object_collisions* const collide_inst_point(int object, bool solid_only, bool prec, bool notme, int x1, int y1)
{
    // Iterating over the instances to detect collision
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        // Retrieving the instance
        enigma::object_collisions* const inst = (enigma::object_collisions*)*it;

        // Doing some Preliminary Checks
        if (notme && inst->id == enigma::instance_event_iterator->inst->id)
            continue;
        if (solid_only && !inst->solid)
            continue;
        if (inst->sprite_index == -1 && inst->mask_index == -1 && inst->polygon_index == -1) //no sprite/mask/polygon then no collision
            continue;

        // Finding the Bounding box of the instance
        int left, top, right, bottom;
        get_bbox_border(left, top, right, bottom, inst);

        if (x1 >= left && x1 <= right && y1 >= top && y1 <= bottom) {

            // Only do polygon if prec is true AND the polygon is set.
            if (!prec) {
                return inst;
            } else if (inst->polygon_index == -1) {
                return inst;
            }

            // Converting the point to a small box
            enigma::Polygon polygon1 = get_bbox_from_dimensions(0, 0, 1, 1);

            // Collision detection
            enigma::Polygon polygon2 = enigma::polygons.get(inst->polygon_index);

            return get_polygon_polygon_collision(x1, y1, inst->x, inst->y, polygon1, polygon2)? inst : NULL;
        }
    }
    return NULL;
}

enigma::object_collisions* const collide_inst_circle(int object, bool solid_only, bool prec, bool notme, int x1, int y1, double r)
{
    return collide_inst_ellipse(object, solid_only, prec, notme, x1, y1, r, r);
}

static bool line_ellipse_intersects(double rx, double ry, double x, double ly1, double ly2)
{
    // Formula: x^2/a^2 + y^2/b^2 = 1   <=>   y = +/- sqrt(b^2*(1 - x^2/a^2))

    const double inner = ry*ry*(1 - x*x/(rx*rx));
    if (inner < 0) {
        return false;
    }
    else {
        const double y1 = -sqrt(inner), y2 = sqrt(inner);
        return y1 <= ly2 && ly1 <= y2;
    }
}

enigma::object_collisions* const collide_inst_ellipse(int object, bool solid_only, bool prec, bool notme, int x1, int y1, double rx, double ry)
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
        if (inst->sprite_index == -1 && inst->mask_index == -1) // No sprite/mask then no collision.
            continue;

        const enigma::BoundingBox &box = inst->$bbox_relative();
        const double x = inst->x, y = inst->y,
                     xscale = inst->image_xscale, yscale = inst->image_yscale,
                     ia = inst->image_angle;
        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left(), box.top(), box.right(), box.bottom(), x, y, xscale, yscale, ia);

        const bool intersects = line_ellipse_intersects(rx, ry, left-x1, top-y1, bottom-y1) ||
                                 line_ellipse_intersects(rx, ry, right-x1, top-y1, bottom-y1) ||
                                 line_ellipse_intersects(ry, rx, top-y1, left-x1, right-x1) ||
                                 line_ellipse_intersects(ry, rx, bottom-y1, left-x1, right-x1) ||
                                 (x1 >= left && x1 <= right && y1 >= top && y1 <= bottom); // Ellipse inside bbox.

        if (intersects) {

            if (!prec) {
                return inst;
            }

            const int collsprite_index = inst->mask_index != -1 ? inst->mask_index : inst->sprite_index;

            enigma::Sprite& sprite = enigma::sprites.get(collsprite_index);

            const int usi = ((int) inst->image_index) % sprite.SubimageCount();

            unsigned char* pixels = (unsigned char*) (sprite.GetSubimage(usi).collisionData);

            if (pixels == 0) { // Bounding Box.
                return inst;
            }
            else { // Precise.
                // Intersection.
                const int ins_left = max(left, (int)(x1-rx));
                const int ins_right = min(right, (int)(x1+rx));
                const int ins_top = max(top, (int)(y1-ry));
                const int ins_bottom = min(bottom, (int)(y1+ry));

                // Check per pixel.

                const int w = sprite.width;
                const int h = sprite.height;

                const double xoffset = sprite.xoffset;
                const double yoffset = sprite.yoffset;

                const bool coll_result = precise_collision_ellipse(
                    ins_left, ins_right, ins_top, ins_bottom,
                    x, y,
                    xscale, yscale,
                    ia,
                    pixels,
                    w, h,
                    xoffset, yoffset,
                    x1, y1, rx, ry
                );

                if (coll_result) {
                    return inst;
                }
            }
        }
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
        if (inst->sprite_index == -1 && inst->mask_index == -1) //no sprite/mask then no collision
            continue;

        const enigma::BoundingBox &box = inst->$bbox_relative();
        const double x = inst->x, y = inst->y,
                     xscale = inst->image_xscale, yscale = inst->image_yscale,
                     ia = inst->image_angle;
        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left(), box.top(), box.right(), box.bottom(), x, y, xscale, yscale, ia);

        if (x1 >= left && x1 <= right && y1 >= top && y1 <= bottom) {

            const int collsprite_index = inst->mask_index != -1 ? inst->mask_index : inst->sprite_index;

            enigma::Sprite& sprite = enigma::sprites.get(collsprite_index);

            const int usi = ((int) inst->image_index) % sprite.SubimageCount();

            unsigned char* pixels = (unsigned char*) (sprite.GetSubimage(usi).collisionData);

            if (pixels == 0) { //bbox.
                enigma_user::instance_destroy(inst->id);
            }
            else { //precise.
                //Intersection.
                const int ins_left = max(left, x1);
                const int ins_right = min(right, x1);
                const int ins_top = max(top, y1);
                const int ins_bottom = min(bottom, y1);

                //Check per pixel.

                const int w = sprite.width;
                const int h = sprite.height;

                const double xoffset = sprite.xoffset;
                const double yoffset = sprite.yoffset;

                const bool coll_result = precise_collision_single(
                    ins_left, ins_right, ins_top, ins_bottom,
                    x, y,
                    xscale, yscale,
                    ia,
                    pixels,
                    w, h,
                    xoffset, yoffset
                );

                if (coll_result) {
                    enigma_user::instance_destroy(inst->id);
                }
            }
        }
    }
}

void change_inst_point(int obj, bool perf, int x1, int y1)
{
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(enigma_user::all); it; ++it)
    {
        enigma::object_collisions* const inst = (enigma::object_collisions*)*it;
        if (inst->sprite_index == -1 && inst->mask_index == -1) //no sprite/mask then no collision
            continue;

        const enigma::BoundingBox &box = inst->$bbox_relative();
        const double x = inst->x, y = inst->y,
                     xscale = inst->image_xscale, yscale = inst->image_yscale,
                     ia = inst->image_angle;
        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left(), box.top(), box.right(), box.bottom(), x, y, xscale, yscale, ia);

        if (x1 >= left && x1 <= right && y1 >= top && y1 <= bottom) {

            const int collsprite_index = inst->mask_index != -1 ? inst->mask_index : inst->sprite_index;

            enigma::Sprite& sprite = enigma::sprites.get(collsprite_index);

            const int usi = ((int) inst->image_index) % sprite.SubimageCount();

            unsigned char* pixels = (unsigned char*) (sprite.GetSubimage(usi).collisionData);

            if (pixels == 0) { //bbox.
                enigma::instance_change_inst(obj, perf, inst);
            }
            else { //precise.
                //Intersection.
                const int ins_left = max(left, x1);
                const int ins_right = min(right, x1);
                const int ins_top = max(top, y1);
                const int ins_bottom = min(bottom, y1);

                //Check per pixel.

                const int w = sprite.width;
                const int h = sprite.height;

                const double xoffset = sprite.xoffset;
                const double yoffset = sprite.yoffset;

                const bool coll_result = precise_collision_single(
                    ins_left, ins_right, ins_top, ins_bottom,
                    x, y,
                    xscale, yscale,
                    ia,
                    pixels,
                    w, h,
                    xoffset, yoffset
                );

                if (coll_result) {
                    enigma::instance_change_inst(obj, perf, inst);
                }
            }
        }
    }
}
