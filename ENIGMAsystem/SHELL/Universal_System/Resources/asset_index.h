#ifndef ASSET_INDEX_H
#define ASSET_INDEX_H

#include "AssetEnum.h"

#include <string>

namespace enigma_user {

int asset_get_index(std::string resource);
int asset_get_type(std::string resource);

}

#endif
