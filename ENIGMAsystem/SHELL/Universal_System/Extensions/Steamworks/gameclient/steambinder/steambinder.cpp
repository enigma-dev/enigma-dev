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

#include "steambinder.h"

#include <cassert>
#include <cstdlib>

namespace steamworks_b {

Init_t SteamBinder::Init{nullptr};
Shutdown_t SteamBinder::Shutdown{nullptr};
RestartAppIfNecessary_t SteamBinder::RestartAppIfNecessary{nullptr};
RunCallbacks_t SteamBinder::RunCallbacks{nullptr};

RegisterCallback_t SteamBinder::RegisterCallback{nullptr};
UnregisterCallback_t SteamBinder::UnregisterCallback{nullptr};

RegisterCallResult_t SteamBinder::RegisterCallResult{nullptr};
UnregisterCallResult_t SteamBinder::UnregisterCallResult{nullptr};

SteamUser_vXXX_t SteamBinder::SteamUser_vXXX{nullptr};

ISteamUser_BLoggedOn_t SteamBinder::ISteamUser_BLoggedOn{nullptr};
ISteamUser_GetSteamID_t SteamBinder::ISteamUser_GetSteamID{nullptr};

SteamFriends_vXXX_t SteamBinder::SteamFriends_vXXX{nullptr};

ISteamFriends_GetPersonaName_t SteamBinder::ISteamFriends_GetPersonaName{nullptr};
ISteamFriends_GetFriendPersonaName_t SteamBinder::ISteamFriends_GetFriendPersonaName{nullptr};
ISteamFriends_ActivateGameOverlay_t SteamBinder::ISteamFriends_ActivateGameOverlay{nullptr};
#ifdef ENIGMA_FAKE_STEAMWORKS_API
ISteamFriends_DeactivateGameOverlay_t SteamBinder::ISteamFriends_DeactivateGameOverlay{
    nullptr};  // Not part of the official API.
#endif         // ENIGMA_FAKE_STEAMWORKS_API
ISteamFriends_ActivateGameOverlayToUser_t SteamBinder::ISteamFriends_ActivateGameOverlayToUser{nullptr};
ISteamFriends_ActivateGameOverlayToWebPage_t SteamBinder::ISteamFriends_ActivateGameOverlayToWebPage{nullptr};
ISteamFriends_ActivateGameOverlayToStore_t SteamBinder::ISteamFriends_ActivateGameOverlayToStore{nullptr};
ISteamFriends_SetPlayedWith_t SteamBinder::ISteamFriends_SetPlayedWith{nullptr};
ISteamFriends_GetSmallFriendAvatar_t SteamBinder::ISteamFriends_GetSmallFriendAvatar{nullptr};
ISteamFriends_GetMediumFriendAvatar_t SteamBinder::ISteamFriends_GetMediumFriendAvatar{nullptr};
ISteamFriends_GetLargeFriendAvatar_t SteamBinder::ISteamFriends_GetLargeFriendAvatar{nullptr};
ISteamFriends_SetRichPresence_t SteamBinder::ISteamFriends_SetRichPresence{nullptr};
ISteamFriends_ClearRichPresence_t SteamBinder::ISteamFriends_ClearRichPresence{nullptr};

SteamUtils_vXXX_t SteamBinder::SteamUtils_vXXX{nullptr};

ISteamUtils_GetImageSize_t SteamBinder::ISteamUtils_GetImageSize{nullptr};
ISteamUtils_GetImageRGBA_t SteamBinder::ISteamUtils_GetImageRGBA{nullptr};
ISteamUtils_GetAppID_t SteamBinder::ISteamUtils_GetAppID{nullptr};
ISteamUtils_SetOverlayNotificationPosition_t SteamBinder::ISteamUtils_SetOverlayNotificationPosition{nullptr};
ISteamUtils_SetWarningMessageHook_t SteamBinder::ISteamUtils_SetWarningMessageHook{nullptr};
ISteamUtils_IsOverlayEnabled_t SteamBinder::ISteamUtils_IsOverlayEnabled{nullptr};
ISteamUtils_SetOverlayNotificationInset_t SteamBinder::ISteamUtils_SetOverlayNotificationInset{nullptr};

SteamUserStats_vXXX_t SteamBinder::SteamUserStats_vXXX{nullptr};

ISteamUserStats_RequestCurrentStats_t SteamBinder::ISteamUserStats_RequestCurrentStats{nullptr};
ISteamUserStats_GetStatInt32_t SteamBinder::ISteamUserStats_GetStatInt32{nullptr};
ISteamUserStats_GetStatFloat_t SteamBinder::ISteamUserStats_GetStatFloat{nullptr};
ISteamUserStats_SetStatInt32_t SteamBinder::ISteamUserStats_SetStatInt32{nullptr};
ISteamUserStats_SetStatFloat_t SteamBinder::ISteamUserStats_SetStatFloat{nullptr};
ISteamUserStats_UpdateAvgRateStat_t SteamBinder::ISteamUserStats_UpdateAvgRateStat{nullptr};
ISteamUserStats_GetAchievement_t SteamBinder::ISteamUserStats_GetAchievement{nullptr};
ISteamUserStats_SetAchievement_t SteamBinder::ISteamUserStats_SetAchievement{nullptr};
ISteamUserStats_ClearAchievement_t SteamBinder::ISteamUserStats_ClearAchievement{nullptr};
ISteamUserStats_StoreStats_t SteamBinder::ISteamUserStats_StoreStats{nullptr};
ISteamUserStats_ResetAllStats_t SteamBinder::ISteamUserStats_ResetAllStats{nullptr};
ISteamUserStats_FindOrCreateLeaderboard_t SteamBinder::ISteamUserStats_FindOrCreateLeaderboard{nullptr};
ISteamUserStats_FindLeaderboard_t SteamBinder::ISteamUserStats_FindLeaderboard{nullptr};
ISteamUserStats_GetLeaderboardName_t SteamBinder::ISteamUserStats_GetLeaderboardName{nullptr};
ISteamUserStats_DownloadLeaderboardEntries_t SteamBinder::ISteamUserStats_DownloadLeaderboardEntries{nullptr};
ISteamUserStats_GetDownloadedLeaderboardEntry_t SteamBinder::ISteamUserStats_GetDownloadedLeaderboardEntry{nullptr};
ISteamUserStats_UploadLeaderboardScore_t SteamBinder::ISteamUserStats_UploadLeaderboardScore{nullptr};

SteamApps_vXXX_t SteamBinder::SteamApps_vXXX{nullptr};

ISteamApps_BIsSubscribed_t SteamBinder::ISteamApps_BIsSubscribed{nullptr};
ISteamApps_GetCurrentGameLanguage_t SteamBinder::ISteamApps_GetCurrentGameLanguage{nullptr};
ISteamApps_GetAvailableGameLanguages_t SteamBinder::ISteamApps_GetAvailableGameLanguages{nullptr};

bool SteamBinder::bind() {
  fs::path libpath{std::string()};

  const char* steam_sdk_path = std::getenv("STEAM_SDK_PATH");

  if (steam_sdk_path != nullptr) {
    libpath.assign(std::string(steam_sdk_path) + "/redistributable_bin/linux64/libsteam_api.so");
    if (!fs::exists(libpath)) {
      DEBUG_MESSAGE("STEAM_SDK_PATH environment variable is set but the file does not exist. Aborting...", M_ERROR);
      return false;
    }
  } else {
    DEBUG_MESSAGE("STEAM_SDK_PATH environment variable is not set. Switching to default path...", M_WARNING);
    libpath.assign(std::string(ENIGMA_STEAMWORKS_EXTENSION_ROOT) +
                   "/gameclient/steambinder/Steamv157/sdk/redistributable_bin/linux64/libsteam_api.so");
  }

  if (!fs::exists(libpath)) {
    DEBUG_MESSAGE("Something went wrong while reading Steamworks SDK. Switching to fake SDK...", M_WARNING);
    libpath.assign(std::string(ENIGMA_STEAMWORKS_EXTENSION_ROOT) +
                   "/gameclient/steambinder/SteamFake/sdk/redistributable_bin/linux64/libfake_steam_api.so");
    if (!fs::exists(libpath)) {
      DEBUG_MESSAGE("Something went wrong while reading fake SDK. Aborting...", M_ERROR);
      return false;
    }
  }

  void* handle = dlopen(libpath.c_str(), RTLD_LAZY);

  SteamBinder::Init = reinterpret_cast<Init_t>(dlsym(handle, "SteamAPI_Init"));

  SteamBinder::Shutdown = reinterpret_cast<Shutdown_t>(dlsym(handle, "SteamAPI_Shutdown"));

  SteamBinder::RestartAppIfNecessary =
      reinterpret_cast<RestartAppIfNecessary_t>(dlsym(handle, "SteamAPI_RestartAppIfNecessary"));

  SteamBinder::RunCallbacks = reinterpret_cast<RunCallbacks_t>(dlsym(handle, "SteamAPI_RunCallbacks"));

  SteamBinder::RegisterCallback = reinterpret_cast<RegisterCallback_t>(dlsym(handle, "SteamAPI_RegisterCallback"));
  SteamBinder::UnregisterCallback =
      reinterpret_cast<UnregisterCallback_t>(dlsym(handle, "SteamAPI_UnregisterCallback"));

  SteamBinder::RegisterCallResult =
      reinterpret_cast<RegisterCallResult_t>(dlsym(handle, "SteamAPI_RegisterCallResult"));
  SteamBinder::UnregisterCallResult =
      reinterpret_cast<UnregisterCallResult_t>(dlsym(handle, "SteamAPI_UnregisterCallResult"));

  SteamBinder::SteamUser_vXXX = reinterpret_cast<SteamUser_vXXX_t>(dlsym(handle, VERSIONED_STEAM_USER_ACCESSOR_NAME));

  SteamBinder::ISteamUser_BLoggedOn =
      reinterpret_cast<ISteamUser_BLoggedOn_t>(dlsym(handle, "SteamAPI_ISteamUser_BLoggedOn"));

  SteamBinder::ISteamUser_GetSteamID =
      reinterpret_cast<ISteamUser_GetSteamID_t>(dlsym(handle, "SteamAPI_ISteamUser_GetSteamID"));

  SteamBinder::SteamFriends_vXXX =
      reinterpret_cast<SteamFriends_vXXX_t>(dlsym(handle, VERSIONED_STEAM_FRIENDS_ACCESSOR_NAME));

  SteamBinder::ISteamFriends_GetPersonaName =
      reinterpret_cast<ISteamFriends_GetPersonaName_t>(dlsym(handle, "SteamAPI_ISteamFriends_GetPersonaName"));
  SteamBinder::ISteamFriends_GetFriendPersonaName = reinterpret_cast<ISteamFriends_GetFriendPersonaName_t>(
      dlsym(handle, "SteamAPI_ISteamFriends_GetFriendPersonaName"));
  SteamBinder::ISteamFriends_ActivateGameOverlay = reinterpret_cast<ISteamFriends_ActivateGameOverlay_t>(
      dlsym(handle, "SteamAPI_ISteamFriends_ActivateGameOverlay"));
#ifdef ENIGMA_FAKE_STEAMWORKS_API
  SteamBinder::ISteamFriends_DeactivateGameOverlay = reinterpret_cast<ISteamFriends_DeactivateGameOverlay_t>(
      dlsym(handle, "SteamAPI_ISteamFriends_DeactivateGameOverlay"));  // Not part of the official API.
#endif                                                                 // ENIGMA_FAKE_STEAMWORKS_API
  SteamBinder::ISteamFriends_ActivateGameOverlayToUser = reinterpret_cast<ISteamFriends_ActivateGameOverlayToUser_t>(
      dlsym(handle, "SteamAPI_ISteamFriends_ActivateGameOverlayToUser"));
  SteamBinder::ISteamFriends_ActivateGameOverlayToWebPage =
      reinterpret_cast<ISteamFriends_ActivateGameOverlayToWebPage_t>(
          dlsym(handle, "SteamAPI_ISteamFriends_ActivateGameOverlayToWebPage"));
  SteamBinder::ISteamFriends_ActivateGameOverlayToStore = reinterpret_cast<ISteamFriends_ActivateGameOverlayToStore_t>(
      dlsym(handle, "SteamAPI_ISteamFriends_ActivateGameOverlayToStore"));
  SteamBinder::ISteamFriends_SetPlayedWith =
      reinterpret_cast<ISteamFriends_SetPlayedWith_t>(dlsym(handle, "SteamAPI_ISteamFriends_SetPlayedWith"));
  SteamBinder::ISteamFriends_GetSmallFriendAvatar = reinterpret_cast<ISteamFriends_GetSmallFriendAvatar_t>(
      dlsym(handle, "SteamAPI_ISteamFriends_GetSmallFriendAvatar"));
  SteamBinder::ISteamFriends_GetMediumFriendAvatar = reinterpret_cast<ISteamFriends_GetMediumFriendAvatar_t>(
      dlsym(handle, "SteamAPI_ISteamFriends_GetMediumFriendAvatar"));
  SteamBinder::ISteamFriends_GetLargeFriendAvatar = reinterpret_cast<ISteamFriends_GetLargeFriendAvatar_t>(
      dlsym(handle, "SteamAPI_ISteamFriends_GetLargeFriendAvatar"));
  SteamBinder::ISteamFriends_SetRichPresence =
      reinterpret_cast<ISteamFriends_SetRichPresence_t>(dlsym(handle, "SteamAPI_ISteamFriends_SetRichPresence"));
  SteamBinder::ISteamFriends_ClearRichPresence =
      reinterpret_cast<ISteamFriends_ClearRichPresence_t>(dlsym(handle, "SteamAPI_ISteamFriends_ClearRichPresence"));

  SteamBinder::SteamUtils_vXXX =
      reinterpret_cast<SteamUtils_vXXX_t>(dlsym(handle, VERSIONED_STEAM_UTILS_ACCESSOR_NAME));

  SteamBinder::ISteamUtils_GetImageSize =
      reinterpret_cast<ISteamUtils_GetImageSize_t>(dlsym(handle, "SteamAPI_ISteamUtils_GetImageSize"));
  SteamBinder::ISteamUtils_GetImageRGBA =
      reinterpret_cast<ISteamUtils_GetImageRGBA_t>(dlsym(handle, "SteamAPI_ISteamUtils_GetImageRGBA"));
  SteamBinder::ISteamUtils_GetAppID =
      reinterpret_cast<ISteamUtils_GetAppID_t>(dlsym(handle, "SteamAPI_ISteamUtils_GetAppID"));
  SteamBinder::ISteamUtils_SetOverlayNotificationPosition =
      reinterpret_cast<ISteamUtils_SetOverlayNotificationPosition_t>(
          dlsym(handle, "SteamAPI_ISteamUtils_SetOverlayNotificationPosition"));
  SteamBinder::ISteamUtils_SetWarningMessageHook = reinterpret_cast<ISteamUtils_SetWarningMessageHook_t>(
      dlsym(handle, "SteamAPI_ISteamUtils_SetWarningMessageHook"));
  SteamBinder::ISteamUtils_IsOverlayEnabled =
      reinterpret_cast<ISteamUtils_IsOverlayEnabled_t>(dlsym(handle, "SteamAPI_ISteamUtils_IsOverlayEnabled"));
  SteamBinder::ISteamUtils_SetOverlayNotificationInset = reinterpret_cast<ISteamUtils_SetOverlayNotificationInset_t>(
      dlsym(handle, "SteamAPI_ISteamUtils_SetOverlayNotificationInset"));

  SteamBinder::SteamUserStats_vXXX =
      reinterpret_cast<SteamUserStats_vXXX_t>(dlsym(handle, VERSIONED_STEAM_USERSTATS_ACCESSOR_NAME));

  SteamBinder::ISteamUserStats_RequestCurrentStats = reinterpret_cast<ISteamUserStats_RequestCurrentStats_t>(
      dlsym(handle, "SteamAPI_ISteamUserStats_RequestCurrentStats"));
  SteamBinder::ISteamUserStats_GetStatInt32 =
      reinterpret_cast<ISteamUserStats_GetStatInt32_t>(dlsym(handle, "SteamAPI_ISteamUserStats_GetStatInt32"));
  SteamBinder::ISteamUserStats_GetStatFloat =
      reinterpret_cast<ISteamUserStats_GetStatFloat_t>(dlsym(handle, "SteamAPI_ISteamUserStats_GetStatFloat"));
  SteamBinder::ISteamUserStats_SetStatInt32 =
      reinterpret_cast<ISteamUserStats_SetStatInt32_t>(dlsym(handle, "SteamAPI_ISteamUserStats_SetStatInt32"));
  SteamBinder::ISteamUserStats_SetStatFloat =
      reinterpret_cast<ISteamUserStats_SetStatFloat_t>(dlsym(handle, "SteamAPI_ISteamUserStats_SetStatFloat"));
  SteamBinder::ISteamUserStats_UpdateAvgRateStat = reinterpret_cast<ISteamUserStats_UpdateAvgRateStat_t>(
      dlsym(handle, "SteamAPI_ISteamUserStats_UpdateAvgRateStat"));
  SteamBinder::ISteamUserStats_GetAchievement =
      reinterpret_cast<ISteamUserStats_GetAchievement_t>(dlsym(handle, "SteamAPI_ISteamUserStats_GetAchievement"));
  SteamBinder::ISteamUserStats_SetAchievement =
      reinterpret_cast<ISteamUserStats_SetAchievement_t>(dlsym(handle, "SteamAPI_ISteamUserStats_SetAchievement"));
  SteamBinder::ISteamUserStats_ClearAchievement =
      reinterpret_cast<ISteamUserStats_ClearAchievement_t>(dlsym(handle, "SteamAPI_ISteamUserStats_ClearAchievement"));
  SteamBinder::ISteamUserStats_StoreStats =
      reinterpret_cast<ISteamUserStats_StoreStats_t>(dlsym(handle, "SteamAPI_ISteamUserStats_StoreStats"));
  SteamBinder::ISteamUserStats_ResetAllStats =
      reinterpret_cast<ISteamUserStats_ResetAllStats_t>(dlsym(handle, "SteamAPI_ISteamUserStats_ResetAllStats"));
  SteamBinder::ISteamUserStats_FindOrCreateLeaderboard = reinterpret_cast<ISteamUserStats_FindOrCreateLeaderboard_t>(
      dlsym(handle, "SteamAPI_ISteamUserStats_FindOrCreateLeaderboard"));
  SteamBinder::ISteamUserStats_FindLeaderboard =
      reinterpret_cast<ISteamUserStats_FindLeaderboard_t>(dlsym(handle, "SteamAPI_ISteamUserStats_FindLeaderboard"));
  SteamBinder::ISteamUserStats_GetLeaderboardName = reinterpret_cast<ISteamUserStats_GetLeaderboardName_t>(
      dlsym(handle, "SteamAPI_ISteamUserStats_GetLeaderboardName"));
  SteamBinder::ISteamUserStats_DownloadLeaderboardEntries =
      reinterpret_cast<ISteamUserStats_DownloadLeaderboardEntries_t>(
          dlsym(handle, "SteamAPI_ISteamUserStats_DownloadLeaderboardEntries"));
  SteamBinder::ISteamUserStats_GetDownloadedLeaderboardEntry =
      reinterpret_cast<ISteamUserStats_GetDownloadedLeaderboardEntry_t>(
          dlsym(handle, "SteamAPI_ISteamUserStats_GetDownloadedLeaderboardEntry"));
  SteamBinder::ISteamUserStats_UploadLeaderboardScore = reinterpret_cast<ISteamUserStats_UploadLeaderboardScore_t>(
      dlsym(handle, "SteamAPI_ISteamUserStats_UploadLeaderboardScore"));

  SteamBinder::SteamApps_vXXX = reinterpret_cast<SteamApps_vXXX_t>(dlsym(handle, VERSIONED_STEAM_APPS_ACCESSOR_NAME));

  SteamBinder::ISteamApps_BIsSubscribed =
      reinterpret_cast<ISteamApps_BIsSubscribed_t>(dlsym(handle, "SteamAPI_ISteamApps_BIsSubscribed"));
  SteamBinder::ISteamApps_GetCurrentGameLanguage = reinterpret_cast<ISteamApps_GetCurrentGameLanguage_t>(
      dlsym(handle, "SteamAPI_ISteamApps_GetCurrentGameLanguage"));
  SteamBinder::ISteamApps_GetAvailableGameLanguages = reinterpret_cast<ISteamApps_GetAvailableGameLanguages_t>(
      dlsym(handle, "SteamAPI_ISteamApps_GetAvailableGameLanguages"));

  return SteamBinder::validate();
}

bool SteamBinder::validate() {
  assert(SteamBinder::Init != nullptr && "Failed to load SteamAPI_Init.");
  assert(SteamBinder::Shutdown != nullptr && "Failed to load SteamAPI_Shutdown.");
  assert(SteamBinder::RestartAppIfNecessary != nullptr && "Failed to load SteamAPI_RestartAppIfNecessary.");
  assert(SteamBinder::RunCallbacks != nullptr && "Failed to load SteamAPI_RunCallbacks.");
  assert(SteamBinder::RegisterCallback != nullptr && "Failed to load SteamAPI_RegisterCallback.");
  assert(SteamBinder::UnregisterCallback != nullptr && "Failed to load SteamAPI_UnregisterCallback.");
  assert(SteamBinder::RegisterCallResult != nullptr && "Failed to load SteamAPI_RegisterCallResult.");
  assert(SteamBinder::UnregisterCallResult != nullptr && "Failed to load SteamAPI_UnregisterCallResult.");
  assert(SteamBinder::SteamUser_vXXX != nullptr &&
         ("Failed to load " + std::string(VERSIONED_STEAM_USER_ACCESSOR_NAME) + ".").c_str());
  assert(SteamBinder::ISteamUser_BLoggedOn != nullptr && "Failed to load SteamAPI_ISteamUser_BLoggedOn.");
  assert(SteamBinder::ISteamUser_GetSteamID != nullptr && "Failed to load SteamAPI_ISteamUser_GetSteamID.");
  assert(SteamBinder::SteamFriends_vXXX != nullptr &&
         ("Failed to load " + std::string(VERSIONED_STEAM_FRIENDS_ACCESSOR_NAME) + ".").c_str());
  assert(SteamBinder::ISteamFriends_GetPersonaName != nullptr &&
         "Failed to load SteamAPI_ISteamFriends_GetPersonaName.");
  assert(SteamBinder::ISteamFriends_GetFriendPersonaName != nullptr &&
         "Failed to load SteamAPI_ISteamFriends_GetFriendPersonaName.");
  assert(SteamBinder::ISteamFriends_ActivateGameOverlay != nullptr &&
         "Failed to load SteamAPI_ISteamFriends_ActivateGameOverlay.");
#ifdef ENIGMA_FAKE_STEAMWORKS_API
  assert(SteamBinder::ISteamFriends_DeactivateGameOverlay != nullptr &&
         "Failed to load SteamAPI_ISteamFriends_DeactivateGameOverlay.");  // Not part of the official API.
#endif                                                                     // ENIGMA_FAKE_STEAMWORKS_API
  assert(SteamBinder::ISteamFriends_ActivateGameOverlayToUser != nullptr &&
         "Failed to load SteamAPI_ISteamFriends_ActivateGameOverlayToUser.");
  assert(SteamBinder::ISteamFriends_ActivateGameOverlayToWebPage != nullptr &&
         "Failed to load SteamAPI_ISteamFriends_ActivateGameOverlayToWebPage.");
  assert(SteamBinder::ISteamFriends_ActivateGameOverlayToStore != nullptr &&
         "Failed to load SteamAPI_ISteamFriends_ActivateGameOverlayToStore.");
  assert(SteamBinder::ISteamFriends_SetPlayedWith != nullptr && "Failed to load SteamAPI_ISteamFriends_SetPlayedWith.");
  assert(SteamBinder::ISteamFriends_GetSmallFriendAvatar != nullptr &&
         "Failed to load SteamAPI_ISteamFriends_GetSmallFriendAvatar.");
  assert(SteamBinder::ISteamFriends_GetMediumFriendAvatar != nullptr &&
         "Failed to load SteamAPI_ISteamFriends_GetMediumFriendAvatar.");
  assert(SteamBinder::ISteamFriends_GetLargeFriendAvatar != nullptr &&
         "Failed to load SteamAPI_ISteamFriends_GetLargeFriendAvatar.");
  assert(SteamBinder::ISteamFriends_SetRichPresence != nullptr &&
         "Failed to load SteamAPI_ISteamFriends_SetRichPresence.");
  assert(SteamBinder::ISteamFriends_ClearRichPresence != nullptr &&
         "Failed to load SteamAPI_ISteamFriends_ClearRichPresence.");
  assert(SteamBinder::SteamUtils_vXXX != nullptr &&
         ("Failed to load " + std::string(VERSIONED_STEAM_UTILS_ACCESSOR_NAME) + ".").c_str());
  assert(SteamBinder::ISteamUtils_GetImageSize != nullptr && "Failed to load SteamAPI_ISteamUtils_GetImageSize.");
  assert(SteamBinder::ISteamUtils_GetImageRGBA != nullptr && "Failed to load SteamAPI_ISteamUtils_GetImageRGBA.");
  assert(SteamBinder::ISteamUtils_GetAppID != nullptr && "Failed to load SteamAPI_ISteamUtils_GetAppID.");
  assert(SteamBinder::ISteamUtils_SetOverlayNotificationPosition != nullptr &&
         "Failed to load SteamAPI_ISteamUtils_SetOverlayNotificationPosition.");
  assert(SteamBinder::ISteamUtils_SetWarningMessageHook != nullptr &&
         "Failed to load SteamAPI_ISteamUtils_SetWarningMessageHook.");
  assert(SteamBinder::ISteamUtils_IsOverlayEnabled != nullptr &&
         "Failed to load SteamAPI_ISteamUtils_IsOverlayEnabled.");
  assert(SteamBinder::ISteamUtils_SetOverlayNotificationInset != nullptr &&
         "Failed to load SteamAPI_ISteamUtils_SetOverlayNotificationInset.");
  assert(SteamBinder::SteamUserStats_vXXX != nullptr &&
         ("Failed to load " + std::string(VERSIONED_STEAM_USERSTATS_ACCESSOR_NAME) + ".").c_str());
  assert(SteamBinder::ISteamUserStats_RequestCurrentStats != nullptr &&
         "Failed to load SteamAPI_ISteamUserStats_RequestCurrentStats.");
  assert(SteamBinder::ISteamUserStats_GetStatInt32 != nullptr &&
         "Failed to load SteamAPI_ISteamUserStats_GetStatInt32.");
  assert(SteamBinder::ISteamUserStats_GetStatFloat != nullptr &&
         "Failed to load SteamAPI_ISteamUserStats_GetStatFloat.");
  assert(SteamBinder::ISteamUserStats_SetStatInt32 != nullptr &&
         "Failed to load SteamAPI_ISteamUserStats_SetStatInt32.");
  assert(SteamBinder::ISteamUserStats_SetStatFloat != nullptr &&
         "Failed to load SteamAPI_ISteamUserStats_SetStatFloat.");
  assert(SteamBinder::ISteamUserStats_UpdateAvgRateStat != nullptr &&
         "Failed to load SteamAPI_ISteamUserStats_UpdateAvgRateStat.");
  assert(SteamBinder::ISteamUserStats_GetAchievement != nullptr &&
         "Failed to load SteamAPI_ISteamUserStats_GetAchievement.");
  assert(SteamBinder::ISteamUserStats_SetAchievement != nullptr &&
         "Failed to load SteamAPI_ISteamUserStats_SetAchievement.");
  assert(SteamBinder::ISteamUserStats_ClearAchievement != nullptr &&
         "Failed to load SteamAPI_ISteamUserStats_ClearAchievement.");
  assert(SteamBinder::ISteamUserStats_StoreStats != nullptr && "Failed to load SteamAPI_ISteamUserStats_StoreStats.");
  assert(SteamBinder::ISteamUserStats_ResetAllStats != nullptr &&
         "Failed to load SteamAPI_ISteamUserStats_ResetAllStats.");
  assert(SteamBinder::ISteamUserStats_FindOrCreateLeaderboard != nullptr &&
         "Failed to load SteamAPI_ISteamUserStats_FindOrCreateLeaderboard.");
  assert(SteamBinder::ISteamUserStats_FindLeaderboard != nullptr &&
         "Failed to load SteamAPI_ISteamUserStats_FindLeaderboard.");
  assert(SteamBinder::ISteamUserStats_GetLeaderboardName != nullptr &&
         "Failed to load SteamAPI_ISteamUserStats_GetLeaderboardName.");
  assert(SteamBinder::ISteamUserStats_DownloadLeaderboardEntries != nullptr &&
         "Failed to load SteamAPI_ISteamUserStats_DownloadLeaderboardEntries.");
  assert(SteamBinder::ISteamUserStats_GetDownloadedLeaderboardEntry != nullptr &&
         "Failed to load SteamAPI_ISteamUserStats_GetDownloadedLeaderboardEntry.");
  assert(SteamBinder::ISteamUserStats_UploadLeaderboardScore != nullptr &&
         "Failed to load SteamAPI_ISteamUserStats_UploadLeaderboardScore.");
  assert(SteamBinder::SteamApps_vXXX != nullptr &&
         ("Failed to load " + std::string(VERSIONED_STEAM_APPS_ACCESSOR_NAME) + ".").c_str());
  assert(SteamBinder::ISteamApps_BIsSubscribed != nullptr && "Failed to load SteamAPI_ISteamApps_BIsSubscribed.");
  assert(SteamBinder::ISteamApps_GetCurrentGameLanguage != nullptr &&
         "Failed to load SteamAPI_ISteamApps_GetCurrentGameLanguage.");
  assert(SteamBinder::ISteamApps_GetAvailableGameLanguages != nullptr &&
         "Failed to load SteamAPI_ISteamApps_GetAvailableGameLanguages.");
  return true;
}

}  // namespace steamworks_b
