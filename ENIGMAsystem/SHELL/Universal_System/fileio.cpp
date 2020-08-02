/** Copyright (C) 2008-2011 Josh Ventura
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

// Simple, intuitive, integer based file I/O

#include "fileio.h"
#include "Resources/AssetArray.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>

#ifdef DEBUG_MODE
#define try_io_and_print(f) print_and_clear_fs_status(__FUNCTION__, f);
#else
#define try_io_and_print(f)
#endif

namespace enigma {
  
  struct file {
    file() {}
    file(const std::string& fName, std::ios_base::openmode mode) : fn(fName) { fs.open(fName, mode); }
    file(file&& other) : fn(other.fn) { fs.swap(other.fs); }
    file(file& other) : fn(other.fn) { fs.swap(other.fs); } 
    std::string fn;
    std::fstream fs;
    // AssArray mandatory
    static const char* getAssetTypeName() { return "FileHandle"; }
    bool isDestroyed() const { return false; }
    void destroy() { fs.close(); }
  };
  
  AssetArray<file> files;
  
  static void print_and_clear_fs_status(const std::string& operation, file& f) {
    if (!f.fs.good()) {
      DEBUG_MESSAGE("Operation: " + operation + " failed on: " + f.fn + " Failbit: " + std::to_string(f.fs.fail())
        + " Badbit: " +  std::to_string(f.fs.bad()) + " EOF: " + std::to_string(f.fs.eof()), MESSAGE_TYPE::M_USER_ERROR);
    }
    bool eof = f.fs.eof();
    f.fs.clear();
    if (eof) f.fs.setstate(std::ios::eofbit);
  }
  
  static inline int file_open(const std::string& fname, std::ios_base::openmode mode) {
    file f(fname, mode);
    
    try_io_and_print(f)
    
    if (f.fs.is_open()) {
      files.add(std::move(f));
      return files.size()-1;
    } else return -1;
  }
} // NAMESPACE enigma


namespace enigma_user {

// Opens the file with the indicated name for reading. The function returns the id of the file that must be used in the other functions. You can open multiple files at the same time (32 max). Don't forget to close them once you are finished with them.
int file_text_open_read(const std::string& fname) {
  return enigma::file_open(fname, std::ios::in);
}

// Opens the indicated file for writing, creating it if it does not exist. The function returns the id of the file that must be used in the other functions.
int file_text_open_write(const std::string& fname) {
  return enigma::file_open(fname, std::ios::out);
}

// Opens the indicated file for appending data at the end, creating it if it does not exist. The function returns the id of the file that must be used in the other functions.
int file_text_open_append(const std::string& fname) {
  return enigma::file_open(fname, std::ios::out | std::ios::app);
}

// Closes the file with the given file id
void file_text_close(int fileid) {
  if (fileid >= 0 && fileid < static_cast<int>(enigma::files.size())) {
    enigma::files.get(fileid).fs.close();
  } else DEBUG_MESSAGE("Cannot close an unopened file: " + std::to_string(fileid), MESSAGE_TYPE::M_USER_ERROR);
}

// Writes the std::string to the file with the given file id.
void file_text_write_string(int fileid, const std::string& str) {
  enigma::files.get(fileid).fs << str;
  try_io_and_print(enigma::files.get(fileid))
}

// Write the real value to the file with the given file id.
void file_text_write_real(int fileid, double x) {
  enigma::files.get(fileid).fs << " " << x << std::setprecision(16);
  try_io_and_print(enigma::files.get(fileid))
}

// Write a newline character to the file.
void file_text_writeln(int fileid) {
  enigma::files.get(fileid).fs << '\n';
  try_io_and_print(enigma::files.get(fileid))
}

// Write a string and newline character to the file.
void file_text_writeln(int fileid, const std::string& str) {
  file_text_write_string(fileid, str);
  file_text_writeln(fileid);
}

// Reads a string from the file with the given file id and returns this string. A string ends at the end of line.
std::string file_text_read_string(int fileid) {
  std::string line;
  if (std::getline(enigma::files.get(fileid).fs, line)) {
    enigma::files.get(fileid).fs.unget();
    try_io_and_print(enigma::files.get(fileid))
  }
  return line;
}

std::string file_text_read_all(int fileid) {
  std::string all, line;
  while(std::getline(enigma::files.get(fileid).fs, line)) {
    try_io_and_print(enigma::files.get(fileid))
    all += line;
  }
  return all;
}

bool file_text_eoln(int fileid) {
  return (static_cast<char>(enigma::files.get(fileid).fs.peek()) == '\n' || enigma::files.get(fileid).fs.eof());
}

double file_text_read_real(int fileid) { // Reads a real value from the file and returns this value.
  double x = 0;
  enigma::files.get(fileid).fs >> x;
  try_io_and_print(enigma::files.get(fileid))
  return x;
}

// Skips the rest of the line in the file and starts at the start of the next line.
std::string file_text_readln(int fileid) {
  std::string line;
  std::getline(enigma::files.get(fileid).fs, line);
  return line;
}

bool file_text_eof(int fileid) { // Returns whether we reached the end of the file.
  return (enigma::files.get(fileid).fs.eof());
}

void load_info(const std::string& fname) {
	int file = file_text_open_read(fname);
	enigma::gameInfoText = file_text_read_all(file);
	file_text_close(file);
}

// Opens the file with the indicated name. The mode indicates what can be done with the file: 0 = reading, 1 = writing, 2 = both reading and writing). When the file does not exist it is created. The function returns the id of the file that must be used in the other functions.
int file_bin_open(const std::string& fname, int mode) {
  // TODO: add other modes like trunc / append?
  switch (mode) {
    case 0: return enigma::file_open(fname, std::ios::in  | std::ios::binary);
    case 1: return enigma::file_open(fname, std::ios::out | std::ios::binary);
    case 2: return enigma::file_open(fname, std::ios::in  | std::ios::out | std::ios::binary);
    default: return -1;
  }
}

// Rewrites the file with the given file id, that is, clears it and starts writing at the start.
bool file_bin_rewrite(int fileid) {
  enigma::files.get(fileid).fs.close();
  enigma::files.get(fileid).fs.open(enigma::files.get(fileid).fn, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
  try_io_and_print(enigma::files.get(fileid))
  return enigma::files.get(fileid).fs.good();
}

// Closes the file with the given file id.
void file_bin_close(int fileid) {
  enigma::files.get(fileid).fs.close();
}

// Returns the size (in bytes) of the file with the given file id.
size_t file_bin_size(int fileid) {
  size_t currPos = enigma::files.get(fileid).fs.tellg();
  enigma::files.get(fileid).fs.seekg(0, enigma::files.get(fileid).fs.end);
  size_t length = enigma::files.get(fileid).fs.tellg();
  enigma::files.get(fileid).fs.seekg(currPos);
  try_io_and_print(enigma::files.get(fileid))
  return length;
}

// Returns the current position (in bytes; 0 is the first position) of the file with the given file id.
size_t file_bin_position(int fileid) {
  return enigma::files.get(fileid).fs.tellg();
}

// Moves the current position of the file to the indicated position. To append to a file move the position to the size of the file before writing.
void file_bin_seek(int fileid, size_t pos) {
  enigma::files.get(fileid).fs.seekg(pos);
  try_io_and_print(enigma::files.get(fileid))
}

// Writes a byte of data to the file with the given file id.
void file_bin_write_byte(int fileid, unsigned char byte) {
  enigma::files.get(fileid).fs << byte;
  try_io_and_print(enigma::files.get(fileid))
}

// Reads a byte of data from the file and returns this
int file_bin_read_byte(int fileid) {
  unsigned char byte = 0;
  enigma::files.get(fileid).fs >> byte;
  bool good = enigma::files.get(fileid).fs.good();
  try_io_and_print(enigma::files.get(fileid))
  return (!good) ? -1 : static_cast<int>(byte);
}

} // NAMESPACE enigma_user
