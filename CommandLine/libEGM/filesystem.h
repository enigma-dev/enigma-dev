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
