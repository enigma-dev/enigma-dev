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

#include "management.h"

#include "gameclient/gc_main.h"
#include "steamworks.h"

bool managements_pre_checks(const std::string& script_name) {
  if (!steamworks_gc::GCMain::is_initialised()) {
    DEBUG_MESSAGE("Calling " + script_name + " failed. Make sure that the API is initialized correctly.", M_ERROR);
    return false;
  }

  return true;
}

namespace enigma_user {

void steam_init() { enigma::extension_steamworks_init(); }

void steam_update() {
  if (!managements_pre_checks("steam_update")) return;

  steamworks_gc::GCMain::run_callbacks();
}

void steam_shutdown() {
  if (!managements_pre_checks("steam_shutdown")) return;

  steamworks_gc::GCMain::shutdown();
}

}  // namespace enigma_user
