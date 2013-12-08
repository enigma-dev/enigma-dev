/******************************************************************************

 ENIGMA
 Copyright (C) 2008-2013 Enigma Strike Force

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#include "ert/object.hpp"

namespace ert {
  object::object(unsigned long id, real_t x, real_t y)
    : id(id), xstart(x), ystart(y), x(x), y(y), xprevious(x), yprevious(y), properties{} {
    this->initialize_properties(this->properties);
  }
  
  void object::initialize_properties(object::object_properties& prop) {
    prop.solid = this->object_is_solid();
    prop.visible = this->object_is_visible();
    prop.persistent = this->object_is_persistent();
    prop.depth = this->object_depth();
    prop.sprite_index = this->object_sprite_index();
    prop.mask_index = this->object_mask_index();
    prop.image_alpha = 1;
    prop.image_speed = 1;
    prop.image_xscale = 1;
    prop.image_yscale = 1;
  }
}
