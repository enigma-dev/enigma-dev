#ifndef ASSET_ENUM_H
#define ASSET_ENUM_H

#include <map>

namespace enigma_user {

enum AssetType : int {
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
  
struct AssetInfo {
  enigma_user::AssetType type;
  int ID;
};

extern std::map<std::string, AssetInfo> assetMap;

}

#endif
