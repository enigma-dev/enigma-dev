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

#include "include.h"

namespace enigma_user {

void steam_is_cloud_enabled_for_app() {
    bool isEnabled = SteamRemoteStorage()->IsCloudEnabledForApp();
    if (isEnabled) {
        cout << "Enabled for account.\n" << endl;
    }
    else {
        cout << "Disbaled for account.\n" << endl;
    }
}

void steam_is_cloud_enabled_for_account() {
    bool isEnabled = SteamRemoteStorage()->IsCloudEnabledForAccount();
    if (isEnabled) {
        cout << "Enabled for account.\n" << endl;
    }
    else {
        cout << "Disbaled for account.\n" << endl;
    }
}

}
