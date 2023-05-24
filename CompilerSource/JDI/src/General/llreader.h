/**
 * @file  llreader.h
 * @brief Header declaring a general-purpose array-to-file adapter.
 *
 * This file is meant to provide a faster alternative to the standard C++
 * stream classes. It can work directly with files and strings alike.
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

/**
  @brief A low-level file reader structure for quick I/O.

  This structure guarantees that its data member represents
  the file as a string in memory, and that its length member
  gives the valid length of the array member. The implementation
  may in fact virtually map the file in memory or read the
  whole thing in at once.

  This class may also represent a simple string in memory
  if one is provided in place of a file.
**/

#ifndef _LLREADER__H
#define _LLREADER__H

#include <string>
#include <filesystem>

class llreader {
 public:
  size_t pos; ///< Our position in the stream
  size_t length; ///< The length of the stream
  size_t lpos; ///< The position of the most-recently-read newline
  size_t lnum; ///< The number of newlines that have been read
  const char *data; ///< The data in the stream
  std::string name; ///< The name or source of this file.

  static constexpr int kFirstLine = 1;

# ifndef NOVALIDATE_LINE_NUMBERS
  size_t validated_pos = 0;
  size_t validated_lpos = 0;
  size_t validated_lnum = kFirstLine;
# endif

  const std::string &get_filename() const { return name; }
  size_t get_line_number() const { return lnum; }
  size_t get_line_position() const { return pos - lpos; }

 private:
  int mode; ///< What kind of stream we have open

 public:
  /**
    Open a file as an array.
    Length will reflect the length of the file, and
    data will mirror its contents.
    @param filename  The name of the file that will be opened.
  **/
  void open(const std::filesystem::path &filename);
  /**
    Encapsulate a string for the duration of its life.

    Length will be set to code.length(), and data will be
    set to its c_str().

    @param contents  A view of the contents to be pointed to.
    @warning As this function only encapsulates the string,
             without copying it; its contents will become
             invalid once the encapsulated string leaves scope.
  **/
  void encapsulate(std::string_view contents);
  /**
    Consume a const char*, taking responsibility for its deallocation.
    @param buffer  The buffer to consume. You should discard this pointer afterward.
    @param length  The length of the data pointed to by buffer.
  **/
  void consume(char* buffer, size_t length);
  /**
    Consumes another \c llreader, stealing its contents then settings its
    mode to closed (while keeping the stolen one open).
  **/
  void consume(llreader& whom);
  /**
    Alias the file buffer to a given const char* buffer.
    @param buffer  The buffer to alias. You are still the owner, and must free it after this class is done with it.
    @param length  The length of the data pointed to by buffer.
    @warning  This class will NOT take responsibility for keeping and freeing the memory pointed to by \p buffer.
              If you do not free the memory, it will be leaked. If you free the memory too soon, an access violation can occur.
  **/
  void alias(const char* buffer, size_t length);
  /**
    Alias the file buffer to a given const char* buffer.
    @param llread  The llreader to alias.
    @warning  Make sure the llreader passed to this function does not leave scope or get freed before this one does,
              or access violations will still occur.
  **/
  void alias(const llreader& llread);
  /**
    Copies a string into a new buffer and uses it as the data.
    Length will be set to code.length(), and data will be set
    to a buffer containing a copy of the viewing range of its data().
    @param contents  A string containing contents to be mirrored.
  **/
  void copy(std::string_view contents);
  /**
    Close the open stream.
    This function will take care of any necessary memory frees
    or file close operations. It is called automatically by the
    destructor.
  **/
  void close();

  // Copying this reader can be expensive.
  llreader &operator=(const llreader &llr) = delete;
  llreader &operator=(llreader &&llr);

  /**
    Returns whether the stream is open.
  **/
  bool is_open() const;

  // =========================================================================
  // == Help with tokenization ===============================================
  // =========================================================================

  /// Skips whitespace from the current position, keeping track of newlines.
  void skip_whitespace();

  char operator[](size_t ind) const { return data[ind]; }
  const char* operator+(size_t x) { return data + x; }
  size_t tell() const { return pos; }
  char at() const { return data[pos]; }
  int at(size_t i) const { return i < length ? data[i] : EOF; }
  char getc() { return data[pos++]; }
  int next() { return ++pos < length ? data[pos] : EOF; }
  int peek_next() { return pos + 1 < length ? data[pos + 1] : EOF; }
  bool eof() const { return pos >= length; }
  bool advance() { return ++pos < length; }
  bool smart_advance() { return take_newline() || advance(); }
  bool skip(size_t count) { return (pos += count) < length; }
  bool at_newline() const { return data[pos] == '\n' || data[pos] == '\r'; }

  bool take_newline() {
    if (data[pos] == '\n') ++pos;
    else {
      if (data[pos] != '\r') return false;
      if (data[++pos] == '\n') ++pos;
    }
    lnum++;
    lpos = pos;
    return true;
  }

  bool take(char c) {
    if (data[pos] != c) return false;
    ++pos;
    return true;
  }
  template<int n> bool take(const char (&str)[n]) {  // fun rolls
    for (int i = 0; i < n - 1; ++i) if (data[pos + i] != str[i]) return false;
    pos += n - 1;
    return true;
  }

  std::string_view slice(size_t start) const { return slice(start, pos); }
  std::string_view slice(size_t start, size_t end) const {
    return std::string_view(data + start, end - start);
  }

  // =========================================================================
  // == Constructors/destructors =============================================
  // =========================================================================

  /**
    Default constructor.
    There's really no good way to make a constructor for each method above.
  **/
  llreader();
  /**
    The (const char*) constructor behaves like open().
    This is because knowing the length of the const char* won't help, anyway.
    @param filename  The name of the file that will be opened.
  **/
  llreader(const std::filesystem::path &filename);
  /**
    Constructs, behaving like either copy() or encapsulate().
    @param contents  A string containing contents to be mirrored.
    @param copy      True if the contents are to be copied, false if they are
                     to simply be pointed to (see \c encapsulate).
  **/
  llreader(std::string name, std::string_view contents, bool copy);
  /// Construct by copying a std::string.
  llreader(std::string name, std::string contents);
  llreader(const llreader&) = delete;
  llreader(llreader&&);
  /** Default destructor. Closes open stream. */
  ~llreader();
};

#endif
