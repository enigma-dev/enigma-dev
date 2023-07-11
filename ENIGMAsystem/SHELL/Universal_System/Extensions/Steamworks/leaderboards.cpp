#include "leaderboards.h"

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

namespace enigma_user {

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
      break;
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
      break;
  }

  steamworks::c_main::get_c_game_client()->get_c_leaderboards()->create_leaderboard(lb_name, leaderboard_sort_method,
                                                                                    leaderboard_display_type);

  return 0;
}

int steam_upload_score(const std::string& lb_name, const int score) {
  if (!leaderboards_pre_checks("steam_upload_score")) return -1;

  steamworks::c_main::get_c_game_client()->get_c_leaderboards()->upload_score(lb_name, score,
                                                                              k_ELeaderboardUploadScoreMethodKeepBest);

  return 0;
}

int steam_upload_score_ext(const std::string& lb_name, const unsigned score, const bool force_update) {
  if (!leaderboards_pre_checks("steam_upload_score_ext")) return -1;

  if (force_update) {
    steamworks::c_main::get_c_game_client()->get_c_leaderboards()->upload_score(
        lb_name, score, k_ELeaderboardUploadScoreMethodForceUpdate);
  } else {
    steamworks::c_main::get_c_game_client()->get_c_leaderboards()->upload_score(lb_name, score);
  }

  return 0;
}

int steam_upload_score_buffer(const std::string& lb_name, const unsigned score, const unsigned buffer) { return -1; }

int steam_upload_score_buffer_ext(const std::string& lb_name, const unsigned score, const unsigned buffer,
                                  const bool force_update) {
  return -1;
}

int steam_download_scores(const std::string& lb_name, const int start_idx, const int end_idx) {
  if (!leaderboards_pre_checks("steam_upload_score")) return -1;

  steamworks::c_main::get_c_game_client()->get_c_leaderboards()->download_scores(
      lb_name, k_ELeaderboardDataRequestGlobal, start_idx, end_idx);

  return 0;
}

int steam_download_scores_around_user(const std::string& lb_name, const int range_start, const int range_end) {
  if (!leaderboards_pre_checks("steam_upload_score")) return -1;

  steamworks::c_main::get_c_game_client()->get_c_leaderboards()->download_scores(
      lb_name, k_ELeaderboardDataRequestGlobalAroundUser, range_start, range_end);

  return 0;
}

int steam_download_friends_scores(const std::string& lb_name) { 
  if (!leaderboards_pre_checks("steam_upload_score")) return -1;

  steamworks::c_main::get_c_game_client()->get_c_leaderboards()->download_scores(
      lb_name, k_ELeaderboardDataRequestFriends);

  return 0;
}

}  // namespace enigma_user
