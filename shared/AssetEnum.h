#ifndef ASSET_ENUM_H
#define ASSET_ENUM_H

#include <map>

namespace enigma_user {

enum AssetType : int {
  asset_any        = -2,
  asset_unknown    = -1,
  asset_object     =  0,
  asset_sprite     =  1,
  asset_sound      =  2,
  asset_room       =  3,
  asset_tiles      =  4,
  asset_background =  4,
  asset_path       =  5,
  asset_script     =  6,
  asset_font       =  7,
  asset_timeline   =  8,
  asset_shader     =  10
};

}

namespace enigma {

static const enigma_user::AssetType assetTypes[] = { 
  enigma_user::asset_object, 
  enigma_user::asset_sprite, 
  enigma_user::asset_sound, 
  enigma_user::asset_room, 
  enigma_user::asset_tiles, 
  enigma_user::asset_background, 
  enigma_user::asset_path, 
  enigma_user::asset_script, 
  enigma_user::asset_font, 
  enigma_user::asset_timeline, 
  enigma_user::asset_shader 
};

}

#endif
