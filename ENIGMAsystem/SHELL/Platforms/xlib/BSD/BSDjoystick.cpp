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

#include "Platforms/General/PFjoystick.h"

namespace enigma
{
  void init_joysticks() {
    enigma_user::joystick_load(0);
    enigma_user::joystick_load(1);
  }

  void handle_joysticks() {

  }
}  // namespace enigma

namespace enigma_user {

  bool joystick_load(int id) {
    return false;
  }

  void joystick_map_button(int id, int butnum, char key) {

  }

  void joystick_map_axis(int id, int axisnum, char keyneg, char keypos) {

  }

  double joystick_axis(int id, int axisnum) {
    return 0;
  }
  bool joystick_button(int id, int buttonnum) {
    return false;
  }

  bool joystick_exists(int id) {
    return false;
  }

  string joystick_name(int id) {
    return "";
  }

  int joystick_axes(int id) {
    return 0;
  }

  int joystick_buttons(int id) {
    return 0;
  }

  bool joystick_has_pov(int id) {
    return false;
  }
  
  int joystick_direction(int id, int axis1, int axis2) {
    return 0;
  }
  
  double joystick_pov(int id, int axis1, int axis2) {
    return 0;
  }
  
  double joystick_pov(int id) {
    return 0;
  }
  
  int joystick_lastbutton = -1;
}
