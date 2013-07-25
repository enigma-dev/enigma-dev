/*
  The MIT License

  Copyright (c) 2010-2013 IsmAvatar <IsmAvatar@gmail.com> and Robert B. Colton

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef ___BSNET_H_
#define ___BSNET_H_

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifdef _WIN32
 #define _WIN32_WINNT 0x0501
 #include <winsock2.h>
 #include <ws2tcpip.h>
 #pragma comment (lib,"ws2_32")
#else
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <fcntl.h>

 #define SOCKET_ERROR -1
 #define closesocket(s) close(s)
#endif

#include "Universal_System/var4.h"

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

// These functions are for compatibility with GameMaker: Studio
void network_connect();
void network_conenct_raw();
void network_create_server();
void network_create_socket();
void network_resolve();
void network_send_broadcase();
void network_send_packet();
void network_send_raw();
void network_send_udp();
void network_set_timeout();
void network_destroy();

#endif
