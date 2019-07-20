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
#include <utility>

#ifdef DEBUG_MODE
  #include "Widget_Systems/widgets_mandatory.h" // for DEBUG_MESSAGE
  #define CHECK_ID(id, ret) \
    if (!exists(id)) { \
      DEBUG_MESSAGE("Requested " + std::string(T::getAssetTypeName()) + " asset " + std::to_string(id) + " does not exist.", MESSAGE_TYPE::M_USER_ERROR); \
      return ret; \
    }
  #define CHECK_ID_V(id) CHECK_ID(id,)
#else
  #define CHECK_ID(id, ret)
  #define CHECK_ID_V(id)
#endif

namespace enigma {

// Asset storage container designed for dense cache-efficient resource processing.
template<typename T>
class AssetArray {
 public:
  // Custom iterator for looping over only the existing assets in the array.
  class iterator {
   public:
    iterator(AssetArray& assets, int ind): assets(assets), ind(ind) {}
    iterator operator++() {
      while (!assets.exists(++ind) && size_t(ind) < assets.size());
      return *this;
    }
    bool operator!=(const iterator& other) const { return ind != other.ind; }
    std::pair<int, T&> operator*() const { return {ind, assets[ind]}; }
   private:
    AssetArray& assets;
    int ind;
  };

  AssetArray() {}

  iterator begin() {
    iterator it(*this, 0);
    return (exists(0) ? it : ++it);
  }
  iterator end() { return iterator(*this, size()); }

  int add(T&& asset) {
    size_t id = size();
    assets_.emplace_back(std::move(asset));
    return (int)id;
  }

  int assign(int id, T&& asset) {
    if (exists(id)) assets_[id].destroy();
    else {
      #ifdef DEBUG_MODE
      if (id < 0) {
        DEBUG_MESSAGE("Attempting to assign " + std::string(T::getAssetTypeName()) + " asset " + std::to_string(id) + " to negative index.", MESSAGE_TYPE::M_USER_ERROR);
        return id;
      }
      #endif
      if (size_t(id) >= size()) assets_.resize(size_t(id) + 1);
    }
    assets_[id] = std::move(asset);
    return id;
  }

  T& get(int id) {
    static T sentinel;
    CHECK_ID(id,sentinel);
    return assets_[id];
  }

  // NOTE: absolutely no bounds checking!
  // only used in rare cases where you
  // already know the asset exists
  T& operator[](int id) {
    return assets_[id];
  }

  int replace(int id, T&& asset) {
    CHECK_ID(id, -1);
    assets_[id].destroy();
    assets_[id] = std::move(asset);
    return id;
  }

  int duplicate(int id) {
    CHECK_ID(id, -1);
    T asset = assets_[id];
    return add(asset);
  }

  void destroy(int id) {
    CHECK_ID_V(id);
    auto& asset = assets_[id];
    asset.destroy();
  }

  size_t size() const { return assets_.size(); }
  bool exists(int id) const { return (id >= 0 && size_t(id) < size() && !assets_[id].isDestroyed()); }

 private:
  std::vector<T> assets_;
};

} // namespace enigma

#endif // E_ASSET_ARRAY
