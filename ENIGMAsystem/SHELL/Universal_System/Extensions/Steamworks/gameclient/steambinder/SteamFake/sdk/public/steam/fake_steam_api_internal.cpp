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
    case (int)(k_iSteamFriendsCallbacks + 31): {
      GameOverlayActivated_t game_overlay_activated;
      game_overlay_activated.m_bActive = true;
      game_overlay_activated.m_bUserInitiated = true;
      game_overlay_activated.m_nAppID = SteamUtils()->GetAppID();
      pCallback->Run(&game_overlay_activated);
    } break;
    case (int)(k_iSteamUserStatsCallbacks + 1): {
      UserStatsReceived_t user_stats_received;
      user_stats_received.m_nGameID = CGameID(SteamUtils()->GetAppID()).ToUint64();
      user_stats_received.m_eResult = k_EResultOK;
      user_stats_received.m_steamIDUser = SteamUser()->GetSteamID();
      pCallback->Run(&user_stats_received);
    } break;
    case (int)(k_iSteamUserStatsCallbacks + 2): {
      UserStatsStored_t user_stats_stored;
      user_stats_stored.m_nGameID = CGameID(SteamUtils()->GetAppID()).ToUint64();
      user_stats_stored.m_eResult = k_EResultOK;
      pCallback->Run(&user_stats_stored);
    } break;
    case (int)(k_iSteamUserStatsCallbacks + 3): {
      UserAchievementStored_t user_achievement_stored;
      user_achievement_stored.m_nGameID = CGameID(SteamUtils()->GetAppID()).ToUint64();
      user_achievement_stored.m_bGroupAchievement = false;
      user_achievement_stored.m_rgchAchievementName[0] = '\0';
      user_achievement_stored.m_nCurProgress = 0;
      user_achievement_stored.m_nMaxProgress = 0;
      pCallback->Run(&user_achievement_stored);
    } break;
    default:
      break;
  }
}

void SteamAPI_UnregisterCallback(class CCallbackBase *pCallback) {}

void SteamAPI_RegisterCallResult(class CCallbackBase *pCallback, SteamAPICall_t hAPICall) {
  switch (pCallback->GetICallback()) {
    case (int)(k_iSteamUserStatsCallbacks + 4): {
      LeaderboardFindResult_t leaderboard_find_result;
      leaderboard_find_result.m_hSteamLeaderboard = 0;
      leaderboard_find_result.m_bLeaderboardFound = true;
      pCallback->Run(&leaderboard_find_result, false, hAPICall);
    } break;
    case (int)(k_iSteamUserStatsCallbacks + 5): {
      LeaderboardScoresDownloaded_t leaderboard_scores_downloaded;
      leaderboard_scores_downloaded.m_hSteamLeaderboard = 0;
      leaderboard_scores_downloaded.m_hSteamLeaderboardEntries = 0;
      leaderboard_scores_downloaded.m_cEntryCount = 1;
      pCallback->Run(&leaderboard_scores_downloaded, false, hAPICall);
    } break;
    case (int)(k_iSteamUserStatsCallbacks + 6): {
      LeaderboardScoreUploaded_t leaderboard_score_uploaded;
      leaderboard_score_uploaded.m_bSuccess = true;
      leaderboard_score_uploaded.m_hSteamLeaderboard = 0;
      leaderboard_score_uploaded.m_nScore = -1;
      leaderboard_score_uploaded.m_bScoreChanged = false;
      leaderboard_score_uploaded.m_nGlobalRankNew = -1;
      leaderboard_score_uploaded.m_nGlobalRankPrevious = -1;
      pCallback->Run(&leaderboard_score_uploaded, false, hAPICall);
    } break;
    default:
      break;
  }
}

void SteamAPI_UnregisterCallResult(class CCallbackBase *pCallback, SteamAPICall_t hAPICall) {}
