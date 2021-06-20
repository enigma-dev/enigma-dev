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
// Collision Util Functions - Functions that are of utility to the main Collision Implementation functions
// . They provide mathematical computations.
////////////////////////////////////

#include "Universal_System/Object_Tiers/collisions_object.h"
#include "Universal_System/Instances/instance_system.h" 
#include "Universal_System/Instances/instance.h"
#include "Universal_System/math_consts.h"
#include "Universal_System/Resources/polygon.h"
#include "Universal_System/Resources/polygon_internal.h"

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
//      vecs_box -- a vector array of Vector2D points from a polygon normals
//      axis     -- a Vector2D specifying the axis
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
// Function that returns the projection points of an ellipse along the angle
// of the axis that is specified. This projection is basically the radius 
// of the ellipse that is skewed by that angle. It returns the two points 
// that intersect the ellipse and the line formed by the radius. These points 
// can then be used for constructing a polygon for minmax projection
//
// Args: 
//      angleOfAxis         -- a double that gives the angle (in radians) from the x-axis 
//                              (NOTE: calculation done on mathematical x-axis)
//      eps_x, eps_y        -- doubles that gives the center coordinate of the ellipse
//      rx, ry              -- doubles that gives the radii of the ellipse (in x and y axis
//                              respectively)
// Returns:
//      ellipse_points      -- vector of Vector2D points containing the line segment that is formed
//                              by the points on the projection
// -------------------------------------------------------------------------
std::vector<enigma::Vector2D> getEllipseProjectionPoints(double angleOfAxis, double eps_x, double eps_y, double rx, double ry) {
    // Using the angle to calculate slew radii.
    // This assumes that the ellipse is perfectly horizontal with zero rotation
    double r_theta = (rx * ry) / ((pow(rx, 2) * pow(sin(angleOfAxis), 2)) + (pow(ry, 2) * pow(cos(angleOfAxis), 2)));

    // Using the offsets, the angle, and the new radius to find
    // the points for projections
    enigma::Vector2D min_proj_point(r_theta * cos(angleOfAxis) - eps_x, r_theta * sin(angleOfAxis) - eps_y);
    enigma::Vector2D max_proj_point(r_theta * cos(angleOfAxis) + eps_x, r_theta * sin(angleOfAxis) + eps_y);

    // Finalizing the polygon
    std::vector<enigma::Vector2D> ellipse_points;
    ellipse_points.push_back(min_proj_point);
    ellipse_points.push_back(max_proj_point);

    return ellipse_points;
}


// -------------------------------------------------------------------------
// Function that returns whether or not two polygons of the two instances are
// colliding or not
//
// Args: 
//      x1, y1      -- position of polygon 1
//      x2, y2      -- position of polygon 2
//      polygon1    -- Polygon object of the first instance
//      polygon2    -- Polygon object of the second instance
// Returns:
//      bool        -- true if collision otherwise false
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
}

// -------------------------------------------------------------------------
// Function that returns whether or not a polygon and an ellipse are
// colliding or not
//
// Args: 
//      x1, y1      -- position of polygon
//      x2, y2      -- position of ellipse
//      polygon     -- Polygon object of the instance
//      rx, ry      -- radii of the ellipse (x-axis and y-axis)
// Returns:
//      bool        -- true if collision otherwise false
// -------------------------------------------------------------------------
bool get_polygon_ellipse_collision(double x1, double y1, double x2, double y2, enigma::Polygon &polygon, double rx, double ry) {
    bool isSeparated = false;
    
    // Preparing Points
    std::vector<enigma::Vector2D> points_poly = polygon.getPoints(x1, y1);

    // Preparing Normals
    std::vector<enigma::Vector2D> normals_poly = polygon.getNorms(x1, y1);

    // Using polygon1 normals
    for (int i = 0; i < normals_poly.size(); ++i) {
        enigma::MinMaxProjection result1, result2;

        // Getting the Ellipse on this axis
        enigma::Vector2D point2 = points_poly[i];
        enigma::Vector2D point1;
        if (i == normals_poly.size() - 1) {
            point1.copy(points_poly[0]);
        } else {
            point1 = points_poly[i + 1];
        }
        double angleOfAxis = -(point1.angleBetween(point2));

        std::vector<enigma::Vector2D> ellipse_points = getEllipseProjectionPoints(angleOfAxis, x2, y2, rx, ry);

        // Get Min Max Projection of all the points on 
        // this normal vector
        result1 = getMinMaxProjection(points_poly, normals_poly[i]);
        result2 = getMinMaxProjection(ellipse_points, normals_poly[i]);

        // Checking Projections for Collision
        isSeparated = result1.max_projection < result2.min_projection || result2.max_projection < result1.min_projection;

        // Break if Separated
        if (isSeparated) {
            break;
        }
    }

    if (!isSeparated) {
        // printf("Collision Detected!\n");
        return true;
    } else {
        // printf("No Collision Detected!\n");
        return false;
    }
}

// -------------------------------------------------------------------------
// Function that makes a polygon from the given bbox dimensions
//
// Args: 
//      x1, y1          -- position of bbox
//      width           -- width of the bbox
//      height          -- height of the bbox
// Returns:
//      bbox_polygon    -- a polygon object made from the parameter values
// -------------------------------------------------------------------------
enigma::Polygon get_bbox_from_dimensions(double x1, double y1, int width, int height) {
    // Creating bbox vectors
    enigma::Vector2D top_left(x1, y1);
    enigma::Vector2D top_right(width, y1);
    enigma::Vector2D bottom_left(x1, height);
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


// -------------------------------------------------------------------------
// Function that returns whether or not a polygon and a bbox are colliding
// or not
//
// Args: 
//      inst1       -- object collisions for the instance that has a polygoo
//      inst2       -- object collisions for the instance that uses the bbox
//      polygon1    -- Polygon object of the first instance
// Returns:
//      inst2       -- if collision otherwise NULL
// -------------------------------------------------------------------------
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


// -------------------------------------------------------------------------
// Function to compute the bbox values used in many collision detection 
// functions
//
// Args: 
//      left, top, right, bottom    -- int values specifying the box, send 
//                                     them as reference since they get 
//                                     updated and returned 
//      inst                        -- instance for which to compute the 
//                                     bbox values
// -------------------------------------------------------------------------
void get_bbox_border(int &left, int &top, int &right, int &bottom, enigma::object_collisions* const inst) {
    const enigma::BoundingBox &box = inst->$bbox_relative();
    const double x = inst->x, y = inst->y,
                    xscale = inst->image_xscale, yscale = inst->image_yscale,
                    ia = inst->image_angle;
    get_border(&left, &right, &top, &bottom, box.left(), box.top(), box.right(), box.bottom(), x, y, xscale, yscale, ia);
}
