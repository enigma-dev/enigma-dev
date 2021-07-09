/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2014 Robert B. Colton
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

#ifndef FILEIO_H
#define FILEIO_H

#include  "file_t.h"

#include <cstdint>

size_t fread_wrapper(void* ptr, size_t size, size_t maxnum, FILE_t* context);
FILE_t* fopen_wrapper(const char* fname, const char* mode);
FILE_t* freopen_wrapper(const char* fname, const char* mode, FILE_t* context);
int64_t fseek_wrapper(FILE_t* context, int64_t offset, int whence);
int fclose_wrapper(FILE_t* context);
int fgetc_wrapper(FILE_t* context);
char* fgets_wrapper(char* str, int num, FILE_t* context);
int fputc_wrapper(int character, FILE_t* context);
int feof_wrapper(FILE_t* context);
int64_t ftell_wrapper(FILE_t* context);
size_t fwrite_wrapper(const void *ptr, size_t size, size_t count, FILE_t* context);

#include <string>

namespace enigma_user {
int file_text_open_read(const std::string& fname);
int file_text_open_write(const std::string& fname);
int file_text_open_append(const std::string& fname);
void file_text_close(int fileid);
void file_text_write_string(int fileid, const std::string& str);
void file_text_write_real(int fileid, double x);
void file_text_writeln(int fileid);
void file_text_writeln(int fileid, const std::string& str);
std::string file_text_read_string(int fileid);
std::string file_text_read_all(int fileid);
double file_text_read_real(int fileid);
std::string file_text_readln(int fileid);
bool file_text_eof(int fileid);
bool file_text_eoln(int fileid);
void load_info(const std::string& fname); // game information function
int file_bin_open(const std::string& fname, int mode);
bool file_bin_rewrite(int fileid);
void file_bin_close(int fileid);
size_t file_bin_size(int fileid);
size_t file_bin_position(int fileid);
void file_bin_seek(int fileid, size_t pos);
void file_bin_write_byte(int fileid, unsigned char byte);
int file_bin_read_byte(int fileid);

} //namespace enigma_user

#endif
