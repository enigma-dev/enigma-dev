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

#include "gc_leaderboards_find_result_cookies.h"

namespace steamworks_gc {

////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////

GCLeaderboardsFindResultCookies::GCLeaderboardsFindResultCookies(const int& id, GCLeaderboards* gc_leaderboards,
                                                                 SteamAPICall_t& steam_api_call)
    : id_(id), gc_leaderboards_(gc_leaderboards), is_done_(false) {
  GCLeaderboardsFindResultCookies::set_call_result(steam_api_call);
}

////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////

bool GCLeaderboardsFindResultCookies::is_done() const { return GCLeaderboardsFindResultCookies::is_done_; }

void GCLeaderboardsFindResultCookies::set_call_result(SteamAPICall_t& steam_api_call) {
  GCLeaderboardsFindResultCookies::m_callResultFindLeaderboard.Set(
      steam_api_call, this, &GCLeaderboardsFindResultCookies::on_find_leaderboard);
}

void GCLeaderboardsFindResultCookies::on_find_leaderboard(LeaderboardFindResult_t* pFindLeaderboardResult,
                                                          bool bIOFailure) {
  if (!pFindLeaderboardResult->m_bLeaderboardFound || bIOFailure) {
    DEBUG_MESSAGE("Failed to find or create leaderboard.", M_ERROR);
    GCLeaderboardsFindResultCookies::is_done_ = true;
    return;
  }

  GCLeaderboardsFindResultCookies::gc_leaderboards_->set_current_leaderboard(
      pFindLeaderboardResult->m_hSteamLeaderboard);

  DEBUG_MESSAGE("Calling FindOrCreateLeaderboard succeeded.", M_INFO);

  // Success? Let's save it.
  // SOGs are failing because of this line but LGM is not.
  // enigma::leaderboards_array.get(GCLeaderboardsFindResultCookies::id_) = pFindLeaderboardResult;

  enigma::push_create_leaderboard_steam_async_event(GCLeaderboardsFindResultCookies::id_, *pFindLeaderboardResult);

  GCLeaderboardsFindResultCookies::is_done_ = true;
}

}  // namespace steamworks_gc
