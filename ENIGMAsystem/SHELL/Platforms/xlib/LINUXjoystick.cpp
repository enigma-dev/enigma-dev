/*
 * Copyright (C) 2011 Josh Ventura
 *
 * This file is part of ENIGMA.
 *
 * ENIGMA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ENIGMA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License (COPYING) for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/joystick.h>
#include <stropts.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
using namespace std;

#include "LINUXjoystick.h"  
#include "../../Universal_System/CallbackArrays.h"  


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
        for (int i = 0; i < 8; i++)
          axis[i] = wrapaxis_positive[i] = wrapaxis_negative[i] = wrapbutton[i] = button[i] = 0;
        for (int i = 8; i < 16; i++)
          wrapbutton[i] = button[i] = 0;
    }
  };
  static vector<e_joystick*> joysticks;
  
  void init_joysticks()
  {
    joystick_load(1);
    joystick_load(2);
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
          float at = my_joystick->axis[a.number] = double(abs(a.value) > 5 ? a.value : 0) / 32767;
          //printf("Wrap: %d, %d\n",my_joystick->wrapaxis_negative[a.number],my_joystick->wrapaxis_positive[a.number]);
          if (a.value <= 0 and my_joystick->wrapaxis_negative[a.number])
            enigma::keybdstatus[(int)my_joystick->wrapaxis_negative[a.number]] = at < -my_joystick->threashold;
          if (a.value >= 0 and my_joystick->wrapaxis_positive[a.number])
            enigma::keybdstatus[(int)my_joystick->wrapaxis_positive[a.number]] = at > my_joystick->threashold;
        }
      else
      {
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
        handle_joystick(joysticks[i]);
  }
}


bool joystick_load(int id)
{
  string devn = "/dev/input/js";
  char dnum[16]; sprintf(dnum,"%d",id - 1);
  devn += dnum;
  int device = open(devn.c_str(), O_RDONLY|O_NONBLOCK);
  if (device == -1)
    return false;
  
  int ac = 4, bc = 4;
  ioctl(device, I_SRDOPT, RMSGN);
  if (ioctl(device, JSIOCGAXES, &ac) or ioctl(device, JSIOCGBUTTONS, &bc))
    return (close(device), false);
  
  char name[256]; name[0] = 0;
  if (ioctl(device, JSIOCGNAME(256), name) > 0)
    devn = name;
  printf("Joystick name: %s\n",name);
  
  while ((signed)enigma::joysticks.size() < id)
    enigma::joysticks.push_back(NULL);
  
  enigma::e_joystick* jsn = new enigma::e_joystick(device, devn, ac, bc);
  enigma::joysticks.push_back(jsn);
  enigma::handle_joystick(jsn);
  return true;
}

void joystick_map_button(int id, int butnum, char key)
{
  if ((unsigned)id >= enigma::joysticks.size())
    return;
  enigma::e_joystick *js = enigma::joysticks[id];
  if (js and butnum < js->buttoncount)
    js->wrapbutton[butnum] = key;
}

void joystick_map_axis(int id, int axisnum, char keyneg, char keypos)
{
  if ((unsigned)id >= enigma::joysticks.size())
    return;
  enigma::e_joystick *js = enigma::joysticks[id];
  if (js and axisnum < js->axiscount)
    js->wrapaxis_negative[axisnum] = keyneg,
    js->wrapaxis_positive[axisnum] = keypos;
}

double joystick_axis(int id, int axisnum) {
  const enigma::e_joystick * const js = enigma::joysticks[id];
  return js->axis[axisnum];
}
bool joystick_button(int id, int buttonnum) {
  const enigma::e_joystick * const js = enigma::joysticks[id];
  return js->button[buttonnum];
}

bool joystick_exists(int id) {
  const enigma::e_joystick * const js = enigma::joysticks[id];
  return js && js->device != -1;
}

string joystick_name(int id) {
  enigma::e_joystick * const js = enigma::joysticks[id];
  return js->name;
}

int joystick_axes(int id) {
  enigma::e_joystick * const js = enigma::joysticks[id];
  return js->axiscount;
}

int joystick_buttons(int id) {
  enigma::e_joystick * const js = enigma::joysticks[id];
  return js->buttoncount;
}

bool joystick_has_pov(int id) {
  return 0;
}
int joystick_direction(int id)
{
  enigma::e_joystick * const js = enigma::joysticks[id];
  if (js->axiscount < 2) return 0;
  const int x = js->axis[0] < -.5 ? 0 : js->axis[0] > .5 ? 2 : 1;
  const int y = js->axis[1] < -.5 ? 0 : js->axis[1] > .5 ? 6 : 3;
  return 97 + x + y;
}
double joystick_pov(int id) {
  return 0;
}
