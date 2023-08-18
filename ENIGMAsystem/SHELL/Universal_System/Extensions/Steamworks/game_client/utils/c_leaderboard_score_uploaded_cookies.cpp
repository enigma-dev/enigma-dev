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

#include "c_leaderboard_score_uploaded_cookies.h"

#include "../../leaderboards.h"
#include "../c_leaderboards.h"

namespace steamworks {

////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////

c_leaderboards_score_uploaded_cookies::c_leaderboards_score_uploaded_cookies(int id, c_leaderboards* c_leaderboards,
                                                                             SteamAPICall_t steam_api_call)
    : id_(id), c_leaderboards_(c_leaderboards), is_done_(false) {
  c_leaderboards_score_uploaded_cookies::set_call_result(steam_api_call);
}

bool c_leaderboards_score_uploaded_cookies::is_done() const { return c_leaderboards_score_uploaded_cookies::is_done_; }

////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////

void c_leaderboards_score_uploaded_cookies::set_call_result(SteamAPICall_t steam_api_call) {
  c_leaderboards_score_uploaded_cookies::m_SteamCallResultUploadScore.Set(
      steam_api_call, this, &c_leaderboards_score_uploaded_cookies::on_upload_score);
}

void c_leaderboards_score_uploaded_cookies::on_upload_score(LeaderboardScoreUploaded_t* pScoreUploadedResult,
                                                            bool bIOFailure) {
  if (!pScoreUploadedResult->m_bSuccess || bIOFailure) {
    DEBUG_MESSAGE("Failed to upload score to leaderboard.", M_ERROR);
    c_leaderboards_score_uploaded_cookies::c_leaderboards_->set_loading(false);
    return;
  }

  // Success? Let's save it.
  // enigma::scores_array.get(c_leaderboards_score_uploaded_cookies::id_) = true;

  // Done? We are ready to accept new requests.
  // c_leaderboards_score_uploaded_cookies::c_leaderboards_->set_loading(false);

  enigma::push_leaderboard_upload_steam_async_event(c_leaderboards_score_uploaded_cookies::id_, pScoreUploadedResult);

  c_leaderboards_score_uploaded_cookies::is_done_ = true;
}

}  // namespace steamworks
