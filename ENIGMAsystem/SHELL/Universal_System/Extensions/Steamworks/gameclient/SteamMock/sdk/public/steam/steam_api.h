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

/**
 * @brief This MACRO is already defined in `ENIGMAsystem/SHELL/Makefile`.
 * 
 */
#ifndef ENIGMA_STEAMWORKS_API_MOCK
#define ENIGMA_STEAMWORKS_API_MOCK
#endif  // ENIGMA_STEAMWORKS_API_MOCK

#ifndef MOCK_STEAM_API_H
#define MOCK_STEAM_API_H

#include "mock_steam_api_common.h"

#include "mock_steamappsaccessor.h"
#include "mock_steamfriendsaccessor.h"
#include "mock_steamuseraccessor.h"
#include "mock_steamuserstatsaccessor.h"
#include "mock_steamutilsaccessor.h"

bool SteamAPI_Init();

void SteamAPI_Shutdown();

bool SteamAPI_RestartAppIfNecessary(uint32 unOwnAppID);

#endif  // MOCK_STEAM_API_H
