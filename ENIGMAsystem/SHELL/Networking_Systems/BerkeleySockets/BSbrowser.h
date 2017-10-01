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

#ifndef ENIGMA_BSBROWSER_H
#define ENIGMA_BSBROWSER_H

#include <string>
using std::string;

void die(const char *e, int n, ...);
int prepare(char **ret, const char *cmd, int argc, ...);
void ftpsend(int in, const char *msg);
void ftpparse(int in, const char *cmd, const char *arg, const char *disp);
char *ftpexpect(int in, const char *exp);

namespace enigma_user {

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

}

#endif // ENIGMA_BSBROWSER_H
