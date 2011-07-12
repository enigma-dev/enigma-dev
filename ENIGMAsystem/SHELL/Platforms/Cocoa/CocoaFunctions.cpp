/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>                      **
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

#include "CocoaFunctions.h"
#include "CocoaWindow.h"
#include <stdio.h>
#include "../../Universal_System/CallbackArrays.h"
#include "../../Universal_System/roomsystem.h"

namespace enigma
{
	extern char keymap[256];
	void ENIGMA_events(void);
	extern int initialize_everything();
}

int handleEvents()
{
}

namespace enigma
{
	void initkeymap();
	void input_initialize()
	{
		//Clear the input arrays
		for(int i=0;i<3;i++){
			enigma::last_mousestatus[i]=0;
			enigma::mousestatus[i]=0;
		}
		for(int i=0;i<256;i++){
			enigma::last_keybdstatus[i]=0;
			enigma::keybdstatus[i]=0;
		}
	}
}

char cocoa_last_keybdstatus[256];
char cocoa_keybdstatus[256];
char cocoa_mousestatus[3];
char cocoa_last_mousestatus[3];



int loopy() {
	
	enigma::ENIGMA_events();
	
	cocoa_io_handle();
	

	return 0;
}


int init() {
enigma::initialize_everything();
	enigma::initkeymap();
}




void key_press(int keycode) {
    //printf("Keycode pressed: %d",keycode); //useful to create the keymap array
	int actualKey = enigma::keymap[keycode];
	
	if (cocoa_keybdstatus[actualKey]==1) {
		cocoa_last_keybdstatus[actualKey]=1; //its already handeled the key press
		
	} else {
		cocoa_keybdstatus[actualKey]=1;
	cocoa_last_keybdstatus[actualKey]=0; //handle key press
		
	}
	
	
}
void key_release(int keycode) {
	int actualKey =enigma::keymap[keycode];
	cocoa_keybdstatus[actualKey]=0;
}

void mouse_press(int x, int y) {
    
    if (cocoa_mousestatus[mb_left-1]==1) {
		cocoa_last_mousestatus[mb_left-1]=1; //its already handeled the mouse press
		
	} else {
		cocoa_mousestatus[mb_left-1]=1;
        cocoa_last_mousestatus[mb_left-1]=0; //handle mouse press
		
	}
    
}



void mouse_release(int x, int y) {
	cocoa_mousestatus[mb_left-1]=0;
}

void mouse_right_press(int x, int y) {
	cocoa_mousestatus[mb_right-1]=1;
}

void mouse_right_release(int x, int y) {
	cocoa_mousestatus[mb_right-1]=0;
}

void cocoa_io_handle() {
    
	for(int i=0;i<256;i++){
		if (enigma::last_keybdstatus[i]==0 && enigma::keybdstatus[i]==1) {
            //in the last frame, i was pressed event, so make last_keybdstatus now equal 1
			cocoa_last_keybdstatus[i]=1;
		}
		else if (enigma::last_keybdstatus[i]==1 && enigma::keybdstatus[i]==0) {
			//in the last frame, i was released event, so make last_keybdstatus now equal 0
			cocoa_last_keybdstatus[i]=0;
		}
	}
    for(int i=0;i<3;i++){
		if (enigma::last_mousestatus[i]==0 && enigma::mousestatus[i]==1) {
            //in the last frame, i was pressed event, so make last_keybdstatus now equal 1
			cocoa_last_mousestatus[i]=1;
		}
		else if (enigma::last_mousestatus[i]==1 && enigma::mousestatus[i]==0) {
			//in the last frame, i was released event, so make last_keybdstatus now equal 0
			cocoa_last_mousestatus[i]=0;
		}
	}
	memcpy(enigma::keybdstatus, cocoa_keybdstatus, sizeof(enigma::keybdstatus));
	memcpy(enigma::last_keybdstatus, cocoa_last_keybdstatus, sizeof(enigma::last_keybdstatus));
    memcpy(enigma::mousestatus, cocoa_mousestatus, sizeof(enigma::mousestatus));
	memcpy(enigma::last_mousestatus, cocoa_last_mousestatus, sizeof(enigma::last_mousestatus));
}





