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

#ifndef MOCK_STEAMAPPS_H
#define MOCK_STEAMAPPS_H

#include "mock_isteamapps.h"

class SteamAppsAccessor : public ISteamApps {
 public:
  static SteamAppsAccessor &getInstance() {
    static SteamAppsAccessor instance;
    return instance;
  }

  // SteamAppsAccessor(SteamAppsAccessor const &) = delete;
  // void operator=(SteamAppsAccessor const &) = delete;

  bool BIsSubscribed() override;
  const char *GetCurrentGameLanguage() override;
  const char *GetAvailableGameLanguages() override;

 private:
  SteamAppsAccessor() {}
  ~SteamAppsAccessor() = default;

  SteamAppsAccessor(SteamAppsAccessor const &);
  void operator=(SteamAppsAccessor const &);
};

inline ISteamApps *SteamApps() { return &SteamAppsAccessor::getInstance(); }

#endif  // MOCK_STEAMAPPS_H
