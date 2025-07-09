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

#ifndef FAKE_STEAMUTILS_H
#define FAKE_STEAMUTILS_H

#include "fake_steam_api.h"

class FakeSteamUtils : public ISteamUtils {
 public:
  static FakeSteamUtils *GetInstance() {
    static FakeSteamUtils instance;
    return &instance;
  }

  uint32 GetSecondsSinceAppActive() override;
  uint32 GetSecondsSinceComputerActive() override;

  EUniverse GetConnectedUniverse() override;

  uint32 GetServerRealTime() override;

  const char *GetIPCountry() override;

  bool GetImageSize(int iImage, uint32 *pnWidth, uint32 *pnHeight) override;

  bool GetImageRGBA(int iImage, uint8 *pubDest, int nDestBufferSize) override;

  STEAM_PRIVATE_API(bool GetCSERIPPort(uint32 *unIP, uint16 *usPort) override;)

  uint8 GetCurrentBatteryPower() override;

  uint32 GetAppID() override;

  void SetOverlayNotificationPosition(ENotificationPosition eNotificationPosition) override;

  bool IsAPICallCompleted(SteamAPICall_t hSteamAPICall, bool *pbFailed) override;
  ESteamAPICallFailure GetAPICallFailureReason(SteamAPICall_t hSteamAPICall) override;
  bool GetAPICallResult(SteamAPICall_t hSteamAPICall, void *pCallback, int cubCallback, int iCallbackExpected,
                        bool *pbFailed) override;

  STEAM_PRIVATE_API(void RunFrame() override;)

  uint32 GetIPCCallCount() override;

  void SetWarningMessageHook(SteamAPIWarningMessageHook_t pFunction) override;

  bool IsOverlayEnabled() override;

  bool BOverlayNeedsPresent() override;

  SteamAPICall_t CheckFileSignature(const char *szFileName) override;

  bool ShowGamepadTextInput(EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode,
                            const char *pchDescription, uint32 unCharMax, const char *pchExistingText) override;

  uint32 GetEnteredGamepadTextLength() override;
  bool GetEnteredGamepadTextInput(char *pchText, uint32 cchText) override;

  const char *GetSteamUILanguage() override;

  bool IsSteamRunningInVR() override;

  void SetOverlayNotificationInset(int nHorizontalInset, int nVerticalInset) override;

  bool IsSteamInBigPictureMode() override;

  void StartVRDashboard() override;

  bool IsVRHeadsetStreamingEnabled() override;

  void SetVRHeadsetStreamingEnabled(bool bEnabled) override;

  bool IsSteamChinaLauncher() override;

  bool InitFilterText(uint32 unFilterOptions = 0) override;

  int FilterText(ETextFilteringContext eContext, CSteamID sourceSteamID, const char *pchInputMessage,
                 char *pchOutFilteredText, uint32 nByteSizeOutFilteredText) override;

  ESteamIPv6ConnectivityState GetIPv6ConnectivityState(ESteamIPv6ConnectivityProtocol eProtocol) override;

  bool IsSteamRunningOnSteamDeck() override;

  bool ShowFloatingGamepadTextInput(EFloatingGamepadTextInputMode eKeyboardMode, int nTextFieldXPosition,
                                    int nTextFieldYPosition, int nTextFieldWidth, int nTextFieldHeight) override;

  void SetGameLauncherMode(bool bLauncherMode) override;

  bool DismissFloatingGamepadTextInput() override;

 private:
  FakeSteamUtils() {}
  ~FakeSteamUtils() {}

  FakeSteamUtils(FakeSteamUtils const &) = delete;
  void operator=(FakeSteamUtils const &) = delete;
};

#endif  // FAKE_STEAMUTILS_H
