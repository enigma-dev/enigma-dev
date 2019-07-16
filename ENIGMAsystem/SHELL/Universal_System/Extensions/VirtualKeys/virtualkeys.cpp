/** Copyright (C) 2019 Robert B. Colton
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

#include "virtualkeys.h"
#include "VirtualKey.h"
#include "Platforms/General/PFwindow.h" // keyboard/mouse
#include "Platforms/platforms_mandatory.h" // extension_update_hooks
#include "Universal_System/mathnc.h" // point_in_rectangle

using namespace enigma_user;

namespace {

AssetArray<VirtualKey> virtual_keys;

void process_virtualkeys() {
  for (int i = 0; size_t(i) < virtual_keys.size(); ++i) {
    if (!virtual_keys.exists(i)) continue;
    VirtualKey& vk = virtual_keys.get(i);

    if (!point_in_rectangle(mouse_x, mouse_y, vk.x, vk.y, vk.x + vk.width, vk.y + vk.height))
      continue;

    if (mouse_check_button_pressed(mb_left)) {
      keyboard_key_press(vk.keycode);
    } else if (mouse_check_button_released(mb_left)) {
      keyboard_key_release(vk.keycode);
    } else if (mouse_check_button(mb_left)) {

    }
  }
}

}

namespace enigma {

void extension_virtualkeys_init() {
  extension_update_hooks.push_back(process_virtualkeys);
}

} // namespace enigma

namespace enigma_user {

int virtual_key_add(int x, int y, int width, int height, int keycode) {
  VirtualKey vk;
  vk.x = x;
  vk.y = y;
  vk.width = width;
  vk.height = height;
  vk.keycode = keycode;

  return virtual_keys.add(std::move(vk));
}

void virtual_key_show(int id) {

}

void virtual_key_hide(int id) {

}

void virtual_key_delete(int id) {
  virtual_keys.destroy(id);
}

bool virtual_key_exists(int id) {
  return virtual_keys.exists(id);
}

}
