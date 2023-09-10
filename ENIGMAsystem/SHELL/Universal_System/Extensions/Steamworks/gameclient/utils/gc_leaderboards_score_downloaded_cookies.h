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

/*
  Whenever a gc_leaderboards::download_scores() is called, a new object from this class will be instantiated 
  with a unique id. This id will be used when the call result arrive. The call result setting operation is 
  moved to this class from gc_leaderboards class, which means in order to setup a call result, you need to 
  instantiate an onject from this class.
*/

#ifndef GC_LEADERBOARDS_SCORE_DOWNLOADED_COOKIES_H
#define GC_LEADERBOARDS_SCORE_DOWNLOADED_COOKIES_H

// TODO: This documentation need to be improved when uploading a game to Steam Store.

#include "gc_leaderboards_cookies.h"

#ifdef ENIGMA_STEAMWORKS_API_MOCK
typedef LeaderboardScoresDownloadedMock LeaderboardScoresDownloaded_t;
#endif  // ENIGMA_STEAMWORKS_API_MOCK

namespace steamworks_gc {

class GCLeaderboards;

class GCLeaderboardsScoreDownloadedCookies : public GCLeaderboardsCookies {
 public:
  GCLeaderboardsScoreDownloadedCookies(int id_, GCLeaderboards* gc_leaderboards, SteamAPICall_t steam_api_call);
  ~GCLeaderboardsScoreDownloadedCookies() = default;

 private:
  /*
      This id is part of the payload that will be attached to the leaderboard find results. This
      id must be returned from AssetArray add() function.
 */
  int id_;

  GCLeaderboards* gc_leaderboards_;

  /*
    This flag will be set to true when the call result is received. This flag will be used to
    determine if we can destroy this object or not.
*/
  bool is_done_;

  bool is_done() const override;

  void set_call_result(SteamAPICall_t steam_api_call) override;

  // Called when SteamUserStats()->DownloadLeaderboardEntries() returns asynchronously
  void on_download_scores(LeaderboardScoresDownloaded_t* pFindLearderboardResult, bool bIOFailure);
#ifndef ENIGMA_STEAMWORKS_API_MOCK
  CCallResult<GCLeaderboardsScoreDownloadedCookies, LeaderboardScoresDownloaded_t> m_SteamCallResultDownloadScores;
#endif  // ENIGMA_STEAMWORKS_API_MOCK
};

}  // namespace steamworks_gc

#endif  // GC_LEADERBOARDS_SCORE_DOWNLOADED_COOKIES_H
