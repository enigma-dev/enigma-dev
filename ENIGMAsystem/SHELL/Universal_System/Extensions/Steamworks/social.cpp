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

#include "social.h"

#include "Graphics_Systems/General/GSsurface.h"
#include "Universal_System/buffers.h"
#include "Universal_System/var4.h"

bool social_pre_checks(const std::string& script_name) {
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

namespace enigma {
unsigned char RGBAtoARGB(unsigned char rgba) {
  unsigned char argb {0b00000000};
  return argb;
}

unsigned char RGBAtoBGRA(unsigned char rgba) {
  unsigned char bgra {0b00000000};
  return bgra;
}
}  // namespace enigma

namespace enigma_user {

/**
 * @note Continue after leaderboards constants to prevent overlapping.
 * 
 */
const unsigned steam_user_avatar_size_small{29};
const unsigned steam_user_avatar_size_medium{30};
const unsigned steam_user_avatar_size_large{31};

bool steam_set_rich_presence(const std::string& key, const std::string& value) {
  if (!social_pre_checks("steam_set_rich_presence")) return false;

  if (!steamworks_gc::GameClient::set_rich_presence(key, value)) {
    DEBUG_MESSAGE(
        "Calling steam_set_rich_presence failed. This happens if key was longer than 256 or had a length of 0, "
        "value was longer than 256, and The user has reached the maximum amount of rich presence keys which is 20.",
        M_ERROR);
    return false;
  }

  return true;
}

void steam_clear_rich_presence() {
  if (!social_pre_checks("steam_clear_rich_presence")) return;

  steamworks_gc::GameClient::clear_rich_presence();
}

bool steam_user_set_played_with(const unsigned long long steam_id) {
  if (!social_pre_checks("steam_user_set_played_with")) return false;

  steamworks_gc::GameClient::set_played_with(steam_id);

  return true;
}

void steam_get_friends_game_info() {}

int steam_get_user_avatar(const unsigned long long userID, const unsigned avatar_size) {
  if (!social_pre_checks("steam_get_user_avatar")) return -1;

  int avatar_id{-1};

  switch (avatar_size) {
    case enigma_user::steam_user_avatar_size_small:
      avatar_id = steamworks_gc::GameClient::get_small_friend_avatar(userID);
      break;
    case enigma_user::steam_user_avatar_size_medium:
      avatar_id = steamworks_gc::GameClient::get_medium_friend_avatar(userID);
      break;
    case enigma_user::steam_user_avatar_size_large:
      avatar_id = steamworks_gc::GameClient::get_large_friend_avatar(userID);
      break;
    default:
      DEBUG_MESSAGE(
          "Calling steam_get_user_avatar failed. Invalid avatar size. The allowed options are: "
          "steam_user_avatar_size_small, steam_user_avatar_size_medium, and steam_user_avatar_size_large.",
          M_ERROR);
      break;
  }

  return avatar_id;
}

// TODO: Check how ENIGMA compiler handles null values.
var steam_image_get_size(const int steam_image_id) {
  var image_size;

  image_size(2) = -1;

  if (!social_pre_checks("steam_image_get_size")) return image_size;

  unsigned width{0}, height{0};

  if (!steamworks_gc::GameClient::get_image_size(steam_image_id, width, height)) {
    DEBUG_MESSAGE(
        "Calling steam_image_get_size failed. This happens if the image handle is not "
        "valid or the sizes weren't filled out",
        M_ERROR);
    return image_size;
  }

  image_size(0) = width;
  image_size(1) = height;

  return image_size;
}

bool steam_image_get_rgba(const int steam_image_id, const int buffer, const int size) {
  if (!social_pre_checks("steam_image_get_rgba")) return false;

  unsigned char* flattened_image{new unsigned char[size]};

  if (!steamworks_gc::GameClient::get_image_rgba(steam_image_id, flattened_image, size)) {
    DEBUG_MESSAGE("Calling steam_image_get_rgba failed.", M_ERROR);
    return false;
  }

  for (unsigned i{0}; i < (unsigned)size; i++) {
    enigma_user::buffer_write(buffer, enigma_user::buffer_u8, flattened_image[i]);
  }

  delete[] flattened_image;

  flattened_image = nullptr;

  return true;
}

bool steam_image_get_bgra(const int steam_image_id, const int buffer, const int size) {
  if (!social_pre_checks("steam_image_get_bgra")) return false;

  unsigned char* flattened_image{new unsigned char[size]};

  if (!steamworks_gc::GameClient::get_image_rgba(steam_image_id, flattened_image, size)) {
    DEBUG_MESSAGE("Calling steam_image_get_bgra failed.", M_ERROR);
    return false;
  }

  // for (unsigned i{0}; i < (unsigned)size; i++) {
  //   flattened_image[i] = enigma::RGBAtoBGRA(flattened_image[i]);
  // }

  for (unsigned i{0}; i < (unsigned)size; i++) {
    enigma_user::buffer_write(buffer, enigma_user::buffer_u8, flattened_image[i]);
  }

  delete[] flattened_image;

  flattened_image = nullptr;

  return true;
}

bool steam_image_get_argb(const int steam_image_id, const int buffer, const int size) {
  if (!social_pre_checks("steam_image_get_argb")) return false;

  unsigned char* flattened_image{new unsigned char[size]};

  if (!steamworks_gc::GameClient::get_image_rgba(steam_image_id, flattened_image, size)) {
    DEBUG_MESSAGE("Calling steam_image_get_argb failed.", M_ERROR);
    return false;
  }

  // for (unsigned i{0}; i < (unsigned)size; i++) {
  //   flattened_image[i] = enigma::RGBAtoARGB(flattened_image[i]);
  // }

  for (unsigned i{0}; i < (unsigned)size; i++) {
    enigma_user::buffer_write(buffer, enigma_user::buffer_u8, flattened_image[i]);
  }

  delete[] flattened_image;

  flattened_image = nullptr;

  return true;
}

int steam_image_create_sprite(const int steam_image_id) {
  if (!social_pre_checks("steam_image_create_sprite")) return -1;

  if (steam_image_id < 0) {
    return -1;
  }

  var image_size{enigma_user::steam_image_get_size(steam_image_id)};

  if (image_size.type == enigma_user::ty_undefined) return -1;

  unsigned buff_size{(unsigned)image_size[0] * (unsigned)image_size[1] * 4};

  if (buff_size < 0) return -1;

  // This should be enigma_user::buffer_t, but this type is only inside AST-Generation branch.
  std::int64_t buffer{enigma_user::buffer_create(buff_size, buffer_fixed, 1)};

  bool success{enigma_user::steam_image_get_argb(steam_image_id, buffer, buff_size)};

  if (!success) return -1;

  int surface{enigma_user::surface_create(image_size[0], image_size[1])};

  enigma_user::buffer_set_surface(buffer, surface, 0);

  int sprite_id{
      enigma_user::sprite_create_from_surface(surface, 0, 0, image_size[0], image_size[1], false, false, 0, 0)};

  enigma_user::surface_free(surface);

  enigma_user::buffer_delete(buffer);

  return sprite_id;
}

}  // namespace enigma_user
