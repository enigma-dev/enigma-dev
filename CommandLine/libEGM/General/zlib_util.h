/** Copyright (C) 2018-2020 Robert B. Colton, Greg Williamson
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

#ifndef ZLIB_UTIL_H
#define ZLIB_UTIL_H

#include "filesystem.h"
#include "file-format.h"

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <future>

class Decoder {
public:
  explicit Decoder(std::istream &in);
  template<class Function, class... Args>
  void threadTempFileWrite(Function&& f, const std::filesystem::path& write_fname, Args&&... args);
  void processTempFileFutures();
  void beginInflate();
  void endInflate();
  void skip(size_t count);
  int read();
  int read2();
  int read3();
  int read4();
  bool readBool();
  void read(char* bytes, size_t length, size_t off=0);
  std::unique_ptr<char[]> read(size_t length, size_t off=0);
  std::string readStr();
  double readD();
  std::unique_ptr<char[]> decompress(size_t &length, size_t off=0);
  void readData(const std::filesystem::path& write_fname, bool compressed=false);
  void readCompressedData(const std::filesystem::path& write_fname);
  void readUncompressedData(const std::filesystem::path& write_fname);
  void readZlibImage(const std::filesystem::path& write_fname, bool transparent);
  void readZlibImage(bool transparent=false);
  void readBGRAImage(const std::filesystem::path& write_fname = TempFileName("gmk_data"), size_t width=0, size_t height=0);
  void setSeed(int seed);
  void postponeName(std::string* name, int id, buffers::TreeNode::TypeCase type);
  void processPostoned(const std::string name, const int id, const buffers::TreeNode::TypeCase type);

private:
  std::istream &in;
  std::unique_ptr<char[]> zlibBuffer;
  int zlibPos, zlibStart;
  std::unique_ptr<int[]> decodeTable;
  std::unordered_map<buffers::TreeNode::TypeCase, std::unordered_map<int, std::vector<std::string*> > > postponeds;
  std::vector<std::pair<std::future<void>, std::filesystem::path> > tempFileFuturesCreated;

  std::unique_ptr<int[]> makeEncodeTable(int seed);
  std::unique_ptr<int[]> makeDecodeTable(std::unique_ptr<int[]> encodeTable);
  std::unique_ptr<int[]> makeDecodeTable(int seed);
};

#endif // ZLIB_UTIL_H
