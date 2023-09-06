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

#ifndef ENIGMA_BUFFERS_H
#define ENIGMA_BUFFERS_H

#include "buffers_data.h"
#include "var4.h"

#include <string>

namespace enigma_user {
/**
 * @brief Serialize a value of a given type as a <tt> std::vector<std::byte> </tt>
 *
 * @par
 * This function makes sure that the @c variant that it is given contains the required type convertible to @c type
 * otherwise causes a fatal error. Currently, serialization for 16-bit floating point numbers is unimplemented.
 *
 * @par
 * Multi-byte data like integers is serialized in big-endian form, where the MSB of the integer comes at the start of
 * the stream, and the LSB comes last. This appears to be in contrast to GMS, which seems to use little endian
 * serialization.
 *
 * @note This is not a user-facing function, however it will be required @c game_save_buffer so I have exported it here
 *
 * @param value The value to be serialized
 * @param type The type to be serialized to
 * @return std::vector<std::byte> The bytes representing the serialized form of the argument
 */
std::vector<std::byte> serialize_to_type(variant &value, buffer_data_t type);

/**
 * @brief Deserialize a value from a given <tt> std::vector<std::byte> </tt>
 *
 * This function makes sure that the pair of iterators given to it are large enough to contain the given type, otherwise
 * it causes a fatal error. Currently, deserialization for 16-bit floating point numbers is unimplemented.
 *
 * @note This is not a user-facing function, however it will be required @c game_load_buffer so I have exported it here
 *
 * @param first The iterator pointing to the first byte of the serialized value
 * @param last The iterator pointing to the last byte of the serialized value
 * @param type The type to be deserialized to
 * @return variant The deserialized form of the bytes contained between the iterators represented as @c type
 */
variant deserialize_from_type(std::vector<std::byte>::iterator first, std::vector<std::byte>::iterator last, buffer_data_t type);

/**
 * @brief Creates a new buffer
 *
 * @par
 * This function automatically reuses handles of buffers which were previously deleted. It is thus not possible to use
 * a buffer handle after the buffer it referenced is deleted as it can be reused again by this function to create some other
 * buffer. This takes O(n) time, proportional to the number of buffers having unique handles.
 *
 * @par
 * If no empty slots are available to create a buffer, it pushes the new buffer onto the end of the buffer list and
 * returns a new buffer handle.
 *
 * @param size The size (in bytes) of the buffer to be created
 * @param type The method used to deal with the bounds of the buffer: wrap, grow, fixed
 * @param alignment The alignment (in bytes) of the data contained in the buffer
 * @return The handle to the newly created buffer
 */
buffer_t buffer_create(std::size_t size, buffer_type_t type, std::size_t alignment);

/**
 * @brief Check if a buffer exists
 *
 * Simply checks if the slot being referenced is within the limits of the buffer list, and if so, checks if the value at
 * that slot is not a null pointer, i.e. an active buffer.
 *
 * @param buffer The handle to the buffer to be checked
 * @return true If the buffer being checked exists
 * @return false If the buffer being checked does not exist
 */
bool buffer_exists(buffer_t buffer);

/**
 * @brief Deletes a buffer
 *
 * This function frees the memory allocated for a buffer, and clears the slot which its handle referenced. A buffer
 * cannot be used after the slot that pointed to it has been cleared. This takes O(1) time.
 *
 * @param buffer The handle to the buffer being deleted
 */
void buffer_delete(buffer_t buffer);

/**
 * @brief Copies the contents of one buffer to another
 *
 * The offsets can be treated differently depending on the types of the buffers being used:\n
 * - @c buffer_wrap: If the offsets of either or both of the source and destination buffers go past the end, wrap them
 *                    around to the beginning.
 * - @c buffer_grow: If either of the offsets go beyond the end of either of the buffers, abort the copy
 * - @c buffer_fixed: If either of the offsets go beyond the end of either of the buffers, abort the copy
 * - @c buffer_fast: If either of the offsets go beyond the end of either of the buffers, abort the copy
 *
 * If the buffer being read from is of the @c buffer_wrap type, @c size is not truncated. Otherwise, the number of bytes
 * copied is at max the number of bytes between the offset into the source buffer and its end.
 *
 * If there is not enough place in a @c buffer_grow destination buffer to store the copied bytes, it is resized to have
 * as many bytes as required + 1 additional byte which is always reserved at the end.
 *
 * @param src_buffer The handle to the buffer from which data has to be copied
 * @param src_offset The offset into the source buffer where reading starts from
 * @param size The number of bytes which are to be copied from the source buffer into the destination buffer
 * @param dest_buffer The handle to the buffer into which the copied data has to be written
 * @param dest_offset The offset into the destination buffer where writing begins from
 */
void buffer_copy(buffer_t src_buffer, std::size_t src_offset, std::size_t size, buffer_t dest_buffer, std::size_t dest_offset);

/**
 * @brief Write a buffer's bytes to a file
 *
 * This function writes the contents of a buffer to a file. It truncates the file on writing, i.e. it clears the previous
 * contents of the file.
 *
 * @see buffer_save_ext
 *
 * @param buffer The handle to the buffer being written
 * @param filename The file to write to
 */
void buffer_save(buffer_t buffer, std::string filename);

/**
 * @brief Write a buffer's bytes to a file from a given offset
 *
 * @par
 * This function works in the same way as @c buffer_copy with two extra arguments: an offset into the buffer and a size
 * argument for the number of bytes being written. If the offset goes past the end of the buffer, it is wrapped around
 * in the case of @c buffer_wrap and is an error otherwise.
 *
 * @par
 * If the bytes being written go off of the end of the buffer, the bytes being written are truncated to the number of bytes
 * from the offset to the end of the buffer for all four types of buffers.
 *
 * @see buffer_copy
 *
 * @param buffer The handle to the buffer being written
 * @param filename The file to write to
 * @param offset The offset into the buffer being written
 * @param size The number of bytes to write to the file
 */
void buffer_save_ext(buffer_t buffer, std::string filename, std::size_t offset, std::size_t size);

/**
 * @brief Load a buffer from a given file
 *
 * This function creates a new buffer whose initial size is the size in bytes of the file being read, with the type being
 * @c buffer_grow and an alignment of 1. The bytes of the file are then copied into the buffer, and the handle to the buffer
 * is returned.
 *
 * @see buffer_load_ext
 * @see buffer_load_partial
 *
 * @param filename The file to load the buffer from
 * @return The handle to the buffer containing the contents of the file
 */
buffer_t buffer_load(std::string filename);

/**
 * @brief Load a file into a given buffer at @c offset
 *
 * This function loads a file into a buffer at a given offset. Depending on the type of the buffer, there are four
 * possible cases:
 * - @c buffer_wrap: If the offset goes past the end of the buffer, it is wrapped around to the beginning.
 * - @c buffer_grow: If there is not enough space in the buffer or the offset goes past the end of the buffer, the
 *                   buffer is resized to the size required.
 * - @c buffer_fixed: If there is not enough space in the buffer, the write is aborted.
 * - @c buffer_fast: If there is not enough space in the buffer, the write is aborted.
 *
 * @note Unlike some other functions, in the case of @c buffer_fixed and @c buffer_fast, the write is not truncated when
 *       there is not enough space, it is downright aborted.
 *
 * @see buffer_load
 * @see buffer_load_partial
 *
 * @param buffer The handle to the buffer being written to
 * @param filename The file to read the contents of
 * @param offset The offset into the buffer where the write will begin from
 */
void buffer_load_ext(buffer_t buffer, std::string filename, std::size_t offset);

/**
 * @brief Load a part of a file into a buffer at a given offset
 *
 * @par
 * This function copies a part of a file starting from an offset into a buffer starting from an offset. If the offset
 * into the file is beyond its end, it is an error and the read is aborted. If the bytes being read would go off of the
 * end of the file, the read is truncated to the end of the file.
 *
 * @par
 * Depending on the type of the buffer, there are four possible cases:
 * - @c buffer_wrap: If the offset goes past the end of the buffer, it is wrapped around to the beginning.
 * - @c buffer_grow: If there is not enough space in the buffer or the offset goes past the end of the buffer, the
 *                   buffer is resized to the size required.
 * - @c buffer_fixed: If there is not enough space in the buffer, the write is aborted.
 * - @c buffer_fast: If there is not enough space in the buffer, the write is aborted.
 *
 * @see buffer_load
 * @see buffer_load_ext
 *
 * @param buffer The handle to the buffer being written to
 * @param filename The file to read the contents of
 * @param src_offset The offset into the file from where reading begins
 * @param src_len The number of bytes to read from the file
 * @param dest_offset The offset into the buffer where the write will being from
 */
void buffer_load_partial(buffer_t buffer, std::string filename, std::size_t src_offset, std::size_t src_len, std::size_t dest_offset);

/**
 * @brief Compress the contents of a buffer using the zlib library
 *
 * This function uses the standard @c Z_DEFAULT_COMPRESSION compression strategy. In all cases, if the offset goes beyond
 * the buffer end, the compression is aborted and a buffer containing the compression data of an empty buffer is returned.
 * If the bytes being compressed would go off of the buffer end, the compression is truncated to the end of the buffer.
 * The buffer is compressed in chunks of 16 kilobytes of data, and a new buffer is created to store the compressed data.
 *
 * @see buffer_decompress
 *
 * @param buffer The handle to the buffer being compressed
 * @param offset The offset into the buffer to start compressing from
 * @param size The number of bytes to be compressed
 * @return The handle to the buffer containing the compressed data
 */
buffer_t buffer_compress(buffer_t buffer, std::size_t offset, std::size_t size);

/**
 * @brief Decompress the contents of a buffer using the zlib library
 *
 * This function decompresses the data stored in a buffer 16 kilobytes at a time, and a new buffer is created to store
 * the decompressed data.
 *
 * @see buffer_compress
 *
 * @param buffer The handle to the buffer being decompressed
 * @return The handle to the buffer containing the decompressed data
 */
buffer_t buffer_decompress(buffer_t buffer);

/**
 * @brief Decode a buffer given a base64 encoded form
 *
 * This function creates a new buffer to store the bytes decoded from a base64 encoded string and returns a handle to it.
 *
 * @see buffer_base64_decode_ext
 * @see buffer_base64_encode
 *
 * @param str The string containing the base64 encoded data
 * @return The handle to the buffer containing the decoded data
 */
buffer_t buffer_base64_decode(std::string str);

/**
 * @brief Decode a base64 encoded string into a buffer at a given offset
 *
 * This function copies the bytes decoded from a base64 encoded string into a buffer at a given offset. Based on the buffer
 * type, four cases are possible:
 * - @c buffer_wrap: If the offset goes beyond the end of the buffer, it is wrapped around to the beginning.
 * - @c buffer_grow: If there is not enough space in the buffer to store the decoded data, the buffer is resized to be
 *                   big enough
 * - @c buffer_fixed: If the offset is beyond the end of the buffer, the write is aborted
 * - @c buffer_fast: If the offset is beyond the end of the buffer, the write is aborted
 *
 * @see buffer_base64_decode
 * @see buffer_base64_encode
 *
 * @param buffer The handle to the buffer being written to
 * @param str The string containing the base64 encoded data
 * @param offset The offset into the buffer where the write will begin from
 */
void buffer_base64_decode_ext(buffer_t buffer, std::string str, std::size_t offset);

/**
 * @brief Base64 encode the contents of a buffer from an offset
 *
 * This function encodes the data contained within a buffer in the base64 format starting at a given offset. If the offset
 * is beyond the end of the buffer, for all types of buffer, the encode is aborted. If the bytes being encoded would go
 * off of the end of the buffer, the number of bytes encoded are truncated to the end of the buffer.
 *
 * @see buffer_base64_decode
 * @see buffer_base64_decode_ext
 *
 * @param buffer The handle to the buffer whose data is being encoded
 * @param offset The offset into the buffer where encoding will start from
 * @param size The number of bytes to be encoded
 * @return The string containing the base64 encoded data
 */
std::string buffer_base64_encode(buffer_t buffer, std::size_t offset, std::size_t size);

/**
 * @brief Calculate the md5 checksum of a buffer's contents starting at an offset
 *
 * This function calculates the md5 checksum of the subset of the contents of the buffer starting at @c offset and @c size
 * bytes in size. If the offset given is beyond the end of the buffer, it returns the checksum of an empty buffer. If the
 * bytes being considered go beyond the end of the buffer, the number of bytes read is truncated to the end of the buffer.
 *
 * @param buffer The handle to the buffer whose checksum is to be calculated
 * @param offset The offset into the buffer where the first byte to be read is located
 * @param size The number of bytes to be used in the checksum calculation
 * @return The string containing the md5 checksum of the buffer
 */
std::string buffer_md5(buffer_t buffer, std::size_t offset, std::size_t size);

/**
 * @brief Calculate the sha1 checksum of a buffer's contents starting at an offset
 *
 * This function calculates the sha1 checksum of the subset of the contents of the buffer starting at @c offset and @c size
 * bytes in size. If the offset given is beyond the end of the buffer, it returns the checksum of an empty buffer. If the
 * bytes being considered go beyond the end of the buffer, the number of bytes read is truncated to the end of the buffer.
 *
 * @param buffer The handle to the buffer whose checksum is to be calculated
 * @param offset The offset into the buffer where the first byte to be read is located
 * @param size The number of bytes to be used in the checksum calculation
 * @return The string containing the sha1 checksum of the buffer
 */
std::string buffer_sha1(buffer_t buffer, std::size_t offset, std::size_t size);

/**
 * @brief Calculate the crc32 checksum of a buffer's contents starting at an offset
 *
 * This function calculates the sha1 checksum of the subset of the contents of the buffer starting at @c offset and @c size
 * bytes in size. If the offset goes beyond the end of the buffer, two cases are possible:
 * - @c buffer_wrap: The offset is wrapped around to the beginning of the buffer
 * - otherwise: the checksum calculation is aborted
 *
 * If the bytes being considered go beyond the end of the buffer, the number of bytes read is truncated to the end of the buffer.
 * This function uses the @c crc32() function from the zlib library.
 *
 * @note There is an incompatibility with GMS here: because I do not know what
 * generator polynomial they are using, I have no way of (at least trying to)
 * exactly replicating how it works in GMS.
 *
 * @param buffer The handle to the buffer whose checksum is to be calculated
 * @param offset The offset into the buffer where the first byte to be read is located
 * @param size The number of bytes to be used in the checksum calculation
 * @return An integer containing the crc32 checksum of the buffer
 */
variant buffer_crc32(buffer_t buffer, std::size_t offset, std::size_t size);


/**
 * @brief Get the address of a buffer
 * @param buffer The handle to the buffer
 * @return The address of the first byte of the buffer
 */
void *buffer_get_address(buffer_t buffer);

/**
 * @brief Get the size of a buffer
 * @param buffer The handle to the buffer
 * @return The number of bytes stored in the buffer
 */
std::size_t buffer_get_size(buffer_t buffer);

/**
 * @brief Get the alignment of a buffer
 * @param buffer The handle to the buffer
 * @return The alignment (in bytes) of the buffer
 */
std::size_t buffer_get_alignment(buffer_t buffer);

/**
 * @brief Get the type of the buffer
 * @param buffer The handle to the buffer
 * @return One of @c buffer_grow, @c buffer_wrap, @c buffer_fixed and @c buffer_fast
 */
buffer_type_t buffer_get_type(buffer_t buffer);

/**
 * @brief Copy the pixels from a surface to a buffer
 *
 * This function copies the pixels stored in the texture of a surface to a buffer in BGRA format. If the alignment of the
 * buffer is not 1, the alignment is ignored and a warning is issued. If the offset goes beyond the end of the buffer, the
 * write is aborted. If the buffer is not big enough to store the surface's pixels, the write is aborted and an error is
 * issued.
 *
 * @note Each pixel requires 4 bytes; 1 byte for each component: B (blue), G (green), R (red) and A (alpha/transparency).
 *
 * @see buffer_set_surface
 *
 * @param buffer The handle to the buffer to write to
 * @param surface The surface whose texture will be copied
 * @param offset The offset into the buffer where writing starts from
 */
void buffer_get_surface(buffer_t buffer, int surface, std::size_t offset = 0);

/**
 * @brief Copy the contents of a buffer to a surface starting at an offset
 *
 * This function copies the bytes stored in a buffer starting at @c offset into the texture of a surface. If the offset
 * goes beyond the end of the buffer, the copy is aborted. If the size of the buffer is lesser than the size of the surface,
 * the copy is aborted and an error is issued.
 *
 * @note Each pixel requires 4 bytes; 1 byte for each component: B (blue), G (green), R (red) and A (alpha/transparency).
 *
 * @see buffer_get_surface
 *
 * @param buffer The handle to the buffer being read from
 * @param surface The surface whose texture will be written to
 * @param offset The offset into the buffer where reading starts from
 */
void buffer_set_surface(buffer_t buffer, int surface, std::size_t offset = 0);

/**
 * @brief Resize a buffer to a given size
 *
 * If the buffer is being resized to a smaller size, all bytes after the new end will not be preserved, so resizing the
 * buffer back to a larger size will not make the old bytes available again.
 *
 * @param buffer The handle to the buffer to be resized
 * @param size The new size of the buffer
 */
void buffer_resize(buffer_t buffer, std::size_t size);

/**
 * @brief Set the seek position of the buffer to a given offset
 *
 * This function sets the position where the next write or read happens in the buffer. There are three possible seek
 * bases:
 * - @c buffer_seek_start: Start seeking from index 0, the start of the buffer
 * - @c buffer_seek_relative: Start seeking from the current seek position
 * - @c buffer_seek_end: Start seeking from the end of the buffer
 *
 * @note A negative @c offset value can be used to seek backwards. In case tbe seek position ends up negative, it is
 * clamped to 0. In a @c buffer_wrap buffer, the seek position is wrapped around to the beginning of the buffer if it goes
 * past the end, otherwise it is clamped to the buffer end in case of the other types.
 *
 * @see buffer_tell
 *
 * @param buffer The handle of the buffer to seek
 * @param base The position to start the seek from
 * @param offset The offset to seek to
 */
void buffer_seek(buffer_t buffer, buffer_seek_t base, long long offset);

/**
 * @brief Get the size of a buffer data type
 *
 * Each data type which can be written to a buffer in GML has a size, they are:
 * - @c buffer_u8: 1 byte
 * - @c buffer_s8: 1 byte
 * - @c buffer_bool: 1 byte
 * - @c buffer_u16: 2 bytes
 * - @c buffer_s16: 2 bytes
 * - @c buffer_u32: 4 bytes
 * - @c buffer_s32: 4 bytes
 * - @c buffer_u64: 8 bytes
 * - @c buffer_f16: 2 bytes
 * - @c buffer_f32: 4 bytes
 * - @c buffer_f64: 8 bytes
 * - @c buffer_string: 0 bytes
 * - @c buffer_text: 0 bytes
 *
 * @note In the cases of @c buffer_string and @c buffer_text, there is no way to know the length before knowing what the
 * string itself is, so the size is considered 0, i.e. to be calculated later.
 *
 * @param type The data type to get the size of
 * @return The size of the data type
 */
std::size_t buffer_sizeof(buffer_data_t type);

/**
 * @brief Get the seek position of the buffer
 *
 * This function returns the position in the buffer where the next read or write will occur.
 *
 * @see buffer_seek
 *
 * @param buffer The handle to the buffer
 * @return The seek position of the buffer
 */
std::size_t buffer_tell(buffer_t buffer);


/**
 * @brief Get the data element at a offset in a buffer
 *
 * This function reads the bytes required to make a new value of the type @c type. It does not modify the seek position
 * in any way.
 *
 * @param buffer The handle to the buffer being read
 * @param offset The offset into the buffer where reading will start from
 * @param type The data type of the data element to be read
 * @return The data in the @c type data type
 */
variant buffer_peek(buffer_t buffer, std::size_t offset, buffer_data_t type);

/**
 * @brief Read the next data element in a buffer
 *
 * This function first advances the seek position until it is aligned to the buffer's alignment. It then reads the data
 * using @c buffer_peek() and seeks to the byte after the current data element.
 *
 * @param buffer The handle to the buffer being read
 * @param type The data type of the data element to be read
 * @return The data in the @c type data type
 */
variant buffer_read(buffer_t buffer, buffer_data_t type);

/**
 * @brief Fill @c size bytes of a buffer at @c offset with @c value
 *
 * This function first pads the buffer with 0 bytes until the seek position is aligned to the alignment of the buffer.
 * Then, if the write goes off of the end of the buffer, it truncates it to the buffer end. Each element being written is
 * padded with enough zeroes so that the next element being written is aligned to the buffer's alignment.
 *
 * @note The zeroes written as padding also count under the number of bytes specified by @c size
 *
 * @note There is an incompatibility with GMS here: the game completely hangs if you try to fill a buffer which
 * doesn't have enough space for @c size bytes. To avoid this issue, I clamp @c size to the buffer end if it is
 * too large instead of emulating a hang.
 *
 * @param buffer The handle to the buffer to be filled
 * @param offset The offset into the buffer where writing will begin
 * @param type The type of the data being written
 * @param value The data itself
 * @param size The number of bytes to write to the buffer
 */
void buffer_fill(buffer_t buffer, std::size_t offset, buffer_data_t type, variant value, std::size_t size);

/**
 * @brief Write a value at a given offset of a buffer
 *
 * This function writes the given @c value into @c buffer starting at @c offset. If the bytes being written go beyond the
 * end of the buffer for any of the four possible types, the write is aborted and an error is issued. This function does
 * not modify the seek position of the buffer.
 *
 * @note there is a GMS incompatibility here; in GMS if the data cannot fit within the current size of the buffer, the
 * write is simply aborted. Here, if the buffer is not large enough, it can be resized if its type is @c buffer_grow and
 * the value @c true is passed for the @c resize parameter.
 *
 * @param buffer The handle of the buffer to write to
 * @param offset The offset into the buffer where the writing will start form
 * @param type The type of the data to be written
 * @param value The data itself
 * @param resize Whether or not the buffer should be resized when it is @c buffer_grow
 */
void buffer_poke(buffer_t buffer, std::size_t offset, buffer_data_t type, variant value, bool resize = false);

/**
 * @brief Write a data element of a given type to a buffer
 *
 * This function first writes padding zeroes until the seek position is aligned to the alignment of the buffer. It then
 * writes @c value to the buffer using @c buffer_poke() and seeks to the byte after the data element being written.
 *
 * @param buffer The handle of the buffer being written to
 * @param type The type of data element being written to the buffer
 * @param value The data itself
 */
void buffer_write(buffer_t buffer, buffer_data_t type, variant value);

/**
 * @brief Serialize the game state (objects, backgrounds and room index) into a buffer
 *
 * This function serializes (in the following order):
 * - The number of active objects
 * - The active objects themselves
 * - The number of inactive objects
 * - The inactive objects themselves
 * - A header byte for the @c enigma::backgrounds @c AssetArray
 * - @c enigma::backgrounds itself
 * - A footer byte for the @c enigma::backgrounds @c AssetArray
 * - The current room index
 * - A SHA1-checksum
 *
 * The serialization for objects works in the following manner: for each class in the object hierarchy, a leading byte
 * is emitted to identify the object and verify that the data following it is valid. The bytes are written as follows:
 * - @c object_basic : @c 0xAA
 * - @c object_planar : @c 0xAB
 * - @c object_timelines : @c 0xAC
 * - @c object_graphics : @c 0xAD
 * - @c object_transform : @c 0xAE
 * - @c object_collisions : @c 0xAF
 * - Any user defined object : @c 0xBB
 *
 * These leading bytes are then followed by the data of each class. While the data that is serialized for the inner
 * classes does not change over time in terms of the contained variables, modifications to the user defined objects can
 * lead to generation of classes whose contents do not remain stable over time.
 *
 * To ensure backwards compatibility of previously saved game state with a future revision of the user-defined object, a
 * symbol table is emitted before each object's data block which maps the name of the serialized variable to the offset
 * from the end of the symbol table where its data is stored. On the C++ side, the compiler emits a table for every
 * object which maps the name of each variable in the object to a function which is responsible for deserializing its
 * state from a byte stream. Then, the intersection of these two tables are taken at runtime, which gives the set of
 * variables that exist both in the current revision of the object and the one serialized as part of the older game state,
 * along with deserialization routines for each variable.
 *
 * As a proof of concept of implementing serialization and deserialization for an object type, the required routines are
 * defined for the @c AssetArray and the @c Background classes to allow serializing and deserializing <tt> enigma::backgrounds </tt>,
 * which is a <tt> AssetArray<Background> </tt>. The methods are:
 * - <tt> std::size_t byte_size() const noexcept </tt> - Get the size (in bytes) of the object
 * - <tt> std::vector<std::size_t> serialize() </tt> - Serialize the object into a byte stream
 * - <tt> std::size_t deserialize_self(std::byte *iter) </tt> - Deserialize @c this from the byte stream and return the
 *                                                              total number of bytes read
 * - <tt> static std::pair<std::size_t, T> deserialize(std::byte *iter) - Deserialize an object from the given byte stream
 *                                                                        and return it along with its size
 *
 * Out of these four, @c byte_size() and @c deserialize() are automatically picked up by the routines defined in
 * @c serialization.h . The other methods have to be detected at compile time by the caller, using code like the following:
 *
 * @code{.cpp}
 * if constexpr (has_serialize_method_v<T>) {
 *   for (std::size_t i = 0; i < assets_.size(); i++) {
 *     std::vector<std::byte> serialized = assets_[i].serialize();
 *     // ...
 *   }
 * } else if constexpr (HAS_INTERNAL_SERIALIZE_FUNCTION()) {
 *   for (std::size_t i = 0; i < assets_.size(); i++) {
 *     enigma::enigma_serialize(operator[](i), len, result);
 *     // ...
 *   }
 * }
 * @endcode
 *
 * Both of the functions used in the <tt> if constexpr </tt> blocks are defined in @c detect_serialization.h . The first
 * one, <tt> has_serialize_method_v<T> </tt>, checks if the object has a method of the form <tt> object.serialize() </tt>
 * following the type given before. The second one, <tt> HAS_INTERNAL_SERIALIZE_FUNCTION() </tt>, checks if the <tt> enigma::serialize </tt>
 * function is invocable with the object's type. Together, these allow handling both cases of <tt> object.serialize() </tt>
 * and <tt> serialize(object) </tt>.
 *
 * After all the state is serialized, the SHA-1 checksum of the buffer's data is calculated and written at the end.
 *
 * An example of the serialized state may look like the following:
 *
 * ┌────────┬─────────────────────────┬─────────────────────────┬────────┬────────┐
 * │00000000│ 00 00 00 00 00 00 00 01 ┊ aa 00 01 86 a1 00 00 00 │0000000•┊×0•××000│
 * │00000010│ 00 ab 00 00 00 00 00 00 ┊ 00 00 00 00 00 00 00 00 │0×000000┊00000000│
 * │00000020│ 00 00 00 00 00 00 00 00 ┊ 00 00 00 00 00 00 00 00 │00000000┊00000000│
 * │*       │                         ┊                         │        ┊        │
 * │00000060│ 00 00 00 00 00 00 00 00 ┊ 00 00 00 00 00 00 00 40 │00000000┊0000000@│
 * │00000070│ 70 e0 00 00 00 00 00 00 ┊ 00 00 00 00 00 00 00 ac │p×000000┊0000000×│
 * │00000080│ 00 00 00 00 00 00 00 00 ┊ ff ff ff ff 00 3f 80 00 │00000000┊××××0?×0│
 * │00000090│ 00 00 00 00 00 00 ad ff ┊ ff ff ff 00 00 00 00 3f │000000××┊×××0000?│
 * │000000a0│ 80 00 00 00 bf f0 00 00 ┊ 00 00 00 00 00 00 00 00 │×000××00┊00000000│
 * │000000b0│ 00 00 00 00 00 01 3f 80 ┊ 00 00 3f 80 00 00 00 00 │00000•?×┊00?×0000│
 * │000000c0│ 00 00 ae 3f f0 00 00 00 ┊ 00 00 00 00 ff ff ff af │00×?×000┊0000××××│
 * │000000d0│ ff ff ff ff 00 ff ff ff ┊ ff 3f 80 00 00 3f 80 00 │××××0×××┊×?×00?×0│
 * │000000e0│ 00 00 00 00 00 00 00 00 ┊ 00 00 00 00 00 bb 00 00 │00000000┊00000×00│
 * │000000f0│ 00 00 00 00 00 02 00 00 ┊ 00 00 00 00 00 06 66 6f │00000•00┊00000•fo│
 * │00000100│ 6f 62 61 72 00 00 00 00 ┊ 00 00 00 39 00 00 00 00 │obar0000┊00090000│
 * │00000110│ 00 00 00 06 62 61 72 66 ┊ 6f 6f 00 00 00 00 00 00 │000•barf┊oo000000│
 * │00000120│ 00 00 00 00 00 00 00 00 ┊ 00 72 00 40 20 00 00 00 │00000000┊0r0@ 000│
 * │00000130│ 00 00 00 00 00 00 00 00 ┊ 00 00 00 00 00 00 00 00 │00000000┊00000000│
 * │*       │                         ┊                         │        ┊        │
 * │00000160│ 00 00 00 00 40 18 00 00 ┊ 00 00 00 00 00 00 00 00 │0000@•00┊00000000│
 * │00000170│ 00 00 00 00 00 00 00 00 ┊ 00 00 00 00 00 00 00 00 │00000000┊00000000│
 * │*       │                         ┊                         │        ┊        │
 * │000001a0│ 00 00 00 00 ee 00 00 00 ┊ 00 00 00 00 00 ef 00 00 │0000×000┊00000×00│
 * │000001b0│ 00 00 df f2 d6 04 a3 26 ┊ f5 ff 44 d2 0e 25 34 ec │00×××•×&┊××D×•%4×│
 * │000001c0│ 85 c7 7f c5 91 0c       ┊                         │××•××_  ┊        │
 * └────────┴─────────────────────────┴─────────────────────────┴────────┴────────┘
 *
 * @see game_load_buffer
 *
 * @param buffer The buffer to store the game state into
 */
void game_save_buffer(buffer_t buffer);

/**
 * @brief Load previously saved state from a buffer
 *
 * This function firstly checks the checksum of the buffer's data. If it matches the checksum written at the end of the
 * buffer, it proceeds with deserializing the state written in the buffer. Before deserializing anything, however,
 * it clears the active and inactive instance lists to avoid issues with clashing object IDs. It then deserializes each
 * object by reading its object index (a unique identifier for each type), and calling the virtual @c deserialize_self()
 * function of the object after using <tt> enigma::instance_create_id </tt> to create the specific object type. After
 * checking the header of the <tt> enigma::backgrounds </tt> data, it reads that as well and verifies the footer. Finally,
 * it reads the room index and calls <tt> enigma::room_goto </tt> with the index.
 *
 * @see game_save_buffer
 *
 * @param buffer The buffer to load the game state from
 */
void game_load_buffer(buffer_t buffer);

}  //namespace enigma_user

#endif  //ENIGMA_BUFFERS_H