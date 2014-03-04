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

#include <string>
#include <vector>
using std::vector;

#include "var4.h"
namespace enigma_user
{

// Other buffer constants
enum {
	buffer_generalerror,
	buffer_invalidtype,
	buffer_outofbounds,
	buffer_outofspace,
	buffer_vbuffer,
};

// Buffer surface constants
enum {
	buffer_surface_copy,
	buffer_surface_grayscale,
	buffer_surface_mask
};

// Buffer seek base
enum {
	buffer_seek_start = 0,
	buffer_seek_relative = 1,
	buffer_seek_end = 2
};

// Buffer types
enum {
	buffer_fixed = 0,
	buffer_grow = 1,
	buffer_wrap = 2,
	buffer_fast = 3
};

// Buffer data types
enum {
	buffer_u8 = 1,
	buffer_s8 = 2,
	buffer_u16 = 3,
	buffer_s16 = 4,
	buffer_u32 = 5,
	buffer_s32 = 6,
	buffer_f16 = 7,
	buffer_f32 = 8,
	buffer_f64 = 9,
	buffer_bool = 10,
	buffer_string = 11
};

}

namespace enigma
{
  struct BinaryBuffer
  {
	vector<unsigned char> data;
	unsigned position;
	unsigned alignment;
	int type;
	
    BinaryBuffer(unsigned size) {
		data.resize(size, 0);
		position = 0;
		alignment = 1;
		type = 0;
	}
	
	~BinaryBuffer() {
	
	}
	
	unsigned GetSize() {
		return data.size();
	}
	
	void Resize(unsigned size) {
		data.resize(size, 0);
	}

	void Seek(unsigned offset) {
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
	
	unsigned char ReadByte() {
		Seek(position);
		unsigned char byte = data[position];
		Seek(position + 1);
		return byte;
	}
	
	void WriteByte(unsigned char byte) {
		Seek(position);
		data[position] = byte;
		Seek(position + 1);
	}

  };
  
  extern vector<BinaryBuffer*> buffers;
}

  #ifdef DEBUG_MODE
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_buffer(binbuff,buff)\
    if (buff < 0 or size_t(buff) >= enigma::buffers.size() or !enigma::buffers[buff]) {\
      show_error("Attempting to access non-existing buffer " + toString(buff), false);\
      return;\
    }\
    enigma::BinaryBuffer *binbuff = enigma::buffers[buff];
  #define get_bufferr(binbuff,buff,r)\
    if (buff < 0 or size_t(buff) >= enigma::buffers.size() or !enigma::buffers[buff]) {\
      show_error("Attempting to access non-existing buffer " + toString(buff), false);\
      return r;\
    }\
    enigma::BinaryBuffer *binbuff = enigma::buffers[buff];
	#else
	  #define get_buffer(binbuff,buff)\
		enigma::BinaryBuffer *binbuff = enigma::buffers[buff];
	  #define get_bufferr(binbuff,buff,r)\
		enigma::BinaryBuffer *binbuff = enigma::buffers[buff];
	#endif

namespace enigma_user
{

int buffer_create(unsigned size, int type, unsigned alignment);
void buffer_delete(int buffer);
void buffer_copy(int src_buffer, unsigned src_offset, unsigned size, int dest_buffer, unsigned dest_offset);
void buffer_save(int buffer, string filename);
void buffer_save_ext(int buffer, string filename, unsigned offset, unsigned size);
int buffer_load(string filename);
void buffer_load_ext(int buffer, string filename, unsigned offset);

int buffer_base64_decode(string str);
int buffer_base64_decode_ext(int buffer, string str, unsigned offset);
string buffer_base64_encode(int buffer, unsigned offset, unsigned size);
string buffer_md5(int buffer, unsigned offset, unsigned size);
string buffer_sha1(int buffer, unsigned offset, unsigned size);

unsigned buffer_get_size(int buffer);
unsigned buffer_get_alignment(int buffer);
int buffer_get_type(int buffer);
void buffer_get_surface(int buffer, int surface, int mode, unsigned offset, int modulo);
void buffer_resize(int buffer, unsigned size);
void buffer_seek(int buffer, int base, unsigned offset);
unsigned buffer_sizeof(int type);
int buffer_tell(int buffer);

variant buffer_peek(int buffer, unsigned offset, int type);
variant buffer_read(int buffer, int type);
void buffer_fill(int buffer, unsigned offset, int type, variant value, unsigned size);
void buffer_poke(int buffer, unsigned offset, int type, variant value);
void buffer_write(int buffer, int type, variant value);

void game_save_buffer(int buffer);
void game_load_buffer(int buffer);

}

