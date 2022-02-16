#include <string>

#include "Platforms/General/PFmain.h"

namespace enigma_user {

std::string get_working_directory() { 
  std::string result = ::ngs::proc::directory_get_current_working();
  #if defined(_WIN32)
  return (!result.empty() && result.back() != '\\') ? result + "\\" : result;
  #else
  return (!result.empty() && result.back() != '/') ? result + "/" : result;
  #endif
}

std::string get_program_filename() { 
  return enigma_user::filename_name(::ngs::proc::executable_from_self()); 
}

std::string get_program_directory() { 
  return enigma_user::filename_path(::ngs::proc::executable_from_self()); 
}

std::string get_program_pathname() { 
  return ::ngs::proc::executable_from_self(); 
}

bool set_working_directory(std::string dname) {
  return ::ngs::proc::directory_set_current_working(dname.c_str());
}

} // namespace enigma_user
