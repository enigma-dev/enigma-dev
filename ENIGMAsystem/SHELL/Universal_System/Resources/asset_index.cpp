#include "asset_index.h"

using enigma::assetMap;

namespace enigma_user {

int asset_get_index(std::string resource) {
 auto it = assetMap.find(resource);
 if (it != assetMap.end()) return it->second.ID;
 else return -1;
}

int asset_get_type(std::string resource) {
  auto it = assetMap.find(resource);
  if (it != assetMap.end()) return it->second.type;
  else return asset_unknown;
}

}
