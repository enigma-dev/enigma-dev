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

#pragma once
#include <string>

namespace ngs::sys {

std::string os_kernel_name();
std::string os_device_name();
std::string os_kernel_release();
std::string os_product_name();
std::string os_kernel_version();
std::string os_architecture();
std::string memory_totalram(bool human_readable);
std::string memory_freeram(bool human_readable);
std::string memory_usedram(bool human_readable);
std::string memory_totalswap(bool human_readable);
std::string memory_freeswap(bool human_readable);
std::string memory_usedswap(bool human_readable);
std::string memory_totalvram(bool human_readable);
std::string gpu_manufacturer();
std::string gpu_renderer();
std::string cpu_vendor();
std::string cpu_processor();
std::string cpu_processor_count();
std::string cpu_core_count();

} // namespace ngs::sys
