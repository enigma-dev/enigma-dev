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
    char mousestatus[3];
    char last_mousestatus[3];
    char last_keybdstatus[256];
    char keybdstatus[256];
    short mousewheel;
}

const int mb_any    =-1;
const int mb_none   =0;
const int mb_left   =1;
const int mb_right  =2;
const int mb_middle =3;

const int vk_left  =37;
const int vk_right =39;
const int vk_up    =38;
const int vk_down  =40;

const int vk_enter  =13;
const int vk_shift  =16;
const int vk_control=17;
const int vk_alt    =18;
const int vk_space  =32;

const int vk_numpad0=96;
const int vk_numpad1=97;
const int vk_numpad2=98;
const int vk_numpad3=99;
const int vk_numpad4=100;
const int vk_numpad5=101;
const int vk_numpad6=102;
const int vk_numpad7=103;
const int vk_numpad8=104;
const int vk_numpad9=105;

const int vk_multiply=106;
const int vk_add     =107;
const int vk_subtract=109;
const int vk_decimal =110;
const int vk_divide  =111;

const int vk_f1 =112;
const int vk_f2 =113;
const int vk_f3 =114;
const int vk_f4 =115;
const int vk_f5 =116;
const int vk_f6 =117;
const int vk_f7 =118;
const int vk_f8 =119;
const int vk_f9 =120;
const int vk_f10=121;
const int vk_f11=122;
const int vk_f12=123;

const int vk_backspace=8;
const int vk_escape   =27;
const int vk_pageup   =33;
const int vk_pagedown =34;
const int vk_end      =35;
const int vk_home     =36;
const int vk_insert   =45;
const int vk_delete   =46;


bool mouse_check_button(double button);
bool mouse_check_button_pressed(double button);
bool mouse_check_button_released(double button);

bool keyboard_check(double key);
bool keyboard_check_pressed(double key);
bool keyboard_check_released(double key);
