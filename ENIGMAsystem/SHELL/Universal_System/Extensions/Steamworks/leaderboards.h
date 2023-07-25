#ifndef LEADERBOARDS_H
#define LEADERBOARDS_H

#include "game_client/c_main.h"

namespace enigma {
extern unsigned lb_entries_download_id;
}  // namespace enigma

namespace enigma_user {

// The maximum number of entries to fetch
extern unsigned lb_max_entries;

// Continue after overlay constants to prevent overlapping
extern const unsigned lb_disp_none;
extern const unsigned lb_disp_numeric;
extern const unsigned lb_disp_time_sec;
extern const unsigned lb_disp_time_ms;

extern const unsigned lb_sort_none;
extern const unsigned lb_sort_ascending;
extern const unsigned lb_sort_descending;

int steam_create_leaderboard(const std::string& lb_name, const unsigned sort_order, const unsigned display_type);
int steam_upload_score(const std::string& lb_name, const int score);
int steam_upload_score_ext(const std::string& lb_name, const unsigned score, const bool force_update);
int steam_upload_score_buffer(const std::string& lb_name, const unsigned score, const unsigned buffer);
int steam_upload_score_buffer_ext(const std::string& lb_name, const unsigned score, const unsigned buffer,
                                  const bool force_update);

// "event_type" - This key will hold the value "leaderboard_download"
// "id" - This key should match the ID value returned by the download calling function
// "status" - The status of the callback, where -1 equals a failure or that no results were returned, and 0 equals a success.
// "lb_name" - This key holds the name of the leaderboard which was posted to
// "numEntries" - The number of "rows" of leaderboard data that is being returned.
/*
     "entries" - A JSON object string that contains another DS map, which will either contain the key "default" 
     (signifying that no results are contained within) or the key " entries", which you can then get the value of. This returned 
     value for "entries" will be a DS list containing each of the ranks from the leaderboard, where each entry in the list will itself 
     reference another DS Map which will contain the keys "name", "score" and " rank", and it may also contain a "data" key depending 
     on the function used to upload.
  */
int steam_download_scores(const std::string& lb_name, const int start_idx, const int end_idx);

int steam_download_scores_around_user(const std::string& lb_name, const int range_start, const int range_end);
int steam_download_friends_scores(const std::string& lb_name);

}  // namespace enigma_user

#endif  // LEADERBOARDS_H
