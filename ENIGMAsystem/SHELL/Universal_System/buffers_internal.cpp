/** Copyright (C) 2013 Robert B. Colton
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
#include "buffers_internal.h"

namespace enigma {
BinaryBufferAsset::BinaryBufferAsset(std::unique_ptr<BinaryBuffer> &&asset_): asset{std::move(asset_)} {}

BinaryBuffer *BinaryBufferAsset::get() {
  return asset.get();
}

const BinaryBuffer *BinaryBufferAsset::get() const {
  return asset.get();
}

BinaryBuffer *BinaryBufferAsset::operator->() {
  return asset.get();
}

const BinaryBuffer *BinaryBufferAsset::operator->() const {
  return asset.get();
}

std::unique_ptr<BinaryBuffer> &BinaryBufferAsset::to_ptr() {
  return asset;
}

const std::unique_ptr<BinaryBuffer> &BinaryBufferAsset::to_ptr() const {
  return asset;
}

const char *BinaryBufferAsset::getAssetTypeName() {
  return "BinaryBufferAsset";
}

bool BinaryBufferAsset::isDestroyed() const {
  return asset == nullptr;
}

void BinaryBufferAsset::destroy() {
  asset.reset(nullptr);
}
}