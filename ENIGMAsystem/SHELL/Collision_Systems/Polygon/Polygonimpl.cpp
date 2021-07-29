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
// In this case, we treat instances as their polygons.
////////////////////////////////////

// Nabeel Danish

#include "Universal_System/Object_Tiers/collisions_object.h"
#include "Universal_System/Instances/instance_system.h" //iter
#include "Universal_System/Instances/instance.h"
#include "Universal_System/math_consts.h"
#include "Universal_System/Resources/polygon.h"
#include "Universal_System/Resources/polygon_internal.h"
#include "../General/collisions_general.h"

#include "Polygonimpl.h"
#include "polygon_collision_util.h"
#include <cmath>
#include <utility>

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
    int left1, top1, right1, bottom1;
    enigma::get_bbox_border(left1, top1, right1, bottom1, inst1);

    // Iterating over instances in the room to detect collision
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
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
        enigma::collision_cases collision_case = enigma::POLYGON_VS_POLYGON;

        if (inst1->polygon_index != -1 && inst2->polygon_index != -1) 
        {
            collision_case = enigma::POLYGON_VS_POLYGON;
        } 
        else if (inst1->polygon_index != -1 && inst2->polygon_index == -1) 
        {
            collision_case = enigma::POLYGON_VS_BBOX;
        } 
        else if (inst1->polygon_index == -1 && inst2->polygon_index != -1) 
        {
            collision_case = enigma::BBOX_VS_POLYGON;
        } 
        else 
        {
            collision_case = enigma::BBOX_VS_BBOX;
        }
        
        // Getting the Bounding box for the second polygon for sweep and prune check
        int left2, right2, top2, bottom2;
        enigma::get_bbox_border(left2, top2, right2, bottom2, inst2);

        // Main Sweep and Prune Check
        if (left1 <= right2 && left2 <= right1 && top1 <= bottom2 && top2 <= bottom1) {
            if (collision_case == enigma::BBOX_VS_BBOX) 
            {
                return inst2;
            }
        } 
        else {
            continue;
        }

        // Main Collision Detection check
        switch (collision_case)
        {
            case enigma::POLYGON_VS_POLYGON: 
            {
                // Fetching Points
                std::vector<glm::vec2> points_poly1 = enigma::polygons.get(inst1->polygon_index).getPoints();
                std::vector<glm::vec2> points_poly2 = enigma::polygons.get(inst2->polygon_index).getPoints();

                // Computing Pivots
                glm::vec2 pivot1 = enigma::polygons.get(inst1->polygon_index).computeCenter();
                glm::vec2 pivot2 = enigma::polygons.get(inst1->polygon_index).computeCenter();

                // Applying transformations
                enigma::transformPoints(points_poly1, 
                                        inst1->x, inst1->y, 
                                        inst1->polygon_angle, pivot1,
                                        inst1->polygon_xscale, inst1->polygon_yscale);

                enigma::transformPoints(points_poly2, 
                                        inst2->x, inst2->y, 
                                        inst2->polygon_angle, pivot2,
                                        inst2->polygon_xscale, inst2->polygon_yscale);
                
                // Collision function call
                return enigma::get_polygon_polygon_collision(points_poly1, points_poly2)? inst2: NULL;
            }
            case enigma::POLYGON_VS_BBOX: 
            {
                return enigma::get_polygon_bbox_collision(inst1, inst2);
            } 
            case enigma::BBOX_VS_POLYGON: 
            {
                return enigma::get_polygon_bbox_collision(inst2, inst1);
            }
            default:
                break;
        } 
    }
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
    enigma::Polygon bbox_main = enigma::get_bbox_from_dimensions(0, 0, width, height);

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
        enigma::get_bbox_border(left, top, right, bottom, inst);

        // Doing a Sweep and Prune Check
        if (left <= x2 && x1 <= right && top <= y2 && y1 <= bottom) {

            // Only do polygon if prec is true AND we have a polygon.
            if (!prec) {
                return inst;
            }
            if (inst->polygon_index == -1) {
                return inst;
            }

            // Fetching points
            std::vector<glm::vec2> points_poly2 = enigma::polygons.get(inst->polygon_index).getPoints();
            std::vector<glm::vec2> bbox_points = bbox_main.getPoints();

            // Applying Transformations
            glm::vec2 pivot2 = enigma::polygons.get(inst->polygon_index).computeCenter();
            enigma::transformPoints(points_poly2, 
                                        inst->x, inst->y, 
                                        inst->polygon_angle, pivot2,
                                        inst->polygon_xscale, inst->polygon_yscale);
            enigma::offsetPoints(bbox_points, x1, y1);

            // Polygon collision check
            return enigma::get_polygon_polygon_collision(bbox_points, points_poly2)? inst : NULL;
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
        enigma::get_bbox_border(left, top, right, bottom, inst);

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
            else 
            {
                // Converting the line to a polygon
                std::vector<glm::vec2> line_points;
                line_points.push_back(glm::vec2(x1, y1));
                line_points.push_back(glm::vec2(x2, y2));

                // Fetching points
                std::vector<glm::vec2> points_poly2 = enigma::polygons.get(inst->polygon_index).getPoints();

                // Applying Transformations
                // Applying Transformations
                glm::vec2 pivot2 = enigma::polygons.get(inst->polygon_index).computeCenter();
                enigma::transformPoints(points_poly2, 
                                        inst->x, inst->y, 
                                        inst->polygon_angle, pivot2,
                                        inst->polygon_xscale, inst->polygon_yscale);

                // doing a polygon polygon check
                return enigma::get_polygon_polygon_collision(line_points, points_poly2)? inst : NULL;
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
        enigma::get_bbox_border(left, top, right, bottom, inst);

        if (x1 >= left && x1 <= right && y1 >= top && y1 <= bottom) {

            // Only do polygon if prec is true AND the polygon is set.
            if (!prec) {
                return inst;
            } else if (inst->polygon_index == -1) {
                return inst;
            }

            // Collision detection
            return enigma::get_polygon_point_collision(inst, x1, y1)? inst : NULL;
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
    // If wrong arguments return
    if (rx == 0 || ry == 0)
        return 0;

    // Debugging Part Starts
    // printf("Ellipse = (x1 = %d, y1 = %d, rx = %f, ry = %f)\n", x1, y1, rx, ry);
    // Debugging Part Ends

    // Iterate over the instances for the collision check
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

        // Getting Bounding box of the instance
        int left, top, right, bottom;
        enigma::get_bbox_border(left, top, right, bottom, inst);

        // Checking for bounding box collision
        const bool intersects = line_ellipse_intersects(rx, ry, left-x1, top-y1, bottom-y1) ||
                                 line_ellipse_intersects(rx, ry, right-x1, top-y1, bottom-y1) ||
                                 line_ellipse_intersects(ry, rx, top-y1, left-x1, right-x1) ||
                                 line_ellipse_intersects(ry, rx, bottom-y1, left-x1, right-x1) ||
                                 (x1 >= left && x1 <= right && y1 >= top && y1 <= bottom); // Ellipse inside bbox.

        // If the bbox collision is happening, we check for polygon only when
        // prec is True AND there is a polygon for the instance
        if (intersects) {
            if (!prec) {
                return inst;
            }
            if (inst->polygon_index == -1) {
                return inst;
            }
            
            // Actual polygon collision detection
            // Fetching points
            std::vector<glm::vec2> points_poly = enigma::polygons.get(inst->polygon_index).getPoints();

            // Applying Transformations
            // Applying Transformations
            glm::vec2 pivot = enigma::polygons.get(inst->polygon_index).computeCenter();
            enigma::transformPoints(points_poly, 
                                        inst->x, inst->y, 
                                        inst->polygon_angle, pivot,
                                        inst->polygon_xscale, inst->polygon_yscale);

            // Collision Detection
            return enigma::get_polygon_ellipse_collision(points_poly, x1, y1, rx, ry)? inst : NULL;
        }
    }
    return NULL;
}

void destroy_inst_point(int object, bool solid_only, int x1, int y1)
{
    // Iterating over instances
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        // Preliminary checks before collisions
        enigma::object_collisions* const inst = (enigma::object_collisions*) *it;
        if (solid_only && !inst->solid)
            continue;
        if (inst->sprite_index == -1 && inst->mask_index == -1 && inst->polygon_index == -1) //no sprite/mask/polygon then no collision
            continue;

        // Bounding Box retrieval and collision check
        int left, top, right, bottom;
        enigma::get_bbox_border(left, top, right, bottom, inst);

        // Main Sweep and Prune collision check
        if (x1 >= left && x1 <= right && y1 >= top && y1 <= bottom) 
        {
            // Destroy the instance for the bbox check if it does not have a 
            // polygon associated
            if (inst->polygon_index == -1) 
            {
                enigma_user::instance_destroy(inst->id);
            }
            else 
            {
                // Otherwise, check for a polygon collision with this point
                // If Polygon and point colliding, than destroy instance
                if (enigma::get_polygon_point_collision(inst, x1, y1)) 
                {
                    enigma_user::instance_destroy(inst->id);
                }
            }
        }
    }
}

void change_inst_point(int obj, bool perf, int x1, int y1)
{
    // Iterating over instances
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(enigma_user::all); it; ++it)
    {
        // Finding the instance
        enigma::object_collisions* const inst = (enigma::object_collisions*)*it;

        // If no sprite/mask/polygon then no collision
        if (inst->sprite_index == -1 && inst->mask_index == -1 && inst->polygon_index == -1) 
            continue;

        // Computing BBOX for sweep and prune check
        int left, top, right, bottom;
        enigma::get_bbox_border(left, top, right, bottom, inst);

        // Sweep and Prune check
        if (x1 >= left && x1 <= right && y1 >= top && y1 <= bottom) 
        {
            // Change the instance for the bbox check if it does not have a 
            // polygon associated
            if (inst->polygon_index == -1) 
            {
                enigma::instance_change_inst(obj, perf, inst);
            }
            else 
            {
                // Otherwise, check for a polygon collision with this point
                // If Polygon and point colliding, than change instance
                if (enigma::get_polygon_point_collision(inst, x1, y1)) 
                {
                    enigma::instance_change_inst(obj, perf, inst);
                }
            }
        }
    }
}
