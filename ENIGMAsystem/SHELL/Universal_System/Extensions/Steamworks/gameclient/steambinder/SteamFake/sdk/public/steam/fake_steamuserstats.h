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

#ifndef FAKE_STEAMUSERSTATS_H
#define FAKE_STEAMUSERSTATS_H

#include "fake_steam_api.h"

class FakeSteamUserStats : public ISteamUserStats {
 public:
  static FakeSteamUserStats *GetInstance() {
    static FakeSteamUserStats instance;
    return &instance;
  }

  void RegisterUserStatsReceivedCallback(class CCallbackBase *pCallback);
  void RegisterUserStatsStoredCallback(class CCallbackBase *pCallback);
  void RegisterUserAchievementStoredCallback(class CCallbackBase *pCallback);
  // void RegisterLeaderboardFindResultCallresult(class CCallbackBase *pCallback, SteamAPICall_t hAPICall);
  // void RegisterLeaderboardScoresDownloadedCallresult(class CCallbackBase *pCallback, SteamAPICall_t hAPICall);
  // void RegisterLeaderboardScoreUploadedCallresult(class CCallbackBase *pCallback, SteamAPICall_t hAPICall);

  bool RequestCurrentStats() override;

  bool GetStat(const char *pchName, int32 *pData) override;

  bool GetStat(const char *pchName, float *pData) override;

  bool SetStat(const char *pchName, int32 nData) override;

  bool SetStat(const char *pchName, float fData) override;

  bool UpdateAvgRateStat(const char *pchName, float flCountThisSession, double dSessionLength) override;

  bool GetAchievement(const char *pchName, bool *pbAchieved) override;
  bool SetAchievement(const char *pchName) override;
  bool ClearAchievement(const char *pchName) override;

  bool GetAchievementAndUnlockTime(const char *pchName, bool *pbAchieved, uint32 *punUnlockTime) override;

  bool StoreStats() override;

  int GetAchievementIcon(const char *pchName) override;

  const char *GetAchievementDisplayAttribute(const char *pchName, const char *pchKey) override;

  bool IndicateAchievementProgress(const char *pchName, uint32 nCurProgress, uint32 nMaxProgress) override;

  uint32 GetNumAchievements() override;

  const char *GetAchievementName(uint32 iAchievement) override;

  SteamAPICall_t RequestUserStats(CSteamID steamIDUser) override;

  bool GetUserStat(CSteamID steamIDUser, const char *pchName, int32 *pData) override;

  bool GetUserStat(CSteamID steamIDUser, const char *pchName, float *pData) override;

  bool GetUserAchievement(CSteamID steamIDUser, const char *pchName, bool *pbAchieved) override;

  bool GetUserAchievementAndUnlockTime(CSteamID steamIDUser, const char *pchName, bool *pbAchieved,
                                       uint32 *punUnlockTime) override;

  bool ResetAllStats(bool bAchievementsToo) override;

  SteamAPICall_t FindOrCreateLeaderboard(const char *pchLeaderboardName, ELeaderboardSortMethod eLeaderboardSortMethod,
                                         ELeaderboardDisplayType eLeaderboardDisplayType) override;

  SteamAPICall_t FindLeaderboard(const char *pchLeaderboardName) override;

  const char *GetLeaderboardName(SteamLeaderboard_t hSteamLeaderboard) override;

  int GetLeaderboardEntryCount(SteamLeaderboard_t hSteamLeaderboard) override;

  ELeaderboardSortMethod GetLeaderboardSortMethod(SteamLeaderboard_t hSteamLeaderboard) override;

  ELeaderboardDisplayType GetLeaderboardDisplayType(SteamLeaderboard_t hSteamLeaderboard) override;

  SteamAPICall_t DownloadLeaderboardEntries(SteamLeaderboard_t hSteamLeaderboard,
                                            ELeaderboardDataRequest eLeaderboardDataRequest, int nRangeStart,
                                            int nRangeEnd) override;

  SteamAPICall_t DownloadLeaderboardEntriesForUsers(SteamLeaderboard_t hSteamLeaderboard,
                                                    STEAM_ARRAY_COUNT_D(cUsers, Array of users to retrieve)
                                                        CSteamID *prgUsers,
                                                    int cUsers) override;

  bool GetDownloadedLeaderboardEntry(SteamLeaderboardEntries_t hSteamLeaderboardEntries, int index,
                                     LeaderboardEntry_t *pLeaderboardEntry, int32 *pDetails, int cDetailsMax) override;

  SteamAPICall_t UploadLeaderboardScore(SteamLeaderboard_t hSteamLeaderboard,
                                        ELeaderboardUploadScoreMethod eLeaderboardUploadScoreMethod, int32 nScore,
                                        const int32 *pScoreDetails, int cScoreDetailsCount) override;

  SteamAPICall_t AttachLeaderboardUGC(SteamLeaderboard_t hSteamLeaderboard, UGCHandle_t hUGC) override;

  SteamAPICall_t GetNumberOfCurrentPlayers() override;

  SteamAPICall_t RequestGlobalAchievementPercentages() override;

  int GetMostAchievedAchievementInfo(char *pchName, uint32 unNameBufLen, float *pflPercent, bool *pbAchieved) override;

  int GetNextMostAchievedAchievementInfo(int iIteratorPrevious, char *pchName, uint32 unNameBufLen, float *pflPercent,
                                         bool *pbAchieved) override;

  bool GetAchievementAchievedPercent(const char *pchName, float *pflPercent) override;

  SteamAPICall_t RequestGlobalStats(int nHistoryDays) override;

  bool GetGlobalStat(const char *pchStatName, int64 *pData) override;

  bool GetGlobalStat(const char *pchStatName, double *pData) override;

  int32 GetGlobalStatHistory(const char *pchStatName, STEAM_ARRAY_COUNT(cubData) int64 *pData, uint32 cubData) override;

  int32 GetGlobalStatHistory(const char *pchStatName, STEAM_ARRAY_COUNT(cubData) double *pData,
                             uint32 cubData) override;

  bool GetAchievementProgressLimits(const char *pchName, int32 *pnMinProgress, int32 *pnMaxProgress) override;

  bool GetAchievementProgressLimits(const char *pchName, float *pfMinProgress, float *pfMaxProgress) override;

 private:
  FakeSteamUserStats() {}
  ~FakeSteamUserStats() {}

  // Callbacks
  class CCallbackBase *pCallbackUserStatsReceived{nullptr};
  class CCallbackBase *pCallbackUserStatsStored{nullptr};
  class CCallbackBase *pCallbackUserAchievementStored{nullptr};

  // Callresults
  // class CCallbackBase *pCallresultLeaderboardFindResult{nullptr};
  // SteamAPICall_t hAPICallLeaderboardFindResult{0};

  // class CCallbackBase *pCallresultLeaderboardScoresDownloaded{nullptr};
  // SteamAPICall_t hAPICallLeaderboardScoresDownloaded{0};

  // class CCallbackBase *pCallresultLeaderboardScoreUploaded{nullptr};
  // SteamAPICall_t hAPICallLeaderboardScoreUploaded{0};

  FakeSteamUserStats(FakeSteamUserStats const &) = delete;
  void operator=(FakeSteamUserStats const &) = delete;
};

#endif  // FAKE_STEAMUSERSTATS_H
