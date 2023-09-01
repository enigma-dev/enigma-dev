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

#ifndef E_ASSET_ARRAY
#define E_ASSET_ARRAY

#include <vector>
#include <string>
#include <utility>

#include "Universal_System/Serialization/serialization.h"
#include "Universal_System/Serialization/detect_serialization.h"

#ifdef DEBUG_MODE
  #include "Widget_Systems/widgets_mandatory.h" // for DEBUG_MESSAGE
  #define CHECK_ID(id, ret) \
    if (!exists(id)) { \
      DEBUG_MESSAGE("Requested " + (std::string)T::getAssetTypeName() + " asset " + std::to_string(id) + " does not exist.", MESSAGE_TYPE::M_USER_ERROR); \
      return ret; \
    }
  #define CHECK_ID_V(id) CHECK_ID(id,)
#else
  #define CHECK_ID(id, ret)
  #define CHECK_ID_V(id)
#endif

namespace enigma_user {
int background_get_width(int backId);
int background_get_height(int backId);
}

namespace enigma {
struct Background;

template<typename T, int LEFT> 
class OffsetVector {
  std::vector<T> data_owner_;
  T* data_;

 public:
  OffsetVector(): data_(nullptr) {}
  OffsetVector(const OffsetVector<T, LEFT> &other):
    data_owner_(other.data_owner_), data_(data_owner_.data() - LEFT) {}
  size_t size() const {
    return data_owner_.size() + LEFT;
  }
  T *data() { return data_; }
  const T *data() const { return data_; }
  template<typename... U> size_t push_back(U... args) {
    data_owner_.push_back(args...);
    data_ = data_owner_.data() - LEFT;
    return size() - 1;
  }
  template<typename... U> size_t emplace_back(U... args) {
    data_owner_.emplace_back(std::move(args)...);
    data_ = data_owner_.data() - LEFT;
    return size() - 1;
  }
  template<typename ind_t> T& operator[](ind_t index) {
    return data()[index];
  }
  template<typename ind_t> const T& operator[](ind_t index) const {
    return data()[index];
  }
  void resize(size_t count) {
    data_owner_.resize(count - LEFT);
    data_ = data_owner_.data() - LEFT;
  }
};

template<typename T> class OffsetVector<T, 0> {
  std::vector<T> data_owner_;
 public:
  size_t size() const {
    return data_owner_.size();
  }
  T *data() { return data_owner_.data(); }
  const T *data() const { return data_owner_.data(); }
  template<typename... U> size_t push_back(U... args) {
    data_owner_.push_back(args...);
    return data_owner_.size() - 1;
  }
  template<typename... U> size_t emplace_back(U... args) {
    data_owner_.emplace_back(std::move(args)...);
    return size() - 1;
  }
  template<typename ind_t> T& operator[](ind_t index) {
    return data()[index];
  }
  template<typename ind_t> const T& operator[](ind_t index) const {
    return data()[index];
  }
  void resize(size_t count) {
    data_owner_.resize(count);
  }
};

// Asset storage container designed for dense cache-efficient resource processing.
template<typename T, int LEFT = 0>
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

  iterator begin() { return ++iterator(*this, -1); }
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
        DEBUG_MESSAGE("Attempting to assign " + (std::string)T::getAssetTypeName() + " asset " + std::to_string(id) + " to negative index.", MESSAGE_TYPE::M_USER_ERROR);
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
  
  const T& get(int id) const {
    static T sentinel;
    CHECK_ID(id,sentinel);
    return assets_[id];
  }

  // NOTE: absolutely no bounds checking!
  // only used in rare cases where you
  // already know the asset exists
  T& operator[](int id) noexcept {
    return assets_[id];
  }

  const T& operator[](int id) const noexcept {
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
    return add(std::move(asset));
  }

  void destroy(int id) {
    CHECK_ID_V(id);
    auto& asset = assets_[id];
    asset.destroy();
  }

  size_t size() const { return assets_.size(); }
  bool exists(int id) const { return (id >= 0 && size_t(id) < size() && !assets_[id].isDestroyed()); }

  T* data() { return assets_.data(); }

  void resize(size_t count) {
    assets_.resize(count);
  }

  std::size_t byte_size() const noexcept {
    std::size_t len = sizeof(std::size_t);
    for (std::size_t i = 0; i < size(); i++) {
      len += assets_[i].byte_size();
    }
    return len;
  }

  // Bytes (de)Serialization
  std::vector<std::byte> serialize() const {
    static_assert(has_serialize_method_v<T> || HAS_INTERNAL_SERIALIZE_FUNCTION(),
                  "Given type is required to have at least one of `x.serialize()` or `serialize(x)`.");

    std::vector<std::byte> result{};
    std::size_t len = 0;
    enigma::bytes_serialization::enigma_serialize(assets_.size(), len, result);
    for (std::size_t i = 0; i < assets_.size(); i++) {
      enigma::bytes_serialization::enigma_serialize(operator[](i), len, result);
    }
    result.shrink_to_fit();
    return result;
  }

  std::size_t deserialize_self(std::byte *iter) {
    static_assert(has_deserialize_self_method_v<T> || has_deserialize_function_v<T> || HAS_DESERIALIZE_FUNCTION(),
                  "Given type is required to have at least one of `x.deserialize_self()` or `deserialize<T>()`");

    std::size_t len = 0;
    std::size_t elements = 0;
    enigma::bytes_serialization::enigma_deserialize(elements, iter, len);
    resize(elements);
    for (std::size_t i = 0; i < elements; i++) {
      enigma::bytes_serialization::enigma_deserialize(assets_[i], iter, len);
    }
    return len;
  }

  static std::pair<AssetArray<T, LEFT>, std::size_t> deserialize(std::byte *iter) {
    if constexpr (has_deserialize_self_method_v<T> || has_deserialize_function_v<T> || HAS_DESERIALIZE_FUNCTION()) {
      AssetArray<T, LEFT> result;
      auto len = result.deserialize_self(iter);
      return {std::move(result), len};
    }
    return {};
  }

  // JSON (de)Serialization
  std::string json_serialize() const {
    static_assert(has_json_serialize_method_v<T> || _JSON_serialization_has_internal_serialize_into_fn_<T>,
                  "Given type is required to have at least one of `x.json_serialize()` or `internal_serialize_into_fn(x)`.");

    std::string result = "[";
    for (std::size_t i = 0; i < assets_.size(); i++) {
      result += enigma::JSON_serialization::enigma_serialize(operator[](i));
      if (i != assets_.size() - 1) {
        result += ",";
      }
    }
    result += "]";
    return result;
  }

  void json_deserialize_self(const std::string & json) {
    static_assert(has_json_deserialize_function_v<T> || _JSON_serialization_has_internal_deserialize_fn_<T> ||has_json_deserialize_self_method_v<T>,
                  "Given type is required to have at least one of `x::json_deserialize()`, `internal_deserialize_fn<T>() or x.json_deserialize_self()`");

    if (json.length() > 2) { 
    std::string jsonCopy = json.substr(1, json.length() - 2);
    std::vector<std::string> parts = enigma::JSON_serialization::json_split(jsonCopy, ',');
    for (auto it = parts.begin(); it != parts.end(); ++it)
      assets_.emplace_back(enigma::JSON_serialization::enigma_deserialize<T>(*it));
  }
  }

  static AssetArray<T, LEFT> json_deserialize(const std::string & json) {
    if constexpr (has_json_deserialize_function_v<T> || _JSON_serialization_has_internal_deserialize_fn_<T>||has_json_deserialize_self_method_v<T>) {
      AssetArray<T, LEFT> result;
      result.json_deserialize_self(json);
      return result;
    }
    return {};
  }

 private:
  OffsetVector<T, LEFT> assets_;
};

} // namespace enigma

#endif // E_ASSET_ARRAY
