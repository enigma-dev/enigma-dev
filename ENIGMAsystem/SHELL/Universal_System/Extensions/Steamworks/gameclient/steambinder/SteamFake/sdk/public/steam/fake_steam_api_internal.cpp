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

#include "fake_steam_api_internal.h"

#include "fake_steam_api.h"

#include "fake_steamapps.h"
#include "fake_steamfriends.h"
#include "fake_steamuser.h"
#include "fake_steamuserstats.h"
#include "fake_steamutils.h"

HSteamUser SteamAPI_GetHSteamUser() { return 0; }

void *SteamInternal_ContextInit(void *pContextInitData) {
  FakeSteamContextInitData *initData = static_cast<FakeSteamContextInitData *>(pContextInitData);

  void **ptr = &initData->ptr;

  initData->pFn(ptr);

  return ptr;
}

void *SteamInternal_FindOrCreateUserInterface(HSteamUser hSteamUser, const char *pszVersion) {
  if (strcmp(pszVersion, STEAMAPPS_INTERFACE_VERSION) == 0) {
    return FakeSteamApps::GetInstance();
  }

  if (strcmp(pszVersion, STEAMFRIENDS_INTERFACE_VERSION) == 0) {
    return FakeSteamFriends::GetInstance();
  }

  if (strcmp(pszVersion, STEAMUSER_INTERFACE_VERSION) == 0) {
    return FakeSteamUser::GetInstance();
  }

  if (strcmp(pszVersion, STEAMUSERSTATS_INTERFACE_VERSION) == 0) {
    return FakeSteamUserStats::GetInstance();
  }

  if (strcmp(pszVersion, STEAMUTILS_INTERFACE_VERSION) == 0) {
    return FakeSteamUtils::GetInstance();
  }

  return nullptr;
}

void SteamAPI_RegisterCallback(class CCallbackBase *pCallback, int iCallback) {
  switch (iCallback) {
    case (int)(k_iSteamFriendsCallbacks + 31):
      FakeSteamFriends::GetInstance()->RegisterGameOverlayActivatedCallback(pCallback);
      break;
    case (int)(k_iSteamUserStatsCallbacks + 1):
      FakeSteamUserStats::GetInstance()->RegisterUserStatsReceivedCallback(pCallback);
      break;
    case (int)(k_iSteamUserStatsCallbacks + 2):
      FakeSteamUserStats::GetInstance()->RegisterUserStatsStoredCallback(pCallback);
      break;
    case (int)(k_iSteamUserStatsCallbacks + 3):
      FakeSteamUserStats::GetInstance()->RegisterUserAchievementStoredCallback(pCallback);
      break;
    default:
      break;
  }
}

void SteamAPI_UnregisterCallback(class CCallbackBase *pCallback) {}

// TODO: Implement this function.
void SteamAPI_RegisterCallResult(class CCallbackBase *pCallback, SteamAPICall_t hAPICall) {
  // switch (pCallback->GetICallback()) {
  //   case (int)(k_iSteamUserStatsCallbacks + 4):
  //     FakeSteamUserStats::GetInstance()->RegisterLeaderboardFindResultCallresult(pCallback, hAPICall);
  //     break;
  //   case (int)(k_iSteamUserStatsCallbacks + 5):
  //     FakeSteamUserStats::GetInstance()->RegisterLeaderboardScoresDownloadedCallresult(pCallback, hAPICall);
  //     break;
  //   case (int)(k_iSteamUserStatsCallbacks + 6):
  //     FakeSteamUserStats::GetInstance()->RegisterLeaderboardScoreUploadedCallresult(pCallback, hAPICall);
  //     break;
  //   default:
  //     break;
  // }
}

void SteamAPI_UnregisterCallResult(class CCallbackBase *pCallback, SteamAPICall_t hAPICall) {}
