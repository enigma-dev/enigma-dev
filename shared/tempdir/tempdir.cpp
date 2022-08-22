#include <string>

#include "lib/cproc/cproc.hpp"

namespace enigma_user {

std::string get_temp_directory() { 
  std::string result = ::ngs::cproc::directory_get_temporary_path();
  #if defined(_WIN32)
  if (!result.empty() && result.back() != '/' && result.back() != '\\') result.push_back('\\');
  #else
  if (!result.empty() && result.back() != '/') result.push_back('/');
  #endif
  return result;
}

} // namespace enigma_user
