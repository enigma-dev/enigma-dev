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

#ifndef FAKE_STEAMUSER_H
#define FAKE_STEAMUSER_H

#include "fake_steam_api.h"

class FakeSteamUser : public ISteamUser {
 public:
  static FakeSteamUser *GetInstance() {
    static FakeSteamUser instance;
    return &instance;
  }

  HSteamUser GetHSteamUser() override;

  bool BLoggedOn() override;

  CSteamID GetSteamID() override;

  int InitiateGameConnection_DEPRECATED(void *pAuthBlob, int cbMaxAuthBlob, CSteamID steamIDGameServer,
                                        uint32 unIPServer, uint16 usPortServer, bool bSecure) override;

  void TerminateGameConnection_DEPRECATED(uint32 unIPServer, uint16 usPortServer) override;

  void TrackAppUsageEvent(CGameID gameID, int eAppUsageEvent, const char *pchExtraInfo = "") override;

  bool GetUserDataFolder(char *pchBuffer, int cubBuffer) override;

  void StartVoiceRecording() override;

  void StopVoiceRecording() override;

  EVoiceResult GetAvailableVoice(uint32 *pcbCompressed, uint32 *pcbUncompressed_Deprecated = 0,
                                 uint32 nUncompressedVoiceDesiredSampleRate_Deprecated = 0) override;

  EVoiceResult GetVoice(bool bWantCompressed, void *pDestBuffer, uint32 cbDestBufferSize, uint32 *nBytesWritten,
                        bool bWantUncompressed_Deprecated = false, void *pUncompressedDestBuffer_Deprecated = 0,
                        uint32 cbUncompressedDestBufferSize_Deprecated = 0,
                        uint32 *nUncompressBytesWritten_Deprecated = 0,
                        uint32 nUncompressedVoiceDesiredSampleRate_Deprecated = 0) override;

  EVoiceResult DecompressVoice(const void *pCompressed, uint32 cbCompressed, void *pDestBuffer, uint32 cbDestBufferSize,
                               uint32 *nBytesWritten, uint32 nDesiredSampleRate) override;

  uint32 GetVoiceOptimalSampleRate() override;

  HAuthTicket GetAuthSessionTicket(void *pTicket, int cbMaxTicket, uint32 *pcbTicket,
                                   const SteamNetworkingIdentity *pSteamNetworkingIdentity) override;

  HAuthTicket GetAuthTicketForWebApi(const char *pchIdentity) override;

  EBeginAuthSessionResult BeginAuthSession(const void *pAuthTicket, int cbAuthTicket, CSteamID steamID) override;

  void EndAuthSession(CSteamID steamID) override;

  void CancelAuthTicket(HAuthTicket hAuthTicket) override;

  EUserHasLicenseForAppResult UserHasLicenseForApp(CSteamID steamID, AppId_t appID) override;

  bool BIsBehindNAT() override;

  void AdvertiseGame(CSteamID steamIDGameServer, uint32 unIPServer, uint16 usPortServer) override;

  SteamAPICall_t RequestEncryptedAppTicket(void *pDataToInclude, int cbDataToInclude) override;

  bool GetEncryptedAppTicket(void *pTicket, int cbMaxTicket, uint32 *pcbTicket) override;

  int GetGameBadgeLevel(int nSeries, bool bFoil) override;

  int GetPlayerSteamLevel() override;

  SteamAPICall_t RequestStoreAuthURL(const char *pchRedirectURL) override;

  bool BIsPhoneVerified() override;

  bool BIsTwoFactorEnabled() override;

  bool BIsPhoneIdentifying() override;

  bool BIsPhoneRequiringVerification() override;

  SteamAPICall_t GetMarketEligibility() override;

  SteamAPICall_t GetDurationControl() override;

  bool BSetDurationControlOnlineState(EDurationControlOnlineState eNewState) override;

 private:
  FakeSteamUser() {}
  ~FakeSteamUser() {}

  FakeSteamUser(FakeSteamUser const &) = delete;
  void operator=(FakeSteamUser const &) = delete;
};

#endif  // FAKE_STEAMUSER_H
