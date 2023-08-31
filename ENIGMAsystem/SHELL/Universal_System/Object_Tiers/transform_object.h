/** Copyright (C) 2010-2011 Josh Ventura
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
  @file  transform_object.cpp
  @brief Adds a rotation, blending, and stretching tier following the graphics tier.
*/

#ifndef ENIGMA_TRANSFORM_OBJECT_H
#define ENIGMA_TRANSFORM_OBJECT_H

#include "graphics_object.h"

namespace enigma
{
  struct object_transform: object_graphics
  {
    static constexpr unsigned char objtype = 0xAE;

    //Other attributes that logically don't belong here
        double image_alpha;
        int    image_blend;
    //Constructors
      object_transform();
      object_transform(unsigned x, int y);
      virtual ~object_transform();

    // Bytes (de)Serialization
      std::vector<std::byte> serialize() override;
      std::size_t deserialize_self(std::byte *iter) override;
      static std::pair<object_transform, std::size_t> deserialize(std::byte *iter);

    // JSON (de)Serialization
      std::string json_serialize() const override;
      void json_deserialize_self(const std::string& json) override;
      static object_transform josn_deserialize(const std::string& json);
  };
} //namespace ennigma

#endif //ENIGMA_TRANSFORM_OBJECT_H