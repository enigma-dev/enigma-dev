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

static inline string get_program_pathname_helper(size_t length) {
  string path;
  length++;
  int mib[4];
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = -1;
  char *buffer = path.data();
  size_t cb = length;
  if (sysctl(mib, 4, buffer, &cb, NULL, 0) == -1) {
    path = get_program_pathname_helper(length);
  } else if (sysctl(mib, 4, buffer, &cb, NULL, 0) == 0) {
    path = buffer;
  }
  return path;
}

namespace fileman {

  string get_program_pathname_ns(bool print) {
    string path;
    size_t length = 256;
    int mib[4];
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PATHNAME;
    mib[3] = -1;
    char *buffer = new char[length]();
    size_t cb = sizeof(buffer);
    if (sysctl(mib, 4, buffer, &cb, NULL, 0) == -1) {
      path = get_program_pathname_helper(length);
    } else if (sysctl(mib, 4, buffer, &cb, NULL, 0) == 0) {
      path = buffer;
    }
    path.shrink_to_fit();
    if (!path.empty()) {
      if (print) {
        cout << "program_pathname = \"" << path << "\"" << endl;
      }
    }
    delete[] buffer;
    return path;
  }

} // namespace fileman
