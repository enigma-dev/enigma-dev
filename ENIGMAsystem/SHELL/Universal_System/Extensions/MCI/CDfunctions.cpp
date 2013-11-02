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
#include <windows.h>
#include <mmsystem.h>

namespace enigma {

void ControlCdTray(TCHAR drive, DWORD command)
{
	// Not used here, only for debug
	MCIERROR mciError = 0;
 
	// Flags for MCI command
	DWORD mciFlags = MCI_WAIT | MCI_OPEN_SHAREABLE | 
		MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT;
 
	// Open drive device and get device ID
	TCHAR elementName[] = { drive };
	MCI_OPEN_PARMS mciOpenParms = { 0 };
	mciOpenParms.lpstrDeviceType = (LPCTSTR)MCI_DEVTYPE_CD_AUDIO;
	mciOpenParms.lpstrElementName = elementName;	
	mciError = mciSendCommand(0, 
		MCI_OPEN, mciFlags, (DWORD_PTR)&mciOpenParms);
 
	// Eject or close tray using device ID
	MCI_SET_PARMS mciSetParms = { 0 };
	mciFlags = MCI_WAIT | command; // command is sent by caller
	mciError = mciSendCommand(mciOpenParms.wDeviceID, 
		MCI_SET, mciFlags, (DWORD_PTR)&mciSetParms);
	
	// Close device ID
	mciFlags = MCI_WAIT;
	MCI_GENERIC_PARMS mciGenericParms = { 0 };
	mciError = mciSendCommand(mciOpenParms.wDeviceID, 
		MCI_CLOSE, mciFlags, (DWORD_PTR)&mciGenericParms);
}
 
// Eject drive tray
void EjectCdTray(TCHAR drive)
{
	ControlCdTray(drive, MCI_SET_DOOR_OPEN);
}
 
// Retract drive tray
void CloseCdTray(TCHAR drive)
{
	ControlCdTray(drive, MCI_SET_DOOR_CLOSED);
}
}

namespace enigma_user {

string MCI_command(string str) {
	string ret;
	mciSendString(str.c_str(), &ret[0], 0, NULL); 
	return ret;
}

void cd_open_door() {
	enigma::EjectCdTray('D');
}

void cd_close_door() {
	enigma::CloseCdTray('D');
}

void cd_init() {
}

unsigned long cd_length() {
}

unsigned cd_number() {
}

void cd_play(unsigned first, unsigned last) {
    TCHAR achCommandBuff[128]; 
    int result;
    MCIERROR err;

    // Form the command string.
    result = sprintf_s(
        achCommandBuff, 128,
        "play cdaudio from %u to %u", first, last); 

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

void cd_paused() {
}

bool cd_playing() {
}

bool cd_present() {
}

unsigned long cd_position() {
}

void cd_set_position(unsigned long pos) {
    TCHAR achCommandBuff[128]; 
    int result;
    MCIERROR err;

    // Form the command string.
    result = sprintf_s(
        achCommandBuff, 128,
        "seek cdaudio to %s", pos); 

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

void cd_set_track_position(unsigned long pos) {
}

unsigned cd_track() {
}

unsigned long cd_track_length() {
}

unsigned long cd_track_position() {
}


}