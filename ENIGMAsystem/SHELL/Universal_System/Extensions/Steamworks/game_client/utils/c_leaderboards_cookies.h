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
  This class will help attach a cookie (payload) to leaderboard call results. This class is mainly for
  integrating with ENIGMA's AssetArray class and not directly related to Steamworks API.
*/

#ifndef LEADERBOARDS_COOKIES_H
#define LEADERBOARDS_COOKIES_H

// TODO: This documentation need to be improved when uploading a game to Steam Store.

#include "../game_client.h"

namespace steamworks {

class c_leaderboards_cookies {
 public:
  virtual ~c_leaderboards_cookies() = default;
  virtual bool is_done() const = 0;

 private:
  virtual void set_call_result(SteamAPICall_t steam_api_call) = 0;
};

}  // namespace steamworks

#endif  // LEADERBOARDS_COOKIES_H
