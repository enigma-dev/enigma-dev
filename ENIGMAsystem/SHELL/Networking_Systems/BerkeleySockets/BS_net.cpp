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

#include "BS_net.h"

#ifndef _WIN32
 #include <netdb.h>
#endif

bool winsock_started = 0;

bool net_init() {
 if (winsock_started) return 1;
#ifdef _WIN32
 // Make sure windows is using Winsock version request 2.2
 WSADATA wsaData;
 if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 0;
 if (LOBYTE(wsaData.wVersion) != 2
 ||  HIBYTE(wsaData.wVersion) != 2) {
  WSACleanup();
  return 0;
 }
#endif
 return winsock_started = 1;
}

bool net_cleanup() {
 if (!winsock_started) return 1;
#ifdef _WIN32
 WSACleanup();
#endif
 return !(winsock_started = 0);
}

//initializes a socket
//serve specifies if this is a server socket (1) or a client socket (0)
//udp specifies whether to use udp (1) or tcp (0)
//if this is not a server, addr and port specify who to connect to.
//IPv4 and v6 are both supported.
//Returns an identifier for the socket, or negative on error.
//For clients, the identifier indicates the server,
//and may be used to receive messages from them.
int net_connect(char *addr, char *port, bool serve, bool udp) {
 net_init();

 struct addrinfo hints, *sinf;
 memset(&hints, 0, sizeof(hints));
 hints.ai_family = AF_UNSPEC; //support both IPv4/6
 hints.ai_socktype = (udp == 0) ? SOCK_STREAM : SOCK_DGRAM;
 hints.ai_flags = AI_PASSIVE;

 if (serve != 0 || addr == NULL || addr[0] == '\0') addr = NULL;
 if (getaddrinfo(addr, port, &hints, &sinf) != 0) return -2;

 int s = socket(sinf->ai_family, sinf->ai_socktype, sinf->ai_protocol);
 if (s == -1) {
  freeaddrinfo(sinf);
  return -3;
 }

 if (serve == 0) { //client
  if (connect(s, sinf->ai_addr, sinf->ai_addrlen) == SOCKET_ERROR) {
   freeaddrinfo(sinf);
   closesocket(s);
   return -5;
  }
 } else { //server
  if (bind(s, sinf->ai_addr, sinf->ai_addrlen) == SOCKET_ERROR) {
   freeaddrinfo(sinf);
   closesocket(s);
   return -4;
  }
  if (udp == 0) { //server tcp
   if (listen(s, 5) == SOCKET_ERROR) {
    freeaddrinfo(sinf);
    closesocket(s);
    return -5;
   }
  } else if (0) { //server udp
   if (connect(s, sinf->ai_addr, sinf->ai_addrlen) == SOCKET_ERROR) {
    freeaddrinfo(sinf);
    closesocket(s);
    return -6;
   }
  }
 }

 freeaddrinfo(sinf);
 return s;
}

//Initializes a tcp socket, which can either be a server or client.
//See net_connect for arguments and returns
int net_connect_tcp(char *addr, char *port, bool serve) {
 return net_connect(addr,port,serve,0);
}

//Initializes a tcp socket, which can either be a server or client.
//See net_connect for arguments and returns
int net_connect_udp(char *localport, bool serve) {
 return net_connect(NULL,localport,serve,1);
}

//A server must accept or reject (ignore) incoming socket connections.
//The argument is this server socket's ID.
//Returns the incoming socket's ID, or -1 if an error occurred.
int net_accept(int s) {
 return accept(s, NULL, NULL);
}

#define BUFSIZE 512
char buf[BUFSIZE];

//Receives data on a socket's stream.
//The argument is the socket to receive data from.
//Returns the data, or NULL on error.

//This function requires improvement due to the sensitive nature of its buffer termination.
//As it functions now, it will fill the full buffer (BUFSIZE) as best it can.
//Iff it reads short of BUFSIZE, it will append a null character.
//Otherwise, if it reads the full buffer, no null character can be appended.
//There is no guarantee that the buffer will not already contain null characters.
char *net_receive(int s) {
 int r;
 if ((r = recv(s,buf,BUFSIZE,0)) == SOCKET_ERROR) return NULL;
 if (r == BUFSIZE) return buf;
 buf[r] = '\0';
 return buf;
}

//A largely debugging/server method for echo-bouncing messages
//That is, receives a message from the specified socket, and sends it back to the same socket.
//Prints the message that was bounced, if available.
//Returns bounce status. 0 on successful bounce. 1 on empty receive, 2 on empty send.
//Returns -1 on receive error. -2 on send error.
int net_bounce(int s) {
 struct sockaddr_storage whom;
 u_int len = sizeof(whom);
 int n = recvfrom(s,buf,BUFSIZE,0,(struct sockaddr *)&whom,&len);
 if (n == 0) return 1;
 if (n == SOCKET_ERROR) return -1;
 printf("Bouncing: %s\n",buf);
 n = sendto(s,buf,n,0,(struct sockaddr *)&whom,sizeof(whom));
 if (n == 0) return 2;
 if (n == SOCKET_ERROR) return -2;
 return 0;
}

//Sends a message to specified socket. (We use a #define in the .h file instead)
//See documentation for Berkeley sockets send() method.
/*int net_send(int s, char *msg, int len) {
 return send(s,msg,len,0);
}*/

//Returns the port of a given socket.
int net_get_port(int s) {
 struct sockaddr_in sa;
 u_int sas = sizeof(sa);
 if (getsockname(s, (struct sockaddr*) &sa, &sas) == SOCKET_ERROR) {
  closesocket(s);
  return -1;
 }
 return ntohs(sa.sin_port);
}

//Sets whether given socket is in blocking mode
//Blocking sockets will block on certain commands (read, accept) if nothing is available yet.
//When a command blocks, it pauses program execution until something is available.
//Non-blocking sockets will immediately return a not-ready value on those commands.
//By default, sockets are initialized as blocking.
//Returns 0 on success, any other value on error.
//Windows users, see the return value of ioctlsocket.
int net_blocking(int s, bool block) {
#ifdef _WIN32
 u_long iMode = (block == 0) ? 1 : 0;
 return ioctlsocket(s,FIONBIO,&iMode);
#else
 int flags, r;
 if ((flags = fcntl(s, F_GETFL, 0)) < 0) return -1;
 if (block == 0) flags |= O_NONBLOCK;
 else flags &= ~O_NONBLOCK;
 if (fcntl(s, F_SETFL, flags) < 0) return -2;
 return 0;
#endif
}

