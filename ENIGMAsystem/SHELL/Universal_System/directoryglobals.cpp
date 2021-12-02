#include <string>

#include "Platforms/General/PFmain.h"

namespace enigma_user {

std::string get_working_directory() { 
  #if defined(_WIN32)
  return ngs::proc::directory_get_current_working() + std::string("\\");
  #else
  return ngs::proc::directory_get_current_working() + std::string("/");
  #endif
}

std::string get_program_filename() { 
  return enigma_user::filename_name(ngs::proc::executable_from_self()); 
}

std::string get_program_directory() { 
  return enigma_user::filename_path(ngs::proc::executable_from_self()); 
}

std::string get_program_pathname() { 
  return ngs::proc::executable_from_self(); 
}

std::string environment_get_variable(std::string name) {
  return ngs::proc::environment_get_variable(name.c_str());
}

bool environment_set_variable(const std::string &name, const std::string &value) {
  return ngs::proc::environment_set_variable(name.c_str(), value.c_str());
}

bool environment_unset_variable(const std::string &name) {
  return ngs::proc::environment_unset_variable(name.c_str());
}

bool set_working_directory(std::string dname) {
  return ngs::proc::directory_set_current_working(dname.c_str());
}

} // namespace enigma_user
