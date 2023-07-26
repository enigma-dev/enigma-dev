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

#include <algorithm>

#include "Universal_System/../Platforms/General/PFmain.h"
#include "Universal_System/Extensions/DataStructures/include.h"

#include "../leaderboards.h"
#include "game_client.h"

namespace steamworks {

////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////

c_leaderboards::c_leaderboards() : current_leaderboard_(NULL), number_of_leaderboard_entries_(0), loading_(false) {}

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
  c_leaderboards::loading_ = false;

  if (!pFindLeaderboardResult->m_bLeaderboardFound || bIOFailure) return;

  c_leaderboards::current_leaderboard_ = pFindLeaderboardResult->m_hSteamLeaderboard;

  DEBUG_MESSAGE("Calling FindOrCreateLeaderboard succeeded.", M_INFO);
}

bool c_leaderboards::upload_score(const int score,
                                  const ELeaderboardUploadScoreMethod leaderboard_upload_score_method) {
  // while (c_leaderboards::loading_)
  //   ;  // Wait for the callback of FindOrCreateLeaderboard to be invoked

  if (NULL == c_leaderboards::current_leaderboard_) return false;

  SteamAPICall_t steam_api_call = SteamUserStats()->UploadLeaderboardScore(
      c_leaderboards::current_leaderboard_, leaderboard_upload_score_method, score, NULL, 0);
  c_leaderboards::m_SteamCallResultUploadScore.Set(steam_api_call, this, &c_leaderboards::on_upload_score);

  return true;
}

void c_leaderboards::on_upload_score(LeaderboardScoreUploaded_t* pScoreUploadedResult, bool bIOFailure) {
  if (!pScoreUploadedResult->m_bSuccess || bIOFailure) {
    DEBUG_MESSAGE("Failed to upload score to leaderboard.", M_ERROR);
  }
}

bool c_leaderboards::download_scores(const ELeaderboardDataRequest leaderboard_data_request, const int range_start,
                                     const int range_end) {
  // while (c_leaderboards::loading_)
  //   ;  // Wait for the callback of FindOrCreateLeaderboard to be invoked

  if (NULL == c_leaderboards::current_leaderboard_) return false;

  c_leaderboards::loading_ = true;

  SteamAPICall_t steam_api_call = SteamUserStats()->DownloadLeaderboardEntries(
      c_leaderboards::current_leaderboard_, leaderboard_data_request, range_start, range_end);
  c_leaderboards::m_SteamCallResultDownloadScores.Set(steam_api_call, this, &c_leaderboards::on_download_scores);

  return true;
}

std::string& get_leaderboard_entries_string(LeaderboardEntry_t leaderboard_entries[],
                                            unsigned leaderboard_entries_size) {
  std::string entries_accumulator{""};
  entries_accumulator += "{";
  entries_accumulator += "\"entries\":";
  entries_accumulator += "[";

  for (unsigned i{0}; i < leaderboard_entries_size; i++) {
    entries_accumulator += "{";
    entries_accumulator += "\"name\":";
    entries_accumulator += "\"";
    entries_accumulator += c_game_client::get_steam_user_persona_name(leaderboard_entries[i].m_steamIDUser);
    entries_accumulator += "\"";
    entries_accumulator += ",";
    entries_accumulator += "\"score\":";
    entries_accumulator += std::to_string(leaderboard_entries[i].m_nScore);
    entries_accumulator += ",";
    entries_accumulator += "\"rank\":";
    entries_accumulator += std::to_string(leaderboard_entries[i].m_nGlobalRank);
    entries_accumulator += ",";
    entries_accumulator += "\"userID\":";
    entries_accumulator += "\"";
    entries_accumulator += std::to_string(leaderboard_entries[i].m_steamIDUser.ConvertToUint64());
    entries_accumulator += "\"";
    entries_accumulator += "}";

    if (i < leaderboard_entries_size - 1) entries_accumulator += ",";
  }

  entries_accumulator += "]";
  entries_accumulator += "}";

  return entries_accumulator;
}

void c_leaderboards::on_download_scores(LeaderboardScoresDownloaded_t* pLeaderboardScoresDownloaded, bool bIOFailure) {
  c_leaderboards::loading_ = false;

  if (bIOFailure) {
    DEBUG_MESSAGE("Failed to download scores from leaderboard.", M_ERROR);
    return;
  }

  DEBUG_MESSAGE("Downloaded scores from leaderboard.", M_INFO);

  LeaderboardEntry_t leaderboard_entries[enigma_user::lb_max_entries];

  // leaderboard entries handle will be invalid once we return from this function. Copy all data now.
  c_leaderboards::number_of_leaderboard_entries_ =
      std::min(pLeaderboardScoresDownloaded->m_cEntryCount, (int)enigma_user::lb_max_entries);
  for (unsigned index = 0; index < c_leaderboards::number_of_leaderboard_entries_; index++) {
    SteamUserStats()->GetDownloadedLeaderboardEntry(pLeaderboardScoresDownloaded->m_hSteamLeaderboardEntries, index,
                                                    &leaderboard_entries[index], NULL, 0);
  }

  std::string& leaderboard_entries_string =
      get_leaderboard_entries_string(leaderboard_entries, c_leaderboards::number_of_leaderboard_entries_);

  std::map<std::string, variant> temp_map = {
      {"entries", leaderboard_entries_string},
      {"lb_name", std::string(SteamUserStats()->GetLeaderboardName(c_leaderboards::current_leaderboard_))},
      {"event_type", "leaderboard_download"},
      {"id", std::to_string(enigma::lb_entries_download_id)},
      {"num_entries", std::to_string(c_leaderboards::number_of_leaderboard_entries_)},
      {"status", 0}};  // TODO: the status must not be constant value

  enigma::posted_async_events.push(temp_map);
}

/*
{
  "entries":
  "{\n    
    \"entries\": 
    [\n
      { \"name\"  : \"TomasJPereyra\", 
      \"score\" : 1, 
      \"rank\"  : 1, 
      \"userID\": \"@i64@110000108ae8556$i64$\" 
      },\n     
    
      { 
        \"name\"  : \"Scott-ish\", 
        \"score\" : 10, 
        \"rank\"  : 2, 
        \"userID\": \"@i64@11000010241f4ea$i64$\" 
      },\n
      
      { 
        \"name\"  : \"Loyal RaveN\", 
        \"score\" : 63, 
        \"rank\"  : 3, 
        \"userID\": \"@i64@11000015c558396$i64$\" 
      },\n
      
      { 
        \"name\"  : \"simon\", 
        \"score\" : 113, 
        \"rank\"  : 4, 
        \"userID\": \"@i64@1100001040447b6$i64$\" 
      },\n
      
      { 
        \"name\"  : \"meFroggy\", 
        \"score\" : 138, 
        \"rank\"  : 5, 
        \"userID\": 
        \"@i64@110000117c9d62b$i64$\", 
        \"data\"  : \"HG1lRnJvZ2d5IHdhcyBoZXJlIDopAAAA\" 
      },\n
      { 
        \"name\"  : \"падонак fashion\", 
        \"score\" : 155, 
        \"rank\"  : 6, 
        \"userID\": \"@i64@11000010be342ce$i64$\", 
        \"data\"  : \"FEZlZWxpbmcgU2lja25lenogd2FzIGhlcmUgOikAAAA=\" 
      },\n
      
      { 
        \"name\"  : \"hermitpal\", 
        \"score\" : 196, 
        \"rank\"  : 7, 
        \"userID\": \"@i64@110000106401474$i64$\", 
        \"data\"  : \"R2hlcm1pdCB3YXMgaGVyZSA6KQA=\" 
      },\n
      
      { 
        \"name\"  : \"Joeyman98\", 
        \"score\" : 239, 
        \"rank\"  : 8, 
        \"userID\": \"@i64@11000010624f1e3$i64$\", 
        \"data\"  : \"I0pvZXltYW45OCB3YXMgaGVyZSA6KQAA\" 
      },\n                
      
      { 
        \"name\"  : \"tdlsoftware\", 
        \"score\" : 246, 
        \"rank\"  : 9, 
        \"userID\": \"@i64@1100001434a5930$i64$\", 
        \"data\"  : \"KnRkbHNvZnR3YXJlIHdhcyBoZXJlIDopAAAAAA==\" 
      },\n                
      
      { 
        \"name\"  : \"Benal\", 
        \"score\" : 254, 
        \"rank\"  : 10, 
        \"userID\": \"@i64@1100001053f418a$i64$\", 
        \"data\"  : \"SEJlbmFsIHdhcyBoZXJlIDopAAA=\" 
      }\n   \n    
    ]\n
  }\n",

  "lb_name":"YYLeaderboard_10\/29\/21--",
  "event_type":"leaderboard_download",
  "id":5.0,
  "num_entries":10.0,
  "status":1.0
  }

*/

}  // namespace steamworks
