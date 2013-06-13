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

int net_connect_tcp(char *addr, char *port, bool serve) {
 return net_connect(addr,port,serve,0);
}

int net_connect_udp(char *localport, bool serve) {
 return net_connect(NULL,localport,serve,1);
}

int net_accept(int sock) {
 return accept(sock, NULL, NULL);
}

#define BUFSIZE 512
char buf[BUFSIZE];

char *net_receive(int sock) {
 int r;
 if ((r = recv(sock,buf,BUFSIZE,0)) == SOCKET_ERROR) return NULL;
 if (r == BUFSIZE) return buf;
 buf[r] = '\0';
 return buf;
}

int net_bounce(int sock) {
 struct sockaddr_storage whom;
 u_int len = sizeof(whom);
 int n = recvfrom(sock,buf,BUFSIZE,0,(struct sockaddr *)&whom,&len);
 if (n == 0) return 1;
 if (n == SOCKET_ERROR) return -1;
 printf("Bouncing: %s\n",buf);
 n = sendto(sock,buf,n,0,(struct sockaddr *)&whom,sizeof(whom));
 if (n == 0) return 2;
 if (n == SOCKET_ERROR) return -2;
 return 0;
}

int net_send(int sock, char* msg, int len) {
  send(sock, msg, len, 0); 
}

int net_get_port(int sock) {
 struct sockaddr_in sa;
 u_int sas = sizeof(sa);
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

