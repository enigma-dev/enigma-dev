#include <string>

#include "Platforms/General/PFmain.h"

namespace enigma_user {

std::string get_working_directory() { 
  #if defined(_WIN32)
  return CrossProcess::DirectoryGetCurrentWorking() + std::string("\\");
  #else
  return CrossProcess::DirectoryGetCurrentWorking() + std::string("/");
  #endif
}

std::string get_program_filename() { 
  return enigma_user::filename_name(CrossProcess::ExecutableFromSelf()); 
}

std::string get_program_directory() { 
  return enigma_user::filename_path(CrossProcess::ExecutableFromSelf()); 
}

std::string get_program_pathname() { 
  return CrossProcess::ExecutableFromSelf(); 
}

std::string environment_get_variable(std::string name) {
  return CrossProcess::EnvironmentGetVariable(name.c_str());
}

bool environment_set_variable(const std::string &name, const std::string &value) {
  return CrossProcess::EnvironmentSetVariable(name.c_str(), value.c_str());
}

bool environment_unset_variable(const std::string &name) {
  return CrossProcess::EnvironmentUnsetVariable(name.c_str());
}

bool set_working_directory(std::string dname) {
  return CrossProcess::DirectorySetCurrentWorking(dname.c_str());
}

} // namespace enigma_user
