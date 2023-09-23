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

/*
  Steam supports persistent leaderboards with automatically ordered entries. These leaderboards can 
  be used to display global and friend leaderboards in your game and on your community webpage. Each 
  Steamworks title can create up to 10,000 leaderboards, and each leaderboard can be retrieved immediately 
  after a player's score has been inserted.
  Check https://partner.steamgames.com/doc/features/leaderboards for more information.
*/

#ifndef GC_LEADERBOARDS_H
#define GC_LEADERBOARDS_H
#pragma once

// TODO: This documentation need to be improved when uploading a game to Steam Store.

#include "gameclient.h"

#define INVALID_LEADERBOARD 0
#define INVALID_LEADERBOARD_ENTRIES 0

namespace steamworks_gc {

/**
 * @brief This enum is created to prevent usage of API's types in the extension.
 * 
 */
enum GCLeaderboardSortMethod {
  k_GCLeaderboardSortMethod_None = 0,
  k_GCLeaderboardSortMethod_Ascending = 1,
  k_GCLeaderboardSortMethod_Descending = 2
};

/**
 * @brief This enum is created to prevent usage of API's types in the extension.
 * 
 */
enum GCLeaderboardDisplayType {
  k_GCLeaderboardDisplayType_None = 0,
  k_GCLeaderboardDisplayType_Numeric = 1,
  k_GCLeaderboardDisplayType_TimeSeconds = 2,
  k_GCLeaderboardDisplayType_TimeMilliSeconds = 3
};

/**
 * @brief This enum is created to prevent usage of API's types in the extension.
 * 
 */
enum GCLeaderboardUploadScoreMethod {
  k_GCLeaderboardUploadScoreMethod_None = 0,
  k_GCLeaderboardUploadScoreMethod_KeepBest = 1,
  k_GCLeaderboardUploadScoreMethod_ForceUpdate = 2,
};

/**
 * @brief This enum is created to prevent usage of API's types in the extension.
 * 
 */
enum GCLeaderboardDataRequest {
  k_GCLeaderboardDataRequest_Global = 0,
  k_GCLeaderboardDataRequest_GlobalAroundUser = 1,
  k_GCLeaderboardDataRequest_Friends = 2,
  k_GCLeaderboardDataRequest_Users = 3
};

class GCLeaderboardsCookies;
class GameClient;

class GCLeaderboards {
 public:
  GCLeaderboards();
  ~GCLeaderboards();

  bool create_leaderboard(const int& id, const std::string& leaderboard_name,
                          const GCLeaderboardSortMethod& gc_leaderboard_sort_method =
                              GCLeaderboardSortMethod::k_GCLeaderboardSortMethod_None,
                          const GCLeaderboardDisplayType& gc_leaderboard_display_type =
                              GCLeaderboardDisplayType::k_GCLeaderboardDisplayType_None);

  void find_leaderboard(const int& id, const std::string& leaderboard_name);

  /*
    Uploads a user score to a specified leaderboard. Uploading scores to Steam is rate limited to 10 uploads 
    per 10 minutes and you may only have one outstanding call to this function at a time. Calls UploadLeaderboardScore.
    Check https://partner.steamgames.com/doc/api/ISteamUserStats#UploadLeaderboardScore for more information.
  */
  bool upload_score(const int& id, const int& score,
                    const GCLeaderboardUploadScoreMethod& gc_leaderboard_upload_score_method =
                        GCLeaderboardUploadScoreMethod::k_GCLeaderboardUploadScoreMethod_None);

  bool download_scores(const int& id,
                       const GCLeaderboardDataRequest& gc_leaderboard_data_request =
                           GCLeaderboardDataRequest::k_GCLeaderboardDataRequest_Global,
                       const int& range_start = -1, const int& range_end = -1);

  void set_current_leaderboard(const SteamLeaderboard_t& leaderboard);
  void set_loading(const bool& loading);

  static bool get_leaderboard_name(std::string& buffer, const SteamLeaderboard_t& leaderboard);

 private:
  SteamLeaderboard_t current_leaderboard_;

  unsigned number_of_leaderboard_entries_;

  bool loading_;

  // TODO: Make sure that this vector doesn't cause race conditions.
  /*
    This vector is used to track all instances of cookies so that we can destroy them when they are done.
  */
  std::vector<GCLeaderboardsCookies*> gc_leaderboards_cookies_;

  void deallocate_all_leaderboards_cookies();

  void deallocate_leaderboards_cookies_if_done();
};

/**
 * @brief This struct is created to prevent usage of API's types in the extension.
 * 
 */
struct GCLeaderboardFindResult {
  SteamLeaderboard_t leaderboard;
  bool leaderboard_found;
};

/**
 * @brief This struct is created to prevent usage of API's types in the extension.
 * 
 */
struct GCLeaderboardScoresDownloadedResult {
  SteamLeaderboard_t leaderboard;
  std::string entries_buffer;
  unsigned number_of_leaderboard_entries;
};

/**
 * @brief This struct is created to prevent usage of API's types in the extension.
 * 
 */
struct GCLeaderboardScoreUploadedResult {
  bool success;
  SteamLeaderboard_t leaderboard;
  int32 score;
  bool score_changed;
  unsigned global_rank_new;
  unsigned global_rank_previous;
};

}  // namespace steamworks_gc

#endif  // GC_LEADERBOARDS_H
