/*

 MIT License
 
 Copyright © 2023 Samuel Venable
 
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

namespace ngs::sys {

std::string human_readable(long double nbytes);
std::string utsname_sysname();
std::string utsname_nodename();
std::string utsname_release();
std::string utsname_codename();
std::string utsname_version();
std::string utsname_machine();
long long memory_totalram();
long long memory_availram();
long long memory_usedram();
long long memory_totalvmem();
long long memory_availvmem();
long long memory_usedvmem();
std::string gpu_vendor();
std::string gpu_renderer();
long long gpu_videomemory();
std::string cpu_vendor();
std::string cpu_brand();
int cpu_numcpus();
int cpu_numcores();

} // namespace ngs::sys
