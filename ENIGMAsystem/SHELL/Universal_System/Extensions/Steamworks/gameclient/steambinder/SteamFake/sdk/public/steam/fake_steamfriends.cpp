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

#include "fake_steamfriends.h"

void FakeSteamFriends::RegisterGameOverlayActivatedCallback(class CCallbackBase *pCallback) {
  FakeSteamFriends::GetInstance()->pCallbackGameOverlayActivated = pCallback;
}

const char *FakeSteamFriends::GetPersonaName() { return "FakeSteamUser"; }

SteamAPICall_t FakeSteamFriends::SetPersonaName(const char *pchPersonaName) { return 0; }

EPersonaState FakeSteamFriends::GetPersonaState() { return k_EPersonaStateOffline; }

int FakeSteamFriends::GetFriendCount(int iFriendFlags) { return 0; }

CSteamID FakeSteamFriends::GetFriendByIndex(int iFriend, int iFriendFlags) { return CSteamID(); }

EFriendRelationship FakeSteamFriends::GetFriendRelationship(CSteamID steamIDFriend) {
  return k_EFriendRelationshipNone;
}

EPersonaState FakeSteamFriends::GetFriendPersonaState(CSteamID steamIDFriend) { return k_EPersonaStateOffline; }

const char *FakeSteamFriends::GetFriendPersonaName(CSteamID steamIDFriend) { return "FakeSteamUser"; }

bool FakeSteamFriends::GetFriendGamePlayed(CSteamID steamIDFriend,
                                           STEAM_OUT_STRUCT() FriendGameInfo_t *pFriendGameInfo) {
  return true;
}

const char *FakeSteamFriends::GetFriendPersonaNameHistory(CSteamID steamIDFriend, int iPersonaName) {
  return "FakeSteamUser";
}

int FakeSteamFriends::GetFriendSteamLevel(CSteamID steamIDFriend) { return 0; }

const char *FakeSteamFriends::GetPlayerNickname(CSteamID steamIDPlayer) { return "FakeSteamUser"; }

int FakeSteamFriends::GetFriendsGroupCount() { return 0; }

FriendsGroupID_t FakeSteamFriends::GetFriendsGroupIDByIndex(int iFG) { return 0; }

const char *FakeSteamFriends::GetFriendsGroupName(FriendsGroupID_t friendsGroupID) { return "FakeFriendsGroup"; }

int FakeSteamFriends::GetFriendsGroupMembersCount(FriendsGroupID_t friendsGroupID) { return 0; }

void FakeSteamFriends::GetFriendsGroupMembersList(FriendsGroupID_t friendsGroupID,
                                                  STEAM_OUT_ARRAY_CALL(nMembersCount, GetFriendsGroupMembersCount,
                                                                       friendsGroupID) CSteamID *pOutSteamIDMembers,
                                                  int nMembersCount) {}

bool FakeSteamFriends::HasFriend(CSteamID steamIDFriend, int iFriendFlags) { return true; }

int FakeSteamFriends::GetClanCount() { return 0; }
CSteamID FakeSteamFriends::GetClanByIndex(int iClan) { return CSteamID(); }
const char *FakeSteamFriends::GetClanName(CSteamID steamIDClan) { return "FakeClan"; }
const char *FakeSteamFriends::GetClanTag(CSteamID steamIDClan) { return "FakeClan"; }

bool FakeSteamFriends::GetClanActivityCounts(CSteamID steamIDClan, int *pnOnline, int *pnInGame, int *pnChatting) {
  return true;
}

SteamAPICall_t FakeSteamFriends::DownloadClanActivityCounts(STEAM_ARRAY_COUNT(cClansToRequest) CSteamID *psteamIDClans,
                                                            int cClansToRequest) {
  return 0;
}

int FakeSteamFriends::GetFriendCountFromSource(CSteamID steamIDSource) { return 0; }
CSteamID FakeSteamFriends::GetFriendFromSourceByIndex(CSteamID steamIDSource, int iFriend) { return CSteamID(); }

bool FakeSteamFriends::IsUserInSource(CSteamID steamIDUser, CSteamID steamIDSource) { return true; }

void FakeSteamFriends::SetInGameVoiceSpeaking(CSteamID steamIDUser, bool bSpeaking) {}

void FakeSteamFriends::ActivateGameOverlay(const char *pchDialog) {
  if (FakeSteamFriends::GetInstance()->pCallbackGameOverlayActivated == nullptr) {
    return;
  }

  GameOverlayActivated_t game_overlay_activated;
  game_overlay_activated.m_bActive = true;
  game_overlay_activated.m_bUserInitiated = true;
  game_overlay_activated.m_nAppID = SteamUtils()->GetAppID();
  FakeSteamFriends::GetInstance()->pCallbackGameOverlayActivated->Run(&game_overlay_activated);
}

void FakeSteamFriends::DeactivateGameOverlay() {
  if (FakeSteamFriends::GetInstance()->pCallbackGameOverlayActivated == nullptr) {
    return;
  }

  GameOverlayActivated_t game_overlay_activated;
  game_overlay_activated.m_bActive = false;
  game_overlay_activated.m_bUserInitiated = true;
  game_overlay_activated.m_nAppID = SteamUtils()->GetAppID();
  FakeSteamFriends::GetInstance()->pCallbackGameOverlayActivated->Run(&game_overlay_activated);
}

void FakeSteamFriends::ActivateGameOverlayToUser(const char *pchDialog, CSteamID steamID) {}

void FakeSteamFriends::ActivateGameOverlayToWebPage(const char *pchURL, EActivateGameOverlayToWebPageMode eMode) {}

void FakeSteamFriends::ActivateGameOverlayToStore(AppId_t nAppID, EOverlayToStoreFlag eFlag) {}

void FakeSteamFriends::SetPlayedWith(CSteamID steamIDUserPlayedWith) {}

void FakeSteamFriends::ActivateGameOverlayInviteDialog(CSteamID steamIDLobby) {}

int FakeSteamFriends::GetSmallFriendAvatar(CSteamID steamIDFriend) { return 0; }

int FakeSteamFriends::GetMediumFriendAvatar(CSteamID steamIDFriend) { return 0; }

int FakeSteamFriends::GetLargeFriendAvatar(CSteamID steamIDFriend) { return 0; }

bool FakeSteamFriends::RequestUserInformation(CSteamID steamIDUser, bool bRequireNameOnly) { return true; }

SteamAPICall_t FakeSteamFriends::RequestClanOfficerList(CSteamID steamIDClan) { return 0; }

CSteamID FakeSteamFriends::GetClanOwner(CSteamID steamIDClan) { return CSteamID(); }

int FakeSteamFriends::GetClanOfficerCount(CSteamID steamIDClan) { return 0; }

CSteamID FakeSteamFriends::GetClanOfficerByIndex(CSteamID steamIDClan, int iOfficer) { return CSteamID(); }

uint32 FakeSteamFriends::GetUserRestrictions() { return 0; }

bool FakeSteamFriends::SetRichPresence(const char *pchKey, const char *pchValue) { return true; }
void FakeSteamFriends::ClearRichPresence() {}
const char *FakeSteamFriends::GetFriendRichPresence(CSteamID steamIDFriend, const char *pchKey) {
  return "FakeRichPresence";
}
int FakeSteamFriends::GetFriendRichPresenceKeyCount(CSteamID steamIDFriend) { return 0; }
const char *FakeSteamFriends::GetFriendRichPresenceKeyByIndex(CSteamID steamIDFriend, int iKey) {
  return "FakeRichPresence";
}

void FakeSteamFriends::RequestFriendRichPresence(CSteamID steamIDFriend) {}

bool FakeSteamFriends::InviteUserToGame(CSteamID steamIDFriend, const char *pchConnectString) { return true; }

int FakeSteamFriends::GetCoplayFriendCount() { return 0; }
CSteamID FakeSteamFriends::GetCoplayFriend(int iCoplayFriend) { return CSteamID(); }
int FakeSteamFriends::GetFriendCoplayTime(CSteamID steamIDFriend) { return 0; }
AppId_t FakeSteamFriends::GetFriendCoplayGame(CSteamID steamIDFriend) { return 0; }

SteamAPICall_t FakeSteamFriends::JoinClanChatRoom(CSteamID steamIDClan) { return 0; }
bool FakeSteamFriends::LeaveClanChatRoom(CSteamID steamIDClan) { return true; }
int FakeSteamFriends::GetClanChatMemberCount(CSteamID steamIDClan) { return 0; }
CSteamID FakeSteamFriends::GetChatMemberByIndex(CSteamID steamIDClan, int iUser) { return CSteamID(); }
bool FakeSteamFriends::SendClanChatMessage(CSteamID steamIDClanChat, const char *pchText) { return true; }
int FakeSteamFriends::GetClanChatMessage(CSteamID steamIDClanChat, int iMessage, void *prgchText, int cchTextMax,
                                         EChatEntryType *peChatEntryType,
                                         STEAM_OUT_STRUCT() CSteamID *psteamidChatter) {
  return 0;
}
bool FakeSteamFriends::IsClanChatAdmin(CSteamID steamIDClanChat, CSteamID steamIDUser) { return true; }

bool FakeSteamFriends::IsClanChatWindowOpenInSteam(CSteamID steamIDClanChat) { return true; }
bool FakeSteamFriends::OpenClanChatWindowInSteam(CSteamID steamIDClanChat) { return true; }
bool FakeSteamFriends::CloseClanChatWindowInSteam(CSteamID steamIDClanChat) { return true; }

bool FakeSteamFriends::SetListenForFriendsMessages(bool bInterceptEnabled) { return true; }
bool FakeSteamFriends::ReplyToFriendMessage(CSteamID steamIDFriend, const char *pchMsgToSend) { return true; }
int FakeSteamFriends::GetFriendMessage(CSteamID steamIDFriend, int iMessageID, void *pvData, int cubData,
                                       EChatEntryType *peChatEntryType) {
  return 0;
}

SteamAPICall_t FakeSteamFriends::GetFollowerCount(CSteamID steamID) { return 0; }
SteamAPICall_t FakeSteamFriends::IsFollowing(CSteamID steamID) { return 0; }
SteamAPICall_t FakeSteamFriends::EnumerateFollowingList(uint32 unStartIndex) { return 0; }

bool FakeSteamFriends::IsClanPublic(CSteamID steamIDClan) { return true; }
bool FakeSteamFriends::IsClanOfficialGameGroup(CSteamID steamIDClan) { return true; }

int FakeSteamFriends::GetNumChatsWithUnreadPriorityMessages() { return 0; }

void FakeSteamFriends::ActivateGameOverlayRemotePlayTogetherInviteDialog(CSteamID steamIDLobby) {}

bool FakeSteamFriends::RegisterProtocolInOverlayBrowser(const char *pchProtocol) { return true; }

void FakeSteamFriends::ActivateGameOverlayInviteDialogConnectString(const char *pchConnectString) {}

SteamAPICall_t FakeSteamFriends::RequestEquippedProfileItems(CSteamID steamID) { return 0; }
bool FakeSteamFriends::BHasEquippedProfileItem(CSteamID steamID, ECommunityProfileItemType itemType) { return true; }
const char *FakeSteamFriends::GetProfileItemPropertyString(CSteamID steamID, ECommunityProfileItemType itemType,
                                                           ECommunityProfileItemProperty prop) {
  return "FakeProfileItem";
}
uint32 FakeSteamFriends::GetProfileItemPropertyUint(CSteamID steamID, ECommunityProfileItemType itemType,
                                                    ECommunityProfileItemProperty prop) {
  return 0;
}
