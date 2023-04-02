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

namespace enigma {

void steam_request_current_stats() {
    bool success = SteamUserStats()->RequestCurrentStats();
    if (success) {
        cout << "yaaay!.\n" << endl;
    }
    else {
        cout << "Fatal error.\n" << endl;
    }
}

void steam_store_stats() {
    bool success = SteamUserStats()->StoreStats();
    if (success) {
        cout << "yaaay!.\n" << endl;
    }
    else {
        cout << "Fatal error.\n" << endl;
    }
}

}

namespace enigma_user {
    void steam_set_achievement() {
        if (!SteamUserStats()->RequestCurrentStats()) {
            cout << "false stats" << endl;
            return;
        }

        if (SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME")) {
            cout << "success setting" << endl;
        }
        else {
            cout << "failed setting" << endl;
        }

        if (SteamUserStats()->StoreStats()) {
            cout << "success storing" << endl;
        }
        else {
            cout << "failed storing" << endl;
        }
    }

    int steam_get_sprite_from_achievement_icon() {
        if (!SteamUserStats()->RequestCurrentStats()) {
            cout << "false stats" << endl;
            return -1;
        }

        int iImage = SteamUserStats()->GetAchievementIcon("ACH_WIN_ONE_GAME");

        uint32 uAvatarWidth, uAvatarHeight;
        bool success = SteamUtils()->GetImageSize( iImage, &uAvatarWidth, &uAvatarHeight );
        if ( !success ) {
            DEBUG_MESSAGE("GetImageSize() failed.\n", M_ERROR);
            return -1;
        }

        const int uImageSizeInBytes = uAvatarWidth * uAvatarHeight * 4;
        uint8 *pAvatarRGBA = new uint8[uImageSizeInBytes];
        success = SteamUtils()->GetImageRGBA( iImage, pAvatarRGBA, uImageSizeInBytes );
        if( !success ) {
            DEBUG_MESSAGE("GetImageRGBA() failed.\n", M_ERROR);
            return -1;
        }

        // buffer_set_surface();

        delete[] pAvatarRGBA;

        return 0;

    }

    

    // bool steam_image_get_rgba(int iImage, uint8 *pAvatarRGBA, int uImageSizeInBytes) {
    //     int hTexture = 0;
    //     bool success = SteamUtils()->GetImageRGBA(iImage, pAvatarRGBA, uImageSizeInBytes);
        
    //     delete[] pAvatarRGBA;

    //     return success;
    // }
}
