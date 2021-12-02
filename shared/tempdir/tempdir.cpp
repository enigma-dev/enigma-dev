#include <string>

#include "apiprocess/process.h"

namespace enigma_user {

std::string get_temp_directory() { 
  return ::ngs::proc::directory_get_temporary_path(); 
}

} // namespace enigma_user
