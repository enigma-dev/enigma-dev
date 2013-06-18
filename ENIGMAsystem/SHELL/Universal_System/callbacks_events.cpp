/** Copyright (C) 2013 forthevin
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

#include <list>

namespace enigma
{
  using std::list;

  typedef void (*callback_t )();

  list<callback_t> before_collision_callbacks;

  void perform_callbacks_before_collision_event() {
    list<callback_t>::iterator it_end = before_collision_callbacks.end();
    for (list<callback_t>::iterator it = before_collision_callbacks.begin(); it != it_end; it++) {
      (*it)();
    }
  }

  void register_callback_before_collision_event(callback_t callback) {
    before_collision_callbacks.push_back(callback);
  }
}

