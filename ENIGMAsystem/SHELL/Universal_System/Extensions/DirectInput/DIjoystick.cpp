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

#include <windows.h>
#include <dinput.h>
#include "DIjoystick.h"  
#include "Platforms/Win32/WINDOWSmain.h"

namespace enigma_user
{

bool joystick_load(int id)
{
	JOYINFO joyinfo; 
	UINT wNumDevs; 
	BOOL bDevAttached; 
 
    if((wNumDevs = joyGetNumDevs()) == 0) 
        return false; 
		
    if (joySetCapture(enigma::hWnd, JOYSTICKID1 + id, NULL, FALSE)) 
    { 
        MessageBeep(MB_ICONEXCLAMATION); 
        MessageBox(enigma::hWnd, "Couldn't capture the joystick.", NULL, 
            MB_OK | MB_ICONEXCLAMATION); 
        return false;
    } 
	
	bDevAttached = joyGetPos(JOYSTICKID1 + id, &joyinfo) != JOYERR_UNPLUGGED; 
    if (!bDevAttached) 
        return false;
		
	return true;
}

double joystick_axis(int id, int axisnum) {
	JOYINFOEX joyinfo; 
    joyGetPosEx(JOYSTICKID1 + id, &joyinfo); 
	switch (axisnum) {
		case 1:
			return joyinfo.dwXpos;
			break;
		case 2:
			return joyinfo.dwYpos;
			break;
		case 3: 
			return joyinfo.dwZpos;
			break;
		case 4:
			return joyinfo.dwRpos;
			break;
		case 5:
			return joyinfo.dwUpos;
			break;
		case 6: 
			return joyinfo.dwVpos;
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

int joystick_direction(int id)
{
  JOYINFOEX joyinfo; 
  joyGetPosEx(JOYSTICKID1 + id, &joyinfo); 		
  const int x = joyinfo.dwXpos < -.5 ? 0 : joyinfo.dwXpos > .5 ? 2 : 1;
  const int y = joyinfo.dwYpos < -.5 ? 0 : joyinfo.dwYpos > .5 ? 6 : 3;
  return 97 + x + y;
}

double joystick_pov(int id) {
	JOYINFOEX joyinfo; 
    joyGetPosEx(JOYSTICKID1 + id, &joyinfo); 
	return joyinfo.dwPOV / 100;
}

}

