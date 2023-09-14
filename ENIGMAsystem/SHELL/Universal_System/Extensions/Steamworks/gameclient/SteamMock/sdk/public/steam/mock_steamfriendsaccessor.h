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

#ifndef MOCK_STEAMFRIENDS_H
#define MOCK_STEAMFRIENDS_H

#include "mock_isteamfriends.h"

class SteamFriendsAccessor : public ISteamFriends {
 public:
 static SteamFriendsAccessor &getInstance() {
    static SteamFriendsAccessor instance;
    return instance;
  }

  // SteamFriendsAccessor(SteamFriendsAccessor const &) = delete;
  // void operator=(SteamFriendsAccessor const &) = delete;

  const char *GetPersonaName() override;

  const char *GetFriendPersonaName(CSteamID steamIDFriend) override;

  void ActivateGameOverlay(const char *pchDialog) override;

  void ActivateGameOverlayToUser(const char *pchDialog, CSteamID steamID) override;

  void ActivateGameOverlayToWebPage(const char *pchURL, EActivateGameOverlayToWebPageMode eMode = 0) override;

  void SetPlayedWith(CSteamID steamIDUserPlayedWith) override;

  void ActivateGameOverlayInviteDialog(CSteamID steamIDLobby) override;

  int GetSmallFriendAvatar(CSteamID steamIDFriend) override;

  int GetMediumFriendAvatar(CSteamID steamIDFriend) override;

  int GetLargeFriendAvatar(CSteamID steamIDFriend) override;

  bool SetRichPresence(const char *pchKey, const char *pchValue) override;
  void ClearRichPresence() override;

  private:
  SteamFriendsAccessor(){}
  ~SteamFriendsAccessor() = default;

  SteamFriendsAccessor(SteamFriendsAccessor const &);
  void operator=(SteamFriendsAccessor const &);
};

inline ISteamFriends *SteamFriends() {
  return &SteamFriendsAccessor::getInstance();
}

#endif  // MOCK_STEAMFRIENDS_H
