/** Copyright (C) 2019 Robert B. Colton
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

#ifdef INCLUDED_FROM_SHELLMAIN
#  error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

#ifndef E_ASSET_ARRAY
#define E_ASSET_ARRAY

#include <vector>
#include <string>

#ifdef DEBUG_MODE
  #include "Widget_Systems/widgets_mandatory.h" // for show_error
  #define CHECK_ID(id, ret) \
    if (!exists(id)) { \
      enigma_user::show_error("Requested asset " + std::to_string(id) + " does not exist.", false); \
      return ret; \
    }
#else
  #define CHECK_ID(id, ret)
#endif

namespace enigma {

// Asset storage container designed for dense cache-efficient resource processing.
template<typename T>
class AssetArray {
public:
  int add(T asset) {
    int id = assets.size();
    assets.emplace_back(asset);
    return id;
  }

  T get(int id) {
    static T sentinel;
    CHECK_ID(id,sentinel);
    return assets[id];
  }

  int replace(int id, T asset) {
    CHECK_ID(id, -1);
    assets[id] = asset;
    return id;
  }

  int duplicate(int id) {
    CHECK_ID(id, -1);
    T asset = assets[id];
    return add(asset);
  }

  size_t size() const { return assets.size(); }
  bool exists(int id) { return (id > 0 && id < size()); }

private:
  std::vector<T> assets;
};

} // namespace enigma

#endif // E_ASSET_ARRAY
