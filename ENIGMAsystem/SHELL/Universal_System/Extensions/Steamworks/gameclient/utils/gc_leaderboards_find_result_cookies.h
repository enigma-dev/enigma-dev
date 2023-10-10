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
 * @brief Whenever a @c GCLeaderboards::create_leaderboard() or @c GCLeaderboards::find_leaderboard() is called, a 
 *        new object from this class will be instantiated with a unique id. This id will be used when the call 
 *        result arrive. The call result setting operation is moved to this class from @c GCLeaderboards class, 
 *        which means in order to setup a call result, you need to instantiate an onject from this class.
 * 
 */

#ifndef GC_LEADERBOARDS_FIND_RESULT_COOKIES_H
#define GC_LEADERBOARDS_FIND_RESULT_COOKIES_H

#include "gc_leaderboards_cookies.h"

namespace steamworks_gc {

class GCLeaderboards;

class GCLeaderboardsFindResultCookies : public GCLeaderboardsCookies {
 public:
  /**
   * @brief GCLeaderboardsFindResultCookies constructor.
   * 
   * @param id the id that will be attached to the callresult. This id is the output of 
   *           AssetArray @c add() function.
   * 
   * @param gc_leaderboards 
   * @param steam_api_call 
   */
  GCLeaderboardsFindResultCookies(const int& id, GCLeaderboards* gc_leaderboards, SteamAPICall_t& steam_api_call);

  // GCLeaderboardsFindResultCookies destructor.
  ~GCLeaderboardsFindResultCookies() = default;

 private:
  /**
   * @brief The unique id that will be attached to the callresult. This id is part of the payload 
   *        that will be attached to the leaderboard find results. This id must be returned from 
   *        AssetArray @c add() function.
   * 
   */
  int id_;

  // Pointer to leaderboards client.
  GCLeaderboards* gc_leaderboards_;

  /**
   * @brief This flag will be set to true when the callresult is received. This flag will be used 
   *        to determine if we can destroy this object or not.
   * 
   * @see @c GCLeaderboardsFindResultCookies::is_done() function.
   * 
   */
  bool is_done_;

  /**
   * @brief is the callresult here and done?
   * 
   * @return true 
   * @return false 
   */
  bool is_done() const override;

  /**
   * @brief Registers a callresult.
   * 
   * @param steam_api_call 
   */
  void set_call_result(SteamAPICall_t& steam_api_call) override;

  /**
   * @brief This function will be called when the callresult is received.
   * 
   * @see @c GCLeaderboards::create_leaderboard() function.
   * @see @c GCLeaderboards::find_leaderboard() function.
   * @see https://partner.steamgames.com/doc/api/ISteamUserStats#LeaderboardFindResult_t for
   *      more information.
   * 
   * @param pFindLeaderboardResult 
   * @param bIOFailure 
   */
  void on_find_leaderboard(LeaderboardFindResult_t* pFindLeaderboardResult, bool bIOFailure);

  /**
   * @brief The callresult that will be used to find leaderboards.
   * 
   */
  CCallResult<GCLeaderboardsFindResultCookies, LeaderboardFindResult_t> m_callResultFindLeaderboard;
};

}  // namespace steamworks_gc

#endif  // GC_LEADERBOARDS_FIND_RESULT_COOKIES_H
