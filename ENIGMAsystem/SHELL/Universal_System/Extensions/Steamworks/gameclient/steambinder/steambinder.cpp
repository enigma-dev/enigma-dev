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

#include <cstdlib>
#include <iostream>

// TODO: Use ASSERT to check nullptrs here.

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
  // Check if the library exists.
  fs::path libpath(std::string(STEAM_SDK_PATH) + "/redistributable_bin/linux64/libsteam_api.so");

  if (!fs::exists(libpath)) {
    libpath.assign(std::string(STEAM_FAKE_SDK_PATH) + "/redistributable_bin/linux64/libfake_steam_api.so");
  }

  void *handle = dlopen(libpath.c_str(), RTLD_LAZY);

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

  return true;
}

}  // namespace steamworks_b
