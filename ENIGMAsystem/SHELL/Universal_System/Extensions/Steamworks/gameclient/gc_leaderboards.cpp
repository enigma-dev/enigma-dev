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

#include "gc_leaderboards.h"

#include "utils/gc_leaderboards_find_result_cookies.h"
#include "utils/gc_leaderboards_score_downloaded_cookies.h"
#include "utils/gc_leaderboards_score_uploaded_cookies.h"

namespace steamworks_gc {

////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////

bool leaderboards_remove_all_lambda(const GCLeaderboardsCookies* cookie) {
  delete cookie;
  cookie = nullptr;
  return true;
}

bool leaderboards_remove_if_done_lambda(const GCLeaderboardsCookies* cookie) {
  bool is_done{cookie->is_done()};
  if (is_done) {
    delete cookie;
    cookie = nullptr;
  }
  return is_done;
}

////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////

GCLeaderboards::GCLeaderboards()
    : current_leaderboard_(INVALID_LEADERBOARD), number_of_leaderboard_entries_(0), loading_(false) {}

GCLeaderboards::~GCLeaderboards() { GCLeaderboards::deallocate_all_leaderboards_cookies(); }

bool GCLeaderboards::create_leaderboard(const int& id, const std::string& leaderboard_name,
                                        const ELeaderboardSortMethod& leaderboard_sort_method,
                                        const ELeaderboardDisplayType& leaderboard_display_type) {
  deallocate_leaderboards_cookies_if_done();

  if (GCLeaderboards::current_leaderboard_ != INVALID_LEADERBOARD) {
    std::string leaderboard_name_buffer;

    if (!GCLeaderboards::get_leaderboard_name(leaderboard_name_buffer, GCLeaderboards::current_leaderboard_)) {
      return false;
    }

    if (leaderboard_name_buffer == leaderboard_name) return true;

    GCLeaderboards::current_leaderboard_ = INVALID_LEADERBOARD;
  }

  SteamAPICall_t steam_api_call{0};

  steam_api_call = steamworks_b::SteamBinder::ISteamUserStats_FindOrCreateLeaderboard(
      steamworks_b::SteamBinder::SteamUserStats_vXXX(), leaderboard_name.c_str(), leaderboard_sort_method,
      leaderboard_display_type);

  if (steam_api_call != 0) {
    GCLeaderboardsCookies* gc_leaderboards_find_result{new GCLeaderboardsFindResultCookies(id, this, steam_api_call)};

    gc_leaderboards_cookies_.push_back(gc_leaderboards_find_result);
  } else {
    // TODO: Write more descriptive error message.
    DEBUG_MESSAGE("Calling FindOrCreateLeaderboard() failed for some reason.", M_ERROR);
    return false;
  }

  return true;
}

void GCLeaderboards::find_leaderboard(const int& id, const std::string& leaderboard_name) {
  deallocate_leaderboards_cookies_if_done();

  if (GCLeaderboards::current_leaderboard_ != INVALID_LEADERBOARD) {
    std::string leaderboard_name_buffer;

    if (!GCLeaderboards::get_leaderboard_name(leaderboard_name_buffer, GCLeaderboards::current_leaderboard_)) {
      return;
    }

    if (leaderboard_name_buffer == leaderboard_name) return;

    GCLeaderboards::current_leaderboard_ = INVALID_LEADERBOARD;
  }

  SteamAPICall_t steam_api_call{0};

  steam_api_call = steamworks_b::SteamBinder::ISteamUserStats_FindLeaderboard(
      steamworks_b::SteamBinder::SteamUserStats_vXXX(), leaderboard_name.c_str());

  if (steam_api_call != 0) {
    GCLeaderboardsCookies* gc_leaderboards_find_result{new GCLeaderboardsFindResultCookies(id, this, steam_api_call)};

    gc_leaderboards_cookies_.push_back(gc_leaderboards_find_result);
  } else {
    // TODO: Write more descriptive error message.
    DEBUG_MESSAGE("Calling FindOrCreateLeaderboard() failed for some reason.", M_ERROR);
  }
}

bool GCLeaderboards::upload_score(const int& id, const int& score,
                                  const ELeaderboardUploadScoreMethod& leaderboard_upload_score_method) {
  deallocate_leaderboards_cookies_if_done();

  if (GCLeaderboards::current_leaderboard_ == INVALID_LEADERBOARD) return false;

  SteamAPICall_t steam_api_call{steamworks_b::SteamBinder::ISteamUserStats_UploadLeaderboardScore(
      steamworks_b::SteamBinder::SteamUserStats_vXXX(), GCLeaderboards::current_leaderboard_,
      leaderboard_upload_score_method, score, nullptr, 0)};

  GCLeaderboardsCookies* gc_leaderboards_score_uploaded{new GCLeaderboardsScoreUploadedCookies(id, steam_api_call)};

  gc_leaderboards_cookies_.push_back(gc_leaderboards_score_uploaded);

  return true;
}

bool GCLeaderboards::download_scores(const int& id, const ELeaderboardDataRequest& leaderboard_data_request,
                                     const int& range_start, const int& range_end) {
  deallocate_leaderboards_cookies_if_done();

  if (GCLeaderboards::current_leaderboard_ == INVALID_LEADERBOARD) return false;

  SteamAPICall_t steam_api_call{steamworks_b::SteamBinder::ISteamUserStats_DownloadLeaderboardEntries(
      steamworks_b::SteamBinder::SteamUserStats_vXXX(), GCLeaderboards::current_leaderboard_, leaderboard_data_request,
      range_start, range_end)};

  GCLeaderboardsCookies* gc_leaderboards_score_downloaded{new GCLeaderboardsScoreDownloadedCookies(id, steam_api_call)};

  gc_leaderboards_cookies_.push_back(gc_leaderboards_score_downloaded);

  return true;
}

void GCLeaderboards::set_current_leaderboard(const SteamLeaderboard_t& leaderboard) {
  GCLeaderboards::current_leaderboard_ = leaderboard;
}

void GCLeaderboards::set_loading(const bool& loading) { GCLeaderboards::loading_ = loading; }

////////////////////////////////////////////////////////
// Static fields & functions (AKA Wrapper functions)
////////////////////////////////////////////////////////

bool GCLeaderboards::get_leaderboard_name(std::string& buffer, const SteamLeaderboard_t& leaderboard) {
  buffer = std::string(steamworks_b::SteamBinder::ISteamUserStats_GetLeaderboardName(
      steamworks_b::SteamBinder::SteamUserStats_vXXX(), leaderboard));
  return true;
}

////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////

void GCLeaderboards::deallocate_all_leaderboards_cookies() {
  GCLeaderboards::gc_leaderboards_cookies_.erase(
      std::remove_if(GCLeaderboards::gc_leaderboards_cookies_.begin(), GCLeaderboards::gc_leaderboards_cookies_.end(),
                     leaderboards_remove_all_lambda),
      GCLeaderboards::gc_leaderboards_cookies_.end());
}

void GCLeaderboards::deallocate_leaderboards_cookies_if_done() {
  GCLeaderboards::gc_leaderboards_cookies_.erase(
      std::remove_if(GCLeaderboards::gc_leaderboards_cookies_.begin(), GCLeaderboards::gc_leaderboards_cookies_.end(),
                     leaderboards_remove_if_done_lambda),
      GCLeaderboards::gc_leaderboards_cookies_.end());
}

}  // namespace steamworks_gc
