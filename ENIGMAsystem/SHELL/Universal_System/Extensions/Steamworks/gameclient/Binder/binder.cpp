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

#include "binder.h"

namespace steamworks_b {

Init_t Binder::Init{nullptr};
Shutdown_t Binder::Shutdown{nullptr};
RestartAppIfNecessary_t Binder::RestartAppIfNecessary{nullptr};
RunCallbacks_t Binder::RunCallbacks{nullptr};

bool Binder::bind() {
  // get env var STEAM_SDK_PATH

  // Check if the library exists.
  // TODO: Fix this path before testing.
  fs::path libpath(
      "/home/saif/Desktop/enigma-dev/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/gameclient/Binder/"
      "Steamv157/sdk/redistributable_bin/linux64/lib_steam_api.so");

  if (!fs::exists(libpath)) {
    if (!Binder::bind_fake()) {
      return false;
    }
    return true;
  }

  if (!Binder::bind_real(libpath)) {
    return false;
  }

  return true;
}

bool Binder::bind_fake() {
  Binder::Init = &SteamAPI_Init;
  Binder::Shutdown = &SteamAPI_Shutdown;
  Binder::RestartAppIfNecessary = &SteamAPI_RestartAppIfNecessary;
  Binder::RunCallbacks = &SteamAPI_RunCallbacks;
  return true;
}

bool Binder::bind_real(fs::path libpath) { return true; }

}  // namespace steamworks_b
