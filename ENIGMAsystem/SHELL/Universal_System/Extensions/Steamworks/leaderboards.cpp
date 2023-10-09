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
 * @todo Use the find_leaderboard() function return id inside upload and download 
 *       functions.
 * 
 */

#include "leaderboards.h"

#include "Universal_System/../Platforms/General/PFmain.h"

bool leaderboards_pre_checks(const std::string& script_name) {
  if (!steamworks_gc::GCMain::is_initialised()) {
    DEBUG_MESSAGE("Calling " + script_name + " failed. Make sure that the API is initialized correctly.", M_ERROR);
    return false;
  }

  if (!steamworks_gc::GameClient::is_user_logged_on()) {
    DEBUG_MESSAGE("Calling " + script_name + " failed. Make sure that the user is logged in.", M_ERROR);
    return false;
  }

  return true;
}

namespace enigma {

unsigned number_of_successful_upload_requests{0};

bool upload_rate_limit_exceeded{false};

AssetArray<LeaderboardFindResult_t*> leaderboards_array;

AssetArray<LeaderboardScoresDownloaded_t*> entries_array;

AssetArray<LeaderboardScoreUploaded_t*> scores_array;

void push_create_leaderboard_steam_async_event(const int& id, const LeaderboardFindResult_t& leaderboard_find_result) {
  std::string leaderboard_name_buffer;

  if (!steamworks_gc::GCLeaderboards::get_leaderboard_name(leaderboard_name_buffer,
                                                           leaderboard_find_result.m_hSteamLeaderboard)) {
    return;
  }

  const std::map<std::string, variant> leaderboard_find_event = {
      {"id", id},
      {"event_type", "create_leaderboard"},
      {"status", leaderboard_find_result.m_bLeaderboardFound},
      {"lb_name", leaderboard_name_buffer}};

  posted_async_events_mutex.lock();
  enigma::posted_async_events.push(leaderboard_find_event);
  posted_async_events_mutex.unlock();
}

void push_leaderboard_upload_steam_async_event(const int& id,
                                               const LeaderboardScoreUploaded_t& leaderboard_score_uploaded_result) {
  // We have a successful upload request after a failed one. Let's reset the number of successful upload requests.
  if (enigma::upload_rate_limit_exceeded) {
    enigma::upload_rate_limit_exceeded = false;
    enigma::number_of_successful_upload_requests = 0;
  }
  enigma::number_of_successful_upload_requests++;  // Increase the number of successful upload requests.

  std::string leaderboard_name_buffer;

  if (!steamworks_gc::GCLeaderboards::get_leaderboard_name(leaderboard_name_buffer,
                                                           leaderboard_score_uploaded_result.m_hSteamLeaderboard)) {
    return;
  }

  /*
      Steam ASYNC: {
        "updated" : 1.0, 
        "lb_name" : "YYLeaderboard_10/29/21--",
        "success" : 1.0,
        "event_type" : "leaderboard_upload",
        "score" : 325.0,
        "post_id" : 6.0
      }
  */
  const std::map<std::string, variant> leaderboard_upload_event = {
      {"event_type", "leaderboard_upload"},
      {"post_id", id},
      {"lb_name", leaderboard_name_buffer},
      {"success", leaderboard_score_uploaded_result.m_bSuccess},
      {"updated", leaderboard_score_uploaded_result.m_bScoreChanged},
      {"score", leaderboard_score_uploaded_result.m_nScore}};

  posted_async_events_mutex.lock();
  enigma::posted_async_events.push(leaderboard_upload_event);
  posted_async_events_mutex.unlock();
}

void push_leaderboard_download_steam_async_event(
    const int& id, const std::string& entries_buffer,
    const LeaderboardScoresDownloaded_t& leaderboard_scores_downloaded_result) {
  std::string leaderboard_name_buffer;

  if (!steamworks_gc::GCLeaderboards::get_leaderboard_name(leaderboard_name_buffer,
                                                           leaderboard_scores_downloaded_result.m_hSteamLeaderboard)) {
    return;
  }
  /*
      Steam ASYNC: {
        "entries" : "{\n    "entries": [\n                { "name"  : "TomasJPereyra", "score" : 1, "rank"  : 1, "userID": "@i64@110000108ae8556$i64$" }
                    ,\n                { "name"  : "Scott-ish", "score" : 10, "rank"  : 2, "userID": "@i64@11000010241f4ea$i64$" },\n                { "name"  : "Loyal RaveN", 
                    "score" : 63, "rank"  : 3, "userID": "@i64@11000015c558396$i64$" },\n                { "name"  : "luca.muehlbauer", "score" : 100, "rank"  : 4, "userID": 
                    "@i64@11000015d44ce04$i64$", "data"  : "UWx1Y2EubXVlaGxiYXVlciB3YXMgaGVyZSA6KQAAAAA=" },\n                { "name"  : "simon", "score" : 113, "rank"  : 5, 
                    "userID": "@i64@1100001040447b6$i64$" },\n                { "name"  : "meFroggy", "score" : 138, "rank"  : 6, "userID": "@i64@110000117c9d62b$i64$", "data"
                      : "HG1lRnJvZ2d5IHdhcyBoZXJlIDopAAAA" },\n                { "name"  : "12349184732146127845127895", "score" : 155, "rank"  : 7, "userID": "@i64@11000010be
                    342ce$i64$", "data"  : "FEZlZWxpbmcgU2lja25lenogd2FzIGhlcmUgOikAAAA=" },\n                { "name"  : "hermitpal", "score" : 196, "rank"  : 8, "userID": 
                    "@i64@110000106401474$i64$", "data"  : "R2hlcm1pdCB3YXMgaGVyZSA6KQA=" },\n                { "name"  : "Shermanica", "score" : 199, "rank"  : 9, "userID": 
                    "@i64@1100001009bb750$i64$" },\n                { "name"  : "Joeyman98", "score" : 239, "rank"  : 10, "userID": "@i64@11000010624f1e3$i64$", "data"  : 
                    "I0pvZXltYW45OCB3YXMgaGVyZSA6KQAA" }\n        \n    ]\n}\n",
        "lb_name" : "YYLeaderboard_10/29/21--",
        "event_type" : "leaderboard_download",
        "id" : 3.0,
        "num_entries" : 10.0,
        "status" : 1.0
      }
  */
  const std::map<std::string, variant> leaderboard_download_event = {
      {"entries", entries_buffer},
      {"lb_name", leaderboard_name_buffer},
      {"event_type", "leaderboard_download"},
      {"id", id},
      {"num_entries", leaderboard_scores_downloaded_result.m_cEntryCount},
      {"status", (leaderboard_scores_downloaded_result.m_cEntryCount == 0) ? 0 : 1}};

  posted_async_events_mutex.lock();
  enigma::posted_async_events.push(leaderboard_download_event);
  posted_async_events_mutex.unlock();
}
}  // namespace enigma

namespace enigma_user {

unsigned lb_max_entries{10};

/**
 * @note Continue after overlay constants to prevent overlapping.
 * 
 */
const unsigned lb_disp_none{22};
const unsigned lb_disp_numeric{23};
const unsigned lb_disp_time_sec{24};
const unsigned lb_disp_time_ms{25};

const unsigned lb_sort_none{26};
const unsigned lb_sort_ascending{27};
const unsigned lb_sort_descending{28};

int steam_create_leaderboard(const std::string& lb_name, const unsigned sort_order, const unsigned display_type) {
  if (!leaderboards_pre_checks("steam_create_leaderboard")) return -1;

  ELeaderboardSortMethod leaderboard_sort_method;

  switch (sort_order) {
    case enigma_user::lb_sort_none:
      leaderboard_sort_method = ELeaderboardSortMethod::k_ELeaderboardSortMethodNone;
      break;
    case enigma_user::lb_sort_ascending:
      leaderboard_sort_method = ELeaderboardSortMethod::k_ELeaderboardSortMethodAscending;
      break;
    case enigma_user::lb_sort_descending:
      leaderboard_sort_method = ELeaderboardSortMethod::k_ELeaderboardSortMethodDescending;
      break;
    default:
      DEBUG_MESSAGE(
          "Calling steam_create_leaderboard failed. Invalid sort order. The allowed options are: lb_sort_none, "
          "lb_sort_ascending, and lb_sort_descending.",
          M_ERROR);
      return -1;
  }

  ELeaderboardDisplayType leaderboard_display_type;

  switch (display_type) {
    case enigma_user::lb_disp_none:
      leaderboard_display_type = ELeaderboardDisplayType::k_ELeaderboardDisplayTypeNone;
      break;
    case enigma_user::lb_disp_numeric:
      leaderboard_display_type = ELeaderboardDisplayType::k_ELeaderboardDisplayTypeNumeric;
      break;
    case enigma_user::lb_disp_time_sec:
      leaderboard_display_type = ELeaderboardDisplayType::k_ELeaderboardDisplayTypeTimeSeconds;
      break;
    case enigma_user::lb_disp_time_ms:
      leaderboard_display_type = ELeaderboardDisplayType::k_ELeaderboardDisplayTypeTimeMilliSeconds;
      break;
    default:
      DEBUG_MESSAGE(
          "Calling steam_create_leaderboard failed. Invalid display type. The allowed options are: lb_disp_none, "
          "lb_disp_numeric, lb_disp_time_sec, and lb_disp_time_ms.",
          M_ERROR);
      return -1;
  }

  const int id{enigma::leaderboards_array.add(nullptr)};

  if (!steamworks_gc::GCMain::get_gameclient()->get_gc_leaderboards()->create_leaderboard(
          id, lb_name, leaderboard_sort_method, leaderboard_display_type)) {
    DEBUG_MESSAGE("Calling steam_create_leaderboard failed.", M_ERROR);
    return -1;
  }

  return id;
}

int steam_upload_score(const std::string& lb_name, const int score) {
  if (!leaderboards_pre_checks("steam_upload_score")) return -1;

  const int find_id{enigma::leaderboards_array.add(nullptr)};

  steamworks_gc::GCMain::get_gameclient()->get_gc_leaderboards()->find_leaderboard(find_id, lb_name);

  const int id{enigma::scores_array.add(nullptr)};

  if (!steamworks_gc::GCMain::get_gameclient()->get_gc_leaderboards()->upload_score(
          id, score, ELeaderboardUploadScoreMethod::k_ELeaderboardUploadScoreMethodKeepBest)) {
    DEBUG_MESSAGE("Calling steam_upload_score failed. Make sure that the leaderboard exists.", M_ERROR);
    return -1;
  }

  return id;
}

int steam_upload_score_ext(const std::string& lb_name, const unsigned score, const bool force_update) {
  if (!leaderboards_pre_checks("steam_upload_score_ext")) return -1;

  const int find_id{enigma::leaderboards_array.add(nullptr)};

  // Resets the gc_leaderboards::current_leaderboard_ attribute and sets up a new call back.
  steamworks_gc::GCMain::get_gameclient()->get_gc_leaderboards()->find_leaderboard(find_id, lb_name);

  const int id{enigma::scores_array.add(nullptr)};
  bool success{false};

  // Checks if we have a leaderboard handle in gc_leaderboards::current_leaderboard_ and then uploads the score.
  if (force_update) {
    success = steamworks_gc::GCMain::get_gameclient()->get_gc_leaderboards()->upload_score(
        id, score, ELeaderboardUploadScoreMethod::k_ELeaderboardUploadScoreMethodForceUpdate);
  } else {
    success = steamworks_gc::GCMain::get_gameclient()->get_gc_leaderboards()->upload_score(id, score);
  }

  if (!success) {
    DEBUG_MESSAGE("Calling steam_upload_score_ext failed. Make sure that the leaderboard exists.", M_ERROR);
    return -1;
  }

  return id;
}

int steam_upload_score_buffer(const std::string& lb_name, const unsigned score, const unsigned buffer) { return -1; }

int steam_upload_score_buffer_ext(const std::string& lb_name, const unsigned score, const unsigned buffer,
                                  const bool force_update) {
  return -1;
}

int steam_download_scores(const std::string& lb_name, const int start_idx, const int end_idx) {
  if (!leaderboards_pre_checks("steam_download_scores")) return -1;

  const int find_id{enigma::leaderboards_array.add(nullptr)};

  steamworks_gc::GCMain::get_gameclient()->get_gc_leaderboards()->find_leaderboard(find_id, lb_name);

  const int id{enigma::entries_array.add(nullptr)};

  if (!steamworks_gc::GCMain::get_gameclient()->get_gc_leaderboards()->download_scores(
          id, ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobal, start_idx, end_idx)) {
    DEBUG_MESSAGE("Calling steam_download_scores failed. Make sure that the leaderboard exists.", M_ERROR);
    return -1;
  }

  return id;
}

int steam_download_scores_around_user(const std::string& lb_name, const int range_start, const int range_end) {
  if (!leaderboards_pre_checks("steam_download_scores_around_user")) return -1;

  const int find_id{enigma::leaderboards_array.add(nullptr)};

  steamworks_gc::GCMain::get_gameclient()->get_gc_leaderboards()->find_leaderboard(find_id, lb_name);

  const int id{enigma::entries_array.add(nullptr)};

  if (!steamworks_gc::GCMain::get_gameclient()->get_gc_leaderboards()->download_scores(
          id, ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobalAroundUser, range_start, range_end)) {
    DEBUG_MESSAGE("Calling steam_download_scores_around_user failed. Make sure that the leaderboard exists.", M_ERROR);
    return -1;
  }

  return id;
}

int steam_download_friends_scores(const std::string& lb_name) {
  if (!leaderboards_pre_checks("steam_download_friends_scores")) return -1;

  const int find_id{enigma::leaderboards_array.add(nullptr)};

  steamworks_gc::GCMain::get_gameclient()->get_gc_leaderboards()->find_leaderboard(find_id, lb_name);

  const int id{enigma::entries_array.add(nullptr)};

  if (!steamworks_gc::GCMain::get_gameclient()->get_gc_leaderboards()->download_scores(
          id, ELeaderboardDataRequest::k_ELeaderboardDataRequestFriends)) {
    DEBUG_MESSAGE("Calling steam_download_friends_scores failed. Make sure that the leaderboard exists.", M_ERROR);
    return -1;
  }

  return id;
}

}  // namespace enigma_user
