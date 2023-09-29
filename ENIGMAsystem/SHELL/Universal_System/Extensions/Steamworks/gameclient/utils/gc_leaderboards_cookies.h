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
 * @brief This class will help attach a cookie (payload) to leaderboard callresults. This class is mainly for 
 *        integrating with ENIGMA's @c AssetArray class and not directly related to Steamworks API.
 * 
 */

#ifndef GC_LEADERBOARDS_COOKIES_H
#define GC_LEADERBOARDS_COOKIES_H

#include "Universal_System/Extensions/Steamworks/gameclient/gc_leaderboards.h"
#include "Universal_System/Extensions/Steamworks/leaderboards.h"

namespace steamworks_gc {

class GCLeaderboardsCookies {
 public:
  // GCLeaderboardsCookies constructor.
  virtual ~GCLeaderboardsCookies() = default;

  /**
   * @brief is the callresult here and done?
   * 
   * @return true 
   * @return false 
   */
  virtual bool is_done() const = 0;

 private:
  /**
   * @brief Registers a callresult.
   * 
   * @param steam_api_call 
   */
  virtual void set_call_result(SteamAPICall_t& steam_api_call) = 0;
};

}  // namespace steamworks_gc

#endif  // GC_LEADERBOARDS_COOKIES_H
