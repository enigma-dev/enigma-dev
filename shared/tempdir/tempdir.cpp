#include <string>

#include "apifilesystem/filesystem.hpp"

namespace enigma_user {

std::string get_temp_directory() { 
  return ngs::fs::directory_get_temporary_path();
}

} // namespace enigma_user
