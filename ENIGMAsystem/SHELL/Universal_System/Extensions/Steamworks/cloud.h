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
    The Steam Cloud provides an easy and transparent remote file storage system 
    for your game. All files written to disk using the cloud functions will be 
    replicated to the Steam servers after the game exits. If the user then changes 
    computers, the files will then be downloaded to the new computer before the 
    game launches, meaning that the game can then access the files by reading them 
    using the appropriate Steam functions. The Steam Client does the work of ensuring 
    that the files are kept synchronized across all computers the user may be 
    accessing.
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Cloud#cloud for
    more information.
*/

#ifndef CLOUD_H
#define CLOUD_H

#include "game_client/c_main.h"

namespace enigma_user {

bool steam_is_cloud_enabled_for_app();

bool steam_is_cloud_enabled_for_account();

int steam_get_quota_total();

int steam_get_quota_free();

bool steam_file_exists(const std::string& filename);

int steam_file_size(const std::string& filename);

bool steam_file_persisted(const std::string& filename);

int steam_file_write(const std::string& filename, const std::string& data, int size);

int steam_file_write_file(std::string& steam_filename, const std::string& local_filename);

std::string steam_file_read(const std::string& filename);

int steam_file_share(const std::string& filename);

int steam_file_delete(const std::string& filename);

}  // namespace enigma_user

#endif  // CLOUD_H
