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

#include "AndroidFunctions.h"
#include "AndroidWindow.h"
#include <stdio.h>
#include "../../Universal_System/CallbackArrays.h"
#include "../../Universal_System/roomsystem.h"
#include <android/log.h>
#define  LOG_TAG    "libenigma"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

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

int loopy() {
	enigma::initkeymap();
	enigma::ENIGMA_events();
	return 0;
}


int init() {
	LOGE("Before initilise everything!",1,1);
	enigma::parameters=new char* [0]; 
	enigma::parameters[0]="/data/data/org.enigmadev/lib/libndkEnigmaGame.so";
enigma::initialize_everything();
}
