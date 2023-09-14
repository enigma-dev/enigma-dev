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

#ifndef MOCK_STEAMUTILS_H
#define MOCK_STEAMUTILS_H

#include "mock_isteamutils.h"

class SteamUtilsAccessor : public ISteamUtils {
 public:
  static SteamUtilsAccessor &getInstance() {
    static SteamUtilsAccessor instance;
    return instance;
  }

  // SteamUtilsAccessor(SteamUtilsAccessor const &) = delete;
  // void operator=(SteamUtilsAccessor const &) = delete;

  bool GetImageSize(int iImage, uint32 *pnWidth, uint32 *pnHeight) override;

  bool GetImageRGBA(int iImage, uint8 *pubDest, int nDestBufferSize) override;

  uint32 GetAppID() override;

  void SetOverlayNotificationPosition(ENotificationPosition eNotificationPosition) override;

  void SetWarningMessageHook() override;

  bool IsOverlayEnabled() override;

  void SetOverlayNotificationInset(int nHorizontalInset, int nVerticalInset) override;

  private:
  SteamUtilsAccessor(){}
  ~SteamUtilsAccessor() = default;

  SteamUtilsAccessor(SteamUtilsAccessor const &);
  void operator=(SteamUtilsAccessor const &);
};

inline ISteamUtils *SteamUtils() {
  return &SteamUtilsAccessor::getInstance();
}

#endif  // MOCK_STEAMUTILS_H
