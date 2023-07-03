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

#include "Universal_System/Extensions/SystemInfo/system.hpp"

namespace enigma_user {

inline std::string human_readable(long long nbytes) {
  return ngs::sys::human_readable((long long)nbytes);
}

inline std::string utsname_sysname() {
  return ngs::sys::utsname_sysname();
}

inline std::string utsname_nodename() {
  return ngs::sys::utsname_nodename();
}

inline std::string utsname_release() {
  return ngs::sys::utsname_release();
}

inline std::string utsname_version() {
  return ngs::sys::utsname_version();
}

inline std::string utsname_codename() {
  return ngs::sys::utsname_codename();
}

inline std::string utsname_machine() {
  return ngs::sys::utsname_machine();
}

inline long long memory_totalram() {
  return ngs::sys::memory_totalram(); 
}

inline long long memory_availram() {
  return ngs::sys::memory_availram(); 
}

inline long long memory_usedram() {
  return ngs::sys::memory_usedram(); 
}

inline long long memory_totalvmem() {
  return ngs::sys::memory_totalvmem(); 
}

inline long long memory_availvmem() {
  return ngs::sys::memory_availvmem(); 
}

inline long long memory_usedvmem() {
  return ngs::sys::memory_usedvmem(); 
}
  
inline std::string gpu_vendor() {
  return ngs::sys::gpu_vendor();
}

inline std::string gpu_renderer() {
  return ngs::sys::gpu_renderer();
}

inline long long gpu_videomemory() {
  return ngs::sys::gpu_videomemory();
}

inline std::string cpu_vendor() {
  return ngs::sys::cpu_vendor();
}

inline std::string cpu_brand() {
  return ngs::sys::cpu_brand();
}

inline int cpu_numcores() {
  return ngs::sys::cpu_numcores();
}

inline int cpu_numcpus() {
  return ngs::sys::cpu_numcpus();
}

} // namespace enigma_user
