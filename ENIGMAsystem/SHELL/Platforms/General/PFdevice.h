/** Copyright (C) 2014 Robert B. Colton
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

#ifndef ENIGMA_PLATFORM_DEVICE_H
#define ENIGMA_PLATFORM_DEVICE_H

namespace enigma_user
{

bool device_mouse_check_button(int device, int button);
bool device_mouse_check_button_pressed(int device, int button);
bool device_mouse_check_button_released(int device, int button);
void device_mouse_dbclick_enable(bool enable); // Studio's manual says this function returns a boolean but I believe it is a typo.
int device_mouse_x(int device);
int device_mouse_y(int device);
int device_mouse_raw_x(int device);
int device_mouse_raw_y(int device);
int device_mouse_x_to_gui(int device);
int device_mouse_y_to_gui(int device);

bool device_is_keypad_open();

float device_get_tilt_x();
float device_get_tilt_y();
float device_get_tilt_z();

}

#endif
