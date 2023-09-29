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

#include "gc_leaderboards_score_uploaded_cookies.h"

namespace steamworks_gc {

////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////

GCLeaderboardsScoreUploadedCookies::GCLeaderboardsScoreUploadedCookies(const int& id, SteamAPICall_t& steam_api_call)
    : id_(id), is_done_(false) {
  GCLeaderboardsScoreUploadedCookies::set_call_result(steam_api_call);
}

////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////

bool GCLeaderboardsScoreUploadedCookies::is_done() const { return GCLeaderboardsScoreUploadedCookies::is_done_; }

void GCLeaderboardsScoreUploadedCookies::set_call_result(SteamAPICall_t& steam_api_call) {
  GCLeaderboardsScoreUploadedCookies::m_SteamCallResultUploadScore.Set(
      steam_api_call, this, &GCLeaderboardsScoreUploadedCookies::on_upload_score);
}

void GCLeaderboardsScoreUploadedCookies::on_upload_score(LeaderboardScoreUploaded_t* pScoreUploadedResult,
                                                         bool bIOFailure) {
  if (!pScoreUploadedResult->m_bSuccess || bIOFailure) {
    if (enigma::number_of_successful_upload_requests % 10 == 0 && enigma::number_of_successful_upload_requests != 0) {
      DEBUG_MESSAGE(
          "Did you create 10 upload requests in less than 10 minutes? Well, the upload rate is limited to "
          "10 upload requests per 10 minutes so you may want to wait before another request.",
          M_WARNING);
      enigma::upload_rate_limit_exceeded = true;
    } else {
      DEBUG_MESSAGE("Failed to upload score to leaderboard.", M_ERROR);
    }
    GCLeaderboardsScoreUploadedCookies::is_done_ = true;
    return;
  }

  // TODO: Build failure when uncommenting this line.
  // TODO: Create a struct for this.
  // Success? Let's save it.
  // LGM and SOGs are failing because of this line.
  // enigma::scores_array.get(GCLeaderboardsScoreUploadedCookies::id_) = pScoreUploadedResult;

  enigma::push_leaderboard_upload_steam_async_event(GCLeaderboardsScoreUploadedCookies::id_, *pScoreUploadedResult);

  GCLeaderboardsScoreUploadedCookies::is_done_ = true;
}

}  // namespace steamworks_gc
