#ifndef EGM_FILESYS_H
#define EGM_FILESYS_H

#include <filesystem>
namespace fs = std::filesystem;
using errc = std::error_code;

using std::string;

bool StartsWith(const string &str, const string &prefix);
string StripPath(string fName);
fs::path TempFileName(const string &pattern);
bool CreateDirectoryRegular(const fs::path &directory);
bool CreateDirectoryRecursive(const fs::path &directory);
fs::path InternalizeFile(const fs::path &file,
                         const fs::path &directory, const fs::path &egm_root);
void DeleteFileRegular(const fs::path &fName);
void DeleteFolder(const fs::path &fName);
bool FolderExists(const string &folder);
inline bool FolderExists(const fs::path &path) {
  return FolderExists(path.string());
}
bool FileExists(const string &fName);
inline bool FileExists(const fs::path &path) {
  return FileExists(path.string());
}

#endif
