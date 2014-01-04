/** Copyright (C) 2013 Robert B. Colton
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

#ifndef ENIGMA_NSNET_H
#define ENIGMA_NSNET_H

void network_connect();
void network_conenct_raw();
void network_create_server();
void network_create_socket();
void network_resolve();
void network_send_broadcase();
void network_send_packet();
void network_send_raw();
void network_send_udp();
void network_set_timeout();
void network_destroy();

#endif
