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

// TODO: Test the cleaning algorithm here.

namespace steamworks {

////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////

bool c_leaderboards_find_result_remove_all_lambda(
    const c_leaderboards_find_result_cookies *c_leaderboards_find_result) {
  delete c_leaderboards_find_result;
  return true;
}

bool c_leaderboards_score_uploaded_remove_all_lambda(
    const c_leaderboards_score_uploaded_cookies *c_leaderboards_score_uploaded) {
  delete c_leaderboards_score_uploaded;
  return true;
}

bool c_leaderboards_score_downloaded_remove_all_lambda(
    const c_leaderboards_score_downloaded_cookies *c_leaderboards_score_downloaded) {
  delete c_leaderboards_score_downloaded;
  return true;
}

bool c_leaderboards_find_result_remove_if_done_lambda(
    const c_leaderboards_find_result_cookies *c_leaderboards_find_result) {
  bool is_done{c_leaderboards_find_result->is_done()};
  if (is_done) delete c_leaderboards_find_result;
  return is_done;
}

bool c_leaderboards_score_uploaded_remove_if_done_lambda(
    const c_leaderboards_score_uploaded_cookies *c_leaderboards_score_uploaded) {
  bool is_done{c_leaderboards_score_uploaded->is_done()};
  if (is_done) delete c_leaderboards_score_uploaded;
  return is_done;
}

bool c_leaderboards_score_downloaded_remove_if_done_lambda(
    const c_leaderboards_score_downloaded_cookies *c_leaderboards_score_downloaded) {
  bool is_done{c_leaderboards_score_downloaded->is_done()};
  if (is_done) delete c_leaderboards_score_downloaded;
  return is_done;
}

////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////

c_leaderboards::c_leaderboards() : current_leaderboard_(NULL), number_of_leaderboard_entries_(0), loading_(false) {}

c_leaderboards::~c_leaderboards() {
  c_leaderboards::deallocate_all_c_leaderboards_find_result_cookies();
  c_leaderboards::deallocate_all_c_leaderboards_score_uploaded_cookies();
  c_leaderboards::deallocate_all_c_leaderboards_score_downloaded_cookies();
}

void c_leaderboards::find_leaderboard(const int id, const std::string &leaderboard_name,
                                      const ELeaderboardSortMethod leaderboard_sort_method,
                                      const ELeaderboardDisplayType leaderboard_display_type) {
  deallocate_c_leaderboards_find_result_cookies_if_done();

  if (c_leaderboards::current_leaderboard_ != NULL) {
    if (std::string(SteamUserStats()->GetLeaderboardName(*c_leaderboards::current_leaderboard_)) == leaderboard_name)
      return;
  }

  c_leaderboards::current_leaderboard_ = NULL;

  SteamAPICall_t steam_api_call{0};

  steam_api_call = SteamUserStats()->FindOrCreateLeaderboard(leaderboard_name.c_str(), leaderboard_sort_method,
                                                             leaderboard_display_type);

  if (steam_api_call != 0) {
    c_leaderboards_find_result_cookies *c_leaderboards_find_result =
        new c_leaderboards_find_result_cookies(id, this, steam_api_call);
    c_leaderboards_find_result_cookies_.push_back(c_leaderboards_find_result);
  } else {
    // TODO: Write more descriptive error message.
    DEBUG_MESSAGE("Calling FindOrCreateLeaderboard() failed for some reason.", M_ERROR);
  }
}

bool c_leaderboards::upload_score(const int id, const int score,
                                  const ELeaderboardUploadScoreMethod leaderboard_upload_score_method) {
  deallocate_c_leaderboards_score_uploaded_cookies_if_done();

  if (NULL == c_leaderboards::current_leaderboard_) return false;

  SteamAPICall_t steam_api_call = SteamUserStats()->UploadLeaderboardScore(
      *c_leaderboards::current_leaderboard_, leaderboard_upload_score_method, score, NULL, 0);

  c_leaderboards_score_uploaded_cookies *c_leaderboards_score_uploaded =
      new c_leaderboards_score_uploaded_cookies(id, this, steam_api_call);
  c_leaderboards_score_uploaded_cookies_.push_back(c_leaderboards_score_uploaded);

  return true;
}

bool c_leaderboards::download_scores(const int id, const ELeaderboardDataRequest leaderboard_data_request,
                                     const int range_start, const int range_end) {
  deallocate_c_leaderboards_score_downloaded_cookies_if_done();

  if (NULL == c_leaderboards::current_leaderboard_) return false;

  SteamAPICall_t steam_api_call = SteamUserStats()->DownloadLeaderboardEntries(
      *c_leaderboards::current_leaderboard_, leaderboard_data_request, range_start, range_end);

  c_leaderboards_score_downloaded_cookies *c_leaderboards_score_downloaded =
      new c_leaderboards_score_downloaded_cookies(id, this, steam_api_call);
  c_leaderboards_score_downloaded_cookies_.push_back(c_leaderboards_score_downloaded);

  return true;
}

std::string c_leaderboards::get_leaderboard_name(const SteamLeaderboard_t leaderboard) {
  if (NULL == leaderboard) {
    DEBUG_MESSAGE("Calling get_leaderboard_name() failed. Make sure to pass a valid leaderboard handle.", M_ERROR);
    return "";
  };

  return std::string(SteamUserStats()->GetLeaderboardName(leaderboard));
}

void c_leaderboards::set_current_leaderboard(const SteamLeaderboard_t leaderboard) {
  c_leaderboards::current_leaderboard_ = const_cast<SteamLeaderboard_t *>(&leaderboard);
}

void c_leaderboards::set_loading(const bool loading) { c_leaderboards::loading_ = loading; }

////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////

void c_leaderboards::deallocate_all_c_leaderboards_find_result_cookies() {
  c_leaderboards::c_leaderboards_find_result_cookies_.erase(
      std::remove_if(c_leaderboards::c_leaderboards_find_result_cookies_.begin(),
                     c_leaderboards::c_leaderboards_find_result_cookies_.end(),
                     c_leaderboards_find_result_remove_all_lambda),
      c_leaderboards::c_leaderboards_find_result_cookies_.end());
}

void c_leaderboards::deallocate_all_c_leaderboards_score_uploaded_cookies() {
  c_leaderboards::c_leaderboards_score_uploaded_cookies_.erase(
      std::remove_if(c_leaderboards::c_leaderboards_score_uploaded_cookies_.begin(),
                     c_leaderboards::c_leaderboards_score_uploaded_cookies_.end(),
                     c_leaderboards_score_uploaded_remove_all_lambda),
      c_leaderboards::c_leaderboards_score_uploaded_cookies_.end());
}

void c_leaderboards::deallocate_all_c_leaderboards_score_downloaded_cookies() {
  c_leaderboards::c_leaderboards_score_downloaded_cookies_.erase(
      std::remove_if(c_leaderboards::c_leaderboards_score_downloaded_cookies_.begin(),
                     c_leaderboards::c_leaderboards_score_downloaded_cookies_.end(),
                     c_leaderboards_score_downloaded_remove_all_lambda),
      c_leaderboards::c_leaderboards_score_downloaded_cookies_.end());
}

void c_leaderboards::deallocate_c_leaderboards_find_result_cookies_if_done() {
  c_leaderboards::c_leaderboards_find_result_cookies_.erase(
      std::remove_if(c_leaderboards::c_leaderboards_find_result_cookies_.begin(),
                     c_leaderboards::c_leaderboards_find_result_cookies_.end(),
                     c_leaderboards_find_result_remove_if_done_lambda),
      c_leaderboards::c_leaderboards_find_result_cookies_.end());
}

void c_leaderboards::deallocate_c_leaderboards_score_uploaded_cookies_if_done() {
  c_leaderboards::c_leaderboards_score_uploaded_cookies_.erase(
      std::remove_if(c_leaderboards::c_leaderboards_score_uploaded_cookies_.begin(),
                     c_leaderboards::c_leaderboards_score_uploaded_cookies_.end(),
                     c_leaderboards_score_uploaded_remove_if_done_lambda),
      c_leaderboards::c_leaderboards_score_uploaded_cookies_.end());
}

void c_leaderboards::deallocate_c_leaderboards_score_downloaded_cookies_if_done() {
  c_leaderboards::c_leaderboards_score_downloaded_cookies_.erase(
      std::remove_if(c_leaderboards::c_leaderboards_score_downloaded_cookies_.begin(),
                     c_leaderboards::c_leaderboards_score_downloaded_cookies_.end(),
                     c_leaderboards_score_downloaded_remove_if_done_lambda),
      c_leaderboards::c_leaderboards_score_downloaded_cookies_.end());
}

}  // namespace steamworks
