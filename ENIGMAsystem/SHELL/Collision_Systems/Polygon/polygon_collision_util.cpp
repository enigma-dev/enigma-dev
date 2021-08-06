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

#include "polygon_collision_util.h"

// -------------------------------------------------------------------------
// Function that returns the Minimum and Maximum
// Projection along an axis of a given normal
// set of points
//
// Args: 
//      vecs_box     -- a vector array of Vector2D points from a polygon normals
//      axis         -- a Vector2D specifying the axis
// Returns:
//      min_max_proj -- MinMaxProjection object that stores minmax information
// -------------------------------------------------------------------------

namespace enigma
{
    MinMaxProjection getMinMaxProjection(std::vector<glm::vec2>& vecs_box, glm::vec2 axis) 
    {
        // Preparing
        double min_proj_box = glm::dot(vecs_box[0], axis);
        double max_proj_box = glm::dot(vecs_box[0], axis);
        int min_dot_box = 0;
        int max_dot_box = 0;

        // Iterating over the points
        for (int j = 1; j < vecs_box.size(); ++j) {
            double current_proj = glm::dot(vecs_box[j], axis);

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
        MinMaxProjection minMaxProjection;
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
    std::vector<glm::vec2> getEllipseProjectionPoints(double angleOfAxis, double eps_x, double eps_y, double rx, double ry) {
        // Using the angle to calculate slew radii.
        // This assumes that the ellipse is perfectly horizontal with zero rotation
        double r_theta = (rx * ry) / sqrt((pow(rx, 2) * pow(sin(angleOfAxis), 2)) + (pow(ry, 2) * pow(cos(angleOfAxis), 2)));

        // Computing the angle of axis from the correct quadrant
        double quad = 1.5708;
        int quadrant = 0;
        if (angleOfAxis > quad && angleOfAxis <= 2 * quad) {
            angleOfAxis = 2 * quad - angleOfAxis;
            quadrant = 1;
        } else if (angleOfAxis > 2 * quad && angleOfAxis <= 3 * quad) {
            angleOfAxis -= 2 * quad;
            quadrant = 2;
        } else if (angleOfAxis > 3 * quad) {
            angleOfAxis = 4 * quad - angleOfAxis;
            quadrant = 3;
        }


        // Using the offsets, the angle, and the new radius to find
        // the points for projections
        double x_component = r_theta * cos(angleOfAxis);
        double y_component = r_theta * sin(angleOfAxis);

        // Adding Offsets according to quadrant
        double min_x, min_y, max_x, max_y;
        switch (quadrant)
        {
        case 0:
        case 2: {
            min_x = eps_x - x_component;
            min_y = eps_y + y_component;
            max_x = eps_x + x_component;
            max_y = eps_y - y_component;
            break;
        }
        case 1:
        case 3: {
            min_x = eps_x - x_component;
            min_y = eps_y - y_component;
            max_x = eps_x + x_component;
            max_y = eps_y + y_component;
            break;
        }
        default:
            break;
        }

        glm::vec2 min_proj_point(min_x, min_y);
        glm::vec2 max_proj_point(max_x, max_y);

        // Finalizing the polygon
        std::vector<glm::vec2> ellipse_points;
        ellipse_points.push_back(min_proj_point);
        ellipse_points.push_back(max_proj_point);

        // Debugging Part Starts
        // printf("min_proj_point = (%f, %f), max_proj_point = (%f, %f)\n", min_proj_point.x, min_proj_point.y, max_proj_point.x, max_proj_point.y);
        // Debugging Part Ends

        return ellipse_points;
    }


    // -------------------------------------------------------------------------
    // Function that returns whether or not two polygons of the two instances are
    // colliding or not
    //
    // Args: 
    //      points_poly1 -- points of first polygon
    //      points_poly2 -- points of second polygon
    // Returns:
    //      bool         -- true if collision otherwise false
    // -------------------------------------------------------------------------
    bool get_polygon_polygon_collision(std::vector<glm::vec2> points_poly1, std::vector<glm::vec2> points_poly2) 
    {
        bool isSeparated = false;

        // Preparing Normals
        std::vector<glm::vec2> normals_poly1 = computeNormals(points_poly1);
        std::vector<glm::vec2> normals_poly2 = computeNormals(points_poly2);

        // Using polygon1 normals to see if there is an axis 
        // on which collision is occuring
        for (int i = 0; i < normals_poly1.size(); ++i) 
        {
            MinMaxProjection result1, result2;

            // Get Min Max Projection of all the points on 
            // this normal vector
            result1 = getMinMaxProjection(points_poly1, normals_poly1[i]);
            result2 = getMinMaxProjection(points_poly2, normals_poly1[i]);

            // Checking Projections for Collision
            isSeparated = result1.max_projection < result2.min_projection || result2.max_projection < result1.min_projection;

            // Break if Separated
            if (isSeparated) 
            {
                break;
            }
        }

        // Using polygon2 normals to see if there is an 
        // axis on which collision is occuring
        if (!isSeparated) 
        {
            for (int i = 0; i < normals_poly2.size(); ++i) 
            {
                MinMaxProjection result1, result2;

                // Get Min Max Projection of all the points on 
                // this normal vector
                result1 = getMinMaxProjection(points_poly1, normals_poly2[i]);
                result2 = getMinMaxProjection(points_poly2, normals_poly2[i]);

                // Checking Projections for Collision
                isSeparated = result1.max_projection < result2.min_projection || result2.max_projection < result1.min_projection;

                // Break if Separated
                if (isSeparated) 
                {
                    break;
                }
            }
        }

        // If there is a single axis where the separation is happening, than the polygons are 
        // not colliding
        if (!isSeparated) 
        {
            // printf("Collision Detected!\n");
            return true;
        } else 
        {
            // printf("No Collision Detected!\n");
            return false;
        }
    }

    // -------------------------------------------------------------------------
    // Function that returns whether or not a polygon and an ellipse are
    // colliding or not
    //
    // Args: 
    //      points_poly -- points of the polygon
    //      x2, y2      -- position of ellipse
    //      rx, ry      -- radii of the ellipse (x-axis and y-axis)
    // Returns:
    //      bool        -- true if collision otherwise false
    // -------------------------------------------------------------------------
    bool get_polygon_ellipse_collision(std::vector<glm::vec2> &points_poly, double x2, double y2, double rx, double ry) 
    {
        bool isSeparated = false;

        // Preparing Normals
        std::vector<glm::vec2> normals_poly = computeNormals(points_poly);

        // Using polygon1 normals
        for (int i = 0; i < normals_poly.size(); ++i) 
        {
            MinMaxProjection result1, result2;

            // First determine the points between which we wish to compute the angle on
            // These points correlate with the normals of the polygons that we computed 
            // above
            glm::vec2 point2 = points_poly[i];
            glm::vec2 point1;

            // The point is either the next one in the list, or the first one
            if (i == normals_poly.size() - 1) 
            {
                point1 = points_poly[0];
            } else 
            {
                point1 = points_poly[i + 1];
            }

            // Computing the angle and using that angle to get the projection of the ellipse
            double angleOfAxis = angleBetweenVectors(point1, point2);
            angleOfAxis -= 1.5708;

            std::vector<glm::vec2> ellipse_points = getEllipseProjectionPoints(angleOfAxis, x2, y2, rx, ry);

            // printf("( %f, %f ) ( %f, %f ) ", point1.x, point1.y, point2.x, point2.y);
            // printf("angleOfAxis = %f ", angleOfAxis);
            // printf("( %f, %f ) ( %f, %f )\n", ellipse_points[0].x, ellipse_points[0].y, ellipse_points[1].x, ellipse_points[1].y);

            // Get Min Max Projection of all the points on this normal vector
            result1 = getMinMaxProjection(points_poly, normals_poly[i]);
            result2 = getMinMaxProjection(ellipse_points, normals_poly[i]);

            // Checking Projections for Collision
            isSeparated = result1.max_projection < result2.min_projection || result2.max_projection < result1.min_projection;

            // Break if Separated
            if (isSeparated) 
            {
                break;
            }
        }
        printf("\n\n");

        if (!isSeparated) 
        {
            return true;
        } 
        else 
        {
            return false;
        }
    }


    // Concave Collision Layer
    bool get_complex_polygon_collision(std::vector<glm::vec2>& points_poly1, 
                                       double offset1_x, double offset1_y, 
                                       double angle1, glm::vec2 pivot1, 
                                       double xscale1, double yscale1, 
                                       bool concave1, std::vector<std::vector<glm::vec2>> subpolygon1,
                                       std::vector<glm::vec2>& points_poly2, 
                                       double offset2_x, double offset2_y, 
                                       double angle2, glm::vec2 pivot2, 
                                       double xscale2, double yscale2, 
                                       bool concave2, std::vector<std::vector<glm::vec2>> subpolygon2,
                                       std::string CASE)
    {
        // If both polygons are not concave than simply transform points and return collision
        if (!concave1 && !concave2)
        {
            // Applying transformations
            enigma::transformPoints(points_poly1, 
                                    offset1_x, offset1_y, 
                                    angle1, pivot1,
                                    xscale1, yscale1);

            enigma::transformPoints(points_poly2, 
                                    offset2_x, offset2_y, 
                                    angle2, pivot2,
                                    xscale2, yscale2);
            
            // Collision function call
            return enigma::get_polygon_polygon_collision(points_poly1, points_poly2);
        }
        // If the first polygon is concave
        else if (concave1 && !concave2)
        {
            // Applying transformations to the second polygon
            enigma::transformPoints(points_poly2, 
                                    offset2_x, offset2_y, 
                                    angle2, pivot2,
                                    xscale2, yscale2);
            
            std::vector<std::vector<glm::vec2>>::iterator it;
            for (it = subpolygon1.begin(); it != subpolygon1.end(); ++it)
            {
                enigma::transformPoints(*it, 
                                    offset1_x, offset1_y, 
                                    angle1, pivot1,
                                    xscale1, yscale1);
                
                if (enigma::get_polygon_polygon_collision(*it, points_poly2))
                    return true;
            }
            return false;
        }
        else if (!concave1 && concave2)
        {
            // Applying transformations to the first polygon
            enigma::transformPoints(points_poly1, 
                                    offset1_x, offset1_y, 
                                    angle1, pivot1,
                                    xscale1, yscale1);

            std::vector<std::vector<glm::vec2>>::iterator it;
            for (it = subpolygon2.begin(); it != subpolygon2.end(); ++it)
            {
                enigma::transformPoints(*it, 
                                    offset2_x, offset2_y, 
                                    angle2, pivot2,
                                    xscale2, yscale2);

                if (enigma::get_polygon_polygon_collision(points_poly1, *it))
                    return true;
            }
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
    Polygon get_bbox_from_dimensions(double x1, double y1, int width, int height) 
    {
        // Creating bbox vectors
        glm::vec2 top_left(x1, y1);
        glm::vec2 top_right(width, y1);
        glm::vec2 bottom_left(x1, height);
        glm::vec2 bottom_right(width, height);

        // Creating bbox polygons
        Polygon bbox_polygon;
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
    //      inst1       -- object collisions for the instance that has a polygon
    //      inst2       -- object collisions for the instance that uses the bbox
    // Returns:
    //      inst2       -- if collision otherwise NULL
    // -------------------------------------------------------------------------
    object_collisions* const get_polygon_bbox_collision(object_collisions* const inst1, object_collisions* const inst2, double x, double y) 
    {
        // polygon vs bbox
        const int collsprite_index2 = inst2->mask_index != -1 ? inst2->mask_index : inst2->sprite_index;
        Sprite& sprite2 = sprites.get(collsprite_index2);

        // Calculating points for bbox
        int w2 = sprite2.width;
        int h2 = sprite2.height;

        // Getting bbox points
        // TODO : optimize this please
        Polygon bbox_polygon = get_bbox_from_dimensions(0, 0, w2, h2);
        std::vector<glm::vec2> bbox_points = bbox_polygon.getPoints();
        offsetPoints(bbox_points, inst2->x, inst2->y);

        // Using parameterized offsets, if passed
        double x1, y1;
        if (x == -1 && y == -1) {
            x1 = inst1->x;
            y1 = inst1->y;
        } else {
            x1 = x;
            y1 = y;
        }

        // Getting polygon points
        std::vector<glm::vec2> points_poly = polygons.get(inst1->polygon_index).getPoints();
        glm::vec2 pivot = polygons.get(inst1->polygon_index).computeCenter();
        transformPoints(points_poly, 
                        x1, y1, 
                        inst1->polygon_angle, pivot,
                        inst1->polygon_xscale, inst1->polygon_yscale);

        // Collision Detection
        return get_polygon_polygon_collision(points_poly, bbox_points)? inst2: NULL;
    }

    // -----------------------------------------------------------------------------
    // Function to detect collision between two instances having polygons
    // 
    // Args:
    //      inst1, inst2    -- instances between which we are detecting collision
    //      x1, y1          -- offset for inst1
    // Returns:
    //                      -- true if collision otherwise false
    // -----------------------------------------------------------------------------
    bool get_polygon_inst_collision(object_collisions* inst1, object_collisions* inst2, double x1, double y1)
    {
        // Fetching Polygon
        enigma::Polygon& poly1 = enigma::polygons.get(inst1->polygon_index);
        enigma::Polygon& poly2 = enigma::polygons.get(inst2->polygon_index);

        // Fetching Points
        std::vector<glm::vec2> points_poly1 = poly1.getPoints();
        std::vector<glm::vec2> points_poly2 = poly2.getPoints();

        // Computing Pivots
        glm::vec2 pivot1 = poly1.computeCenter();
        glm::vec2 pivot2 = poly2.computeCenter();

        return enigma::get_complex_polygon_collision(points_poly1, x1, y1, 
                                                     inst1->polygon_angle, pivot1, inst1->polygon_xscale, inst1->polygon_yscale,
                                                     poly1.isConcave(), poly1.getSubpolygons(), 
                                                     points_poly2, inst2->x, inst2->y, inst2->polygon_angle, pivot2, 
                                                     inst2->polygon_xscale, inst2->polygon_yscale, poly2.isConcave(), poly2.getSubpolygons());
    }

    // -------------------------------------------------------------------------
    // Function to detect collision between a point and a polygon
    // Args:
    //      inst    - instance that has a polygon to detect collision with
    //      x1, y1  - position of the point
    // Returns:
    //              - true if collision otherwise false
    // -------------------------------------------------------------------------
    bool get_polygon_point_collision(object_collisions* inst, int x1, int y1)
    {
        // Converting the point to a small box
        Polygon bbox_main = get_bbox_from_dimensions(0, 0, 0.2, 0.2);

        // Fetching points
        std::vector<glm::vec2> points_poly2 = polygons.get(inst->polygon_index).getPoints();
        std::vector<glm::vec2> bbox_points = bbox_main.getPoints();

        // Applying Transformations
        // Applying Transformations
        glm::vec2 pivot2 = polygons.get(inst->polygon_index).computeCenter();
        transformPoints(points_poly2, 
                                    inst->x, inst->y, 
                                    inst->polygon_angle, pivot2,
                                    inst->polygon_xscale, inst->polygon_yscale);
        offsetPoints(bbox_points, x1, y1);

        // Collision detection
        return get_polygon_polygon_collision(bbox_points, points_poly2) != NULL;
    }

    // --------------------------------------------------------------------------------
    // Function to compute the overlap between two projections. 
    // Args:
    //      pA, pB     -- MinMaxProjection objects between which we are computing the 
    //                    the overlap. The overlap's sign is computed from pA 
    // 
    // Returns:
    //      overlap    -- a double that is the computed overlap between the projections
    // --------------------------------------------------------------------------------
    double compute_overlap(const MinMaxProjection& pA, const MinMaxProjection& pB)
    {
        auto left_overlap = pA.max_projection - pB.min_projection, right_overlap = pB.max_projection - pA.min_projection;
        return left_overlap < right_overlap ? left_overlap : -right_overlap;
    }

    // --------------------------------------------------------------------------------
    // Function to compute the Minimum Translation Vector between two polygon points
    // Args:
    //      points_poly1, points_poly2  -- glm::vec2 arrays containing polygon points
    // 
    // Returns:
    //      smallest_axis               -- the MTV between the two polygons
    // --------------------------------------------------------------------------------
    glm::vec2 compute_MTV(std::vector<glm::vec2> &points_poly1, std::vector<glm::vec2> &points_poly2)
    {
        bool isSeparated = false;
        double overlap = std::numeric_limits<double>::infinity();
        glm::vec2 smallest_axis;

        // Preparing Normals
        std::vector<glm::vec2> normals_poly1 = computeNormals(points_poly1);
        std::vector<glm::vec2> normals_poly2 = computeNormals(points_poly2);

        // Using polygon1 normals to see if there is an axis 
        // on which collision is occuring
        for (int i = 0; i < normals_poly1.size(); ++i) 
        {
            MinMaxProjection result1, result2;

            // Get Min Max Projection of all the points on 
            // this normal vector
            result1 = getMinMaxProjection(points_poly1, normals_poly1[i]);
            result2 = getMinMaxProjection(points_poly2, normals_poly1[i]);

            // Checking Projections for Collision
            isSeparated = result1.max_projection < result2.min_projection || result2.max_projection < result1.min_projection;

            // If no collision than no MTV
            if (isSeparated) 
            {
                return glm::vec2(0, 0);
            } 
            else
            {
                // Compute MTV
                double current_overlap = compute_overlap(result1, result2);
                if (std::abs(current_overlap) < std::abs(overlap))
                {
                    overlap = current_overlap;
                    smallest_axis = normals_poly1[i];
                }
            } 
        }

        // Using polygon2 normals to see if there is an 
        // axis on which collision is occuring
        for (int i = 0; i < normals_poly2.size(); ++i) 
        {
            MinMaxProjection result1, result2;

            // Get Min Max Projection of all the points on 
            // this normal vector
            result1 = getMinMaxProjection(points_poly1, normals_poly2[i]);
            result2 = getMinMaxProjection(points_poly2, normals_poly2[i]);

            // Checking Projections for Collision
            isSeparated = result1.max_projection < result2.min_projection || result2.max_projection < result1.min_projection;

            // If no collision than no MTV
            if (isSeparated) 
            {
                return glm::vec2(0, 0);
            }
            else
            {
                // Compute MTV
                double current_overlap = compute_overlap(result1, result2);
                if (std::abs(current_overlap) < std::abs(overlap))
                {
                    overlap = current_overlap;
                    smallest_axis = normals_poly2[i];
                }
            } 
        }
        // Projecting the overlap
        smallest_axis.x *= overlap;
        smallest_axis.y *= overlap;

        return smallest_axis;
    }
}
