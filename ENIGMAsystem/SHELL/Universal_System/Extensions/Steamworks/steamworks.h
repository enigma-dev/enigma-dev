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

#ifndef STEAMWORKS_H
#define STEAMWORKS_H

#include <filesystem>
#include <fstream>


namespace fs = std::filesystem;

namespace enigma {

/**
 * @brief This function initialises the steam APIs.
 * 
 * @note This function is automatically called by the extension system.
 * 
 */
void extension_steamworks_init();

/**
 * @brief This function fires an Async event from the Steamworks extension.
 * 
 * @note This function is called inside @c Platforms\General\PFmain.cpp to 
 *       fire an Async event from the Steamworks extension.
 * 
 * @note It should be fired from @c Universal_System\Extensions\Asynchronous
 *       extension but currently it is not thread safe.
 * 
 */
void fireSteamworksEvent();

}  // namespace enigma

#endif  // STEAMWORKS_H
