// /** Copyright (C) 2023-2024 Saif Kandil
// ***
// *** This file is a part of the ENIGMA Development Environment.
// ***
// *** ENIGMA is free software: you can redistribute it and/or modify it under the
// *** terms of the GNU General Public License as published by the Free Software
// *** Foundation, version 3 of the license or any later version.
// ***
// *** This application and its source code is distributed AS-IS, WITHOUT ANY
// *** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// *** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// *** details.
// ***
// *** You should have received a copy of the GNU General Public License along
// *** with this code. If not, see <http://www.gnu.org/licenses/>
// **/

// #include "include.h"

// namespace enigma {
//     CSteamID steam_get_user_steam_id() {
//         cout << "id getting" << endl;
//         return SteamUser()->GetSteamID();
//     }
// }

// namespace enigma_user {

//     // int steam_get_sprite_from_avatar() {
//     //     if (!SteamUserStats()->RequestCurrentStats()) {
//     //         cout << "false stats" << endl;
//     //         return;
//     //     }

//     //     int iImage = SteamUserStats()->GetAchievementIcon("ACH_WIN_ONE_GAME");

//     //     uint32 uAvatarWidth, uAvatarHeight;
//     //     bool success = SteamUtils()->GetImageSize( iImage, &uAvatarWidth, &uAvatarHeight );
//     //     if ( !success ) {
//     //         DEBUG_MESSAGE("GetImageSize() failed.\n", M_ERROR);
//     //         return -1;
//     //     }

//     //     const int uImageSizeInBytes = uAvatarWidth * uAvatarHeight * 4;
//     //     uint8 *pAvatarRGBA = new uint8[uImageSizeInBytes];
//     //     success = SteamUtils()->GetImageRGBA( iImage, pAvatarRGBA, uImageSizeInBytes );
//     //     if( !success ) {
//     //         DEBUG_MESSAGE("GetImageRGBA() failed.\n", M_ERROR);
//     //         return -1;
//     //     }

//     //     int buffer = enigma_user::buffer_create(uImageSizeInBytes,buffer_fixed,buffer_u8);

//     //     int surface = enigma_user::surface_create(uAvatarWidth, uAvatarHeight);

//     //     enigma_user::buffer_set_surface(buffer, surface, buffer_surface_copy); // 0

//     //     int sprite = enigma_user::sprite_create_from_surface(surface, 0, 0, uAvatarWidth, uAvatarHeight, false, false, 0, 0);

//     //     delete[] pAvatarRGBA;

//     //     return sprite;
//     // }
// }



