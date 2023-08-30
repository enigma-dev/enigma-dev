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

#include "Universal_System/buffers_data.h"
#include "Universal_System/estring.h"
#include "buffers.h"
#include "buffers_internal.h"

#include "md5.h"
#include "sha1.h"
#include "Resources/AssetArray.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Graphics_Systems/General/GSsurface.h"
#include "sha1.h"
#include "Universal_System/Instances/instance.h"
#include "Universal_System/Instances/instance_system.h"
#include "Universal_System/Instances/instance_system_frontend.h"
#include "Universal_System/Serialization/serialization.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/Resources/backgrounds_internal.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <zlib.h>

#define ZLIB_CHUNK_SIZE 16384

namespace enigma {
AssetArray<BinaryBufferAsset> buffers{};

std::size_t get_free_buffer() {
  for (std::size_t i = 0; i < buffers.size(); i++) {
    if (buffers[i].get() == nullptr) {
      return i;
    }
  }
  return buffers.size();
}
}  // namespace enigma

namespace enigma_user {
std::vector<std::byte> serialize_to_type(variant &value, buffer_data_t type) {
#define BYTE(x) static_cast<std::byte>(x)
  // If a positive value is too large for `intXX_t`, it'll be chopped in half because the conversion from double
  // to signed integer will truncate the highest sign bit and replace it with its own sign bit (which will be 0).
  switch (type) {
    case buffer_u8: case buffer_s8: case buffer_bool: {
      if (value.type != ty_real) {
        DEBUG_MESSAGE("serialize_to_type: Expected numeric value to be passed in", MESSAGE_TYPE::M_FATAL_ERROR);
      }

      std::uint8_t as_int = 0;
      if (value.rval.d > std::numeric_limits<std::int8_t>::max()) {
        as_int = static_cast<std::uint8_t>(value.rval.d);
      } else {
        as_int = static_cast<std::int8_t>(value.rval.d);
      }
      return {BYTE(as_int)};
    }

    case buffer_u16: case buffer_s16: {
      if (value.type != ty_real) {
        DEBUG_MESSAGE("serialize_to_type: Expected numeric value to be passed in", MESSAGE_TYPE::M_FATAL_ERROR);
      }

      std::uint16_t as_int = 0;
      if (value.rval.d > std::numeric_limits<std::int16_t>::max()) {
        as_int = static_cast<std::uint16_t>(value.rval.d);
      } else {
        as_int = static_cast<std::int16_t>(value.rval.d);
      }
      return {BYTE((as_int >> 8) & 0xff), BYTE((as_int) & 0xff)};
    }

    case buffer_u32: case buffer_s32: {
      if (value.type != ty_real) {
        DEBUG_MESSAGE("serialize_to_type: Expected numeric value to be passed in", MESSAGE_TYPE::M_FATAL_ERROR);
      }

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
      if (value.type != ty_real) {
        DEBUG_MESSAGE("serialize_to_type: Expected numeric value to be passed in", MESSAGE_TYPE::M_FATAL_ERROR);
      }

      std::uint64_t as_int = static_cast<std::int64_t>(value.rval.d);
      return {BYTE((as_int >> 56) & 0xff), BYTE((as_int >> 48) & 0xff),
              BYTE((as_int >> 40) & 0xff), BYTE((as_int >> 32) & 0xff),
              BYTE((as_int >> 24) & 0xff), BYTE((as_int >> 16) & 0xff),
              BYTE((as_int >> 8) & 0xff), BYTE(as_int & 0xff)};
    }

    case buffer_f16:
      DEBUG_MESSAGE("serialize_to_type: buffer_f16 is unimplemented!", MESSAGE_TYPE::M_FATAL_ERROR);
      return {};

    case buffer_f32: {
      static_assert(sizeof(float) == 4, "Expected `float` type to be 4 bytes wide");
      if (value.type != ty_real) {
        DEBUG_MESSAGE("serialize_to_type: Expected numeric value to be passed in", MESSAGE_TYPE::M_FATAL_ERROR);
      }

      std::uint32_t as_int = bit_cast<std::uint32_t>(static_cast<float>(value.rval.d));
      return {BYTE((as_int >> 24) & 0xff), BYTE((as_int >> 16) & 0xff),
              BYTE((as_int >> 8) & 0xff), BYTE(as_int & 0xff)};
    }

    case buffer_f64: {
      static_assert(sizeof(double) == 8, "Expected `double` type to be 8 bytes wide");
      if (value.type != ty_real) {
        DEBUG_MESSAGE("serialize_to_type: Expected numeric value to be passed in", MESSAGE_TYPE::M_FATAL_ERROR);
      }

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

variant deserialize_from_type(std::vector<std::byte>::iterator first, std::vector<std::byte>::iterator last, buffer_data_t type) {
#define DOUBLE(x) static_cast<double>(x)
  switch (type) {
    case buffer_u8: case buffer_s8: case buffer_bool: {
      if (std::distance(first, last) != 1) {
        DEBUG_MESSAGE("deserialize_from_type: Expected span to be of correct size", MESSAGE_TYPE::M_FATAL_ERROR);
      }

      std::uint8_t value = static_cast<std::uint8_t>(*first++);
      assert(first == last);

      return {type == buffer_s8 ? DOUBLE(static_cast<int8_t>(value)) : value};
    }

    case buffer_u16: case buffer_s16: {
      if (std::distance(first, last) != 2) {
        DEBUG_MESSAGE("deserialize_from_type: Expected span to be of correct size", MESSAGE_TYPE::M_FATAL_ERROR);
      }

      std::uint16_t value = static_cast<std::uint16_t>(*first++);
      value = (value << 8) | static_cast<std::uint16_t>(*first++);
      assert(first == last);

      return {type == buffer_s16 ? DOUBLE(static_cast<int16_t>(value)) : value};
    }

    case buffer_u32: case buffer_s32: {
      if (std::distance(first, last) != 4) {
        DEBUG_MESSAGE("deserialize_from_type: Expected span to be of correct size", MESSAGE_TYPE::M_FATAL_ERROR);
      }

      std::uint32_t value = static_cast<std::uint32_t>(*(first++));
      value = (value << 8) | static_cast<std::uint32_t>(*(first++));
      value = (value << 8) | static_cast<std::uint32_t>(*(first++));
      value = (value << 8) | static_cast<std::uint32_t>(*(first++));
      assert(first == last);

      return {type == buffer_s32 ? DOUBLE(static_cast<int32_t>(value)) : value};
    }

    case buffer_u64: {
      if (std::distance(first, last) != 8) {
        DEBUG_MESSAGE("deserialize_from_type: Expected span to be of correct size", MESSAGE_TYPE::M_FATAL_ERROR);
      }

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
      assert("deserialize_from_type: buffer_f16 is unimplemented!" && false);

    case buffer_f32: {
      if (std::distance(first, last) != 4) {
        DEBUG_MESSAGE("deserialize_from_type: Expected span to be of correct size", MESSAGE_TYPE::M_FATAL_ERROR);
      }

      std::uint32_t as_int = static_cast<std::uint32_t>(*first++);
      as_int = (as_int << 8) | static_cast<std::uint32_t>(*first++);
      as_int = (as_int << 8) | static_cast<std::uint32_t>(*first++);
      as_int = (as_int << 8) | static_cast<std::uint32_t>(*first++);
      assert(first == last);

      return {bit_cast<float>(as_int)};
    }

    case buffer_f64: {
      if (std::distance(first, last) != 8) {
        DEBUG_MESSAGE("deserialize_from_type: Expected span to be of correct size", MESSAGE_TYPE::M_FATAL_ERROR);
      }

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

void write_to_buffer(enigma::BinaryBuffer *binbuff, const std::vector<std::byte> &data, std::size_t offset) {
  switch (binbuff->type) {
    case buffer_wrap:
      if (offset >= binbuff->GetSize()) {
        offset = offset % binbuff->GetSize();
      }

      if (data.size() + offset > binbuff->data.size()) {
        if (data.size() > binbuff->data.size()) {
          // Instead of doing more writes than we need, we can just find out the subset of the bytes that we will be
          // writing to the buffer
          std::size_t first = data.size() - binbuff->data.size();
          std::copy(data.begin() + first, data.end() - offset, binbuff->data.begin() + offset);
          std::copy(data.end() - offset, data.end(), binbuff->data.begin());
        } else {
          std::size_t extra = (data.size() + offset) - binbuff->data.size();
          std::copy(data.begin(), data.end() - extra, binbuff->data.begin() + offset);
          std::copy(data.end() - extra, data.end(), binbuff->data.begin());
        }
      } else {
        std::copy(data.begin(), data.end(), binbuff->data.begin() + offset);
      }
      break;

    case buffer_grow:
      if (data.size() + offset > binbuff->data.size()) {
        binbuff->Resize(data.size() + offset);
      }
      std::copy(data.begin(), data.end(), binbuff->data.begin() + offset);
      break;

    case buffer_fixed:
    case buffer_fast: {
      std::size_t over = 0;
      if (offset >= binbuff->data.size()) {
        DEBUG_MESSAGE("write_to_buffer: Offset beyond end of fixed/fast buffer, aborting write", MESSAGE_TYPE::M_USER_ERROR);
        return;
      } else if (data.size() + offset > binbuff->data.size()) {
        DEBUG_MESSAGE("write_to_buffer: Data being read cannot fit into fixed/fast buffer, truncating", MESSAGE_TYPE::M_WARNING);
        over = (data.size() + offset) - binbuff->data.size();
      }
      std::copy(data.begin(), data.end() - over, binbuff->data.begin() + offset);
      break;
    }
  }
}

void write_to_file(std::ofstream &file, const std::vector<std::byte>::iterator bytes, std::size_t size) {
  // Hide the unsafe code inside a safe abstraction
  file.write(reinterpret_cast<const char *>(&*bytes), size);
}

std::vector<std::byte> read_from_file(std::ifstream &file, std::size_t size) {
  // Hide the unsafe code inside a safe abstraction
  std::vector<std::byte> result{};
  result.resize(size);
  file.read(reinterpret_cast<char *>(result.data()), size);
  return result;
}

void read_from_file(std::ifstream &file, std::vector<std::byte>::iterator bytes, std::size_t size) {
  // Hide the unsafe code inside a safe abstraction
  // Very easy buffer overflow bug here, caller has to ensure enough space is there
  file.read(reinterpret_cast<char *>(&*bytes), size);
}

char to_base64_char(std::uint8_t index) {
  if (index > 63) {
    DEBUG_MESSAGE("to_base64_char: index (" + std::to_string(index) + ") out of range", MESSAGE_TYPE::M_FATAL_ERROR);
    return '!';
  } else {
    return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[index];
  }
}

std::uint8_t from_base64_char(char ch) {
  if ('A' <= ch && ch <= 'Z') {
    return ch - 'A';
  } else if ('a' <= ch && ch <= 'z') {
    return 26 + (ch - 'a');
  } else if ('0' <= ch && ch <= '9') {
    return 52 + (ch - '0');
  } else if (ch == '+') {
    return 62;
  } else if (ch == '/') {
    return 63;
  } else if (ch == '=') {
    return 0;
  } else {
    DEBUG_MESSAGE("from_base64_char: character '" + std::string{ch} + "' cannot be converted to an index",
                  MESSAGE_TYPE::M_FATAL_ERROR);
    return 255;
  }
}

std::string internal_buffer_base64_encode(std::vector<std::byte>::iterator bytes, std::size_t size) {
  std::string result{};

  std::size_t remainder = size % 3;
  std::size_t i = 0;
  for (; i < size - remainder; i += 3) {
    std::uint32_t triplet = static_cast<std::uint32_t>(bytes[i]) << 16 |
                            static_cast<std::uint32_t>(bytes[i + 1]) << 8 |
                            static_cast<std::uint32_t>(bytes[i + 2]);

    std::uint8_t first  = (triplet & (0b111111 << 18)) >> 18;
    std::uint8_t second = (triplet & (0b111111 << 12)) >> 12;
    std::uint8_t third  = (triplet & (0b111111 << 6)) >> 6;
    std::uint8_t fourth = triplet & 0b111111;
    result += to_base64_char(first);
    result += to_base64_char(second);
    result += to_base64_char(third);
    result += to_base64_char(fourth);
  }

  // We have some of a triplet still left
  if (remainder != 0) {
    if (remainder == 1) {
      std::uint8_t singlet = static_cast<std::uint8_t>(bytes[size - 1]);
      std::uint8_t first  = (singlet & (0b111111 << 2)) >> 2;
      std::uint8_t second = (singlet & 0b11) << 4;

      result += to_base64_char(first);
      result += to_base64_char(second);
      result += "==";
    } else if (remainder == 2) {
      std::uint16_t doublet = static_cast<std::uint8_t>(bytes[size - 2]) << 8 |
                             static_cast<std::uint8_t>(bytes[size - 1]);
      std::uint8_t first  = (doublet & (0b111111 << 10)) >> 10;
      std::uint8_t second = (doublet & (0b111111 << 4)) >> 4;
      std::uint8_t third  = (doublet & 0b11111) << 2;

      result += to_base64_char(first);
      result += to_base64_char(second);
      result += to_base64_char(third);
      result += '=';
    }
  }

  return result;
}

std::vector<std::byte> internal_buffer_base64_decode(std::string_view str) {
  if (str.length() % 4 != 0) {
    DEBUG_MESSAGE("internal_buffer_base64_decode: string length not a multiple of 4, padded incorrectly",
                  MESSAGE_TYPE::M_WARNING);
  }

  std::vector<std::byte> result{};

  std::size_t remainder = str.length() % 4;
  std::size_t i = 0;
  for (; i < str.length() - remainder; i += 4) {
    std::uint32_t value = from_base64_char(str[i]) << 18 |
                          from_base64_char(str[i + 1]) << 12 |
                          from_base64_char(str[i + 2]) << 6 |
                          from_base64_char(str[i + 3]);

    result.push_back(static_cast<std::byte>(value >> 16));
    if (str[i + 2] != '=') {
      result.push_back(static_cast<std::byte>(value >> 8));
    }
    if (str[i + 3] != '=') {
      result.push_back(static_cast<std::byte>(value));
    }
  }

  // We have unpadded data :(
  if (remainder != 0) {
    if (remainder == 2) {
      std::uint8_t value = from_base64_char(str[str.length() - 2]) << 2 |
                           from_base64_char(str[str.length() - 1]) >> 4;

      result.push_back(std::byte{value});
    } else if (remainder == 3) {
      std::uint16_t value = from_base64_char(str[str.length() - 3]) << 10 |
                            from_base64_char(str[str.length() - 2]) << 4 |
                            from_base64_char(str[str.length() - 1]) >> 2;

      result.push_back(static_cast<std::byte>(value >> 8));
      result.push_back(static_cast<std::byte>(value & 0b11111111));
    }
  }

  return result;
}

extern char to_base16_char(std::uint8_t index);

std::string internal_md5(const std::string &str) {
  return md5_string_utf8(str);
}

std::string internal_md5(std::vector<std::byte>::iterator bytes, std::size_t size) {
  MD5_CTX ctx;
  std::uint8_t digest[16];

  MD5Init(&ctx);
  MD5Update(&ctx, reinterpret_cast<std::uint8_t *>(&*bytes), size);
  MD5Final(digest, &ctx);

  std::string result{};
  result.reserve(32);
  for (std::uint8_t value : digest) {
    result += to_base16_char(value >> 4);
    result += to_base16_char(value & 0xf);
  }

  return result;
}

std::string internal_sha1(const std::string &str) {
  return sha1_string_utf8(str);
}

std::string internal_sha1(std::vector<std::byte>::iterator bytes, std::size_t size) {
  SHA1Context ctx;
  std::uint8_t message_digest[20];
  int err = SHA1Reset(&ctx);
  if (err != 0) {
    DEBUG_MESSAGE("internal_sha1: sha1 error (" + std::to_string(err) + ")", MESSAGE_TYPE::M_FATAL_ERROR);
    return "";
  }

  err = SHA1Input(&ctx, reinterpret_cast<const std::uint8_t *>(&*bytes), size);
  if (err != 0) {
    DEBUG_MESSAGE("internal_sha1: sha1 error (" + std::to_string(err) + ")", MESSAGE_TYPE::M_FATAL_ERROR);
    return "";
  }

  err = SHA1Result(&ctx, message_digest);
  if (err != 0) {
    DEBUG_MESSAGE("internal_sha1: sha1 error (" + std::to_string(err) + ")", MESSAGE_TYPE::M_FATAL_ERROR);
    return "";
  }

  std::string result{};
  result.reserve(40);
  for (std::uint8_t value : message_digest) {
    result += to_base16_char(value >> 4);
    result += to_base16_char(value & 0xf);
  }

  return result;
}

buffer_t make_new_buffer(std::size_t size, buffer_type_t type, std::size_t alignment) {
  auto buffer = std::make_unique<enigma::BinaryBuffer>(std::vector<std::byte>(size), 0, alignment, type);
  if (std::size_t id = enigma::get_free_buffer(); id == enigma::buffers.size()) {
    return enigma::buffers.add(std::move(buffer));
  } else {
    enigma::buffers.assign(id, std::move(buffer));
    return id;
  }
}

buffer_t buffer_create(std::size_t size, buffer_type_t type, std::size_t alignment) {
  return make_new_buffer(size, type, alignment);
}

void buffer_delete(buffer_t buffer) {
  enigma::buffers.destroy(buffer);
}

bool buffer_exists(buffer_t buffer) {
  return (buffer >= 0 && static_cast<std::size_t>(buffer) < enigma::buffers.size() &&
          enigma::buffers[buffer].get() != nullptr);
}

void buffer_copy(buffer_t src_buffer, std::size_t src_offset, std::size_t size, buffer_t dest_buffer, std::size_t dest_offset) {
  GET_BUFFER(srcbuff, src_buffer);
  GET_BUFFER(dstbuff, dest_buffer);

  if (src_offset >= srcbuff->GetSize()) {
    if (srcbuff->type == buffer_wrap) {
      src_offset = src_offset % srcbuff->GetSize();
    } else {
      DEBUG_MESSAGE("buffer_copy: source offset greater than source size, aborting write", MESSAGE_TYPE::M_ERROR);
      return;
    }
  }

  if (dest_offset >= dstbuff->GetSize()) {
    if (dstbuff->type == buffer_wrap) {
      dest_offset = dest_offset % dstbuff->GetSize();
    } else {
      DEBUG_MESSAGE("buffer_copy: destination offset greater than destination size, aborting write", MESSAGE_TYPE::M_ERROR);
      return;
    }
  }

  std::size_t written_bytes = [&srcbuff, &src_offset, &size]() {
    if (srcbuff->type == buffer_wrap) {
      return size;
    } else {
      return std::min(srcbuff->GetSize() - src_offset, size);
    }
  }();

  if (dstbuff->type == buffer_fixed || dstbuff->type == buffer_fast) {
    if (dest_offset + written_bytes > dstbuff->GetSize()) {
      DEBUG_MESSAGE("buffer_copy: bytes written out of range for fixed/fast buffer, truncating", MESSAGE_TYPE::M_WARNING);
      written_bytes = dstbuff->GetSize() - dest_offset;
    }
  }

  switch (dstbuff->type) {
    case buffer_wrap: {
      if (srcbuff->type == buffer_wrap && src_offset + written_bytes >= srcbuff->GetSize()) {
        for (std::size_t i = 0; i < size; i++) {
          dstbuff->data[(i + dest_offset) % dstbuff->GetSize()] = srcbuff->data[(i + src_offset) % srcbuff->GetSize()];
        }
      } else {
        for (std::size_t i = 0; i < size; i++) {
          dstbuff->data[(i + dest_offset) % dstbuff->GetSize()] = srcbuff->data[i + src_offset];
        }
      }
      break;
    }

    case buffer_grow: {
      if (dest_offset + written_bytes >= dstbuff->GetSize()) {
        dstbuff->Resize(dest_offset + written_bytes + 1);
      }

      [[fallthrough]];
    }

    case buffer_fixed:
    case buffer_fast: {
      if (srcbuff->type == buffer_wrap && src_offset + written_bytes >= srcbuff->GetSize()) {
        for (std::size_t i = 0; i < size; i++) {
          dstbuff->data[i + dest_offset] = srcbuff->data[(i + src_offset) % srcbuff->GetSize()];
        }
      } else {
        for (std::size_t i = 0; i < size; i++) {
          dstbuff->data[i + dest_offset] = srcbuff->data[i + src_offset];
        }
      }
      break;
    }
  }
}

void buffer_save(buffer_t buffer, string filename) {
  GET_BUFFER(binbuff, buffer);
  std::ofstream myfile(filename.c_str());
  if (!myfile.is_open()) {
    DEBUG_MESSAGE("Unable to open file " + filename, MESSAGE_TYPE::M_ERROR);
    return;
  }

  write_to_file(myfile, binbuff->data.begin(), binbuff->GetSize());
  myfile.close();
}

void buffer_save_ext(buffer_t buffer, string filename, std::size_t offset, std::size_t size) {
  GET_BUFFER(binbuff, buffer);
  std::ofstream myfile(filename.c_str());

  // NOTE: There is an incompatibility with GMS here, in terms of two things:
  // - GMS only seems to write the bytes which were actually written to the buffer, and not the entirety of the buffer
  //   itself, i.e. if you wrote 2 bytes to the buffer and saved it, the resulting file would be only 2 bytes long
  // - When @c offset is greater than @c size, GMS seems to write the last written byte to the file and then stops, which
  //   I really do not understand.

  if (!myfile.is_open()) {
    DEBUG_MESSAGE("Unable to open file " + filename, MESSAGE_TYPE::M_ERROR);
    return;
  }

  if (offset >= binbuff->GetSize()) {
    if (binbuff->type == buffer_wrap) {
      offset = offset % binbuff->GetSize();
    } else {
      DEBUG_MESSAGE("buffer_save_ext: offset beyond end of buffer, aborting write", MESSAGE_TYPE::M_ERROR);
      return;
    }
  }

  if (offset + size >= binbuff->GetSize()) {
    DEBUG_MESSAGE("buffer_save_ext: offset (" + std::to_string(offset) + ") + size (" + std::to_string(size) +
                  ") greater than buffer size (" + std::to_string(binbuff->GetSize()) + "), truncating to buffer end",
                  MESSAGE_TYPE::M_WARNING);
    size = binbuff->GetSize() - offset;
  }

  write_to_file(myfile, binbuff->data.begin() + offset, size);
  myfile.close();
}

buffer_t buffer_load(string filename) {
  buffer_t id = make_new_buffer(0, buffer_grow, 1);
  enigma::BinaryBuffer *buffer = enigma::buffers.get(id).get();

  std::ifstream myfile(filename.c_str());
  if (!myfile.is_open()) {
    DEBUG_MESSAGE("Unable to open file " + filename, MESSAGE_TYPE::M_ERROR);
    return -1;
  }

  auto size = std::filesystem::file_size(filename);
  buffer->data.resize(size);
  read_from_file(myfile, buffer->data.begin(), size);
  myfile.close();

  return id;
}

void buffer_load_ext(buffer_t buffer, string filename, std::size_t offset) {
  GET_BUFFER(binbuff, buffer);

  std::ifstream myfile(filename.c_str());
  if (!myfile.is_open()) {
    DEBUG_MESSAGE("Unable to open file " + filename, MESSAGE_TYPE::M_ERROR);
    return;
  }

  std::vector<std::byte> data = read_from_file(myfile, std::filesystem::file_size(filename));
  write_to_buffer(binbuff, data, offset);

  myfile.close();
}

void buffer_load_partial(buffer_t buffer, std::string filename, std::size_t src_offset, std::size_t src_len, std::size_t dest_offset) {
  GET_BUFFER(dstbuff, buffer);

  std::ifstream myfile(filename.c_str());
  if (!myfile.is_open()) {
    DEBUG_MESSAGE("Unable to open file " + filename, MESSAGE_TYPE::M_ERROR);
    return;
  }

  std::size_t file_size = std::filesystem::file_size(filename.c_str());
  if (src_offset >= file_size) {
    DEBUG_MESSAGE("buffer_load_partial: source offset (" + std::to_string(src_offset) + ") greater than file size (" +
                  std::to_string(file_size) + "), aborting read", MESSAGE_TYPE::M_ERROR);
    myfile.close();
    return;
  }

  if (src_offset + src_len > file_size) {
    DEBUG_MESSAGE("buffer_load_partial: file size (" + std::to_string(file_size) + ") lesser than source offset (" +
                  std::to_string(src_offset) + ") + source length (" + std::to_string(src_len) + "), truncating read",
                  MESSAGE_TYPE::M_WARNING);
    src_len = file_size - src_offset;
  }

  if (dest_offset >= dstbuff->GetSize()) {
    switch (dstbuff->type) {
      case buffer_grow:
        dstbuff->data.resize(dest_offset + src_len + 1);
        break;
      case buffer_wrap:
        dest_offset = dest_offset % dstbuff->GetSize();
        break;
      case buffer_fixed:
      case buffer_fast:
        DEBUG_MESSAGE("buffer_load_partial: destination offset (" + std::to_string(dest_offset) +
                      ") greater than fixed/fast buffer size (" + std::to_string(dstbuff->GetSize()) +
                      "), aborting write", MESSAGE_TYPE::M_ERROR);
        myfile.close();
        return;
    }
  }

  if (dest_offset + src_len > dstbuff->GetSize()) {
    switch (dstbuff->type) {
      case buffer_grow:
        dstbuff->data.resize(dest_offset + src_len + 1);
        break;
      case buffer_wrap:
        break;
      case buffer_fixed:
      case buffer_fast:
        DEBUG_MESSAGE("buffer_load_partial: destination offset (" + std::to_string(dest_offset) +
                      ") + source length (" + std::to_string(src_len) + ") greater than fixed/fast buffer size (" +
                      std::to_string(dstbuff->GetSize()) + "), aborting write", MESSAGE_TYPE::M_ERROR);
        myfile.close();
        return;
    }
  }

  for (std::size_t i = 0; i < src_offset; i++) {
    myfile.get();
  }

  std::vector<std::byte> bytes = read_from_file(myfile, src_len);

  switch (dstbuff->type) {
    case buffer_wrap: {
      for (std::size_t i = 0; i < bytes.size(); i++) {
        dstbuff->data[(i + dest_offset) % dstbuff->GetSize()] = bytes[i];
      }
      break;
    }

    case buffer_grow:
    case buffer_fixed:
    case buffer_fast: {
      std::copy(bytes.begin(), bytes.end(), dstbuff->data.begin() + dest_offset);
      break;
    }
  }

  myfile.close();
}

std::string zlib_error_string(int ret) {
  switch (ret) {
    case Z_ERRNO:
      return "Z_ERRORNO: " + std::to_string(ret);
    case Z_STREAM_ERROR:
      return "invalid compression level";
    case Z_DATA_ERROR:
      return "invalid or incomplete deflate data";
    case Z_MEM_ERROR:
      return "out of memory";
    case Z_VERSION_ERROR:
      return "zlib version mismatch";
    default:
      return "unknown error";
  }
}

buffer_t buffer_compress(buffer_t buffer, std::size_t offset, std::size_t size) {
  GET_BUFFER_R(srcbuff, buffer, -1);

  buffer_t id = make_new_buffer(0, buffer_grow, 1);
  enigma::BinaryBuffer *dstbuff = enigma::buffers.get(id).get();

  if (offset > srcbuff->GetSize()) {
    DEBUG_MESSAGE("buffer_compress: offset (" + std::to_string(offset) + ") > buffer size (" +
                  std::to_string(srcbuff->GetSize()) + "), making empty stream",
                  MESSAGE_TYPE::M_ERROR);
    offset = srcbuff->GetSize();
  }

  if (offset + size > srcbuff->GetSize()) {
    DEBUG_MESSAGE("buffer_compress:  offset (" + std::to_string(offset) + ") + size (" + std::to_string(size) +
                  ") >= buffer size (" + std::to_string(srcbuff->GetSize()) + "), truncating",
                  MESSAGE_TYPE::M_ERROR);
    size = srcbuff->GetSize() - offset;
  }

  z_stream stream;
  std::byte in[ZLIB_CHUNK_SIZE];
  std::byte out[ZLIB_CHUNK_SIZE];

  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;
  stream.avail_in = 0;
  stream.next_in = Z_NULL;

  int ret = deflateInit(&stream, Z_DEFAULT_COMPRESSION);
  if (ret != Z_OK) {
    DEBUG_MESSAGE("buffer_compress: error initializing zlib stream (" + zlib_error_string(ret) + ")",
                  MESSAGE_TYPE::M_FATAL_ERROR);
    return -1;
  }

  int flush{};
  std::size_t read = 0;
  do {
    if (read < size && (size - read) > ZLIB_CHUNK_SIZE) {
      stream.avail_in = ZLIB_CHUNK_SIZE;

      std::copy_n(srcbuff->data.begin() + offset + read, ZLIB_CHUNK_SIZE, in);

      read += ZLIB_CHUNK_SIZE;
      flush = Z_NO_FLUSH;
    } else if (read <= size) {
      std::size_t remaining = size - read;
      stream.avail_in = remaining;

      std::copy_n(srcbuff->data.begin() + offset + read, remaining, in);

      read = srcbuff->GetSize();
      flush = Z_FINISH;
    }

    stream.next_in = reinterpret_cast<unsigned char *>(in);

    do {
      stream.avail_out = ZLIB_CHUNK_SIZE;
      stream.next_out = reinterpret_cast<unsigned char *>(out);

      ret = deflate(&stream, flush);
      if (ret == Z_STREAM_ERROR) {
        DEBUG_MESSAGE("buffer_compress: zlib compression error (" + zlib_error_string(ret) + ")",
                      MESSAGE_TYPE::M_FATAL_ERROR);
      }

      std::size_t have = ZLIB_CHUNK_SIZE - stream.avail_out;
      std::size_t written = dstbuff->GetSize();

      dstbuff->Resize(dstbuff->GetSize() + have);
      std::copy_n(out, have, dstbuff->data.begin() + written);
    } while (stream.avail_out == 0);

    if (stream.avail_in != 0) {
      DEBUG_MESSAGE("buffer_compress: input not consumed fully", MESSAGE_TYPE::M_FATAL_ERROR);
    }
  } while (flush != Z_FINISH);

  if (ret != Z_STREAM_END) {
    DEBUG_MESSAGE("buffer_compress: buffer not compressed fully (" + zlib_error_string(ret) + ")",
                  MESSAGE_TYPE::M_FATAL_ERROR);
  }

  (void)deflateEnd(&stream);

  return id;
}

buffer_t buffer_decompress(buffer_t buffer) {
  GET_BUFFER_R(srcbuff, buffer, -1);

  buffer_t id = make_new_buffer(0, buffer_grow, 1);
  enigma::BinaryBuffer *dstbuff = enigma::buffers.get(id).get();

  z_stream stream;
  std::byte in[ZLIB_CHUNK_SIZE];
  std::byte out[ZLIB_CHUNK_SIZE];

  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;
  stream.avail_in = 0;
  stream.next_in = Z_NULL;

  int ret = inflateInit(&stream);
  if (ret != Z_OK) {
    DEBUG_MESSAGE("buffer_decompress: error initializing zlib stream (" + zlib_error_string(ret) + ")",
                  MESSAGE_TYPE::M_FATAL_ERROR);
    return -1;
  }

  std::size_t read = 0;
  do {
    if (read < srcbuff->GetSize() && (srcbuff->GetSize() - read) > ZLIB_CHUNK_SIZE) {
      stream.avail_in = ZLIB_CHUNK_SIZE;

      std::copy_n(srcbuff->data.begin() + read, ZLIB_CHUNK_SIZE, in);

      read += ZLIB_CHUNK_SIZE;
    } else if (read < srcbuff->GetSize()) {
      std::size_t remaining = srcbuff->GetSize() - read;
      stream.avail_in = remaining;

      std::copy_n(srcbuff->data.begin() + read, remaining, in);

      read = srcbuff->GetSize();
    }

    stream.next_in = reinterpret_cast<unsigned char *>(in);

    do {
      stream.avail_out = ZLIB_CHUNK_SIZE;
      stream.next_out = reinterpret_cast<unsigned char *>(out);

      ret = inflate(&stream, Z_NO_FLUSH);
      if (ret == Z_STREAM_ERROR) {
        DEBUG_MESSAGE("buffer_compress: zlib decompression error (" + zlib_error_string(ret) + ")",
                      MESSAGE_TYPE::M_FATAL_ERROR);
      }

      switch (ret) {
        case Z_NEED_DICT:
          ret = Z_DATA_ERROR;
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
          DEBUG_MESSAGE("buffer_decompress: zlib decompression error, aborting (" + zlib_error_string(ret) + ")",
                        MESSAGE_TYPE::M_FATAL_ERROR);
          (void)inflateEnd(&stream);
          return -1;
      }

      std::size_t have = ZLIB_CHUNK_SIZE - stream.avail_out;
      std::size_t written = dstbuff->GetSize();

      dstbuff->Resize(dstbuff->GetSize() + have);
      std::copy_n(out, have, dstbuff->data.begin() + written);
    } while (stream.avail_out == 0);
  } while (ret != Z_STREAM_END);

  (void)inflateEnd(&stream);
  dstbuff->Seek(0);

  if (ret == Z_STREAM_END) {
    return id;
  } else {
    DEBUG_MESSAGE("buffer_decompress: zlib decompression error (" + zlib_error_string(ret) + ")",
                  MESSAGE_TYPE::M_FATAL_ERROR);
    return -1;
  }
}

void buffer_fill(buffer_t buffer, std::size_t offset, buffer_data_t type, variant value, std::size_t size) {
  GET_BUFFER(binbuff, buffer);

  // NOTE: There is an incompatibility with GMS here: the game completely hangs if you try to fill a buffer which
  // doesn't have enough space for @c size bytes. To avoid this issue, I clamp @c size to the buffer end if it is
  // too large instead of emulating a hang.

  std::size_t orig_off = offset;
  while (offset % binbuff->alignment != 0 && offset < binbuff->GetSize()) {
    binbuff->WriteByte(std::byte{0});
    offset++;
  }

  if (offset + size > binbuff->GetSize()) {
    DEBUG_MESSAGE("buffer_fill: size too large; clamping to buffer end (max: " + std::to_string(binbuff->GetSize()) + ", got offset: " + std::to_string(orig_off) + " size: " + std::to_string(size) + ")", MESSAGE_TYPE::M_WARNING);
    size = binbuff->GetSize() - offset;
  }
  std::vector<std::byte> bytes = enigma_user::serialize_to_type(value, type);

  std::size_t padding = 0;
  if (bytes.size() % binbuff->alignment != 0) {
    padding = binbuff->alignment - (bytes.size() % binbuff->alignment);
  }
  std::vector<std::byte> padding_bytes{};
  padding_bytes.resize(padding, std::byte{0});

  std::size_t element_size = bytes.size() + padding;
  std::size_t times = size / element_size;
  std::size_t remainder = size % element_size;

  for (std::size_t i = 0; i < times; i++) {
    std::copy(bytes.begin(), bytes.end(),
              binbuff->data.begin() + offset + i * element_size);
    std::copy(padding_bytes.begin(), padding_bytes.end(),
              binbuff->data.begin() + offset + i * element_size + bytes.size());
  }
  if (remainder != 0 && remainder >= bytes.size()) {
    std::copy(bytes.begin(), bytes.end(),
              binbuff->data.begin() + offset + times * element_size);
    if (remainder - bytes.size() >= padding_bytes.size()) {
      std::copy(bytes.begin(), bytes.end(),
                binbuff->data.begin() + offset + times * element_size + bytes.size());
    }
  }
  binbuff->Seek(0);
}
  
void *buffer_get_address(buffer_t buffer) {
  GET_BUFFER_R(binbuff, buffer, nullptr);
  return reinterpret_cast<void *>(binbuff->data.data());
}

std::size_t buffer_get_size(buffer_t buffer) {
  GET_BUFFER_R(binbuff, buffer, -1);
  return binbuff->GetSize();
}

std::size_t buffer_get_alignment(buffer_t buffer) {
  GET_BUFFER_R(binbuff, buffer, -1);
  return binbuff->alignment;
}

buffer_type_t buffer_get_type(buffer_t buffer) {
  GET_BUFFER_R(binbuff, buffer, static_cast<buffer_type_t>(-1));
  return binbuff->type;
}

void buffer_get_surface(buffer_t buffer, int surface, std::size_t offset) {
  GET_BUFFER(binbuff, buffer);

  if (binbuff->alignment != 1) {
    DEBUG_MESSAGE("buffer_get_surface: alignment is not 1, ignoring and using 1 anyways", MESSAGE_TYPE::M_WARNING);
  }

  if (offset >= binbuff->GetSize()) {
    DEBUG_MESSAGE("buffer_get_surface: offset (" + std::to_string(offset) + ") greater than buffer size (" +
                  std::to_string(binbuff->GetSize()) + "), aborting", MESSAGE_TYPE::M_ERROR);
    return;
  }

  int texture = surface_get_texture(surface);
  std::uint32_t width = 0;
  std::uint32_t height = 0;

  std::uint8_t *pixels = enigma::graphics_copy_texture_pixels(texture, &width, &height);

  if (binbuff->GetSize() - offset < width * height * 4) {
    if (binbuff->type == buffer_grow) {
      binbuff->Resize(offset + (width * height * 4));
    } else {
      DEBUG_MESSAGE("buffer_get_surface: buffer not big enough; offset (" + std::to_string(offset) + ") + size (" +
                    std::to_string(binbuff->GetSize()) + ") < width (" + std::to_string(width) + ") * height (" +
                    std::to_string(height) + ") * 4, aborting", MESSAGE_TYPE::M_ERROR);
      delete[] pixels;
      return;
    }
  }

  std::transform(pixels, pixels + width * height * 4, binbuff->data.begin() + offset,
                 [](std::uint8_t value) { return std::byte{value}; });
}

void buffer_set_surface(buffer_t buffer, int surface, std::size_t offset) {
  GET_BUFFER(binbuff, buffer);

  if (offset >= binbuff->GetSize()) {
    DEBUG_MESSAGE("buffer_set_surface: offset (" + std::to_string(offset) + ") greater than buffer size (" +
                  std::to_string(binbuff->GetSize()) + "), aborting", MESSAGE_TYPE::M_ERROR);
    return;
  }

  int texture = surface_get_texture(surface);
  std::uint32_t width = surface_get_width(surface);
  std::uint32_t height = surface_get_height(surface);

  if (binbuff->GetSize() - offset < width * height * 4) {
    if (binbuff->type == buffer_grow) {
      binbuff->Resize(offset + (width * height * 4));
    } else {
      DEBUG_MESSAGE("buffer_set_surface: buffer not big enough; offset (" + std::to_string(offset) + ") + size (" +
                    std::to_string(binbuff->GetSize()) + ") < width (" + std::to_string(width) + ") * height (" +
                    std::to_string(height) + ") * 4, aborting", MESSAGE_TYPE::M_ERROR);
      return;
    }
  }

  enigma::graphics_push_texture_pixels(texture, width, height, reinterpret_cast<std::uint8_t *>(binbuff->data.data() + offset));
}

void buffer_resize(buffer_t buffer, std::size_t size) {
  GET_BUFFER(binbuff, buffer);
  binbuff->Resize(size);
}

void buffer_seek(buffer_t buffer, buffer_seek_t base, long long offset) {
  GET_BUFFER(binbuff, buffer);
  switch (base) {
    case buffer_seek_start:
      binbuff->Seek(offset);
      break;
    case buffer_seek_end:
      binbuff->Seek(binbuff->GetSize() - 1 + offset);
      break;
    case buffer_seek_relative:
      binbuff->Seek(binbuff->position + offset);
      break;
  }
}

std::size_t buffer_sizeof(buffer_data_t type) {
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

std::size_t buffer_tell(buffer_t buffer) {
  GET_BUFFER_R(binbuff, buffer, -1);
  return binbuff->position;
}

variant buffer_peek(buffer_t buffer, std::size_t offset, buffer_data_t type) {
  GET_BUFFER_R(binbuff, buffer, -1);
  if (type != buffer_string && type != buffer_text) {
    return deserialize_from_type(binbuff->data.begin() + offset, binbuff->data.begin() + offset + buffer_sizeof(type),
                                 type);
  } else {
    std::string data;
    for (std::size_t pos = offset; pos < binbuff->GetSize() && binbuff->data[pos] != std::byte{0};) {
      data.push_back(static_cast<char>(binbuff->data[pos]));
      if (binbuff->type == buffer_wrap) {
        pos = (pos + 1) % binbuff->GetSize();
      } else {
        pos++;
      }
    }

    return {data};
  }
}

variant buffer_read(buffer_t buffer, buffer_data_t type) {
  GET_BUFFER_R(binbuff, buffer, -1);
  while (binbuff->position % binbuff->alignment != 0) {
    if (binbuff->ReadByte() != std::byte{0}) {
      DEBUG_MESSAGE("buffer_peek: internal error: buffer not padded with zeroes, probably read something incorrect", MESSAGE_TYPE::M_FATAL_ERROR);
    }
  }

  auto result = buffer_peek(buffer, binbuff->position, type);

  if (type != buffer_string && type != buffer_text) {
    binbuff->Seek(binbuff->position + buffer_sizeof(type));
  } else {
    while (binbuff->position < binbuff->GetSize() && binbuff->data[binbuff->position] != std::byte{0}) {
      binbuff->ReadByte(); // read the string, because we do not know its length
    }
    binbuff->ReadByte(); // skip the null terminator
  }
  return result;
}

void buffer_poke(buffer_t buffer, std::size_t offset, buffer_data_t type, variant value, bool resize) {
  GET_BUFFER(binbuff, buffer);

  // NOTE: there is a GMS incompatibility here; in GMS if the data cannot fit within the current size of the buffer, the
  // write is simply aborted. Here, if the buffer is not large enough, it can be resized if its type is @c buffer_grow and
  // the value @c true is passed for the @c resize parameter.

  std::vector<std::byte> data = serialize_to_type(value, type);

  if ((data.size() + offset) > binbuff->GetSize() && ((!resize && binbuff->type == buffer_grow) ||
      binbuff->type == buffer_fixed || binbuff->type == buffer_fast)) {
    DEBUG_MESSAGE("buffer_poke: Write would go off end of buffer, aborting", MESSAGE_TYPE::M_ERROR);
    return;
  }

  write_to_buffer(binbuff, data, offset);
}

void buffer_write(buffer_t buffer, buffer_data_t type, variant value) {
  GET_BUFFER(binbuff, buffer);
  while (binbuff->position % binbuff->alignment != 0) {
    binbuff->WriteByte(std::byte{0});
  }

  std::vector<std::byte> data = serialize_to_type(value, type);
  write_to_buffer(binbuff, data, binbuff->position);

  if (type != buffer_string && type != buffer_text) {
    binbuff->Seek(binbuff->position + buffer_sizeof(type));
  } else {
    while (binbuff->position < binbuff->GetSize() && binbuff->data[binbuff->position] != std::byte{0}) {
      binbuff->ReadByte();  // read the string, because we do not know its length
    }
    binbuff->ReadByte();  // skip the null terminator
  }
}

string buffer_md5(buffer_t buffer, std::size_t offset, std::size_t size) {
  GET_BUFFER_R(binbuff, buffer, internal_md5(""));

  if (offset > binbuff->GetSize()) {
    DEBUG_MESSAGE("buffer_md5: offset (" + std::to_string(offset) + ") > buffer size (" +
                      std::to_string(binbuff->GetSize()) + "), making empty stream",
                  MESSAGE_TYPE::M_ERROR);
    offset = binbuff->GetSize();
  }

  if (offset + size > binbuff->GetSize()) {
    DEBUG_MESSAGE("buffer_md5:  offset (" + std::to_string(offset) + ") + size (" + std::to_string(size) +
                      ") >= buffer size (" + std::to_string(binbuff->GetSize()) + "), truncating",
                  MESSAGE_TYPE::M_ERROR);
    size = binbuff->GetSize() - offset;
  }

  return internal_md5(binbuff->data.begin() + offset, size);
}

string buffer_sha1(buffer_t buffer, std::size_t offset, std::size_t size) {
  GET_BUFFER_R(binbuff, buffer, internal_sha1(""));

  if (offset > binbuff->GetSize()) {
    DEBUG_MESSAGE("buffer_sha1: offset (" + std::to_string(offset) + ") > buffer size (" +
                      std::to_string(binbuff->GetSize()) + "), making empty stream",
                  MESSAGE_TYPE::M_ERROR);
    offset = binbuff->GetSize();
  }

  if (offset + size > binbuff->GetSize()) {
    DEBUG_MESSAGE("buffer_sha1:  offset (" + std::to_string(offset) + ") + size (" + std::to_string(size) +
                      ") >= buffer size (" + std::to_string(binbuff->GetSize()) + "), truncating",
                  MESSAGE_TYPE::M_ERROR);
    size = binbuff->GetSize() - offset;
  }

  return internal_sha1(binbuff->data.begin() + offset, size);
}

variant buffer_crc32(buffer_t buffer, std::size_t offset, std::size_t size) {
  GET_BUFFER_R(binbuff, buffer, -1);

  // NOTE: There is an incompatibility with GMS here: because I do not know what
  // generator polynomial they are using, I have no way of (at least trying to)
  // exactly replicating how it works in GMS.
  //
  // I have approximated it as:
  // - if offset greater than buffer size, only wrap for buffer_wrap, otherwise
  //   abort with 0
  // - if offset + size greater than buffer size, truncate data being hashed

  if (offset > binbuff->GetSize()) {
    if (binbuff->type == buffer_wrap) {
      offset = offset % binbuff->GetSize();
    } else {
      DEBUG_MESSAGE("buffer_crc32: offset (" + std::to_string(offset) + ") > buffer size (" +
                    std::to_string(binbuff->GetSize()) + "), aborting",
                    MESSAGE_TYPE::M_ERROR);
      return crc32(0, nullptr, 0);
    }
  }

  if (offset + size >= binbuff->GetSize()) {
    DEBUG_MESSAGE("buffer_crc32:  offset (" + std::to_string(offset) + ") + size (" + std::to_string(size) +
                  ") >= buffer size (" + std::to_string(binbuff->GetSize()) + "), truncating",
                  MESSAGE_TYPE::M_ERROR);
    size = binbuff->GetSize() - offset;
  }

  return crc32(0, reinterpret_cast<const unsigned char *>(binbuff->data.data() + offset), size);
}

buffer_t buffer_base64_decode(string str) {
  buffer_t id = make_new_buffer(0, buffer_grow, 1);
  enigma::BinaryBuffer *buffer = enigma::buffers.get(id).get();

  std::vector<std::byte> decoded = internal_buffer_base64_decode(str);
  buffer->data.resize(decoded.size() + 1);
  std::copy(decoded.begin(), decoded.end(), buffer->data.begin());

  return id;
}

void buffer_base64_decode_ext(buffer_t buffer, string str, std::size_t offset) {
  GET_BUFFER(binbuff, buffer);

  std::vector<std::byte> decoded = internal_buffer_base64_decode(str);

  switch(binbuff->type) {
    case buffer_grow: {
      if (offset + decoded.size() >= binbuff->GetSize()) {
        binbuff->Resize(offset + decoded.size() + 1);
      }
      std::copy(decoded.begin(), decoded.end(), binbuff->data.begin() + offset);
      break;
    }
    case buffer_wrap: {
      binbuff->Seek(offset);
      for (auto &byte : decoded) {
        binbuff->WriteByte(byte);
      }
      break;
    }
    case buffer_fixed:
    case buffer_fast: {
      if (offset >= binbuff->GetSize()) {
        DEBUG_MESSAGE("buffer_base64_decode_ext: offset greater than fixed/fast buffer size, aborting write",
                      MESSAGE_TYPE::M_ERROR);
        return;
      }
      std::size_t written = decoded.size();
      if (offset + decoded.size() >= binbuff->GetSize()) {
        DEBUG_MESSAGE("buffer_base64_decode_ext: offset (" + std::to_string(offset) + ") + decoded string size (" +
                      std::to_string(decoded.size()) + ") greater than fixed/fast buffer size (" +
                      std::to_string(binbuff->GetSize()) + "), truncating data",
                      MESSAGE_TYPE::M_WARNING);
        written = binbuff->GetSize() - offset;
      }

      std::copy(decoded.begin(), decoded.begin() + written, binbuff->data.begin() + offset);
      break;
    }
  }
}

std::string buffer_base64_encode(buffer_t buffer, std::size_t offset, std::size_t size) {
  GET_BUFFER_R(binbuff, buffer, "");

  if (offset >= binbuff->GetSize()) {
    DEBUG_MESSAGE("buffer_base64_encode: offset beyond end of buffer, aborting", MESSAGE_TYPE::M_ERROR);
    return base64_encode("");
  }

  if (offset + size > binbuff->GetSize()) {
    DEBUG_MESSAGE("buffer_base64_encode: given offset (" + std::to_string(offset) + ") + size (" + std::to_string(size)
                  + ") greater than buffer size (" + std::to_string(binbuff->GetSize()) + "), truncating encode",
                  MESSAGE_TYPE::M_WARNING);
    size = std::min(binbuff->GetSize() - offset, size);
  }

  return internal_buffer_base64_encode(binbuff->data.begin() + offset, size);
}

namespace {
std::array<std::uint8_t, SHA1HashSize> internal_sha1_checksum(std::vector<std::byte>::iterator first, std::vector<std::byte>::iterator last) {
  SHA1Context ctx;
  int err = SHA1Reset(&ctx);
  if (err != 0) {
    DEBUG_MESSAGE("calculate_checksum: sha1 error (" + std::to_string(err) + ")", MESSAGE_TYPE::M_FATAL_ERROR);
    return {};
  }

  std::array<std::uint8_t, SHA1HashSize> digest{};
  err = SHA1Input(&ctx, reinterpret_cast<std::uint8_t *>(&*first), std::distance(first, last));
  if (err != 0) {
    DEBUG_MESSAGE("calculate_checksum: sha1 error (" + std::to_string(err) + ")", MESSAGE_TYPE::M_FATAL_ERROR);
    return {};
  }

  err = SHA1Result(&ctx, digest.data());
  if (err != 0) {
    DEBUG_MESSAGE("calculate_checksum: sha1 error (" + std::to_string(err) + ")", MESSAGE_TYPE::M_FATAL_ERROR);
    return {};
  }

  return digest;
}

std::array<std::uint8_t, SHA1HashSize> calculate_checksum(std::vector<std::byte> &buffer) {
  return internal_sha1_checksum(buffer.begin(), buffer.end());
}

void store_checksum(std::vector<std::byte> &buffer, std::array<uint8_t, SHA1HashSize> digest) {
  std::transform(digest.begin(), digest.end(), std::back_inserter(buffer),
                 [](std::uint8_t v) { return std::byte{v}; });
  buffer.shrink_to_fit();
}

bool verify_checksum(std::vector<std::byte> &buffer) {
  if (buffer.size() < SHA1HashSize) {
    DEBUG_MESSAGE("verify_checksum: buffer size too small to contain checksum", MESSAGE_TYPE::M_FATAL_ERROR);
    return false;
  }

  std::array<std::uint8_t, SHA1HashSize> expected = internal_sha1_checksum(buffer.begin(), buffer.end() - SHA1HashSize);

  std::array<std::uint8_t, SHA1HashSize> digest{};
  std::transform(buffer.end() - SHA1HashSize, buffer.end(), digest.begin(),
                 [](std::byte b) { return static_cast<std::uint8_t>(b); });

  for (std::size_t i = 0; i < SHA1HashSize; i++) {
    if (expected[i] != digest[i]) {
      return false;
    }
  }
  return true;
}
}

void game_save_buffer(buffer_t buffer, enum SerializationBackend backend ) {
  GET_BUFFER(binbuff, buffer)
  std::size_t ptr = 0;

  binbuff->data.resize(sizeof(std::size_t));
  enigma::bytes_serialization::internal_serialize_into<std::size_t>(&binbuff->data[ptr], enigma::instance_list.size());
  for (auto &[id, obj] : enigma::instance_list) {
    auto buf = obj->inst->serialize();
    ptr = binbuff->data.size();
    binbuff->data.resize(binbuff->data.size() + buf.size());
    std::move(buf.begin(), buf.end(), &binbuff->data[ptr]);
  }

  ptr = binbuff->data.size();
  binbuff->data.resize(binbuff->data.size() + sizeof(std::size_t));
  enigma::bytes_serialization::internal_serialize_into<std::size_t>(&binbuff->data[ptr],
                                      enigma::instance_deactivated_list.size());
  for (auto &[id, obj] : enigma::instance_deactivated_list) {
    auto buf = obj->serialize();
    ptr = binbuff->data.size();
    binbuff->data.resize(binbuff->data.size() + buf.size());
    std::move(buf.begin(), buf.end(), &binbuff->data[ptr]);
  }

  ptr = binbuff->data.size();
  binbuff->data.resize(binbuff->data.size() + 1);
  binbuff->data[ptr] = std::byte{0xee};

  ptr = binbuff->data.size();
  binbuff->data.resize(binbuff->data.size() + enigma::backgrounds.byte_size());
  auto buf = enigma::backgrounds.serialize();
  std::copy(buf.begin(), buf.end(), &binbuff->data[ptr]);

  ptr = binbuff->data.size();
  binbuff->data.resize(binbuff->data.size() + 1);
  binbuff->data[ptr] = std::byte{0xef};

  ptr = binbuff->data.size();
  binbuff->data.resize(binbuff->data.size() + sizeof(int));
  enigma::bytes_serialization::internal_serialize_into(&binbuff->data[ptr], static_cast<int>(enigma_user::room.rval.d));

  store_checksum(binbuff->data, calculate_checksum(binbuff->data));
}

void game_load_buffer(buffer_t buffer) {
  GET_BUFFER(binbuff, buffer)
  if (!verify_checksum(binbuff->data)) {
    DEBUG_MESSAGE("game_load_buffer: Checksum is not correct, aborting", MESSAGE_TYPE::M_FATAL_ERROR);
    return;
  }

  std::vector<int> active_ids{};
  std::transform(enigma::instance_list.begin(), enigma::instance_list.end(), std::back_inserter(active_ids),
                 [](auto &value) { return value.first; });
  for (int id : active_ids) {
    instance_destroy(id);
  }
  std::vector<int> inactive_ids{};
  std::transform(enigma::instance_deactivated_list.begin(), enigma::instance_deactivated_list.end(), std::back_inserter(inactive_ids),
                 [](auto &value) { return value.first; });
  for (int id : inactive_ids) {
    instance_destroy(id);
  }

  std::byte *ptr = &binbuff->data[0];
  std::size_t active_size = enigma::bytes_serialization::internal_deserialize<std::size_t>(ptr);
  ptr += sizeof(std::size_t);
  for (std::size_t i = 0; i < active_size; i++) {
    // This should be an object, where `id` is serialized first and `object_id` second
    auto obj_id = enigma::bytes_serialization::internal_deserialize<int>(ptr + 1);
    auto obj_ind = enigma::bytes_serialization::internal_deserialize<int>(ptr + 1 + sizeof(unsigned int));
    auto obj = enigma::instance_create_id(0, 0, obj_ind, obj_id);
    ptr += obj->deserialize_self(ptr);
    obj->activate();
  }

  std::size_t inactive_size = enigma::bytes_serialization::internal_deserialize<std::size_t>(ptr);
  ptr += sizeof(std::size_t);
  for (std::size_t i = 0; i < inactive_size; i++) {
    auto obj_ind = enigma::bytes_serialization::internal_deserialize<int>(ptr + sizeof(unsigned int));
    auto obj = enigma::instance_create_id(0, 0, obj_ind, -1);
    ptr += obj->deserialize_self(ptr);
    obj->deactivate();
  }

  auto type = enigma::bytes_serialization::internal_deserialize<unsigned char>(ptr);
  if (type != 0xee) {
    DEBUG_MESSAGE("Invalid enigma::backgrounds header", MESSAGE_TYPE::M_FATAL_ERROR);
  }
  ptr += 1;

  ptr += enigma::backgrounds.deserialize_self(ptr);

  type = enigma::bytes_serialization::internal_deserialize<unsigned char>(ptr);
  if (type != 0xef) {
    DEBUG_MESSAGE("Invalid enigma::backgrounds footer", MESSAGE_TYPE::M_FATAL_ERROR);
  }
  ptr += 1;

  auto room_index = enigma::bytes_serialization::internal_deserialize<int>(ptr);
  enigma_user::room_goto(room_index);
  ptr += sizeof(int);
}

}  // namespace enigma_user
