/** Copyright (C) 2021 Nabeel Danish  <nabeelben@gmail.com>
*** This file is part of the ENIGMA Development Environment
**/

#include "collisions_general.h"

namespace enigma
{
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
    void get_bbox_border(int &left, int &top, int &right, int &bottom, 
        const enigma::object_collisions* inst, double x, double y) 
    {
        // Compute the bbox from the polygon 
        // TODO (Nabeel) : Merge this IF condition when the polygon collision system 
        //                  is merged
        if (inst->polygon_index != -1)
        {
            // Using parameterized points if sent
            double x1, y1;
            if (x == -1 && y == -1)
            {
                x1 = inst->x;
                y1 = inst->y;
            } else {
                x1 = x;
                y1 = y;
            }

            // Fetching transformed points
            enigma::Polygon poly = enigma::polygons.get(inst->polygon_index);
            std::vector<glm::vec2> points = poly.getOffsetPoints();
            glm::vec2 pivot(0, 0);
            enigma::transformPoints(points, 
                                    x1, y1, 
                                    inst->polygon_angle, pivot,
                                    inst->polygon_xscale, inst->polygon_yscale);
            
            // Computing bbox
            enigma::BoundingBox box = enigma::computeBBOXFromPoints(points);
            left = box.left();
            top = box.top();
            right = box.right();
            bottom = box.bottom();
        }
        // If the polygon is not availble, the bbox is computed from the polygon
        else if (inst->sprite_index != -1)
        {
            const enigma::BoundingBox &box = inst->$bbox_relative();
            const double x = inst->x, y = inst->y,
                            xscale = inst->image_xscale, yscale = inst->image_yscale,
                            ia = inst->image_angle;
            get_border(&left, &right, &top, &bottom, box.left(), box.top(), box.right(), box.bottom(), x, y, xscale, yscale, ia);
        }
    }
}
