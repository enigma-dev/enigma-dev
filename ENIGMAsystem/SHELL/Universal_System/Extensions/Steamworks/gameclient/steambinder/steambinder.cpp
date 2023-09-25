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

#include <iostream>

namespace steamworks_b {

Init_t Binder::Init{nullptr};
Shutdown_t Binder::Shutdown{nullptr};
RestartAppIfNecessary_t Binder::RestartAppIfNecessary{nullptr};
RunCallbacks_t Binder::RunCallbacks{nullptr};

RegisterCallback_t Binder::RegisterCallback{nullptr};
UnregisterCallback_t Binder::UnregisterCallback{nullptr};

RegisterCallResult_t Binder::RegisterCallResult{nullptr};
UnregisterCallResult_t Binder::UnregisterCallResult{nullptr};

SteamUser_v023_t Binder::SteamUser_v023{nullptr};

ISteamUser_BLoggedOn_t Binder::ISteamUser_BLoggedOn{nullptr};
ISteamUser_GetSteamID_t Binder::ISteamUser_GetSteamID{nullptr};

SteamFriends_v017_t Binder::SteamFriends_v017{nullptr};

ISteamFriends_GetPersonaName_t Binder::ISteamFriends_GetPersonaName{nullptr};
ISteamFriends_GetFriendPersonaName_t Binder::ISteamFriends_GetFriendPersonaName{nullptr};
ISteamFriends_ActivateGameOverlay_t Binder::ISteamFriends_ActivateGameOverlay{nullptr};
ISteamFriends_ActivateGameOverlayToUser_t Binder::ISteamFriends_ActivateGameOverlayToUser{nullptr};
ISteamFriends_ActivateGameOverlayToWebPage_t Binder::ISteamFriends_ActivateGameOverlayToWebPage{nullptr};
ISteamFriends_ActivateGameOverlayToStore_t Binder::ISteamFriends_ActivateGameOverlayToStore{nullptr};
ISteamFriends_SetPlayedWith_t Binder::ISteamFriends_SetPlayedWith{nullptr};
ISteamFriends_ActivateGameOverlayInviteDialog_t Binder::ISteamFriends_ActivateGameOverlayInviteDialog{nullptr};
ISteamFriends_GetSmallFriendAvatar_t Binder::ISteamFriends_GetSmallFriendAvatar{nullptr};
ISteamFriends_GetMediumFriendAvatar_t Binder::ISteamFriends_GetMediumFriendAvatar{nullptr};
ISteamFriends_GetLargeFriendAvatar_t Binder::ISteamFriends_GetLargeFriendAvatar{nullptr};
ISteamFriends_SetRichPresence_t Binder::ISteamFriends_SetRichPresence{nullptr};
ISteamFriends_ClearRichPresence_t Binder::ISteamFriends_ClearRichPresence{nullptr};

SteamUtils_v010_t Binder::SteamUtils_v010{nullptr};

ISteamUtils_GetImageSize_t Binder::ISteamUtils_GetImageSize{nullptr};
ISteamUtils_GetImageRGBA_t Binder::ISteamUtils_GetImageRGBA{nullptr};
ISteamUtils_GetAppID_t Binder::ISteamUtils_GetAppID{nullptr};
ISteamUtils_SetOverlayNotificationPosition_t Binder::ISteamUtils_SetOverlayNotificationPosition{nullptr};
ISteamUtils_SetWarningMessageHook_t Binder::ISteamUtils_SetWarningMessageHook{nullptr};
ISteamUtils_IsOverlayEnabled_t Binder::ISteamUtils_IsOverlayEnabled{nullptr};
ISteamUtils_SetOverlayNotificationInset_t Binder::ISteamUtils_SetOverlayNotificationInset{nullptr};

SteamUserStats_v012_t Binder::SteamUserStats_v012{nullptr};

ISteamUserStats_RequestCurrentStats_t Binder::ISteamUserStats_RequestCurrentStats{nullptr};
ISteamUserStats_GetStatInt32_t Binder::ISteamUserStats_GetStatInt32{nullptr};
ISteamUserStats_GetStatFloat_t Binder::ISteamUserStats_GetStatFloat{nullptr};
ISteamUserStats_SetStatInt32_t Binder::ISteamUserStats_SetStatInt32{nullptr};
ISteamUserStats_SetStatFloat_t Binder::ISteamUserStats_SetStatFloat{nullptr};
ISteamUserStats_UpdateAvgRateStat_t Binder::ISteamUserStats_UpdateAvgRateStat{nullptr};
ISteamUserStats_GetAchievement_t Binder::ISteamUserStats_GetAchievement{nullptr};
ISteamUserStats_SetAchievement_t Binder::ISteamUserStats_SetAchievement{nullptr};
ISteamUserStats_ClearAchievement_t Binder::ISteamUserStats_ClearAchievement{nullptr};
ISteamUserStats_StoreStats_t Binder::ISteamUserStats_StoreStats{nullptr};
ISteamUserStats_ResetAllStats_t Binder::ISteamUserStats_ResetAllStats{nullptr};
ISteamUserStats_FindOrCreateLeaderboard_t Binder::ISteamUserStats_FindOrCreateLeaderboard{nullptr};
ISteamUserStats_FindLeaderboard_t Binder::ISteamUserStats_FindLeaderboard{nullptr};
ISteamUserStats_GetLeaderboardName_t Binder::ISteamUserStats_GetLeaderboardName{nullptr};
ISteamUserStats_GetLeaderboardEntryCount_t Binder::ISteamUserStats_GetLeaderboardEntryCount{nullptr};
ISteamUserStats_DownloadLeaderboardEntries_t Binder::ISteamUserStats_DownloadLeaderboardEntries{nullptr};
ISteamUserStats_GetDownloadedLeaderboardEntry_t Binder::ISteamUserStats_GetDownloadedLeaderboardEntry{nullptr};
ISteamUserStats_UploadLeaderboardScore_t Binder::ISteamUserStats_UploadLeaderboardScore{nullptr};

SteamApps_v008_t Binder::SteamApps_v008{nullptr};

ISteamApps_BIsSubscribed_t Binder::ISteamApps_BIsSubscribed{nullptr};
ISteamApps_GetCurrentGameLanguage_t Binder::ISteamApps_GetCurrentGameLanguage{nullptr};
ISteamApps_GetAvailableGameLanguages_t Binder::ISteamApps_GetAvailableGameLanguages{nullptr};

bool Binder::bind() {
  // get env var STEAM_SDK_PATH

  // Check if the library exists.
  fs::path libpath(
      "/home/saif/Desktop/enigma-dev/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/gameclient/steambinder/"
      "Steamv157/sdk/redistributable_bin/linux64/libsteam_api.so");

  if (!fs::exists(libpath)) {
    libpath.assign(
        "/home/saif/Desktop/enigma-dev/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/gameclient/"
        "steambinder/SteamFake/sdk/redistributable_bin/linux64/libfake_steam_api.so");
  }

  void *handle = dlopen(libpath.c_str(), RTLD_LAZY);

  Binder::Init = reinterpret_cast<Init_t>(dlsym(handle, "SteamAPI_Init"));

  //   std::cout << "Init: " << std::to_string(Binder::Init == nullptr) << std::endl;  /////////////////

  Binder::Shutdown = reinterpret_cast<Shutdown_t>(dlsym(handle, "SteamAPI_Shutdown"));

  //   std::cout << "Shutdown: " << std::to_string(Binder::Shutdown == nullptr) << std::endl;  //////////////////

  Binder::RestartAppIfNecessary =
      reinterpret_cast<RestartAppIfNecessary_t>(dlsym(handle, "SteamAPI_RestartAppIfNecessary"));

  //   std::cout << "RestartAppIfNecessary: " << std::to_string(Binder::RestartAppIfNecessary == nullptr)
  //             << std::endl;  /////////////////

  Binder::RunCallbacks = reinterpret_cast<RunCallbacks_t>(dlsym(handle, "SteamAPI_RunCallbacks"));

  //   std::cout << "RunCallbacks: " << std::to_string(Binder::RunCallbacks == nullptr) << std::endl;  /////////////////

  Binder::RegisterCallback = reinterpret_cast<RegisterCallback_t>(dlsym(handle, "SteamAPI_RegisterCallback"));
  Binder::UnregisterCallback = reinterpret_cast<UnregisterCallback_t>(dlsym(handle, "SteamAPI_UnregisterCallback"));

  Binder::RegisterCallResult = reinterpret_cast<RegisterCallResult_t>(dlsym(handle, "SteamAPI_RegisterCallResult"));
  Binder::UnregisterCallResult =
      reinterpret_cast<UnregisterCallResult_t>(dlsym(handle, "SteamAPI_UnregisterCallResult"));

  Binder::SteamUser_v023 = reinterpret_cast<SteamUser_v023_t>(dlsym(handle, "SteamAPI_SteamUser_v023"));

  //   std::cout << "SteamUser_v023: " << std::to_string(Binder::SteamUser_v023 == nullptr) << std::endl;  /////////////////

  Binder::ISteamUser_BLoggedOn =
      reinterpret_cast<ISteamUser_BLoggedOn_t>(dlsym(handle, "SteamAPI_ISteamUser_BLoggedOn"));

  //   std::cout << "ISteamUser_BLoggedOn: " << std::to_string(Binder::ISteamUser_BLoggedOn == nullptr)
  //             << std::endl;  /////////////////

  Binder::ISteamUser_GetSteamID =
      reinterpret_cast<ISteamUser_GetSteamID_t>(dlsym(handle, "SteamAPI_ISteamUser_GetSteamID"));

  //   std::cout << "ISteamUser_GetSteamID: " << std::to_string(Binder::ISteamUser_GetSteamID == nullptr)
  //             << std::endl;  /////////////////

  Binder::SteamFriends_v017 = reinterpret_cast<SteamFriends_v017_t>(dlsym(handle, "SteamAPI_SteamFriends_v017"));

  Binder::ISteamFriends_GetPersonaName =
      reinterpret_cast<ISteamFriends_GetPersonaName_t>(dlsym(handle, "SteamAPI_ISteamFriends_GetPersonaName"));
  Binder::ISteamFriends_GetFriendPersonaName = reinterpret_cast<ISteamFriends_GetFriendPersonaName_t>(
      dlsym(handle, "SteamAPI_ISteamFriends_GetFriendPersonaName"));
  Binder::ISteamFriends_ActivateGameOverlay = reinterpret_cast<ISteamFriends_ActivateGameOverlay_t>(
      dlsym(handle, "SteamAPI_ISteamFriends_ActivateGameOverlay"));
  Binder::ISteamFriends_ActivateGameOverlayToUser = reinterpret_cast<ISteamFriends_ActivateGameOverlayToUser_t>(
      dlsym(handle, "SteamAPI_ISteamFriends_ActivateGameOverlayToUser"));
  Binder::ISteamFriends_ActivateGameOverlayToWebPage = reinterpret_cast<ISteamFriends_ActivateGameOverlayToWebPage_t>(
      dlsym(handle, "SteamAPI_ISteamFriends_ActivateGameOverlayToWebPage"));
  Binder::ISteamFriends_ActivateGameOverlayToStore = reinterpret_cast<ISteamFriends_ActivateGameOverlayToStore_t>(
      dlsym(handle, "SteamAPI_ISteamFriends_ActivateGameOverlayToStore"));
  Binder::ISteamFriends_SetPlayedWith =
      reinterpret_cast<ISteamFriends_SetPlayedWith_t>(dlsym(handle, "SteamAPI_ISteamFriends_SetPlayedWith"));
  Binder::ISteamFriends_ActivateGameOverlayInviteDialog =
      reinterpret_cast<ISteamFriends_ActivateGameOverlayInviteDialog_t>(
          dlsym(handle, "SteamAPI_ISteamFriends_ActivateGameOverlayInviteDialog"));
  Binder::ISteamFriends_GetSmallFriendAvatar = reinterpret_cast<ISteamFriends_GetSmallFriendAvatar_t>(
      dlsym(handle, "SteamAPI_ISteamFriends_GetSmallFriendAvatar"));
  Binder::ISteamFriends_GetMediumFriendAvatar = reinterpret_cast<ISteamFriends_GetMediumFriendAvatar_t>(
      dlsym(handle, "SteamAPI_ISteamFriends_GetMediumFriendAvatar"));
  Binder::ISteamFriends_GetLargeFriendAvatar = reinterpret_cast<ISteamFriends_GetLargeFriendAvatar_t>(
      dlsym(handle, "SteamAPI_ISteamFriends_GetLargeFriendAvatar"));
  Binder::ISteamFriends_SetRichPresence =
      reinterpret_cast<ISteamFriends_SetRichPresence_t>(dlsym(handle, "SteamAPI_ISteamFriends_SetRichPresence"));
  Binder::ISteamFriends_ClearRichPresence =
      reinterpret_cast<ISteamFriends_ClearRichPresence_t>(dlsym(handle, "SteamAPI_ISteamFriends_ClearRichPresence"));

  Binder::SteamUtils_v010 = reinterpret_cast<SteamUtils_v010_t>(dlsym(handle, "SteamAPI_SteamUtils_v010"));

  Binder::ISteamUtils_GetImageSize =
      reinterpret_cast<ISteamUtils_GetImageSize_t>(dlsym(handle, "SteamAPI_ISteamUtils_GetImageSize"));
  Binder::ISteamUtils_GetImageRGBA =
      reinterpret_cast<ISteamUtils_GetImageRGBA_t>(dlsym(handle, "SteamAPI_ISteamUtils_GetImageRGBA"));
  Binder::ISteamUtils_GetAppID =
      reinterpret_cast<ISteamUtils_GetAppID_t>(dlsym(handle, "SteamAPI_ISteamUtils_GetAppID"));
  Binder::ISteamUtils_SetOverlayNotificationPosition = reinterpret_cast<ISteamUtils_SetOverlayNotificationPosition_t>(
      dlsym(handle, "SteamAPI_ISteamUtils_SetOverlayNotificationPosition"));
  Binder::ISteamUtils_SetWarningMessageHook = reinterpret_cast<ISteamUtils_SetWarningMessageHook_t>(
      dlsym(handle, "SteamAPI_ISteamUtils_SetWarningMessageHook"));
  Binder::ISteamUtils_IsOverlayEnabled =
      reinterpret_cast<ISteamUtils_IsOverlayEnabled_t>(dlsym(handle, "SteamAPI_ISteamUtils_IsOverlayEnabled"));
  Binder::ISteamUtils_SetOverlayNotificationInset = reinterpret_cast<ISteamUtils_SetOverlayNotificationInset_t>(
      dlsym(handle, "SteamAPI_ISteamUtils_SetOverlayNotificationInset"));

  Binder::SteamUserStats_v012 = reinterpret_cast<SteamUserStats_v012_t>(dlsym(handle, "SteamAPI_SteamUserStats_v012"));

  Binder::ISteamUserStats_RequestCurrentStats = reinterpret_cast<ISteamUserStats_RequestCurrentStats_t>(
      dlsym(handle, "SteamAPI_ISteamUserStats_RequestCurrentStats"));
  Binder::ISteamUserStats_GetStatInt32 =
      reinterpret_cast<ISteamUserStats_GetStatInt32_t>(dlsym(handle, "SteamAPI_ISteamUserStats_GetStatInt32"));
  Binder::ISteamUserStats_GetStatFloat =
      reinterpret_cast<ISteamUserStats_GetStatFloat_t>(dlsym(handle, "SteamAPI_ISteamUserStats_GetStatFloat"));
  Binder::ISteamUserStats_SetStatInt32 =
      reinterpret_cast<ISteamUserStats_SetStatInt32_t>(dlsym(handle, "SteamAPI_ISteamUserStats_SetStatInt32"));
  Binder::ISteamUserStats_SetStatFloat =
      reinterpret_cast<ISteamUserStats_SetStatFloat_t>(dlsym(handle, "SteamAPI_ISteamUserStats_SetStatFloat"));
  Binder::ISteamUserStats_UpdateAvgRateStat = reinterpret_cast<ISteamUserStats_UpdateAvgRateStat_t>(
      dlsym(handle, "SteamAPI_ISteamUserStats_UpdateAvgRateStat"));
  Binder::ISteamUserStats_GetAchievement =
      reinterpret_cast<ISteamUserStats_GetAchievement_t>(dlsym(handle, "SteamAPI_ISteamUserStats_GetAchievement"));
  Binder::ISteamUserStats_SetAchievement =
      reinterpret_cast<ISteamUserStats_SetAchievement_t>(dlsym(handle, "SteamAPI_ISteamUserStats_SetAchievement"));
  Binder::ISteamUserStats_ClearAchievement =
      reinterpret_cast<ISteamUserStats_ClearAchievement_t>(dlsym(handle, "SteamAPI_ISteamUserStats_ClearAchievement"));
  Binder::ISteamUserStats_StoreStats =
      reinterpret_cast<ISteamUserStats_StoreStats_t>(dlsym(handle, "SteamAPI_ISteamUserStats_StoreStats"));
  Binder::ISteamUserStats_ResetAllStats =
      reinterpret_cast<ISteamUserStats_ResetAllStats_t>(dlsym(handle, "SteamAPI_ISteamUserStats_ResetAllStats"));
  Binder::ISteamUserStats_FindOrCreateLeaderboard = reinterpret_cast<ISteamUserStats_FindOrCreateLeaderboard_t>(
      dlsym(handle, "SteamAPI_ISteamUserStats_FindOrCreateLeaderboard"));
  Binder::ISteamUserStats_FindLeaderboard =
      reinterpret_cast<ISteamUserStats_FindLeaderboard_t>(dlsym(handle, "SteamAPI_ISteamUserStats_FindLeaderboard"));
  Binder::ISteamUserStats_GetLeaderboardName = reinterpret_cast<ISteamUserStats_GetLeaderboardName_t>(
      dlsym(handle, "SteamAPI_ISteamUserStats_GetLeaderboardName"));
  Binder::ISteamUserStats_GetLeaderboardEntryCount = reinterpret_cast<ISteamUserStats_GetLeaderboardEntryCount_t>(
      dlsym(handle, "SteamAPI_ISteamUserStats_GetLeaderboardEntryCount"));
  Binder::ISteamUserStats_DownloadLeaderboardEntries = reinterpret_cast<ISteamUserStats_DownloadLeaderboardEntries_t>(
      dlsym(handle, "SteamAPI_ISteamUserStats_DownloadLeaderboardEntries"));
  Binder::ISteamUserStats_GetDownloadedLeaderboardEntry =
      reinterpret_cast<ISteamUserStats_GetDownloadedLeaderboardEntry_t>(
          dlsym(handle, "SteamAPI_ISteamUserStats_GetDownloadedLeaderboardEntry"));
  Binder::ISteamUserStats_UploadLeaderboardScore = reinterpret_cast<ISteamUserStats_UploadLeaderboardScore_t>(
      dlsym(handle, "SteamAPI_ISteamUserStats_UploadLeaderboardScore"));

  Binder::SteamApps_v008 = reinterpret_cast<SteamApps_v008_t>(dlsym(handle, "SteamAPI_SteamApps_v008"));

  Binder::ISteamApps_BIsSubscribed =
      reinterpret_cast<ISteamApps_BIsSubscribed_t>(dlsym(handle, "SteamAPI_ISteamApps_BIsSubscribed"));
  Binder::ISteamApps_GetCurrentGameLanguage = reinterpret_cast<ISteamApps_GetCurrentGameLanguage_t>(
      dlsym(handle, "SteamAPI_ISteamApps_GetCurrentGameLanguage"));
  Binder::ISteamApps_GetAvailableGameLanguages = reinterpret_cast<ISteamApps_GetAvailableGameLanguages_t>(
      dlsym(handle, "SteamAPI_ISteamApps_GetAvailableGameLanguages"));

  return true;
}

}  // namespace steamworks_b
