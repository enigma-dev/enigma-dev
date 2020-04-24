#include "asset_index.h"

using enigma::assetMap;
using enigma::assetTypes;

namespace enigma_user {

int asset_get_index(std::string resource, AssetType type) {
 if (type == asset_any) {
  for (const AssetType t : assetTypes) {
    const auto& m = assetMap.at(t);
    auto it = m.find(resource);
    if (it != m.end()) return it->second;
    } 
  } else {
    const auto& m = assetMap.at(type);
    auto it = m.find(resource);
    if (it != m.end()) return it->second;
  }
  
  return -1;
}

int asset_get_type(std::string resource) {
  for (const AssetType t : assetTypes) {
    const auto& m = assetMap.at(t);
    auto it = m.find(resource);
    if (it != m.end()) return t;
  }
  
  return asset_unknown;
}

}
