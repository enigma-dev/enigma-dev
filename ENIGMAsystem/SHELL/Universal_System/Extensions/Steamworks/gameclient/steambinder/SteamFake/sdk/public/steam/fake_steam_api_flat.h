/** Copyright (C) 2023-2024 Saif Kandil (k0T0z)
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

#ifndef FAKE_STEAM_API_FLAT_H
#define FAKE_STEAM_API_FLAT_H

#include "steam/steam_api_flat.h"

/**
 * @note Only for testing purposes.
 * 
 * @param self 
 */
S_API void SteamAPI_ISteamFriends_DeactivateGameOverlay(ISteamFriends* self);

#endif  // FAKE_STEAM_API_FLAT_H
