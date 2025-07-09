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

/**
 * @brief Steam supports persistent leaderboards with automatically ordered entries. These 
 *        leaderboards can be used to display global and friend leaderboards in your game 
 *        and on your community webpage. Each Steamworks title can create up to 10,000 leaderboards, 
 *        and each leaderboard can be retrieved immediately after a player's score has been inserted.
 * 
 * @note Check https://partner.steamgames.com/doc/features/leaderboards for more information.
 * 
 */

#ifndef GC_LEADERBOARDS_H
#define GC_LEADERBOARDS_H
#pragma once

#include "gameclient.h"

#define INVALID_LEADERBOARD 0
#define INVALID_LEADERBOARD_ENTRIES 0

namespace steamworks_gc {

class GCLeaderboardsCookies;
class GameClient;

class GCLeaderboards {
 public:
  // GCLeaderboards constructor.
  GCLeaderboards();

  // GCLeaderboards destructor.
  ~GCLeaderboards();

  /**
   * @brief Create a leaderboard object
   * 
   * @param id the id that will be attached to the callresult. This id is the output of
   *           AssetArray @c add() function.
   * 
   * @param leaderboard_name the name of the leaderboard.
   * @param leaderboard_sort_method the sort method of the leaderboard.
   * @param leaderboard_display_type the display type of the leaderboard.
   * @return true If the leaderboard is successfully created.
   * @return false If the leaderboard is not successfully created.
   */
  bool create_leaderboard(
      const int& id, const std::string& leaderboard_name,
      const ELeaderboardSortMethod& leaderboard_sort_method = ELeaderboardSortMethod::k_ELeaderboardSortMethodNone,
      const ELeaderboardDisplayType& leaderboard_display_type = ELeaderboardDisplayType::k_ELeaderboardDisplayTypeNone);

  /**
   * @brief 
   * 
   * @param id the id that will be attached to the callresult. This id is the output of
   *           AssetArray @c add() function.
   * 
   * @param leaderboard_name the name of the leaderboard.
   */
  void find_leaderboard(const int& id, const std::string& leaderboard_name);

  /**
   * @brief Uploads a user score to a specified leaderboard.
   * 
   * @note Uploading scores to Steam is rate limited to 10 uploads per 10 minutes and you may only 
   *       have one outstanding call to this function at a time.
   * 
   * @note Check https://partner.steamgames.com/doc/api/ISteamUserStats#UploadLeaderboardScore for 
   *       more information.
   * 
   * @param id The id that will be attached to the callresult.
   * @param score The score that will be uploaded.
   * @param leaderboard_upload_score_method 
   * @return true If the score is successfully uploaded.
   * @return false If the score is not successfully uploaded.
   */
  bool upload_score(const int& id, const int& score,
                    const ELeaderboardUploadScoreMethod& leaderboard_upload_score_method =
                        ELeaderboardUploadScoreMethod::k_ELeaderboardUploadScoreMethodNone);

  /**
   * @brief 
   * 
   * @param id The id that will be attached to the callresult.
   * @param leaderboard_data_request The leaderboard data request.
   * @param range_start The range start.
   * @param range_end The range end.
   * @return true If the scores are successfully downloaded.
   * @return false If the scores are not successfully downloaded.
   */
  bool download_scores(const int& id,
                       const ELeaderboardDataRequest& leaderboard_data_request =
                           ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobal,
                       const int& range_start = -1, const int& range_end = -1);

  /**
   * @brief Set the current leaderboard object
   * 
   * @param leaderboard If the leaderboard is successfully set.
   */
  void set_current_leaderboard(const SteamLeaderboard_t& leaderboard);

  /**
   * @brief Set the loading object
   * 
   * @param loading If the leaderboard is loading.
   */
  void set_loading(const bool& loading);

  /**
   * @brief Get the leaderboard name object
   * 
   * @param buffer The buffer that will be used to store the leaderboard name.
   * @param leaderboard The leaderboard that will be used to retrieve the leaderboard name.
   * @return true If the leaderboard name is successfully retrieved.
   * @return false If the leaderboard name is not successfully retrieved.
   */
  static bool get_leaderboard_name(std::string& buffer, const SteamLeaderboard_t& leaderboard);

 private:
  SteamLeaderboard_t current_leaderboard_;

  unsigned number_of_leaderboard_entries_;

  bool loading_;

  /**
  * @brief This vector is used to track all instances of cookies so that we can destroy them when 
  *        they are done.
  * 
  * @todo Make sure that this vector doesn't cause race conditions.
  * 
  */
  std::vector<GCLeaderboardsCookies*> gc_leaderboards_cookies_;

  /**
  * @brief Deallocates all leaderboards cookies.
  * 
  */
  void deallocate_all_leaderboards_cookies();

  /**
   * @brief Deallocates leaderboards cookies if done.
   * 
   */
  void deallocate_leaderboards_cookies_if_done();
};

}  // namespace steamworks_gc

#endif  // GC_LEADERBOARDS_H
