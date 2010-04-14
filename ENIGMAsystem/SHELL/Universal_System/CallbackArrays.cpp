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

namespace enigma
{
	char mousestatus[3];
	char last_mousestatus[3];
	char last_keybdstatus[256];
	char keybdstatus[256];
	short mousewheel;
}

bool mouse_check_button(double button)
{
	int b=(int)button;
	switch(b){
	case-1:return enigma::mousestatus[0]+enigma::mousestatus[1]+enigma::mousestatus[2]>0;
	case 0:return !(enigma::mousestatus[0]+enigma::mousestatus[1]+enigma::mousestatus[2]);
	case 1:case 2:case 3:return enigma::mousestatus[b-1];
	default:return 0;
	}
}

bool mouse_check_button_pressed(double button)
{
	int b=(int)button;
    switch(b){
    case-1:return (enigma::mousestatus[0]==1 && enigma::last_mousestatus[0]==0)
           ||(enigma::mousestatus[1]==1 && enigma::last_mousestatus[1]==0)
           ||(enigma::mousestatus[2]==1 && enigma::last_mousestatus[2]==0);
    case 0:return (enigma::mousestatus[0]!=1 || enigma::last_mousestatus[0]!=0)
           &&(enigma::mousestatus[1]!=1 || enigma::last_mousestatus[1]!=0)
           &&(enigma::mousestatus[2]!=1 || enigma::last_mousestatus[2]!=0);
    case 1:case 2:case 3:return enigma::mousestatus[b-1]==1 && enigma::last_mousestatus[b-1]==0;
    default:return 0;
    }
}

bool mouse_check_button_released(double button)
{
	int b=(int)button;
	switch(b){
	case-1:return (enigma::mousestatus[0]==0 && enigma::last_mousestatus[0]==1)
           ||(enigma::mousestatus[1]==0 && enigma::last_mousestatus[1]==1)
           ||(enigma::mousestatus[2]==0 && enigma::last_mousestatus[2]==1);
	case 0:return (enigma::mousestatus[0]!=0 || enigma::last_mousestatus[0]!=1)
           &&(enigma::mousestatus[1]!=0 || enigma::last_mousestatus[1]!=1)
           &&(enigma::mousestatus[2]!=0 || enigma::last_mousestatus[2]!=1);
	case 1:case 2:case 3:return enigma::mousestatus[b-1]==0 && enigma::last_mousestatus[b-1]==1;
	default:return 0;
	}
}

bool keyboard_check(double key)
{
	int b=(unsigned int)key;
    if(b==0){
		for(int i=0;i<255;i++)
			if(enigma::keybdstatus[i]==1) return 0;
		return 1;}
    if(b==-1){
		for(int i=0;i<255;i++)
			if (enigma::keybdstatus[i]==1) return 1;
		return 0;}
	return b<256 && enigma::keybdstatus[b]==1;
}

bool keyboard_check_pressed(double key)
{
	int b=(unsigned int)key;
    if(b==0){
		for(int i=0;i<255;i++)
			if(enigma::keybdstatus[i]==1 && enigma::last_keybdstatus[i]==0) return 0;
		return 1;}
    if(b==-1){
		for(int i=0;i<255;i++)
			if(enigma::keybdstatus[i]==1 && enigma::last_keybdstatus[i]==0) return 1;
		return 0;}
    return b<256 && enigma::keybdstatus[b]==1 && enigma::last_keybdstatus[b]==0;
}

bool keyboard_check_released(double key)
{
	int b=(unsigned int)key;
    if(b==0){
		for(int i=0;i<255;i++)
			if(enigma::keybdstatus[i]==0 && enigma::last_keybdstatus[i]==1) return 0;
		return 1;}
    if(b==1){
		for(int i=0;i<255;i++)
			if(enigma::keybdstatus[i]==0 && enigma::last_keybdstatus[i]==1) return 1;
		return 0;}
	return b<256 && enigma::keybdstatus[b]==0 && enigma::last_keybdstatus[b]==1;
}

