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

#include "../leaderboards.h"

namespace steamworks {

////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////

c_leaderboards::c_leaderboards()
    : current_leaderboard_(NULL),
      number_of_leaderboard_entries_(0),
      loading_(false)
// last_leaderboard_found_id_(-1),
// last_score_downloaded_id_(-1),
// last_score_uploaded_id_(-1)
{}

void c_leaderboards::find_leaderboard(const std::string& leaderboard_name,
                                      const ELeaderboardSortMethod leaderboard_sort_method,
                                      const ELeaderboardDisplayType leaderboard_display_type) {
  if (c_leaderboards::loading_) return;

  if (c_leaderboards::current_leaderboard_ != NULL) {
    if (SteamUserStats()->GetLeaderboardName(c_leaderboards::current_leaderboard_) == leaderboard_name) return;
  }

  c_leaderboards::current_leaderboard_ = NULL;

  SteamAPICall_t steam_api_call = SteamUserStats()->FindOrCreateLeaderboard(
      leaderboard_name.c_str(), leaderboard_sort_method, leaderboard_display_type);

  if (steam_api_call != 0) {
    c_leaderboards::m_callResultFindLeaderboard.Set(steam_api_call, this, &c_leaderboards::on_find_leaderboard);
    c_leaderboards::loading_ = true;
  }
}

void c_leaderboards::on_find_leaderboard(LeaderboardFindResult_t* pFindLeaderboardResult, bool bIOFailure) {
  if (!pFindLeaderboardResult->m_bLeaderboardFound || bIOFailure) {
    DEBUG_MESSAGE("Failed to find or create leaderboard.", M_ERROR);
    return;
  }

  c_leaderboards::current_leaderboard_ = pFindLeaderboardResult->m_hSteamLeaderboard;

  DEBUG_MESSAGE("Calling FindOrCreateLeaderboard succeeded.", M_INFO);

  // Success? Let's save it.
  // enigma::leaderboard_found_array.get() = pFindLeaderboardResult->m_hSteamLeaderboard;

  // Done? We are ready to accept new requests.
  c_leaderboards::loading_ = false;

  enigma::push_create_leaderboard_steam_async_event(pFindLeaderboardResult);
}

bool c_leaderboards::upload_score(const int score,
                                  const ELeaderboardUploadScoreMethod leaderboard_upload_score_method) {
  // while (c_leaderboards::loading_)
  //   ;  // Wait for the callback of FindOrCreateLeaderboard to be invoked

  if (NULL == c_leaderboards::current_leaderboard_ || loading_) return -false;

  SteamAPICall_t steam_api_call = SteamUserStats()->UploadLeaderboardScore(
      c_leaderboards::current_leaderboard_, leaderboard_upload_score_method, score, NULL, 0);
  c_leaderboards::m_SteamCallResultUploadScore.Set(steam_api_call, this, &c_leaderboards::on_upload_score);

  return true;
}

void c_leaderboards::on_upload_score(LeaderboardScoreUploaded_t* pScoreUploadedResult, bool bIOFailure) {
  if (!pScoreUploadedResult->m_bSuccess || bIOFailure) {
    DEBUG_MESSAGE("Failed to upload score to leaderboard.", M_ERROR);
    return;
  }

  // Success? Let's save it.
  // enigma::scores_array.get() = true;

  // Done? We are ready to accept new requests.
  c_leaderboards::loading_ = false;

  enigma::push_leaderboard_upload_steam_async_event(pScoreUploadedResult);
}

bool c_leaderboards::download_scores(const ELeaderboardDataRequest leaderboard_data_request, const int range_start,
                                     const int range_end) {
  // while (c_leaderboards::loading_)
  //   ;  // Wait for the callback of FindOrCreateLeaderboard to be invoked

  if (NULL == c_leaderboards::current_leaderboard_ || loading_) return false;

  c_leaderboards::loading_ = true;

  SteamAPICall_t steam_api_call = SteamUserStats()->DownloadLeaderboardEntries(
      c_leaderboards::current_leaderboard_, leaderboard_data_request, range_start, range_end);
  c_leaderboards::m_SteamCallResultDownloadScores.Set(steam_api_call, this, &c_leaderboards::on_download_scores);

  return true;
}

void c_leaderboards::on_download_scores(LeaderboardScoresDownloaded_t* pLeaderboardScoresDownloaded, bool bIOFailure) {
  if (bIOFailure) {
    DEBUG_MESSAGE("Failed to download scores from leaderboard.", M_ERROR);
    c_leaderboards::loading_ = false;
    return;
  }

  DEBUG_MESSAGE("Downloaded scores from leaderboard.", M_INFO);

  enigma::push_leaderboard_download_steam_async_event(pLeaderboardScoresDownloaded);
  
  // Entries are saved? We are ready to accept new requests.
  c_leaderboards::loading_ = false;
}

std::string c_leaderboards::get_leaderboard_name(const SteamLeaderboard_t leaderboard) {
  if (NULL == leaderboard) {
    DEBUG_MESSAGE("Calling get_leaderboard_name() failed. Make to pass a valid leaderboard handle.", M_ERROR);
    return "";
  };

  return std::string(SteamUserStats()->GetLeaderboardName(leaderboard));
}

}  // namespace steamworks
