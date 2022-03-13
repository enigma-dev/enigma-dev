#include <string>

#include "Platforms/General/PFregistry.h"
#include "Platforms/platforms_mandatory.h"

using std::string;

namespace enigma_user {

  bool registry_write_string(string name, string str) {
    return false;
  }

  bool registry_write_real(string name, unsigned long val) {
    return false;
  }

  string registry_read_string(string name) {
    return "";
  }

  unsigned long registry_read_real(string name) {
    return 0;
  }

  bool registry_exists(string name) {
    return false;
  }

  bool registry_write_string_ext(string subpath, string name, string str) {
    return false;
  }

  bool registry_write_real_ext(string subpath, string name, unsigned long val) {
	return false;
  }

  string registry_read_string_ext(string subpath, string name) {
    return "";
  }

  unsigned long registry_read_real_ext(string subpath, string name) {
    return 0;
  }

  bool registry_exists_ext(string subpath, string name) {
    return false;
  }
  
  string registry_get_path() {
    return "";
  }
 
  bool registry_set_path(string subpath) {
    return false;
  }
  
  string registry_get_key() {
    return "";
  }

  bool registry_set_key(string keystr) {
    return false;
  }
  
} // namespace enigma_user
