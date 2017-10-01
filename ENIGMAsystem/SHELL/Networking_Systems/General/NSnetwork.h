/** Copyright (C) 2013-2014 Robert B. Colton
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

#ifndef ENIGMA_NSNETWORK_H
#define ENIGMA_NSNETWORK_H

#include <string>
using std::string;

namespace enigma_user {

int network_connect(int socket, string url, int port);
int network_conenct_raw(int socket, string url, int port);
int network_create_server(int type, int port, int clients);
int network_create_socket(int type);
void network_destroy(int socket);
string network_resolve(string url);
unsigned network_send_broadcast(int socket, int port, int buffer, unsigned size);
unsigned network_send_packet(int socket, int buffer, unsigned size);
unsigned network_send_raw(int socket, int buffer, unsigned size);
unsigned network_send_udp(int socket, string url, int port, int buffer, unsigned size);
void network_set_timeout(int socket, long read, long write);

}

#endif
