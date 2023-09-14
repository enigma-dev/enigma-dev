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

#ifndef MOCK_ISTEAMFRIENDS_H
#define MOCK_ISTEAMFRIENDS_H

#include "mock_steam_api_common.h"

class ISteamFriends {
 public:
  virtual const char *GetPersonaName() = 0;

  virtual const char *GetFriendPersonaName(CSteamID steamIDFriend) = 0;

  virtual void ActivateGameOverlay(const char *pchDialog) = 0;

  virtual void ActivateGameOverlayToUser(const char *pchDialog, CSteamID steamID) = 0;

  virtual void ActivateGameOverlayToWebPage(const char *pchURL, EActivateGameOverlayToWebPageMode eMode = 0) = 0;

  virtual void SetPlayedWith(CSteamID steamIDUserPlayedWith) = 0;

  virtual void ActivateGameOverlayInviteDialog(CSteamID steamIDLobby) = 0;

  virtual int GetSmallFriendAvatar(CSteamID steamIDFriend) = 0;

  virtual int GetMediumFriendAvatar(CSteamID steamIDFriend) = 0;

  virtual int GetLargeFriendAvatar(CSteamID steamIDFriend) = 0;

  virtual bool SetRichPresence(const char *pchKey, const char *pchValue) = 0;
  virtual void ClearRichPresence() = 0;
};

struct GameOverlayActivatedMock
{
	uint8 m_bActive;
	bool m_bUserInitiated;
	AppId_t m_nAppID;
};

#endif  // MOCK_ISTEAMFRIENDS_H
