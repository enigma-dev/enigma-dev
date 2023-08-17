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
  This class will help attach a cookie (payload) to a leaderboard call results. This class is mainly for
  integrating with ENIGMA's AssetArray class and not directly related to Steamworks API.
*/

#ifndef LEADERBOARDS_FIND_RESULT_COOKIES_H
#define LEADERBOARDS_FIND_RESULT_COOKIES_H

// TODO: This documentation need to be improved when uploading a game to Steam Store.

#include "../game_client.h"

namespace steamworks {

class c_leaderboards;

class c_leaderboards_find_result_cookies {
 public:
  c_leaderboards_find_result_cookies(int id_, c_leaderboards* c_leaderboards, SteamAPICall_t steam_api_call);
  ~c_leaderboards_find_result_cookies() = default;

  bool is_done() const { return c_leaderboards_find_result_cookies::is_done_; }

 private:
  c_leaderboards* c_leaderboards_;

  /*
      This id is part of the payload that will be attached to the leaderboard find results. This
      id must be returned from AssetArray add() function.
 */
  int id_;

  /*
    This flag will be set to true when the call result is received. This flag will be used to
    determine if we can destroy this object or not.
*/
  bool is_done_;

  void set_call_result(SteamAPICall_t steam_api_call);

  // Called when SteamUserStats()->FindOrCreateLeaderboard() returns asynchronously
  void on_find_leaderboard(LeaderboardFindResult_t* pFindLearderboardResult, bool bIOFailure);
  CCallResult<c_leaderboards_find_result_cookies, LeaderboardFindResult_t> m_callResultFindLeaderboard;
};
}  // namespace steamworks

#endif  // LEADERBOARDS_FIND_RESULT_COOKIES_H
