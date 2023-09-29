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

#ifndef GAMECLIENT_REMOTE_STORAGE_H
#define GAMECLIENT_REMOTE_STORAGE_H
#pragma once

#include "gameclient.h"

namespace steamworks_gc {

class GameClient;

class GCRemoteStorage {
 public:
  // GCRemoteStorage constructor.
  GCRemoteStorage() = default;

  // GCRemoteStorage destructor.
  ~GCRemoteStorage() = default;
};
}  // namespace steamworks_gc

#endif  // GAMECLIENT_REMOTE_STORAGE_H
