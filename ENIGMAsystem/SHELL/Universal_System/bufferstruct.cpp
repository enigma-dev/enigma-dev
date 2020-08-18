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
#include "Widget_Systems/widgets_mandatory.h"

#include <cstring>
#include <fstream>
#include <iostream>

using std::string;

namespace enigma {
std::vector<BinaryBuffer*> buffers(0);

BinaryBuffer::BinaryBuffer(unsigned size) {
  data.resize(size, 0);
  position = 0;
  alignment = 1;
  type = 0;
}

unsigned BinaryBuffer::GetSize() { return data.size(); }

void BinaryBuffer::Resize(unsigned size) { data.resize(size, 0); }

void BinaryBuffer::Seek(unsigned offset) {
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
        position = GetSize() - 1;
        return;
    }
  }
}

unsigned char BinaryBuffer::ReadByte() {
  Seek(position);
  unsigned char byte = data[position];
  Seek(position + 1);
  return byte;
}

void BinaryBuffer::WriteByte(unsigned char byte) {
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

std::vector<unsigned char> valToBytes(variant value, unsigned count) {
  std::vector<unsigned char> result(0);
  for (unsigned i = 0; i < count; i++) {
    result.push_back(value >> ((i)*8));
  }
  return result;
}
}  // namespace enigma

namespace enigma_user {

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
  return (buffer >= 0 && buffer < enigma::buffers.size() && enigma::buffers[buffer] != nullptr);
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
  std::vector<char> data;
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
  unsigned pos = offset;
  for (unsigned i = 0; i < buffer_sizeof(type); i++) {
    binbuff[pos] = value[i];
    if (i > binbuff->GetSize()) {
      if (binbuff->type != buffer_wrap) {
        break;
      }
      pos = 0;
    }
  }
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

//NOTE: This function should most likely be added in graphics systems.
void buffer_get_surface(int buffer, int surface, int mode, unsigned offset, int modulo) {
  //get_buffer(binbuff, buffer);
  //TODO: Write this function
}

void buffer_resize(int buffer, unsigned size) {
  get_buffer(binbuff, buffer);
  binbuff->Resize(size);
}

void buffer_seek(int buffer, int base, unsigned offset) {
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
    case buffer_u8:
      return 1;
    case buffer_s8:
      return 1;
    case buffer_u16:
      return 2;
    case buffer_s16:
      return 2;
    case buffer_u32:
      return 4;
    case buffer_s32:
      return 4;
    case buffer_f16:
      return 2;
    case buffer_f32:
      return 4;
    case buffer_f64:
      return 8;
    case buffer_bool:
      return 1;
    case buffer_string:
      return 0;
    default:
      return 0;
  }
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
    long res = 0;
    for (unsigned i = 0; i < buffer_sizeof(type); i++) {
      res += binbuff->ReadByte() << i * 8;
    }
    return res;
  } else {
    char byte = '1';
    std::vector<char> data;
    while (byte != 0x00) {
      byte = binbuff->ReadByte();
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
    std::vector<unsigned char> data = enigma::valToBytes(value, buffer_sizeof(type));
    for (unsigned i = 0; i < data.size(); i++) {
      binbuff->WriteByte(data[i]);
    }
  } else {
    char byte = '1';
    unsigned pos = 0;
    while (byte != 0x00) {
      byte = value[pos];
      pos += 1;
      binbuff->WriteByte(byte);
    }
    if (binbuff->alignment > pos) {
      for (unsigned i = 0; i < binbuff->alignment - pos; i++) {
        binbuff->WriteByte(0);
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
