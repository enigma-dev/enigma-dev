/** Copyright (C) 2014 Harijs Grinbergs
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

#ifndef BGUI_GROUPS_H
#define BGUI_GROUPS_H

#include <vector>

namespace enigma {
namespace gui {
class Group {
 public:
  unsigned int id;

  std::vector<unsigned int> group_buttons;
  std::vector<unsigned int> group_toggles;
  std::vector<unsigned int> group_windows;
};
}  //namespace gui
}  //namespace enigma

#endif
