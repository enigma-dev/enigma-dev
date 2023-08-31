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

#include "game_client/c_main.h"

namespace enigma_user {

void steam_set_rich_presence(const std::string& key, const std::string& value);
void steam_clear_rich_presence();

bool steam_user_set_played_with(long long steam_id);

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
        - steam_image_get_bgra
        - steam_image_get_rgba
        - steam_image_get_size
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
int steam_get_user_avatar(long long userID, int avatar_size);

/*
    Fetches dimensions for the said Steam image ID. If the call succeeds, the return value 
    is a two-element array containing width and height in pixels.
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Social#steam_image_get_size
    for more information.
*/
void steam_image_get_size(long long steam_image_id);

bool steam_image_get_rgba(long long steam_image_id, int buffer, int size);
bool steam_image_get_bgra(long long steam_image_id, int buffer, int size);

}  // namespace enigma_user

#endif  // SOCIAL_H
