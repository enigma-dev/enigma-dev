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

#ifndef ENIGMA_PLATFORM_REGISTRY
#define ENIGMA_PLATFORM_REGISTRY

#include <string>

namespace enigma_user {

  bool registry_write_string(std::string name, std::string str);
  bool registry_write_real(std::string name, unsigned long val);
  std::string registry_read_string(std::string name);
  unsigned long registry_read_real(std::string name);
  bool registry_exists(std::string name);
  bool registry_write_string_ext(std::string subpath, std::string name, std::string str);
  bool registry_write_real_ext(std::string subpath, std::string name, unsigned long val);
  std::string registry_read_string_ext(std::string subpath, std::string name);
  unsigned long registry_read_real_ext(std::string subpath, std::string name);
  bool registry_exists_ext(std::string subpath, std::string name);
  std::string registry_get_path();
  bool registry_set_path(std::string subpath);
  std::string registry_get_key();
  bool registry_set_key(std::string keystr);

}

#endif //ENIGMA_PLATFORM_REGISTRY
