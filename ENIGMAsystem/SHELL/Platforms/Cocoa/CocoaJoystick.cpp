/** Copyright (C) 2016 Rodrigo Rocha
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
using namespace std;

namespace enigma
{
  //stub
}

/* Note: empty stub functions to build Mac executables without errors (and no joystick support, of course) */
namespace enigma_user
{
  bool joystick_load(int id) {
    return false; //stub
  }
  bool joystick_exists(int id) {
    return false; //stub
  }
  string joystick_name(int id) {
    return "stub"; //stub
  }
  int joystick_axes(int id) {
    return 0; //stub
  }
  int joystick_buttons(int id) {
    return 0; //stub
  }
  bool joystick_has_pov(int id) {
    return false; //stub
  }
  int joystick_direction(int id, int axis1=0, int axis2=1) {
    return 0;
  }
  double joystick_pov(int id) {
    return -1.0;
  }
  double joystick_pov(int id, int axis1, int axis2) {
    return -1.0;
  }

  double joystick_axis(int id, int axis) {
    return 0.0; //stub
  }
  bool joystick_button(int id, int button) {
    return false; //stub
  }

  void joystick_map_button(int id, int butnum, char key) {
    //stub
  }
  void joystick_map_axis(int id, int axisnum, char keyneg, char keypos) {
    //stub
  }
  
  int joystick_lastbutton = -1;
}
