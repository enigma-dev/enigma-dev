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

#include "gc_leaderboards.h"

#include "utils/gc_leaderboards_find_result_cookies.h"
#include "utils/gc_leaderboards_score_downloaded_cookies.h"
#include "utils/gc_leaderboards_score_uploaded_cookies.h"

namespace steamworks_gc {

////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////

bool leaderboards_remove_all_lambda(const GCLeaderboardsCookies *cookie) {
  delete cookie;
  return true;
}

bool leaderboards_remove_if_done_lambda(const GCLeaderboardsCookies *cookie) {
  bool is_done{cookie->is_done()};
  if (is_done) delete cookie;
  return is_done;
}

////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////

GCLeaderboards::GCLeaderboards()
    : current_leaderboard_(INVALID_LEADERBOARD), number_of_leaderboard_entries_(0), loading_(false) {}

GCLeaderboards::~GCLeaderboards() { GCLeaderboards::deallocate_all_leaderboards_cookies(); }

bool GCLeaderboards::create_leaderboard(const int id, const std::string &leaderboard_name,
                                        const GCLeaderboardSortMethod gc_leaderboard_sort_method,
                                        const GCLeaderboardDisplayType gc_leaderboard_display_type) {
  deallocate_leaderboards_cookies_if_done();

  if (GCLeaderboards::current_leaderboard_ != INVALID_LEADERBOARD) {
    if (std::string(SteamUserStats()->GetLeaderboardName(GCLeaderboards::current_leaderboard_)) == leaderboard_name)
      return true;

    GCLeaderboards::current_leaderboard_ = INVALID_LEADERBOARD;
  }

#ifndef ENIGMA_STEAMWORKS_API_MOCK
  ELeaderboardSortMethod leaderboard_sort_method{ELeaderboardSortMethod::k_ELeaderboardSortMethodNone};

  switch (gc_leaderboard_sort_method) {
    case GCLeaderboardSortMethod::k_GCLeaderboardSortMethod_Ascending:
      leaderboard_sort_method = ELeaderboardSortMethod::k_ELeaderboardSortMethodAscending;
      break;
    case GCLeaderboardSortMethod::k_GCLeaderboardSortMethod_Descending:
      leaderboard_sort_method = ELeaderboardSortMethod::k_ELeaderboardSortMethodDescending;
      break;
    default:
      leaderboard_sort_method = ELeaderboardSortMethod::k_ELeaderboardSortMethodNone;
      break;
  }

  ELeaderboardDisplayType leaderboard_display_type{ELeaderboardDisplayType::k_ELeaderboardDisplayTypeNone};

  switch (gc_leaderboard_display_type) {
    case GCLeaderboardDisplayType::k_GCLeaderboardDisplayType_Numeric:
      leaderboard_display_type = ELeaderboardDisplayType::k_ELeaderboardDisplayTypeNumeric;
      break;
    case GCLeaderboardDisplayType::k_GCLeaderboardDisplayType_TimeSeconds:
      leaderboard_display_type = ELeaderboardDisplayType::k_ELeaderboardDisplayTypeTimeSeconds;
      break;
    case GCLeaderboardDisplayType::k_GCLeaderboardDisplayType_TimeMilliSeconds:
      leaderboard_display_type = ELeaderboardDisplayType::k_ELeaderboardDisplayTypeTimeMilliSeconds;
      break;
    default:
      leaderboard_display_type = ELeaderboardDisplayType::k_ELeaderboardDisplayTypeNone;
      break;
  }

  SteamAPICall_t steam_api_call{0};

  steam_api_call = SteamUserStats()->FindOrCreateLeaderboard(leaderboard_name.c_str(), leaderboard_sort_method,
                                                             leaderboard_display_type);
#else
  SteamAPICall_t steam_api_call{0};

  steam_api_call = SteamUserStats()->FindOrCreateLeaderboard(
      leaderboard_name.c_str(), (unsigned)gc_leaderboard_sort_method, (unsigned)gc_leaderboard_display_type);
#endif  // ENIGMA_STEAMWORKS_API_MOCK

  if (steam_api_call != 0) {
    GCLeaderboardsCookies *gc_leaderboards_find_result = new GCLeaderboardsFindResultCookies(id, this, steam_api_call);

    gc_leaderboards_cookies_.push_back(gc_leaderboards_find_result);
  } else {
    // TODO: Write more descriptive error message.
    DEBUG_MESSAGE("Calling FindOrCreateLeaderboard() failed for some reason.", M_ERROR);
    return false;
  }

  return true;
}

void GCLeaderboards::find_leaderboard(const int id, const std::string &leaderboard_name) {
  deallocate_leaderboards_cookies_if_done();

  if (GCLeaderboards::current_leaderboard_ != INVALID_LEADERBOARD) {
    if (std::string(SteamUserStats()->GetLeaderboardName(GCLeaderboards::current_leaderboard_)) == leaderboard_name)
      return;

    GCLeaderboards::current_leaderboard_ = INVALID_LEADERBOARD;
  }

  SteamAPICall_t steam_api_call{0};

  steam_api_call = SteamUserStats()->FindLeaderboard(leaderboard_name.c_str());

  if (steam_api_call != 0) {
    GCLeaderboardsCookies *gc_leaderboards_find_result = new GCLeaderboardsFindResultCookies(id, this, steam_api_call);

    gc_leaderboards_cookies_.push_back(gc_leaderboards_find_result);
  } else {
    // TODO: Write more descriptive error message.
    DEBUG_MESSAGE("Calling FindOrCreateLeaderboard() failed for some reason.", M_ERROR);
  }
}

bool GCLeaderboards::upload_score(const int id, const int score,
                                  const GCLeaderboardUploadScoreMethod gc_leaderboard_upload_score_method) {
  deallocate_leaderboards_cookies_if_done();

  if (GCLeaderboards::current_leaderboard_ == INVALID_LEADERBOARD) return false;

#ifndef ENIGMA_STEAMWORKS_API_MOCK
  ELeaderboardUploadScoreMethod leaderboard_upload_score_method{
      ELeaderboardUploadScoreMethod::k_ELeaderboardUploadScoreMethodNone};

  switch (gc_leaderboard_upload_score_method) {
    case GCLeaderboardUploadScoreMethod::k_GCLeaderboardUploadScoreMethod_KeepBest:
      leaderboard_upload_score_method = ELeaderboardUploadScoreMethod::k_ELeaderboardUploadScoreMethodKeepBest;
      break;
    case GCLeaderboardUploadScoreMethod::k_GCLeaderboardUploadScoreMethod_ForceUpdate:
      leaderboard_upload_score_method = ELeaderboardUploadScoreMethod::k_ELeaderboardUploadScoreMethodForceUpdate;
      break;
    default:
      leaderboard_upload_score_method = ELeaderboardUploadScoreMethod::k_ELeaderboardUploadScoreMethodNone;
      break;
  }

  SteamAPICall_t steam_api_call = SteamUserStats()->UploadLeaderboardScore(
      GCLeaderboards::current_leaderboard_, leaderboard_upload_score_method, score, nullptr, 0);
#else
  SteamAPICall_t steam_api_call = SteamUserStats()->UploadLeaderboardScore(
      GCLeaderboards::current_leaderboard_, (unsigned)gc_leaderboard_upload_score_method, score, nullptr, 0);
#endif  // ENIGMA_STEAMWORKS_API_MOCK

  GCLeaderboardsCookies *gc_leaderboards_score_uploaded =
      new GCLeaderboardsScoreUploadedCookies(id, this, steam_api_call);

  gc_leaderboards_cookies_.push_back(gc_leaderboards_score_uploaded);

  return true;
}

bool GCLeaderboards::download_scores(const int id, const GCLeaderboardDataRequest gc_leaderboard_data_request,
                                     const int range_start, const int range_end) {
  deallocate_leaderboards_cookies_if_done();

  if (GCLeaderboards::current_leaderboard_ == INVALID_LEADERBOARD) return false;

#ifndef ENIGMA_STEAMWORKS_API_MOCK
  ELeaderboardDataRequest leaderboard_data_request{ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobal};

  switch (gc_leaderboard_data_request) {
    case GCLeaderboardDataRequest::k_GCLeaderboardDataRequest_GlobalAroundUser:
      leaderboard_data_request = ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobalAroundUser;
      break;
    case GCLeaderboardDataRequest::k_GCLeaderboardDataRequest_Friends:
      leaderboard_data_request = ELeaderboardDataRequest::k_ELeaderboardDataRequestFriends;
      break;
    case GCLeaderboardDataRequest::k_GCLeaderboardDataRequest_Users:
      leaderboard_data_request = ELeaderboardDataRequest::k_ELeaderboardDataRequestUsers;
      break;
    default:
      leaderboard_data_request = ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobal;
      break;
  }

  SteamAPICall_t steam_api_call = SteamUserStats()->DownloadLeaderboardEntries(
      GCLeaderboards::current_leaderboard_, leaderboard_data_request, range_start, range_end);
#else
  SteamAPICall_t steam_api_call = SteamUserStats()->DownloadLeaderboardEntries(
      GCLeaderboards::current_leaderboard_, (unsigned)gc_leaderboard_data_request, range_start, range_end);
#endif  // ENIGMA_STEAMWORKS_API_MOCK

  GCLeaderboardsCookies *gc_leaderboards_score_downloaded =
      new GCLeaderboardsScoreDownloadedCookies(id, this, steam_api_call);

  gc_leaderboards_cookies_.push_back(gc_leaderboards_score_downloaded);

  return true;
}

void GCLeaderboards::set_current_leaderboard(const SteamLeaderboard_t leaderboard) {
  GCLeaderboards::current_leaderboard_ = leaderboard;
}

void GCLeaderboards::set_loading(const bool loading) { GCLeaderboards::loading_ = loading; }

////////////////////////////////////////////////////////
// Static fields & functions (AKA Wrapper functions)
////////////////////////////////////////////////////////

std::string GCLeaderboards::get_leaderboard_name(const SteamLeaderboard_t leaderboard) {
  return std::string(SteamUserStats()->GetLeaderboardName(leaderboard));
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
