/** Copyright (C) 2013 Robert B. Colton
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
using std::string;

namespace enigma_user {
	enum {
		gp_face1 = 0,
		gp_face2 = 1,
		gp_face3 = 2,
		gp_face4 = 3,
		gp_shoulderl = 4,
		gp_shoulderr = 5,
		gp_shoulderlb = 6,
		gp_shoulderrb = 7,
		gp_select = 8,
		gp_start = 9,
		gp_stickl = 10, 
		gp_stickr = 11,
		gp_padu = 12,
		gp_padd = 13,
		gp_padl = 14,
		gp_padr = 15,
		gp_axislh = 16, 
		gp_axislv = 17,
		gp_axisrh = 18,
		gp_axisrv = 19,
		
		gp_empty = 20,
		gp_low = 21, 
		gp_medium = 22,
		gp_full = 23,
		
		gp_disconnected = 24,
		gp_wired = 25,
		gp_alkaline = 26,
		gp_nimh = 27,
		gp_unknown = 28
	};
    bool gamepad_is_supported();
    bool gamepad_is_connected(int num);
	int gamepad_get_device_count();
	int gamepad_get_max_device_count();
    string gamepad_get_description(int device);
	int gamepad_get_battery_type(int device);
	int gamepad_get_battery_charge(int device);
    float gamepad_get_button_threshold(int device);
    void gamepad_set_button_threshold(int device, float threshold);
	void gamepad_set_vibration(int device, float left, float right);
    int gamepad_axis_count(int device);
    float gamepad_axis_value(int device, int axis);
    bool gamepad_button_check(int device, int button);
    bool gamepad_button_check_pressed(int device, int button);
    bool gamepad_button_check_released(int device, int button);
    int gamepad_button_count(int device);
    float gamepad_button_value(int device, int button);
}