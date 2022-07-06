/** Copyright (C) 2013 Robert B. Colton
*** Copyright (C) 2022 Dhruv Chawla
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

#ifndef ENIGMA_BUFFERS_INTERNAL_H
#define ENIGMA_BUFFERS_INTERNAL_H

#include "buffers_data.h"
#include "Resources/AssetArray.h"
#include <memory>
#include <vector>

namespace enigma
{
  struct BinaryBuffer
  {
    std::vector<std::byte> data;
    std::size_t position;
    std::size_t alignment;
    enigma_user::buffer_type_t type;
    
    BinaryBuffer(std::size_t size);
    BinaryBuffer(std::vector<std::byte> &&data, std::size_t position, std::size_t alignment, enigma_user::buffer_type_t type);
    ~BinaryBuffer() = default;

    std::size_t GetSize();
    void Resize(std::size_t size);
    void Seek(long long offset);

    std::byte ReadByte();
    void WriteByte(std::byte byte);
  };

  struct BinaryBufferAsset {
    std::unique_ptr<BinaryBuffer> asset;

    BinaryBufferAsset() = default;
    BinaryBufferAsset(std::unique_ptr<BinaryBuffer> &&asset_);

    BinaryBuffer *operator->();
    const BinaryBuffer *operator->() const;
    BinaryBuffer *get();
    const BinaryBuffer *get() const;

    std::unique_ptr<BinaryBuffer> &to_ptr();
    const std::unique_ptr<BinaryBuffer> &to_ptr() const;

    // Necessary for AssetArray interface
    static const char* getAssetTypeName();
    bool isDestroyed() const;
    void destroy();
  };
  
  extern AssetArray<BinaryBufferAsset> buffers;
}

#ifdef DEBUG_MODE
#include "Widget_Systems/widgets_mandatory.h"
#define GET_BUFFER(binbuff, buff)                                                                            \
  if (buff < 0 or size_t(buff) >= enigma::buffers.size() or enigma::buffers[buff].get() == nullptr) {        \
    DEBUG_MESSAGE("Attempting to access non-existing buffer " + toString(buff), MESSAGE_TYPE::M_USER_ERROR); \
    return;                                                                                                  \
  }                                                                                                          \
  enigma::BinaryBuffer *binbuff = enigma::buffers[buff].get();
#define GET_BUFFER_R(binbuff, buff, r)                                                                       \
  if (buff < 0 or size_t(buff) >= enigma::buffers.size() or enigma::buffers[buff].get() == nullptr) {        \
    DEBUG_MESSAGE("Attempting to access non-existing buffer " + toString(buff), MESSAGE_TYPE::M_USER_ERROR); \
    return r;                                                                                                \
  }                                                                                                          \
  enigma::BinaryBuffer *binbuff = enigma::buffers[buff].get();
#else
#define GET_BUFFER(binbuff, buff) enigma::BinaryBuffer *binbuff = enigma::buffers[buff].get();
#define GET_BUFFER_R(binbuff, buff, r) enigma::BinaryBuffer *binbuff = enigma::buffers[buff].get();
#endif

#endif // ENIGMA_BUFFERS_INTERNAL_H
