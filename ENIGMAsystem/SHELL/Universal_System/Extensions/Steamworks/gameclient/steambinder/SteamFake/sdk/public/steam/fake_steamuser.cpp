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

#include "fake_steamuser.h"

HSteamUser FakeSteamUser::GetHSteamUser() { return 0; }

bool FakeSteamUser::BLoggedOn() { return true; }

CSteamID FakeSteamUser::GetSteamID() {
  return CSteamID();
}

int FakeSteamUser::InitiateGameConnection_DEPRECATED(void *pAuthBlob, int cbMaxAuthBlob, CSteamID steamIDGameServer,
                                                     uint32 unIPServer, uint16 usPortServer, bool bSecure) {
  return 0;
}

void FakeSteamUser::TerminateGameConnection_DEPRECATED(uint32 unIPServer, uint16 usPortServer) {}

void FakeSteamUser::TrackAppUsageEvent(CGameID gameID, int eAppUsageEvent, const char *pchExtraInfo) {}

bool FakeSteamUser::GetUserDataFolder(char *pchBuffer, int cubBuffer) { return true; }

void FakeSteamUser::StartVoiceRecording() {}

void FakeSteamUser::StopVoiceRecording() {}

EVoiceResult FakeSteamUser::GetAvailableVoice(uint32 *pcbCompressed, uint32 *pcbUncompressed_Deprecated,
                                              uint32 nUncompressedVoiceDesiredSampleRate_Deprecated) {
  return k_EVoiceResultOK;
}

EVoiceResult FakeSteamUser::GetVoice(bool bWantCompressed, void *pDestBuffer, uint32 cbDestBufferSize,
                                     uint32 *nBytesWritten, bool bWantUncompressed_Deprecated,
                                     void *pUncompressedDestBuffer_Deprecated,
                                     uint32 cbUncompressedDestBufferSize_Deprecated,
                                     uint32 *nUncompressBytesWritten_Deprecated,
                                     uint32 nUncompressedVoiceDesiredSampleRate_Deprecated) {
  return k_EVoiceResultOK;
}

EVoiceResult FakeSteamUser::DecompressVoice(const void *pCompressed, uint32 cbCompressed, void *pDestBuffer,
                                            uint32 cbDestBufferSize, uint32 *nBytesWritten, uint32 nDesiredSampleRate) {
  return k_EVoiceResultOK;
}

uint32 FakeSteamUser::GetVoiceOptimalSampleRate() { return 0; }

HAuthTicket FakeSteamUser::GetAuthSessionTicket(void *pTicket, int cbMaxTicket, uint32 *pcbTicket,
                                                const SteamNetworkingIdentity *pSteamNetworkingIdentity) {
  return 0;
}

HAuthTicket FakeSteamUser::GetAuthTicketForWebApi(const char *pchIdentity) { return 0; }

EBeginAuthSessionResult FakeSteamUser::BeginAuthSession(const void *pAuthTicket, int cbAuthTicket, CSteamID steamID) {
  return k_EBeginAuthSessionResultOK;
}

void FakeSteamUser::EndAuthSession(CSteamID steamID) {}

void FakeSteamUser::CancelAuthTicket(HAuthTicket hAuthTicket) {}

EUserHasLicenseForAppResult FakeSteamUser::UserHasLicenseForApp(CSteamID steamID, AppId_t appID) {
  return k_EUserHasLicenseResultHasLicense;
}

bool FakeSteamUser::BIsBehindNAT() { return true; }

void FakeSteamUser::AdvertiseGame(CSteamID steamIDGameServer, uint32 unIPServer, uint16 usPortServer) {}

SteamAPICall_t FakeSteamUser::RequestEncryptedAppTicket(void *pDataToInclude, int cbDataToInclude) { return 0; }

bool FakeSteamUser::GetEncryptedAppTicket(void *pTicket, int cbMaxTicket, uint32 *pcbTicket) { return true; }

int FakeSteamUser::GetGameBadgeLevel(int nSeries, bool bFoil) { return 0; }

int FakeSteamUser::GetPlayerSteamLevel() { return 0; }

SteamAPICall_t FakeSteamUser::RequestStoreAuthURL(const char *pchRedirectURL) { return 0; }

bool FakeSteamUser::BIsPhoneVerified() { return true; }

bool FakeSteamUser::BIsTwoFactorEnabled() { return true; }

bool FakeSteamUser::BIsPhoneIdentifying() { return true; }

bool FakeSteamUser::BIsPhoneRequiringVerification() { return true; }

SteamAPICall_t FakeSteamUser::GetMarketEligibility() { return 0; }

SteamAPICall_t FakeSteamUser::GetDurationControl() { return 0; }

bool FakeSteamUser::BSetDurationControlOnlineState(EDurationControlOnlineState eNewState) { return true; }
