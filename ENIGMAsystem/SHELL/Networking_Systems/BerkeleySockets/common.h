#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>

#ifdef _WIN32
 #ifndef _WIN32_WINNT
  #define _WIN32_WINNT 0x0501
 #endif
 #include <winsock2.h>
 #include <ws2tcpip.h>
 typedef int socklen_t;
#else
 #include <netdb.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <fcntl.h>

 #define SOCKET_ERROR -1
 #define closesocket(s) close(s)
#endif

#include "Universal_System/var4.h"
