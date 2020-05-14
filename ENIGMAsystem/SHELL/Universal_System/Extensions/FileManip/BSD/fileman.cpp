/*

 MIT License
 
 Copyright © 2020 Samuel Venable
 
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

#include "../fileman.h"
#include <sys/types.h>
#include <sys/sysctl.h>
#include <iostream>
#include <cstddef>

using std::string;
using std::size_t;
using std::cout;
using std::endl;

static const size_t PATH_SIZE = 256;

static inline string get_program_pathname_helper(string path, size_t length) {
  int mib[4];
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = -1;
  path.resize(length, '\0');
  char *buffer = path.data();
  size_t cb = length;
  int result = sysctl(mib, 4, buffer, &cb, NULL, 0);
  if (result == -1) {
    length += PATH_SIZE;
    path = get_program_pathname_helper(path, length);
  } else if (result == 0) {
    path = string(buffer) + "\0";
    path.shrink_to_fit();
  }
  return path;
}

namespace fileman {

  string get_program_pathname_ns(bool print) {
    string path = get_program_pathname_helper("", PATH_SIZE);
    if (!path.empty()) {
      if (print) {
        cout << "program_pathname = \"" << path << "\"" << endl;
      }
    }
    return path;
  }

} // namespace fileman
