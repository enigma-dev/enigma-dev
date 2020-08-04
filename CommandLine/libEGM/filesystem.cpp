#include "filesystem.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>

bool StartsWith(const string &str, const string &prefix) {
  if (prefix.length() > str.length()) return false;
  return str.substr(0, prefix.length()) == prefix;
}

string StripPath(string fName) {
  size_t pos = fName.find_last_of('/');
  if (pos == string::npos)
    return fName;

  return fName.substr(pos+1, fName.length());
}

bool CreateDirectory(const fs::path &directory) {
  errc ec;
  if (fs::create_directory(directory, ec) || !ec) return true;
  std::cerr << "Failed to create directory " << directory << std::endl;
  return false;
}

bool CreateDirectoryRecursive(const fs::path &directory) {
  std::stack<fs::path> dirs;
  fs::path dir = directory;
  while (directory.has_parent_path() && !FolderExists(dir.parent_path())) {
    dir = dir.parent_path();
    dirs.push(dir);
  }
  
  while(!dirs.empty() && CreateDirectory(dirs.top())) {
    dirs.pop();
  }
    
  return !dirs.empty();
}

fs::path InternalizeFile(const fs::path &file,
                         const fs::path &directory, const fs::path &egm_root) {
  const fs::path data = "data";
  fs::path demistified = fs::canonical(fs::absolute(file));
  if (StartsWith(demistified.string(), egm_root.string())) {
    return fs::relative(demistified, directory);
  }
  if (!CreateDirectory(directory/data)) {
    std::cerr << "Failed to copy \"" << file
              << "\" into EGM: could not create output directory." << std::endl;
    return "";
  }
  fs::path relative = data/StripPath(file.string());

  if (!fs::copy_file(file, directory/relative)) {
    std::cerr << "Failed to copy \"" << file << "\" into EGM." << std::endl;
    return "";
  }

  return relative;
}

fs::path TempFileName(const string &pattern) {
  static int increment = 0;
  static std::string prefix = "";
  if (prefix.empty()) {
    // init random seed
    srand(time(NULL));
    // prefix 5 random digits
    for (size_t i = 0; i < 5; ++i)
      prefix += std::to_string(rand() % 10);
  }
  std::string name = prefix + pattern + std::to_string(increment++);
  return (fs::temp_directory_path().string() + '/' + name);
}

void DeleteFile(const string &fName) {
  std::remove(fName.c_str());
}

void DeleteFolder(const string &fName) {
  fs::remove_all(fName.c_str());
}

bool FolderExists(const string &folder) {
  const fs::path f = folder;
  return (fs::exists(f) && fs::is_directory(f));
}

bool FileExists(const string &fName) {
  const fs::path f = fName;
  return (fs::exists(f) && fs::is_regular_file(f));
}
