////////////////////////////////////
// Collision Util Functions - Functions that are of utility to the main Collision Implementation functions
// . They provide mathematical computations.
////////////////////////////////////

#ifndef POLYGON_COLLISION_UTIL_H
#define POLYGON_COLLISION_UTIL_H

#include "Universal_System/Object_Tiers/collisions_object.h"
#include "Universal_System/Instances/instance_system.h" 
#include "Universal_System/Instances/instance.h"
#include "Universal_System/math_consts.h"
#include "Universal_System/Resources/polygon.h"
#include "Universal_System/Resources/polygon_internal.h"
#include "../General/collisions_general.h"

#include <cmath>
#include <utility>

namespace enigma
{
    // DEFING GLOBAL CONSTANTS FOR CASES
    enum collision_cases {POLYGON_VS_POLYGON, POLYGON_VS_BBOX, BBOX_VS_POLYGON, BBOX_VS_BBOX, POLYGON_VS_PREC, PREC_VS_POLYGON};

    // Projection
    MinMaxProjection getMinMaxProjection(std::vector<glm::vec2>& vecs_box, glm::vec2 axis);
    std::vector<glm::vec2> getEllipseProjectionPoints(double angleOfAxis, double eps_x, double eps_y, double rx, double ry);  

    // Simple Collision
    bool get_polygon_polygon_collision(std::vector<glm::vec2> points_poly1, std::vector<glm::vec2> points_poly2);
    bool get_polygon_ellipse_collision(std::vector<glm::vec2> &points_poly, double x2, double y2, double rx, double ry);
    
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
                                       std::string CASE = "");

    // Interface Collision functions
    bool get_polygon_point_collision(object_collisions* inst, int x1, int y1);
    object_collisions* const get_polygon_bbox_collision(object_collisions* const inst1, object_collisions* const inst2, double x = -1, double y = -1, bool first_one = true);
    bool get_polygon_inst_collision(object_collisions* inst1, object_collisions* inst2, double x1, double y1);

    // BBOX
    Polygon get_bbox_from_dimensions(double x1, double y1, int width, int height);

    // MTV
    double compute_overlap(const MinMaxProjection& pA, const MinMaxProjection& pB);
    glm::vec2 compute_MTV(std::vector<glm::vec2> &points_poly1, std::vector<glm::vec2> &points_poly2);
}
#endif // ~POLYGON_COLLISION_UTIL_H
