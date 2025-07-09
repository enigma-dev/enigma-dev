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

#include "fake_steam_api.h"

/**
 * @brief This function is used to initialize the Steam API.
 * 
 * @note This function was implemented in the fake Steam API for CI testing
 *       purposes till v1.57. After v1.58a this function is no longer used
 *       as its implementation is introduced inside v1.58 however 
 *       @c SteamAPI_InitEx() function must be implemented instead.
 * 
 * @note v1.58 is broken and v1.58a is the fixed version.
 * 
 * @note This function still exists in v1.58 for compatibility reasons.
 * 
 * @see @c SteamAPI_InitEx() function.
 * 
 * @return true If the Steam API was successfully initialized.
 * @return false If the Steam API failed to initialize. @see @c GCMain::init()
 *         for more details.
 */
// bool SteamAPI_Init() { return true; }

/**
 * @brief This is the new function that is used to initialize the Steam API
 *        which is introduced in v1.58 and is used instead of
 *        @c SteamAPI_Init() function.
 * 
 * @param pszInternalCheckInterfaceVersions The internal check interface versions.
 * @param pOutErrMsg Buffer to store the error message in case of failure.
 * @return ESteamAPIInitResult The result of the Steam API initialization.
 */
ESteamAPIInitResult SteamInternal_SteamAPI_Init(const char *pszInternalCheckInterfaceVersions,
                                                SteamErrMsg *pOutErrMsg) {
  return k_ESteamAPIInitResult_OK;
}

void SteamAPI_Shutdown() {}

/**
 * @brief 
 * 
 * @param unOwnAppID 
 * @return false when success
 * @return true when failure
 */
bool SteamAPI_RestartAppIfNecessary(uint32 unOwnAppID) { return false; }
