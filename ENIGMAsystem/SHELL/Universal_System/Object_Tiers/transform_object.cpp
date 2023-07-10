/** Copyright (C) 2010-2011 Josh Ventura
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

#include "transform_object.h"

#include "Widget_Systems/widgets_mandatory.h"
#include "../Serialization/serialization.h"

namespace enigma {
object_transform::object_transform() : object_graphics() {}
object_transform::object_transform(unsigned _x, int _y) : object_graphics(_x, _y) {}
object_transform::~object_transform() {}

std::vector<std::byte> object_transform::serialize() {
  auto bytes = object_graphics::serialize();
  std::size_t len = 0;

  enigma_serialize<unsigned char>(object_transform::objtype, len, bytes);
  enigma_serialize(image_alpha, len, bytes);
  enigma_serialize(image_blend, len, bytes);

  bytes.shrink_to_fit();
  return bytes;
}

std::size_t object_transform::deserialize_self(std::byte *iter) {
  auto len = object_graphics::deserialize_self(iter);

  unsigned char type;
  enigma_deserialize(type, iter, len);
  if (type != object_transform::objtype) {
    DEBUG_MESSAGE("object_transform::deserialize_self: Object type '" + std::to_string(type) +
                      "' does not match expected: " + std::to_string(object_transform::objtype),
                  MESSAGE_TYPE::M_FATAL_ERROR);
  }
  enigma_deserialize(image_alpha, iter, len);
  enigma_deserialize(image_blend, iter, len);

  return len;
}

std::pair<object_transform, std::size_t> object_transform::deserialize(std::byte *iter) {
  object_transform result;
  auto len = result.deserialize_self(iter);
  return {std::move(result), len};
}
}  // namespace enigma
