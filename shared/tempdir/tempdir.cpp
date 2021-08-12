#include <string>

#if defined(_WIN32)
#include <windows.h>
#include "../strings_util.h"
#else
#include <sys/stat.h>
#endif

#include "../crossprocess/crossprocess.h"

namespace enigma {

static std::string tempdir;
const char *DirectoryGetTemporaryPath() {
  #if defined(_WIN32)
  wchar_t tmp[MAX_PATH + 1];
  if (GetTempPathW(MAX_PATH + 1, tmp)) {
    tempdir = strings_util::shorten(tmp); 
    return tempdir.c_str();
  }
  return CrossProcess::DirectoryGetCurrentWorking();
  #else
  tempdir = CrossProcess::EnviromentGetVariable("TMPDIR"); 
  if (tempdir.empty()) tempdir = CrossProcess::EnviromentGetVariable("TMP");
  if (tempdir.empty()) tempdir = CrossProcess::EnviromentGetVariable("TEMP");
  if (tempdir.empty()) tempdir = CrossProcess::EnviromentGetVariable("TEMPDIR");
  if (tempdir.empty()) tempdir = "/tmp/";
  if (tempdir.back() != '/') tempdir += "/";
  struct stat sb = { 0 };
  if (stat(tempdir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
    return tempdir.c_str();
  }
  return CrossProcess::DirectoryGetCurrentWorking();
  #endif
}

} // namespace enigma

namespace enigma_user {

std::string get_temp_directory() { 
  return enigma::DirectoryGetTemporaryPath(); 
}

} // namespace enigma_user