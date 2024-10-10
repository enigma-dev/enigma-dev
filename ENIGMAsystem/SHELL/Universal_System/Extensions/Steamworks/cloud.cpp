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

#include "cloud.h"

bool cloud_pre_checks(const std::string& script_name) {
  if (!steamworks_gc::GCMain::is_initialised()) {
    DEBUG_MESSAGE("Calling " + script_name + " failed. Make sure that the API is initialized correctly.", M_ERROR);
    return false;
  }

  if (!steamworks_gc::GameClient::is_user_logged_on()) {
    DEBUG_MESSAGE("Calling " + script_name + " failed. Make sure that the user is logged in.", M_ERROR);
    return false;
  }

  return true;
}

namespace enigma_user {

bool steam_is_cloud_enabled_for_app() { return false; }

bool steam_is_cloud_enabled_for_account() { return false; }

int steam_get_quota_total() { return -1; }

int steam_get_quota_free() { return -1; }

bool steam_file_exists(const std::string& filename) { return false; }

int steam_file_size(const std::string& filename) { return -1; }

bool steam_file_persisted(const std::string& filename) { return false; }

int steam_file_write(const std::string& filename, const std::string& data, int size) { return -1; }

int steam_file_write_file(std::string& steam_filename, const std::string& local_filename) { return -1; }

std::string steam_file_read(const std::string& filename) { return ""; }

int steam_file_share(const std::string& filename) { return -1; }

int steam_file_delete(const std::string& filename) { return -1; }

}  // namespace enigma_user
