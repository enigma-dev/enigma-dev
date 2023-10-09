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

#include "fake_steamuserstats.h"

void FakeSteamUserStats::RegisterUserStatsReceivedCallback(class CCallbackBase *pCallback) {
  FakeSteamUserStats::GetInstance()->pCallbackUserStatsReceived = pCallback;
}

void FakeSteamUserStats::RegisterUserStatsStoredCallback(class CCallbackBase *pCallback) {
  FakeSteamUserStats::GetInstance()->pCallbackUserStatsStored = pCallback;
}

void FakeSteamUserStats::RegisterUserAchievementStoredCallback(class CCallbackBase *pCallback) {
  FakeSteamUserStats::GetInstance()->pCallbackUserAchievementStored = pCallback;
}

// void FakeSteamUserStats::RegisterLeaderboardFindResultCallresult(class CCallbackBase *pCallback,
//                                                                  SteamAPICall_t hAPICall) {
//   FakeSteamUserStats::GetInstance()->pCallresultLeaderboardFindResult = pCallback;
//   FakeSteamUserStats::GetInstance()->hAPICallLeaderboardFindResult = hAPICall;
// }

// void FakeSteamUserStats::RegisterLeaderboardScoresDownloadedCallresult(class CCallbackBase *pCallback,
//                                                                        SteamAPICall_t hAPICall) {
//   FakeSteamUserStats::GetInstance()->pCallresultLeaderboardScoresDownloaded = pCallback;
//   FakeSteamUserStats::GetInstance()->hAPICallLeaderboardScoresDownloaded = hAPICall;
// }

// void FakeSteamUserStats::RegisterLeaderboardScoreUploadedCallresult(class CCallbackBase *pCallback,
//                                                                     SteamAPICall_t hAPICall) {
//   FakeSteamUserStats::GetInstance()->pCallresultLeaderboardScoreUploaded = pCallback;
//   FakeSteamUserStats::GetInstance()->hAPICallLeaderboardScoreUploaded = hAPICall;
// }

bool FakeSteamUserStats::RequestCurrentStats() {
  if (FakeSteamUserStats::GetInstance()->pCallbackUserStatsReceived == nullptr) {
    return false;
  }

  UserStatsReceived_t user_stats_received;
  user_stats_received.m_nGameID = CGameID(SteamUtils()->GetAppID()).ToUint64();
  user_stats_received.m_eResult = k_EResultOK;
  user_stats_received.m_steamIDUser = SteamUser()->GetSteamID();
  FakeSteamUserStats::GetInstance()->pCallbackUserStatsReceived->Run(&user_stats_received);
  return true;
}

bool FakeSteamUserStats::GetStat(const char *pchName, int32 *pData) { return true; }

bool FakeSteamUserStats::GetStat(const char *pchName, float *pData) { return true; }

bool FakeSteamUserStats::SetStat(const char *pchName, int32 nData) { return true; }

bool FakeSteamUserStats::SetStat(const char *pchName, float fData) { return true; }

bool FakeSteamUserStats::UpdateAvgRateStat(const char *pchName, float flCountThisSession, double dSessionLength) {
  return true;
}

bool FakeSteamUserStats::GetAchievement(const char *pchName, bool *pbAchieved) { return true; }
bool FakeSteamUserStats::SetAchievement(const char *pchName) { return true; }
bool FakeSteamUserStats::ClearAchievement(const char *pchName) { return true; }

bool FakeSteamUserStats::GetAchievementAndUnlockTime(const char *pchName, bool *pbAchieved, uint32 *punUnlockTime) {
  return true;
}

bool FakeSteamUserStats::StoreStats() {
  if (FakeSteamUserStats::GetInstance()->pCallbackUserStatsStored == nullptr ||
      FakeSteamUserStats::GetInstance()->pCallbackUserAchievementStored == nullptr) {
    return false;
  }

  UserStatsStored_t user_stats_stored;
  user_stats_stored.m_nGameID = CGameID(SteamUtils()->GetAppID()).ToUint64();
  user_stats_stored.m_eResult = k_EResultOK;
  FakeSteamUserStats::GetInstance()->pCallbackUserStatsStored->Run(&user_stats_stored);

  UserAchievementStored_t user_achievement_stored;
  user_achievement_stored.m_nGameID = CGameID(SteamUtils()->GetAppID()).ToUint64();
  user_achievement_stored.m_bGroupAchievement = false;
  user_achievement_stored.m_rgchAchievementName[0] = '\0';
  user_achievement_stored.m_nCurProgress = 0;
  user_achievement_stored.m_nMaxProgress = 0;
  FakeSteamUserStats::GetInstance()->pCallbackUserAchievementStored->Run(&user_achievement_stored);

  return true;
}

int FakeSteamUserStats::GetAchievementIcon(const char *pchName) { return 0; }

const char *FakeSteamUserStats::GetAchievementDisplayAttribute(const char *pchName, const char *pchKey) {
  return "FakeAchievement";
}

bool FakeSteamUserStats::IndicateAchievementProgress(const char *pchName, uint32 nCurProgress, uint32 nMaxProgress) {
  return true;
}

uint32 FakeSteamUserStats::GetNumAchievements() { return 0; }

const char *FakeSteamUserStats::GetAchievementName(uint32 iAchievement) { return "FakeAchievement"; }

SteamAPICall_t FakeSteamUserStats::RequestUserStats(CSteamID steamIDUser) {
  return 1;  // Return a valid SteamAPICall_t
}

bool FakeSteamUserStats::GetUserStat(CSteamID steamIDUser, const char *pchName, int32 *pData) { return true; }

bool FakeSteamUserStats::GetUserStat(CSteamID steamIDUser, const char *pchName, float *pData) { return true; }

bool FakeSteamUserStats::GetUserAchievement(CSteamID steamIDUser, const char *pchName, bool *pbAchieved) {
  return true;
}

bool FakeSteamUserStats::GetUserAchievementAndUnlockTime(CSteamID steamIDUser, const char *pchName, bool *pbAchieved,
                                                         uint32 *punUnlockTime) {
  return true;
}

bool FakeSteamUserStats::ResetAllStats(bool bAchievementsToo) { return true; }

SteamAPICall_t FakeSteamUserStats::FindOrCreateLeaderboard(const char *pchLeaderboardName,
                                                           ELeaderboardSortMethod eLeaderboardSortMethod,
                                                           ELeaderboardDisplayType eLeaderboardDisplayType) {
  // if (FakeSteamUserStats::GetInstance()->pCallresultLeaderboardFindResult == nullptr) {
  //   return 1;  // Return a valid SteamAPICall_t
  // }

  // LeaderboardFindResult_t leaderboard_find_result;
  // leaderboard_find_result.m_hSteamLeaderboard = 0;
  // leaderboard_find_result.m_bLeaderboardFound = true;
  // FakeSteamUserStats::GetInstance()->pCallresultLeaderboardFindResult->Run(
  //     &leaderboard_find_result, false, FakeSteamUserStats::GetInstance()->hAPICallLeaderboardFindResult);
  // return FakeSteamUserStats::GetInstance()->hAPICallLeaderboardFindResult;
  return 1;  // Return a valid SteamAPICall_t
}

SteamAPICall_t FakeSteamUserStats::FindLeaderboard(const char *pchLeaderboardName) {
  // if (FakeSteamUserStats::GetInstance()->pCallresultLeaderboardFindResult == nullptr) {
  //   return 1;  // Return a valid SteamAPICall_t
  // }

  // LeaderboardFindResult_t leaderboard_find_result;
  // leaderboard_find_result.m_hSteamLeaderboard = 0;
  // leaderboard_find_result.m_bLeaderboardFound = true;
  // FakeSteamUserStats::GetInstance()->pCallresultLeaderboardFindResult->Run(
  //     &leaderboard_find_result, false, FakeSteamUserStats::GetInstance()->hAPICallLeaderboardFindResult);
  // return FakeSteamUserStats::GetInstance()->hAPICallLeaderboardFindResult;
  return 1;  // Return a valid SteamAPICall_t
}

const char *FakeSteamUserStats::GetLeaderboardName(SteamLeaderboard_t hSteamLeaderboard) { return "FakeLeaderboard"; }

int FakeSteamUserStats::GetLeaderboardEntryCount(SteamLeaderboard_t hSteamLeaderboard) { return 0; }

ELeaderboardSortMethod FakeSteamUserStats::GetLeaderboardSortMethod(SteamLeaderboard_t hSteamLeaderboard) {
  return k_ELeaderboardSortMethodNone;
}

ELeaderboardDisplayType FakeSteamUserStats::GetLeaderboardDisplayType(SteamLeaderboard_t hSteamLeaderboard) {
  return k_ELeaderboardDisplayTypeNone;
}

SteamAPICall_t FakeSteamUserStats::DownloadLeaderboardEntries(SteamLeaderboard_t hSteamLeaderboard,
                                                              ELeaderboardDataRequest eLeaderboardDataRequest,
                                                              int nRangeStart, int nRangeEnd) {
  // if (FakeSteamUserStats::GetInstance()->pCallresultLeaderboardScoresDownloaded == nullptr) {
  //   return 1;  // Return a valid SteamAPICall_t
  // }

  // LeaderboardScoresDownloaded_t leaderboard_scores_downloaded;
  // leaderboard_scores_downloaded.m_hSteamLeaderboard = 0;
  // leaderboard_scores_downloaded.m_hSteamLeaderboardEntries = 0;
  // leaderboard_scores_downloaded.m_cEntryCount = 1;
  // FakeSteamUserStats::GetInstance()->pCallresultLeaderboardScoresDownloaded->Run(
  //     &leaderboard_scores_downloaded, false, FakeSteamUserStats::GetInstance()->hAPICallLeaderboardScoresDownloaded);
  // return FakeSteamUserStats::GetInstance()->hAPICallLeaderboardScoresDownloaded;
  return 1;  // Return a valid SteamAPICall_t
}

SteamAPICall_t FakeSteamUserStats::DownloadLeaderboardEntriesForUsers(
    SteamLeaderboard_t hSteamLeaderboard, STEAM_ARRAY_COUNT_D(cUsers, Array of users to retrieve) CSteamID *prgUsers,
    int cUsers) {
  return 1;  // Return a valid SteamAPICall_t
}

bool FakeSteamUserStats::GetDownloadedLeaderboardEntry(SteamLeaderboardEntries_t hSteamLeaderboardEntries, int index,
                                                       LeaderboardEntry_t *pLeaderboardEntry, int32 *pDetails,
                                                       int cDetailsMax) {
  pLeaderboardEntry->m_steamIDUser = CSteamID();
  pLeaderboardEntry->m_nGlobalRank = 0;
  pLeaderboardEntry->m_nScore = 0;
  pLeaderboardEntry->m_cDetails = 0;
  return true;
}

SteamAPICall_t FakeSteamUserStats::UploadLeaderboardScore(SteamLeaderboard_t hSteamLeaderboard,
                                                          ELeaderboardUploadScoreMethod eLeaderboardUploadScoreMethod,
                                                          int32 nScore, const int32 *pScoreDetails,
                                                          int cScoreDetailsCount) {
  // if (FakeSteamUserStats::GetInstance()->pCallresultLeaderboardScoreUploaded == nullptr) {
  //   return 1;  // Return a valid SteamAPICall_t
  // }

  // LeaderboardScoreUploaded_t leaderboard_score_uploaded;
  // leaderboard_score_uploaded.m_bSuccess = true;
  // leaderboard_score_uploaded.m_hSteamLeaderboard = 0;
  // leaderboard_score_uploaded.m_nScore = -1;
  // leaderboard_score_uploaded.m_bScoreChanged = false;
  // leaderboard_score_uploaded.m_nGlobalRankNew = -1;
  // leaderboard_score_uploaded.m_nGlobalRankPrevious = -1;
  // FakeSteamUserStats::GetInstance()->pCallresultLeaderboardScoreUploaded->Run(
  //     &leaderboard_score_uploaded, false, FakeSteamUserStats::GetInstance()->hAPICallLeaderboardScoreUploaded);
  // return FakeSteamUserStats::GetInstance()->hAPICallLeaderboardScoreUploaded;
  return 1;  // Return a valid SteamAPICall_t
}

SteamAPICall_t FakeSteamUserStats::AttachLeaderboardUGC(SteamLeaderboard_t hSteamLeaderboard, UGCHandle_t hUGC) {
  return 1;  // Return a valid SteamAPICall_t
}

SteamAPICall_t FakeSteamUserStats::GetNumberOfCurrentPlayers() {
  return 1;  // Return a valid SteamAPICall_t
}

SteamAPICall_t FakeSteamUserStats::RequestGlobalAchievementPercentages() {
  return 1;  // Return a valid SteamAPICall_t
}

int FakeSteamUserStats::GetMostAchievedAchievementInfo(char *pchName, uint32 unNameBufLen, float *pflPercent,
                                                       bool *pbAchieved) {
  return 0;
}

int FakeSteamUserStats::GetNextMostAchievedAchievementInfo(int iIteratorPrevious, char *pchName, uint32 unNameBufLen,
                                                           float *pflPercent, bool *pbAchieved) {
  return 0;
}

bool FakeSteamUserStats::GetAchievementAchievedPercent(const char *pchName, float *pflPercent) { return true; }

SteamAPICall_t FakeSteamUserStats::RequestGlobalStats(int nHistoryDays) {
  return 1;  // Return a valid SteamAPICall_t
}

bool FakeSteamUserStats::GetGlobalStat(const char *pchStatName, int64 *pData) { return true; }

bool FakeSteamUserStats::GetGlobalStat(const char *pchStatName, double *pData) { return true; }

int32 FakeSteamUserStats::GetGlobalStatHistory(const char *pchStatName, STEAM_ARRAY_COUNT(cubData) int64 *pData,
                                               uint32 cubData) {
  return 0;
}

int32 FakeSteamUserStats::GetGlobalStatHistory(const char *pchStatName, STEAM_ARRAY_COUNT(cubData) double *pData,
                                               uint32 cubData) {
  return 0;
}

bool FakeSteamUserStats::GetAchievementProgressLimits(const char *pchName, int32 *pnMinProgress, int32 *pnMaxProgress) {
  return true;
}

bool FakeSteamUserStats::GetAchievementProgressLimits(const char *pchName, float *pfMinProgress, float *pfMaxProgress) {
  return true;
}
