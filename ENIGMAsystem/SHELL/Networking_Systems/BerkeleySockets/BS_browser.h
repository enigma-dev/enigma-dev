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

// Implementation of some major aspects of protocols for http and ftp.
// This allows easy communication with FTP and HTTP servers.
// An example is also provided in the main() method at the bottom.

/*
The following resources may be handy in writing your http client/server:
Http made really easy
http://www.jmarshall.com/easy/http/#structure
W3C HTTP/1.1 (especially helpful with understanding headers)
http://www.w3.org/Protocols/rfc2616/rfc2616.html
RFC 2616 (very difficult to read)
http://www.ietf.org/rfc/rfc2616.txt
*/

#include "BS_net.h"
#include <stdarg.h>
#include <stdlib.h>

void die(const char *e, int n, ...);
int prepare(char **ret, const char *cmd, int argc, ...);
void ftpsend(int in, const char *msg);
void ftpparse(int in, const char *cmd, const char *arg, const char *disp);
char *ftpexpect(int in, const char *exp);
//Opens a connection with an FTP server using given hostname, username, and password.
//Returns an identifier for the FTP connection.
int net_ftp_open(string host, string user, string pass);
//Uploads data to an ftp server to be stored as file.
//Indicate the ftp connection, the remote filename to store the data in, and the data (and size)
void net_ftp_send(int in, string file, string msg, int msglen);
//Properly terminates the connection with this ftp socket.
void net_ftp_close(int in);
//Queries a http host and location, returns the contents
//This method handles the packets and headers for you
string net_http(string host, string loc);

