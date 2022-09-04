/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2011-2012 polygone
*** Copyright (C) 2013 Robert B Colton, canthelp
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
#include "serialization.h"
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

    enigma_internal_serialize<unsigned char>(object_planar::objtype, len, bytes);
    enigma_internal_serialize(x, len, bytes);
    enigma_internal_serialize(y, len, bytes);
    enigma_internal_serialize(xprevious, len, bytes);
    enigma_internal_serialize(yprevious, len, bytes);
    enigma_internal_serialize(xstart, len, bytes);
    enigma_internal_serialize(ystart, len, bytes);
#ifdef ISLOCAL_persistent
    enigma_internal_serialize(persistent, len, bytes);
#endif
    enigma_internal_serialize(direction, len, bytes);
    enigma_internal_serialize(speed, len, bytes);
    enigma_internal_serialize(hspeed, len, bytes);
    enigma_internal_serialize(vspeed, len, bytes);
    enigma_internal_serialize(gravity, len, bytes);
    enigma_internal_serialize(gravity_direction, len, bytes);
    enigma_internal_serialize(friction, len, bytes);

    bytes.shrink_to_fit();
    return bytes;
  }

  std::size_t object_planar::deserialize_self(std::byte *iter) {
    auto len = object_basic::deserialize_self(iter);

    unsigned char type;
    enigma_internal_deserialize(type, iter, len);
    if (type != object_planar::objtype) {
      DEBUG_MESSAGE("object_planar::deserialize_self: Object type '" + std::to_string(type) +
                        "' does not match expected: " + std::to_string(object_planar::objtype),
                    MESSAGE_TYPE::M_FATAL_ERROR);
    }
    enigma_internal_deserialize(x, iter, len);
    enigma_internal_deserialize(y, iter, len);
    enigma_internal_deserialize(xprevious, iter, len);
    enigma_internal_deserialize(yprevious, iter, len);
    enigma_internal_deserialize(xstart, iter, len);
    enigma_internal_deserialize(ystart, iter, len);
#ifdef ISLOCAL_persistent
    enigma_internal_deserialize(persistent, iter, len);
#endif
    enigma_internal_deserialize_variant(direction, iter, len);
    enigma_internal_deserialize_variant(speed, iter, len);
    enigma_internal_deserialize_variant(hspeed, iter, len);
    enigma_internal_deserialize_variant(vspeed, iter, len);
    enigma_internal_deserialize(gravity, iter, len);
    enigma_internal_deserialize(gravity_direction, iter, len);
    enigma_internal_deserialize(friction, iter, len);

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
