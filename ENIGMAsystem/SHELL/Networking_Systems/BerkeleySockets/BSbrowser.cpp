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

#include "BSbrowser.h"
#include "BSnet.h"
#include "common.h"
#include <stdarg.h>
#include <stdlib.h>
using namespace enigma_user;

void die(const char *e, int n, ...) {
 printf("%s error (%d)\n",e,errno);
 int i;
 va_list s;
 va_start(s,n);
 for (i = 0; i < n; ++i)
  closesocket(va_arg(s,int));
 va_end(s);
 exit(1);
}

int prepare(char **ret, const char *cmd, int argc, ...) {
 char *str;
 int i, len = strlen(cmd);
 va_list vl;
 //get lengths of args minus %s
 va_start(vl,argc);
 for (i = 0; i < argc; ++i) {
  str = va_arg(vl,char*);
  len += strlen(str) - 2;
 }
 va_end(vl);
 //allocate
 *ret = (char *)malloc(len);
 //print
 va_start(vl,argc);
 vsprintf(*ret,cmd,vl);
 va_end(vl);
 return len;
}

void ftpsend(int in, const char *msg) {
 printf("> %s",msg);
 if (net_send_raw(in,msg,strlen(msg)) < 0)
  die("Send",1,in);
}

void ftpparse(int in, const char *cmd, const char *arg, const char *disp) {
 int len = strlen(cmd) + strlen(arg) - 2;
 char *packet = (char *)malloc(len);
 sprintf(packet,cmd,arg);
 printf("> ");
 printf(cmd,disp);
 len = net_send_raw(in,packet,len);
 free(packet);
 if (len < 0) die("Send",1,in);
}

char *ftpexpect(int in, const char *exp) {
 char *packet = (char *)net_receive(in).c_str();
 printf("%s",packet);
 if (strncmp(packet,exp,4) != 0) die("Response",1,in);
 return packet;
}

namespace enigma_user {

int net_ftp_open(string host, string user, string pass) {
 int in = net_connect_tcp(host,"ftp",0);
 if (in < 0) die("Connect",0);
 ftpexpect(in,"220 ");

 ftpparse(in,"USER %s\r\n",user.c_str(),user.c_str());
 ftpexpect(in,"331 ");

 ftpparse(in,"PASS %s\r\n",pass.c_str(),"<omitted>");
 ftpexpect(in,"230 ");
 
 return in;
}

void net_ftp_send(int in, string file, string msg, int msglen) {
 ftpsend(in,"PASV\r\n");
 char *ip = ftpexpect(in,"227 ");

 //get passive IP and port
 //note, IP resides in buffer, do not overwrite until finished
 ip = strchr(ip,'(') + 1;
 char *r2 = strchr(ip,',');
 char p,port[6];
 for (p = 0; p < 3; ++p) {
  *r2 = '.';
  r2 = strchr(r2,',');
 }
 *(r2++) = '\0';
 sprintf(port,"%d",atoi(r2) * 256 + atoi(strchr(r2,',') + 1));
 int out = net_connect_tcp(ip,port,0);

 ftpparse(in,"STOR %s\r\n",file.c_str(),file.c_str()); //ip overwritten
 ftpexpect(in,"150 ");
 net_send_raw(out,msg,msglen);
 closesocket(out);
 ftpexpect(in,"226 ");
}

void net_ftp_close(int in) {
 ftpsend(in,"QUIT\r\n");
 ftpexpect(in,"221 ");
 closesocket(in);
}

string net_http(string host, string loc) {
 char *packet;

 int s = net_connect_tcp(host,"http",0);
 if (s < 0) die("Connect",0);

 const char *cmd = "\
GET %s HTTP/1.1\r\n\
Host: %s\r\n\
User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:12.0) Gecko/20100101 Firefox/12.0\r\n\
Connection: close\r\n\r\n";
 int len = prepare(&packet,cmd,2,loc.c_str(),host.c_str());
 int r = net_send_raw(s,packet,len);
 free(packet);
 if (r < 0) die("Send",1,s);

 do {
  if ((packet = (char*)net_receive(s).c_str()) == NULL)
   die("Receive",1,s);

  //HTTP 1.1 requires handling of 100 Continue.
  while (strstr(packet,"HTTP/1.1 100 Continue\r\n") != NULL)
   packet = strstr(packet,"\r\n\r\n") + 4;
  //skip the header
  packet = strstr(packet,"\r\n\r\n");
 } while (packet == NULL); //packet not filled? Go back for more.
 packet += 4;

 closesocket(s);
 return packet;
}

}
