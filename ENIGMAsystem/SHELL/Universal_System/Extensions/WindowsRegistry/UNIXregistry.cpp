/*

 MIT License
 
 Copyright © 2022 Samuel Venable
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
*/

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
