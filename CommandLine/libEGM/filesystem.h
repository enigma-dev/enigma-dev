#ifndef EGM_FILESYSTEM_H
#define EGM_FILESYSTEM_H

#include <filesystem>

namespace fs = std::filesystem;
using errc = std::error_code;

using std::string;

bool StartsWith(const string &str, const string &prefix);
string StripPath(string fName);
string TempFileName(const string &pattern);
bool CreateDirectory(const fs::path &directory);
fs::path InternalizeFile(const fs::path &file,
                         const fs::path &directory, const fs::path &egm_root);
void DeleteFile(const string &fName);
void DeleteFolder(const string &fName);
bool FolderExists(const string &folder);
inline bool FolderExists(const fs::path &path) {
  return FolderExists(path.string());
}
bool FileExists(const string &fName);
inline bool FileExists(const fs::path &path) {
  return FileExists(path.string());
}

std::string FileToString(const std::string &fName);
inline std::string FileToString(const fs::path &path) {
  return FileToString(path.string());
}

#endif
