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

#include "Universal_System/Resources/AssetArray.h"
#include "game_client/c_main.h"

#include <sstream>

namespace enigma {

// TODO: See if you can improve the type of these arrays.
extern AssetArray<SteamLeaderboard_t*>
    leaderboards_array;  // This will be used for returning the id for each leaderboard we find.
extern AssetArray<LeaderboardEntry_t*>
    entries_array;                     // This will be used for returning the id for each leaderboard we download.
extern AssetArray<bool> scores_array;  // This will be used for returning the id for each score we upload.

void push_create_leaderboard_steam_async_event(int id, LeaderboardFindResult_t* pFindLeaderboardResult);
void push_leaderboard_upload_steam_async_event(int id, LeaderboardScoreUploaded_t* pScoreUploadedResult);
void push_leaderboard_download_steam_async_event(int id, LeaderboardScoresDownloaded_t* pLeaderboardScoresDownloaded,
                                                 std::stringstream* leaderboard_entries_buffer);
}  // namespace enigma

namespace enigma_user {

/*
    NOTE:   These constant is not existing in GMS's.

    This constant specifies the maximum number of leaderboard entries we can display.
*/
extern unsigned lb_max_entries;

/*
    These constants specify the display type of a leaderboard and should be used with the function steam_create_leaderboard().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#leaderboarddisplaytype for more information.
*/
extern const unsigned lb_disp_none;
extern const unsigned lb_disp_numeric;
extern const unsigned lb_disp_time_sec;
extern const unsigned lb_disp_time_ms;

/*
    These constants specify the sort order of a leaderboard and should be used with the function steam_create_leaderboard().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#leaderboardsortorder for more information.
*/
extern const unsigned lb_sort_none;
extern const unsigned lb_sort_ascending;
extern const unsigned lb_sort_descending;

/*
    This function used to create a new leaderboard for your game. Calls c_leaderboards::find_leaderboard().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#steam_create_leaderboard for more information.
*/
/*
    Async DS map key/value pairs:
        - "id" - The asynchronous request ID.
        - "event_type" - The string value "create_leaderboard".
        - "status" - The status code, 0 if the leaderboard was created and 1 if it already existed.
        - "lb_name" - The name of the leaderboard.
*/
int steam_create_leaderboard(const std::string& lb_name, const unsigned sort_order, const unsigned display_type);

/*
    This function will send a score to the given leaderboard. If the function call fails for any reason it will return -1 
    and the Async event will not be triggered. Calls c_leaderboards::upload_score().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#steam_upload_score for more information.
*/
/*
    Async DS map key/value pairs:
        - "event_type" - This key will hold the value "leaderboard_upload".
        - "post_id" - This key should match the ID value returned by the upload calling function.
        - "lb_name" - This key holds the name of the leaderboard which was posted to.
        - "success" - Will be 1 if the post succeeded, 0 failed.
        - "updated" - Will be 1 if the leaderboard score was actually updated (ie: the new score was better) or 0 otherwise.
        - "score" - This key holds the score which was posted.
*/
int steam_upload_score(const std::string& lb_name, const int score);

/*
    This function will send a score to the given leaderboard. It is similar to the function steam_upload_scorebut has 
    an extra argument that will allow you to force the update of the score, as by default Steam only updates the score 
    if it is better than the previous one. If the function call fails for any reason it will return -1 and the Async event 
    will not be triggered. Calls c_leaderboards::upload_score().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#steam_upload_score_ext for more information.
*/
/*
    Async DS map key/value pairs same as steam_upload_score().
*/
int steam_upload_score_ext(const std::string& lb_name, const unsigned score, const bool force_update);

/*
    This function will send a score to the given leaderboard along with a data package created from a buffer. The buffer 
    should be no more than 256 bytes in size - anything beyond that will be chopped off - and can contain any data you require.
    If the function call fails for any reason it will return -1 and the Async event will not be triggered. Calls
    c_leaderboards::
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#steam_upload_score_buffer for more information.
*/
/*
    Async DS map key/value pairs same as steam_upload_score().
*/
int steam_upload_score_buffer(const std::string& lb_name, const unsigned score, const unsigned buffer);

/*
    This function will send a score to the given leaderboard along with a data package created from a buffer. The buffer 
    should be no more than 256 bytes in size - anything beyond that will be chopped off - and can contain any data you require. 
    This function is similar to steam_upload_score_buffer but has an extra argument that will allow you to force the update of 
    the score, as by default Steam only updates the score if it is better than the previous one. If the function call fails for
    any reason it will return -1 and the Async event will not be triggered. Calls c_leaderboards::
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#steam_upload_score_buffer_ext for more information.
*/
/*
    Async DS map key/value pairs same as steam_upload_score().
*/
int steam_upload_score_buffer_ext(const std::string& lb_name, const unsigned score, const unsigned buffer,
                                  const bool force_update);

/*
    This function is used retrieve a sequential range of leaderboard entries by leaderboard ranking. If the function call fails 
    for any reason it will return -1 and the async event will not be triggered. Calls c_leaderboards::download_scores().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#steam_download_scores for more information.
*/
/*
    Async DS map key/value pairs:
        - "event_type" - This key will hold the value "leaderboard_download".
        - "id" - This key should match the ID value returned by the download calling function.
        - "status" - The status of the callback, where 0 equals a failure or that no results were returned, and 1 equals a success.
        - "lb_name" - This key holds the name of the leaderboard which was posted to.
        - "num_entries" - The number of "rows" of leaderboard data that is being returned.
        - "entries" - A JSON object string that contains another DS map, which will either contain the key "default" (signifying 
          that no results are contained within) or the key " entries", which you can then get the value of. This returned value for 
          "entries" will be a DS list containing each of the ranks from the leaderboard, where each entry in the list will itself 
          reference another DS Map which will contain the keys "name", "score" and " rank", and it may also contain a "data" key 
          depending on the function used to upload.
*/
int steam_download_scores(const std::string& lb_name, const int start_idx, const int end_idx);

/*
    This function is used retrieve a sequential range of leaderboard entries by leaderboard ranking. If the function call fails 
    for any reason it will return -1 and the async event will not be triggered. Calls c_leaderboards::download_scores().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#steam_download_scores_around_user for more information.
*/
/*
    Async DS map key/value pairs same as steam_download_scores().
*/
int steam_download_scores_around_user(const std::string& lb_name, const int range_start, const int range_end);

/*
    This function is used retrieve a sequential range of leaderboard entries by leaderboard ranking. If the function call fails 
    for any reason it will return -1 and the async event will not be triggered. Calls c_leaderboards::download_scores().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#steam_download_friends_scores for more information.
*/
/*
    Async DS map key/value pairs same as steam_download_scores().
*/
int steam_download_friends_scores(const std::string& lb_name);

}  // namespace enigma_user

#endif  // LEADERBOARDS_H
