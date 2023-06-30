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

// #include <fstream>
// #include <iostream>

// #include "ENIGMAsystem\SHELL\Graphics_Systems\General\GSsurface.h"
// #include "ENIGMAsystem\SHELL\Universal_System\buffers.h"

// #include <windows.h>

// #include <filesystem>
// namespace fs = std::filesystem;

// #include "Widget_Systems/widgets_mandatory.h"
// #include "Platforms/platforms_mandatory.h"

// #include <public\steam\steam_api.h>

// using std::ofstream;
// using std::ifstream;
// using std::cout;
// using std::endl;

#include "steamworks.h"
#include "general.h"
#include "management.h"
#include "overlay.h"

// namespace enigma {
//     void init();

//     void steam_request_current_stats();

//     void steam_store_stats();
//     CSteamID steam_get_user_steam_id();
// }

// namespace enigma_user {

//     int steam_get_auth_session_ticket(uint8 m_rgubTicketIGaveThisUser[], uint32 m_cubTicketIGaveThisUser);
//     int steam_begin_auth_session(uint8 m_rgubTicketIGaveThisUser[], uint32 m_cubTicketIGaveThisUser, CSteamID steamID);

//     void test();
//     void steam_initialised();
//     void steam_periodic_test();
//     void steam_is_user_logged_on();

//     void steam_is_overlay_enabled();
//     void steam_activate_overlay();

//     void steam_create_leaderboard();
//     void steam_find_leaderboard();

//     void steam_set_achievement();

//     void steam_is_cloud_enabled_for_app();
//     void steam_is_cloud_enabled_for_account();

//     // Achievements & Stats
//     int steam_get_sprite_from_achievement_icon();
//     // int steam_image_get_size(int iImage);
//     // bool steam_image_get_rgba(int iImage, uint8 *pAvatarRGBA, int uImageSizeInBytes);

//     // social API
//     // int steam_get_sprite_from_avatar();
// }
