/** Copyright (C) 2008-2011 Josh Ventura
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

/**
  @file  graphics_object.h
  @brief Adds a collisions-related tier following the transform tier.
*/

#include "collisions_object.h"
#include <cmath>

namespace enigma
{
    int object_collisions::$bbox_left() const
    {
        if (image_angle == 0)
            return (image_xscale >= 0) ?
                   ((mask_index >= 0 ? sprite_get_bbox_left_relative(mask_index)*image_xscale : (sprite_index >= 0 ? sprite_get_bbox_left_relative(sprite_index)*image_xscale : 0)) + x + .5) :
                   ((mask_index >= 0 ? (sprite_get_bbox_right_relative(mask_index) + 1)*image_xscale - 1 : (sprite_index >= 0 ? (sprite_get_bbox_right_relative(sprite_index) + 1)*image_xscale - 1: 0)) + x + .5);

        const double arad = image_angle*(M_PI/180.0);
        const int quad = int(fmod(fmod(image_angle, 360) + 360, 360)/90.0);
        double w, h;
        w = ((image_xscale >= 0)^(quad == 1 || quad == 2)) ?
            (mask_index >= 0 ? sprite_get_bbox_left_relative(mask_index)*image_xscale : (sprite_index >= 0 ? sprite_get_bbox_left_relative(sprite_index)*image_xscale : 0)) :
            (mask_index >= 0 ? (sprite_get_bbox_right_relative(mask_index) + 1)*image_xscale - 1 : (sprite_index >= 0 ? (sprite_get_bbox_right_relative(sprite_index) + 1)*image_xscale - 1: 0));
        h = ((image_yscale >= 0)^(quad == 2 || quad == 3)) ?
            (mask_index >= 0 ? sprite_get_bbox_top_relative(mask_index)*image_yscale : (sprite_index >= 0 ? sprite_get_bbox_top_relative(sprite_index)*image_yscale : 0)) :
            (mask_index >= 0 ? (sprite_get_bbox_bottom_relative(mask_index) + 1)*image_yscale - 1 : (sprite_index >= 0 ? (sprite_get_bbox_bottom_relative(sprite_index) + 1)*image_yscale - 1: 0));

        return cos(arad)*w + sin(arad)*h + x + .5;
    }

    int object_collisions::$bbox_right() const
    {
        if (image_angle == 0)
            return (image_xscale >= 0) ?
                   ((mask_index >= 0 ? (sprite_get_bbox_right_relative(mask_index) + 1)*image_xscale - 1 : (sprite_index >= 0 ? (sprite_get_bbox_right_relative(sprite_index) + 1)*image_xscale - 1: 0)) + x + .5) :
                   ((mask_index >= 0 ? sprite_get_bbox_left_relative(mask_index)*image_xscale : (sprite_index >= 0 ? sprite_get_bbox_left_relative(sprite_index)*image_xscale : 0)) + x + .5);

        const double arad = image_angle*(M_PI/180.0);
        const int quad = int(fmod(fmod(image_angle, 360) + 360, 360)/90.0);
        double w, h;
        w = ((image_xscale >= 0)^(quad == 1 || quad == 2)) ?
            (mask_index >= 0 ? (sprite_get_bbox_right_relative(mask_index) + 1)*image_xscale - 1 : (sprite_index >= 0 ? (sprite_get_bbox_right_relative(sprite_index) + 1)*image_xscale - 1: 0)) :
            (mask_index >= 0 ? sprite_get_bbox_left_relative(mask_index)*image_xscale : (sprite_index >= 0 ? sprite_get_bbox_left_relative(sprite_index)*image_xscale : 0));
        h = ((image_yscale >= 0)^(quad == 2 || quad == 3)) ?
            (mask_index >= 0 ? (sprite_get_bbox_bottom_relative(mask_index) + 1)*image_yscale - 1 : (sprite_index >= 0 ? (sprite_get_bbox_bottom_relative(sprite_index) + 1)*image_yscale - 1: 0)) :
            (mask_index >= 0 ? sprite_get_bbox_top_relative(mask_index)*image_yscale : (sprite_index >= 0 ? sprite_get_bbox_top_relative(sprite_index)*image_yscale : 0));

        return cos(arad)*w + sin(arad)*h + x + .5;
    }

    int object_collisions::$bbox_top() const
    {
        if (image_angle == 0)
            return (image_xscale >= 0) ?
                ((mask_index >= 0 ? sprite_get_bbox_top_relative(mask_index)*image_yscale : (sprite_index >= 0 ? sprite_get_bbox_top_relative(sprite_index)*image_yscale : 0)) + y + .5):
                ((mask_index >= 0 ? (sprite_get_bbox_bottom_relative(mask_index) + 1)*image_yscale - 1 : (sprite_index >= 0 ? (sprite_get_bbox_bottom_relative(sprite_index) + 1)*image_yscale - 1: 0)) + y + .5);

        const double arad = image_angle*(M_PI/180.0);
        const int quad = int(fmod(fmod(image_angle, 360) + 360, 360)/90.0);
        double w, h;
        w = ((image_xscale >= 0)^(quad == 2 || quad == 3)) ?
            (mask_index >= 0 ? (sprite_get_bbox_right_relative(mask_index) + 1)*image_xscale - 1 : (sprite_index >= 0 ? (sprite_get_bbox_right_relative(sprite_index) + 1)*image_xscale - 1: 0)) :
            (mask_index >= 0 ? sprite_get_bbox_left_relative(mask_index)*image_xscale : (sprite_index >= 0 ? sprite_get_bbox_left_relative(sprite_index)*image_xscale : 0));
        h = ((image_yscale >= 0)^(quad == 1 || quad == 2)) ?
            (mask_index >= 0 ? sprite_get_bbox_top_relative(mask_index)*image_yscale : (sprite_index >= 0 ? sprite_get_bbox_top_relative(sprite_index)*image_yscale : 0)) :
            (mask_index >= 0 ? (sprite_get_bbox_bottom_relative(mask_index) + 1)*image_yscale - 1 : (sprite_index >= 0 ? (sprite_get_bbox_bottom_relative(sprite_index) + 1)*image_yscale - 1: 0));

        return cos(arad)*h -sin(arad)*w + y + .5;
    }

    int object_collisions::$bbox_bottom() const
    {
        if (image_angle == 0)
            return (image_xscale >= 0) ?
                ((mask_index >= 0 ? (sprite_get_bbox_bottom_relative(mask_index) + 1)*image_yscale - 1 : (sprite_index >= 0 ? (sprite_get_bbox_bottom_relative(sprite_index) + 1)*image_yscale - 1: 0)) + y + .5) :
                ((mask_index >= 0 ? sprite_get_bbox_top_relative(mask_index)*image_yscale : (sprite_index >= 0 ? sprite_get_bbox_top_relative(sprite_index)*image_yscale : 0)) + y + .5);

        const double arad = image_angle*(M_PI/180.0);
        const int quad = int(fmod(fmod(image_angle, 360) + 360, 360)/90.0);
        double w, h;
        w = ((image_xscale >= 0)^(quad == 2 || quad == 3)) ?
            (mask_index >= 0 ? sprite_get_bbox_left_relative(mask_index)*image_xscale : (sprite_index >= 0 ? sprite_get_bbox_left_relative(sprite_index)*image_xscale : 0)) :
            (mask_index >= 0 ? (sprite_get_bbox_right_relative(mask_index) + 1)*image_xscale - 1 : (sprite_index >= 0 ? (sprite_get_bbox_right_relative(sprite_index) + 1)*image_xscale - 1: 0));
        h = ((image_yscale >= 0)^(quad == 1 || quad == 2)) ?
            (mask_index >= 0 ? (sprite_get_bbox_bottom_relative(mask_index) + 1)*image_yscale - 1 : (sprite_index >= 0 ? (sprite_get_bbox_bottom_relative(sprite_index) + 1)*image_yscale - 1: 0)) :
            (mask_index >= 0 ? sprite_get_bbox_top_relative(mask_index)*image_yscale : (sprite_index >= 0 ? sprite_get_bbox_top_relative(sprite_index)*image_yscale : 0));

        return cos(arad)*h - sin(arad)*w + y + .5;
    }

    const bbox_rect_t& object_collisions::$bbox_relative() const
    {
        return (mask_index >= 0 ? sprite_get_bbox_relative(mask_index) : sprite_get_bbox_relative(sprite_index));
    }
    const bbox_rect_t& object_collisions::$bbox() const
    {
         return (mask_index >= 0 ? sprite_get_bbox(mask_index) : sprite_get_bbox(sprite_index));
    }

    object_collisions::object_collisions(): object_transform() {}
    object_collisions::object_collisions(unsigned _id,int _objid): object_transform(_id,_objid) {}
    object_collisions::~object_collisions() {}
}
