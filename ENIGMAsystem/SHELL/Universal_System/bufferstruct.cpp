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
#include "libEGMstd.h"

#include "Resources/AssetArray.h" // TODO: start actually using for this resource
#include "Graphics_Systems/graphics_mandatory.h"
#include "Graphics_Systems/General/GSsurface.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>

using std::string;

namespace enigma {
std::vector<BinaryBuffer*> buffers(0);

BinaryBuffer::BinaryBuffer(std::size_t size) {
  data.resize(size, std::byte{0});
  position = 0;
  alignment = 1;
  type = 0;
}

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

int get_free_buffer() {
  for (unsigned i = 0; i < buffers.size(); i++) {
    if (!buffers[i]) {
      return i;
    }
  }
  return buffers.size();
}

std::vector<std::byte> valToBytes(variant value, int type) {
  return enigma_user::serialize_to_type(value, type);
}
}  // namespace enigma

namespace enigma_user {
std::vector<std::byte> serialize_to_type(variant &value, int type) {
#define BYTE(x) static_cast<std::byte>(x)
  // If a positive value is too large for `intXX_t`, it'll be chopped in half because the conversion from double
  // to signed integer will truncate the highest sign bit and replace it with its own sign bit (which will be 0).
  switch (type) {
    case buffer_u8: case buffer_s8: case buffer_bool: {
      assert("Expected numeric value to be passed in" && value.type == ty_real);
      std::uint8_t as_int = 0;
      if (value.rval.d > std::numeric_limits<std::int8_t>::max()) {
        as_int = static_cast<std::uint8_t>(value.rval.d);
      } else {
        as_int = static_cast<std::int8_t>(value.rval.d);
      }
      return {BYTE(as_int)};
    }

    case buffer_u16: case buffer_s16: {
      assert("Expected numeric value to be passed in" && value.type == ty_real);
      std::uint16_t as_int = 0;
      if (value.rval.d > std::numeric_limits<std::int16_t>::max()) {

        as_int = static_cast<std::uint16_t>(value.rval.d);
      } else {
        as_int = static_cast<std::int16_t>(value.rval.d);
      }
      return {BYTE((as_int >> 8) & 0xff), BYTE((as_int) & 0xff)};
    }

    case buffer_u32: case buffer_s32: {
      assert("Expected numeric value to be passed in" && value.type == ty_real);
      std::uint32_t as_int = 0;
      if (value.rval.d > std::numeric_limits<std::int32_t>::max()) {
        as_int = static_cast<std::uint32_t>(value.rval.d);
      } else {
        as_int = static_cast<std::int32_t>(value.rval.d);
      }

      return {BYTE((as_int >> 24) & 0xff), BYTE((as_int >> 16) & 0xff),
              BYTE((as_int >> 8) & 0xff), BYTE(as_int & 0xff)};
    }

    case buffer_u64: {
      assert("Expected numeric value to be passed in" && value.type == ty_real);
      std::uint64_t as_int = static_cast<std::int64_t>(value.rval.d);
      return {BYTE((as_int >> 56) & 0xff), BYTE((as_int >> 48) & 0xff),
              BYTE((as_int >> 40) & 0xff), BYTE((as_int >> 32) & 0xff),
              BYTE((as_int >> 24) & 0xff), BYTE((as_int >> 16) & 0xff),
              BYTE((as_int >> 8) & 0xff), BYTE(as_int & 0xff)};
    }

    case buffer_f16:
      assert("Unimplemented!" && false);

    case buffer_f32: {
      static_assert(sizeof(float) == 4, "Expected `float` type to be 4 bytes wide");
      assert("Expected numeric value to be passed in" && value.type == ty_real);
      std::uint32_t as_int = bit_cast<std::uint32_t>(static_cast<float>(value.rval.d));
      return {BYTE((as_int >> 24) & 0xff), BYTE((as_int >> 16) & 0xff),
              BYTE((as_int >> 8) & 0xff), BYTE(as_int & 0xff)};
    }

    case buffer_f64: {
      static_assert(sizeof(double) == 8, "Expected `double` type to be 8 bytes wide");
      assert("Expected numeric value to be passed in" && value.type == ty_real);
      std::uint64_t as_int = bit_cast<std::uint64_t>(static_cast<double>(value.rval.d));
      return {BYTE((as_int >> 56) & 0xff), BYTE((as_int >> 48) & 0xff),
              BYTE((as_int >> 40) & 0xff), BYTE((as_int >> 32) & 0xff),
              BYTE((as_int >> 24) & 0xff), BYTE((as_int >> 16) & 0xff),
              BYTE((as_int >> 8) & 0xff), BYTE(as_int & 0xff)};
    }

    case buffer_string: case buffer_text: {
      std::string &val = value.sval();
      std::vector<std::byte> result;
      result.reserve(val.size() + 1);
      std::transform(val.begin(), val.end(), std::back_inserter(result),
                     [](char value) { return std::byte{static_cast<std::uint8_t>(value)}; });
      result.emplace_back(std::byte{0});
      return result;
    }

    default:
      return {};
  }
#undef BYTE
}

variant deserialize_from_type(std::vector<std::byte>::iterator first, std::vector<std::byte>::iterator last, int type) {
#define DOUBLE(x) static_cast<double>(x)
  switch (type) {
    case buffer_u8: case buffer_s8: case buffer_bool: {
      assert("Expected span to be of correct size" && std::distance(first, last) == 1);
      std::uint8_t value = static_cast<std::uint8_t>(*first++);
      assert(first == last);

      return {type == buffer_s8 ? DOUBLE(static_cast<int8_t>(value)) : value};
    }

    case buffer_u16: case buffer_s16: {
      assert("Expected span to be of correct size" && std::distance(first, last) == 2);
      std::uint16_t value = static_cast<std::uint16_t>(*first++);
      value = (value << 8) | static_cast<std::uint16_t>(*first++);
      assert(first == last);

      return {type == buffer_s16 ? DOUBLE(static_cast<int16_t>(value)) : DOUBLE(value)};
    }

    case buffer_u32: case buffer_s32: {
      assert("Expected span to be of correct size" && std::distance(first, last) == 4);
      std::uint32_t value = static_cast<std::uint32_t>(*(first++));
      value = (value << 8) | static_cast<std::uint32_t>(*(first++));
      value = (value << 8) | static_cast<std::uint32_t>(*(first++));
      value = (value << 8) | static_cast<std::uint32_t>(*(first++));
      assert(first == last);

      return {type == buffer_s32 ? DOUBLE(static_cast<int32_t>(value)) : value};
    }

    case buffer_u64: {
      assert("Expected span to be of correct size" && std::distance(first, last) == 8);
      std::uint64_t value = static_cast<std::uint64_t>(*first++);
      value = (value << 8) | static_cast<std::uint64_t>(*first++);
      value = (value << 8) | static_cast<std::uint64_t>(*first++);
      value = (value << 8) | static_cast<std::uint64_t>(*first++);
      value = (value << 8) | static_cast<std::uint64_t>(*first++);
      value = (value << 8) | static_cast<std::uint64_t>(*first++);
      value = (value << 8) | static_cast<std::uint64_t>(*first++);
      value = (value << 8) | static_cast<std::uint64_t>(*first++);
      assert(first == last);

      return {value};
    }

    case buffer_f16:
      assert("Unimplemented!" && false);

    case buffer_f32: {
      assert("Expected span to be of correct size" && std::distance(first, last) == 4);
      std::uint32_t as_int = static_cast<std::uint32_t>(*first++);
      as_int = (as_int << 8) | static_cast<std::uint32_t>(*first++);
      as_int = (as_int << 8) | static_cast<std::uint32_t>(*first++);
      as_int = (as_int << 8) | static_cast<std::uint32_t>(*first++);
      assert(first == last);

      return {bit_cast<float>(as_int)};
    }

    case buffer_f64: {
      assert("Expected span to be of correct size" && std::distance(first, last) == 8);
      std::uint64_t as_int = static_cast<std::uint64_t>(*first++);
      as_int = (as_int << 8) | static_cast<std::uint64_t>(*first++);
      as_int = (as_int << 8) | static_cast<std::uint64_t>(*first++);
      as_int = (as_int << 8) | static_cast<std::uint64_t>(*first++);
      as_int = (as_int << 8) | static_cast<std::uint64_t>(*first++);
      as_int = (as_int << 8) | static_cast<std::uint64_t>(*first++);
      as_int = (as_int << 8) | static_cast<std::uint64_t>(*first++);
      as_int = (as_int << 8) | static_cast<std::uint64_t>(*first++);
      assert(first == last);

      return {bit_cast<double>(as_int)};
    }

    case buffer_string: case buffer_text: {
      std::string result;
      result.reserve(std::distance(first, last));
      std::transform(first, last, std::back_inserter(result),
                     [](std::byte b) { return static_cast<char>(b); });

      return {std::move(result)};
    }

    default:
      return {};
  }
#undef DOUBLE
}


int buffer_create(unsigned size, int type, unsigned alignment) {
  enigma::BinaryBuffer* buffer = new enigma::BinaryBuffer(size);
  buffer->type = type;
  buffer->alignment = alignment;
  int id = enigma::get_free_buffer();
  enigma::buffers.insert(enigma::buffers.begin() + id, buffer);
  return id;
}

void buffer_delete(int buffer) {
  get_buffer(binbuff, buffer);
  delete binbuff;
  enigma::buffers[buffer] = nullptr;
}

bool buffer_exists(int buffer) {
  return (buffer >= 0 && (size_t)buffer < enigma::buffers.size() && enigma::buffers[buffer] != nullptr);
}

void buffer_copy(int src_buffer, unsigned src_offset, unsigned size, int dest_buffer, unsigned dest_offset) {
  get_buffer(srcbuff, src_buffer);
  get_buffer(dstbuff, dest_buffer);

  unsigned over = size - srcbuff->GetSize();
  switch (dstbuff->type) {
    case buffer_wrap:
      dstbuff->data.insert(dstbuff->data.begin() + dest_offset, srcbuff->data.begin() + src_offset,
                           srcbuff->data.begin() + src_offset + size - over);
      dstbuff->data.insert(dstbuff->data.begin() + dest_offset, srcbuff->data.begin(), srcbuff->data.begin() + over);
      break;
    case buffer_grow:
      dstbuff->data.insert(dstbuff->data.begin() + dest_offset, srcbuff->data.begin() + src_offset,
                           srcbuff->data.begin() + src_offset + size);
      break;
    default:
      dstbuff->data.insert(dstbuff->data.begin() + dest_offset, srcbuff->data.begin() + src_offset,
                           srcbuff->data.begin() + src_offset + size - over);
      break;
  }
}

void buffer_save(int buffer, string filename) {
  get_buffer(binbuff, buffer);
  std::ofstream myfile(filename.c_str());
  if (!myfile.is_open()) {
    DEBUG_MESSAGE("Unable to open file " + filename, MESSAGE_TYPE::M_ERROR);
    return;
  }
  myfile.write(reinterpret_cast<const char*>(&binbuff->data[0]), binbuff->data.size());
  myfile.close();
}

void buffer_save_ext(int buffer, string filename, unsigned offset, unsigned size) {
  get_buffer(binbuff, buffer);
  std::ofstream myfile(filename.c_str());
  if (!myfile.is_open()) {
    DEBUG_MESSAGE("Unable to open file " + filename, MESSAGE_TYPE::M_ERROR);
    return;
  }

  unsigned over = size - binbuff->GetSize();
  switch (binbuff->type) {
    case buffer_wrap:
      myfile.write(reinterpret_cast<const char*>(&binbuff->data[offset]), size - over);
      myfile.write(reinterpret_cast<const char*>(&binbuff->data[0]), over);
      break;
    case buffer_grow:
      //TODO: Might need to use min(size, binbuff->GetSize()); for the last parameter.
      //Depends on whether Stupido will write 0's to fill in the entire size you gave it even though the data isn't that big.
      myfile.write(reinterpret_cast<const char*>(&binbuff->data[offset]), size);
      break;
    default:
      myfile.write(reinterpret_cast<const char*>(&binbuff->data[offset]), binbuff->GetSize());
      break;
  }

  myfile.close();
}

int buffer_load(string filename) {
  enigma::BinaryBuffer* buffer = new enigma::BinaryBuffer(0);
  buffer->type = buffer_grow;
  buffer->alignment = 1;
  int id = enigma::get_free_buffer();
  enigma::buffers.insert(enigma::buffers.begin() + id, buffer);

  std::ifstream myfile(filename.c_str());
  if (!myfile.is_open()) {
    DEBUG_MESSAGE("Unable to open file " + filename, MESSAGE_TYPE::M_ERROR);
    return -1;
  }
  myfile.read(reinterpret_cast<char*>(&buffer->data[0]), myfile.tellg());
  myfile.close();

  return id;
}

void buffer_load_ext(int buffer, string filename, unsigned offset) {
  get_buffer(binbuff, buffer);

  std::ifstream myfile(filename.c_str());
  if (!myfile.is_open()) {
    DEBUG_MESSAGE("Unable to open file " + filename, MESSAGE_TYPE::M_ERROR);
    return;
  }
  std::vector<std::byte> data;
  myfile.read(reinterpret_cast<char*>(&data[0]), myfile.tellg());
  unsigned over = data.size() - binbuff->GetSize();
  switch (binbuff->type) {
    case buffer_wrap:
      binbuff->data.insert(binbuff->data.begin() + offset, data.begin(), data.end() - over);
      binbuff->data.insert(binbuff->data.begin(), data.begin(), data.begin() + over);
      break;
    case buffer_grow:
      binbuff->data.insert(binbuff->data.begin() + offset, data.begin(), data.end());
      break;
    default:
      binbuff->data.insert(binbuff->data.begin() + offset, data.begin(), data.end() - over);
      break;
  }

  myfile.close();
}

void buffer_fill(int buffer, unsigned offset, int type, variant value, unsigned size) {
  get_buffer(binbuff, buffer);
  unsigned nsize = offset + size;
  if (binbuff->GetSize() < nsize && binbuff->type == buffer_grow) {
    binbuff->data.resize(nsize);
  }
  std::vector<std::byte> bytes = enigma_user::serialize_to_type(value, type);
  std::size_t times = size / bytes.size();
  for (int i = 0; i < times; i++) {
    std::copy(bytes.begin(), bytes.end(), binbuff->data.begin() + i * bytes.size());
  }
  binbuff->Seek(0);
}
  
void *buffer_get_address(int buffer) {
  #ifdef DEBUG_MODE
  if (buffer < 0 or size_t(buffer) >= enigma::buffers.size() or !enigma::buffers[buffer]) {
    DEBUG_MESSAGE("Attempting to access non-existing buffer " + toString(buffer), MESSAGE_TYPE::M_USER_ERROR);
    return nullptr;
  }
  #endif
  enigma::BinaryBuffer *binbuff = enigma::buffers[buffer];
  return reinterpret_cast<void *>(binbuff->data.data());
}

unsigned buffer_get_size(int buffer) {
  get_bufferr(binbuff, buffer, -1);
  return binbuff->GetSize();
}

unsigned buffer_get_alignment(int buffer) {
  get_bufferr(binbuff, buffer, -1);
  return binbuff->alignment;
}

int buffer_get_type(int buffer) {
  get_bufferr(binbuff, buffer, -1);
  return binbuff->type;
}

void buffer_get_surface(int buffer, int surface, int mode, unsigned offset, int modulo) {
  //get_buffer(binbuff, buffer);
  //TODO: Write this function
  DEBUG_MESSAGE("Function unimplemented: buffer_get_surface", MESSAGE_TYPE::M_WARNING);
}

void buffer_set_surface(int buffer, int surface, int mode, unsigned offset, int modulo) {
  int tex = surface_get_texture(surface);
  int wid = surface_get_width(surface);
  int hgt = surface_get_height(surface);
  if (buffer_get_size(buffer) == buffer_sizeof(buffer_u64) * wid * hgt) {
    enigma::graphics_push_texture_pixels(tex, wid, hgt, (unsigned char *)buffer_get_address(buffer));
  } else { // execution can not continue safely with wrong buffer size
    DEBUG_MESSAGE("Buffer allocated with wrong length!", MESSAGE_TYPE::M_WARNING);
  }
}

void buffer_resize(int buffer, unsigned size) {
  get_buffer(binbuff, buffer);
  binbuff->Resize(size);
}

void buffer_seek(int buffer, int base, long long offset) {
  get_buffer(binbuff, buffer);
  switch (base) {
    case buffer_seek_start:
      binbuff->Seek(offset);
      break;
    case buffer_seek_end:
      binbuff->Seek(binbuff->GetSize() + offset);
      break;
    case buffer_seek_relative:
      binbuff->Seek(binbuff->position + offset);
      break;
  }
}

unsigned buffer_sizeof(int type) {
  switch (type) {
    case buffer_u8: case buffer_s8: case buffer_bool:
      return 1;
    case buffer_u16: case buffer_s16: case buffer_f16:
      return 2;
    case buffer_u32: case buffer_s32: case buffer_f32:
      return 4;
    case buffer_u64: case buffer_f64:
      return 8;
    case buffer_string: case buffer_text: default:
      break;
  }
  return 0;
}

int buffer_tell(int buffer) {
  get_bufferr(binbuff, buffer, -1);
  return binbuff->position;
}

variant buffer_peek(int buffer, unsigned offset, int type) {
  get_bufferr(binbuff, buffer, -1);
  binbuff->Seek(offset);
  if (type != buffer_string) {
    //unsigned dsize = buffer_sizeof(type) + binbuff->alignment - 1;
    //NOTE: These buffers most likely need a little more code added to take care of endianess on different architectures.
    //TODO: Fix floating point precision.
    variant value = deserialize_from_type(binbuff->data.begin() + offset, binbuff->data.begin() + offset + buffer_sizeof(type), type);
    binbuff->Seek(binbuff->position + buffer_sizeof(type));
    return value;
  } else {
    char byte = '1';
    std::vector<char> data;
    while (byte != 0x00) {
      byte = static_cast<char>(binbuff->ReadByte());
      data.push_back(byte);
    }
    return variant(&data[0]);
  }
}

variant buffer_read(int buffer, int type) {
  get_bufferr(binbuff, buffer, -1);
  return buffer_peek(buffer, binbuff->position, type);
}

void buffer_poke(int buffer, unsigned offset, int type, variant value) {
  get_buffer(binbuff, buffer);
  binbuff->Seek(offset);
  if (type != buffer_string) {
    //TODO: Implement buffer alignment.
    //unsigned dsize = buffer_sizeof(type); //+ binbuff->alignment - 1;
    std::vector<std::byte> data = enigma::valToBytes(value, type);
    for (unsigned i = 0; i < data.size(); i++) {
      binbuff->WriteByte(data[i]);
    }
  } else {
    std::byte byte{'1'};
    unsigned pos = 0;
    while (byte != std::byte{0x00}) {
      byte = static_cast<std::byte>(value[pos]);
      pos += 1;
      binbuff->WriteByte(byte);
    }
    if (binbuff->alignment > pos) {
      for (unsigned i = 0; i < binbuff->alignment - pos; i++) {
        binbuff->WriteByte(std::byte{0});
      }
    }
  }
}

void buffer_write(int buffer, int type, variant value) {
  get_buffer(binbuff, buffer);
  buffer_poke(buffer, binbuff->position, type, value);
}

string buffer_md5(int buffer, unsigned offset, unsigned size) {
  //get_bufferr(binbuff, buffer, 0);
  //TODO: Write this function
  return NULL;
}

string buffer_sha1(int buffer, unsigned offset, unsigned size) {
  //get_bufferr(binbuff, buffer, 0);
  //TODO: Write this function
  return NULL;
}

int buffer_base64_decode(string str) {
  enigma::BinaryBuffer* buffer = new enigma::BinaryBuffer(0);
  buffer->type = buffer_grow;
  buffer->alignment = 1;
  int id = enigma::get_free_buffer();
  enigma::buffers.insert(enigma::buffers.begin() + id, buffer);
  //TODO: Write this function
  return id;
}

int buffer_base64_decode_ext(int buffer, string str, unsigned offset) {
  //get_bufferr(binbuff, buffer, -1);
  //TODO: Write this function
  return 0;
}

string buffer_base64_encode(int buffer, unsigned offset, unsigned size) {
  //get_bufferr(binbuff, buffer, 0);
  //TODO: Write this function
  return NULL;
}

void game_save_buffer(int buffer) {
  //get_buffer(binbuff, buffer);
  //TODO: Write this function
}

void game_load_buffer(int buffer) {
  //get_buffer(binbuff, buffer);
  //TODO: Write this function
}

}  // namespace enigma_user
