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

namespace enigma_user
{

bool joystick_load(int id);
bool joystick_exists(int id);
string joystick_name(int id);
int joystick_axes(int id);
int joystick_buttons(int id);
bool joystick_has_pov(int id);
int joystick_direction(int id); // Numpad key style. WTF.
#define joystick_check_button(id, numb) joystick_button(id, numb)
#define joystick_xpos(id) joystick_axis(id,1)
#define joystick_ypos(id) joystick_axis(id,2)
#define joystick_zpos(id) joystick_axis(id,3)
#define joystick_rpos(id) joystick_axis(id,4)
#define joystick_upos(id) joystick_axis(id,5)
#define joystick_vpos(id) joystick_axis(id,6)
double joystick_pov(int id);

double joystick_axis(int id, int axis);
bool joystick_button(int id, int button);

}

