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

#include "fake_steamutils.h"

uint32 FakeSteamUtils::GetSecondsSinceAppActive() { return 0; }
uint32 FakeSteamUtils::GetSecondsSinceComputerActive() { return 0; }

EUniverse FakeSteamUtils::GetConnectedUniverse() { return k_EUniverseInvalid; }

uint32 FakeSteamUtils::GetServerRealTime() { return 0; }

const char *FakeSteamUtils::GetIPCountry() { return "FakeCountry"; }

bool FakeSteamUtils::GetImageSize(int iImage, uint32 *pnWidth, uint32 *pnHeight) {
  *pnWidth = 184;
  *pnHeight = 184;
  return true;
}

bool FakeSteamUtils::GetImageRGBA(int iImage, uint8 *pubDest, int nDestBufferSize) {
  for (unsigned i{0}; i < (unsigned)nDestBufferSize; i++) pubDest[i] = 0;
  return true;
}

bool FakeSteamUtils::GetCSERIPPort(uint32 *unIP, uint16 *usPort) { return true; }

uint8 FakeSteamUtils::GetCurrentBatteryPower() { return 0; }

uint32 FakeSteamUtils::GetAppID() {
  return 480;  // Spacewar's AppID
}

void FakeSteamUtils::SetOverlayNotificationPosition(ENotificationPosition eNotificationPosition) {}

bool FakeSteamUtils::IsAPICallCompleted(SteamAPICall_t hSteamAPICall, bool *pbFailed) { return true; }
ESteamAPICallFailure FakeSteamUtils::GetAPICallFailureReason(SteamAPICall_t hSteamAPICall) {
  return k_ESteamAPICallFailureNone;
}
bool FakeSteamUtils::GetAPICallResult(SteamAPICall_t hSteamAPICall, void *pCallback, int cubCallback,
                                      int iCallbackExpected, bool *pbFailed) {
  return true;
}

void FakeSteamUtils::RunFrame() {}

uint32 FakeSteamUtils::GetIPCCallCount() { return 0; }

void FakeSteamUtils::SetWarningMessageHook(SteamAPIWarningMessageHook_t pFunction) {}

bool FakeSteamUtils::IsOverlayEnabled() { return true; }

bool FakeSteamUtils::BOverlayNeedsPresent() { return true; }

SteamAPICall_t FakeSteamUtils::CheckFileSignature(const char *szFileName) { return 0; }

bool FakeSteamUtils::ShowGamepadTextInput(EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode,
                                          const char *pchDescription, uint32 unCharMax, const char *pchExistingText) {
  return true;
}

uint32 FakeSteamUtils::GetEnteredGamepadTextLength() { return 0; }
bool FakeSteamUtils::GetEnteredGamepadTextInput(char *pchText, uint32 cchText) { return true; }

const char *FakeSteamUtils::GetSteamUILanguage() { return "FakeLanguage"; }

bool FakeSteamUtils::IsSteamRunningInVR() { return true; }

void FakeSteamUtils::SetOverlayNotificationInset(int nHorizontalInset, int nVerticalInset) {}

bool FakeSteamUtils::IsSteamInBigPictureMode() { return true; }

void FakeSteamUtils::StartVRDashboard() {}

bool FakeSteamUtils::IsVRHeadsetStreamingEnabled() { return true; }

void FakeSteamUtils::SetVRHeadsetStreamingEnabled(bool bEnabled) {}

bool FakeSteamUtils::IsSteamChinaLauncher() { return true; }

bool FakeSteamUtils::InitFilterText(uint32 unFilterOptions) { return true; }

int FakeSteamUtils::FilterText(ETextFilteringContext eContext, CSteamID sourceSteamID, const char *pchInputMessage,
                               char *pchOutFilteredText, uint32 nByteSizeOutFilteredText) {
  return 0;
}

ESteamIPv6ConnectivityState FakeSteamUtils::GetIPv6ConnectivityState(ESteamIPv6ConnectivityProtocol eProtocol) {
  return k_ESteamIPv6ConnectivityState_Unknown;
}

bool FakeSteamUtils::IsSteamRunningOnSteamDeck() { return true; }

bool FakeSteamUtils::ShowFloatingGamepadTextInput(EFloatingGamepadTextInputMode eKeyboardMode, int nTextFieldXPosition,
                                                  int nTextFieldYPosition, int nTextFieldWidth, int nTextFieldHeight) {
  return true;
}

void FakeSteamUtils::SetGameLauncherMode(bool bLauncherMode) {}

bool FakeSteamUtils::DismissFloatingGamepadTextInput() { return true; }
