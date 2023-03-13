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

#ifndef ENIGMA_VIRTUALKEYS_H
#define ENIGMA_VIRTUALKEYS_H

namespace enigma {

void extension_virtualkeys_init();

} // namespace enigma

namespace enigma_user {

void virtual_key_set_keyboard_press_visible(bool visible);
bool virtual_key_get_keyboard_press_visible();
void virtual_key_set_press_visible(bool visible);
bool virtual_key_get_press_visible();
int virtual_key_add(int x, int y, int width, int height, int keycode);
void virtual_key_delete(int id);
bool virtual_key_exists(int id);
int virtual_key_get_x(int id);
int virtual_key_get_y(int id);
int virtual_key_get_width(int id);
int virtual_key_get_height(int id);
void virtual_key_set_area(int id, int x, int y, int width, int height);
int virtual_key_get_keycode(int id);
void virtual_key_set_keycode(int id, int keycode);
bool virtual_key_get_pressed(int id);
void virtual_key_set_visible(int id, bool visible);
bool virtual_key_get_visible(int id);
inline void virtual_key_show(int id) { virtual_key_set_visible(id, true); }
inline void virtual_key_hide(int id) { virtual_key_set_visible(id, false); }
void virtual_key_set_sprite(int id, int spr);
int virtual_key_get_sprite(int id, int spr);
void virtual_key_draw(int id);

} // namespace enigma_user

#endif // ENIGMA_VIRTUALKEYS_H
