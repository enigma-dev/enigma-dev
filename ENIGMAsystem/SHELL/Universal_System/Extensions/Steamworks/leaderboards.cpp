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

// TODO: Use the find_leaderboard() function return id inside upload and download functions.

#include "leaderboards.h"

#include "Universal_System/../Platforms/General/PFmain.h"
#include "game_client/c_leaderboards.h"

bool leaderboards_pre_checks(const std::string& script_name) {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling " + script_name + " failed. Make sure that the API is initialized correctly.", M_ERROR);
    return false;
  }

  if (!steamworks::c_game_client::is_user_logged_on()) {
    DEBUG_MESSAGE("Calling " + script_name + " failed. Make sure that the user is logged in.", M_ERROR);
    return false;
  }

  return true;
}

namespace enigma {
// struct Leaderboard {
//   // stuff here
// }
AssetArray<SteamLeaderboard_t*> leaderboards_array;
AssetArray<LeaderboardEntry_t*> entries_array;
AssetArray<bool> scores_array;

void push_create_leaderboard_steam_async_event(int id, LeaderboardFindResult_t* pFindLeaderboardResult) {
  const std::map<std::string, variant> leaderboard_find_event = {
      {"id", id},
      {"event_type", "create_leaderboard"},
      {"status", pFindLeaderboardResult->m_bLeaderboardFound},
      {"lb_name", steamworks::c_leaderboards::get_leaderboard_name(pFindLeaderboardResult->m_hSteamLeaderboard)}};

  // wait(&enigma::mutex);
  enigma::posted_async_events.push(leaderboard_find_event);
  // signal(&enigma::mutex);
}

void push_leaderboard_upload_steam_async_event(int id, LeaderboardScoreUploaded_t* pScoreUploadedResult) {
  // GMS's output:
  /*
      Steam ASYNC: {"updated":1.0,"lb_name":"YYLeaderboard_10/29/21--","success":1.0,"event_type":"leaderboard_upload","score":325.0,"post_id":6.0}
  */
  const std::map<std::string, variant> leaderboard_upload_event = {
      {"event_type", "leaderboard_upload"},
      {"post_id", id},
      {"lb_name", steamworks::c_leaderboards::get_leaderboard_name(pScoreUploadedResult->m_hSteamLeaderboard)},
      {"success", pScoreUploadedResult->m_bSuccess},
      {"updated", pScoreUploadedResult->m_bScoreChanged},
      {"score", pScoreUploadedResult->m_nScore}};

  // wait(&enigma::mutex);
  enigma::posted_async_events.push(leaderboard_upload_event);
  // signal(&enigma::mutex);
}

void push_leaderboard_download_steam_async_event(int id, LeaderboardScoresDownloaded_t* pLeaderboardScoresDownloaded,
                                                 std::stringstream& leaderboard_entries_buffer) {
  // GMS's output:
  /*
      Steam ASYNC: {"entries":"{\n    "entries": [\n                { "name"  : "TomasJPereyra", "score" : 1, "rank"  : 1, "userID": "@i64@110000108ae8556$i64$" }
      ,\n                { "name"  : "Scott-ish", "score" : 10, "rank"  : 2, "userID": "@i64@11000010241f4ea$i64$" },\n                { "name"  : "Loyal RaveN", 
      "score" : 63, "rank"  : 3, "userID": "@i64@11000015c558396$i64$" },\n                { "name"  : "luca.muehlbauer", "score" : 100, "rank"  : 4, "userID": 
      "@i64@11000015d44ce04$i64$", "data"  : "UWx1Y2EubXVlaGxiYXVlciB3YXMgaGVyZSA6KQAAAAA=" },\n                { "name"  : "simon", "score" : 113, "rank"  : 5, 
      "userID": "@i64@1100001040447b6$i64$" },\n                { "name"  : "meFroggy", "score" : 138, "rank"  : 6, "userID": "@i64@110000117c9d62b$i64$", "data"
        : "HG1lRnJvZ2d5IHdhcyBoZXJlIDopAAAA" },\n                { "name"  : "12349184732146127845127895", "score" : 155, "rank"  : 7, "userID": "@i64@11000010be
      342ce$i64$", "data"  : "FEZlZWxpbmcgU2lja25lenogd2FzIGhlcmUgOikAAAA=" },\n                { "name"  : "hermitpal", "score" : 196, "rank"  : 8, "userID": 
      "@i64@110000106401474$i64$", "data"  : "R2hlcm1pdCB3YXMgaGVyZSA6KQA=" },\n                { "name"  : "Shermanica", "score" : 199, "rank"  : 9, "userID": 
      "@i64@1100001009bb750$i64$" },\n                { "name"  : "Joeyman98", "score" : 239, "rank"  : 10, "userID": "@i64@11000010624f1e3$i64$", "data"  : 
      "I0pvZXltYW45OCB3YXMgaGVyZSA6KQAA" }\n        \n    ]\n}\n","lb_name":"YYLeaderboard_10/29/21--","event_type":"leaderboard_download","id":3.0,
      "num_entries":10.0,"status":1.0}
  */
  const std::map<std::string, variant> leaderboard_download_event = {
      {"entries", leaderboard_entries_buffer.str()},
      {"lb_name", steamworks::c_leaderboards::get_leaderboard_name(pLeaderboardScoresDownloaded->m_hSteamLeaderboard)},
      {"event_type", "leaderboard_download"},
      {"id", id},
      {"num_entries", pLeaderboardScoresDownloaded->m_cEntryCount},
      {"status", (pLeaderboardScoresDownloaded->m_cEntryCount == 0)? 0 : 1}
  };

  // wait(&enigma::mutex);
  enigma::posted_async_events.push(leaderboard_download_event);
  // signal(&enigma::mutex);
}
}  // namespace enigma

namespace enigma_user {

unsigned lb_max_entries{10};

// Continue after overlay constants to prevent overlapping
const unsigned lb_disp_none{21};
const unsigned lb_disp_numeric{22};
const unsigned lb_disp_time_sec{23};
const unsigned lb_disp_time_ms{24};

const unsigned lb_sort_none{25};
const unsigned lb_sort_ascending{26};
const unsigned lb_sort_descending{27};

int steam_create_leaderboard(const std::string& lb_name, const unsigned sort_order, const unsigned display_type) {
  if (!leaderboards_pre_checks("steam_create_leaderboard")) return -1;

  ELeaderboardSortMethod leaderboard_sort_method;

  switch (sort_order) {
    case enigma_user::lb_sort_none:
      leaderboard_sort_method = k_ELeaderboardSortMethodNone;
      break;
    case enigma_user::lb_sort_ascending:
      leaderboard_sort_method = k_ELeaderboardSortMethodAscending;
      break;
    case enigma_user::lb_sort_descending:
      leaderboard_sort_method = k_ELeaderboardSortMethodDescending;
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
      leaderboard_display_type = k_ELeaderboardDisplayTypeNone;
      break;
    case enigma_user::lb_disp_numeric:
      leaderboard_display_type = k_ELeaderboardDisplayTypeNumeric;
      break;
    case enigma_user::lb_disp_time_sec:
      leaderboard_display_type = k_ELeaderboardDisplayTypeTimeSeconds;
      break;
    case enigma_user::lb_disp_time_ms:
      leaderboard_display_type = k_ELeaderboardDisplayTypeTimeMilliSeconds;
      break;
    default:
      DEBUG_MESSAGE(
          "Calling steam_create_leaderboard failed. Invalid display type. The allowed options are: lb_disp_none, "
          "lb_disp_numeric, lb_disp_time_sec, and lb_disp_time_ms.",
          M_ERROR);
      return -1;
  }

  const int id{enigma::leaderboards_array.add(NULL)};

  steamworks::c_main::get_c_game_client()->get_c_leaderboards()->find_leaderboard(id, lb_name, leaderboard_sort_method,
                                                                                  leaderboard_display_type);

  return id;
}

int steam_upload_score(const std::string& lb_name, const int score) {
  if (!leaderboards_pre_checks("steam_upload_score")) return -1;

  const int find_id{enigma::leaderboards_array.add(NULL)};

  steamworks::c_main::get_c_game_client()->get_c_leaderboards()->find_leaderboard(
      find_id, lb_name, k_ELeaderboardSortMethodNone, k_ELeaderboardDisplayTypeNone);

  const int id{enigma::scores_array.add(false)};

  if (!steamworks::c_main::get_c_game_client()->get_c_leaderboards()->upload_score(
          id, score, k_ELeaderboardUploadScoreMethodKeepBest)) {
    DEBUG_MESSAGE(
        "Calling steam_upload_score failed. Make sure that the leaderboard exists.", M_ERROR);
    return -1;
  }

  return id;
}

int steam_upload_score_ext(const std::string& lb_name, const unsigned score, const bool force_update) {
  if (!leaderboards_pre_checks("steam_upload_score_ext")) return -1;

  const int find_id{enigma::leaderboards_array.add(NULL)};

  steamworks::c_main::get_c_game_client()->get_c_leaderboards()->find_leaderboard(
      find_id, lb_name, k_ELeaderboardSortMethodNone, k_ELeaderboardDisplayTypeNone);

  const int id{enigma::scores_array.add(false)};
  bool success{false};

  if (force_update) {
    success = steamworks::c_main::get_c_game_client()->get_c_leaderboards()->upload_score(
        id, score, k_ELeaderboardUploadScoreMethodForceUpdate);
  } else {
    success = steamworks::c_main::get_c_game_client()->get_c_leaderboards()->upload_score(id, score);
  }

  if (!success) {
    DEBUG_MESSAGE(
        "Calling steam_upload_score_ext failed. Make sure that the leaderboard exists.", M_ERROR);
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

  const int find_id{enigma::leaderboards_array.add(NULL)};

  steamworks::c_main::get_c_game_client()->get_c_leaderboards()->find_leaderboard(
      find_id, lb_name, k_ELeaderboardSortMethodNone, k_ELeaderboardDisplayTypeNone);

  const int id{enigma::entries_array.add(NULL)};

  if (!steamworks::c_main::get_c_game_client()->get_c_leaderboards()->download_scores(
          id, k_ELeaderboardDataRequestGlobal, start_idx, end_idx)) {
    DEBUG_MESSAGE(
        "Calling steam_download_scores failed. Make sure that the leaderboard exists.", M_ERROR);
    return -1;
  }

  return id;
}

int steam_download_scores_around_user(const std::string& lb_name, const int range_start, const int range_end) {
  if (!leaderboards_pre_checks("steam_download_scores_around_user")) return -1;

  const int find_id{enigma::leaderboards_array.add(NULL)};

  steamworks::c_main::get_c_game_client()->get_c_leaderboards()->find_leaderboard(
      find_id, lb_name, k_ELeaderboardSortMethodNone, k_ELeaderboardDisplayTypeNone);

  const int id{enigma::entries_array.add(NULL)};

  if (!steamworks::c_main::get_c_game_client()->get_c_leaderboards()->download_scores(
          id, k_ELeaderboardDataRequestGlobalAroundUser, range_start, range_end)) {
    DEBUG_MESSAGE(
        "Calling steam_download_scores_around_user failed. Make sure that the leaderboard exists.", M_ERROR);
    return -1;
  }

  return id;
}

int steam_download_friends_scores(const std::string& lb_name) {
  if (!leaderboards_pre_checks("steam_download_friends_scores")) return -1;

  const int find_id{enigma::leaderboards_array.add(NULL)};

  steamworks::c_main::get_c_game_client()->get_c_leaderboards()->find_leaderboard(
      find_id, lb_name, k_ELeaderboardSortMethodNone, k_ELeaderboardDisplayTypeNone);

  const int id{enigma::entries_array.add(NULL)};

  if (!steamworks::c_main::get_c_game_client()->get_c_leaderboards()->download_scores(
          id, k_ELeaderboardDataRequestFriends)) {
    DEBUG_MESSAGE(
        "Calling steam_download_friends_scores failed. Make sure that the leaderboard exists.", M_ERROR);
    return -1;
  }

  return id;
}

}  // namespace enigma_user
