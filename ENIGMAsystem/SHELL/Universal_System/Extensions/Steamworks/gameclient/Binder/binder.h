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

#ifndef BINDER_H
#define BINDER_H

/*
    This is the lowest layer that game_client layer mainly depends on.
*/
#include "steam/steam_api.h"

#include <dlfcn.h>
#include <filesystem>
// #include <cstdlib>

namespace fs = std::filesystem;

typedef bool (*Init_t)();
typedef void (*Shutdown_t)();
typedef bool (*RestartAppIfNecessary_t)(uint32);
typedef void (*RunCallbacks_t)();

namespace steamworks_b {

class Binder {
 public:
  static Init_t Init;
  static Shutdown_t Shutdown;
  static RestartAppIfNecessary_t RestartAppIfNecessary;
  static RunCallbacks_t RunCallbacks;

  static bool bind();

 private:
  static bool bind_fake();
  static bool bind_real(fs::path libpath);
};

}  // namespace steamworks_b

#endif  // BINDER_H
