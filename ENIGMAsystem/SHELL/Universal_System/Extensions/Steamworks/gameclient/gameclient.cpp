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
#include "gc_main.h"
#include "gc_overlay.h"
#include "gc_remotestorage.h"
#include "gc_statsandachievements.h"

// TODO: Test the cleaning algorithm here.
// TODO: Check if this (x == nullptr) equal to this (!x).

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
      steam_id_local_user_(CSteamID()),
      steam_app_id_(INVALID_APP_ID),
      current_game_language_(std::string()),
      available_game_languages_(std::string()) {
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
  GameClient::gc_statsandachievements_ = new GCStatsAndAchievements(GameClient::steam_app_id_);
  GameClient::gc_leaderboards_ = new GCLeaderboards();
  GameClient::gc_remotestorage_ = new GCRemoteStorage();

  if (steamworks_b::Binder::ISteamUser_GetSteamID == nullptr || steamworks_b::Binder::SteamUser_v023 == nullptr) {
    DEBUG_MESSAGE("GameClient::init() failed due to loading error.", M_ERROR);
    return;
  }

  GameClient::steam_id_local_user_ =
      steamworks_b::Binder::ISteamUser_GetSteamID(steamworks_b::Binder::SteamUser_v023());

  if (steamworks_b::Binder::ISteamUtils_GetAppID == nullptr || steamworks_b::Binder::SteamUtils_v010 == nullptr) {
    DEBUG_MESSAGE("GameClient::init() failed due to loading error.", M_ERROR);
    return;
  }

  GameClient::steam_app_id_ = steamworks_b::Binder::ISteamUtils_GetAppID(steamworks_b::Binder::SteamUtils_v010());

  if (steamworks_b::Binder::SteamApps_v008 == nullptr) {
    DEBUG_MESSAGE("GameClient::init() failed due to loading error.", M_ERROR);
    return;
  }

  if (steamworks_b::Binder::ISteamApps_GetCurrentGameLanguage == nullptr) {
    DEBUG_MESSAGE("GameClient::init() failed due to loading error.", M_ERROR);
    return;
  }

  GameClient::current_game_language_ =
      std::string(steamworks_b::Binder::ISteamApps_GetCurrentGameLanguage(steamworks_b::Binder::SteamApps_v008()));

  if (steamworks_b::Binder::ISteamApps_GetAvailableGameLanguages == nullptr) {
    DEBUG_MESSAGE("GameClient::init() failed due to loading error.", M_ERROR);
    return;
  }

  GameClient::available_game_languages_ =
      std::string(steamworks_b::Binder::ISteamApps_GetAvailableGameLanguages(steamworks_b::Binder::SteamApps_v008()));
}

GCOverlay* GameClient::get_gc_overlay() { return GameClient::gc_overlay_; }

GCStatsAndAchievements* GameClient::get_gc_statsandachievements() { return GameClient::gc_statsandachievements_; }

GCLeaderboards* GameClient::get_gc_leaderboards() { return GameClient::gc_leaderboards_; }

GCRemoteStorage* GameClient::get_gc_remotestorage() { return GameClient::gc_remotestorage_; }

CSteamID GameClient::get_steam_id_local_user() { return GameClient::steam_id_local_user_; }

uint32 GameClient::get_steam_app_id() { return GameClient::steam_app_id_; }

bool GameClient::get_current_game_language(std::string& buffer) {
  buffer = GameClient::current_game_language_;
  return true;
}

bool GameClient::get_available_game_languages(std::string& buffer) {
  buffer = GameClient::available_game_languages_;
  return true;
}

////////////////////////////////////////////////////////
// Static fields & functions (AKA Wrapper functions)
////////////////////////////////////////////////////////

bool GameClient::is_user_logged_on() {
  if (steamworks_b::Binder::ISteamUser_BLoggedOn == nullptr || steamworks_b::Binder::SteamUser_v023 == nullptr) {
    DEBUG_MESSAGE("GameClient::is_user_logged_on() failed due to loading error.", M_ERROR);
    return false;
  }

  return steamworks_b::Binder::ISteamUser_BLoggedOn(steamworks_b::Binder::SteamUser_v023());
}

bool GameClient::get_steam_persona_name(std::string& buffer) {
  if (steamworks_b::Binder::ISteamFriends_GetPersonaName == nullptr ||
      steamworks_b::Binder::SteamFriends_v017 == nullptr) {
    DEBUG_MESSAGE("GameClient::get_steam_persona_name() failed due to loading error.", M_ERROR);
    return false;
  }

  buffer = std::string(steamworks_b::Binder::ISteamFriends_GetPersonaName(steamworks_b::Binder::SteamFriends_v017()));
  return true;
}

bool GameClient::get_steam_user_persona_name(std::string& buffer, const uint64& steam_id) {
  if (steamworks_b::Binder::ISteamFriends_GetFriendPersonaName == nullptr ||
      steamworks_b::Binder::SteamFriends_v017 == nullptr) {
    DEBUG_MESSAGE("GameClient::get_steam_user_persona_name() failed due to loading error.", M_ERROR);
    return false;
  }

  buffer = std::string(
      steamworks_b::Binder::ISteamFriends_GetFriendPersonaName(steamworks_b::Binder::SteamFriends_v017(), steam_id));
  return true;
}

bool GameClient::is_subscribed() {
  if (steamworks_b::Binder::ISteamApps_BIsSubscribed == nullptr || steamworks_b::Binder::SteamApps_v008 == nullptr) {
    DEBUG_MESSAGE("GameClient::is_subscribed() failed due to loading error.", M_ERROR);
    return false;
  }

  return steamworks_b::Binder::ISteamApps_BIsSubscribed(steamworks_b::Binder::SteamApps_v008());
}

bool GameClient::set_rich_presence(const std::string& key, const std::string& value) {
  if (steamworks_b::Binder::ISteamFriends_SetRichPresence == nullptr ||
      steamworks_b::Binder::SteamFriends_v017 == nullptr) {
    DEBUG_MESSAGE("GameClient::set_rich_presence() failed due to loading error.", M_ERROR);
    return false;
  }

  return steamworks_b::Binder::ISteamFriends_SetRichPresence(steamworks_b::Binder::SteamFriends_v017(), key.c_str(),
                                                             value.c_str());
}

void GameClient::clear_rich_presence() {
  if (steamworks_b::Binder::ISteamFriends_ClearRichPresence == nullptr ||
      steamworks_b::Binder::SteamFriends_v017 == nullptr) {
    DEBUG_MESSAGE("GameClient::clear_rich_presence() failed due to loading error.", M_ERROR);
    return;
  }

  steamworks_b::Binder::ISteamFriends_ClearRichPresence(steamworks_b::Binder::SteamFriends_v017());
}

void GameClient::set_played_with(const uint64& steam_id) {
  if (steamworks_b::Binder::ISteamFriends_SetPlayedWith == nullptr ||
      steamworks_b::Binder::SteamFriends_v017 == nullptr) {
    DEBUG_MESSAGE("GameClient::set_played_with() failed due to loading error.", M_ERROR);
    return;
  }

  steamworks_b::Binder::ISteamFriends_SetPlayedWith(steamworks_b::Binder::SteamFriends_v017(), steam_id);
}

bool GameClient::get_image_size(const int& image, uint32& width, uint32& height) {
  if (steamworks_b::Binder::ISteamUtils_GetImageSize == nullptr || steamworks_b::Binder::SteamUtils_v010 == nullptr) {
    DEBUG_MESSAGE("GameClient::get_image_size() failed due to loading error.", M_ERROR);
    return false;
  }

  return steamworks_b::Binder::ISteamUtils_GetImageSize(steamworks_b::Binder::SteamUtils_v010(), image, &width,
                                                        &height);
}

bool GameClient::get_image_rgba(const int& image, uint8* buffer, const int& buffer_size) {
  if (steamworks_b::Binder::ISteamUtils_GetImageRGBA == nullptr || steamworks_b::Binder::SteamUtils_v010 == nullptr) {
    DEBUG_MESSAGE("GameClient::get_image_rgba() failed due to loading error.", M_ERROR);
    return false;
  }

  return steamworks_b::Binder::ISteamUtils_GetImageRGBA(steamworks_b::Binder::SteamUtils_v010(), image, buffer,
                                                        buffer_size);
}

int32 GameClient::get_small_friend_avatar(const uint64& steam_id_friend) {
  if (steamworks_b::Binder::ISteamFriends_GetSmallFriendAvatar == nullptr ||
      steamworks_b::Binder::SteamFriends_v017 == nullptr) {
    DEBUG_MESSAGE("GameClient::get_small_friend_avatar() failed due to loading error.", M_ERROR);
    return 0;
  }

  return steamworks_b::Binder::ISteamFriends_GetSmallFriendAvatar(steamworks_b::Binder::SteamFriends_v017(),
                                                                  steam_id_friend);
}

int32 GameClient::get_medium_friend_avatar(const uint64& steam_id_friend) {
  if (steamworks_b::Binder::ISteamFriends_GetMediumFriendAvatar == nullptr ||
      steamworks_b::Binder::SteamFriends_v017 == nullptr) {
    DEBUG_MESSAGE("GameClient::get_medium_friend_avatar() failed due to loading error.", M_ERROR);
    return 0;
  }

  return steamworks_b::Binder::ISteamFriends_GetMediumFriendAvatar(steamworks_b::Binder::SteamFriends_v017(),
                                                                   steam_id_friend);
}

int32 GameClient::get_large_friend_avatar(const uint64& steam_id_friend) {
  if (steamworks_b::Binder::ISteamFriends_GetLargeFriendAvatar == nullptr ||
      steamworks_b::Binder::SteamFriends_v017 == nullptr) {
    DEBUG_MESSAGE("GameClient::get_large_friend_avatar() failed due to loading error.", M_ERROR);
    return 0;
  }

  return steamworks_b::Binder::ISteamFriends_GetLargeFriendAvatar(steamworks_b::Binder::SteamFriends_v017(),
                                                                  steam_id_friend);
}

}  // namespace steamworks_gc
