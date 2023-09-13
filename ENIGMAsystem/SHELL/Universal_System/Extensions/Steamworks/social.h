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
    The following set of functions are used for setting or getting social 
    information.
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Social#social for
    more information.
*/

#ifndef SOCIAL_H
#define SOCIAL_H

#include "gameclient/gc_main.h"

// TODO: Social API will be postponed until this merged https://github.com/enigma-dev/enigma-dev/pull/2361.

namespace enigma {
// TODO: Move these functions to the gameclient layer.
/**
 * @brief 
 * 
 * @note Implement this function.
 * 
 * @param rgba 
 * @return unsigned char 
 */
unsigned char RGBAtoARGB(unsigned char rgba);

/**
 * @brief 
 * 
 * @note Implement this function.
 * 
 * @param rgba 
 * @return unsigned char 
 */
unsigned char RGBAtoBGRA(unsigned char rgba);
}  // namespace enigma

namespace enigma_user {

extern const unsigned steam_user_avatar_size_small;
extern const unsigned steam_user_avatar_size_medium;
extern const unsigned steam_user_avatar_size_large;

/*
    NOTE:   This function is different from GMS's as it returns bool instead of void.

    Sets a Rich Presence key/value for the current user that is automatically shared to 
    all friends playing the same game. Calls steamworks::gameclient::set_rich_presence().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Social#steam_set_rich_presence
    for more information.
*/
bool steam_set_rich_presence(const std::string& key, const std::string& value);

/*
    Clears all of the current user's Rich Presence key/values. Calls 
    steamworks::gameclient::clear_rich_presence().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Social#steam_clear_rich_presence
    for more information.
*/
void steam_clear_rich_presence();

/*
    Adds the given user to the "recently played with" list (accessed via "Players" - "Recent games") 
    menu in Steam overlay. This is usually something to do on session start for all remote users.
    Calls steamworks::gameclient::set_played_with().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Social#steam_user_set_played_with
    for more information.
*/
bool steam_user_set_played_with(const unsigned long long steam_id);

/*
    Returns an array of information about what the current user's Steam friends 
    are playing. Equivalent to what can be seen in Steam Friends UI.
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Social#steam_get_friends_game_info
    for more information.
*/
/*
    Players information is returned in the following format:
        - friendId -> The Steam user id
        - gameId -> The Steam game id
        - lobbyId -> The Steam lobby id (if hosting a lobby that is open for friends to join - otherwise 0)
        - name -> The friend's user name
*/
void steam_get_friends_game_info();

/*
    Fetches an avatar for the specified user ID. Returns 0 if no avatar is set for the user; 
    Returns -1 if the request is pending, in which case an Steam Async Event will be triggered. 
    Returns positive IDs if the avatar is ready, this id is to be used with the following function:
        - enigma_user::steam_image_get_bgra()
        - enigma_user::steam_image_get_rgba()
        - enigma_user::steam_image_get_size()
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Social#steam_get_user_avatar for
    more information.
*/
/*
    Async DS map key/value pairs:
        "event_type" -> The string value "avatar_image_loaded"
        "success" -> Whether the async action succeeded
        "user_id" -> The associated user's ID
        "image" -> The image ID that would otherwise be returned by the function
        "width" -> The image width, in pixels
        "height" -> The image height, in pixels ****
*/
int steam_get_user_avatar(const unsigned long long userID, const unsigned avatar_size);

/*
    NOTE:  This function has a special return type.

    Fetches dimensions for the said Steam image ID. If the call succeeds, the return value 
    is a two-element array containing width and height in pixels.
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Social#steam_image_get_size
    for more information.
*/
var steam_image_get_size(const int steam_image_id);

/**
 * @brief 
 * 
 * @note This function requires the buffer work from GSoC 2022 projects which is merged into
 *     AST-Generation branch.
 * 
 * @param steam_image_id 
 * @param buffer 
 * @param size 
 * @return true 
 * @return false 
 */
bool steam_image_get_rgba(const int steam_image_id, const int buffer, const int size);

/**
 * @brief 
 * 
 * @note This function requires the buffer work from GSoC 2022 projects which is merged into
 *      AST-Generation branch.
 * 
 * @param steam_image_id 
 * @param buffer 
 * @param size 
 * @return true 
 * @return false 
 */
bool steam_image_get_bgra(const int steam_image_id, const int buffer, const int size);

/**
 * @brief 
 * 
 * @note This function is not available in GMS.
 * 
 * @note This function requires the buffer work from GSoC 2022 projects which is merged into
 *      AST-Generation branch.
 * 
 * @param steam_image_id 
 * @param buffer 
 * @param size 
 * @return true 
 * @return false 
 */
bool steam_image_get_argb(const int steam_image_id, const int buffer, const int size);

/**
 * @brief 
 * 
 * @note This function is not available in GMS.
 * 
 * @note This function requires the buffer work from GSoC 2022 projects which is merged into
 *     AST-Generation branch.
 * 
 * @param steam_image_id 
 * @return int 
 */
int steam_image_create_sprite(const int steam_image_id);

}  // namespace enigma_user

#endif  // SOCIAL_H
