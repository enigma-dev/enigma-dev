/** Copyright (C) 2023-2024 Saif Kandil
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

#ifndef MOCK_ISTEAMUTILS_H
#define MOCK_ISTEAMUTILS_H

#include "mock_steam_api_common.h"

class ISteamUtils {
 public:
  virtual bool GetImageSize(int iImage, uint32 *pnWidth, uint32 *pnHeight) = 0;

  virtual bool GetImageRGBA(int iImage, uint8 *pubDest, int nDestBufferSize) = 0;

  virtual uint32 GetAppID() = 0;

  virtual void SetOverlayNotificationPosition(ENotificationPosition eNotificationPosition) = 0;

  virtual void SetWarningMessageHook() = 0;

  virtual bool IsOverlayEnabled() = 0;

  virtual void SetOverlayNotificationInset(int nHorizontalInset, int nVerticalInset) = 0;
};

#endif  // MOCK_ISTEAMUTILS_H
