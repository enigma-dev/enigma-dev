/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2021 Nabeel Danish
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
 * @file  graphics_object.h
 * @brief Adds a collisions-related tier following the transform tier.
 */

#include "collisions_object.h"

#include "serialization.h"
#include "Universal_System/math_consts.h"
#include "Universal_System/Resources/sprites.h"
#include "Universal_System/Resources/sprites_internal.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <cmath>
#include <floatcomp.h>

using enigma_user::sprite_get_bbox_right_relative;
using enigma_user::sprite_get_bbox_left_relative;
using enigma_user::sprite_get_bbox_top_relative;
using enigma_user::sprite_get_bbox_bottom_relative;

namespace enigma
{
    int object_collisions::$bbox_left() const
    {
        if (fzero(image_angle))
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
        if (fzero(image_angle))
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
        if (fzero(image_angle))
            return (image_yscale >= 0) ?
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
        if (fzero(image_angle))
            return (image_yscale >= 0) ?
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

    const BoundingBox object_collisions::$bbox_relative() const
    {
        return (mask_index >= 0 ? sprite_get_bbox_relative(mask_index) : sprite_get_bbox_relative(sprite_index));
    }
    
    const BoundingBox& object_collisions::$bbox() const
    {
         return (mask_index >= 0 ? sprite_get_bbox(mask_index) : sprite_get_bbox(sprite_index));
    }

    object_collisions::object_collisions(): object_transform() { 
        polygon_index = -1;
        polygon_xscale = polygon_yscale = 1;
        polygon_angle = 0;
    }

    object_collisions::object_collisions(unsigned _id,int _objid): object_transform(_id,_objid) {
        polygon_index = -1;
        polygon_xscale = polygon_yscale = 1;
        polygon_angle = 0;
    }

    object_collisions::~object_collisions() {}

    std::vector<std::byte> object_collisions::serialize() {
      auto bytes = object_transform::serialize();
      std::size_t len = 0;

      enigma_internal_serialize<unsigned char>(object_collisions::objtype, len, bytes);
      enigma_internal_serialize(mask_index, len, bytes);
      enigma_internal_serialize(solid, len, bytes);
      enigma_internal_serialize(polygon_index, len, bytes);
      enigma_internal_serialize(polygon_xscale, len, bytes);
      enigma_internal_serialize(polygon_yscale, len, bytes);
      enigma_internal_serialize(polygon_angle, len, bytes);

      bytes.shrink_to_fit();
      return bytes;
    }

    std::size_t object_collisions::deserialize_self(std::byte* iter) {
      auto len = object_transform::deserialize_self(iter);

      unsigned char type;
      enigma_internal_deserialize(type, iter, len);
      if (type != object_collisions::objtype) {
        DEBUG_MESSAGE("object_collisions::deserialize_self: Object type '" + std::to_string(type) +
                          "' does not match expected: " + std::to_string(object_collisions::objtype),
                      MESSAGE_TYPE::M_FATAL_ERROR);
      }
      enigma_internal_deserialize(mask_index, iter, len);
      enigma_internal_deserialize(solid, iter, len);
      enigma_internal_deserialize(polygon_index, iter, len);
      enigma_internal_deserialize(polygon_xscale, iter, len);
      enigma_internal_deserialize(polygon_yscale, iter, len);
      enigma_internal_deserialize(polygon_angle, iter, len);

      return len;
    }

    std::pair<object_collisions, std::size_t> object_collisions::deserialize(std::byte* iter) {
      object_collisions result;
      auto len = result.deserialize_self(iter);
      return {std::move(result), len};
    }
}
