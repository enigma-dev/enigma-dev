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

#include "../General/NSmplay.h"

#include <dplay.h> 	// directplay main
#include <dplobby.h>   // the directplay lobby

#define   INITGUID      // to use the predefined ids

namespace enigma {
	LPDIRECTPLAY3A   	lpdp = NULL;    	// interface pointer to directplay
	LPDIRECTPLAYLOBBY2A  lpdplobby = NULL;   // lobby interface pointer
}

namespace enigma_user {

void mplay_init_ipx() {

}

void mplay_init_modem(string initstr, string phonenr) {

}

bool mplay_init_serial(unsigned portno, unsigned baudrate, unsigned stopbits, unsigned parity, unsigned flow) {

}

bool mplay_init_tcpip(string addr) {
	LPDIRECTPLAYLOBBYA    	old_lpdplobbyA = NULL;	// old lobby pointer
	DPCOMPOUNDADDRESSELEMENT  Address[2];           	// to create compound addr
	DWORD 	addressSize = 0;   	// size of compound address
	LPVOID	lpConnection= NULL;	// pointer to make connection

	CoInitialize(NULL);	// registering COM

	// creating directplay object
	if  ( CoCreateInstance(CLSID_DirectPlay, NULL, CLSCTX_INPROC_SERVER,
     	IID_IDirectPlay3A,(LPVOID*)&enigma::lpdp ) != S_OK)
	{
    	// return  a messagebox error
    	CoUninitialize();   // unregister the comp
    	return false;
	}

	// creating lobby object
	DirectPlayLobbyCreate(NULL, &old_lpdplobbyA, NULL, NULL, 0);

	// get new interface of lobby
	old_lpdplobbyA->QueryInterface(IID_IDirectPlayLobby2A, (LPVOID *)&enigma::lpdplobby);

	old_lpdplobbyA->Release();   // release old interface since we have new one

	// fill in data for address
	Address[0].guidDataType = DPAID_ServiceProvider;
	Address[0].dwDataSize   = sizeof(GUID);
	Address[0].lpData   	= (LPVOID)&DPSPGUID_TCPIP;  // TCP ID

	Address[1].guidDataType = DPAID_INet;
	Address[1].dwDataSize   = lstrlen(addr.c_str())+1;
	Address[1].lpData   	= (void*)addr.c_str();

	// get size to create address
	// this method will return DPERR_BUFFERTOOSMALL – not an error
	enigma::lpdplobby->CreateCompoundAddress(Address, 2, NULL, &addressSize);

	lpConnection = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, addressSize);  // allocating mem

	// now creating the address
	enigma::lpdplobby->CreateCompoundAddress(Address, 2, lpConnection, &addressSize);

	// initialize the tcp connection
	enigma::lpdp->InitializeConnection(lpConnection, 0);

	HeapFree(GetProcessHeap(), 0, lpConnection);    // free allocated memory

	return true;    // success
}

void mplay_end() {
	DWORD hr = enigma::lpdp->Close(); 

	enigma::lpdp->Release();
	enigma::lpdp = NULL;
}

}