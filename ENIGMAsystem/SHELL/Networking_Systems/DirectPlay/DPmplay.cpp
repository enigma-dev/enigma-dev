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

#define DPESC_TIMEOUT 69 // i guess WINE team forgot this in the DirectPlay headers with MinGW

namespace enigma {
LPDIRECTPLAY3A   	lpdp = NULL;    	// interface pointer to directplay
LPDIRECTPLAYLOBBY2A  lpdplobby = NULL;   // lobby interface pointer
bool session_migrate_host = false; // when the host leaves, should the server move to the next available host or just close
	
BOOL   gameServer;     	// flag for client/server
char   player_name[10];	// local player name, limit it to 10 chars
char   session_name[10];   // name of session, also limit to 10 chars
char   tcp_address[15];	// tcp ip address to connect to

class DP_PLAYER_LIST_ELEM {

DPID	dpid;  	// the directplay id of player
char	name[10];  // name of player
DWORD   flags; 	// directplay player flags

// any other info you might need
};

//DP_PLAYER_LIST   *dp_player_list;   // list of players in current session
	
// our callback function
BOOL FAR PASCAL EnumSessionsCallback(LPCDPSESSIONDESC2 lpThisSD,
                                 	LPDWORD lpdwTimeOut,
                                 	DWORD dwFlags, LPVOID lpContext)
{
	HWND   hwnd;  	// handle. I suggest as listbox handle

	if (dwFlags & DPESC_TIMEOUT)  // if finished enumerating stop
    	return(FALSE);

	hwnd = (HWND) lpContext;   // get window handle

	// lpThisSd-> lpszSessionNameA  // store this value, name of session
	// lpThis->guidInstance     	// store this, the instance of the host

	return(TRUE);  // keep enumerating
}

// our enumeration function
int EnumSessions(HWND hwnd, GUID app_guid, DWORD dwFlags)
{
	DPSESSIONDESC2   session_desc;   // session desc
	
	ZeroMemory(&session_desc, sizeof(DPSESSIONDESC2));
	session_desc.dwSize = sizeof(DPSESSIONDESC2);
	session_desc.guidInstance = app_guid;

	// enumerate the session. Check for error here. Vital
	lpdp->EnumSessions(&session_desc, 0, EnumSessionsCallback, hwnd, dwFlags);

	return(1);   // success
}

}

// types of messages the application will receive
const    DWORD    DP_MSG_CHATSTRING = 0;  // chat message

// the structure of a string message to send
typedef struct DP_STRING_MSG_TYP   // for variable string
{
	DWORD  dwType; 	// type of message
	char   szMsg[1];   // variable length message

}; //DP_STRING_MSG *DP_STRING_MSG_PTR;

namespace enigma_user {

bool mplay_init_ipx() {
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
	Address[0].lpData   	= (LPVOID)&DPSPGUID_IPX;  // TCP ID

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

bool mplay_init_modem(string initstr, string phonenr) {
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
	Address[0].lpData   	= (LPVOID)&DPSPGUID_MODEM;  // TCP ID

	Address[1].guidDataType = DPAID_INet;
	Address[1].dwDataSize   = lstrlen(initstr.c_str())+1;
	Address[1].lpData   	= (void*)initstr.c_str();

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

bool mplay_init_serial(unsigned portno, unsigned baudrate, unsigned stopbits, unsigned parity, unsigned flow) {
	LPDIRECTPLAYLOBBYA    	old_lpdplobbyA = NULL;	// old lobby pointer
	DPCOMPOUNDADDRESSELEMENT  Address[2];           	// to create compound addr
	DWORD 	addressSize = 0;   	// size of compound address
	LPVOID	lpConnection = NULL;	// pointer to make connection
	DPCOMPORTADDRESS comp;
	
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
	Address[0].lpData   	= (LPVOID)&DPSPGUID_SERIAL;  // TCP ID

	comp.dwComPort = portno;
    comp.dwBaudRate = baudrate;
    comp.dwStopBits = stopbits;
    comp.dwParity = parity;
    comp.dwFlowControl = flow;
	
	Address[1].guidDataType = DPAID_ComPort;
	Address[1].dwDataSize   = sizeof(DPCOMPORTADDRESS);
	Address[1].lpData   	= &comp;

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

unsigned mplay_player_find() {

}

int mplay_player_id(unsigned numb) {

}

int mplay_player_id(string name) {

}

string mplay_player_name(unsigned numb) {

}

bool mplay_session_create(string sesname, unsigned playnumb, string playername) {
	DPSESSIONDESC2   session_desc;

	ZeroMemory(&session_desc, sizeof(DPSESSIONDESC2));
	session_desc.dwSize = sizeof(DPSESSIONDESC2);
	//session_desc.guidInstance = NULL;// instance you have save somewhere;
	
	if (enigma::session_migrate_host) {
		session_desc.dwFlags = DPSESSION_MIGRATEHOST;
	} else {
		session_desc.dwFlags = DPSESSION_KEEPALIVE;
	}

	// and join the session
	enigma::lpdp->Open(&session_desc, DPOPEN_CREATE | DPOPEN_RETURNSTATUS);
	
	DPNAME	pname;  	// name type
	DPID  	dpid;  	// the dpid of the player created given by directplay

	ZeroMemory(&pname,sizeof(DPNAME));   // clear out structure
	pname.lpszShortNameA = (char*)playername.c_str();  // the name the from the user
	pname.lpszLongNameA = NULL;

	enigma::lpdp->CreatePlayer(&dpid, &pname, NULL, NULL, 0, 0);
	
	return true; // success
}

void mplay_session_end() {
	DWORD hr = enigma::lpdp->Close(); 
}

unsigned mplay_session_find() {

}

bool mplay_session_join(unsigned numb, string playername) {
	DPSESSIONDESC2   session_desc;

	ZeroMemory(&session_desc, sizeof(DPSESSIONDESC2));
	session_desc.dwSize = sizeof(DPSESSIONDESC2);
	//session_desc.guidInstance = NULL;// instance you have save somewhere;
	
	if (enigma::session_migrate_host) {
		session_desc.dwFlags = DPSESSION_MIGRATEHOST;
	} else {
		session_desc.dwFlags = DPSESSION_KEEPALIVE;
	}

	// and join the session
	enigma::lpdp->Open(&session_desc, DPOPEN_JOIN);
	
	DPNAME	pname;  	// name type
	DPID  	dpid;  	// the dpid of the player created given by directplay

	ZeroMemory(&pname,sizeof(DPNAME));   // clear out structure
	pname.lpszShortNameA = (char*)playername.c_str();  // the name the from the user
	pname.lpszLongNameA = NULL;

	enigma::lpdp->CreatePlayer(&dpid, &pname, NULL, NULL, 0, 0);
	
	return true; // success
}

void mplay_session_mode(bool move) {
	enigma::session_migrate_host = move;
}

string mplay_session_name(unsigned numb) {

}

int mplay_session_status() {

}

void mplay_end() {
	if (enigma::lpdp)   // if connection already up, so it won't be null
	{
    	if (enigma::lpdplobby)
        	enigma::lpdplobby->Release();

    	enigma::lpdp->Release();

    	CoUninitialize();  // unregister the COM
	}

	enigma::lpdp   	= NULL;  // set to NULL, safe practice here
	enigma::lpdplobby  = NULL;
}

void mplay_data_mode(bool guar) {
}
 
variant mplay_data_read(unsigned ind) {
}
 
void mplay_data_write(unsigned ind, variant val) {
}
 
void mplay_message_clear() {
}
 
unsigned mplay_message_count(string player) {
}

unsigned mplay_message_count(unsigned player) {
}

unsigned mplay_message_id() {
}

string mplay_message_name() {
}
 
variant mplay_message_value() {
}
 
unsigned mplay_message_player() {
}
 
bool mplay_message_receive(string player) {
}
 
bool mplay_message_receive(unsigned player) {
}
 
bool mplay_message_send(string player, unsigned id, variant val) {
	//DP_STRING_MSG_PTR   lpStringMsg;	// message pointer
	DWORD           	dwMessageSize;  // size of message
    
	string msg = val;
	if (!strlen(msg.c_str())) {
		return false;
	}
	//dwMessageSize = sizeof(DP_STRING_MSG)+lstrlen(msg.c_str()); // get size

	// allocate space
	//lpStringMsg = (DP_STRING_MSG_PTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwMessageSize);  // allocating mem

	//lpStringMsg->dwType = id;      	// set the type
	//lstrcpy(lpStringMsg->szMsg, msg.c_str());  // copy the string

	// send the string
	//enigma::lpdp->Send(mplay_player_id(player), id, DP_SEND_GUARANTEED,
    	//lpStringMsg, dwMessageSize);

	return true;   // success
}

bool mplay_message_send(unsigned player, unsigned id, variant val) {
	//DP_STRING_MSG_PTR   lpStringMsg;	// message pointer
	DWORD           	dwMessageSize;  // size of message
    
	string msg = val;
	if (!strlen(msg.c_str())) {
		return false;
	}
	//dwMessageSize = sizeof(DP_STRING_MSG)+lstrlen(msg.c_str()); // get size

	// allocate space
	//lpStringMsg = (DP_STRING_MSG_PTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwMessageSize);  // allocating mem

	//lpStringMsg->dwType = id;      	// set the type
	//lstrcpy(lpStringMsg->szMsg, msg.c_str());  // copy the string

	// send the string
	//enigma::lpdp->Send(player, id, DP_SEND_GUARANTEED,
    	//lpStringMsg, dwMessageSize);

	return true;   // success
}
 
bool mplay_message_send_guaranteed(string player, unsigned id, variant val) {
}
 
bool mplay_message_send_guaranteed(unsigned player, unsigned id, variant val) {
}

}