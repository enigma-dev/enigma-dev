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

#include "c_leaderboards_find_result_cookies.h"

#include "../../leaderboards.h"
#include "../c_leaderboards.h"

namespace steamworks {

////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////

c_leaderboards_find_result_cookies::c_leaderboards_find_result_cookies(int id, c_leaderboards* c_leaderboards,
                                                                       SteamAPICall_t steam_api_call)
    : id_(id), c_leaderboards_(c_leaderboards), is_done_(false) {
  c_leaderboards_find_result_cookies::set_call_result(steam_api_call);
}

bool c_leaderboards_find_result_cookies::is_done() const { return c_leaderboards_find_result_cookies::is_done_; }

////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////

void c_leaderboards_find_result_cookies::set_call_result(SteamAPICall_t steam_api_call) {
  c_leaderboards_find_result_cookies::m_callResultFindLeaderboard.Set(
      steam_api_call, this, &c_leaderboards_find_result_cookies::on_find_leaderboard);
}

void c_leaderboards_find_result_cookies::on_find_leaderboard(LeaderboardFindResult_t* pFindLeaderboardResult,
                                                             bool bIOFailure) {
  if (!pFindLeaderboardResult->m_bLeaderboardFound || bIOFailure) {
    DEBUG_MESSAGE("Failed to find or create leaderboard.", M_ERROR);
    c_leaderboards_find_result_cookies::c_leaderboards_->set_loading(false);
    return;
  }

  c_leaderboards_find_result_cookies::c_leaderboards_->set_current_leaderboard(
      pFindLeaderboardResult->m_hSteamLeaderboard);

  DEBUG_MESSAGE("Calling FindOrCreateLeaderboard succeeded.", M_INFO);

  // Success? Let's save it.
  // enigma::leaderboards_array.get(c_leaderboards_find_result_cookies::id_) =
  //     &pFindLeaderboardResult->m_hSteamLeaderboard;

  // Done? We are ready to accept new requests.
  // c_leaderboards_find_result_cookies::c_leaderboards_->set_loading(false);
  
  enigma::push_create_leaderboard_steam_async_event(c_leaderboards_find_result_cookies::id_, pFindLeaderboardResult);

  c_leaderboards_find_result_cookies::is_done_ = true;
}

}  // namespace steamworks
