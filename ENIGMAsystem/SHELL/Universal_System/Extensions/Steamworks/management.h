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

#ifndef MANAGEMENT_H
#define MANAGEMENT_H

// TODO: This documentation need to be improved when uploading a game to Steam Store.

namespace enigma_user {

/*
    NOTE: This function is automatically called by the extension system.
    
    This function initialises the steam APIs. Calls 
    enigma::extension_steamworks_init().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Management#steam_init
    for more information.
*/
void steam_init();

/*
    This function updates the steam APIs. Calls c_main::run_callbacks(). This function 
    is required to be called in order for the Steamworks extension to work. We recommend 
    you place this function in a persistent controller object that calls it inside its 
    Step Event.
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Management#steam_update
    for more information.
*/
void steam_update();

/*
    This function shuts down the Steamworks API, releases pointers and frees memory.
    This function is required to be called in order for the Steamworks extension to 
    work. We recommend you place this function in the GameEnd Event of a controller 
    object. You need to check if this is not a game_restart(). Calls c_main::shutdown().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Management#steam_shutdown
    for more information.
*/
void steam_shutdown();

}  // namespace enigma_user

#endif  // MANAGEMENT_H
