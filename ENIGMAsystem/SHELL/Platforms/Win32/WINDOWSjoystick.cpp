/** Copyright (C) 2013-2014 Robert B. Colton
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

#include <math.h>
#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

#include <string>
using std::string;

#include "Platforms/General/PFjoystick.h"
#include "Platforms/Win32/WINDOWSmain.h"

#include "Universal_System/scalar.h"
#include "Universal_System/math_consts.h"

namespace enigma_user
{

bool joystick_load(int id)
{
	JOYINFO joyinfo;
	UINT wNumDevs;

    if((wNumDevs = joyGetNumDevs()) == 0)
        return false;

    if (joySetCapture(enigma::hWnd, JOYSTICKID1 + id, 0, FALSE))
    {
        MessageBeep(MB_ICONEXCLAMATION);
        MessageBox(enigma::hWnd, "Couldn't capture the joystick.", NULL,
            MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

	return (joyGetPos(JOYSTICKID1 + id, &joyinfo) == JOYERR_NOERROR);
}

double joystick_axis(int id, int axisnum) {
	JOYINFOEX joyinfo;
	JOYCAPS joycaps;
    joyGetDevCaps(JOYSTICKID1 + id, &joycaps, sizeof(joycaps));
	switch (axisnum) {
		case 1:
			joyinfo.dwFlags = JOY_RETURNX | JOY_RETURNCENTERED;
			joyGetPosEx(JOYSTICKID1 + id, &joyinfo);
			return (joyinfo.dwXpos - (joycaps.wXmax + joycaps.wXmin) / 2.f) / ((joycaps.wXmax - joycaps.wXmin)/ 2.f);
			break;
		case 2:
			joyinfo.dwFlags = JOY_RETURNY | JOY_RETURNCENTERED;
			joyGetPosEx(JOYSTICKID1 + id, &joyinfo);
			return (joyinfo.dwYpos - (joycaps.wYmax + joycaps.wYmin) / 2.f) / ((joycaps.wYmax - joycaps.wYmin)/ 2.f);
			break;
		case 3:
			joyinfo.dwFlags = JOY_RETURNZ | JOY_RETURNCENTERED;
			joyGetPosEx(JOYSTICKID1 + id, &joyinfo);
			return (joyinfo.dwZpos - (joycaps.wZmax + joycaps.wZmin) / 2.f) / ((joycaps.wZmax - joycaps.wZmin)/ 2.f);
			break;
		case 4:
			joyinfo.dwFlags = JOY_RETURNR | JOY_RETURNCENTERED;
			joyGetPosEx(JOYSTICKID1 + id, &joyinfo);
			return (joyinfo.dwRpos - (joycaps.wRmax + joycaps.wRmin) / 2.f) / ((joycaps.wRmax - joycaps.wRmin)/ 2.f);
			break;
		case 5:
			joyinfo.dwFlags = JOY_RETURNU | JOY_RETURNCENTERED;
			joyGetPosEx(JOYSTICKID1 + id, &joyinfo);
			return (joyinfo.dwUpos - (joycaps.wUmax + joycaps.wUmin) / 2.f) / ((joycaps.wUmax - joycaps.wUmin)/ 2.f);
			break;
		case 6:
			joyinfo.dwFlags = JOY_RETURNV | JOY_RETURNCENTERED;
			joyGetPosEx(JOYSTICKID1 + id, &joyinfo);
			return (joyinfo.dwVpos - (joycaps.wVmax + joycaps.wVmin) / 2.f) / ((joycaps.wVmax - joycaps.wVmin)/ 2.f);
			break;
		default:
			return 0;
	}
}

bool joystick_button(int id, int buttonnum) {
	JOYINFOEX joyinfo;
	joyinfo.dwFlags = JOY_RETURNBUTTONS;
    joyGetPosEx(JOYSTICKID1 + id, &joyinfo);
	return (joyinfo.dwButtons & (JOY_BUTTON1 << buttonnum));
}

bool joystick_exists(int id) {
	JOYINFO joyinfo;
	UINT wNumDevs;
	BOOL bDevAttached;

    if((wNumDevs = joyGetNumDevs()) == 0)
        return false;
    bDevAttached = joyGetPos(JOYSTICKID1 + id, &joyinfo) == JOYERR_NOERROR;
	return bDevAttached;
}

string joystick_name(int id) {
	JOYCAPS joycaps;
    joyGetDevCaps(JOYSTICKID1 + id, &joycaps, sizeof(joycaps));
	return joycaps.szPname;
}

int joystick_axes(int id) {
	JOYCAPS joycaps;
    joyGetDevCaps(JOYSTICKID1 + id, &joycaps, sizeof(joycaps));
	return joycaps.wNumAxes;
}

int joystick_buttons(int id) {
	JOYCAPS joycaps;
    joyGetDevCaps(JOYSTICKID1 + id, &joycaps, sizeof(joycaps));
	return joycaps.wNumButtons;
}

bool joystick_has_pov(int id) {
	JOYCAPS joycaps;
    joyGetDevCaps(JOYSTICKID1 + id, &joycaps, sizeof(joycaps));
	return (joycaps.wCaps & JOYCAPS_HASPOV);
}

int joystick_direction(int id, int axis1, int axis2)
{
  JOYINFOEX joyinfo;
  joyGetPosEx(JOYSTICKID1 + id, &joyinfo);
  double a1, a2;
  a1 = joystick_axis(id, axis1);
  a2 = joystick_axis(id, axis2);
  const int x = a1 < -.5 ? 0 : a1 > .5 ? 2 : 1;
  const int y = a2 < -.5 ? 0 : a2 > .5 ? 6 : 3;
  return 97 + x + y;
}

double joystick_pov(int id) {
	JOYINFOEX joyinfo;
	joyinfo.dwFlags = JOY_RETURNPOV;
    joyGetPosEx(JOYSTICKID1 + id, &joyinfo);
	if (joyinfo.dwPOV != 0xFFFF)
		return joyinfo.dwPOV / 100.f;
	else
		return -1.f;
}

double joystick_pov(int id, int axis1, int axis2) {
	double a1, a2;
	a1 = joystick_axis(id, axis1);
	a2 = joystick_axis(id, axis2);
	if (a1 == 0 && a2 == 0) {
		return -1.f;
	}
	// in C, atan2 is y,x not x,y
	return ma_angle_from_radians((atan2(-a1, a2) + M_PI));
}

}
