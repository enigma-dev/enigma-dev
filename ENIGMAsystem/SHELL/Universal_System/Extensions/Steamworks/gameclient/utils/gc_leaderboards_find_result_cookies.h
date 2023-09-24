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

/*
  Whenever a gc_leaderboards::find_leaderboard() is called, a new object from this class will be instantiated 
  with a unique id. This id will be used when the call result arrive. The call result setting operation is 
  moved to this class from gc_leaderboards class, which means in order to setup a call result, you need to 
  instantiate an onject from this class.
*/

#ifndef GC_LEADERBOARDS_FIND_RESULT_COOKIES_H
#define GC_LEADERBOARDS_FIND_RESULT_COOKIES_H

// TODO: This documentation need to be improved when uploading a game to Steam Store.

#include "gc_leaderboards_cookies.h"

namespace steamworks_gc {

class GCLeaderboards;

class GCLeaderboardsFindResultCookies : public GCLeaderboardsCookies {
 public:
  GCLeaderboardsFindResultCookies(const int& id, GCLeaderboards* gc_leaderboards, SteamAPICall_t& steam_api_call);
  ~GCLeaderboardsFindResultCookies() = default;

 private:
  /*
      This id is part of the payload that will be attached to the leaderboard find results. This
      id must be returned from AssetArray add() function.
 */
  int id_;

  GCLeaderboards* gc_leaderboards_;

  /*
    This flag will be set to true when the call result is received. This flag will be used to
    determine if we can destroy this object or not.
*/
  bool is_done_;

  bool is_done() const override;

  void set_call_result(SteamAPICall_t& steam_api_call) override;

  // Called when SteamUserStats()->FindOrCreateLeaderboard() returns asynchronously
  void on_find_leaderboard(LeaderboardFindResult_t* pFindLearderboardResult, bool bIOFailure);
  // CCallResult<GCLeaderboardsFindResultCookies, LeaderboardFindResult_t> m_callResultFindLeaderboard;
};

}  // namespace steamworks_gc

#endif  // GC_LEADERBOARDS_FIND_RESULT_COOKIES_H
