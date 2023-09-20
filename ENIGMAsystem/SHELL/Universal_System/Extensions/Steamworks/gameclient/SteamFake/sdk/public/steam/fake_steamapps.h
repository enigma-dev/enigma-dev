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

#ifndef FAKE_STEAMAPPS_H
#define FAKE_STEAMAPPS_H

#include "fake_steam_api.h"

class FakeSteamApps : public ISteamApps {
 public:
  static FakeSteamApps *GetInstance() {
    static FakeSteamApps instance;
    return &instance;
  }

  bool BIsSubscribed() override;
  bool BIsLowViolence() override;
  bool BIsCybercafe() override;
  bool BIsVACBanned() override;
  const char *GetCurrentGameLanguage() override;
  const char *GetAvailableGameLanguages() override;

  bool BIsSubscribedApp(AppId_t appID) override;

  bool BIsDlcInstalled(AppId_t appID) override;

  uint32 GetEarliestPurchaseUnixTime(AppId_t nAppID) override;

  bool BIsSubscribedFromFreeWeekend() override;

  int GetDLCCount() override;

  bool BGetDLCDataByIndex(int iDLC, AppId_t *pAppID, bool *pbAvailable, char *pchName, int cchNameBufferSize) override;

  void InstallDLC(AppId_t nAppID) override;
  void UninstallDLC(AppId_t nAppID) override;

  void RequestAppProofOfPurchaseKey(AppId_t nAppID) override;

  bool GetCurrentBetaName(char *pchName, int cchNameBufferSize) override;
  bool MarkContentCorrupt(bool bMissingFilesOnly) override;
  uint32 GetInstalledDepots(AppId_t appID, DepotId_t *pvecDepots, uint32 cMaxDepots) override;

  uint32 GetAppInstallDir(AppId_t appID, char *pchFolder, uint32 cchFolderBufferSize) override;
  bool BIsAppInstalled(AppId_t appID) override;

  CSteamID GetAppOwner() override;

  const char *GetLaunchQueryParam(const char *pchKey) override;

  bool GetDlcDownloadProgress(AppId_t nAppID, uint64 *punBytesDownloaded, uint64 *punBytesTotal) override;

  int GetAppBuildId() override;

  void RequestAllProofOfPurchaseKeys() override;

  SteamAPICall_t GetFileDetails(const char *pszFileName) override;

  int GetLaunchCommandLine(char *pszCommandLine, int cubCommandLine) override;

  bool BIsSubscribedFromFamilySharing() override;

  bool BIsTimedTrial(uint32 *punSecondsAllowed, uint32 *punSecondsPlayed) override;

  bool SetDlcContext(AppId_t nAppID) override;

 private:
  FakeSteamApps() {}
  ~FakeSteamApps() {}

  FakeSteamApps(FakeSteamApps const &) = delete;
  void operator=(FakeSteamApps const &) = delete;
};

#endif  // FAKE_STEAMAPPS_H
