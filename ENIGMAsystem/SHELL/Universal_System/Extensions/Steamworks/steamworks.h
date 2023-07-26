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

#ifndef STEAMWORKS_H
#define STEAMWORKS_H

// TODO: This documentation need to be improved when uploading a game to Steam Store.

#include "game_client/c_main.h"

#include <fstream>
#include <iostream>

#include <filesystem>
namespace fs = std::filesystem;

namespace enigma {

/*
    NOTE:   This function is automatically called by the extension system.
    
    This function initialises the steam APIs. Calls c_main::init().
*/
void extension_steamworks_init();

}  // namespace enigma

#endif  // STEAMWORKS_H
