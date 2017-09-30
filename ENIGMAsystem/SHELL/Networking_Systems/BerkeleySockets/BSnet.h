/** Copyright (C) 2010 IsmAvatar
*** Copyright (C) 2013-2014 Robert B. Colton
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

#ifndef ENIGMA_BSNET_H
#define ENIGMA_BSNET_H

#include <string>
using std::string;

namespace enigma_user {

bool net_init();
bool net_cleanup();
//initializes a socket
//serve specifies if this is a server socket (1) or a client socket (0)
//udp specifies whether to use udp (1) or tcp (0)
//if this is not a server, addr and port specify who to connect to.
//IPv4 and v6 are both supported.
//Returns an identifier for the socket, or negative on error.
//For clients, the identifier indicates the server,
//and may be used to receive messages from them.
int net_connect(string addr, string port, bool server, bool udp);
//Initializes a tcp socket, which can either be a server or client.
//See net_connect for arguments and returns
int net_connect_tcp(string addr, string port, bool server);
//Initializes a udp socket, which can either be a server or client.
//See net_connect for arguments and returns
int net_connect_udp(string localport, bool server);
//A server must accept or reject (ignore) incoming socket connections.
//The argument is this server socket's ID.
//Returns the incoming socket's ID, or -1 if an error occurred.
int net_accept(int sock); 

//Receives data on a socket's stream.
//The argument is the socket to receive data from.
//Returns the data, or NULL on error.

//This function requires improvement due to the sensitive nature of its buffer termination.
//As it functions now, it will fill the full buffer (BUFSIZE) as best it can.
//Iff it reads short of BUFSIZE, it will append a null character.
//Otherwise, if it reads the full buffer, no null character can be appended.
//There is no guarantee that the buffer will not already contain null characters.
string net_receive(int sock);
//A largely debugging/server method for echo-bouncing messages
//That is, receives a message from the specified socket, and sends it back to the same socket.
//Prints the message that was bounced, if available.
//Returns bounce status. 0 on successful bounce. 1 on empty receive, 2 on empty send.
//Returns -1 on receive error. -2 on send error.
int net_bounce(int sock);
//Sends a message to specified socket. (We use a #define in the .h file instead)
//See documentation for Berkeley sockets send() method.
int net_send_raw(int sock, string msg, int len);
//Returns the port of a given socket.
int net_get_port(int sock);
//Sets whether given socket is in blocking mode
//Blocking sockets will block on certain commands (read, accept) if nothing is available yet.
//When a command blocks, it pauses program execution until something is available.
//Non-blocking sockets will immediately return a not-ready value on those commands.
//By default, sockets are initialized as blocking.
//Returns 0 on success, any other value on error.
//Windows users, see the return value of ioctlsocket.
int net_blocking(int sock, bool block);

}

#endif // ENIGMA_BSNET_H
