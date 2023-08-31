/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2011-2012 polygone
*** Copyright (C) 2013 Robert B Colton, canthelp
*** Copyright (C) 2023 Fares Atef
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
  @file  planar_object.cpp
  @brief Adds a planar tier following the main tier.
*/

#include <math.h>

#include <floatcomp.h>

#include "Universal_System/var4.h"
#include "object.h"
#include "../Serialization/serialization.h"
#include "Universal_System/math_consts.h"
#include "Universal_System/reflexive_types.h"
#include "Widget_Systems/widgets_mandatory.h"

#include "planar_object.h"

#ifdef PATH_EXT_SET
#  include "Universal_System/Extensions/Paths/path_functions.h"
#endif

namespace enigma
{
  object_planar::object_planar()
  {
    hspeed.vspd  = &vspeed.rval.d;
      hspeed.dir = &direction.rval.d;
      hspeed.spd = &speed.rval.d;
    vspeed.hspd  = &hspeed.rval.d;
      vspeed.dir = &direction.rval.d;
      vspeed.spd = &speed.rval.d;
    direction.spd    = &speed.rval.d;
      direction.hspd = &hspeed.rval.d;
      direction.vspd = &vspeed.rval.d;
    speed.dir    = &direction.rval.d;
      speed.hspd = &hspeed.rval.d;
      speed.vspd = &vspeed.rval.d;
  }
  object_planar::object_planar(unsigned _id, int objid): object_basic(_id,objid)
  {
    hspeed.vspd  = &vspeed.rval.d;
      hspeed.dir = &direction.rval.d;
      hspeed.spd = &speed.rval.d;
    vspeed.hspd  = &hspeed.rval.d;
      vspeed.dir = &direction.rval.d;
      vspeed.spd = &speed.rval.d;
    direction.spd    = &speed.rval.d;
      direction.hspd = &hspeed.rval.d;
      direction.vspd = &vspeed.rval.d;
    speed.dir    = &direction.rval.d;
      speed.hspd = &hspeed.rval.d;
      speed.vspd = &vspeed.rval.d;
  }

  //This just needs implemented virtually so instance_destroy works.
  object_planar::~object_planar() {}

  std::vector<std::byte> object_planar::serialize() {
    std::vector<std::byte> bytes = object_basic::serialize();
    std::size_t len = 0;

    enigma::bytes_serialization::enigma_serialize<unsigned char>(object_planar::objtype, len, bytes);
    enigma::bytes_serialization::enigma_serialize_many(len, bytes, x, y, xprevious, yprevious, xstart, ystart);
#ifdef ISLOCAL_persistent
    enigma::bytes_serialization::enigma_serialize(persistent, len, bytes);
#endif
    enigma::bytes_serialization::enigma_serialize_many(len, bytes, direction, speed, hspeed, vspeed, gravity, gravity_direction, friction);

    bytes.shrink_to_fit();
    return bytes;
  }

  std::size_t object_planar::deserialize_self(std::byte *iter) {
    auto len = object_basic::deserialize_self(iter);

    unsigned char type;
    enigma::bytes_serialization::enigma_deserialize(type, iter, len);
    if (type != object_planar::objtype) {
      DEBUG_MESSAGE("object_planar::deserialize_self: Object type '" + std::to_string(type) +
                        "' does not match expected: " + std::to_string(object_planar::objtype),
                    MESSAGE_TYPE::M_FATAL_ERROR);
    }
    enigma::bytes_serialization::enigma_deserialize_many(iter, len, x, y, xprevious, yprevious, xstart, ystart);
#ifdef ISLOCAL_persistent
    enigma::bytes_serialization::enigma_deserialize(persistent, iter, len);
#endif
    enigma::bytes_serialization::enigma_deserialize_many(iter, len, direction, speed, hspeed, vspeed, gravity, gravity_direction, friction);

    hspeed.vspd    = &vspeed.rval.d;
    hspeed.dir     = &direction.rval.d;
    hspeed.spd     = &speed.rval.d;

    vspeed.hspd    = &hspeed.rval.d;
    vspeed.dir     = &direction.rval.d;
    vspeed.spd     = &speed.rval.d;

    direction.spd  = &speed.rval.d;
    direction.hspd = &hspeed.rval.d;
    direction.vspd = &vspeed.rval.d;

    speed.dir      = &direction.rval.d;
    speed.hspd     = &hspeed.rval.d;
    speed.vspd     = &vspeed.rval.d;

    return len;
  }

  std::pair<object_planar, std::size_t> object_planar::deserialize(std::byte *iter) {
    object_planar result;
    auto len = result.deserialize_self(iter);
    return {std::move(result), len};
  }

  std::string object_planar::json_serialize() const {
    std::string json = "{";

    json += "\"object_type\":\"object_planar\",";
    json += "\"object_basic\":" + object_basic::json_serialize()+ ",";
    json += "\"x\":" + enigma::JSON_serialization::enigma_serialize(x) + ",";
    json += "\"y\":" + enigma::JSON_serialization::enigma_serialize(y) + ",";
    json += "\"xprevious\":" + enigma::JSON_serialization::enigma_serialize(xprevious) + ",";
    json += "\"yprevious\":" + enigma::JSON_serialization::enigma_serialize(yprevious) + ",";
    json += "\"xstart\":" + enigma::JSON_serialization::enigma_serialize(xstart) + ",";
    json += "\"ystart\":" + enigma::JSON_serialization::enigma_serialize(ystart) + ",";
#ifdef ISLOCAL_persistent
    json += "\"persistent\":" + enigma::JSON_serialization::enigma_serialize(persistent) + ",";
#endif
    json += "\"direction\":" + enigma::JSON_serialization::enigma_serialize(direction) + ",";
    json += "\"speed\":" + enigma::JSON_serialization::enigma_serialize(speed) + ",";
    json += "\"hspeed\":" + enigma::JSON_serialization::enigma_serialize(hspeed) + ",";
    json += "\"vspeed\":" + enigma::JSON_serialization::enigma_serialize(vspeed) + ",";
    json += "\"gravity\":" + enigma::JSON_serialization::enigma_serialize(gravity) + ",";
    json += "\"gravity_direction\":" + enigma::JSON_serialization::enigma_serialize(gravity_direction) + ",";
    json += "\"friction\":" + enigma::JSON_serialization::enigma_serialize(friction);

    json += "}";

    return json;
  }

  void object_planar::json_deserialize_self(const std::string &json) {
    std::string type = enigma::JSON_serialization::enigma_deserialize<std::string>(enigma::JSON_serialization::json_find_value(json,"object_type"));
    if (type != "object_planar") {
      DEBUG_MESSAGE("object_planar::json_deserialize_self: Object type '" + type +
                        "' does not match expected: object_planar",
                    MESSAGE_TYPE::M_FATAL_ERROR);
    }

    object_basic::json_deserialize_self(enigma::JSON_serialization::json_find_value(json,"object_basic"));

    x = enigma::JSON_serialization::enigma_deserialize<cs_scalar>(enigma::JSON_serialization::json_find_value(json,"x"));
    y = enigma::JSON_serialization::enigma_deserialize<cs_scalar>(enigma::JSON_serialization::json_find_value(json,"y"));
    xprevious = enigma::JSON_serialization::enigma_deserialize<cs_scalar>(enigma::JSON_serialization::json_find_value(json,"xprevious"));
    yprevious = enigma::JSON_serialization::enigma_deserialize<cs_scalar>(enigma::JSON_serialization::json_find_value(json,"yprevious"));
    xstart = enigma::JSON_serialization::enigma_deserialize<cs_scalar>(enigma::JSON_serialization::json_find_value(json,"xstart"));
    ystart = enigma::JSON_serialization::enigma_deserialize<cs_scalar>(enigma::JSON_serialization::json_find_value(json,"ystart"));
#ifdef ISLOCAL_persistent
    persistent = enigma::JSON_serialization::enigma_deserialize<bool>(enigma::JSON_serialization::json_find_value(json,"persistent"));
#endif
    direction = enigma::JSON_serialization::enigma_deserialize<variant>(enigma::JSON_serialization::json_find_value(json,"direction"));
    speed = enigma::JSON_serialization::enigma_deserialize<variant>(enigma::JSON_serialization::json_find_value(json,"speed"));
    hspeed = enigma::JSON_serialization::enigma_deserialize<variant>(enigma::JSON_serialization::json_find_value(json,"hspeed"));
    vspeed = enigma::JSON_serialization::enigma_deserialize<variant>(enigma::JSON_serialization::json_find_value(json,"vspeed"));
    gravity = enigma::JSON_serialization::enigma_deserialize<cs_scalar>(enigma::JSON_serialization::json_find_value(json,"gravity"));
    gravity_direction = enigma::JSON_serialization::enigma_deserialize<cs_scalar>(enigma::JSON_serialization::json_find_value(json,"gravity_direction"));
    friction = enigma::JSON_serialization::enigma_deserialize<cs_scalar>(enigma::JSON_serialization::json_find_value(json,"friction"));

    hspeed.vspd    = &vspeed.rval.d;
    hspeed.dir     = &direction.rval.d;
    hspeed.spd     = &speed.rval.d;

    vspeed.hspd    = &hspeed.rval.d;
    vspeed.dir     = &direction.rval.d;
    vspeed.spd     = &speed.rval.d;

    direction.spd  = &speed.rval.d;
    direction.hspd = &hspeed.rval.d;
    direction.vspd = &vspeed.rval.d;

    speed.dir      = &direction.rval.d;
    speed.hspd     = &hspeed.rval.d;
    speed.vspd     = &vspeed.rval.d;
  }

  object_planar object_planar::json_deserialize(const std::string& json){
    object_planar result;
    result.json_deserialize_self(json);
    return result;
  }

  void propagate_locals(object_planar* instance)
  {
    #ifdef PATH_EXT_SET // TODO(#997): this does not belong here...
      if (enigma_user::path_update()) {
        instance->speed = 0;
        return;
      }
    #endif

    if (fnzero(instance->gravity) || fnzero(instance->friction))
    {
      double
        hb4 = instance->hspeed.rval.d,
        vb4 = instance->vspeed.rval.d;
      int sign = (instance->speed > 0) - (instance->speed < 0);

      if (instance->hspeed != 0) {
        instance->hspeed.rval.d -= (sign * instance->friction)
            * cos(instance->direction.rval.d * M_PI/180);
      }
      if ((hb4 > 0 && instance->hspeed.rval.d < 0)
      ||  (hb4 < 0 && instance->hspeed.rval.d > 0)) {
        instance->hspeed.rval.d = 0;
      }
      if (instance->vspeed != 0) {
        instance->vspeed.rval.d -= (sign * instance->friction)
            * -sin(instance->direction.rval.d * M_PI/180);
      }
      if ((vb4 > 0 && instance->vspeed.rval.d < 0)
      ||  (vb4 < 0 && instance->vspeed.rval.d > 0)) {
        instance->vspeed.rval.d=0;
      }

      // XXX: The likely_if here is the == 270 case; the rest might not be worth
      // checking, as they're mostly just prolonging the inevitable
      if (fequal(instance->gravity_direction, 270)) {
        instance->vspeed.rval.d += (instance->gravity);
      } else if (fequal(instance->gravity_direction, 180)) {
        instance->hspeed.rval.d -= (instance->gravity);
      } else if (fequal(instance->gravity_direction, 90)) {
        instance->vspeed.rval.d -= (instance->gravity);
      } else if (fequal(instance->gravity_direction, 0)) {
        instance->hspeed.rval.d += (instance->gravity);
      } else {
        instance->hspeed.rval.d +=
            (instance->gravity) * cos(instance->gravity_direction * M_PI/180);
        instance->vspeed.rval.d +=
            (instance->gravity) *-sin(instance->gravity_direction * M_PI/180);
      }

      /*
      if(instance->speed.rval.d<0)
        //instance->direction.rval.d = fmod(instance->direction.rval.d + 180, 360),
        instance->speed.    rval.d = -hypotf(instance->hspeed.rval.d, instance->vspeed.rval.d);
      else
        instance->direction.rval.d = fmod(instance->direction.rval.d, 360),
        instance->speed.    rval.d =  hypotf(instance->hspeed.rval.d, instance->vspeed.rval.d);
      if(instance->direction.rval.d < 0)
        instance->direction.rval.d += 360;*/

      instance->speed.rval.d = instance->speed.rval.d < 0? -hypot(instance->hspeed.rval.d, instance->vspeed.rval.d) :
      hypot(instance->hspeed.rval.d, instance->vspeed.rval.d);
      if (fabs(instance->speed.rval.d) > 1e-12) {
        instance->direction.rval.d = fmod((atan2(-instance->vspeed.rval.d, instance->hspeed.rval.d) * (180/M_PI))
        + (instance->speed.rval.d < 0?  180 : 360), 360);
      }
    }
    instance->x += instance->hspeed.rval.d;
    instance->y += instance->vspeed.rval.d;
  }
}
