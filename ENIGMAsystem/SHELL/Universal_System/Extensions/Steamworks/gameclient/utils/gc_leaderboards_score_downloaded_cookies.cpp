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

#include "gc_leaderboards_score_downloaded_cookies.h"

#include <sstream>

namespace steamworks_gc {

////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////

// TODO: Add data key/value pair (if existed).
/*
  TODO: When using the concatenation operator '+' to concatenate strings instead of '<<', the string content blowed up
  because if something I don't understand.
*/
// TODO: The userID is not the same as the steamID.
// TODO: This function should be modified with https://github.com/enigma-dev/enigma-dev/pull/2358.
void get_leaderboard_entries(LeaderboardEntry_t leaderboard_entries[], unsigned leaderboard_entries_size,
                             std::stringstream& leaderboard_entries_buffer) {
  leaderboard_entries_buffer << '{';  // Entries object open bracket
  leaderboard_entries_buffer << '\"' << "entries" << '\"' << ':';
  leaderboard_entries_buffer << '[';  // Entries array open bracket

  std::string name_buffer;
  for (unsigned i{0}; i < leaderboard_entries_size; i++) {
    GameClient::get_steam_user_persona_name(name_buffer, leaderboard_entries[i].m_steamIDUser.ConvertToUint64());

    leaderboard_entries_buffer << '{';
    leaderboard_entries_buffer << '\"' << "name" << '\"' << ':';
    leaderboard_entries_buffer << '\"' << name_buffer << '\"' << ',';
    leaderboard_entries_buffer << '\"' << "score" << '\"' << ':';
    leaderboard_entries_buffer << std::to_string(leaderboard_entries[i].m_nScore) << ',';
    leaderboard_entries_buffer << '\"' << "rank" << '\"' << ':';
    leaderboard_entries_buffer << std::to_string(leaderboard_entries[i].m_nGlobalRank) << ',';
    leaderboard_entries_buffer << '\"' << "userID" << '\"' << ':';
    leaderboard_entries_buffer << '\"' << std::to_string(leaderboard_entries[i].m_steamIDUser.ConvertToUint64())
                               << '\"';
    leaderboard_entries_buffer << '}';

    // Add comma if not last entry
    if (i < leaderboard_entries_size - 1) leaderboard_entries_buffer << ',';
  }

  leaderboard_entries_buffer << ']';  // Entries array close bracket
  leaderboard_entries_buffer << '}';  // Entries object close bracket
}

////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////

GCLeaderboardsScoreDownloadedCookies::GCLeaderboardsScoreDownloadedCookies(const int& id,
                                                                           SteamAPICall_t& steam_api_call)
    : id_(id), is_done_(false) {
  GCLeaderboardsScoreDownloadedCookies::set_call_result(steam_api_call);
}

////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////

bool GCLeaderboardsScoreDownloadedCookies::is_done() const { return GCLeaderboardsScoreDownloadedCookies::is_done_; }

void GCLeaderboardsScoreDownloadedCookies::set_call_result(SteamAPICall_t& steam_api_call) {
  GCLeaderboardsScoreDownloadedCookies::m_SteamCallResultDownloadScores.Set(
      steam_api_call, this, &GCLeaderboardsScoreDownloadedCookies::on_download_scores);
}

void GCLeaderboardsScoreDownloadedCookies::on_download_scores(
    LeaderboardScoresDownloaded_t* pLeaderboardScoresDownloaded, bool bIOFailure) {
  if (bIOFailure) {
    DEBUG_MESSAGE("Failed to download scores from leaderboard.", M_ERROR);
    GCLeaderboardsScoreDownloadedCookies::is_done_ = true;
    return;
  }

  DEBUG_MESSAGE("Downloaded scores from leaderboard.", M_INFO);

  LeaderboardEntry_t leaderboard_entries[enigma_user::lb_max_entries];

  // Leaderboard entries handle will be invalid once we return from this function. Copy all data now.
  const int number_of_leaderboard_entries =
      std::min(pLeaderboardScoresDownloaded->m_cEntryCount, (int)enigma_user::lb_max_entries);
  for (unsigned index{0}; index < (unsigned)number_of_leaderboard_entries; index++) {
    steamworks_b::SteamBinder::ISteamUserStats_GetDownloadedLeaderboardEntry(
        steamworks_b::SteamBinder::SteamUserStats_vXXX(), pLeaderboardScoresDownloaded->m_hSteamLeaderboardEntries,
        index, &leaderboard_entries[index], nullptr, 0);
  }

  // Now our entries is here, let's save it.
  // SOGs are failing because of this line but LGM is not.
  // enigma::entries_array.get(GCLeaderboardsScoreDownloadedCookies::id_) = pLeaderboardScoresDownloaded;

  std::stringstream leaderboard_entries_buffer;

  get_leaderboard_entries(leaderboard_entries, number_of_leaderboard_entries, leaderboard_entries_buffer);

  enigma::push_leaderboard_download_steam_async_event(GCLeaderboardsScoreDownloadedCookies::id_,
                                                      leaderboard_entries_buffer.str(), *pLeaderboardScoresDownloaded);

  GCLeaderboardsScoreDownloadedCookies::is_done_ = true;
}

}  // namespace steamworks_gc
