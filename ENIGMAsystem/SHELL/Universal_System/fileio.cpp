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
#include "Widget_Systems/widgets_mandatory.h"

#include <fstream>
#include <iostream>
#include <string>

using std::string;

namespace enigma
{
  struct file {
    file(const::string& fName, std::ios_base::openmode mode) : fn(fName) { fs.open(fName, mode); }
    std::string fn;
    std::fstream fs;  
  };
  
  std::vector<file> files;
  extern string gameInfoText;
  
  static inline int file_open(const string& fname, std::ios_base::openmode mode) {
    file f(fname, mode);
    
    if (f.fs.is_open()) {
      files.emplace_back(std::move(f));
      return files.size()-1;
    } else return -1;
  }
}

namespace enigma_user
{

int file_text_open_read(string fname) // Opens the file with the indicated name for reading. The function returns the id of the file that must be used in the other functions. You can open multiple files at the same time (32 max). Don't forget to close them once you are finished with them.
{
  return enigma::file_open(fname, std::ios::in);
}

int file_text_open_write(string fname) // Opens the indicated file for writing, creating it if it does not exist. The function returns the id of the file that must be used in the other functions.
{
  return enigma::file_open(fname, std::ios::out);
}

int file_text_open_append(string fname) // Opens the indicated file for appending data at the end, creating it if it does not exist. The function returns the id of the file that must be used in the other functions.
{
  return enigma::file_open(fname, std::ios::app);
}

void file_text_close(int fileid) // Closes the file with the given file id.
{
  if (fileid > 0 && fileid < static_cast<int>(enigma::files.size())) {
    enigma::files[fileid].fs.close();
  } else DEBUG_MESSAGE("Cannot close an unopened file.", MESSAGE_TYPE::M_USER_ERROR);

}

void file_text_write_string(int fileid, string str) { // Writes the string to the file with the given file id.
  enigma::files[fileid].fs << str;
}

void file_text_write_real(int fileid, double x) // Write the real value to the file with the given file id.
{
  enigma::files[fileid].fs << x;
}

void file_text_writeln(int fileid) // Write a newline character to the file.
{
  enigma::files[fileid].fs << '\n';
}

void file_text_writeln(int fileid,string str) // Write a string and newline character to the file.
{
  file_text_write_string(fileid,str);
  file_text_writeln(fileid);
}

string file_text_read_string(int fileid) { // Reads a string from the file with the given file id and returns this string. A string ends at the end of line.
  string line;
  std::getline(enigma::files[fileid].fs, line);
  return line;
}

string file_text_read_all(int fileid) {
  std::string all, line;
  while(std::getline(enigma::files[fileid].fs, line)) all += line;
  return all;
}

bool file_text_eoln(int fileid)
{
 return (static_cast<char>(enigma::files[fileid].fs.peek()) == '\n');
}

inline bool is_whitespace(char x) { return x == ' ' or x == '\t' or x == '\r' or x == '\n'; }

double file_text_read_real(int fileid) { // Reads a real value from the file and returns this value.
  double x;
  enigma::files[fileid].fs >> x;
  return x;
}

std::string file_text_readln(int fileid) // Skips the rest of the line in the file and starts at the start of the next line.
{
  return file_text_read_string(fileid);
}

bool file_text_eof(int fileid) { // Returns whether we reached the end of the file.
  return enigma::files[fileid].fs.eof();
}

void load_info(string fname) {
	int file = file_text_open_read(fname);
	enigma::gameInfoText = file_text_read_all(file);
	file_text_close(file);
}

int file_bin_open(string fname, int mode) // Opens the file with the indicated name. The mode indicates what can be done with the file: 0 = reading, 1 = writing, 2 = both reading and writing). When the file does not exist it is created. The function returns the id of the file that must be used in the other functions.
{
  switch (mode) {
    case 0: return enigma::file_open(fname, std::ios::in  | std::ios::binary);
    case 1: return enigma::file_open(fname, std::ios::out | std::ios::binary);
    case 2: return enigma::file_open(fname, std::ios::in  | std::ios::out | std::ios::binary);
    default: return -1;
  }
}

bool file_bin_rewrite(int fileid) // Rewrites the file with the given file id, that is, clears it and starts writing at the start.
{
  enigma::files[fileid].fs.clear();
  enigma::files[fileid].fs.close();
  enigma::files[fileid].fs.open(enigma::files[fileid].fn, std::ios::in | std::ios::out | std::ios::binary);
  return enigma::files[fileid].fs.good();
}

void file_bin_close(int fileid) // Closes the file with the given file id.
{
  enigma::files[fileid].fs.close();
}

size_t file_bin_size(int fileid) // Returns the size (in bytes) of the file with the given file id.
{
  size_t currPos = enigma::files[fileid].fs.tellg();
  enigma::files[fileid].fs.seekg(0, enigma::files[fileid].fs.end);
  size_t length = enigma::files[fileid].fs.tellg();
  enigma::files[fileid].fs.seekg(currPos);
  return length;
}

size_t file_bin_position(int fileid) { // Returns the current position (in bytes; 0 is the first position) of the file with the given file id.
  return enigma::files[fileid].fs.tellg();
}

void file_bin_seek(int fileid, size_t pos) { // Moves the current position of the file to the indicated position. To append to a file move the position to the size of the file before writing.
  enigma::files[fileid].fs.seekg(pos);
}

void file_bin_write_byte(int fileid, unsigned char byte) { // Writes a byte of data to the file with the given file id.
  enigma::files[fileid].fs << byte;
}

int file_bin_read_byte(int fileid) { // Reads a byte of data from the file and returns this.
  char byte;
  enigma::files[fileid].fs.get(byte);
  return (enigma::files[fileid].fs.good()) ? (int)byte : -1;
}

}
