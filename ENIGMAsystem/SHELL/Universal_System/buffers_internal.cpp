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
#include "buffers.h"
#include "buffers_internal.h"
#include "../libEGMstd.h"


namespace enigma {
////////////////////////////////////////////////////////////////////////////////
// BinaryBuffer
////////////////////////////////////////////////////////////////////////////////
BinaryBuffer::BinaryBuffer(std::size_t size) {
  data.resize(size, std::byte{0});
  position = 0;
  alignment = 1;
  type = 0;
}

BinaryBuffer::BinaryBuffer(std::vector<std::byte> &&data, std::size_t position, std::size_t alignment, int type):
    data{std::move(data)}, position{position}, alignment{alignment}, type{type} {}

std::size_t BinaryBuffer::GetSize() { return data.size(); }

void BinaryBuffer::Resize(std::size_t size) { data.resize(size, std::byte{0}); }

void BinaryBuffer::Seek(long long offset) {
  position = offset;
  while (position >= GetSize()) {
    switch (type) {
      case enigma_user::buffer_grow:
        Resize(position + 1);
        return;
      case enigma_user::buffer_wrap:
        position -= GetSize();
        return;
      default:
        position = GetSize() - (position - GetSize());
        return;
    }
  }
}

std::byte BinaryBuffer::ReadByte() {
  Seek(position);
  std::byte byte = data[position];
  Seek(position + 1);
  return byte;
}

void BinaryBuffer::WriteByte(std::byte byte) {
  Seek(position);
  data[position] = byte;
  Seek(position + 1);
}

////////////////////////////////////////////////////////////////////////////////
// BinaryBufferAsset
////////////////////////////////////////////////////////////////////////////////
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