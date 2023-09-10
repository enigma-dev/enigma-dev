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

#ifndef MOCK_ISTEAMAPPS_H
#define MOCK_ISTEAMAPPS_H

#include "mock_steam_api_common.h"

class ISteamApps {
 public:
  virtual bool BIsSubscribed() = 0;
  virtual const char *GetCurrentGameLanguage() = 0;
  virtual const char *GetAvailableGameLanguages() = 0;
};

#endif  // MOCK_ISTEAMAPPS_H
