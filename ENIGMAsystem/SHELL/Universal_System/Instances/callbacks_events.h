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

#ifndef ENIGMA_CALLBACKS_EVENTS_H
#define ENIGMA_CALLBACKS_EVENTS_H

namespace enigma {
  // Before collision event.
  void perform_callbacks_before_collision_event();
  void register_callback_before_collision_event(void (*callback)());

  // Particle updating.
  void perform_callbacks_particle_updating();
  void register_callback_particle_updating(void (*callback)());

  // Clean up room-end.
  void perform_callbacks_clean_up_roomend();
  void register_callback_clean_up_roomend(void (*callback)());
}

#endif // ENIGMA_CALLBACKS_EVENTS_H
