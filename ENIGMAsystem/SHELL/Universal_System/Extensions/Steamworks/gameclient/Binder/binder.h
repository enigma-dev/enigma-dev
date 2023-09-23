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

#ifndef BINDER_H
#define BINDER_H

/*
    This is the lowest layer that game_client layer mainly depends on.
*/
#include "steam/steam_api.h"
#include "steam/steam_api_flat.h"

#include <dlfcn.h>
#include <filesystem>
// #include <cstdlib>

namespace fs = std::filesystem;

typedef bool (*Init_t)();
typedef void (*Shutdown_t)();
typedef bool (*RestartAppIfNecessary_t)(uint32);
typedef void (*RunCallbacks_t)();

typedef void* (*SteamUser_v023_t)();

typedef bool (*ISteamUser_BLoggedOn_t)(void*);
typedef uint64_steamid (*ISteamUser_GetSteamID_t)(void*);

typedef void* (*SteamFriends_v017_t)();

typedef const char* (*ISteamFriends_GetPersonaName_t)(void*);
typedef const char* (*ISteamFriends_GetFriendPersonaName_t)(void*, uint64_steamid);
typedef void (*ISteamFriends_ActivateGameOverlay_t)(void*, const char*);
typedef void (*ISteamFriends_ActivateGameOverlayToUser_t)(void*, const char*, uint64_steamid);
typedef void (*ISteamFriends_ActivateGameOverlayToWebPage_t)(void*, const char*, EActivateGameOverlayToWebPageMode);
typedef void (*ISteamFriends_ActivateGameOverlayToStore_t)(void*, AppId_t, EOverlayToStoreFlag);
typedef void (*ISteamFriends_SetPlayedWith_t)(void*, uint64_steamid);
typedef void (*ISteamFriends_ActivateGameOverlayInviteDialog_t)(void*, uint64_steamid);
typedef int (*ISteamFriends_GetSmallFriendAvatar_t)(void*, uint64_steamid);
typedef int (*ISteamFriends_GetMediumFriendAvatar_t)(void*, uint64_steamid);
typedef int (*ISteamFriends_GetLargeFriendAvatar_t)(void*, uint64_steamid);
typedef bool (*ISteamFriends_SetRichPresence_t)(void*, const char*, const char*);
typedef void (*ISteamFriends_ClearRichPresence_t)(void*);

typedef void* (*SteamUtils_v010_t)();

typedef bool (*ISteamUtils_GetImageSize_t)(void*, int, uint32*, uint32*);
typedef bool (*ISteamUtils_GetImageRGBA_t)(void*, int, uint8*, int);
typedef uint32 (*ISteamUtils_GetAppID_t)(void*);
typedef void (*ISteamUtils_SetOverlayNotificationPosition_t)(void*, ENotificationPosition);
typedef void (*ISteamUtils_SetWarningMessageHook_t)(void*, SteamAPIWarningMessageHook_t);
typedef bool (*ISteamUtils_IsOverlayEnabled_t)(void*);
typedef void (*ISteamUtils_SetOverlayNotificationInset_t)(void*, int, int);

typedef void* (*SteamUserStats_v012_t)();

typedef bool (*ISteamUserStats_RequestCurrentStats_t)(void*);
typedef bool (*ISteamUserStats_GetStatInt32_t)(void*, const char*, int32*);
typedef bool (*ISteamUserStats_GetStatFloat_t)(void*, const char*, float*);
typedef bool (*ISteamUserStats_SetStatInt32_t)(void*, const char*, int32);
typedef bool (*ISteamUserStats_SetStatFloat_t)(void*, const char*, float);
typedef bool (*ISteamUserStats_UpdateAvgRateStat_t)(void*, const char*, float, double);
typedef bool (*ISteamUserStats_GetAchievement_t)(void*, const char*, bool*);
typedef bool (*ISteamUserStats_SetAchievement_t)(void*, const char*);
typedef bool (*ISteamUserStats_ClearAchievement_t)(void*, const char*);
typedef bool (*ISteamUserStats_StoreStats_t)(void*);
typedef bool (*ISteamUserStats_ResetAllStats_t)(void*, bool);
typedef SteamAPICall_t (*ISteamUserStats_FindOrCreateLeaderboard_t)(void*, const char*, ELeaderboardSortMethod, ELeaderboardDisplayType);
typedef SteamAPICall_t (*ISteamUserStats_FindLeaderboard_t)(void*, const char*);
typedef const char* (*ISteamUserStats_GetLeaderboardName_t)(void*, SteamLeaderboard_t);
typedef int (*ISteamUserStats_GetLeaderboardEntryCount_t)(void*, SteamLeaderboard_t);
typedef SteamAPICall_t (*ISteamUserStats_DownloadLeaderboardEntries_t)(void*, SteamLeaderboard_t, ELeaderboardDataRequest, int, int);
typedef bool (*ISteamUserStats_GetDownloadedLeaderboardEntry_t)(void*, SteamLeaderboardEntries_t, int, LeaderboardEntry_t*, int32*, int);
typedef SteamAPICall_t (*ISteamUserStats_UploadLeaderboardScore_t)(void*, SteamLeaderboard_t, ELeaderboardUploadScoreMethod, int32, const int32*, int);

typedef void* (*SteamApps_v008_t)();

typedef bool (*ISteamApps_BIsSubscribed_t)(void*);
typedef const char* (*ISteamApps_GetCurrentGameLanguage_t)(void*);
typedef const char* (*ISteamApps_GetAvailableGameLanguages_t)(void*);

namespace steamworks_b {

class Binder {
 public:
  static Init_t Init;
  static Shutdown_t Shutdown;
  static RestartAppIfNecessary_t RestartAppIfNecessary;
  static RunCallbacks_t RunCallbacks;

  static bool bind();

  static SteamUser_v023_t SteamUser_v023;

  static ISteamUser_BLoggedOn_t ISteamUser_BLoggedOn;
  static ISteamUser_GetSteamID_t ISteamUser_GetSteamID;

  static SteamFriends_v017_t SteamFriends_v017;

  static ISteamFriends_GetPersonaName_t ISteamFriends_GetPersonaName;
  static ISteamFriends_GetFriendPersonaName_t ISteamFriends_GetFriendPersonaName;
  static ISteamFriends_ActivateGameOverlay_t ISteamFriends_ActivateGameOverlay;
  static ISteamFriends_ActivateGameOverlayToUser_t ISteamFriends_ActivateGameOverlayToUser;
  static ISteamFriends_ActivateGameOverlayToWebPage_t ISteamFriends_ActivateGameOverlayToWebPage;
  static ISteamFriends_ActivateGameOverlayToStore_t ISteamFriends_ActivateGameOverlayToStore;
  static ISteamFriends_SetPlayedWith_t ISteamFriends_SetPlayedWith;
  static ISteamFriends_ActivateGameOverlayInviteDialog_t ISteamFriends_ActivateGameOverlayInviteDialog;
  static ISteamFriends_GetSmallFriendAvatar_t ISteamFriends_GetSmallFriendAvatar;
  static ISteamFriends_GetMediumFriendAvatar_t ISteamFriends_GetMediumFriendAvatar;
  static ISteamFriends_GetLargeFriendAvatar_t ISteamFriends_GetLargeFriendAvatar;
  static ISteamFriends_SetRichPresence_t ISteamFriends_SetRichPresence;
  static ISteamFriends_ClearRichPresence_t ISteamFriends_ClearRichPresence;

  static SteamUtils_v010_t SteamUtils_v010;

  static ISteamUtils_GetImageSize_t ISteamUtils_GetImageSize;
  static ISteamUtils_GetImageRGBA_t ISteamUtils_GetImageRGBA;
  static ISteamUtils_GetAppID_t ISteamUtils_GetAppID;
  static ISteamUtils_SetOverlayNotificationPosition_t ISteamUtils_SetOverlayNotificationPosition;
  static ISteamUtils_SetWarningMessageHook_t ISteamUtils_SetWarningMessageHook;
  static ISteamUtils_IsOverlayEnabled_t ISteamUtils_IsOverlayEnabled;
  static ISteamUtils_SetOverlayNotificationInset_t ISteamUtils_SetOverlayNotificationInset;

  static SteamUserStats_v012_t SteamUserStats_v012;

  static ISteamUserStats_RequestCurrentStats_t ISteamUserStats_RequestCurrentStats;
  static ISteamUserStats_GetStatInt32_t ISteamUserStats_GetStatInt32;
  static ISteamUserStats_GetStatFloat_t ISteamUserStats_GetStatFloat;
  static ISteamUserStats_SetStatInt32_t ISteamUserStats_SetStatInt32;
  static ISteamUserStats_SetStatFloat_t ISteamUserStats_SetStatFloat;
  static ISteamUserStats_UpdateAvgRateStat_t ISteamUserStats_UpdateAvgRateStat;
  static ISteamUserStats_GetAchievement_t ISteamUserStats_GetAchievement;
  static ISteamUserStats_SetAchievement_t ISteamUserStats_SetAchievement;
  static ISteamUserStats_ClearAchievement_t ISteamUserStats_ClearAchievement;
  static ISteamUserStats_StoreStats_t ISteamUserStats_StoreStats;
  static ISteamUserStats_ResetAllStats_t ISteamUserStats_ResetAllStats;
  static ISteamUserStats_FindOrCreateLeaderboard_t ISteamUserStats_FindOrCreateLeaderboard;
  static ISteamUserStats_FindLeaderboard_t ISteamUserStats_FindLeaderboard;
  static ISteamUserStats_GetLeaderboardName_t ISteamUserStats_GetLeaderboardName;
  static ISteamUserStats_GetLeaderboardEntryCount_t ISteamUserStats_GetLeaderboardEntryCount;
  static ISteamUserStats_DownloadLeaderboardEntries_t ISteamUserStats_DownloadLeaderboardEntries;
  static ISteamUserStats_GetDownloadedLeaderboardEntry_t ISteamUserStats_GetDownloadedLeaderboardEntry;
  static ISteamUserStats_UploadLeaderboardScore_t ISteamUserStats_UploadLeaderboardScore;

  static SteamApps_v008_t SteamApps_v008;

  static ISteamApps_BIsSubscribed_t ISteamApps_BIsSubscribed;
  static ISteamApps_GetCurrentGameLanguage_t ISteamApps_GetCurrentGameLanguage;
  static ISteamApps_GetAvailableGameLanguages_t ISteamApps_GetAvailableGameLanguages;

 private:
  static bool bind_fake();
  static bool bind_real(fs::path libpath);
};

}  // namespace steamworks_b

#endif  // BINDER_H
