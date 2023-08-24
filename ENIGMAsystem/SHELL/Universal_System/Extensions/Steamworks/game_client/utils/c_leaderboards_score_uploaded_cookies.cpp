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

#include "c_leaderboards_score_uploaded_cookies.h"

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

////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////

bool c_leaderboards_score_uploaded_cookies::is_done() const { return c_leaderboards_score_uploaded_cookies::is_done_; }

void c_leaderboards_score_uploaded_cookies::set_call_result(SteamAPICall_t steam_api_call) {
  c_leaderboards_score_uploaded_cookies::m_SteamCallResultUploadScore.Set(
      steam_api_call, this, &c_leaderboards_score_uploaded_cookies::on_upload_score);
}

void c_leaderboards_score_uploaded_cookies::on_upload_score(LeaderboardScoreUploaded_t* pScoreUploadedResult,
                                                            bool bIOFailure) {
  if (!pScoreUploadedResult->m_bSuccess || bIOFailure) {
    if ((enigma::number_of_successful_upload_requests + 1) % 10 == 0) {
      DEBUG_MESSAGE(
          "Did you create 10 upload requests in less than 10 minutes? Well, the upload rate is limited to "
          "10 upload requests per 10 minutes so you may want to wait before another request.",
          M_WARNING);
      enigma::upload_rate_limit_exceeded = true;
    }
    else DEBUG_MESSAGE("Failed to upload score to leaderboard.", M_ERROR);
    // c_leaderboards_score_uploaded_cookies::c_leaderboards_->set_loading(false);
    c_leaderboards_score_uploaded_cookies::is_done_ = true;
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
