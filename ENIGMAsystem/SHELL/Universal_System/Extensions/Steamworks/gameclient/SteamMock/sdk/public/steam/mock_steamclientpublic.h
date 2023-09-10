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

#ifndef MOCK_STEAMCLIENTPUBLIC_H
#define MOCK_STEAMCLIENTPUBLIC_H

#include "mock_steamtypes.h"

class CSteamID {
 public:
  CSteamID() = default;
  ~CSteamID() = default;

  CSteamID(uint64 stean_id) { SetFromUint64(stean_id); }

  void SetFromUint64(uint64 stean_id) { CSteamID::steam_id_mock.steam_id = stean_id; }

  uint64 ConvertToUint64() const { return CSteamID::steam_id_mock.steam_id; }

  AccountID_t GetAccountID() const { return CSteamID::steam_id_mock.component_mock.account_id; }

 private:
  union SteamIDMock {
    struct SteamIDComponentMock {
      uint32 account_id : 32;
    } component_mock;

    uint64 steam_id;
  } steam_id_mock;
};

class CGameID {
 public:
  explicit CGameID(uint64 game_id) { CGameID::game_id = game_id; }

  explicit CGameID(uint32 app_id) {
    CGameID::game_id = 0;
    CGameID::game_id_mock.app_id = app_id;
  }

  ~CGameID() = default;

  uint64 ToUint64() const { return CGameID::game_id; }

 private:
  struct GameIDMock {
    unsigned int app_id : 24;
    unsigned int m_nType : 8;
    unsigned int m_nModID : 32;
  };

  union {
    uint64 game_id;
    GameIDMock game_id_mock;
  };
};

#endif  // MOCK_STEAMCLIENTPUBLIC_H
