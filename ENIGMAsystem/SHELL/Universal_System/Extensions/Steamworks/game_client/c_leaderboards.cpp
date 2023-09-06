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

#include "c_leaderboards.h"

#include "utils/c_leaderboards_find_result_cookies.h"
#include "utils/c_leaderboards_score_downloaded_cookies.h"
#include "utils/c_leaderboards_score_uploaded_cookies.h"

namespace steamworks {

////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////

bool c_leaderboards_remove_all_lambda(const c_leaderboards_cookies *cookie) {
  delete cookie;
  return true;
}

bool c_leaderboards_remove_if_done_lambda(const c_leaderboards_cookies *cookie) {
  bool is_done{cookie->is_done()};
  if (is_done) delete cookie;
  return is_done;
}

////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////

c_leaderboards::c_leaderboards()
    : current_leaderboard_(INVALID_LEADERBOARD), number_of_leaderboard_entries_(0), loading_(false) {}

c_leaderboards::~c_leaderboards() { c_leaderboards::deallocate_all_c_leaderboards_cookies(); }

bool c_leaderboards::create_leaderboard(const int id, const std::string &leaderboard_name,
                                        const ELeaderboardSortMethod leaderboard_sort_method,
                                        const ELeaderboardDisplayType leaderboard_display_type) {
  deallocate_c_leaderboards_cookies_if_done();

  if (c_leaderboards::current_leaderboard_ != INVALID_LEADERBOARD) {
    if (std::string(SteamUserStats()->GetLeaderboardName(c_leaderboards::current_leaderboard_)) == leaderboard_name)
      return true;

    c_leaderboards::current_leaderboard_ = INVALID_LEADERBOARD;
  }

  SteamAPICall_t steam_api_call{0};

  steam_api_call = SteamUserStats()->FindOrCreateLeaderboard(leaderboard_name.c_str(), leaderboard_sort_method,
                                                             leaderboard_display_type);

  if (steam_api_call != 0) {
    c_leaderboards_cookies *c_leaderboards_find_result =
        new c_leaderboards_find_result_cookies(id, this, steam_api_call);

    c_leaderboards_cookies_.push_back(c_leaderboards_find_result);
  } else {
    // TODO: Write more descriptive error message.
    DEBUG_MESSAGE("Calling FindOrCreateLeaderboard() failed for some reason.", M_ERROR);
    return false;
  }

  return true;
}

void c_leaderboards::find_leaderboard(const int id, const std::string &leaderboard_name) {
  deallocate_c_leaderboards_cookies_if_done();

  if (c_leaderboards::current_leaderboard_ != INVALID_LEADERBOARD) {
    if (std::string(SteamUserStats()->GetLeaderboardName(c_leaderboards::current_leaderboard_)) == leaderboard_name)
      return;

    c_leaderboards::current_leaderboard_ = INVALID_LEADERBOARD;
  }

  SteamAPICall_t steam_api_call{0};

  steam_api_call = SteamUserStats()->FindLeaderboard(leaderboard_name.c_str());

  if (steam_api_call != 0) {
    c_leaderboards_cookies *c_leaderboards_find_result =
        new c_leaderboards_find_result_cookies(id, this, steam_api_call);

    c_leaderboards_cookies_.push_back(c_leaderboards_find_result);
  } else {
    // TODO: Write more descriptive error message.
    DEBUG_MESSAGE("Calling FindOrCreateLeaderboard() failed for some reason.", M_ERROR);
  }
}

bool c_leaderboards::upload_score(const int id, const int score,
                                  const ELeaderboardUploadScoreMethod leaderboard_upload_score_method) {
  deallocate_c_leaderboards_cookies_if_done();

  if (c_leaderboards::current_leaderboard_ == INVALID_LEADERBOARD) return false;

  SteamAPICall_t steam_api_call = SteamUserStats()->UploadLeaderboardScore(
      c_leaderboards::current_leaderboard_, leaderboard_upload_score_method, score, NULL, 0);

  c_leaderboards_cookies *c_leaderboards_score_uploaded =
      new c_leaderboards_score_uploaded_cookies(id, this, steam_api_call);

  c_leaderboards_cookies_.push_back(c_leaderboards_score_uploaded);

  return true;
}

bool c_leaderboards::download_scores(const int id, const ELeaderboardDataRequest leaderboard_data_request,
                                     const int range_start, const int range_end) {
  deallocate_c_leaderboards_cookies_if_done();

  if (c_leaderboards::current_leaderboard_ == INVALID_LEADERBOARD) return false;

  SteamAPICall_t steam_api_call = SteamUserStats()->DownloadLeaderboardEntries(
      c_leaderboards::current_leaderboard_, leaderboard_data_request, range_start, range_end);

  c_leaderboards_cookies *c_leaderboards_score_downloaded =
      new c_leaderboards_score_downloaded_cookies(id, this, steam_api_call);

  c_leaderboards_cookies_.push_back(c_leaderboards_score_downloaded);

  return true;
}

std::string c_leaderboards::get_leaderboard_name(const SteamLeaderboard_t leaderboard) {
  return std::string(SteamUserStats()->GetLeaderboardName(leaderboard));
}

void c_leaderboards::set_current_leaderboard(const SteamLeaderboard_t leaderboard) {
  c_leaderboards::current_leaderboard_ = leaderboard;
}

void c_leaderboards::set_loading(const bool loading) { c_leaderboards::loading_ = loading; }

////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////

void c_leaderboards::deallocate_all_c_leaderboards_cookies() {
  c_leaderboards::c_leaderboards_cookies_.erase(
      std::remove_if(c_leaderboards::c_leaderboards_cookies_.begin(), c_leaderboards::c_leaderboards_cookies_.end(),
                     c_leaderboards_remove_all_lambda),
      c_leaderboards::c_leaderboards_cookies_.end());
}

void c_leaderboards::deallocate_c_leaderboards_cookies_if_done() {
  c_leaderboards::c_leaderboards_cookies_.erase(
      std::remove_if(c_leaderboards::c_leaderboards_cookies_.begin(), c_leaderboards::c_leaderboards_cookies_.end(),
                     c_leaderboards_remove_if_done_lambda),
      c_leaderboards::c_leaderboards_cookies_.end());
}

}  // namespace steamworks
