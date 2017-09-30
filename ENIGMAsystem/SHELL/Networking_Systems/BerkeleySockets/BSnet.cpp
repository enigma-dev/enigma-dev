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

#include "BSnet.h"
#include "common.h"
#include "libEGMstd.h"

bool winsock_started = 0;

namespace enigma_user {

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

int net_connect(string addr, string port, bool server, bool udp) {
 net_init();

 struct addrinfo hints, *sinf;
 memset(&hints, 0, sizeof(hints));
 hints.ai_family = AF_UNSPEC; //support both IPv4/6
 hints.ai_socktype = (udp == 0) ? SOCK_STREAM : SOCK_DGRAM;
 hints.ai_flags = AI_PASSIVE;

 if (getaddrinfo(addr.c_str(), port.c_str(), &hints, &sinf) != 0) return -2;

 int s = socket(sinf->ai_family, sinf->ai_socktype, sinf->ai_protocol);
 if (s == -1) {
  freeaddrinfo(sinf);
  return -3;
 }

 if (!server) { //client
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
  if (!udp) { //server tcp
   if (listen(s, 5) == SOCKET_ERROR) {
    freeaddrinfo(sinf);
    closesocket(s);
    return -5;
   }
  } else { //server udp
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

int net_connect_tcp(string addr, string port, bool server) {
 return net_connect(addr, port, server, 0);
}

int net_connect_udp(string localport, bool server) {
 return net_connect(NULL, localport, server, 1);
}

int net_accept(int sock) {
 return accept(sock, NULL, NULL);
}

#define BUFSIZE 512
char buf[BUFSIZE];

string net_receive(int sock) {
 int r;
 buf[0] = '\0';
 if ((r = recv(sock,buf,BUFSIZE,0)) == SOCKET_ERROR) return buf;
 if (r == BUFSIZE) return buf;
 return buf;
}

int net_bounce(int sock) {
 struct sockaddr_storage whom;
 socklen_t len = sizeof(whom);
 int n = recvfrom(sock,buf,BUFSIZE,0,(struct sockaddr *)&whom,&len);
 if (n == 0) return 1;
 if (n == SOCKET_ERROR) return -1;
 printf("Bouncing: %s\n",buf);
 n = sendto(sock,buf,n,0,(struct sockaddr *)&whom,sizeof(whom));
 if (n == 0) return 2;
 if (n == SOCKET_ERROR) return -2;
 return 0;
}

int net_send_raw(int sock, string msg, int len) {
  send(sock, msg.c_str(), len, 0);
  return 0;
}

int net_get_port(int sock) {
 struct sockaddr_in sa;
 socklen_t sas = sizeof(sa);
 if (getsockname(sock, (struct sockaddr*) &sa, &sas) == SOCKET_ERROR) {
  closesocket(sock);
  return -1;
 }
 return ntohs(sa.sin_port);
}

int net_blocking(int sock, bool block) {
#ifdef _WIN32
 u_long iMode = (block == 0) ? 1 : 0;
 return ioctlsocket(sock,FIONBIO,&iMode);
#else
 int flags, r;
 if ((flags = fcntl(sock, F_GETFL, 0)) < 0) return -1;
 if (block == 0) flags |= O_NONBLOCK;
 else flags &= ~O_NONBLOCK;
 if (fcntl(sock, F_SETFL, flags) < 0) return -2;
 return 0;
#endif
}

}

