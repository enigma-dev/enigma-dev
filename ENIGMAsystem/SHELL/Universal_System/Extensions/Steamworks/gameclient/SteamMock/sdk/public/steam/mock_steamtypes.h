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

#ifndef MOCK_STEAMTYPES_H
#define MOCK_STEAMTYPES_H

typedef unsigned int uint32;
typedef signed int int32;
typedef signed long int lint64;
typedef unsigned long int ulint64;
typedef unsigned long long int uint64;
typedef signed long long int int64;
typedef unsigned char uint8;
typedef signed char int8;

typedef int32 EActivateGameOverlayToWebPageMode;
typedef int32 ENotificationPosition;

typedef uint32 ELeaderboardDataRequest;

typedef uint32 ELeaderboardUploadScoreMethod;

typedef uint32 ELeaderboardSortMethod;
typedef uint32 ELeaderboardDisplayType;

typedef uint32 AppId_t;
const AppId_t k_uAppIdInvalid = 0x0;

typedef uint32 AccountID_t;

typedef uint64 SteamAPICall_t;
typedef uint64 SteamLeaderboard_t;
typedef uint64 SteamLeaderboardEntries_t;

#endif // MOCK_STEAMTYPES_H
