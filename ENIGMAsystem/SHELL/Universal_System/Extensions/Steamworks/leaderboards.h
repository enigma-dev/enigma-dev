#ifndef LEADERBOARDS_H
#define LEADERBOARDS_H

#include "game_client/c_main.h"

namespace enigma_user {

// Continue after overlay constants to prevent overlapping
const unsigned lb_disp_none = 21;
const unsigned lb_disp_numeric = 22;
const unsigned lb_disp_time_sec = 23;
const unsigned lb_disp_time_ms = 24;

const unsigned lb_sort_none = 25;
const unsigned lb_sort_ascending = 26;
const unsigned lb_sort_descending = 27;

int steam_create_leaderboard(const std::string& lb_name, const unsigned sort_order, const unsigned display_type);
int steam_upload_score(const std::string& lb_name, const unsigned score);
int steam_upload_score_ext(const std::string& lb_name, const unsigned score, const bool force_update);
int steam_upload_score_buffer(const std::string& lb_name, const unsigned score, const unsigned buffer);
int steam_upload_score_buffer_ext(const std::string& lb_name, const unsigned score, const unsigned buffer, const bool force_update);
int steam_download_scores(const std::string& lb_name, const unsigned start_idx, const unsigned end_idx);
int steam_download_scores_around_user(const std::string& lb_name, const unsigned range_start, const unsigned range_end);
int steam_download_friends_scores(const std::string& lb_name);

}  // namespace enigma_user

#endif  // LEADERBOARDS_H
