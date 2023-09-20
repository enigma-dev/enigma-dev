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

bool FakeSteamUserStats::RequestCurrentStats() { return true; }

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

bool FakeSteamUserStats::StoreStats() { return true; }

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
  return 1;  // Return a valid SteamAPICall_t
}

SteamAPICall_t FakeSteamUserStats::FindLeaderboard(const char *pchLeaderboardName) {
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
  CSteamID steamID;
  pLeaderboardEntry->m_steamIDUser = steamID;
  pLeaderboardEntry->m_nGlobalRank = 0;
  pLeaderboardEntry->m_nScore = 0;
  pLeaderboardEntry->m_cDetails = 0;
  return true;
}

SteamAPICall_t FakeSteamUserStats::UploadLeaderboardScore(SteamLeaderboard_t hSteamLeaderboard,
                                                          ELeaderboardUploadScoreMethod eLeaderboardUploadScoreMethod,
                                                          int32 nScore, const int32 *pScoreDetails,
                                                          int cScoreDetailsCount) {
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
