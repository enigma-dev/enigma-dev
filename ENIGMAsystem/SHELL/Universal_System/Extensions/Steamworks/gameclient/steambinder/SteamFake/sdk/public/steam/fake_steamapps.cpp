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

#include "fake_steamapps.h"

bool FakeSteamApps::BIsSubscribed() { return true; }
bool FakeSteamApps::BIsLowViolence() { return true; }
bool FakeSteamApps::BIsCybercafe() { return true; }
bool FakeSteamApps::BIsVACBanned() { return true; }
const char *FakeSteamApps::GetCurrentGameLanguage() { return "FakeLanguage"; }
const char *FakeSteamApps::GetAvailableGameLanguages() { return "FakeLanguages"; }

bool FakeSteamApps::BIsSubscribedApp(AppId_t appID) { return true; }

bool FakeSteamApps::BIsDlcInstalled(AppId_t appID) { return true; }

uint32 FakeSteamApps::GetEarliestPurchaseUnixTime(AppId_t nAppID) { return 0; }

bool FakeSteamApps::BIsSubscribedFromFreeWeekend() { return true; }

int FakeSteamApps::GetDLCCount() { return 0; }

bool FakeSteamApps::BGetDLCDataByIndex(int iDLC, AppId_t *pAppID, bool *pbAvailable, char *pchName,
                                       int cchNameBufferSize) {
  return true;
}

void FakeSteamApps::InstallDLC(AppId_t nAppID) {}
void FakeSteamApps::UninstallDLC(AppId_t nAppID) {}

void FakeSteamApps::RequestAppProofOfPurchaseKey(AppId_t nAppID) {}

bool FakeSteamApps::GetCurrentBetaName(char *pchName, int cchNameBufferSize) { return true; }
bool FakeSteamApps::MarkContentCorrupt(bool bMissingFilesOnly) { return true; }
uint32 FakeSteamApps::GetInstalledDepots(AppId_t appID, DepotId_t *pvecDepots, uint32 cMaxDepots) { return 0; }

uint32 FakeSteamApps::GetAppInstallDir(AppId_t appID, char *pchFolder, uint32 cchFolderBufferSize) { return 0; }
bool FakeSteamApps::BIsAppInstalled(AppId_t appID) { return true; }

CSteamID FakeSteamApps::GetAppOwner() {
  return CSteamID();
}

const char *FakeSteamApps::GetLaunchQueryParam(const char *pchKey) { return "FakeQueryParam"; }

bool FakeSteamApps::GetDlcDownloadProgress(AppId_t nAppID, uint64 *punBytesDownloaded, uint64 *punBytesTotal) {
  return true;
}

int FakeSteamApps::GetAppBuildId() { return 0; }

void FakeSteamApps::RequestAllProofOfPurchaseKeys() {}

SteamAPICall_t FakeSteamApps::GetFileDetails(const char *pszFileName) { return 0; }

int FakeSteamApps::GetLaunchCommandLine(char *pszCommandLine, int cubCommandLine) { return 0; }

bool FakeSteamApps::BIsSubscribedFromFamilySharing() { return true; }

bool FakeSteamApps::BIsTimedTrial(uint32 *punSecondsAllowed, uint32 *punSecondsPlayed) { return true; }

bool FakeSteamApps::SetDlcContext(AppId_t nAppID) { return true; }
