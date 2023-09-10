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

#ifndef MOCK_ISTEAMUSERSTATS_H
#define MOCK_ISTEAMUSERSTATS_H

#include "mock_steam_api_common.h"

enum { k_StatNameMaxLength = 128 };

class ISteamUserStats {
 public:
  virtual bool RequestCurrentStats() = 0;

  virtual bool GetStat(const char *pchName, int32 *pData) = 0;

  virtual bool GetStat(const char *pchName, float *pData) = 0;

  virtual bool SetStat(const char *pchName, int32 nData) = 0;

  virtual bool SetStat(const char *pchName, float fData) = 0;

  virtual bool UpdateAvgRateStat(const char *pchName, float flCountThisSession, double dSessionLength) = 0;

  virtual bool GetAchievement(const char *pchName, bool *pbAchieved) = 0;
  virtual bool SetAchievement(const char *pchName) = 0;
  virtual bool ClearAchievement(const char *pchName) = 0;

  virtual bool StoreStats() = 0;

  virtual const char *GetAchievementName(uint32 iAchievement) = 0;

  virtual bool ResetAllStats(bool bAchievementsToo) = 0;

  virtual SteamAPICall_t FindOrCreateLeaderboard(const char *pchLeaderboardName,
                                                 ELeaderboardSortMethod eLeaderboardSortMethod,
                                                 ELeaderboardDisplayType eLeaderboardDisplayType) = 0;

  virtual SteamAPICall_t FindLeaderboard(const char *pchLeaderboardName) = 0;

  virtual const char *GetLeaderboardName(SteamLeaderboard_t hSteamLeaderboard) = 0;

  virtual ELeaderboardSortMethod GetLeaderboardSortMethod(SteamLeaderboard_t hSteamLeaderboard) = 0;

  virtual ELeaderboardDisplayType GetLeaderboardDisplayType(SteamLeaderboard_t hSteamLeaderboard) = 0;

  virtual SteamAPICall_t DownloadLeaderboardEntries(SteamLeaderboard_t hSteamLeaderboard,
                                                    ELeaderboardDataRequest eLeaderboardDataRequest, int nRangeStart,
                                                    int nRangeEnd) = 0;

  virtual SteamAPICall_t UploadLeaderboardScore(SteamLeaderboard_t hSteamLeaderboard,
                                                ELeaderboardUploadScoreMethod eLeaderboardUploadScoreMethod,
                                                int32 nScore, const int32 *pScoreDetails, int cScoreDetailsCount) = 0;
};

struct LeaderboardFindResultMock {
  SteamLeaderboard_t m_hSteamLeaderboard;
  uint8 m_bLeaderboardFound;
};

struct LeaderboardScoresDownloadedMock {
  SteamLeaderboard_t m_hSteamLeaderboard;
  SteamLeaderboardEntries_t m_hSteamLeaderboardEntries;
  int m_cEntryCount;
};

struct LeaderboardScoreUploadedMock {
  uint8 m_bSuccess;
  SteamLeaderboard_t m_hSteamLeaderboard;
  int32 m_nScore;
  uint8 m_bScoreChanged;
  int m_nGlobalRankNew;
  int m_nGlobalRankPrevious;
};

struct UserStatsReceivedMock {
  uint64 m_nGameID;
  CSteamID m_steamIDUser;
};

struct UserStatsStoredMock {
  uint64 m_nGameID;
};

struct UserAchievementStoredMock {
  uint64 m_nGameID;
  bool m_bGroupAchievement;
  char m_rgchAchievementName[k_StatNameMaxLength];
  uint32 m_nCurProgress;
  uint32 m_nMaxProgress;
};

#endif  // MOCK_ISTEAMUSERSTATS_H
