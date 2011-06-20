/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

namespace enigma {
  void init_joysticks();
  void handle_joysticks();
}

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
void joystick_map_button(int id, int butnum, char key);
void joystick_map_axis(int id, int axisnum, char keyneg, char keypos);
bool joystick_load(int id);
