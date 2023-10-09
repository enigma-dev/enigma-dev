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
 * @brief The following set of functions are used for setting or getting social 
 *        information.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Social#social for 
 *      more information.
 * 
 * @todo Social API will be postponed until this merged 
 *       https://github.com/enigma-dev/enigma-dev/pull/2361.
 * 
 */

#ifndef SOCIAL_H
#define SOCIAL_H

#include "gameclient/gc_main.h"

namespace enigma {
/**
 * @brief 
 * 
 * @note Implement this function.
 * 
 * @todo Move this function to the gameclient layer.
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
 * @todo Move this function to the gameclient layer.
 * 
 * @param rgba 
 * @return unsigned char 
 */
unsigned char RGBAtoBGRA(unsigned char rgba);
}  // namespace enigma

namespace enigma_user {

/**
 * @brief These constants specify the size of the avatar to be retrieved when using the
 *        function @c steam_get_user_avatar() function.
 * 
 */
extern const unsigned steam_user_avatar_size_small;
extern const unsigned steam_user_avatar_size_medium;
extern const unsigned steam_user_avatar_size_large;

/**
 * @brief Sets a Rich Presence key/value for the current user that is automatically shared to 
 *        all friends playing the same game.
 * 
 * @note This function is different from GMS's as it returns @c bool instead of @c void.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Social#steam_set_rich_presence 
 *      for more information.
 * 
 * @param key The name of the Rich Presence key to set.
 * @param value The value to set for the given key.
 * @return true If the key/value was set successfully.
 * @return false If the key/value was not set successfully.
 */
bool steam_set_rich_presence(const std::string& key, const std::string& value);

/**
 * @brief Clears all of the current user's Rich Presence key/values.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Social#steam_clear_rich_presence 
 *      for more information.
 * 
 */
void steam_clear_rich_presence();

/**
 * @brief Adds the given user to the "recently played with" list (accessed via "Players" - "Recent games") 
 *        menu in Steam overlay. This is usually something to do on session start for all remote users.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Social#steam_user_set_played_with 
 *      for more information.
 * 
 * @param steam_id The Steam user id to add to the list.
 * @return true If the user was added successfully.
 * @return false If the user was not added successfully.
 */
bool steam_user_set_played_with(const unsigned long long steam_id);

/**
 * @brief Returns an array of information about what the current user's Steam friends 
 *        are playing. Equivalent to what can be seen in Steam Friends UI.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Social#steam_get_friends_game_info 
 *      for more information.
 * 
 * @par
 * Players information is returned in the following format:
 * - @c "friendId" -> The Steam user id.
 * - @c "gameId" -> The Steam game id.
 * - @c "lobbyId" -> The Steam lobby id (if hosting a lobby that is open for friends to 
 *                   join - otherwise 0).
 * - @c "name" -> The friend's user name.
 * 
 */
void steam_get_friends_game_info();

/**
 * @brief Fetches an avatar for the specified user ID. Returns 0 if no avatar is set for the user; 
 *        Returns -1 if the request is pending, in which case an Steam Async Event will be triggered. 
 *        Returns positive IDs if the avatar is ready, this id is to be used with the following 
 *        function:
 *        - @c enigma_user::steam_image_get_bgra()
 *        - @c enigma_user::steam_image_get_rgba()
 *        - @c enigma_user::steam_image_get_size()
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Social#steam_get_user_avatar for 
 *      more information.
 * 
 * @par
 * Async DS map key/value pairs:
 * - @c "event_type" -> The string value "avatar_image_loaded"
 * - @c "success" -> Whether the async action succeeded
 * - @c "user_id" -> The associated user's ID
 * - @c "image" -> The image ID that would otherwise be returned by the function
 * - @c "width" -> The image width, in pixels
 * - @c "height" -> The image height, in pixels ****
 * 
 * @param userID The Steam user id to get the avatar for.
 * @param avatar_size The size of the avatar to get. @see constants above.
 * @return int The image ID of the avatar.
 */
int steam_get_user_avatar(const unsigned long long userID, const unsigned avatar_size);

/**
 * @brief Fetches dimensions for the said Steam image ID. If the call succeeds, the return value 
 *        is a two-element array containing width and height in pixels.
 * 
 * @note This function has a special return type. This is one of ENIGMA's primitives.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Social#steam_image_get_size 
 *      for more information.
 * 
 * @param steam_image_id The Steam image id to get the dimensions for.
 * @return var 1D array containing width and height in pixels respectively.
 */
var steam_image_get_size(const int steam_image_id);

/**
 * @brief 
 * 
 * @todo This function requires the buffer work from GSoC 2022 projects which is merged into
 *       AST-Generation branch.
 * 
 * @param steam_image_id The Steam image id to get the image data for.
 * @param buffer The buffer to store the image data in.
 * @param size The size of the buffer.
 * @return true If the image data was retrieved successfully.
 * @return false If the image data was not retrieved successfully.
 */
bool steam_image_get_rgba(const int steam_image_id, const int buffer, const int size);

/**
 * @brief 
 * 
 * @todo This function requires the buffer work from GSoC 2022 projects which is merged into
 *       AST-Generation branch.
 * 
 * @param steam_image_id The Steam image id to get the image data for.
 * @param buffer The buffer to store the image data in.
 * @param size The size of the buffer.
 * @return true If the image data was retrieved successfully.
 * @return false If the image data was not retrieved successfully.
 */
bool steam_image_get_bgra(const int steam_image_id, const int buffer, const int size);

/**
 * @brief 
 * 
 * @note This function is not available in GMS.
 * 
 * @todo This function requires the buffer work from GSoC 2022 projects which is merged into
 *       AST-Generation branch.
 * 
 * @param steam_image_id The Steam image id to get the image data for.
 * @param buffer The buffer to store the image data in.
 * @param size The size of the buffer.
 * @return true If the image data was retrieved successfully.
 * @return false If the image data was not retrieved successfully.
 */
bool steam_image_get_argb(const int steam_image_id, const int buffer, const int size);

/**
 * @brief 
 * 
 * @note This function is not available in GMS.
 * 
 * @todo This function requires the buffer work from GSoC 2022 projects which is merged into
 *       AST-Generation branch.
 * 
 * @param steam_image_id The Steam image id to get the image data for.
 * @return int The sprite id of the image.
 */
int steam_image_create_sprite(const int steam_image_id);

}  // namespace enigma_user

#endif  // SOCIAL_H
