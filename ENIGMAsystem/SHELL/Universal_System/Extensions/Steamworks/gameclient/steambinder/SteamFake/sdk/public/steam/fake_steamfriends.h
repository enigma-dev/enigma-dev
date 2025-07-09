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

#ifndef FAKE_STEAMFRIENDS_H
#define FAKE_STEAMFRIENDS_H

#include "fake_steam_api.h"

class FakeSteamFriends : public ISteamFriends {
 public:
  static FakeSteamFriends *GetInstance() {
    static FakeSteamFriends instance;
    return &instance;
  }

  void RegisterGameOverlayActivatedCallback(class CCallbackBase *pCallback);

  const char *GetPersonaName() override;

  SteamAPICall_t SetPersonaName(const char *pchPersonaName) override;

  EPersonaState GetPersonaState() override;

  int GetFriendCount(int iFriendFlags) override;

  CSteamID GetFriendByIndex(int iFriend, int iFriendFlags) override;

  EFriendRelationship GetFriendRelationship(CSteamID steamIDFriend) override;

  EPersonaState GetFriendPersonaState(CSteamID steamIDFriend) override;

  const char *GetFriendPersonaName(CSteamID steamIDFriend) override;

  bool GetFriendGamePlayed(CSteamID steamIDFriend, STEAM_OUT_STRUCT() FriendGameInfo_t *pFriendGameInfo) override;

  const char *GetFriendPersonaNameHistory(CSteamID steamIDFriend, int iPersonaName) override;

  int GetFriendSteamLevel(CSteamID steamIDFriend) override;

  const char *GetPlayerNickname(CSteamID steamIDPlayer) override;

  int GetFriendsGroupCount() override;

  FriendsGroupID_t GetFriendsGroupIDByIndex(int iFG) override;

  const char *GetFriendsGroupName(FriendsGroupID_t friendsGroupID) override;

  int GetFriendsGroupMembersCount(FriendsGroupID_t friendsGroupID) override;

  void GetFriendsGroupMembersList(FriendsGroupID_t friendsGroupID,
                                  STEAM_OUT_ARRAY_CALL(nMembersCount, GetFriendsGroupMembersCount, friendsGroupID)
                                      CSteamID *pOutSteamIDMembers,
                                  int nMembersCount) override;

  bool HasFriend(CSteamID steamIDFriend, int iFriendFlags) override;

  int GetClanCount() override;
  CSteamID GetClanByIndex(int iClan) override;
  const char *GetClanName(CSteamID steamIDClan) override;
  const char *GetClanTag(CSteamID steamIDClan) override;

  bool GetClanActivityCounts(CSteamID steamIDClan, int *pnOnline, int *pnInGame, int *pnChatting) override;

  SteamAPICall_t DownloadClanActivityCounts(STEAM_ARRAY_COUNT(cClansToRequest) CSteamID *psteamIDClans,
                                            int cClansToRequest) override;

  int GetFriendCountFromSource(CSteamID steamIDSource) override;
  CSteamID GetFriendFromSourceByIndex(CSteamID steamIDSource, int iFriend) override;

  bool IsUserInSource(CSteamID steamIDUser, CSteamID steamIDSource) override;

  void SetInGameVoiceSpeaking(CSteamID steamIDUser, bool bSpeaking) override;

  void ActivateGameOverlay(const char *pchDialog) override;

  /**
   * @brief This function will be used to simulate the Steam Overlay deactivation using keyboard 
   *        shortcuts or close button on the top right corner.
   * 
   * @note This function is not part of the Steamworks API. It is only used for testing purposes.
   * 
   */
  void DeactivateGameOverlay();

  void ActivateGameOverlayToUser(const char *pchDialog, CSteamID steamID) override;

  void ActivateGameOverlayToWebPage(const char *pchURL, EActivateGameOverlayToWebPageMode eMode =
                                                            k_EActivateGameOverlayToWebPageMode_Default) override;

  void ActivateGameOverlayToStore(AppId_t nAppID, EOverlayToStoreFlag eFlag) override;

  void SetPlayedWith(CSteamID steamIDUserPlayedWith) override;

  void ActivateGameOverlayInviteDialog(CSteamID steamIDLobby) override;

  int GetSmallFriendAvatar(CSteamID steamIDFriend) override;

  int GetMediumFriendAvatar(CSteamID steamIDFriend) override;

  int GetLargeFriendAvatar(CSteamID steamIDFriend) override;

  bool RequestUserInformation(CSteamID steamIDUser, bool bRequireNameOnly) override;

  SteamAPICall_t RequestClanOfficerList(CSteamID steamIDClan) override;

  CSteamID GetClanOwner(CSteamID steamIDClan) override;

  int GetClanOfficerCount(CSteamID steamIDClan) override;

  CSteamID GetClanOfficerByIndex(CSteamID steamIDClan, int iOfficer) override;

  uint32 GetUserRestrictions() override;

  bool SetRichPresence(const char *pchKey, const char *pchValue) override;
  void ClearRichPresence() override;
  const char *GetFriendRichPresence(CSteamID steamIDFriend, const char *pchKey) override;
  int GetFriendRichPresenceKeyCount(CSteamID steamIDFriend) override;
  const char *GetFriendRichPresenceKeyByIndex(CSteamID steamIDFriend, int iKey) override;

  void RequestFriendRichPresence(CSteamID steamIDFriend) override;

  bool InviteUserToGame(CSteamID steamIDFriend, const char *pchConnectString) override;

  int GetCoplayFriendCount() override;
  CSteamID GetCoplayFriend(int iCoplayFriend) override;
  int GetFriendCoplayTime(CSteamID steamIDFriend) override;
  AppId_t GetFriendCoplayGame(CSteamID steamIDFriend) override;

  SteamAPICall_t JoinClanChatRoom(CSteamID steamIDClan) override;
  bool LeaveClanChatRoom(CSteamID steamIDClan) override;
  int GetClanChatMemberCount(CSteamID steamIDClan) override;
  CSteamID GetChatMemberByIndex(CSteamID steamIDClan, int iUser) override;
  bool SendClanChatMessage(CSteamID steamIDClanChat, const char *pchText) override;
  int GetClanChatMessage(CSteamID steamIDClanChat, int iMessage, void *prgchText, int cchTextMax,
                         EChatEntryType *peChatEntryType, STEAM_OUT_STRUCT() CSteamID *psteamidChatter) override;
  bool IsClanChatAdmin(CSteamID steamIDClanChat, CSteamID steamIDUser) override;

  bool IsClanChatWindowOpenInSteam(CSteamID steamIDClanChat) override;
  bool OpenClanChatWindowInSteam(CSteamID steamIDClanChat) override;
  bool CloseClanChatWindowInSteam(CSteamID steamIDClanChat) override;

  bool SetListenForFriendsMessages(bool bInterceptEnabled) override;
  bool ReplyToFriendMessage(CSteamID steamIDFriend, const char *pchMsgToSend) override;
  int GetFriendMessage(CSteamID steamIDFriend, int iMessageID, void *pvData, int cubData,
                       EChatEntryType *peChatEntryType) override;

  SteamAPICall_t GetFollowerCount(CSteamID steamID) override;
  SteamAPICall_t IsFollowing(CSteamID steamID) override;
  SteamAPICall_t EnumerateFollowingList(uint32 unStartIndex) override;

  bool IsClanPublic(CSteamID steamIDClan) override;
  bool IsClanOfficialGameGroup(CSteamID steamIDClan) override;

  int GetNumChatsWithUnreadPriorityMessages() override;

  void ActivateGameOverlayRemotePlayTogetherInviteDialog(CSteamID steamIDLobby) override;

  bool RegisterProtocolInOverlayBrowser(const char *pchProtocol) override;

  void ActivateGameOverlayInviteDialogConnectString(const char *pchConnectString) override;

  SteamAPICall_t RequestEquippedProfileItems(CSteamID steamID) override;
  bool BHasEquippedProfileItem(CSteamID steamID, ECommunityProfileItemType itemType) override;
  const char *GetProfileItemPropertyString(CSteamID steamID, ECommunityProfileItemType itemType,
                                           ECommunityProfileItemProperty prop) override;
  uint32 GetProfileItemPropertyUint(CSteamID steamID, ECommunityProfileItemType itemType,
                                    ECommunityProfileItemProperty prop) override;

 private:
  FakeSteamFriends() {}
  ~FakeSteamFriends() {}

  class CCallbackBase *pCallbackGameOverlayActivated { nullptr };

  FakeSteamFriends(FakeSteamFriends const &) = delete;
  void operator=(FakeSteamFriends const &) = delete;
};

#endif  // FAKE_STEAMFRIENDS_H
