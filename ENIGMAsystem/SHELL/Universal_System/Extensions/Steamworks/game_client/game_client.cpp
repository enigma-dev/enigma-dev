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

#include "game_client.h"

#include "c_leaderboards.h"
#include "c_overlay.h"
#include "c_stats_and_achievements.h"

// TODO: Test the cleaning algorithm here.

namespace steamworks {

// c_game_client* game_client_{NULL};
// c_game_client* game_client() { return game_client_; }

class c_leaderboards_find_result_cookies;
class c_leaderboards_score_uploaded_cookies;
class c_leaderboards_score_downloaded_cookies;

////////////////////////////////////////////////////////
// Public fields & functions
////////////////////////////////////////////////////////

c_game_client::c_game_client()
    : c_overlay_(NULL),
      c_stats_and_achievements_(NULL),
      c_leaderboards_(NULL),
      c_steam_id_local_user_(SteamUser()->GetSteamID()),
      steam_app_id_(SteamUtils()->GetAppID()),
      current_game_language_(std::string(SteamApps()->GetCurrentGameLanguage())),
      available_game_languages_(std::string(SteamApps()->GetAvailableGameLanguages())) {
  init();
}

bool c_leaderboards_find_result_lambda(const c_leaderboards_find_result_cookies* c_leaderboards_find_result) {
  delete c_leaderboards_find_result;
  return true;
}

bool c_leaderboards_score_uploaded_lambda(
    const c_leaderboards_score_uploaded_cookies* c_leaderboards_score_uploaded) {
  delete c_leaderboards_score_uploaded;
  return true;
}

bool c_leaderboards_score_downloaded_lambda(
    const c_leaderboards_score_downloaded_cookies* c_leaderboards_score_downloaded) {
  delete c_leaderboards_score_downloaded;
  return true;
}

c_game_client::~c_game_client() {
  if (NULL != c_game_client::c_overlay_) delete c_game_client::c_overlay_;
  if (NULL != c_game_client::c_stats_and_achievements_) delete c_game_client::c_stats_and_achievements_;
  if (NULL != c_game_client::c_leaderboards_) delete c_game_client::c_leaderboards_;

  c_leaderboards_find_result_cookies_instances_tracker.erase(
      std::remove_if(c_leaderboards_find_result_cookies_instances_tracker.begin(),
                     c_leaderboards_find_result_cookies_instances_tracker.end(),
                     c_leaderboards_find_result_lambda),
      c_leaderboards_find_result_cookies_instances_tracker.end());

  c_leaderboards_score_uploaded_cookies_instances_tracker.erase(
      std::remove_if(c_leaderboards_score_uploaded_cookies_instances_tracker.begin(),
                     c_leaderboards_score_uploaded_cookies_instances_tracker.end(),
                     c_leaderboards_score_uploaded_lambda),
      c_leaderboards_score_uploaded_cookies_instances_tracker.end());

  c_leaderboards_score_downloaded_cookies_instances_tracker.erase(
      std::remove_if(c_leaderboards_score_downloaded_cookies_instances_tracker.begin(),
                     c_leaderboards_score_downloaded_cookies_instances_tracker.end(),
                     c_leaderboards_score_downloaded_lambda),
      c_leaderboards_score_downloaded_cookies_instances_tracker.end());
}

void c_game_client::init() {
  // game_client_ = this;

  c_game_client::c_overlay_ = new c_overlay();
  c_game_client::c_stats_and_achievements_ = new c_stats_and_achievements();
  c_game_client::c_leaderboards_ = new c_leaderboards();
}

c_overlay* c_game_client::get_c_overlay() { return c_game_client::c_overlay_; }

c_stats_and_achievements* c_game_client::get_c_stats_and_achievements() {
  return c_game_client::c_stats_and_achievements_;
}

c_leaderboards* c_game_client::get_c_leaderboards() { return c_game_client::c_leaderboards_; }

CSteamID c_game_client::get_c_steam_id_local_user() { return c_game_client::c_steam_id_local_user_; }

unsigned c_game_client::get_steam_app_id() { return c_game_client::steam_app_id_; }

std::string c_game_client::get_current_game_language() { return c_game_client::current_game_language_; }

std::string c_game_client::get_available_game_languages() { return c_game_client::available_game_languages_; }

////////////////////////////////////////////////////////
// Static fields & functions
////////////////////////////////////////////////////////

bool c_game_client::is_user_logged_on() { return SteamUser()->BLoggedOn(); }

std::string c_game_client::get_steam_persona_name() { return std::string(SteamFriends()->GetPersonaName()); }

std::string c_game_client::get_steam_user_persona_name(CSteamID c_steam_id) {
  return std::string(SteamFriends()->GetFriendPersonaName(c_steam_id));
}

bool c_game_client::is_subscribed() { return SteamApps()->BIsSubscribed(); }

}  // namespace steamworks
