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
using namespace enigma_user;

#ifdef _WIN32
 #define DLLEXPORT extern "C" __declspec(dllexport)
#else
 #define DLLEXPORT extern "C"
#endif

DLLEXPORT double dllInit() { return net_init(); }

double doConnect(char *ip, double port, double mode, bool serve, bool udp)
{
 char sport[6]; //can't exceed 65535
 snprintf(sport,6,"%f",port);
 int s = net_connect(ip,sport,serve,udp);
 if (s < 0) return -1;
 net_blocking(s,(char) mode);
 return s;
}

DLLEXPORT double tcpconnect(char*ip, double port, double mode) { return doConnect(ip,port,mode,true,false); }

DLLEXPORT double tcplisten(double port, double max, double mode) { return doConnect(NULL,port,mode,false,false); }

DLLEXPORT double udpconnect(double port, double mode) { return doConnect(NULL,port,mode,false,true); }

DLLEXPORT double closesock(double sockid) { return closesocket((int)sockid) == 0 ? 1 : -1; }

DLLEXPORT double setsync(double sockid, double mode) { net_blocking((int)sockid,(char)mode) == 0 ? 1 : -1; }

DLLEXPORT double tcpaccept(double sockid, double mode)
{
 int s = net_accept((int) sockid);
 if (s >= 0) net_blocking(s,(char) mode);
 return s;
}

