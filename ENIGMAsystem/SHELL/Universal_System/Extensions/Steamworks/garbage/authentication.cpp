/** Copyright (C) 2023-2024 Saif Kandil
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

#include "include.h"

namespace enigma_user {
    int steam_get_auth_session_ticket(uint8 m_rgubTicketIGaveThisUser[], uint32 m_cubTicketIGaveThisUser) {
        HAuthTicket m_hAuthTicketIGaveThisUser;
	    if ( m_cubTicketIGaveThisUser == 0 )
	    {
	    	m_hAuthTicketIGaveThisUser = SteamUser()->GetAuthSessionTicket( m_rgubTicketIGaveThisUser, sizeof( m_rgubTicketIGaveThisUser ), &m_cubTicketIGaveThisUser );
	    }
        return m_hAuthTicketIGaveThisUser;
    } // steam_get_auth_session_ticket

    int steam_begin_auth_session(uint8 m_rgubTicketIGaveThisUser[], uint32 m_cubTicketIGaveThisUser, CSteamID steamID) {
        return SteamUser()->BeginAuthSession( m_rgubTicketIGaveThisUser , sizeof( m_rgubTicketIGaveThisUser ), steamID);
    } // steam_begin_auth_session
} // enigma_user
