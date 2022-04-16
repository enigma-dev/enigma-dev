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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 *
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include "llreader.h"

#include <cstdio>
#include <cstring>
#include <iostream>

#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
  #include <windows.h>
#else
  #include <sys/mman.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
  #include <fcntl.h>
#endif

/// Enumeration of open states for an \c llreader.
enum {
  FT_CLOSED, ///< No file is currently open; either no file was ever opened, or it has since been closed.
  FT_BUFFER, ///< The file data is stored in a buffer that must be freed at close.
  FT_MMAP,   ///< The file data is in a memory mapped file which must be closed using the system-specific method.
  FT_ALIAS   ///< The file data is a pointer to a buffer owned by another object or function, and should not be freed.
};

using namespace std;

void llreader::encapsulate(string_view contents) {
  data = contents.data();
  length = contents.length();
  mode = FT_ALIAS;
  pos = 0;
  lpos = 0;
  lnum = kFirstLine;
}
void llreader::copy(string_view contents) {
  length = contents.length();
  char* buf = new char[length + 1];
  memcpy(buf, contents.data(), length);
  buf[length] = 0;
  pos = 0;
  lpos = 0;
  lnum = kFirstLine;
  mode = FT_BUFFER;
  data = buf;
}

static const string dot(1, '.');
inline string fn_path(const char *fn) {
  #if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
  #  define or_is_any_other_separator(c) or (c == '\\')
  #else
  #  define or_is_any_other_separator(c)
  #endif
  const char* last = fn;
  for (const char* i = fn; *i; ++i)
    if (*i == '/' or_is_any_other_separator(*i))
      last = i;
  return last == fn? dot : string(fn, last);
}

llreader::llreader(): pos(0), length(0), lpos(0), lnum(kFirstLine),
                      data(nullptr), name(""), mode(FT_CLOSED) {}
llreader::llreader(llreader &&other): llreader() { consume(other); }
llreader::llreader(const std::filesystem::path &filename): llreader() {
  open(filename);
}
llreader::llreader(std::string bname, std::string contents): pos(0), length(0),
    lpos(0), lnum(kFirstLine), data(nullptr), name(bname), mode(FT_CLOSED) {
  copy(contents);
}
llreader::llreader(std::string bname, std::string_view contents, bool copy_):
    pos(0), length(0),  lpos(0), lnum(kFirstLine), data(nullptr), name(bname),
    mode(FT_CLOSED) {
  copy_ ? copy(contents) : encapsulate(contents);
}
llreader::~llreader() { close(); }

void llreader::open(const std::filesystem::path &filename) {
  #ifdef DEBUG_MODE
    if (mode != FT_CLOSED and mode != FT_ALIAS)
      cerr << "ERROR! Leaked a file in open()." << endl;
  #endif
#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
  SECURITY_ATTRIBUTES sec;
  sec.nLength = sizeof(sec), sec.lpSecurityDescriptor = nullptr, sec.bInheritHandle = true;
  HANDLE hFile = CreateFileW(filename.c_str(), GENERIC_READ, FILE_SHARE_READ,
                             &sec, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS,
                             nullptr);
  if (hFile == INVALID_HANDLE_VALUE)
    return;
  HANDLE mapping = CreateFileMapping(hFile, &sec, PAGE_READONLY, 0, 0, nullptr);
  if (mapping == INVALID_HANDLE_VALUE) {
    CloseHandle(hFile);
    return;
  }
  data = (const char*)MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);
  if (data) {
    mode = FT_MMAP;
    length = GetFileSize(hFile, nullptr);
  }
  CloseHandle(mapping);
  CloseHandle(hFile);
#else
  int fd = ::open(filename.u8string().c_str(), O_RDONLY);
  if (fd == -1) return;
  struct stat statbuf;
  fstat(fd, &statbuf), length = statbuf.st_size;
  data = (const char*)mmap(nullptr, length, PROT_READ, MAP_SHARED, fd, 0);
  mode = FT_MMAP;
  ::close(fd);
#endif
  name = filename.u8string();

  pos = 0;
  lpos = 0;
  lnum = kFirstLine;

# ifndef NOVALIDATE_LINE_NUMBERS
  validated_pos = 0;
  validated_lpos = 0;
  validated_lnum = kFirstLine;
# endif
}

void llreader::alias(const char* buffer, size_t len) {
  #ifdef DEBUG_MODE
    if (mode != FT_CLOSED and mode != FT_ALIAS)
      cerr << "ERROR! Leaked a file in alias(buffer, length)." << endl;
  #endif
  mode = FT_ALIAS;
  pos = 0, length = len;
  data = buffer;
  name = "<user buffer>";

# ifndef NOVALIDATE_LINE_NUMBERS
  validated_pos = 0;
  validated_lpos = 0;
  validated_lnum = kFirstLine;
# endif
}

void llreader::alias(const llreader &llread) {
  #ifdef DEBUG_MODE
    if (mode != FT_CLOSED and mode != FT_ALIAS)
      cerr << "ERROR! Leaked a file in alias(llreader)." << endl;
  #endif
  mode = FT_ALIAS;
  length = llread.length;
  pos = llread.pos;
  data = llread.data;
  name = llread.name;
  lpos = llread.lpos;
  lnum = llread.lnum;

# ifndef NOVALIDATE_LINE_NUMBERS
  validated_pos = 0;
  validated_lpos = 0;
  validated_lnum = kFirstLine;
# endif
}

void llreader::consume(char* buffer, size_t len) {
  #ifdef DEBUG_MODE
    if (mode != FT_CLOSED and mode != FT_ALIAS)
      cerr << "ERROR! Leaked a file in consume(buffer, length)" << endl;
  #endif
  mode = FT_BUFFER;
  pos = 0, length = len;
  data = buffer;
  name = "<copy of user buffer>";

# ifndef NOVALIDATE_LINE_NUMBERS
  validated_pos = 0;
  validated_lpos = 0;
  validated_lnum = kFirstLine;
# endif
}

llreader &llreader::operator=(llreader &&other) {
  consume(other);
  return *this;
}

void llreader::consume(llreader& whom) {
  #ifdef DEBUG_MODE
    if (mode != FT_CLOSED and mode != FT_ALIAS)
      cerr << "ERROR! Leaked a file in consume(llreader)" << endl;
  #endif
  mode = whom.mode;
  pos = whom.pos;
  lpos = whom.lpos;
  lnum = whom.lnum;
  length = whom.length;
  data = whom.data;
  whom.mode = FT_CLOSED;
  whom.length = 0;
  whom.data = nullptr;
  name = whom.name;

# ifndef NOVALIDATE_LINE_NUMBERS
  validated_pos = 0;
  validated_lpos = 0;
  validated_lnum = kFirstLine;
# endif
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
            UnmapViewOfFile(data);
          #else
            munmap((void*)data, length);
          #endif
        #endif
      break;
    case FT_ALIAS: default: data = nullptr; break;
  }
  mode = FT_CLOSED;
}

bool llreader::is_open() const {
  return mode != FT_CLOSED;
}

void llreader::skip_whitespace() {
  while (pos < length) switch (data[pos]) {
    case '\r': {
      if (++pos < length && data[pos] == '\n') ++pos;
      ++lnum;
      lpos = pos;
      continue;
    }
    case '\n': {
      ++lnum;
      lpos = ++pos;
      continue;
    }
    case ' ': case '\t': case '\v': case '\f': {
      ++pos;
      continue;
    }
    // XXX: There are about a hundred other cases to handle here,
    // but unicode whitespace is currently forbidden in the standard.
    default: return;
  }
}
