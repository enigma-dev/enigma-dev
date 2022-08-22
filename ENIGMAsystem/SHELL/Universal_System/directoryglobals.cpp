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

#include "Platforms/General/PFmain.h"

namespace enigma_user {

std::string get_working_directory() { 
  std::string result = ::ngs::cproc::directory_get_current_working();
  #if defined(_WIN32)
  return (!result.empty() && result.back() != '\\') ? result + "\\" : result;
  #else
  return (!result.empty() && result.back() != '/') ? result + "/" : result;
  #endif
}

std::string get_program_filename() { 
  return enigma_user::filename_name(::ngs::cproc::executable_from_self()); 
}

std::string get_program_directory() { 
  return enigma_user::filename_path(::ngs::cproc::executable_from_self()); 
}

std::string get_program_pathname() { 
  return ::ngs::cproc::executable_from_self(); 
}

bool set_working_directory(std::string dname) {
  return ::ngs::cproc::directory_set_current_working(dname.c_str());
}

} // namespace enigma_user
