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

/*
bool mouse_check_button(double button)
bool mouse_check_button_pressed(double button)
bool mouse_check_button_released(double button)
bool keyboard_check(double key)
bool keyboard_check_pressed(double key)
bool keyboard_check_released(double key)
*/


namespace enigma
{
  extern char mousestatus[3];
  extern char last_mousestatus[3];
  extern char last_keybdstatus[256];
  extern char keybdstatus[256];
}

enum {
  mb_any    = -1,
  mb_none   =  0,
  mb_left   =  1,
  mb_right  =  2,
  mb_middle =  3,
};

enum {
  vk_anykey = 1,
  vk_nokey  = 0,
  
  vk_left  = 37,
  vk_right = 39,
  vk_up    = 38,
  vk_down  = 40,
  
  vk_tab    = 9,
  vk_enter  = 13,
  vk_shift  = 16,
  vk_control= 17,
  vk_alt    = 18,
  vk_space  = 32,
  
  vk_numpad0 = 96,
  vk_numpad1 = 97,
  vk_numpad2 = 98,
  vk_numpad3 = 99,
  vk_numpad4 = 100,
  vk_numpad5 = 101,
  vk_numpad6 = 102,
  vk_numpad7 = 103,
  vk_numpad8 = 104,
  vk_numpad9 = 105,
  
  vk_multiply = 106,
  vk_add      = 107,
  vk_subtract = 109,
  vk_decimal  = 110,
  vk_divide   = 111,
  
  vk_f1  = 112,
  vk_f2  = 113,
  vk_f3  = 114,
  vk_f4  = 115,
  vk_f5  = 116,
  vk_f6  = 117,
  vk_f7  = 118,
  vk_f8  = 119,
  vk_f9  = 120,
  vk_f10 = 121,
  vk_f11 = 122,
  vk_f12 = 123,

  vk_backspace = 8,
  vk_escape    = 27,
  vk_pageup    = 33,
  vk_pagedown  = 34,
  vk_end       = 35,
  vk_home      = 36,
  vk_insert    = 45,
  vk_delete    = 46,
  
  // These are for check_direct only
  vk_lcontrol = 162,
  vk_rcontrol = 163,
  vk_lalt     = 164,
  vk_ralt     = 165,
  
  // This one's Windows only
  vk_printscreen = 42,
  
  //These are ENIGMA-only
  vk_caps   = 20,
  vk_scroll = 145,
  vk_pause  = 19,
  vk_lsuper = 91,
  vk_rsuper = 92,
};


extern double mouse_x, mouse_y;
extern int mouse_button, mouse_lastbutton;

bool mouse_check_button(int button);
bool mouse_check_button_pressed(int button);
bool mouse_check_button_released(int button);

extern short mouse_hscrolls;
extern short mouse_vscrolls;

bool keyboard_check(int key);
bool keyboard_check_pressed(int key);
bool keyboard_check_released(int key);

void io_clear();
