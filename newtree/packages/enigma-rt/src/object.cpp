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
    : id(id), xstart(x), ystart(y), x(x), y(y) {
  }
  
  property_ro<object, unsigned long, &object::get_object_index> object::object_index() {
    return {this};
  }
  
  property_ro<object, real_t, &object::get_image_xscale> object::image_xscale() {
    return {this};
  }
}
