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
  Steam supports persistent leaderboards with automatically ordered entries. These leaderboards can 
  be used to display global and friend leaderboards in your game and on your community webpage. Each 
  Steamworks title can create up to 10,000 leaderboards, and each leaderboard can be retrieved immediately 
  after a player's score has been inserted.
  Check https://partner.steamgames.com/doc/features/leaderboards for more information.
*/

#ifndef GAMECLIENT_LEADERBOARDS_H
#define GAMECLIENT_LEADERBOARDS_H

// TODO: This documentation need to be improved when uploading a game to Steam Store.

#include "game_client.h"

namespace steamworks {

class c_game_client;

class c_leaderboards {
 public:
  c_leaderboards();
  ~c_leaderboards() = default;

  void find_leaderboard(const std::string& leaderboard_name, const ELeaderboardSortMethod leaderboard_sort_method,
                        const ELeaderboardDisplayType leaderboard_display_type);
  bool upload_score(const int score, const ELeaderboardUploadScoreMethod leaderboard_upload_score_method =
                                         k_ELeaderboardUploadScoreMethodNone);

  int download_scores(const ELeaderboardDataRequest leaderboard_data_request, const int range_start = -1,
                       const int range_end = -1);

 private:
  SteamLeaderboard_t current_leaderboard_;
  bool loading_;
  unsigned number_of_leaderboard_entries_;
  
  int last_score_downloaded_id_;

  // Called when SteamUserStats()->FindOrCreateLeaderboard() returns asynchronously
  void on_find_leaderboard(LeaderboardFindResult_t* pFindLearderboardResult, bool bIOFailure);
  CCallResult<c_leaderboards, LeaderboardFindResult_t> m_callResultFindLeaderboard;

  // Called when SteamUserStats()->UploadLeaderboardScore() returns asynchronously
  void on_upload_score(LeaderboardScoreUploaded_t* pFindLearderboardResult, bool bIOFailure);
  CCallResult<c_leaderboards, LeaderboardScoreUploaded_t> m_SteamCallResultUploadScore;

  // Called when SteamUserStats()->DownloadLeaderboardEntries() returns asynchronously
  void on_download_scores(LeaderboardScoresDownloaded_t* pFindLearderboardResult, bool bIOFailure);
  CCallResult<c_leaderboards, LeaderboardScoresDownloaded_t> m_SteamCallResultDownloadScores;
};
}  // namespace steamworks

#endif  // GAMECLIENT_LEADERBOARDS_H
