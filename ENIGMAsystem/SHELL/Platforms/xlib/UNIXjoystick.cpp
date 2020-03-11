/** Copyright (C) 2011-2012 Josh Ventura
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


#include "LINUXjoystick.h"  

#include "Platforms/General/PFwindow.h"  
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/scalar.h"
#include "Universal_System/make_unique.h"

#ifdef __linux__
#include <linux/joystick.h>
#else // __FreeBSD__
#include <machine/joystick.h>
#endif

#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <sys/stat.h>

using namespace std;

namespace enigma
{
  struct e_joystick
  {
    int device;
    string name;
    int axiscount;
    int buttoncount;
    
    float *axis;
    bool *button;
    
    float threashold;
    char *wrapaxis_positive;
    char *wrapaxis_negative;
    char *wrapbutton;
    
    e_joystick(int d, string n, int ac, int bc): device(d), name(n), axiscount(ac), buttoncount(bc),
      axis(new float[ac]), button(new bool[bc]), threashold(.25),
      wrapaxis_positive(new char[ac]), wrapaxis_negative(new char[ac]), wrapbutton(new char[bc])
    {
        for (int i = 0; i < ac; i++)
          axis[i] = wrapaxis_positive[i] = wrapaxis_negative[i] = 0;
        for (int i = 0; i < bc; i++)
          wrapbutton[i] = button[i] = 0;
    }
    
    ~e_joystick() {
      close(device);
      delete[] axis;
      delete[] button;
      delete[] wrapaxis_positive;
      delete[] wrapaxis_negative;
      delete[] wrapbutton;
    }
  };
  
  static vector<std::unique_ptr<e_joystick>> joysticks;
  
  void init_joysticks() {
    enigma_user::joystick_load(0);
    enigma_user::joystick_load(1);
  }
  
  static void handle_joystick(e_joystick *my_joystick)
  {
    js_event a;
    int rd = read(my_joystick->device, &a, sizeof(a));
    a.type &= ~JS_EVENT_INIT;
    while (rd and rd != -1)
    {
      if (rd != sizeof(a)) {
        //printf("Joystick error!\n");
        return;
      }
      if (a.type != JS_EVENT_BUTTON)
        if (a.type != JS_EVENT_AXIS)
          return;
        else
        {
          //printf("Axis %d rotated in %d\n", a.number, a.value);
          float at = my_joystick->axis[a.number] = (abs(a.value) > 5 ? double(a.value) / 32767. : 0.);
          //printf("Wrap: %d, %d\n",my_joystick->wrapaxis_negative[a.number],my_joystick->wrapaxis_positive[a.number]);
          if (a.value <= 0 and my_joystick->wrapaxis_negative[a.number])
            enigma::keybdstatus[(int)my_joystick->wrapaxis_negative[a.number]] = at < -my_joystick->threashold;
          if (a.value >= 0 and my_joystick->wrapaxis_positive[a.number])
            enigma::keybdstatus[(int)my_joystick->wrapaxis_positive[a.number]] = at > my_joystick->threashold;
        }
      else
      {
        enigma_user::joystick_lastbutton = a.number;
        //printf("Button %d pressed: %d\n", a.number, a.value);
        my_joystick->button[a.number] = a.value;
        //printf("Wrap: %d\n",my_joystick->wrapbutton[a.number]);
        if (my_joystick->wrapbutton[a.number])
          enigma::keybdstatus[(int)my_joystick->wrapbutton[a.number]] = a.value;
      }
      rd = read(my_joystick->device,&a,sizeof(a));
      a.type &= ~JS_EVENT_INIT;
    }
  }
  void handle_joysticks()
  {
    for (unsigned i = 0; i < joysticks.size(); i++)
      if (joysticks[i])
        handle_joystick(joysticks[i].get());
  }
}  // namespace enigma

namespace {
  string device_name(int js_id) {
    char sps[32];
    sprintf(sps, "/dev/input/js%d", js_id);
    return sps;
  }
}  // namespace

namespace enigma_user
{
  
  #ifdef DEBUG_MODE
  #  define checkId(failv) { if (size_t(id) >= enigma::joysticks.size()) { return (failv); } }
  #  define checkPositiveId(failv) { if (id < 0) return (failv); }
  #else
  #  define checkId(failv)
  #  define checkPositiveId(failv)
  #endif

  bool joystick_load(int id)
  {
    checkPositiveId(false);
    
    string devn = device_name(id);
    int device = open(devn.c_str(), O_RDONLY|O_NONBLOCK);
    if (device == -1)
      return false;
    
    int ac = 4, bc = 4;
    if (ioctl(device, JSIOCGAXES, &ac) or ioctl(device, JSIOCGBUTTONS, &bc))
      return (close(device), false);
    
    char name[256] = {0};
    if (ioctl(device, JSIOCGNAME(256), name) != -1)
      devn = name;

    #ifdef DEBUG_MODE
    DEBUG_MESSAGE(std::string("Joystick name: ") + name, MESSAGE_TYPE::M_INFO);
    #endif

    // There is no guarantee that because /dev/js5 exists, /dev/js0 exists
    if (enigma::joysticks.size() <= id) enigma::joysticks.resize(id);
    enigma::joysticks.emplace(enigma::joysticks.begin() + id, make_unique<enigma::e_joystick>(device, devn, ac, bc));
    enigma::handle_joystick(enigma::joysticks[id].get());
    return true;
  }

  void joystick_map_button(int id, int butnum, char key)
  {
    checkId(void());
    if ((unsigned)id >= enigma::joysticks.size())
      return;
    enigma::e_joystick *js = enigma::joysticks[id].get();
    if (js and butnum < js->buttoncount)
      js->wrapbutton[butnum] = key;
  }

  void joystick_map_axis(int id, int axisnum, char keyneg, char keypos)
  {
    checkId(void());
    if ((unsigned)id >= enigma::joysticks.size())
      return;
    enigma::e_joystick *js = enigma::joysticks[id].get();
    if (js and axisnum < js->axiscount)
      js->wrapaxis_negative[axisnum] = keyneg,
      js->wrapaxis_positive[axisnum] = keypos;
  }

  double joystick_axis(int id, int axisnum) {
    checkId(0);
    const enigma::e_joystick * const js = enigma::joysticks[id].get();
    return js->axis[axisnum];
  }
  bool joystick_button(int id, int buttonnum) {
    checkId(0);
    const enigma::e_joystick * const js = enigma::joysticks[id].get();
    return js->button[buttonnum];
  }

  bool joystick_exists(int id) {
    checkPositiveId(false);
    if (size_t(id) < enigma::joysticks.size()) {
      const enigma::e_joystick * const js = enigma::joysticks[id].get();
      if (js && js->device != -1) return true;
    }
    struct stat ignore;
    return (stat(device_name(id).c_str(), &ignore) == 0);
  }

  string joystick_name(int id) {
    checkId("");
    enigma::e_joystick * const js = enigma::joysticks[id].get();
    return js->name;
  }

  int joystick_axes(int id) {
    checkId(0);
    enigma::e_joystick * const js = enigma::joysticks[id].get();
    return js->axiscount;
  }

  int joystick_buttons(int id) {
    checkId(0);
    enigma::e_joystick * const js = enigma::joysticks[id].get();
    return js->buttoncount;
  }

  bool joystick_has_pov(int id) {
    checkId(false);
    return false;
  }
  
  int joystick_direction(int id, int axis1, int axis2)
  {
    checkId(0);
    enigma::e_joystick * const js = enigma::joysticks[id].get();
    if (js->axiscount < 2) return 0;
    const int x = js->axis[axis1] < -.5 ? 0 : js->axis[axis1] > .5 ? 2 : 1;
    const int y = js->axis[axis2] < -.5 ? 0 : js->axis[axis2] > .5 ? 6 : 3;
    return 97 + x + y;
  }
  
  double joystick_pov(int id, int axis1, int axis2) {
	checkId(0);
	double a1, a2;
	a1 = joystick_axis(id, axis1);
	a2 = joystick_axis(id, axis2);
	if (a1 == 0 && a2 == 0) {
		return -1.f;
	}
	// in C, atan2 is y,x not x,y
	return ma_angle_from_radians((atan2(-a1, a2) + M_PI));
  }
  
  double joystick_pov(int id) {
    checkId(0);
    return joystick_pov(id, 0, 1);
  }
  
  int joystick_lastbutton = -1;
}

