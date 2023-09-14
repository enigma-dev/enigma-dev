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

#include "gameclient.h"

#include "gc_leaderboards.h"
#include "gc_overlay.h"
#include "gc_remotestorage.h"
#include "gc_statsandachievements.h"

// TODO: Test the cleaning algorithm here.

namespace steamworks_gc {

// gameclient* gameclient_{nullptr};
// gameclient* gameclient() { return gameclient_; }

////////////////////////////////////////////////////////
// Public fields & functions
////////////////////////////////////////////////////////

GameClient::GameClient()
    : gc_overlay_(nullptr),
      gc_statsandachievements_(nullptr),
      gc_leaderboards_(nullptr),
      gc_remotestorage_(nullptr),
      steam_id_local_user_(SteamUser()->GetSteamID()),
      steam_app_id_(SteamUtils()->GetAppID()),
      current_game_language_(std::string(SteamApps()->GetCurrentGameLanguage())),
      available_game_languages_(std::string(SteamApps()->GetAvailableGameLanguages())) {
  init();
}

GameClient::~GameClient() {
  if (nullptr != GameClient::gc_overlay_) delete GameClient::gc_overlay_;
  if (nullptr != GameClient::gc_statsandachievements_) delete GameClient::gc_statsandachievements_;
  if (nullptr != GameClient::gc_leaderboards_) delete GameClient::gc_leaderboards_;
  if (nullptr != GameClient::gc_remotestorage_) delete GameClient::gc_remotestorage_;
}

void GameClient::init() {
  // gameclient_ = this;

  GameClient::gc_overlay_ = new GCOverlay();
  GameClient::gc_statsandachievements_ = new GCStatsAndAchievements();
  GameClient::gc_leaderboards_ = new GCLeaderboards();
  GameClient::gc_remotestorage_ = new GCRemoteStorage();
}

GCOverlay* GameClient::get_gc_overlay() { return GameClient::gc_overlay_; }

GCStatsAndAchievements* GameClient::get_gc_statsandachievements() {
  return GameClient::gc_statsandachievements_;
}

GCLeaderboards* GameClient::get_gc_leaderboards() { return GameClient::gc_leaderboards_; }

GCRemoteStorage* GameClient::get_gc_remotestorage() { return GameClient::gc_remotestorage_; }

CSteamID GameClient::get_steam_id_local_user() { return GameClient::steam_id_local_user_; }

uint32 GameClient::get_steam_app_id() { return GameClient::steam_app_id_; }

std::string GameClient::get_current_game_language() { return GameClient::current_game_language_; }

std::string GameClient::get_available_game_languages() { return GameClient::available_game_languages_; }

////////////////////////////////////////////////////////
// Static fields & functions (AKA Wrapper functions)
////////////////////////////////////////////////////////

bool GameClient::is_user_logged_on() { return SteamUser()->BLoggedOn(); }

std::string GameClient::get_steam_persona_name() { return std::string(SteamFriends()->GetPersonaName()); }

std::string GameClient::get_steam_user_persona_name(const uint64 steam_id) {
  CSteamID c_steam_id(steam_id);
  return std::string(SteamFriends()->GetFriendPersonaName(c_steam_id));
}

bool GameClient::is_subscribed() { return SteamApps()->BIsSubscribed(); }

bool GameClient::set_rich_presence(const std::string& key, const std::string& value) {
  return SteamFriends()->SetRichPresence(key.c_str(), value.c_str());
}

void GameClient::clear_rich_presence() { SteamFriends()->ClearRichPresence(); }

void GameClient::set_played_with(const uint64 steam_id) {
  CSteamID c_steam_id(steam_id);
  SteamFriends()->SetPlayedWith(c_steam_id);
}

bool GameClient::get_image_size(const int image, uint32* width, uint32* height) {
  return SteamUtils()->GetImageSize(image, width, height);
}

bool GameClient::get_image_rgba(const int image, uint8* buffer, const int buffer_size) {
  return SteamUtils()->GetImageRGBA(image, buffer, buffer_size);
}

int32 GameClient::get_small_friend_avatar(const uint64 steam_id_friend) {
  CSteamID c_steam_id(steam_id_friend);
  return SteamFriends()->GetSmallFriendAvatar(c_steam_id);
}

int32 GameClient::get_medium_friend_avatar(const uint64 steam_id_friend) {
  CSteamID c_steam_id(steam_id_friend);
  return SteamFriends()->GetMediumFriendAvatar(c_steam_id);
}

int32 GameClient::get_large_friend_avatar(const uint64 steam_id_friend) {
  CSteamID c_steam_id(steam_id_friend);
  return SteamFriends()->GetLargeFriendAvatar(c_steam_id);
}

}  // namespace steamworks_gc
