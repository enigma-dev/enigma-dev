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

#ifndef MOCK_STEAMUSERSTATS_H
#define MOCK_STEAMUSERSTATS_H

#include "mock_isteamuserstats.h"

class SteamUserStatsAccessor : public ISteamUserStats {
 public:
  static SteamUserStatsAccessor &getInstance() {
    static SteamUserStatsAccessor instance;
    return instance;
  }

  //   SteamUserStatsAccessor(SteamUserStatsAccessor const &) = delete;
  //   void operator=(SteamUserStatsAccessor const &) = delete;

  bool RequestCurrentStats() override;

  bool GetStat(const char *pchName, int32 *pData) override;

  bool GetStat(const char *pchName, float *pData) override;

  bool SetStat(const char *pchName, int32 nData) override;

  bool SetStat(const char *pchName, float fData) override;

  bool UpdateAvgRateStat(const char *pchName, float flCountThisSession, double dSessionLength) override;

  bool GetAchievement(const char *pchName, bool *pbAchieved) override;
  bool SetAchievement(const char *pchName) override;
  bool ClearAchievement(const char *pchName) override;

  bool StoreStats() override;

  const char *GetAchievementName(uint32 iAchievement) override;

  bool ResetAllStats(bool bAchievementsToo) override;

  SteamAPICall_t FindOrCreateLeaderboard(const char *pchLeaderboardName, ELeaderboardSortMethod eLeaderboardSortMethod,
                                         ELeaderboardDisplayType eLeaderboardDisplayType) override;

  SteamAPICall_t FindLeaderboard(const char *pchLeaderboardName) override;

  const char *GetLeaderboardName(SteamLeaderboard_t hSteamLeaderboard) override;

  SteamAPICall_t DownloadLeaderboardEntries(SteamLeaderboard_t hSteamLeaderboard,
                                            ELeaderboardDataRequest eLeaderboardDataRequest, int nRangeStart,
                                            int nRangeEnd) override;

  bool GetDownloadedLeaderboardEntry(SteamLeaderboardEntries_t hSteamLeaderboardEntries, int index,
                                     LeaderboardEntry_t *pLeaderboardEntry, int32 *pDetails, int cDetailsMax) override;

  SteamAPICall_t UploadLeaderboardScore(SteamLeaderboard_t hSteamLeaderboard,
                                        ELeaderboardUploadScoreMethod eLeaderboardUploadScoreMethod, int32 nScore,
                                        const int32 *pScoreDetails, int cScoreDetailsCount) override;

 private:
  SteamUserStatsAccessor() {}
  ~SteamUserStatsAccessor() = default;

  SteamUserStatsAccessor(SteamUserStatsAccessor const &);
  void operator=(SteamUserStatsAccessor const &);
};

inline ISteamUserStats *SteamUserStats() { return &SteamUserStatsAccessor::getInstance(); }

#endif  // MOCK_STEAMUSERSTATS_H
