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
  The Steam API supports persistent leaderboards with automatically ordered entries. 
  These leaderboards can be used to display global and friend leaderboards in your game 
  and on the community web page for your game. Each game can have up to 10,000 leaderboards, 
  and each leaderboard can be retrieved immediately after a player's score has been inserted 
  into it, but note that for each leaderboard, a player can have only one entry, although 
  there is no limit on the number of players per leaderboard.
  Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#leaderboards
  for more information.
*/

#ifndef LEADERBOARDS_H
#define LEADERBOARDS_H

// TODO: This documentation need to be improved when uploading a game to Steam Store.

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
