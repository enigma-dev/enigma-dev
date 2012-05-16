/**
 * @file  llreader.cpp
 * @brief Source implementing a general-purpose array-to-file adapter.
 * 
 * This implementation is meant to figure out what functions are available for
 * mapping a file in memory, and use them. Otherwise, it must simply read the
 * entire file into memory. The implementation must also provide a way to copy
 * and mirror std::string contents.
 * 
 * @section License
 * 
 * Copyright (C) 2011 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include <cstdio>
#include <cstring>
#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
  #include <windows.h>  
#else
  #include <sys/mman.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
#endif

#include "llreader.h"

/// Enumeration of open states for an \c llreader.
enum {
  FT_CLOSED, ///< No file is currently open; either no file was ever opened, or it has since been closed.
  FT_BUFFER, ///< The file data is stored in a buffer that must be freed at close.
  FT_MMAP,   ///< The file data is in a memory mapped file which must be closed using the system-specific method.
  FT_ALIAS   ///< The file data is a pointer to a buffer owned by another object or function, and should not be freed.
};

using namespace std;

void llreader::encapsulate(string &contents) {
  data = contents.c_str();
  length = contents.length();
  mode = FT_ALIAS;
}
void llreader::copy(string contents) {
  length = contents.length();
  char* buf = new char[length + 1];
  memcpy(buf, contents.c_str(), length);
  buf[length] = 0;
  mode = FT_BUFFER;
  data = buf;
}

llreader::llreader(): mode(FT_CLOSED), pos(0), length(0), data(NULL) {}
llreader::llreader(const char* filename): mode(FT_CLOSED), pos(0), length(0), data(NULL) { open(filename); }
llreader::llreader(std::string contents, bool cp): mode(FT_CLOSED), pos(0), length(0), data(NULL) { cp? copy(contents) : encapsulate(contents); }

#include <iostream>
llreader::llreader(const llreader& x): mode(FT_BUFFER), pos(x.pos), length(FT_BUFFER), data(NULL) {
  cout << "COPY CALLED ON LLREADER" << endl;
  if (x.mode == FT_CLOSED) mode = FT_CLOSED;
  else {
    char *buf = new char[x.length+1];
    memcpy(buf, x.data, x.length);
    buf[length = x.length] = 0;
    data = buf;
  }
}
llreader::~llreader() { close(); }

static inline void dump_in(const char* name, llreader* dest) {
  FILE* in = fopen(name,"rb");
  fseek(in,0,SEEK_END);
  long int sz = ftell(in);
  fseek(in,0,SEEK_SET);
  char* buf = new char[sz+1];
  fread(buf, 1, sz, in);
  buf[sz] = 0;
  dest->data = buf;
  dest->length = sz;
  fclose(in);
}

void llreader::open(const char* filename) {
#if defined(IO_FALLBACK)
  #warn Compiling in fallback file mode. May lead to slowness.
  dump_in(filename);
#elif defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
  #error unimplemented
#else
  int fd = ::open(filename, O_RDONLY);
  if (fd == -1)
    return;
  struct stat statbuf;
  fstat(fd, &statbuf), length = statbuf.st_size;
  data = (const char*)mmap(NULL, length, PROT_READ, MAP_SHARED, fd, 0);
  mode = FT_MMAP;
#endif
}

void llreader::alias(const char* buffer, size_t len) {
  mode = FT_ALIAS;
  pos = 0, length = len;
  data = buffer;
}

void llreader::alias(const llreader &llread) {
  mode = FT_ALIAS;
  pos = llread.pos, length = llread.length;
  data = llread.data;
}

void llreader::consume(char* buffer, size_t len) {
  mode = FT_BUFFER;
  pos = 0, length = len;
  data = buffer;
}

void llreader::consume(llreader& whom) {
  mode = whom.mode;
  pos = whom.pos;
  length = whom.length;
  data = whom.data;
  whom.mode = FT_CLOSED;
  whom.length = 0;
  whom.data = NULL;
}

void llreader::close() {
  switch (mode) {
    case FT_BUFFER: delete []data;
    case FT_CLOSED: break;
    case FT_MMAP:
        #ifdef IO_FALLBACK
          fprintf(stderr,"Error: Attempting to free what could never have been allocated...\n");
        #else
          #if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
            #error unimplemented
          #else
            munmap((void*)data, length);
          #endif
        #endif
      break;
    case FT_ALIAS: default: data = NULL; break;
  }
  mode = FT_CLOSED;
}

bool llreader::is_open() {
  return mode != FT_CLOSED;
}
