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

#include "fake_steam_api.h"

#include "fake_steam_api_flat.h"
#include "fake_steamfriends.h"

/**
 * @brief These versions will be updated on each new version of Steamworks SDK.
 * 
 */
#define SteamAPI_SteamUser_vXXX SteamAPI_SteamUser_v023
#define SteamAPI_SteamFriends_vXXX SteamAPI_SteamFriends_v017
#define SteamAPI_SteamUtils_vXXX SteamAPI_SteamUtils_v010
#define SteamAPI_SteamUserStats_vXXX SteamAPI_SteamUserStats_v012
#define SteamAPI_SteamApps_vXXX SteamAPI_SteamApps_v008

/////////////////////////////////////////////// SteamUser ///////////////////////////////////////////////

ISteamUser* SteamAPI_SteamUser_vXXX() { return SteamUser(); }

bool SteamAPI_ISteamUser_BLoggedOn(ISteamUser* self) { return self->BLoggedOn(); }
uint64_steamid SteamAPI_ISteamUser_GetSteamID(ISteamUser* self) { return self->GetSteamID().ConvertToUint64(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////// SteamFriends ////////////////////////////////////////////

ISteamFriends* SteamAPI_SteamFriends_vXXX() { return SteamFriends(); }

const char* SteamAPI_ISteamFriends_GetPersonaName(ISteamFriends* self) { return self->GetPersonaName(); }

const char* SteamAPI_ISteamFriends_GetFriendPersonaName(ISteamFriends* self, uint64_steamid steamIDFriend) {
  return self->GetFriendPersonaName(CSteamID(steamIDFriend));
}

void SteamAPI_ISteamFriends_ActivateGameOverlay(ISteamFriends* self, const char* pchDialog) {
  self->ActivateGameOverlay(pchDialog);
}

void SteamAPI_ISteamFriends_DeactivateGameOverlay(ISteamFriends* self) {
  FakeSteamFriends::GetInstance()->DeactivateGameOverlay();
}

void SteamAPI_ISteamFriends_ActivateGameOverlayToUser(ISteamFriends* self, const char* pchDialog,
                                                      uint64_steamid steamID) {
  self->ActivateGameOverlayToUser(pchDialog, CSteamID(steamID));
}

void SteamAPI_ISteamFriends_ActivateGameOverlayToWebPage(ISteamFriends* self, const char* pchURL,
                                                         EActivateGameOverlayToWebPageMode eMode) {
  self->ActivateGameOverlayToWebPage(pchURL, eMode);
}

void SteamAPI_ISteamFriends_ActivateGameOverlayToStore(ISteamFriends* self, AppId_t nAppID, EOverlayToStoreFlag eFlag) {
  self->ActivateGameOverlayToStore(nAppID, eFlag);
}

void SteamAPI_ISteamFriends_SetPlayedWith(ISteamFriends* self, uint64_steamid steamIDUserPlayedWith) {
  self->SetPlayedWith(CSteamID(steamIDUserPlayedWith));
}

int SteamAPI_ISteamFriends_GetSmallFriendAvatar(ISteamFriends* self, uint64_steamid steamIDFriend) {
  return self->GetSmallFriendAvatar(CSteamID(steamIDFriend));
}

int SteamAPI_ISteamFriends_GetMediumFriendAvatar(ISteamFriends* self, uint64_steamid steamIDFriend) {
  return self->GetMediumFriendAvatar(CSteamID(steamIDFriend));
}

int SteamAPI_ISteamFriends_GetLargeFriendAvatar(ISteamFriends* self, uint64_steamid steamIDFriend) {
  return self->GetLargeFriendAvatar(CSteamID(steamIDFriend));
}

bool SteamAPI_ISteamFriends_SetRichPresence(ISteamFriends* self, const char* pchKey, const char* pchValue) {
  return self->SetRichPresence(pchKey, pchValue);
}

void SteamAPI_ISteamFriends_ClearRichPresence(ISteamFriends* self) { self->ClearRichPresence(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////// SteamUtils //////////////////////////////////////////////

ISteamUtils* SteamAPI_SteamUtils_vXXX() { return SteamUtils(); }

bool SteamAPI_ISteamUtils_GetImageSize(ISteamUtils* self, int iImage, uint32* pnWidth, uint32* pnHeight) {
  return self->GetImageSize(iImage, pnWidth, pnHeight);
}

bool SteamAPI_ISteamUtils_GetImageRGBA(ISteamUtils* self, int iImage, uint8* pubDest, int nDestBufferSize) {
  return self->GetImageRGBA(iImage, pubDest, nDestBufferSize);
}

uint32 SteamAPI_ISteamUtils_GetAppID(ISteamUtils* self) { return self->GetAppID(); }

void SteamAPI_ISteamUtils_SetOverlayNotificationPosition(ISteamUtils* self,
                                                         ENotificationPosition eNotificationPosition) {
  self->SetOverlayNotificationPosition(eNotificationPosition);
}

void SteamAPI_ISteamUtils_SetWarningMessageHook(ISteamUtils* self, SteamAPIWarningMessageHook_t pFunction) {
  self->SetWarningMessageHook(pFunction);
}

bool SteamAPI_ISteamUtils_IsOverlayEnabled(ISteamUtils* self) { return self->IsOverlayEnabled(); }

void SteamAPI_ISteamUtils_SetOverlayNotificationInset(ISteamUtils* self, int nHorizontalInset, int nVerticalInset) {
  self->SetOverlayNotificationInset(nHorizontalInset, nVerticalInset);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////// SteamUserStats //////////////////////////////////////////

ISteamUserStats* SteamAPI_SteamUserStats_vXXX() { return SteamUserStats(); }

bool SteamAPI_ISteamUserStats_RequestCurrentStats(ISteamUserStats* self) { return self->RequestCurrentStats(); }

bool SteamAPI_ISteamUserStats_GetStatInt32(ISteamUserStats* self, const char* pchName, int32* pData) {
  return self->GetStat(pchName, pData);
}

bool SteamAPI_ISteamUserStats_GetStatFloat(ISteamUserStats* self, const char* pchName, float* pData) {
  return self->GetStat(pchName, pData);
}

bool SteamAPI_ISteamUserStats_SetStatInt32(ISteamUserStats* self, const char* pchName, int32 nData) {
  return self->SetStat(pchName, nData);
}

bool SteamAPI_ISteamUserStats_SetStatFloat(ISteamUserStats* self, const char* pchName, float fData) {
  return self->SetStat(pchName, fData);
}

bool SteamAPI_ISteamUserStats_UpdateAvgRateStat(ISteamUserStats* self, const char* pchName, float flCountThisSession,
                                                double dSessionLength) {
  return self->UpdateAvgRateStat(pchName, flCountThisSession, dSessionLength);
}

bool SteamAPI_ISteamUserStats_GetAchievement(ISteamUserStats* self, const char* pchName, bool* pbAchieved) {
  return self->GetAchievement(pchName, pbAchieved);
}

bool SteamAPI_ISteamUserStats_SetAchievement(ISteamUserStats* self, const char* pchName) {
  return self->SetAchievement(pchName);
}

bool SteamAPI_ISteamUserStats_ClearAchievement(ISteamUserStats* self, const char* pchName) {
  return self->ClearAchievement(pchName);
}

bool SteamAPI_ISteamUserStats_StoreStats(ISteamUserStats* self) { return self->StoreStats(); }

bool SteamAPI_ISteamUserStats_ResetAllStats(ISteamUserStats* self, bool bAchievementsToo) {
  return self->ResetAllStats(bAchievementsToo);
}

SteamAPICall_t SteamAPI_ISteamUserStats_FindOrCreateLeaderboard(ISteamUserStats* self, const char* pchLeaderboardName,
                                                                ELeaderboardSortMethod eLeaderboardSortMethod,
                                                                ELeaderboardDisplayType eLeaderboardDisplayType) {
  return self->FindOrCreateLeaderboard(pchLeaderboardName, eLeaderboardSortMethod, eLeaderboardDisplayType);
}

SteamAPICall_t SteamAPI_ISteamUserStats_FindLeaderboard(ISteamUserStats* self, const char* pchLeaderboardName) {
  return self->FindLeaderboard(pchLeaderboardName);
}

const char* SteamAPI_ISteamUserStats_GetLeaderboardName(ISteamUserStats* self, SteamLeaderboard_t hSteamLeaderboard) {
  return self->GetLeaderboardName(hSteamLeaderboard);
}

SteamAPICall_t SteamAPI_ISteamUserStats_DownloadLeaderboardEntries(ISteamUserStats* self,
                                                                   SteamLeaderboard_t hSteamLeaderboard,
                                                                   ELeaderboardDataRequest eLeaderboardDataRequest,
                                                                   int nRangeStart, int nRangeEnd) {
  return self->DownloadLeaderboardEntries(hSteamLeaderboard, eLeaderboardDataRequest, nRangeStart, nRangeEnd);
}

bool SteamAPI_ISteamUserStats_GetDownloadedLeaderboardEntry(ISteamUserStats* self,
                                                            SteamLeaderboardEntries_t hSteamLeaderboardEntries,
                                                            int index, LeaderboardEntry_t* pLeaderboardEntry,
                                                            int32* pDetails, int cDetailsMax) {
  return self->GetDownloadedLeaderboardEntry(hSteamLeaderboardEntries, index, pLeaderboardEntry, pDetails, cDetailsMax);
}

SteamAPICall_t SteamAPI_ISteamUserStats_UploadLeaderboardScore(
    ISteamUserStats* self, SteamLeaderboard_t hSteamLeaderboard,
    ELeaderboardUploadScoreMethod eLeaderboardUploadScoreMethod, int32 nScore, const int32* pScoreDetails,
    int cScoreDetailsCount) {
  return self->UploadLeaderboardScore(hSteamLeaderboard, eLeaderboardUploadScoreMethod, nScore, pScoreDetails,
                                      cScoreDetailsCount);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////// SteamApps ///////////////////////////////////////////////

ISteamApps* SteamAPI_SteamApps_vXXX() { return SteamApps(); }

bool SteamAPI_ISteamApps_BIsSubscribed(ISteamApps* self) { return self->BIsSubscribed(); }

const char* SteamAPI_ISteamApps_GetCurrentGameLanguage(ISteamApps* self) { return self->GetCurrentGameLanguage(); }

const char* SteamAPI_ISteamApps_GetAvailableGameLanguages(ISteamApps* self) {
  return self->GetAvailableGameLanguages();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
