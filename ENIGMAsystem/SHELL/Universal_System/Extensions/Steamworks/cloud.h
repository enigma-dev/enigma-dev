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
 * @brief The Steam Cloud provides an easy and transparent remote file storage system 
 *        for your game. All files written to disk using the cloud functions will be replicated 
 *        to the Steam servers after the game exits. If the user then changes computers, the 
 *        files will then be downloaded to the new computer before the game launches, meaning 
 *        that the game can then access the files by reading them using the appropriate Steam 
 *        functions. The Steam Client does the work of ensuring that the files are kept synchronized 
 *        across all computers the user may be accessing.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Cloud#cloud for more information.
 * 
 */

#ifndef CLOUD_H
#define CLOUD_H

#include "gameclient/gc_main.h"

namespace enigma_user {

/**
 * @brief 
 * 
 * @note Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Cloud#steam_is_cloud_enabled_for_app
 *       for more information.
 * 
 * @return true 
 * @return false 
 */
bool steam_is_cloud_enabled_for_app();

/**
 * @brief 
 * 
 * @note Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Cloud#steam_is_cloud_enabled_for_account
 *       for more information.
 * 
 * @return true 
 * @return false 
 */
bool steam_is_cloud_enabled_for_account();

/**
 * @brief 
 * 
 * @note Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Cloud#steam_get_quota_total
 *       for more information.
 * 
 * @return int 
 */
int steam_get_quota_total();

/**
 * @brief 
 * 
 * @note Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Cloud#steam_get_quota_free
 *       for more information.
 * 
 * @return int 
 */
int steam_get_quota_free();

/**
 * @brief 
 * 
 * @note Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Cloud#steam_file_exists
 *       for more information.
 * 
 * @param filename 
 * @return true 
 * @return false 
 */
bool steam_file_exists(const std::string& filename);

/**
 * @brief 
 * 
 * @note Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Cloud#steam_file_size
 *       for more information.
 * 
 * @param filename 
 * @return int 
 */
int steam_file_size(const std::string& filename);

/**
 * @brief 
 * 
 * @note Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Cloud#steam_file_persisted
 *       for more information.
 * 
 * @param filename 
 * @return true 
 * @return false 
 */
bool steam_file_persisted(const std::string& filename);

/**
 * @brief 
 * 
 * @note Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Cloud#steam_file_write
 *       for more information.
 * 
 * @param filename 
 * @param data 
 * @param size 
 * @return int 
 */
int steam_file_write(const std::string& filename, const std::string& data, int size);

/**
 * @brief 
 * 
 * @note Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Cloud#steam_file_write_file
 *       for more information.
 * 
 * @param steam_filename 
 * @param local_filename 
 * @return int 
 */
int steam_file_write_file(std::string& steam_filename, const std::string& local_filename);

/**
 * @brief 
 * 
 * @note Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Cloud#steam_file_read
 *       for more information.
 * 
 * @param filename 
 * @return std::string 
 */
std::string steam_file_read(const std::string& filename);

/**
 * @brief 
 * 
 * @note Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Cloud#steam_file_share
 *       for more information.
 * 
 * @param filename 
 * @return int 
 */
int steam_file_share(const std::string& filename);

/**
 * @brief 
 * 
 * @note Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Cloud#steam_file_delete
 *       for more information.
 * 
 * @param filename 
 * @return int 
 */
int steam_file_delete(const std::string& filename);

}  // namespace enigma_user

#endif  // CLOUD_H
