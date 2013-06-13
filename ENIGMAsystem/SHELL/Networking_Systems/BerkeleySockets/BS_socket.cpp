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

