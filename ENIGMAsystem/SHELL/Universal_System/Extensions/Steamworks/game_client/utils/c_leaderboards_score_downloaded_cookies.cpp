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

#include "c_leaderboards_score_downloaded_cookies.h"

#include "../../leaderboards.h"
#include "../c_leaderboards.h"

namespace steamworks {

////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////

// TODO: Add data key/value pair (if existed).
// TODO: When using the concatenation operator '+' to concatenate strings instead of '<<', the string content blowed up
// because if something I don't understand.
void get_leaderboard_entries(LeaderboardEntry_t leaderboard_entries[], unsigned leaderboard_entries_size,
                             std::stringstream& leaderboard_entries_buffer) {
  leaderboard_entries_buffer << '{';  // Entries object open bracket
  leaderboard_entries_buffer << '\"' << "entries" << '\"' << ':';
  leaderboard_entries_buffer << '[';  // Entries array open bracket

  for (unsigned i{0}; i < leaderboard_entries_size; i++) {
    leaderboard_entries_buffer << '{';
    leaderboard_entries_buffer << '\"' << "name" << '\"' << ':';
    leaderboard_entries_buffer << '\"'
                                  << steamworks::c_game_client::get_steam_user_persona_name(
                                         leaderboard_entries[i].m_steamIDUser)
                                  << '\"' << ',';
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

c_leaderboards_score_downloaded_cookies::c_leaderboards_score_downloaded_cookies(int id, c_leaderboards* c_leaderboards,
                                                                                 SteamAPICall_t steam_api_call)
    : id_(id), c_leaderboards_(c_leaderboards), is_done_(false) {
  c_leaderboards_score_downloaded_cookies::set_call_result(steam_api_call);
}

bool c_leaderboards_score_downloaded_cookies::is_done() const {
  return c_leaderboards_score_downloaded_cookies::is_done_;
}

////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////

void c_leaderboards_score_downloaded_cookies::set_call_result(SteamAPICall_t steam_api_call) {
  c_leaderboards_score_downloaded_cookies::m_SteamCallResultDownloadScores.Set(
      steam_api_call, this, &c_leaderboards_score_downloaded_cookies::on_download_scores);
}

void c_leaderboards_score_downloaded_cookies::on_download_scores(
    LeaderboardScoresDownloaded_t* pLeaderboardScoresDownloaded, bool bIOFailure) {
  if (bIOFailure) {
    DEBUG_MESSAGE("Failed to download scores from leaderboard.", M_ERROR);
    c_leaderboards_score_downloaded_cookies::c_leaderboards_->set_loading(false);
    return;
  }

  DEBUG_MESSAGE("Downloaded scores from leaderboard.", M_INFO);

  LeaderboardEntry_t leaderboard_entries[enigma_user::lb_max_entries];

  // Leaderboard entries handle will be invalid once we return from this function. Copy all data now.
  const int number_of_leaderboard_entries =
      std::min(pLeaderboardScoresDownloaded->m_cEntryCount, (int)enigma_user::lb_max_entries);
  for (unsigned index = 0; index < number_of_leaderboard_entries; index++) {
    SteamUserStats()->GetDownloadedLeaderboardEntry(pLeaderboardScoresDownloaded->m_hSteamLeaderboardEntries, index,
                                                    &leaderboard_entries[index], NULL, 0);
  }

  // Now our entries is here, let's save it.
  // enigma::entries_array.get(c_leaderboards_score_downloaded_cookies::id_) = leaderboard_entries;

  // Entries are saved? We are ready to accept new requests.
  // c_leaderboards_score_downloaded_cookies::c_leaderboards_->set_loading(false);

  std::stringstream leaderboard_entries_buffer;

  get_leaderboard_entries(leaderboard_entries, number_of_leaderboard_entries, leaderboard_entries_buffer);

  enigma::push_leaderboard_download_steam_async_event(c_leaderboards_score_downloaded_cookies::id_,
                                                      pLeaderboardScoresDownloaded, leaderboard_entries_buffer);

  c_leaderboards_score_downloaded_cookies::is_done_ = true;
}

}  // namespace steamworks
