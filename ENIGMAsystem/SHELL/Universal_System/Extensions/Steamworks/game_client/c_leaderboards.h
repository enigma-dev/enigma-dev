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

#define INVALID_LEADERBOARD 0

namespace steamworks {

class c_leaderboards_cookies;
class c_game_client;

class c_leaderboards {
 public:
  c_leaderboards();
  ~c_leaderboards();

  bool create_leaderboard(const int id, const std::string& leaderboard_name,
                          const ELeaderboardSortMethod leaderboard_sort_method = k_ELeaderboardSortMethodNone,
                          const ELeaderboardDisplayType leaderboard_display_type = k_ELeaderboardDisplayTypeNone);

  void find_leaderboard(const int id, const std::string& leaderboard_name);

  /*
    Uploads a user score to a specified leaderboard. Uploading scores to Steam is rate limited to 10 uploads 
    per 10 minutes and you may only have one outstanding call to this function at a time. Calls UploadLeaderboardScore.
    Check https://partner.steamgames.com/doc/api/ISteamUserStats#UploadLeaderboardScore for more information.
  */
  bool upload_score(
      const int id, const int score,
      const ELeaderboardUploadScoreMethod leaderboard_upload_score_method = k_ELeaderboardUploadScoreMethodNone);

  bool download_scores(const int id, const ELeaderboardDataRequest leaderboard_data_request, const int range_start = -1,
                       const int range_end = -1);

  void set_current_leaderboard(const SteamLeaderboard_t leaderboard);
  void set_loading(const bool loading);

  static std::string get_leaderboard_name(const SteamLeaderboard_t leaderboard);

 private:
  SteamLeaderboard_t current_leaderboard_;

  unsigned number_of_leaderboard_entries_;

  bool loading_;

  // TODO: Make sure that this vector doesn't cause race conditions.
  /*
    This vector is used to track all instances of cookies so that we can destroy them when they are done.
  */
  std::vector<c_leaderboards_cookies*> c_leaderboards_cookies_;

  void deallocate_all_c_leaderboards_cookies();

  void deallocate_c_leaderboards_cookies_if_done();
};
}  // namespace steamworks

#endif  // GAMECLIENT_LEADERBOARDS_H
