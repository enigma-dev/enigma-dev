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
// GM front-end functions - Implementations of standard GM collision functions.
////////////////////////////////////

#include <cmath>
#include <limits>

#include "Universal_System/Object_Tiers/collisions_object.h"
#include "Universal_System/Instances/instance_system.h" //iter
#include "Universal_System/roomsystem.h"
#include "Collision_Systems/collision_mandatory.h" //iter
#include "Universal_System/Instances/instance.h"
#include "Universal_System/math_consts.h"
#include "Universal_System/var_array.h"

#include "../General/CSfuncs.h"
#include "../General/collisions_general.h"
#include "Polygonimpl.h"
#include "polygon_collision_util.h"

namespace enigma_user
{
    bool place_free(cs_scalar x, cs_scalar y)
    {
        return collide_inst_inst(all, true, true, x, y) == NULL;
    }

    bool place_empty(cs_scalar x, cs_scalar y)
    {
        return collide_inst_inst(all, false, true, x, y) == NULL;
    }

    bool place_meeting(cs_scalar x, cs_scalar y, int object)
    {
        return collide_inst_inst(object, false, true, x, y);
    }

    enigma::instance_t instance_place(cs_scalar x, cs_scalar y, int object)
    {
        enigma::object_collisions* const r = collide_inst_inst(object, false, true, x, y);
        return r == NULL ? noone : static_cast<int>(r->id);
    }
}

namespace enigma 
{
    object_basic *place_meeting_inst(cs_scalar x, cs_scalar y, int object)
    {
        return collide_inst_inst(object, false, true, x, y);
    }
}

namespace enigma_user
{

    bool position_free(cs_scalar x, cs_scalar y)
    {
        return collide_inst_point(all, true, true, false, x + 0.5, y + 0.5) == NULL;
    }

    bool position_empty(cs_scalar x, cs_scalar y)
    {
        return collide_inst_point(all, false, true, false, x + 0.5, y + 0.5) == NULL;
    }

    bool position_meeting(cs_scalar x, cs_scalar y, int object)
    {
        return collide_inst_point(object, false, true, false, x + 0.5, y + 0.5);
    }

    void position_destroy_object(cs_scalar x, cs_scalar y, int object, bool solid_only)
    {
        destroy_inst_point(object, solid_only, x + 0.5, y + 0.5);
    }

    void position_destroy_solid(cs_scalar x, cs_scalar y)
    {
        destroy_inst_point(all, true, x + 0.5, y + 0.5);
    }

    void position_destroy(cs_scalar x, cs_scalar y)
    {
        destroy_inst_point(all, false, x + 0.5, y + 0.5);
    }

    void position_change(cs_scalar x, cs_scalar y, int obj, bool perf)
    {
        change_inst_point(obj, perf, x + 0.5, y + 0.5);
    }

    enigma::instance_t instance_position(cs_scalar x, cs_scalar y, int object)
    {
        const enigma::object_collisions* r = collide_inst_point(object, false, true, false, x + 0.5, y + 0.5);
        return r == NULL ? noone : static_cast<int>(r->id);
    }

    enigma::instance_t collision_rectangle(cs_scalar x1, cs_scalar y1, cs_scalar x2, cs_scalar y2, int obj, bool prec, bool notme)
    {
        const enigma::object_collisions* r = collide_inst_rect(obj, false, prec, notme, x1 + 0.5, y1 + 0.5, x2 + 0.5, y2 + 0.5); //false is for solid_only
        return r == NULL ? noone : static_cast<int>(r->id);
    }

    enigma::instance_t collision_line(cs_scalar x1, cs_scalar y1, cs_scalar x2, cs_scalar y2, int obj, bool prec, bool notme)
    {
        const enigma::object_collisions* r = collide_inst_line(obj, false, prec, notme, x1 + 0.5, y1 + 0.5, x2 + 0.5, y2 + 0.5);
        return r == NULL ? noone : static_cast<int>(r->id);
    }

    enigma::instance_t collision_point(cs_scalar x, cs_scalar y, int obj, bool prec, bool notme)
    {
        const enigma::object_collisions* r = collide_inst_point(obj, false, prec, notme, x + 0.5, y + 0.5);
        return r == NULL ? noone : static_cast<int>(r->id);
    }

    enigma::instance_t collision_circle(cs_scalar x, cs_scalar y, double radius, int obj, bool prec, bool notme)
    {
        const enigma::object_collisions* r = collide_inst_circle(obj, false, prec, notme, x + 0.5, y + 0.5, radius);
        return r == NULL ? noone : static_cast<int>(r->id);
    }

    enigma::instance_t collision_ellipse(cs_scalar x1, cs_scalar y1, cs_scalar x2, cs_scalar y2, int obj, bool prec, bool notme)
    {
        double x = ((x1 + x2) / 2) + 0.5;
        double y = ((y1 + y2) / 2) + 0.5;
        double rx = fabs(x2 - x1) / 2;
        double ry = fabs(y2 - y1) / 2;
        const enigma::object_collisions* r = collide_inst_ellipse(obj, false, prec, notme, x, y, rx, ry);
        return r == NULL ? noone : static_cast<int>(r->id);
    }

    double distance_to_object(int object)
    {
        // Fetching the instance of the calling object
        const enigma::object_collisions* inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
        
        // If no sprite/mask/polygon than distance cannot be computed
        if (inst1->sprite_index == -1 && inst1->mask_index == -1 && inst1->polygon_index == -1)
            return -1;
        
        // Set the initial distance as infinity, since we are computing the lowest distance
        // to the instance
        double distance = std::numeric_limits<double>::infinity();
        double tempdist;

        // Computing BBOX of the calling instance
        int left1, top1, right1, bottom1;
        enigma::get_bbox_border(left1, top1, right1, bottom1, inst1);

        // Iterating over the instances of the specified object
        for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
        {
            const enigma::object_collisions* inst2 = (enigma::object_collisions*) *it;

            // If same instance than no distance
            if (inst1 == inst2) 
                continue;

            // If no sprite/mask/polygon than no collision
            if (inst2->sprite_index == -1 && inst2->mask_index == -1 && inst2->polygon_index == -1)
                continue;

            // Computing the BBOX of the second instance
            int left2, top2, right2, bottom2;
            enigma::get_bbox_border(left2, top2, right2, bottom2, inst2);

            // Computing the distance
            const int right  = min(right1, right2),   left = max(left1, left2),
                    bottom = min(bottom1, bottom2), top  = max(top1, top2);

            tempdist = hypot((left > right ? left - right : 0),
                            (top > bottom ? top - bottom : 0));

            // If distance is min set as new distance
            if (tempdist < distance)
            {
                distance = tempdist;
            }
        }
        // If the distance was not computed than it is returned as -1
        return (distance == std::numeric_limits<double>::infinity() ? -1 : distance);
    }

    double distance_to_point(cs_scalar x, cs_scalar y)
    {
        // Fetching the instance
        enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
        
        // If no sprite/mask/polygon than cannot compute distance
        if (inst1->sprite_index == -1 && inst1->mask_index == -1 && inst1->polygon_index == -1)
            return -1;

        // Computing the BBOX of the instance
        int left1, top1, right1, bottom1;
        enigma::get_bbox_border(left1, top1, right1, bottom1, inst1);

        // Computing the distance and returning
        return fabs(hypot(min(left1 - x, right1 - x),
                        min(top1 - y, bottom1 - y)));
    }

    double move_contact_object(int object, double angle, double max_dist, bool solid_only)
    {
        // Fetching the calling instance
        enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
        
        // If no sprite/mask/polygon than no collision
        if (inst1->sprite_index == -1 && inst1->mask_index == -1 && inst1->polygon_index == -1) 
        {
            return -4;
        }

        // If no collision than no movement after contact
        const double x = inst1->x, y = inst1->y;
        if (collide_inst_inst(object, solid_only, true, x, y) != NULL) 
        {
            return 0;
        }

        const double DMIN = 1, DMAX = 1000; // Arbitrary max for non-positive values, 1000 fits with other implementations.
        const double contact_distance = DMIN;
        double mid_dist = max_dist; // mid_dist is used for the subtraction part.
        if (max_dist <= 0) { // Use the arbitrary max for non-positive values.
            max_dist = DMAX;
        }
        mid_dist = max_dist;
        const double radang = (fmod(fmod(angle, 360) + 360, 360))*(M_PI/180.0);
        const double sin_angle = sin(radang), cos_angle = cos(radang);

        // --------------------------------
        // Subtraction Starts
        // --------------------------------
        const int quad = int(angle/90.0);

        // Getting the bounding box of the instance
        int left1, top1, right1, bottom1;
        enigma::get_bbox_border(left1, top1, right1, bottom1, inst1);

        // Iterating over the instances to find the one with the collision
        for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
        {
            const enigma::object_collisions* inst2 = (enigma::object_collisions*)*it;
            
            // If no sprite/mask/polygon than no collision
            if (inst2->sprite_index == -1 && inst2->mask_index == -1 && inst2->polygon_index == -1)
                continue;

            // If the second instance is not solid given the solid check, or same instance, than no 
            // collision
            if (inst2->id == inst1->id || (solid_only && !inst2->solid))
                continue;

            // Computing BBOX for the second instance
            int left2, top2, right2, bottom2;
            enigma::get_bbox_border(left2, top2, right2, bottom2, inst2);

            // If no sweep and prune collision
            if (right2 >= left1 && bottom2 >= top1 && left2 <= right1 && top2 <= bottom1)
            {
                mid_dist = DMIN;
                break;
            }

            switch (quad)
            {
                case 0:
                    if ((left2 > right1 || top1 > bottom2) &&
                    angle_difference(point_direction(right1, bottom1, left2, top2),angle) >= 0  &&
                    angle_difference(point_direction(left1, top1, right2, bottom2),angle) <= 0)
                    {
                        if (angle_difference(point_direction(right1, top1, left2, bottom2),angle) > 0)
                        {
                            mid_dist = min(mid_dist, (top1 - bottom2 - contact_distance)/sin_angle);
                        }
                        else
                        {
                            mid_dist = min(mid_dist, (left2 - right1 - contact_distance)/cos_angle);
                        }
                    }
                break;
                case 1:
                    if ((left1 > right2 || top1 > bottom2) &&
                    angle_difference(point_direction(left1, bottom1, right2, top2),angle) <= 0  &&
                    angle_difference(point_direction(right1, top1, left2, bottom2),angle) >= 0)
                    {
                        if (angle_difference(point_direction(left1, top1, right2, bottom2),angle) > 0)
                        {
                            mid_dist = min(mid_dist, (right2 - left1 + contact_distance)/cos_angle);
                        }
                        else
                        {
                            mid_dist = min(mid_dist, (top1 - bottom2 - contact_distance)/sin_angle);
                        }
                    }
                break;
                case 2:
                    if ((left1 > right2 || top2 > bottom1) &&
                    angle_difference(point_direction(right1, bottom1, left2, top2),angle) <= 0  &&
                    angle_difference(point_direction(left1, top1, right2, bottom2),angle) >= 0)
                    {
                        if (angle_difference(point_direction(left1, bottom1, right2, top2),angle) > 0)
                        {
                            mid_dist = min(mid_dist, (bottom1 - top2 + contact_distance)/sin_angle);
                        }
                        else
                        {
                            mid_dist = min(mid_dist, (right2 - left1 + contact_distance)/cos_angle);
                        }
                    }
                break;
                case 3:
                    if ((left2 > right1 || top2 > bottom1) &&
                    angle_difference(point_direction(right1, top1, left2, bottom2),angle) <= 0  &&
                    angle_difference(point_direction(left1, bottom1, right2, top2),angle) >= 0)
                    {
                        if (angle_difference(point_direction(right1, bottom1, left2, top2),angle) > 0)
                        {
                            mid_dist = min(mid_dist, (left2 - right1 - contact_distance)/cos_angle);
                        }
                        else
                        {
                            mid_dist = min(mid_dist, (bottom1 - top2 + contact_distance)/sin_angle);
                        }
                    }
                break;
            }
        }

        double current_dist = DMIN;
        {
            // Avoid moving to position which isn't free. mid_dist is not guaranteed to indicate a free position.
            double next_x = x + mid_dist*cos_angle;
            double next_y = y - mid_dist*sin_angle;
            if (collide_inst_inst(object, solid_only, true, next_x, next_y) == NULL) {
                inst1->x = next_x;
                inst1->y = next_y;
                current_dist = mid_dist;
            }
        }

        // -------------------------
        // Subtraction end.
        // -------------------------

        for (; current_dist <= max_dist; current_dist++)
        {
            const double next_x = x + current_dist*cos_angle;
            const double next_y = y - current_dist*sin_angle;
            if (collide_inst_inst(object, solid_only, true, next_x, next_y) != NULL) {
                current_dist--;
                break;
            }
            else {
                inst1->x = next_x;
                inst1->y = next_y;
            }
        }

        return current_dist;
    }

    double move_outside_object(int object, double angle, double max_dist, bool solid_only)
    {
        // Fetching the calling instance
        enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
        
        // If no sprite/mask/polygon than no collision
        if (inst1->sprite_index == -1 && inst1->mask_index == -1 && inst1->polygon_index == -1) 
        {
            return -4;
        }

        // Determing collision
        double x = inst1->x, y = inst1->y;
        if (collide_inst_inst(object, solid_only, true, x, y) == NULL) {
            return 0;
        }

        const double DMIN = 1, DMAX = 1000; // Arbitrary max for non-positive values, 1000 fits with other implementations.
        if (max_dist <= 0) { // Use the arbitrary max for non-positive values.
            max_dist = DMAX;
        }
        const double radang = (fmod(fmod(angle, 360) + 360, 360))*(M_PI/180.0);

        double current_dist;
        for (current_dist = DMIN; current_dist <= max_dist; current_dist++)
        {
            const double next_x = x + current_dist*cos(radang);
            const double next_y = y - current_dist*sin(radang);
            inst1->x = next_x;
            inst1->y = next_y;
            if (collide_inst_inst(object, solid_only, true, next_x, next_y) == NULL) {
                break;
            }
        }

        return current_dist;
    }

    bool move_bounce_object(int object, bool adv, bool solid_only)
    {
        // Fetching the calling instance
        enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
        
        // If no sprite/mask/polygon than no collision
        if (inst1->sprite_index == -1 && inst1->mask_index == -1 && inst1->polygon_index == -1)
            return false;

        if (collide_inst_inst(object, solid_only, true, inst1->x, inst1->y) == NULL &&
            collide_inst_inst(object, solid_only, true, inst1->x + inst1->hspeed, inst1->y + inst1->vspeed) == NULL) {
            return false;
        }

        if (collide_inst_inst(object, solid_only, true, inst1->x, inst1->y) != NULL) {
            // Return the instance to its previous position.
            inst1->x = inst1->xprevious;
            inst1->y = inst1->yprevious;
        }

        const double x_start = inst1->x, y_start = inst1->y;

        const double x = inst1->x + inst1->hspeed, y = inst1->y + inst1->vspeed;

        if (adv) {
            const double angle_radial = 10.0; // The angle increment for the radials.
            const int radial_max = max(int(180/angle_radial), 1); // The maximum number of radials in one direction.

            const double effective_direction = inst1->speed >= 0 ? inst1->direction : fmod(inst1->direction+180.0, 360.0);
            const double flipped_direction = fmod(effective_direction + 180.0, 360.0);
            const double speed = abs((double) inst1->speed + 1); //max(1, abs(inst1->speed));

            // Find the normal direction of the collision by doing radial collisions based on the speed and flipped direction.

            int d1, d2;
            for (d1 = 0; d1 < radial_max; d1++) // Positive direction.
            {
                const double rad = (flipped_direction + d1*angle_radial)*M_PI/180.0;
                const double x1 = x + speed*cos(rad);
                const double y1 = y - speed*sin(rad);
                if (collide_inst_inst(object, solid_only, true, x1, y1) != NULL) {
                    break;
                }
            }
            for (d2 = 0; d2 > -radial_max; d2--) // Negative direction.
            {
                const double rad = (flipped_direction + d2*angle_radial)*M_PI/180.0;
                const double x1 = x + speed*cos(rad);
                const double y1 = y - speed*sin(rad);
                if (collide_inst_inst(object, solid_only, true, x1, y1) != NULL) {
                    break;
                }
            }

            const int d = int(round((d1 + d2)/2.0));
            const double normal_direction = fmod(flipped_direction + d*angle_radial + 360.0, 360.0);

            // Flip and then mirror the effective direction unit vector along the direction of the normal.

            const double normal_rad = normal_direction * M_PI / 180.0;
            const double normal_x = cos(normal_rad), normal_y = -sin(normal_rad);

            const double flipped_rad = flipped_direction * M_PI / 180.0;
            const double flipped_x = cos(flipped_rad), flipped_y = -sin(flipped_rad);

            const double dot_product = normal_x*flipped_x + normal_y*flipped_y;
            const double flipped_on_normal_x = dot_product*normal_x, flipped_on_normal_y = dot_product*normal_y;

            const double crossed_x = flipped_x - flipped_on_normal_x, crossed_y = flipped_y - flipped_on_normal_y;

            const double mirror_x = flipped_x - 2*crossed_x, mirror_y = flipped_y - 2*crossed_y;

            // Set final direction from flipped, mirrored direction unit vector.

            const double mirror_direction = fmod(atan2(-mirror_y, mirror_x) * 180.0 / M_PI + 360.0, 360.0);
            inst1->direction = inst1->speed >= 0 ? mirror_direction : fmod(mirror_direction + 180.0, 360.0);
            return true;
        }
        else {
            const double direction = inst1->speed >= 0 ? inst1->direction : fmod(inst1->direction+180.0, 360.0);
            const bool free_horizontal = collide_inst_inst(object, solid_only, true, x, y_start) == NULL;
            const bool free_vertical = collide_inst_inst(object, solid_only, true, x_start, y) == NULL;
            double new_direction;
            if (!free_horizontal && free_vertical) {
                new_direction = direction <= 180.0 ? fmod(180.0 - direction, 360.0) : fmod((360.0 - direction) + 180.0, 360.0);
            }
            else if (free_horizontal && !free_vertical) {
                new_direction = fmod(360.0 - direction, 360.0);
            }
            else {
                new_direction = fmod(direction + 180.0, 360.0);
            }
            inst1->direction = inst1->speed >= 0 ? new_direction : fmod(new_direction + 180.0, 360.0);

            return true;
        }
    }

}

typedef std::pair<int,enigma::object_basic*> inode_pair;

namespace enigma_user
{

    void instance_deactivate_region(int rleft, int rtop, int rwidth, int rheight, bool inside, bool notme) 
    {
        // Iterating over the instances
        for (enigma::iterator it = enigma::instance_list_first(); it; ++it) 
        {
            // notme bool given than don't check for this instance
            if (notme && (*it)->id == enigma::instance_event_iterator->inst->id) 
                continue;

            // Getting the instance
            enigma::object_collisions* const inst = (enigma::object_collisions*) *it;

            // If no sprite/mask/polygon then no collision
            if (inst->sprite_index == -1 && inst->mask_index == -1 && inst->polygon_index == -1) 
                continue;

            // Bounding Box retrieval and collision check
            int left, top, right, bottom;
            enigma::get_bbox_border(left, top, right, bottom, inst);

            // Sweep and Prune Check
            if ((left <= (rleft + rwidth) && rleft <= right && top <= (rtop + rheight) && rtop <= bottom) == inside) 
            {
                inst->deactivate();
                enigma::instance_deactivated_list.insert(inode_pair(inst->id,inst));
            }
        }
    }

    void instance_activate_region(int rleft, int rtop, int rwidth, int rheight, bool inside) 
    {
        // Iterating over the instances
        std::map<int,enigma::object_basic*>::iterator iter = enigma::instance_deactivated_list.begin();
        while (iter != enigma::instance_deactivated_list.end()) 
        {
            enigma::object_collisions* const inst = (enigma::object_collisions*) iter->second;

            // no sprite/mask/polygon then no collision
            if (inst->sprite_index == -1 && inst->mask_index == -1 && inst->polygon_index == -1)
            {
                ++iter;
                continue;
            }

            // Bounding Box retrieval and collision check
            int left, top, right, bottom;
            enigma::get_bbox_border(left, top, right, bottom, inst);

            if ((left <= (rleft + rwidth) && rleft <= right && top <= (rtop + rheight) && rtop <= bottom) == inside) 
            {
                inst->activate();
                enigma::instance_deactivated_list.erase(iter++);
            } 
            else 
            {
                ++iter;
            }
        }
    }

}

static bool line_ellipse_intersects(cs_scalar rx, cs_scalar ry, cs_scalar x, cs_scalar ly1, cs_scalar ly2)
{
    // Formula: x^2/a^2 + y^2/b^2 = 1   <=>   y = +/- sqrt(b^2*(1 - x^2/a^2))

    const cs_scalar inner = ry*ry*(1 - x*x/(rx*rx));
    if (inner < 0) {
        return false;
    }
    else {
        const cs_scalar y1 = -sqrt(inner), y2 = sqrt(inner);
        return y1 <= ly2 && ly1 <= y2;
    }
}

namespace enigma_user
{

    void instance_deactivate_circle(int x, int y, int r, bool inside, bool notme)
    {
        // Iterating over the instances
        for (enigma::iterator it = enigma::instance_list_first(); it; ++it)
        {
            // boolean given notme than collision not checked for this instance
            if (notme && (*it)->id == enigma::instance_event_iterator->inst->id)
                continue;

            // Fetching the instance
            enigma::object_collisions* const inst = (enigma::object_collisions*) *it;

            // If no sprite/mask/polygon then no collision
            if (inst->sprite_index == -1 && inst->mask_index == -1 && inst->polygon_index == -1) 
                continue;

            // Doing a Sweep and Prune check using BBOX
            int left, top, right, bottom;
            enigma::get_bbox_border(left, top, right, bottom, inst);

            // TODO (Nabeel) : Does this needs a polygon collision check?

            const bool intersects = line_ellipse_intersects(r, r, left-x, top-y, bottom-y) ||
                                    line_ellipse_intersects(r, r, right-x, top-y, bottom-y) ||
                                    line_ellipse_intersects(r, r, top-y, left-x, right-x) ||
                                    line_ellipse_intersects(r, r, bottom-y, left-x, right-x) ||
                                    (x >= left && x <= right && y >= top && y <= bottom); // Circle inside bbox.

            // If the instance intersects the ellipse
            if (intersects)
            {
                if (inside)
                {
                    inst->deactivate();
                    enigma::instance_deactivated_list.insert(inode_pair(inst->id,inst));
                }
            }
            else
            {
                if (!inside)
                {
                    inst->deactivate();
                    enigma::instance_deactivated_list.insert(inode_pair(inst->id,inst));
                }
            }
        }
    }

    void instance_activate_circle(int x, int y, int r, bool inside)
    {
        // Iterating over the instances
        std::map<int,enigma::object_basic*>::iterator iter = enigma::instance_deactivated_list.begin();
        while (iter != enigma::instance_deactivated_list.end()) 
        {
            enigma::object_collisions* const inst = (enigma::object_collisions*)iter->second;

            // If no sprite/mask/polygon then no collision
            if (inst->sprite_index == -1 && inst->mask_index == -1 && inst->polygon_index == -1)
            {
                ++iter;
                continue;
            }

            // Doing a Sweep and Prune check using BBOX
            int left, top, right, bottom;
            enigma::get_bbox_border(left, top, right, bottom, inst);

            const bool intersects = line_ellipse_intersects(r, r, left-x, top-y, bottom-y) ||
                                    line_ellipse_intersects(r, r, right-x, top-y, bottom-y) ||
                                    line_ellipse_intersects(r, r, top-y, left-x, right-x) ||
                                    line_ellipse_intersects(r, r, bottom-y, left-x, right-x) ||
                                    (x >= left && x <= right && y >= top && y <= bottom); // Circle inside bbox.

            bool removed = false;
            if (intersects)
            {
                if (inside)
                {
                    inst->activate();
                    enigma::instance_deactivated_list.erase(iter++);
                    removed = true;
                }
            }
            else
            {
                if (!inside)
                {
                    inst->activate();
                    enigma::instance_deactivated_list.erase(iter++);
                    removed = true;
                }
            }
            if (!removed) 
            {
                ++iter;
            }
        }
    }
 
    var instance_get_mtv(int object)
    {
        var MTV_return;
        MTV_return[0] = 0;
        MTV_return[1] = 1;

        // Fetching the instance
        enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);

        // If no polygon than no MTV
        if (inst1->polygon_index == -1)
            return MTV_return;
        
        // Computing BBOX of the calling instance
        int left1, top1, right1, bottom1;
        enigma::get_bbox_border(left1, top1, right1, bottom1, inst1);

        // Iterating over the instances of the specified object
        for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
        {
            // Selecting the instance
            enigma::object_collisions* const inst2 = (enigma::object_collisions*) *it;

            // If no polygon than no collision
            if (inst2->polygon_index == -1)
                continue;
            
            // Computing the BBOX of the second instance
            int left2, top2, right2, bottom2;
            enigma::get_bbox_border(left2, top2, right2, bottom2, inst2);

            // Main Sweep and Prune Check
            if (left1 <= right2 && left2 <= right1 && top1 <= bottom2 && top2 <= bottom1)
            {
                // Fetching Points
                std::vector<glm::vec2> points_poly1 = enigma::polygons.get(inst1->polygon_index).getOffsetPoints();
                std::vector<glm::vec2> points_poly2 = enigma::polygons.get(inst2->polygon_index).getOffsetPoints();

                // Computing Pivots
                glm::vec2 pivot1(0, 0);
                glm::vec2 pivot2(0, 0);

                // Applying transformations
                enigma::transformPoints(points_poly1, 
                                        inst1->x, inst1->y, 
                                        inst1->polygon_angle, pivot1,
                                        inst1->polygon_xscale, inst1->polygon_yscale);

                enigma::transformPoints(points_poly2, 
                                        inst2->x, inst2->y, 
                                        inst2->polygon_angle, pivot2,
                                        inst2->polygon_xscale, inst2->polygon_yscale);

                // Computing MTV
                glm::vec2 mtv = enigma::compute_MTV(points_poly1, points_poly2);
                
                // Packing in var and return
                var MTV_return = array_create(2);
                array_set(MTV_return, 0, mtv.x);
                array_set(MTV_return, 1, mtv.y);
                return MTV_return;
            }
        }
        return MTV_return;
    }
}
