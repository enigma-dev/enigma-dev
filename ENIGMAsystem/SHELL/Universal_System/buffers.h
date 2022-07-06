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

#ifndef ENIGMA_BUFFERS_H
#define ENIGMA_BUFFERS_H

#include "buffers_data.h"
#include "var4.h"

#include <string>

namespace enigma_user {
std::vector<std::byte> serialize_to_type(variant &value, buffer_data_t type);
variant deserialize_from_type(std::vector<std::byte>::iterator first, std::vector<std::byte>::iterator last, buffer_data_t type);

buffer_t buffer_create(std::size_t size, buffer_type_t type, std::size_t alignment);
bool buffer_exists(buffer_t buffer);
void buffer_delete(buffer_t buffer);
void buffer_copy(buffer_t src_buffer, std::size_t src_offset, std::size_t size, buffer_t dest_buffer, std::size_t dest_offset);
void buffer_save(buffer_t buffer, std::string filename);
void buffer_save_ext(buffer_t buffer, std::string filename, std::size_t offset, std::size_t size);
buffer_t buffer_load(std::string filename);
void buffer_load_ext(buffer_t buffer, std::string filename, std::size_t offset);

buffer_t buffer_compress(buffer_t buffer, std::size_t offset, std::size_t size);
buffer_t buffer_decompress(buffer_t buffer);

buffer_t buffer_base64_decode(std::string str);
void buffer_base64_decode_ext(buffer_t buffer, std::string str, std::size_t offset);
std::string buffer_base64_encode(buffer_t buffer, std::size_t offset, std::size_t size);
std::string buffer_md5(buffer_t buffer, std::size_t offset, std::size_t size);
std::string buffer_sha1(buffer_t buffer, std::size_t offset, std::size_t size);
variant buffer_crc32(buffer_t buffer, std::size_t offset, std::size_t size);

void *buffer_get_address(buffer_t buffer);
std::size_t buffer_get_size(buffer_t buffer);
std::size_t buffer_get_alignment(buffer_t buffer);
buffer_type_t buffer_get_type(buffer_t buffer);
void buffer_get_surface(buffer_t buffer, int surface, std::size_t offset = 0);
void buffer_set_surface(buffer_t buffer, int surface, std::size_t offset = 0);
void buffer_resize(buffer_t buffer, std::size_t size);
void buffer_seek(buffer_t buffer, buffer_seek_t base, long long offset);
std::size_t buffer_sizeof(buffer_data_t type);
std::size_t buffer_tell(buffer_t buffer);

variant buffer_peek(buffer_t buffer, std::size_t offset, buffer_data_t type);
variant buffer_read(buffer_t buffer, buffer_data_t type);
void buffer_fill(buffer_t buffer, std::size_t offset, buffer_data_t type, variant value, std::size_t size);
void buffer_poke(buffer_t buffer, std::size_t offset, buffer_data_t type, variant value, bool resize = false);
void buffer_write(buffer_t buffer, buffer_data_t type, variant value);

void game_save_buffer(buffer_t buffer);
void game_load_buffer(buffer_t buffer);

}  //namespace enigma_user

#endif  //ENIGMA_BUFFERS_H
