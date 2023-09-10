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

#include "mock_steamuserstatsaccessor.h"

bool SteamUserStatsAccessor::RequestCurrentStats() { return true; }

bool SteamUserStatsAccessor::GetStat(const char *pchName, int32 *pData) { return true; }

bool SteamUserStatsAccessor::GetStat(const char *pchName, float *pData) { return true; }

bool SteamUserStatsAccessor::SetStat(const char *pchName, int32 nData) { return true; }

bool SteamUserStatsAccessor::SetStat(const char *pchName, float fData) { return true; }

bool SteamUserStatsAccessor::UpdateAvgRateStat(const char *pchName, float flCountThisSession, double dSessionLength) {
  return true;
}

bool SteamUserStatsAccessor::GetAchievement(const char *pchName, bool *pbAchieved) {
  *pbAchieved = true;
  return true;
}
bool SteamUserStatsAccessor::SetAchievement(const char *pchName) { return true; }
bool SteamUserStatsAccessor::ClearAchievement(const char *pchName) { return true; }

bool SteamUserStatsAccessor::StoreStats() { return true; }

const char *SteamUserStatsAccessor::GetAchievementName(uint32 iAchievement) { return "MockAchievement"; }

bool SteamUserStatsAccessor::ResetAllStats(bool bAchievementsToo) { return true; }

SteamAPICall_t SteamUserStatsAccessor::FindOrCreateLeaderboard(const char *pchLeaderboardName,
                                                               ELeaderboardSortMethod eLeaderboardSortMethod,
                                                               ELeaderboardDisplayType eLeaderboardDisplayType) {
  return 1;  // Return a valid SteamAPICall_t
}

SteamAPICall_t SteamUserStatsAccessor::FindLeaderboard(const char *pchLeaderboardName) {
  return 1;  // // Return a valid SteamAPICall_t
}

const char *SteamUserStatsAccessor::GetLeaderboardName(SteamLeaderboard_t hSteamLeaderboard) {
  return "MockLeaderboard";
}

SteamAPICall_t SteamUserStatsAccessor::DownloadLeaderboardEntries(SteamLeaderboard_t hSteamLeaderboard,
                                                                  ELeaderboardDataRequest eLeaderboardDataRequest,
                                                                  int nRangeStart, int nRangeEnd) {
  return 1;  // Return a valid SteamAPICall_t
}

bool SteamUserStatsAccessor::GetDownloadedLeaderboardEntry(SteamLeaderboardEntries_t hSteamLeaderboardEntries,
                                                           int index, LeaderboardEntry_t *pLeaderboardEntry,
                                                           int32 *pDetails, int cDetailsMax) {
  pLeaderboardEntry->m_steamIDUser = 0;
  pLeaderboardEntry->m_nGlobalRank = 0;
  pLeaderboardEntry->m_nScore = 0;
  pLeaderboardEntry->m_cDetails = 0;
  return true;
};

SteamAPICall_t SteamUserStatsAccessor::UploadLeaderboardScore(
    SteamLeaderboard_t hSteamLeaderboard, ELeaderboardUploadScoreMethod eLeaderboardUploadScoreMethod, int32 nScore,
    const int32 *pScoreDetails, int cScoreDetailsCount) {
  return 1;  // Return a valid SteamAPICall_t
}
