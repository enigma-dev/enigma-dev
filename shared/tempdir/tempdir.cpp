#include <string>

#if defined(_WIN32)
#include <windows.h>
#include "../strings_util.h"
#else
#include <sys/stat.h>
#endif

#include "../apiprocess/process.h"

namespace enigma {

const char *DirectoryGetTemporaryPath() {
  return ngs::proc::directory_get_temporary_path(); 
}

} // namespace enigma

namespace enigma_user {

std::string get_temp_directory() { 
  return enigma::DirectoryGetTemporaryPath(); 
}

} // namespace enigma_user
