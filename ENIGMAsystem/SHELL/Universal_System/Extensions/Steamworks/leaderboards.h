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
 * @brief The Steam API supports persistent leaderboards with automatically ordered entries. 
 *        These leaderboards can be used to display global and friend leaderboards in your game 
 *        and on the community web page for your game. Each game can have up to 10,000 leaderboards, 
 *        and each leaderboard can be retrieved immediately after a player's score has been inserted 
 *        into it, but note that for each leaderboard, a player can have only one entry, although 
 *        there is no limit on the number of players per leaderboard.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#leaderboards 
 *      for more information
 * 
 */

#ifndef LEADERBOARDS_H
#define LEADERBOARDS_H

#include "Universal_System/Resources/AssetArray.h"
#include "gameclient/gc_leaderboards.h"
#include "gameclient/gc_main.h"

namespace enigma {

/**
 * @brief Stores number of successful upload requests. This variable related to 
 *        ENIGMA as it help displaying a message to the user when the upload 
 *        rate is exceeded.
 * 
 * @note This is only used for more meaningful error messages.
 * 
 */
/*
    
*/
extern unsigned number_of_successful_upload_requests;

/**
 * @brief Whether we exceeeded the rate limit or not. This variable works with 
 *        @c number_of_successful_upload_requests variable as it will be true if 
 *        the user already got an error message.
 * 
 * @note This is only used for more meaningful error messages.
 * 
 */
extern bool upload_rate_limit_exceeded;

/**
 * @brief 
 * 
 * @note This will be used for returning the id for each leaderboard we find.
 * 
 * @bug Currently, AssetArray doesn't support pointers.
 * 
 */
extern AssetArray<LeaderboardFindResult_t*> leaderboards_array;

/**
 * @brief 
 * 
 * @note This will be used for returning the id for each leaderboard we upload.
 * 
 * @bug Currently, AssetArray doesn't support pointers.
 * 
 */
extern AssetArray<LeaderboardScoresDownloaded_t*> entries_array;

/**
 * @brief 
 * 
 * @note This will be used for returning the id for each score we upload.
 * 
 * @bug Currently, AssetArray doesn't support pointers.
 * 
 */
extern AssetArray<LeaderboardScoreUploaded_t*> scores_array;

/**
 * @brief 
 * 
 * @param id 
 * @param pFindLeaderboardResult 
 */
void push_create_leaderboard_steam_async_event(const int& id, const LeaderboardFindResult_t& pFindLeaderboardResult);

/**
 * @brief 
 * 
 * @param id 
 * @param pScoreUploadedResult 
 */
void push_leaderboard_upload_steam_async_event(const int& id, const LeaderboardScoreUploaded_t& pScoreUploadedResult);

/**
 * @brief 
 * 
 * @param id 
 * @param pLeaderboardScoresDownloaded 
 */
void push_leaderboard_download_steam_async_event(const int& id, const std::string& entries_buffer,
                                                 const LeaderboardScoresDownloaded_t& pLeaderboardScoresDownloaded);
}  // namespace enigma

namespace enigma_user {

/**
 * @brief This constant specifies the maximum number of leaderboard entries we can display.
 * 
 * @note This constant is not existing in GMS's.
 * 
 */
extern unsigned lb_max_entries;

/**
 * @brief These constants specify the display type of a leaderboard and should be used with 
 *        the @c steam_create_leaderboard() function.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#leaderboarddisplaytype 
 *      for more information
 * 
 */
extern const unsigned lb_disp_none;
extern const unsigned lb_disp_numeric;
extern const unsigned lb_disp_time_sec;
extern const unsigned lb_disp_time_ms;

/**
 * @brief These constants specify the sort order of a leaderboard and should be used with 
 *        the @c steam_create_leaderboard() function.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#leaderboardsortorder 
 *      for more information.
 * 
 */
extern const unsigned lb_sort_none;
extern const unsigned lb_sort_ascending;
extern const unsigned lb_sort_descending;

/**
 * @brief This function used to create a new leaderboard for your game.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#steam_create_leaderboard 
 *      for more information.
 * 
 * @par
 * Async DS map key/value pairs:
 * - @c "id" -> The asynchronous request ID.
 * - @c "event_type" -> The string value "create_leaderboard".
 * - @c "status" -> The status code, 0 if the leaderboard was created and 1 if it already existed.
 * - @c "lb_name" -> The name of the leaderboard.
 * 
 * @param lb_name The name of the leaderboard to create.
 * @param sort_order The sort order of the leaderboard. @see constants above.
 * @param display_type The display type of the leaderboard. @see constants above.
 * @return int The asynchronous request ID.
 */
int steam_create_leaderboard(const std::string& lb_name, const unsigned sort_order, const unsigned display_type);

/**
 * @brief This function will send a score to the given leaderboard. If the function call fails for 
 *        any reason it will return -1 and the Async event will not be triggered.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#steam_upload_score 
 *      for more information.
 * 
 * @par
 * Async DS map key/value pairs:
 * - @c "event_type" -> This key will hold the value @c "leaderboard_upload".
 * - @c "post_id" -> This key should match the ID value returned by the upload calling function.
 * - @c "lb_name" -> This key holds the name of the leaderboard which was posted to.
 * - @c "success" -> Will be 1 if the post succeeded, 0 failed.
 * - @c "updated" -> Will be 1 if the leaderboard score was actually updated (ie: the new score was 
 *                   better) or 0 otherwise.
 * - @c "score" -> This key holds the score which was posted.
 * 
 * @param lb_name The name of the leaderboard to post to.
 * @param score The score to post.
 * @return int The asynchronous request ID.
 */
int steam_upload_score(const std::string& lb_name, const int score);

/**
 * @brief This function will send a score to the given leaderboard. It is similar to the 
 *        @c steam_upload_score() fucntion but has an extra argument that will allow you 
 *        to force the update of the score, as by default Steam only updates the score if 
 *        it is better than the previous one. If the function call fails for any reason it 
 *        will return -1 and the Async event will not be triggered.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#steam_upload_score_ext 
 *      for more information.
 * 
 * @par
 * Async DS map key/value pairs same as @c steam_upload_score() function.
 * 
 * @param lb_name The name of the leaderboard to post to.
 * @param score The score to post.
 * @param force_update The force update flag.
 * @return int The asynchronous request ID.
 */
int steam_upload_score_ext(const std::string& lb_name, const unsigned score, const bool force_update);

/**
 * @brief This function will send a score to the given leaderboard along with a data package 
 *        created from a buffer. The buffer should be no more than 256 bytes in size - anything 
 *        beyond that will be chopped off - and can contain any data you require. If the 
 *        function call fails for any reason it will return -1 and the Async event will not be 
 *        triggered.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#steam_upload_score_buffer 
 *      for more information.
 * 
 * @par
 * Async DS map key/value pairs same as @c steam_upload_score() function.
 * 
 * @param lb_name The name of the leaderboard to post to.
 * @param score The score to post.
 * @param buffer The buffer to post.
 * @return int The asynchronous request ID.
 */
int steam_upload_score_buffer(const std::string& lb_name, const unsigned score, const unsigned buffer);

/**
 * @brief This function will send a score to the given leaderboard along with a data package created 
 *        from a buffer. The buffer should be no more than 256 bytes in size - anything beyond that 
 *        will be chopped off - and can contain any data you require. This function is similar to 
 *        @c steam_upload_score_buffer() function but has an extra argument that will allow you to 
 *        force the update of the score, as by default Steam only updates the score if it is better 
 *        than the previous one. If the function call fails forany reason it will return -1 and the 
 *        Async event will not be triggered.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#steam_upload_score_buffer_ext 
 *      for more information.
 * 
 * @par
 * Async DS map key/value pairs same as @c steam_upload_score() function.
 * 
 * @param lb_name The name of the leaderboard to post to.
 * @param score The score to post.
 * @param buffer The buffer to post.
 * @param force_update The force update flag.
 * @return int The asynchronous request ID.
 */
int steam_upload_score_buffer_ext(const std::string& lb_name, const unsigned score, const unsigned buffer,
                                  const bool force_update);

/**
 * @brief This function is used retrieve a sequential range of leaderboard entries by leaderboard ranking. 
 *        If the function call fails for any reason it will return -1 and the async event will not be 
 *        triggered.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#steam_download_scores 
 *      for more information.
 * 
 * @par
 * Async DS map key/value pairs:
 * - @c "event_type" > This key will hold the value @c "leaderboard_download".
 * - @c "id" -> This key should match the ID value returned by the download calling function.
 * - @c "status" -> The status of the callback, where 0 equals a failure or that no results were returned, and 1 equals a success.
 * - @c "lb_name" -> This key holds the name of the leaderboard which was posted to.
 * - @c "num_entries" -> The number of "rows" of leaderboard data that is being returned.
 * - @c "entries" -> A JSON object string that contains another DS map, which will either contain the key "default" (signifying 
 *                that no results are contained within) or the key " entries", which you can then get the value of. This returned value for 
 *                "entries" will be a DS list containing each of the ranks from the leaderboard, where each entry in the list will itself 
 *                reference another DS Map which will contain the keys "name", "score" and " rank", and it may also contain a "data" key 
 *                depending on the function used to upload.
 * 
 * @param lb_name The name of the leaderboard to download.
 * @param start_idx The index of the first entry to download.
 * @param end_idx The index of the last entry to download.
 * @return int The asynchronous request ID.
 */
int steam_download_scores(const std::string& lb_name, const int start_idx, const int end_idx);

/**
 * @brief This function is used retrieve a sequential range of leaderboard entries by leaderboard ranking. 
 *        If the function call fails for any reason it will return -1 and the async event will not be 
 *        triggered.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#steam_download_scores_around_user 
 *      for more information.
 * 
 * @par
 * Async DS map key/value pairs same as @c steam_download_scores() function.
 * 
 * @param lb_name The name of the leaderboard to download.
 * @param range_start The number of entries to download before the user's entry.
 * @param range_end The number of entries to download after the user's entry.
 * @return int The asynchronous request ID.
 */
int steam_download_scores_around_user(const std::string& lb_name, const int range_start, const int range_end);

/**
 * @brief This function is used retrieve a sequential range of leaderboard entries by leaderboard ranking. 
 *        If the function call fails for any reason it will return -1 and the async event will not be 
 *        triggered.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Leaderboards#steam_download_friends_scores 
 *      for more information.
 * 
 * @par
 * Async DS map key/value pairs same as @c steam_download_scores() function.
 * 
 * @param lb_name 
 * @return int 
 */
int steam_download_friends_scores(const std::string& lb_name);

}  // namespace enigma_user

#endif  // LEADERBOARDS_H
