#ifdef USE_BOOST_FS
  #include <boost/filesystem.hpp>
  #include <boost/system/error_code.hpp>
  namespace fs = boost::filesystem;
  using errc = boost::system::error_code;
  namespace boost {
    template<class ForwardIt>
    ForwardIt next(
        ForwardIt it,
        typename std::iterator_traits<ForwardIt>::difference_type n = 1);
  }  // namespace boost
#else
  #ifdef __APPLE__
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
  #else
    #include <filesystem>
    namespace fs = std::filesystem;
  #endif
  using errc = std::error_code;
#endif

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
