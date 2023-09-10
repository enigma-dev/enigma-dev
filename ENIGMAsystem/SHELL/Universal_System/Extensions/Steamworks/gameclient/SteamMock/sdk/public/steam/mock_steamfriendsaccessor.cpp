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

#include "mock_steamfriendsaccessor.h"

const char *SteamFriendsAccessor::GetPersonaName() { return "MockName"; }

const char *SteamFriendsAccessor::GetFriendPersonaName(CSteamID steamIDFriend) { return "MockFriendName"; }

void SteamFriendsAccessor::ActivateGameOverlay(const char *pchDialog) {}

void SteamFriendsAccessor::ActivateGameOverlayToUser(const char *pchDialog, CSteamID steamID) {}

void SteamFriendsAccessor::ActivateGameOverlayToWebPage(const char *pchURL, EActivateGameOverlayToWebPageMode eMode) {}

void SteamFriendsAccessor::SetPlayedWith(CSteamID steamIDUserPlayedWith) {}

void SteamFriendsAccessor::ActivateGameOverlayInviteDialog(CSteamID steamIDLobby) {}

int SteamFriendsAccessor::GetSmallFriendAvatar(CSteamID steamIDFriend) { return 0; }

int SteamFriendsAccessor::GetMediumFriendAvatar(CSteamID steamIDFriend) { return 0; }

int SteamFriendsAccessor::GetLargeFriendAvatar(CSteamID steamIDFriend) { return 0; }

bool SteamFriendsAccessor::SetRichPresence(const char *pchKey, const char *pchValue) { return true; }
void SteamFriendsAccessor::ClearRichPresence() {}
