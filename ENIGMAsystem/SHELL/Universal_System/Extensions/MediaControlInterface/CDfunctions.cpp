/** Copyright (C) 2013 Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY {
} without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "CDfunctions.h"

#include <stdio.h>
#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

#include <mmsystem.h>

namespace enigma_user {

string MCI_command(string str) {
	string ret;
	mciSendString(str.c_str(), &ret[0], 0, NULL); 
	return ret;
}

void cd_open_door() {
	mciSendString("set cdaudio door open wait", NULL, 0, NULL);
}

void cd_close_door() {
	mciSendString("set cdaudio door closed wait", NULL, 0, NULL);
}

void cd_init() {
	mciSendString("open cdaudio", NULL, 0, NULL); 
	//mciSendString("close cdaudio", NULL, 0, NULL); 
}

unsigned long cd_length() {

}

unsigned cd_number() {
	unsigned count;
    MCIERROR mciError;
    TCHAR mciReturnBuffer[512];
    mciError = mciSendString("status cdaudio number of tracks", mciReturnBuffer, sizeof(mciReturnBuffer), NULL);
    if (mciError != 0)
    {
        //TODO: Show error message
		// HRESULT_FROM_WIN32(mciError)
        return 0;
    }
    sscanf(mciReturnBuffer, "%d", &count);
	return count;
}

void cd_play(unsigned first, unsigned last) {
	mciSendString("set cdaudio time format tmsf", NULL, 0, NULL);

	if (first == 1 && last == 1000) {
		mciSendString("play cdaudio", NULL, 0, NULL);
		return;
	}

	unsigned last_track = cd_number();
	if (last > last_track) { last = last_track; }

    TCHAR achCommandBuff[128]; 
    int result;
    MCIERROR err;

    // Form the command string.
	sprintf(achCommandBuff, "play cdaudio from %u to %u", first, last);

    if (result == -1) {
		//TODO: show error message
        return;
    }

    // Send the command string.
    err = mciSendString(achCommandBuff, NULL, 0, NULL); 
    if (err != 0) {
		//TODO: show error message
        return;
    }
	
    return;
}

void cd_pause() {
	mciSendString("pause cdaudio", NULL, 0, NULL);
}

void cd_resume() {
	mciSendString("resume cdaudio", NULL, 0, NULL); 
}

void cd_stop() {
	mciSendString("stop cdaudio", NULL, 0, NULL); 
}

bool cd_paused() {
    MCIERROR mciError;
    TCHAR mciReturnBuffer[512];

    mciError = mciSendString("status cdaudio mode", mciReturnBuffer, sizeof(mciReturnBuffer), NULL);
    if (mciError != 0)
    {
        //TODO: Show error message
		//HRESULT_FROM_WIN32(mciError)
        return false;
    }
    return (stricmp(mciReturnBuffer, "paused") == 0 || stricmp(mciReturnBuffer, "stopped") == 0);
}

bool cd_playing() {
    MCIERROR mciError;
    TCHAR mciReturnBuffer[512];

    mciError = mciSendString("status cdaudio mode", mciReturnBuffer, sizeof(mciReturnBuffer), NULL);
    if (mciError != 0)
    {
        //TODO: Show error message
		//HRESULT_FROM_WIN32(mciError)
        return false;
    }
    return (stricmp(mciReturnBuffer, "playing") == 0);
}

bool cd_present() {
    MCIERROR mciError;
    TCHAR mciReturnBuffer[512];

    mciError = mciSendString("status cdaudio media present", mciReturnBuffer, sizeof(mciReturnBuffer), NULL);
    if (mciError != 0)
    {
        //TODO: Show error message
		//HRESULT_FROM_WIN32(mciError)
        return false;
    }
    return (stricmp(mciReturnBuffer, "false") != 0);
}

unsigned long cd_position() {
	mciSendString("set cdaudio time format milliseconds", NULL, 0, NULL);
	unsigned long position;
	MCIERROR mciError;
	TCHAR mciReturnBuffer[512];
	mciError = mciSendString("status cdaudio position", mciReturnBuffer, sizeof(mciReturnBuffer), NULL);
    if (mciError != 0)
    {
        //TODO: Show error message
		// HRESULT_FROM_WIN32(mciError)
        return 0;
    }
	sscanf(mciReturnBuffer, "%d", &position);
	return position;
}

void cd_set_position(unsigned long pos) {
	bool was_playing = cd_playing();

	mciSendString("set cdaudio time format milliseconds", NULL, 0, NULL);
	mciSendString("set seek exactly on", NULL, 0, NULL);
    TCHAR achCommandBuff[128]; 
    int result;
    MCIERROR err;
	
    // Form the command string.
    result = sprintf(achCommandBuff, "seek cdaudio to %u", pos); 

    if (result == -1) {
		//TODO: show error message
        return;
    }

    // Send the command string.
    err = mciSendString(achCommandBuff, NULL, 0, NULL); 
    if (err != 0) {
		//TODO: show error message
        return;
    }

	if (was_playing) {
		mciSendString("play cdaudio", NULL, 0, NULL);
	}
	
    return;
}

void cd_set_track_position(unsigned long pos) {
	mciSendString("set cdaudio time format milliseconds", NULL, 0, NULL);
	unsigned long position;
	MCIERROR mciError;
	TCHAR mciCommandBuffer[512];
	TCHAR mciReturnBuffer[512];
	sprintf(mciCommandBuffer, "status cdaudio position track %u", cd_track());
	mciError = mciSendString(mciCommandBuffer, mciReturnBuffer, sizeof(mciReturnBuffer), NULL);
    if (mciError != 0)
    {
        //TODO: Show error message
		// HRESULT_FROM_WIN32(mciError)
        return;
    }
	sscanf(mciReturnBuffer, "%d", &position);
	cd_set_position(position + pos);
}

unsigned cd_track() {
	unsigned current;
    MCIERROR mciError;
    TCHAR mciReturnBuffer[512];
    mciError = mciSendString("status cdaudio current track", mciReturnBuffer, sizeof(mciReturnBuffer), NULL);
    if (mciError != 0)
    {
        //TODO: Show error message
		// HRESULT_FROM_WIN32(mciError)
        return 0;
    }
    sscanf(mciReturnBuffer, "%d", &current);
	return current;
}

unsigned long cd_track_length(unsigned n) {
	mciSendString("set cdaudio time format milliseconds", NULL, 0, NULL);
	unsigned long length;
	MCIERROR mciError;
	TCHAR mciCommandBuffer[512];
	TCHAR mciReturnBuffer[512];
	sprintf(mciCommandBuffer, "status cdaudio length track %u", n);
	mciError = mciSendString(mciCommandBuffer, mciReturnBuffer, sizeof(mciReturnBuffer), NULL);
    if (mciError != 0)
    {
        //TODO: Show error message
		// HRESULT_FROM_WIN32(mciError)
        return 0;
    }
	sscanf(mciReturnBuffer, "%d", &length);
	return length;
}

unsigned long cd_track_position() {
	mciSendString("set cdaudio time format milliseconds", NULL, 0, NULL);
	unsigned long position;
	MCIERROR mciError;
	TCHAR mciCommandBuffer[512];
	TCHAR mciReturnBuffer[512];
	sprintf(mciCommandBuffer, "status cdaudio position track %u", cd_track());
	mciError = mciSendString(mciCommandBuffer, mciReturnBuffer, sizeof(mciReturnBuffer), NULL);
    if (mciError != 0)
    {
        //TODO: Show error message
		// HRESULT_FROM_WIN32(mciError)
        return 0;
    }
	sscanf(mciReturnBuffer, "%d", &position);
	return cd_position() - position;
}

}
