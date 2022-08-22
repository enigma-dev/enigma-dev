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

#include <string>
using std::string;

#include "Universal_System/var4.h"

namespace enigma_user {
	int mplay_connect_status(); 
	void mplay_end(); 
	bool mplay_init_ipx();  
	bool mplay_init_modem(string initstr, string phonenr); 
	bool mplay_init_serial(unsigned portno, unsigned baudrate, unsigned stopbits, unsigned parity, unsigned flow); 
	bool mplay_init_tcpip(string addr); 
	string mplay_ipaddress(); 
	unsigned mplay_player_find(); 
	int mplay_player_id(unsigned numb); 
	int mplay_player_id(string name);
	string mplay_player_name(unsigned numb); 
	bool mplay_session_create(string sesname, unsigned playnumb, string playername); 
	void mplay_session_end(); 
	unsigned mplay_session_find(); 
	bool mplay_session_join(unsigned numb, string playername); 
	void mplay_session_mode(bool move);
	string mplay_session_name(unsigned numb); 
	int mplay_session_status(); 
	void mplay_data_mode(bool guar); 
	evariant mplay_data_read(unsigned ind); 
	void mplay_data_write(unsigned ind, evariant val); 
	void mplay_message_clear(); 
	unsigned mplay_message_count(string player);
	unsigned mplay_message_count(unsigned player);
	unsigned mplay_message_id();
	string mplay_message_name(); 
	evariant mplay_message_value(); 
	unsigned mplay_message_player(); 
	bool mplay_message_receive(string player); 
	bool mplay_message_receive(unsigned player); 
	bool mplay_message_send(string player, unsigned id, evariant val);
	bool mplay_message_send(unsigned player, unsigned id, evariant val); 
	bool mplay_message_send_guaranteed(string player, unsigned id, evariant val); 
	bool mplay_message_send_guaranteed(unsigned player, unsigned id, evariant val); 
}