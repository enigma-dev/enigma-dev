/** Copyright (C) 2025 Samuel Venable
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

#include <string>

#include "Platforms/platforms_mandatory.h"
#include "Universal_System/var4.h"

namespace enigma_user {

  int capture_add(enigma::rvt window);
  bool capture_exists(int ind);
  bool capture_delete(int ind);
  int capture_get_width(int ind);
  int capture_get_height(int ind);
  bool capture_grab_frame_buffer(int ind, void *buffer);
  bool capture_update(int ind);
  int capture_create_window_list();
  bool capture_window_list_exists(int list);
  wid_t capture_get_window_id(int list, int ind);
  int capture_get_window_id_length(int list);
  bool capture_destroy_window_list(int list);
  std::string capture_get_window_caption(wid_t window);
  bool capture_get_fixedsize(int ind);
  bool capture_set_fixedsize(int ind, bool fixed);
  window_t capture_native_window_from_window_id(wid_t window);
  wid_t capture_window_id_from_native_window(enigma::rvt window);

} // namespace enigma_user
