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

  GameClient::gc_overlay_ = nullptr;
  GameClient::gc_statsandachievements_ = nullptr;
  GameClient::gc_leaderboards_ = nullptr;
  GameClient::gc_remotestorage_ = nullptr;
}

void GameClient::init() {
  // gameclient_ = this;

  GameClient::steam_id_local_user_ =
      steamworks_b::SteamBinder::ISteamUser_GetSteamID(steamworks_b::SteamBinder::SteamUser_vXXX());

  GameClient::steam_app_id_ =
      steamworks_b::SteamBinder::ISteamUtils_GetAppID(steamworks_b::SteamBinder::SteamUtils_vXXX());

  if (GameClient::steam_app_id_ == INVALID_APP_ID) {
    DEBUG_MESSAGE("Invalid AppID.", M_ERROR);
    return;
  }

  GameClient::current_game_language_ = std::string(
      steamworks_b::SteamBinder::ISteamApps_GetCurrentGameLanguage(steamworks_b::SteamBinder::SteamApps_vXXX()));

  GameClient::available_game_languages_ = std::string(
      steamworks_b::SteamBinder::ISteamApps_GetAvailableGameLanguages(steamworks_b::SteamBinder::SteamApps_vXXX()));

  GameClient::gc_overlay_ = new GCOverlay();
  GameClient::gc_statsandachievements_ = new GCStatsAndAchievements(GameClient::steam_app_id_);
  GameClient::gc_leaderboards_ = new GCLeaderboards();
  GameClient::gc_remotestorage_ = new GCRemoteStorage();
}

GCOverlay* GameClient::get_gc_overlay() { return GameClient::gc_overlay_; }

GCStatsAndAchievements* GameClient::get_gc_statsandachievements() { return GameClient::gc_statsandachievements_; }

GCLeaderboards* GameClient::get_gc_leaderboards() { return GameClient::gc_leaderboards_; }

GCRemoteStorage* GameClient::get_gc_remotestorage() { return GameClient::gc_remotestorage_; }

CSteamID GameClient::get_steam_id_local_user() { return GameClient::steam_id_local_user_; }

uint32 GameClient::get_steam_app_id() { return GameClient::steam_app_id_; }

void GameClient::get_current_game_language(std::string& buffer) {
  buffer = GameClient::current_game_language_;
}

void GameClient::get_available_game_languages(std::string& buffer) {
  buffer = GameClient::available_game_languages_;
}

////////////////////////////////////////////////////////
// Static fields & functions (AKA Wrapper functions)
////////////////////////////////////////////////////////

bool GameClient::is_user_logged_on() {
  return steamworks_b::SteamBinder::ISteamUser_BLoggedOn(steamworks_b::SteamBinder::SteamUser_vXXX());
}

void GameClient::get_steam_persona_name(std::string& buffer) {
  buffer = std::string(
      steamworks_b::SteamBinder::ISteamFriends_GetPersonaName(steamworks_b::SteamBinder::SteamFriends_vXXX()));
}

void GameClient::get_steam_user_persona_name(std::string& buffer, const uint64& steam_id) {
  buffer = std::string(steamworks_b::SteamBinder::ISteamFriends_GetFriendPersonaName(
      steamworks_b::SteamBinder::SteamFriends_vXXX(), steam_id));
}

bool GameClient::is_subscribed() {
  return steamworks_b::SteamBinder::ISteamApps_BIsSubscribed(steamworks_b::SteamBinder::SteamApps_vXXX());
}

bool GameClient::set_rich_presence(const std::string& key, const std::string& value) {
  return steamworks_b::SteamBinder::ISteamFriends_SetRichPresence(steamworks_b::SteamBinder::SteamFriends_vXXX(),
                                                                  key.c_str(), value.c_str());
}

void GameClient::clear_rich_presence() {
  steamworks_b::SteamBinder::ISteamFriends_ClearRichPresence(steamworks_b::SteamBinder::SteamFriends_vXXX());
}

void GameClient::set_played_with(const uint64& steam_id) {
  steamworks_b::SteamBinder::ISteamFriends_SetPlayedWith(steamworks_b::SteamBinder::SteamFriends_vXXX(), steam_id);
}

bool GameClient::get_image_size(const int& image, uint32& width, uint32& height) {
  return steamworks_b::SteamBinder::ISteamUtils_GetImageSize(steamworks_b::SteamBinder::SteamUtils_vXXX(), image,
                                                             &width, &height);
}

bool GameClient::get_image_rgba(const int& image, uint8* buffer, const int& buffer_size) {
  return steamworks_b::SteamBinder::ISteamUtils_GetImageRGBA(steamworks_b::SteamBinder::SteamUtils_vXXX(), image,
                                                             buffer, buffer_size);
}

int32 GameClient::get_small_friend_avatar(const uint64& steam_id_friend) {
  return steamworks_b::SteamBinder::ISteamFriends_GetSmallFriendAvatar(steamworks_b::SteamBinder::SteamFriends_vXXX(),
                                                                       steam_id_friend);
}

int32 GameClient::get_medium_friend_avatar(const uint64& steam_id_friend) {
  return steamworks_b::SteamBinder::ISteamFriends_GetMediumFriendAvatar(steamworks_b::SteamBinder::SteamFriends_vXXX(),
                                                                        steam_id_friend);
}

int32 GameClient::get_large_friend_avatar(const uint64& steam_id_friend) {
  return steamworks_b::SteamBinder::ISteamFriends_GetLargeFriendAvatar(steamworks_b::SteamBinder::SteamFriends_vXXX(),
                                                                       steam_id_friend);
}

}  // namespace steamworks_gc

void Custom_SteamAPI_RegisterCallback(class CCallbackBase* pCallback, int iCallback) {
  steamworks_b::SteamBinder::RegisterCallback(pCallback, iCallback);
}

void Custom_SteamAPI_UnregisterCallback(class CCallbackBase* pCallback) {
  steamworks_b::SteamBinder::UnregisterCallback(pCallback);
}

void Custom_SteamAPI_RegisterCallResult(class CCallbackBase* pCallback, SteamAPICall_t hAPICall) {
  steamworks_b::SteamBinder::RegisterCallResult(pCallback, hAPICall);
}

void Custom_SteamAPI_UnregisterCallResult(class CCallbackBase* pCallback, SteamAPICall_t hAPICall) {
  steamworks_b::SteamBinder::UnregisterCallResult(pCallback, hAPICall);
}
