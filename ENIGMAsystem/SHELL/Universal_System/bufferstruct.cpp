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
#include <cstring>
using namespace std;

#include "Graphics_Systems/graphics_mandatory.h"
#include "libEGMstd.h"
#include "bufferstruct.h"

#ifdef DEBUG_MODE
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_background(bck2d,back)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      show_error("Attempting to draw non-existing background " + toString(back), false);\
      return;\
    }\
    enigma::background *bck2d = enigma::backgroundstructarray[back];
  #define get_backgroundnv(bck2d,back,r)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      show_error("Attempting to draw non-existing background " + toString(back), false);\
      return r;\
    }\
    enigma::background *bck2d = enigma::backgroundstructarray[back];
#else
  #define get_background(bck2d,back)\
    enigma::background *bck2d = enigma::backgroundstructarray[back];
  #define get_backgroundnv(bck2d,back,r)\
    enigma::background *bck2d = enigma::backgroundstructarray[back];
#endif

namespace enigma
{

}

namespace enigma_user
{

int buffer_create(unsigned size, int type, unsigned alignment) {
}

void buffer_delete(int buffer) {
}

void buffer_copy(int src_buffer, unsigned src_offset, unsigned size, int dest_buffer, unsigned dest_offset) {
}

void buffer_save(int buffer, string filename) {
}

void buffer_save_ext(int buffer, string filename, unsigned offset, unsigned size) {
}

void buffer_load(string filename) {
}

void buffer_load_ext(int buffer, string filename, unsigned offset) {
}

void buffer_fill(int buffer, unsigned offset, int type, variant value, unsigned size) {
}

int buffer_get_size(int buffer) {
}

void buffer_get_surface(int buffer, int surface, int mode, unsigned offset, int modulo) {
}

string buffer_md5(int buffer, unsigned offset, unsigned size) {
}

string buffer_sha1(int buffer, unsigned offset, unsigned size) {
}

int buffer_base64_decode(string str) {
}

int buffer_base64_decode_ext(int buffer, string str, unsigned offset) {
}

string buffer_base64_encode(int buffer, unsigned offset, unsigned size) {
}

void buffer_poke(int buffer, unsigned offset, int type, variant value) {
}

void buffer_resize(int buffer, unsigned size) {
}

void buffer_seek(int buffer, unsigned base, unsigned offset) {
}

void buffer_sizeof(int type) {
}

int buffer_tell(int buffer) {
}

variant buffer_peek(int buffer, unsigned offset, int type) {
}

variant buffer_read(int buffer, int type) {
}

void buffer_write(int buffer, int type, variant value) {
}

void game_save_buffer(int buffer) {
}

void game_load_buffer(int buffer) {
}

}

